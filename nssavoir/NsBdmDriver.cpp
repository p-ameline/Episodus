//---------------------------------------------------------------------------
//
//  		                        BDM query objects
//
//---------------------------------------------------------------------------

#include <stdlib.h>

#include "nssavoir/NsBdmDriver.h"
#include "nssavoir/nsPathor.h"
#include "nautilus/nssuper.h"
#include "nssavoir/NsConver.h"
#include "nsoutil/ibdm.h"
#include "curl/nsRest.h"
#include "nsutil/tinyxml.h"
#include "nsldv/NsLdvDrug.h"
#include "nsepisod/nsCsvParser.h"

//***************************************************************************
// 							            NSBdmDriver methods
//***************************************************************************

//---------------------------------------------------------------------------
//   Constructor
//---------------------------------------------------------------------------
NSBdmDriver::NSBdmDriver(NSContexte* pCtx)
            :NSRoot(pCtx)
{
  _pBdmDialog = (ChoixBdmDialog*) 0 ;
  _pRest      = (Rest*) 0 ;
  _isBusy     = false ;

  _sBasicPathForDocs = string("https://www.vidal.fr/") ;

  if (((NSContexte*) NULL == pContexte) || (pContexte->getBamType() == NSContexte::btNone))
    return ;

  _pRest = new Rest() ;
}

//---------------------------------------------------------------------------
//   Destructor
//---------------------------------------------------------------------------
NSBdmDriver::~NSBdmDriver()
{
  if (_pRest)
    delete _pRest ;
}

/**
 * Inject user's IDs in vars
 */
void
NSBdmDriver::initVars(vector<Var>* paVars)
{
  if ((vector<Var>*) NULL == paVars)
    return ;

  paVars->clear() ;

  paVars->push_back(Var(string("app_id"),  pContexte->getBamApplicationID())) ;
  paVars->push_back(Var(string("app_key"), pContexte->getBamApplicationKey())) ;
}

/**
 * Get drug information from the CIS code
 */
bool
NSBdmDriver::getDrugInfoFromCisCode(const string sCisCode, NsSelectableDrug* pFoundDrug)
{
  if ((string("") == sCisCode) || ((NsSelectableDrug*) NULL == pFoundDrug))
    return false ;

  vector<Var> aSpecificVars ;
  aSpecificVars.push_back(Var(string("code"),   sCisCode)) ;
  aSpecificVars.push_back(Var(string("filter"), string("PRODUCT"))) ;

  NsSelectableDrugArray aDrugsList ;

  string sSeed = string("") ;

  getDrugsForSeed(&aDrugsList, &sSeed, string("search"), &aSpecificVars) ;

  if (aDrugsList.size() != 1)
    return false ;

  *pFoundDrug = *(aDrugsList[0]) ;

  return true ;
}

/**
 * Get the BAM Identifier from the CIS code
 */
string
NSBdmDriver::getBamIdFromCisCode(const string sCisCode)
{
  if (string("") == sCisCode)
    return string("") ;

  NsSelectableDrug drug ;
  if (false == getDrugInfoFromCisCode(sCisCode, &drug))
    return string("") ;

  return drug.getBdmID() ;
}

/**
 * Get the ATC code for a given product (from its BAM Identifier)
 */
string
NSBdmDriver::getAtcCodeFromBamId(const string sBamId)
{
  if ((string("") == sBamId) || ((Rest*) NULL == _pRest))
    return string("") ;

  NSBdmEntryArray aListeArray ;

  // Preparing request
  //
  vector<Var> vars ;
  initVars(&vars) ;

  string sRequest = string("product/") + sBamId + string("/atc-classification") ;

  bool bExistNextPage = true ;
  while (bExistNextPage)
  {
    // Executing request
    //
    string sMessage = string("Calling BDM with a GET on query: ") + sRequest ;
    pContexte->getSuperviseur()->trace(&sMessage, 1, NSSuper::trDetails) ;

    string sResult = _pRest->request(sRequest, string("GET"), vars) ;

    if (string("") == sResult)
      return string("") ;

    // Make certain that it doesn't turn infinitely with the same request
    //
    string sPreviousRequest = sRequest ;

    // Parse result and returns the href of link with rel="next"
    //
    sRequest = fillArrayOfBdmEntriesFromResults(&aListeArray, &sResult, string("vidal:id"), string("vidal:code"), string("vidal:name")) ;

    if ((string("") == sRequest) || (sPreviousRequest == sRequest))
      bExistNextPage = false ;
    else
      initVars(&vars) ;
  }

  if (aListeArray.empty())
    return string("") ;

  // Get the ATC code with max length
  //
  string sAtcCode  = string("") ;
  size_t iCodeSize = 0 ;

  for (NSBdmEntryIter it = aListeArray.begin() ; aListeArray.end() != it ; it++)
  {
    size_t iKeySize = strlen((*it)->getCode().c_str()) ;
    if (iKeySize > iCodeSize)
    {
      sAtcCode  = (*it)->getCode() ;
      iCodeSize = iKeySize ;
    }
  }

  return sAtcCode ;
}

/**
 * Get the BAM Identifier for an ATC code
 */
string
NSBdmDriver::getBamIdForAtcCode(const string sAtcCode)
{
  if ((string("") == sAtcCode) || ((Rest*) NULL == _pRest))
    return string("") ;

  NSBdmEntryArray aListeArray ;

  // Preparing request
  //
  vector<Var> vars ;
  initVars(&vars) ;

  vars.push_back(Var(string("code"),   sAtcCode)) ;
  vars.push_back(Var(string("filter"), string("atc_classification"))) ;

  string sRequest = string("search") ;

  bool bExistNextPage = true ;
  while (bExistNextPage)
  {
    // Executing request
    //
    string sMessage = string("Calling BDM with a GET on query: ") + sRequest ;
    pContexte->getSuperviseur()->trace(&sMessage, 1, NSSuper::trDetails) ;

    string sResult = _pRest->request(sRequest, string("GET"), vars) ;

    if (string("") == sResult)
      return string("") ;

    // Make certain that it doesn't turn infinitely with the same request
    //
    string sPreviousRequest = sRequest ;

    // Parse result and returns the href of link with rel="next"
    //
    sRequest = fillArrayOfBdmEntriesFromResults(&aListeArray, &sResult, string("vidal:id"), string("vidal:code"), string("vidal:name"), string(""), string(""), string("")) ;

    if ((string("") == sRequest) || (sPreviousRequest == sRequest))
      bExistNextPage = false ;
    else
      initVars(&vars) ;
  }

  if (aListeArray.empty())
    return string("") ;

  // Get the first answer
  //
  for (NSBdmEntryIter it = aListeArray.begin() ; aListeArray.end() != it ; it++)
    if ((*it)->getCode() == sAtcCode)
      return (*it)->getID() ;

  return string("") ;
}

/**
 * Get the ATC code for a given product (from its BAM Identifier)
 */
string
NSBdmDriver::getMedicaBaseCodeFromBamVmpId(const string sBamVmpId)
{
  if ((string("") == sBamVmpId) || ((Rest*) NULL == _pRest))
    return string("") ;

  NSBdmEntryArray aListeArray ;

  // Preparing request
  //
  vector<Var> vars ;
  initVars(&vars) ;

  string sRequest = string("vmp/") + sBamVmpId ;

  bool bExistNextPage = true ;
  while (bExistNextPage)
  {
    // Executing request
    //
    string sMessage = string("Calling BDM with a GET on query: ") + sRequest ;
    pContexte->getSuperviseur()->trace(&sMessage, 1, NSSuper::trDetails) ;

    string sResult = _pRest->request(sRequest, string("GET"), vars) ;

    if (string("") == sResult)
      return string("") ;

    // Make certain that it doesn't turn infinitely with the same request
    //
    string sPreviousRequest = sRequest ;

    // Parse result and returns the href of link with rel="next"
    //
    sRequest = fillArrayOfBdmEntriesFromResults(&aListeArray, &sResult, string("vidal:id"), string("vidal:medicaBaseName"), string("vidal:medicaBaseName"), string(""), string("medicaBaseCode"), string("")) ;

    if ((string("") == sRequest) || (sPreviousRequest == sRequest))
      bExistNextPage = false ;
    else
      initVars(&vars) ;
  }

  if (aListeArray.empty())
    return string("") ;

  // Get the first answer
  //
  for (NSBdmEntryIter it = aListeArray.begin() ; aListeArray.end() != it ; it++)
    if ((*it)->getID() == sBamVmpId)
      return (*it)->getCode() ;

  return string("") ;
}

void
NSBdmDriver::getBlocksForProduct(const string sCisCode, NsXmlBlocksArray* pBlocks)
{
  if ((NsXmlBlocksArray*) NULL == pBlocks)
    return ;
}

void
NSBdmDriver::getBlocksFromUrl(const string sUrl, NsXmlBlocksArray* pBlocks)
{
  if (((NsXmlBlocksArray*) NULL == pBlocks) || (string("") == sUrl) || ((Rest*) NULL == _pRest))
    return ;

  // Preparing request
  //
  string sRequest = sUrl ;

  vector<Var> vars ;
  initVars(&vars) ;

  bool bExistNextPage = true ;
  while (bExistNextPage)
  {
    // Executing request
    //
    string sMessage = string("getBlocksFromUrl: Calling BDM with a GET on query: ") + sRequest ;
    pContexte->getSuperviseur()->trace(&sMessage, 1, NSSuper::trDetails) ;

    string sResult = _pRest->request(sRequest, string("GET"), vars) ;

    if (string("") == sResult)
      return ;

    sRequest = addBlocksFromResults(pBlocks, &sResult) ;

    if (string("") == sRequest)
      bExistNextPage = false ;
    else
      initVars(&vars) ;
  }
}

string
NSBdmDriver::addBlocksFromResults(NsXmlBlocksArray* pBlocks, const string* psResult)
{
  if (((NsXmlBlocksArray*) NULL == pBlocks) || ((string*) NULL == psResult))
    return string("") ;

  if (string("") == *psResult)
    return string("") ;

  // Processing results
  //
  TiXmlDocument docForSearch ;
  docForSearch.Parse(psResult->c_str()) ;

  TiXmlHandle docHandle(&docForSearch) ;

  // Each block is inside an "entry" tag
  //
  TiXmlElement* pEntryElement = docHandle.FirstChild("feed").FirstChild("entry").Element() ;
  while (pEntryElement)
  {
    NsXmlBlock XmlBlock ;

    // Get title
    //
    XmlBlock.setTitle(getTextForTag(pEntryElement, string("title"))) ;
    XmlBlock.setUniqueId(getTextForTag(pEntryElement, string("id"))) ;

    // Get links
    //
    TiXmlNode* pIdNode = pEntryElement->FirstChild("link") ;
    if (pIdNode)
    {
      TiXmlElement* pLinkAsElement = pIdNode->ToElement() ;
      while (pLinkAsElement)
      {
        string sTitle = string("") ;
        string sType  = string("") ;
        string sHref  = string("") ;
        string sRel   = string("") ;

        // Get attributes
        //
        TiXmlAttribute* pAttribute = pLinkAsElement->FirstAttribute() ;
        while (pAttribute)
        {
          string sName = pAttribute->Name() ;

          if      (string("rel") == sName)
            sRel  = pAttribute->Value() ;
          else if (string("href") == sName)
            sHref  = pAttribute->Value() ;
          else if (string("type") == sName)
            sType  = pAttribute->Value() ;
          else if (string("title") == sName)
            sTitle = pAttribute->Value() ;

          pAttribute = pAttribute->Next() ;
        }

        NsHtmlLink newLink(sTitle, sHref, sType, sRel) ;

        XmlBlock.addLink(newLink) ;

        pLinkAsElement = pLinkAsElement->NextSiblingElement("link") ;
      }
    }

    // Get information, it is located after the <content/> tag
    //
    TiXmlNode* pContentNode = pEntryElement->FirstChild("content") ;
    if (pContentNode)
    {
      TiXmlElement* pContentElement = pContentNode->ToElement() ;
      if (pContentElement)
        pContentElement = pContentElement->NextSiblingElement() ;
      while (pContentElement)
      {
        // Get tag name and text
        //
        string sTagName = string(pContentElement->Value()) ;
        string sText    = getTextForNode(pContentElement) ;

        NsXmlEntry entry(GetCleanTag(sTagName), sText) ;

        // Get attributes
        //
        TiXmlAttribute* pAttribute = pContentElement->FirstAttribute() ;
        while (pAttribute)
        {
          entry.addAttribute(pAttribute->Name(), pAttribute->Value()) ;

          pAttribute = pAttribute->Next() ;
        }

        XmlBlock.addEntry(entry) ;

        pContentElement = pContentElement->NextSiblingElement() ;
      }
    }

    pBlocks->push_back(new NsXmlBlock(XmlBlock)) ;

    pEntryElement = pEntryElement->NextSiblingElement() ;
  }

  // Try to find a link to next page
  //
  TiXmlHandle docLinkHandle(&docForSearch) ;
  TiXmlElement* pLinkElement = docLinkHandle.FirstChild("feed").FirstChild("link").Element() ;
  while (pLinkElement)
  {
    string sRelAttribute = pLinkElement->Attribute(string("rel")) ;
    if (string("next") == sRelAttribute)
      return pLinkElement->Attribute(string("href")) ;

    pLinkElement = pLinkElement->NextSiblingElement("link") ;
  }

  return string("") ;
}

bool
NSBdmDriver::getDrugInformation(NsSelectableDrug* pDrugInformation, string* pCisCode)
{
  if (((NsSelectableDrug*) NULL == pDrugInformation) || ((string*) NULL == pCisCode))
    return false ;

  vector<Var> aSpecificVars ;
  aSpecificVars.push_back(Var(string("code"),   *pCisCode)) ;
  aSpecificVars.push_back(Var(string("filter"), string("PRODUCT"))) ;

  NsSelectableDrugArray aDrugsList ;

  string sSeed = string("") ;

  getDrugsForSeed(&aDrugsList, &sSeed, string("search"), &aSpecificVars) ;

  if (aDrugsList.size() == 0)
    return false ;

  // Prevent prescribed node id from reset
  //
  string sPrescribedNodeId = pDrugInformation->getPrescribedNodeId() ;

  // In case there are several co-marketed drugs with an identical CIS code,
  // then we must select the original one (which name doesn't contain any '[')
  //
  if (aDrugsList.size() > 1)
  {
    bool bFound = false ;
    for (NsSelectableDrugIter dIt = aDrugsList.begin() ; aDrugsList.end() != dIt ; dIt++)
      if (NPOS == (*dIt)->getLabel().find("["))
      {
        *pDrugInformation = **dIt ;
        bFound = true ;
      }

    if (false == bFound)
      return false ;
  }
  else
    *pDrugInformation = *(aDrugsList[0]) ;

  pDrugInformation->setPrescribedNodeId(sPrescribedNodeId) ;

  getSecurityIndicators(pDrugInformation) ;

  return true ;
}

/**
 * Get the complete list of Medicabase IDs and labels
 */
bool
NSBdmDriver::getMedicabaseContent(NSBdmEntryArray* paListeArray)
{
  if (((NSBdmEntryArray*) NULL == paListeArray) || ((Rest*) NULL == _pRest))
    return false ;

  // Preparing request
  //
  vector<Var> vars ;
  initVars(&vars) ;

  string sRequest = string("vmps") ;

  bool bExistNextPage = true ;
  while (bExistNextPage)
  {
    // Executing request
    //
    string sMessage = string("Calling BDM with a GET on query: ") + sRequest ;
    pContexte->getSuperviseur()->trace(&sMessage, 1, NSSuper::trDetails) ;

    string sResult = _pRest->request(sRequest, string("GET"), vars) ;

    if (string("") == sResult)
      return true ;

    // Make certain that it doesn't turn infinitely with the same request
    //
    string sPreviousRequest = sRequest ;

    // Parse result and returns the href of link with rel="next"
    //
    sRequest = fillArrayOfBdmEntriesFromResults(paListeArray, &sResult, string("vidal:medicaBaseName"), string("vidal:medicaBaseName"), string("medicaBaseCode"), string("")) ;

    if ((string("") == sRequest) || (sPreviousRequest == sRequest))
      bExistNextPage = false ;
    else
      initVars(&vars) ;
  }

  return true ;
}

/**
 * Get the virtual drug Lexicon code from a speciality Lexicon code
 */
string
NSBdmDriver::getVirtualDrug(const string sSpecialityCode)
{
  if (string("") == sSpecialityCode)
    return string("") ;

  InterfaceBdm bdm(pContexte) ;

  // First, find the CIS code for this Lexicon code
  //
  string sCodeSens = pContexte->getDico()->donneCodeSens(&sSpecialityCode) ;
  if (false == bdm.isCodeLexiMedInDB(sCodeSens))
    return string("") ;

  string sCisCode = bdm.pBdm->getCodeCIP() ;
  if (string("") == sCisCode)
    return string("") ;

  // Get the virtual code for the specialty
  //
  string sMedicabaseCode = getMedicabaseFromCis(sCisCode) ;
  if (string("") == sMedicabaseCode)
    return string("") ;

  // Get the Lexicon code for the vistual drug
  //
  if (false == bdm.isCodegeneriqGrpeInDB(sMedicabaseCode))
    return string("") ;

  string sLeximedCode = bdm.pBdm->getLeximed() ;
  NSDico::donneCodeComplet(sLeximedCode) ;

  return sLeximedCode ;
}

NSBdmEntry*
NSBdmDriver::getUnitForId(string &sUnitBamId)
{
  if (_aUnitsArray.empty())
    return (NSBdmEntry*) 0 ;

  for (NSBdmEntryIter it = _aUnitsArray.begin() ; _aUnitsArray.end() != it ; it++)
    if ((*it)->getID() == sUnitBamId)
      return *it ;

  return (NSBdmEntry*) 0 ;
}

/**
 * Get the virtual drug Medicabase code from a CIS code
 */
string
NSBdmDriver::getMedicabaseFromCis(const string sCisCode)
{
  if (string("") == sCisCode)
    return string("") ;

  // Get Bam vmp Id
  //
  NsSelectableDrug drug ;
  if (false == getDrugInfoFromCisCode(sCisCode, &drug))
    return string("") ;

  // Get MedicaBase Code from Bam Vmp Id
  //
  return getMedicaBaseCodeFromBamVmpId(drug.getVirtualDrugBdmId()) ;
}

/**
 * Fills a list from the content of a table that fits a given seed (free text entry)
 */
void
NSBdmDriver::getListForSeed(NSBdmEntryArray* paListeArray, string* pSeed, BAMTABLETYPE iTableType)
{
  if (((NSBdmEntryArray*) NULL == paListeArray) || ((string*) NULL == pSeed))
    return ;

  paListeArray->vider() ;

  while (_isBusy)
    pContexte->getSuperviseur()->getApplication()->PumpWaitingMessages() ;

  _isBusy = true ;

  switch(iTableType)
  {
    case bamTableIndication :
      getIndicationsForSeed(paListeArray, pSeed) ;
      break ;
    case bamTableSubstance :
      getSubstancesForSeed(paListeArray, pSeed) ;
      break ;
    case bamTableATC :
      getAtcCodesForSeed(paListeArray, pSeed) ;
      break ;
    case bamTableCim10 :
      getCim10CodesForSeed(paListeArray, pSeed) ;
      break ;
    case bamTableAllergies :
      getAllergiesForSeed(paListeArray, pSeed) ;
      break ;
    case bamTableMolecules :
      getMoleculesForSeed(paListeArray, pSeed) ;
      break ;
  }

  _isBusy = false ;
}

/**
 * Get the indications (bam Id and label) for a given free text entry
 */
void
NSBdmDriver::getIndicationsForSeed(NSBdmEntryArray* paListeArray, string* pSeed)
{
  getElementsForSeed(paListeArray, pSeed, string("indications"),
                                          string("vidal:id"),
                                          string(""),
                                          string("vidal:name"),
                                          (vector<Var>*) 0) ;
}

/**
 * Get the ICD10 codes (bam Id, code and label) for a given free text entry
 */
void
NSBdmDriver::getCim10CodesForSeed(NSBdmEntryArray* paListeArray, string* pSeed)
{
  getElementsForSeed(paListeArray, pSeed, string("pathologies"),
                                          string("vidal:id"),
                                          string("vidal:code"),
                                          string("vidal:name"),
                                          (vector<Var>*) 0) ;
}

/**
 * Get the active substances codes (bam Id and label) for a given free text entry
 */
void
NSBdmDriver::getSubstancesForSeed(NSBdmEntryArray* paListeArray, string* pSeed)
{
  getElementsForSeed(paListeArray, pSeed, string("molecules/active-substances"),
                                          string("vidal:id"),
                                          string(""),
                                          string("vidal:fullName"),
                                          (vector<Var>*) 0) ;
}

/**
 * Get the ATC codes (bam Id, code and label) for a given free text entry
 */
void
NSBdmDriver::getAtcCodesForSeed(NSBdmEntryArray* paListeArray, string* pSeed)
{
  if (((NSBdmEntryArray*) NULL == paListeArray) || ((string*) NULL == pSeed))
    return ;

  getElementsForSeed(paListeArray, pSeed, string("atc-classifications"),
                                          string("vidal:id"),  // string("id"),
                                          string("vidal:code"),
                                          string("title"),
                                          (vector<Var>*) 0) ;

/*
  vector<Var> aSpecificVars ;
  aSpecificVars.push_back(Var(string("filter"), string("atc_classification"))) ;

  getElementsForSeed(paListeArray, pSeed, string("search"),
                                          string("vidal:code"),  // string("id"),
                                          string("title"),
                                          &aSpecificVars) ;

  // Need to remove the "vidal://atc_classification/" in front of each code
  //
  if (paListeArray->empty())
    return ;

  string sToRemove = string("vidal://atc_classification/") ;
  size_t iSize     = strlen(sToRemove.c_str()) ;

  for (NSBdmEntryIter it = paListeArray->begin() ; paListeArray->end() != it ; it++)
  {
    string sKey = (*it)->getID() ;
    if ((strlen(sKey.c_str()) > iSize) && (string(sKey, 0, iSize) == sToRemove))
    {
      sKey = string(sKey, iSize, strlen(sKey.c_str()) - iSize) ;
      (*it)->setID(sKey) ;
    }
  }
*/
}

/**
 * Get the active allergy codes (bam Id, category (allergy or molecule) and label) from a given free text entry
 */
void
NSBdmDriver::getAllergiesForSeed(NSBdmEntryArray* paListeArray, string* pSeed)
{
  getElementsForSeed(paListeArray, pSeed, string("allergies"),
                                          string("vidal:id"),
                                          string("id"),
                                          string("vidal:name"),
                                          (vector<Var>*) 0) ;
}

/**
 * Get the active substances codes (bam Id and label) for a given free text entry
 */
void
NSBdmDriver::getMoleculesForSeed(NSBdmEntryArray* paListeArray, string* pSeed)
{
  getElementsForSeed(paListeArray, pSeed, string("molecules"),
                                          string("vidal:id"),
                                          string(""),
                                          string("vidal:name"),
                                          (vector<Var>*) 0) ;
}

/**
 *   Load the array of units from file. We do that because current REST API
 *   doesn't allow to query units. The only option is to get them all and it
 *   is by far more efficient to get it from disk. Of course, it is assumed that
 *   it's content doesn't change (often).
 */
void
NSBdmDriver::getUnitCodesForSeed(string* /* pSeed */)
{
  // If already loaded, there is nothing to do
  //
  if (false == _aUnitsArray.empty())
    return ;

/*
  getElementsForSeed(&_aUnitsArray, pSeed, string("units"),
                                           string("vidal:unitId"),
                                           string(""),
                                           string("vidal:singularName"),
                                           (vector<Var>*) 0) ;
*/

  NSCsvParser cvsParser(pContexte->getSuperviseur()) ;

  cvsParser.setHasHeader(false) ;
  cvsParser.setSeparator(',') ;

  // Parse CSV file
  //
  string sFileName = pContexte->PathName(string("FPER")) + string("vidal_units.csv") ;

  if (false == cvsParser.parse(sFileName, true, true))
    return ;

  // Use "records" from CSV file to fill In Patients list
  //
  NSCsvRecordArray* pRecords = cvsParser.getRecords() ;
  if (((NSCsvRecordArray*) NULL == pRecords) || pRecords->empty())
    return ;

  _aUnitsArray.vider() ;

  for (NSCsvRecordIter CsvRecIt = pRecords->begin() ; pRecords->end() != CsvRecIt ; CsvRecIt++)
  {
    NSCsvFieldArray* pFields = (*CsvRecIt)->getFields() ;

    string sID    = string("") ;
    string sCode  = string("") ;
    string sLabel = string("") ;

    for (NSCsvFieldIter FieldIt = pFields->begin() ; pFields->end() != FieldIt ; FieldIt++)
    {
      if      ((*FieldIt)->getCol() == 0)
        sID    = (*FieldIt)->getValue() ;
      else if ((*FieldIt)->getCol() == 1)
        sCode  = (*FieldIt)->getValue() ;
      else if ((*FieldIt)->getCol() == 2)
        sLabel = (*FieldIt)->getValue() ;
    }

    _aUnitsArray.push_back(new NSBdmEntry(sID, sCode, sLabel)) ;

    if (pContexte->getSuperviseur()->getApplication())
      pContexte->getSuperviseur()->getApplication()->PumpWaitingMessages() ;
  }
}

void
NSBdmDriver::getElementsForSeed(NSBdmEntryArray* paListeArray, string* pSeed, const string sQuery, const string sIdTag, const string sCodeTag, const string sLabelTag, const vector<Var>* paSpecificVars)
{
  if ((NSBdmEntryArray*) NULL == paListeArray)
    return ;

  paListeArray->vider() ;

  if (((string*) NULL == pSeed) || (string("") == *pSeed) || ((Rest*) NULL == _pRest))
    return ;

  if (strlen(pSeed->c_str()) < 3)
    return ;

  // Preparing request
  //
  vector<Var> vars ;
  initVars(&vars) ;

  if     ((string("units") != sQuery) && (string("molecules") != sQuery))
    vars.push_back(Var(string("q"), FromISOToUTF8(*pSeed))) ;
  else if (string("molecules") == sQuery)
    vars.push_back(Var(string("startwith"), FromISOToUTF8(*pSeed), true)) ;

  if (paSpecificVars && (false == paSpecificVars->empty()))
    for (vector<Var>::const_iterator it = paSpecificVars->begin() ; paSpecificVars->end() != it ; it++)
      vars.push_back(Var(*it)) ;

  string sRequest = sQuery ;

  bool bExistNextPage = true ;
  while (bExistNextPage)
  {
    // Executing request
    //
    string sMessage = string("Calling BDM with a GET on query: ") + sRequest ;
    pContexte->getSuperviseur()->trace(&sMessage, 1, NSSuper::trDetails) ;

    string sResult = _pRest->request(sRequest, string("GET"), vars) ;

    if (string("") == sResult)
      return ;

    // Make certain that it doesn't turn infinitely with the same request
    //
    string sPreviousRequest = sRequest ;

    // Parse result and returns the href of link with rel="next"
    //
    sRequest = fillArrayOfBdmEntriesFromResults(paListeArray, &sResult, sIdTag, sCodeTag, sLabelTag) ;

    if ((string("") == sRequest) || (sPreviousRequest == sRequest))
      bExistNextPage = false ;
    else
      initVars(&vars) ;
  }

  if (false == paListeArray->empty())
    sort(paListeArray->begin(), paListeArray->end(), BdmEntrySortByLabelInf) ;
}

string
NSBdmDriver::fillArrayOfBdmEntriesFromResults(NSBdmEntryArray* paListeArray, const string* psResult, const string sIdTag, const string sCodeTag, const string sLabelTag, const string sIdAttribute, const string sCodeAttribute, const string sLabelAttribute)
{
  if (((NSBdmEntryArray*) NULL == paListeArray) || ((string*) NULL == psResult))
    return string("") ;

  if (string("") == *psResult)
    return string("") ;

  // Processing results
  //
  TiXmlDocument docForSearch ;
  docForSearch.Parse(psResult->c_str()) ;

  TiXmlHandle docHandle(&docForSearch) ;
  TiXmlElement* pEntryElement = docHandle.FirstChild("feed").FirstChild("entry").Element() ;

  while (pEntryElement)
  {
    // Get ID
    //
    string sID = string("") ;
    if (string("") == sIdAttribute)
      sID = getTextForTag(pEntryElement, sIdTag) ;
    else
      sID = getAttributeForTag(pEntryElement, sIdTag, sIdAttribute) ;

    // Get code
    //
    string sCode = string("") ;
    if (string("") == sCodeAttribute)
      sCode = getTextForTag(pEntryElement, sCodeTag) ;
    else
      sCode = getAttributeForTag(pEntryElement, sCodeTag, sCodeAttribute) ;

    // Get label
    //
    string sLabel = string("") ;
    if (string("") == sLabelAttribute)
      sLabel = getTextForTag(pEntryElement, sLabelTag) ;
    else
      sLabel = getAttributeForTag(pEntryElement, sLabelTag, sLabelAttribute) ;

    if ((string("") != sID) && (string("") != sLabel) &&
                        (false == isEntryInArray(paListeArray, sID, sCode, sLabel)))
      paListeArray->push_back(new NSBdmEntry(sID, sCode, sLabel)) ;

    pEntryElement = pEntryElement->NextSiblingElement() ;
  }

  // Try to find a link to next page
  //
  TiXmlHandle docLinkHandle(&docForSearch) ;
  TiXmlElement* pLinkElement = docLinkHandle.FirstChild("feed").FirstChild("link").Element() ;
  while (pLinkElement)
  {
    string sRelAttribute = pLinkElement->Attribute(string("rel")) ;
    if (string("next") == sRelAttribute)
      return pLinkElement->Attribute(string("href")) ;

    pLinkElement = pLinkElement->NextSiblingElement("link") ;
  }

  return string("") ;
}

/**
 * Get ICD10 information from a code
 */
bool
NSBdmDriver::getIcd10Entry(NSBdmEntry* pResult, const string sIcdCode)
{
  if ((string("") == sIcdCode) || ((NSBdmEntry*) NULL == pResult) || ((Rest*) NULL == _pRest))
    return false ;

  NSBdmEntryArray aListeArray ;

  // Preparing request
  //
  vector<Var> vars ;
  initVars(&vars) ;

  string sRequest = string("pathologies?code=") + sIcdCode + string("&type=CIM10") ;

  bool bExistNextPage = true ;
  while (bExistNextPage)
  {
    // Executing request
    //
    string sMessage = string("Calling BDM with a GET on query: ") + sRequest ;
    pContexte->getSuperviseur()->trace(&sMessage, 1, NSSuper::trDetails) ;

    string sResult = _pRest->request(sRequest, string("GET"), vars) ;

    if (string("") == sResult)
      return false ;

    // Make certain that it doesn't turn infinitely with the same request
    //
    string sPreviousRequest = sRequest ;

    // Parse result and returns the href of link with rel="next"
    //
    sRequest = fillArrayOfBdmEntriesFromResults(&aListeArray, &sResult, string("vidal:id"), string("vidal:code"), string("vidal:name")) ;

    if ((string("") == sRequest) || (sPreviousRequest == sRequest))
      bExistNextPage = false ;
    else
      initVars(&vars) ;
  }

  if (aListeArray.empty())
    return false ;

  NSBdmEntryIter it = aListeArray.begin() ;

  *pResult = **it ;

  return true ;
}

/**
 *  Initialize patient information to the prescription checking process
 */
void
NSBdmDriver::initializeChecker(NSPrescriptionCheckingMessage *pMsg)
{
  if ((NSPrescriptionCheckingMessage*) NULL == pMsg)
    return ;

  NSPatientChoisi* pPatEnCours = pContexte->getPatient() ;
  if ((NSPatientChoisi*) NULL == pPatEnCours)
    return ;

  // Get patient information block
  //
  NSPresCheckPatientIntformation* pPatientInfo = pMsg->getPatientInformation() ;
  if ((NSPresCheckPatientIntformation*) NULL == pPatientInfo)
    return ;

  // Get basic information block in patient information
  //
  NSBdmEntryArray* pBasicInfo = pPatientInfo->getBasicInformation() ;
  if ((NSBdmEntryArray*) NULL == pBasicInfo)
    return ;

  pBasicInfo->vider() ;

  // Add birthdate
  //
  NVLdVTemps tNaissance ;
  if (tNaissance.initFromDate(pPatEnCours->getNaissance()))
    pBasicInfo->push_back(new NSBdmEntry(string("dateOfBirth"), tNaissance.getIso8601(), string("Date de naissance : ") + tNaissance.donneFormattedDateHeure(pContexte, pContexte->getUserLanguage()))) ;

  // Add gender
  //
  if      (pPatEnCours->estMasculin())
    pBasicInfo->push_back(new NSBdmEntry(string("gender"), string("MALE"), string("Genre masculin"))) ;
  else if (pPatEnCours->estFeminin())
    pBasicInfo->push_back(new NSBdmEntry(string("gender"), string("FEMALE"), string("Genre féminin"))) ;
}

/**
 *  Add patient information to the prescription checking process
 */
void
NSBdmDriver::addPatientInfoToChecker(NSPrescriptionCheckingMessage *pMsg, const NSPatPathoArray* pInformation)
{
  if (((NSPrescriptionCheckingMessage*) NULL == pMsg) ||
        ((NSPatPathoArray*) NULL == pInformation) || pInformation->empty())
    return ;

  // Get basic information into patient information
  //
  NSPresCheckPatientIntformation* pPatientInfo = pMsg->getPatientInformation() ;
  if ((NSPresCheckPatientIntformation*) NULL == pPatientInfo)
    return ;

  NSBdmEntryArray* pBasicInfo = pPatientInfo->getBasicInformation() ;
  if ((NSBdmEntryArray*) NULL == pBasicInfo)
    return ;

  // Get tag and unit expected by the BAM
  //
  string sTag  = string("") ;
  string sUnit = string("") ;

  PatPathoConstIter it = pInformation->begin() ;

  string sConcept             = (*it)->getLexique() ;
  string sIncomingInformation = (*it)->getLexiqueSens() ;

  if      (string("VPOID") == sIncomingInformation)
  {
    sTag  = string("weight") ;
    sUnit = string("2KG00") ;
  }
  else if (string("VTAIL") == sIncomingInformation)
  {
    sTag  = string("height") ;
    sUnit = string("2CM00") ;
  }
  else if (string("VCLAM") == sIncomingInformation)
  {
    sTag  = string("creatin") ;
    sUnit = string("2MLPM") ;
  }

  if (string("") == sTag)
    return ;

  it++ ;

  if (pInformation->end() == it)
    return ;

  // Check if we got the proper unit, and try to convert it if not
  //
  string sValue = (*it)->getComplement() ;
  if (string("") == sValue)
    return ;

  if ((*it)->getUnitSens() != sUnit)
  {
    NSCV NsCv(pContexte->getSuperviseur()) ;

    DBIResult result = NsCv.open() ;
	  if (result != DBIERR_NONE)
    {
    	string sErrorMessage = pContexte->getSuperviseur()->getText("unitConversion", "errorOpeningConversionDatabase") ;
      erreur(sErrorMessage.c_str(), standardError, result) ;
      return ;
    }

    double dVal = StringToDouble(sValue) ;
	  bool bCvtSuccess = NsCv.ConvertirUnite(&dVal, sUnit, (*it)->getUnitSens(), sIncomingInformation) ;
    sValue = DoubleToString(&dVal, -1, 0) ;

    NsCv.close() ;

    if (false == bCvtSuccess)
      return ;
  }

  // Build label
  //
  string sLangue  = pContexte->getUserLanguage() ;
  string sConceptLabel ;
  pContexte->getDico()->donneLibelle(sLangue, &sConcept, &sConceptLabel) ;

  string sUnitCode = sUnit ;
  pContexte->getDico()->donneCodeComplet(sUnitCode) ;
  string sUnitLabel ;
  pContexte->getDico()->donneLibelle(sLangue, &sUnitCode, &sUnitLabel) ;

  string sLabel = sConceptLabel + string(" : ") + sValue + string(" ") + sUnitLabel ;
  sLabel[0] = pseumaj(sLabel[0]) ;

  pBasicInfo->push_back(new NSBdmEntry(sTag, sValue, sLabel)) ;
}

/**
 *  Add patient information to the prescription checking process
 */
void
NSBdmDriver::addPatientInfoToChecker(NSPrescriptionCheckingMessage *pMsg, const string sInformation, const string sValue, const string sLabel)
{
  if (((NSPrescriptionCheckingMessage*) NULL == pMsg) ||
                 (string("") == sInformation) || (string("") == sValue))
    return ;

  // Get basic information into patient information
  //
  NSPresCheckPatientIntformation* pPatientInfo = pMsg->getPatientInformation() ;
  if ((NSPresCheckPatientIntformation*) NULL == pPatientInfo)
    return ;

  NSBdmEntryArray* pBasicInfo = pPatientInfo->getBasicInformation() ;
  if ((NSBdmEntryArray*) NULL == pBasicInfo)
    return ;

  pBasicInfo->push_back(new NSBdmEntry(sInformation, sValue, sLabel)) ;
}

/**
 *  Add pathology to the prescription checking process
 */
void
NSBdmDriver::addPathologyToChecker(NSPrescriptionCheckingMessage *pMsg, const string sIcdCode)
{
  if (((NSPrescriptionCheckingMessage*) NULL == pMsg) || (string("") == sIcdCode))
    return ;

  // Get basic information into patient information
  //
  NSPresCheckPatientIntformation* pPatientInfo = pMsg->getPatientInformation() ;
  if ((NSPresCheckPatientIntformation*) NULL == pPatientInfo)
    return ;

  NSBdmEntryArray* pPathoInfo = pPatientInfo->getPathologies() ;
  if ((NSBdmEntryArray*) NULL == pPathoInfo)
    return ;

  string sValue = string("") ;

  // Get Label and Bam ID
  //
  string sLabel = string("") ;

  NSBdmEntry bdmEntry ;
  if (getIcd10Entry(&bdmEntry, sIcdCode))
  {
    sLabel = bdmEntry.getCode() + string(" ") + bdmEntry.getLabel() ;
    sValue = string("vidal://cim10/code/") + bdmEntry.getCode() ;
  }

  if (string("") != sValue)
    pPathoInfo->push_back(new NSBdmEntry(string("pathology"), sValue, sLabel)) ;
}

/**
 *  Add an allergy to a molecule to the prescription checking process
 */
void
NSBdmDriver::addMoleculeToChecker(NSPrescriptionCheckingMessage *pMsg, const string sBamID, const string sLabel)
{
  if (((NSPrescriptionCheckingMessage*) NULL == pMsg) || (string("") == sBamID))
    return ;

  // Get basic information into patient information
  //
  NSPresCheckPatientIntformation* pPatientInfo = pMsg->getPatientInformation() ;
  if ((NSPresCheckPatientIntformation*) NULL == pPatientInfo)
    return ;

  NSBdmEntryArray* pMoleculeInfo = pPatientInfo->getMolecules() ;
  if ((NSBdmEntryArray*) NULL == pMoleculeInfo)
    return ;

  string sValue = string("vidal://molecule/") + sBamID ;

  pMoleculeInfo->push_back(new NSBdmEntry(string("molecule"), sValue, sLabel)) ;
}

/**
 *  Add an allergy to the prescription checking process
 */
void
NSBdmDriver::addAllergyToChecker(NSPrescriptionCheckingMessage *pMsg, const string sBamID, const string sLabel)
{
  if (((NSPrescriptionCheckingMessage*) NULL == pMsg) || (string("") == sBamID))
    return ;

  // Get basic information into patient information
  //
  NSPresCheckPatientIntformation* pPatientInfo = pMsg->getPatientInformation() ;
  if ((NSPresCheckPatientIntformation*) NULL == pPatientInfo)
    return ;

  NSBdmEntryArray* pMoleculeInfo = pPatientInfo->getMolecules() ;
  if ((NSBdmEntryArray*) NULL == pMoleculeInfo)
    return ;

  string sValue = string("vidal://allergy/") + sBamID ;

  pMoleculeInfo->push_back(new NSBdmEntry(string("allergies"), sValue, sLabel)) ;
}

/**
 *  Add a prescription line to the prescription checking process
 */
void
NSBdmDriver::addPrescriptionLineToChecker(NSPrescriptionCheckingMessage *pMsg, const NSPresCheckPrescriptionLine* pLine)
{
  if (((NSPrescriptionCheckingMessage*) NULL == pMsg) ||
      ((NSPresCheckPrescriptionLine*) NULL == pLine))
    return ;

  NSPresCheckPrescriptionLine* pNewLine = new NSPresCheckPrescriptionLine(*pLine) ;

  // Adapt dose unit
  //
  if (string("") != pNewLine->getDoseUnit())
  {
    string sUnit = pNewLine->getDoseUnit() ;

    pNewLine->setDoseUnit(string("")) ;

    // Get all synonyms in order to compare them with bdm's unit
    //
    VectString aSynonyms ;

    NSPathoInfoArray aSynRecords ;
    if (pContexte->getDico()->getAllSynonyms(sUnit, &aSynRecords, pContexte->getUserLanguage()))
    {
      if (false == aSynRecords.empty())
        for (pthIter it = aSynRecords.begin() ; aSynRecords.end() != it ; it++)
        {
          string sLabel = string("") ;
          (*it)->_Donnees.donneLibelleAffiche(&sLabel) ;
          aSynonyms.add(pseumaj(sLabel)) ;
        }
    }

    if (false == aSynonyms.empty())
    {
      // Just to make certain that units are loaded
      //
      string sUnit = **(aSynonyms.begin()) ;
      getUnitCodesForSeed(&sUnit) ;

      if (false == _aUnitsArray.empty())
        for (NSBdmEntryIter it = _aUnitsArray.begin() ; _aUnitsArray.end() != it ; it++)
          if (aSynonyms.contains(pseumaj((*it)->getLabel())))
          {
            pNewLine->setDoseUnit((*it)->getID()) ;
            break ;
          }
    }
  }

  pMsg->getPrescriptionLines()->push_back(pNewLine) ;
}

/**
 * Send the prescription checking message and get back alerts
 */
void
NSBdmDriver::checkPrescription(NSPrescriptionCheckingMessage *pMsg, ArrayCopyDrugs *pDrugs)
{
  if (((NSPrescriptionCheckingMessage*) NULL == pMsg) || ((Rest*) NULL == _pRest))
    return ;

  // Create message to post
  //
  string sMessageToSend = string("<?xml version=\"1.0\" encoding=\"UTF-8\"?>") + string(NewLineUnix) ;
  sMessageToSend += string("<prescription>") + string(NewLineUnix) ;

  sMessageToSend += pMsg->getPatientBlock() ;
  sMessageToSend += pMsg->getPrescriptionBlock() ;

  sMessageToSend += string("</prescription>") + string(NewLineUnix) ;

  sMessageToSend = FromISOToUTF8(sMessageToSend) ;

  // Send mesage and get answer
  //
  vector<Var> vars ;
  initVars(&vars) ;
  // vars.push_back(Var(string("start-page"), string("1"))) ;
  vars.push_back(Var(string("body"), sMessageToSend)) ;

  // Executing request
  //
  string sMessage = string("Calling BDM with a POST for prescription check.") ;
  pContexte->getSuperviseur()->trace(&sMessage, 1, NSSuper::trDetails) ;

  string sResult = _pRest->request(string("alerts/full"), string("POST"), vars) ;

  // Processing results
  //
  if (string("") != sResult)
    processCheckPrescriptionResult(&sResult, pDrugs) ;

  // Reseting "prescription indexes"
  //
  if (false == pDrugs->empty())
    for (drugsIter it = pDrugs->begin() ; pDrugs->end() != it ; it++)
      if ((*it)->getBamDrug())
        (*it)->getBamDrug()->setCheckPrescriptionIndex(string("")) ;
}

void
NSBdmDriver::getDrugsForCriteria(NsSelectableDrugArray* paDrugsList, string* pCode, BAMTABLETYPE iTableType)
{
  if (((NsSelectableDrugArray*) NULL == paDrugsList) || ((string*) NULL == pCode))
    return ;

  paDrugsList->vider() ;

  while (_isBusy)
    pContexte->getSuperviseur()->getApplication()->PumpWaitingMessages() ;

  _isBusy = true ;

  switch(iTableType)
  {
    case bamTableIndication :
      getDrugsForIndication(paDrugsList, pCode) ;
      break ;
    case bamTableSubstance :
      getDrugsForSubstance(paDrugsList, pCode) ;
      break ;
    case bamTableATC :
      getDrugsForAtcCode(paDrugsList, pCode) ;
      break ;
  }

  _isBusy = false ;
}

void
NSBdmDriver::getDrugsForSeed(NsSelectableDrugArray* paDrugsList, string* pSeed, const string sQuery, const vector<Var>* paSpecificVars)
{
  if ((NsSelectableDrugArray*) NULL == paDrugsList)
    return ;

  paDrugsList->vider() ;

  if ((string("") == sQuery) || ((Rest*) NULL == _pRest))
    return ;

  // The search criteria can be the seed (to inject in the query) or a Var
  //
  if ((((string*) NULL == pSeed) || (string("") == *pSeed)) &&
      (((vector<Var>*) NULL == paSpecificVars) || paSpecificVars->empty()))
    return ;

  // Preparing request
  //
  string sRequest = sQuery ;

  if (pSeed)
  {
    size_t pos = sRequest.find("[$code$]") ;
    while (NPOS != pos)
    {
      sRequest.replace(pos, strlen("[$code$]"), *pSeed) ;
      pos = sRequest.find("[$code$]", pos + 1) ;
    }
  }

  vector<Var> vars ;
  initVars(&vars) ;

  if (paSpecificVars && (false == paSpecificVars->empty()))
    for (vector<Var>::const_iterator it = paSpecificVars->begin() ; paSpecificVars->end() != it ; it++)
      vars.push_back(Var(*it)) ;

  bool bExistNextPage = true ;
  while (bExistNextPage)
  {
    // Executing request
    //
    string sMessage = string("Calling BDM with a GET on query: ") + sRequest ;
    pContexte->getSuperviseur()->trace(&sMessage, 1, NSSuper::trDetails) ;

    string sResult = _pRest->request(sRequest, string("GET"), vars) ;

    if (string("") == sResult)
      return ;

    sRequest = fillArrayOfDrugsFromResults(paDrugsList, &sResult) ;

    if (string("") == sRequest)
      bExistNextPage = false ;
    else
      initVars(&vars) ;
  }

  if (false == paDrugsList->empty())
    for (NsSelectableDrugIter it = paDrugsList->begin() ; paDrugsList->end() != it ; it++)
      fillDrugComponents(*it) ;
}

void
NSBdmDriver::getDrugsForIndication(NsSelectableDrugArray* paDrugsList, string* pCode)
{
  getDrugsForSeed(paDrugsList, pCode, string("indication/[$code$]/products"), (vector<Var>*) 0) ;
}

void
NSBdmDriver::getDrugsForSubstance(NsSelectableDrugArray* paDrugsList, string* pCode)
{
  getDrugsForSeed(paDrugsList, pCode, string("molecule/active-substance/[$code$]/products"), (vector<Var>*) 0) ;
}

void
NSBdmDriver::getDrugsForAtcCode(NsSelectableDrugArray* paDrugsList, string* pCode)
{
  getDrugsForSeed(paDrugsList, pCode, string("atc-classification/[$code$]/products"), (vector<Var>*) 0) ;
}

/**
 *  Fill an array of drugs from a result and returns link to next page
 *
 *  It could have been cleaner to separate the 2 functions (filling the array
 *  and looking for the link to next page) but it would mean parsing twice
 */
string
NSBdmDriver::fillArrayOfDrugsFromResults(NsSelectableDrugArray* paDrugsList, const string* psResult)
{
  if (((NsSelectableDrugArray*) NULL == paDrugsList) || ((string*) NULL == psResult))
    return string("") ;

  if (string("") == *psResult)
    return string("") ;

  // Processing results
  //
  TiXmlDocument docForSearch ;
  docForSearch.Parse(psResult->c_str()) ;

  TiXmlHandle docHandle(&docForSearch) ;
  TiXmlElement* pEntryElement = docHandle.FirstChild("feed").FirstChild("entry").Element() ;

  while (pEntryElement)
  {
    string sLabel    = getTextForTag(pEntryElement, string("title")) ;
    string sVidalID  = getTextForTag(pEntryElement, string("vidal:id")) ;
    string sCodeCis  = getTextForTag(pEntryElement, string("vidal:cis")) ;
    string sMinPrice = getTextForTag(pEntryElement, string("vidal:minUcdRangePrice")) ;
    string sMaxPrice = getTextForTag(pEntryElement, string("vidal:maxUcdRangePrice")) ;

    if ((string("") != sCodeCis) && (string("") != sLabel))
    {
      string sPrice = sMinPrice ;
      if (sMinPrice != sMaxPrice)
        sPrice += string(" - ") + sMaxPrice ;

      NsSelectableDrug* pDrug = new NsSelectableDrug(sVidalID, sCodeCis, sLabel, sPrice) ;

      pDrug->setOnMarketDate(getTextForTag(pEntryElement, string("vidal:onMarketDate"))) ;
      pDrug->setAmmLabel(getTextForTag(pEntryElement, string("vidal:ammType"))) ;
      pDrug->setAmmType(getAttributeForTag(pEntryElement, string("vidal:ammType"), string("vidalId"))) ;
      pDrug->setMarketStatusLabel(getTextForTag(pEntryElement, string("vidal:marketStatus"))) ;
      pDrug->setMarketStatusType(getAttributeForTag(pEntryElement, string("vidal:marketStatus"), string("name"))) ;
      pDrug->setWithoutPrescription(getTextForTag(pEntryElement, string("vidal:withoutPrescription"))) ;
      pDrug->setCompanyLabel(getTextForTag(pEntryElement, string("vidal:company"))) ;
      pDrug->setCompanyBdmId(getAttributeForTag(pEntryElement, string("vidal:company"), string("vidalId"))) ;
      pDrug->setCompanyRoleType(getAttributeForTag(pEntryElement, string("vidal:company"), string("type"))) ;
      pDrug->setListId(getAttributeForTag(pEntryElement, string("vidal:list"), string("name"))) ;
      pDrug->setListLabel(getTextForTag(pEntryElement, string("vidal:list"))) ;
      pDrug->setVirtualDrudId(getAttributeForTag(pEntryElement, string("vidal:vmp"), string("vidalId"))) ;
      pDrug->setVirtualDrudLabel(getTextForTag(pEntryElement, string("vidal:vmp"))) ;
      pDrug->setGalenicFormId(getAttributeForTag(pEntryElement, string("vidal:galenicForm"), string("vidalId"))) ;
      pDrug->setGalenicFormLabel(getTextForTag(pEntryElement, string("vidal:galenicForm"))) ;
      pDrug->setVigilanceWarning(getTextForTag(pEntryElement, string("vidal:vigilance"))) ;
      pDrug->setMinUcdRangePrice(sMinPrice) ;
      pDrug->setMaxUcdRangePrice(sMaxPrice) ;
      pDrug->setRefundRate(getTextForTag(pEntryElement, string("vidal:refundRate"))) ;
      pDrug->setDispensationPlace(getAttributeForTag(pEntryElement, string("vidal:dispensationPlace"), string("name"))) ;
      pDrug->setGenericType(getTextForTag(pEntryElement, string("vidal:onMarketDate"))) ;
      pDrug->setHorsGHS(getTextForTag(pEntryElement, string("vidal:horsGHS"))) ;
      pDrug->setPrescribableByMidwife(getTextForTag(pEntryElement, string("vidal:midwife"))) ;
      pDrug->setDrugInSport(getTextForTag(pEntryElement, string("vidal:drugInSport"))) ;
      pDrug->setBeCareful(getTextForTag(pEntryElement, string("vidal:beCareful"))) ;
      pDrug->setRetrocession(getTextForTag(pEntryElement, string("vidal:retrocession"))) ;
      pDrug->setSafetyAlert(getTextForTag(pEntryElement, string("vidal:safetyAlert"))) ;

      // Process links
      //
      TiXmlNode* pIdNode = pEntryElement->FirstChild(string("link")) ;
      if (pIdNode)
      {
        TiXmlElement* pLinkAsElement = pIdNode->ToElement() ;
        while (pLinkAsElement)
        {
          pDrug->addLink(pLinkAsElement->Attribute(string("title")),
                         pLinkAsElement->Attribute(string("href")),
                         pLinkAsElement->Attribute(string("type")),
                         pLinkAsElement->Attribute(string("rel"))) ;

          pLinkAsElement = pLinkAsElement->NextSiblingElement("link") ;
        }
      }

      paDrugsList->push_back(pDrug) ;
    }

    pEntryElement = pEntryElement->NextSiblingElement() ;
  }

  // Try to find a link to next page
  //
  TiXmlHandle docLinkHandle(&docForSearch) ;
  TiXmlElement* pLinkElement = docLinkHandle.FirstChild("feed").FirstChild("link").Element() ;
  while (pLinkElement)
  {
    string sRelAttribute = pLinkElement->Attribute(string("rel")) ;
    if (string("next") == sRelAttribute)
      return pLinkElement->Attribute(string("href")) ;

    pLinkElement = pLinkElement->NextSiblingElement("link") ;
  }

  return string("") ;
}

/**
 * Fill the _aActivePrinciples array for a given drug
 */
void
NSBdmDriver::fillDrugComponents(NsSelectableDrug* pDrug)
{
  if ((NsSelectableDrug*) NULL == pDrug)
    return ;

  // Get the proper link
  //
  string sRequest = pDrug->getLinkFromTitle(string("MOLECULES")) ;
  if (string("") == sRequest)
    return ;

  vector<Var> vars ;
  initVars(&vars) ;

  bool bExistNextPage = true ;
  while (bExistNextPage)
  {
    // Executing request
    //
    string sMessage = string("Calling BDM with a GET on query: ") + sRequest ;
    pContexte->getSuperviseur()->trace(&sMessage, 1, NSSuper::trDetails) ;

    string sResult = _pRest->request(sRequest, string("GET"), vars) ;

    if (string("") == sResult)
      return ;

    sRequest = fillArrayOfDrugComponents(pDrug->getComponents(), &sResult) ;

    if (string("") == sRequest)
      bExistNextPage = false ;
    else
      initVars(&vars) ;
  }
}

/**
 *  Fill an array of drugs from a result and returns link to next page
 *
 *  It could have been cleaner to separate the 2 functions (filling the array
 *  and looking for the link to next page) but it would mean parsing twice
 */
string
NSBdmDriver::fillArrayOfDrugComponents(NsDrugComponentsArray* paDrugComponentsList, const string* psResult)
{
  if (((NsDrugComponentsArray*) NULL == paDrugComponentsList) || ((string*) NULL == psResult))
    return string("") ;

  if (string("") == *psResult)
    return string("") ;

  // Processing results
  //
  TiXmlDocument docForSearch ;
  docForSearch.Parse(psResult->c_str()) ;

  TiXmlHandle docHandle(&docForSearch) ;
  TiXmlElement* pEntryElement = docHandle.FirstChild("feed").FirstChild("entry").Element() ;

  while (pEntryElement)
  {
    string sLabel   = getTextForTag(pEntryElement, string("title")) ;
    string sVidalID = getTextForTag(pEntryElement, string("vidal:id")) ;
    string sRanking = getTextForTag(pEntryElement, string("vidal:ranking")) ;
    string sType    = getAttributeForTag(pEntryElement, string("vidal:itemType"), string("name")) ;

    if ((string("") != sVidalID) && (string("") != sLabel))
    {
      NsDrugComponent* pComponent = new NsDrugComponent(sVidalID, sLabel) ;

      pComponent->setItemType(sType) ;
      pComponent->setRanking(sRanking) ;

      paDrugComponentsList->push_back(pComponent) ;
    }

    pEntryElement = pEntryElement->NextSiblingElement() ;
  }

  // Try to find a link to next page
  //
  TiXmlHandle docLinkHandle(&docForSearch) ;
  TiXmlElement* pLinkElement = docLinkHandle.FirstChild("feed").FirstChild("link").Element() ;
  while (pLinkElement)
  {
    string sRelAttribute = pLinkElement->Attribute(string("rel")) ;
    if (string("next") == sRelAttribute)
      return pLinkElement->Attribute(string("href")) ;

    pLinkElement = pLinkElement->NextSiblingElement("link") ;
  }

  return string("") ;
}

/**
 * Get Security indicators that extend the drug information
 */
void
NSBdmDriver::getSecurityIndicators(NsSelectableDrug* pDrugInformation)
{
  if ((NsSelectableDrug*) NULL == pDrugInformation)
    return ;

  NsHtmlLinksArray* pLinks = pDrugInformation->getLinks() ;

  if (((NsHtmlLinksArray*) NULL == pLinks) || pLinks->empty())
    return ;

  // Get the indicator link
  //
  NsHtmlLinkIter it = pLinks->begin() ;
  for ( ; pLinks->end() != it ; it++)
    if (pseumaj((*it)->getTitle()) == string("INDICATORS"))
      break ;

  // Not found, then leave
  //
  if (pLinks->end() == it)
    return ;

  // Get blocks for this URL
  //
  NsXmlBlocksArray aBlocks ;
  getBlocksFromUrl((*it)->getURL(), &aBlocks) ;

  if (aBlocks.empty())
    return ;

  // Get indicators entries and add them to drug information
  //
  NsXmlEntriesArray* pEntries = (*aBlocks.begin())->getEntries() ;
  if (((NsXmlEntriesArray*) NULL == pEntries) || pEntries->empty())
    return ;

  NsXmlEntriesArray* pIndicatorsTank = pDrugInformation->getIndicators() ;

  for (NsXmlEntryIter entryIt = pEntries->begin() ; pEntries->end() != entryIt ; entryIt++)
    if (pseumaj((*entryIt)->getField()) == string("INDICATOR"))
      pIndicatorsTank->push_back(new NsXmlEntry(**entryIt)) ;
}

void
NSBdmDriver::processCheckPrescriptionResult(const string* psResult, ArrayCopyDrugs *pDrugs)
{
  if (((string*) NULL == psResult) || (string("") == *psResult))
    return ;

  if (((ArrayCopyDrugs*) NULL == pDrugs) || pDrugs->empty())
    return ;

  // Reset alerts
  //
  for (drugsIter it = pDrugs->begin() ; pDrugs->end() != it ; it++)
  {
    NsSelectableDrug* pBamDrug = (*it)->getBamDrug() ;
    if (pBamDrug)
      pBamDrug->resetCheckAlerts() ;
  }

  // Processing results
  //
  TiXmlDocument docForSearch ;
  docForSearch.Parse(psResult->c_str()) ;

  TiXmlHandle docHandle(&docForSearch) ;
  TiXmlElement* pEntryElement = docHandle.FirstChild("feed").FirstChild("entry").Element() ;

  while (pEntryElement)
  {
    // Get entry tag's category attribute
    //
    string sCategory = pEntryElement->Attribute(string("vidal:categories")) ;
    sCategory = pseumaj(sCategory) ;

    if      (string("PRESCRIPTION_LINE") == sCategory)
      processCheckPrescriptionLineResult(pEntryElement, pDrugs) ;
    else if (string("ALERT") == sCategory)
      processCheckPrescriptionAlertResult(pEntryElement, pDrugs) ;
    else if (string("PATIENT") == sCategory)
      processCheckPrescriptionAlertResult(pEntryElement, pDrugs) ;
    else
      break ;

    pEntryElement = pEntryElement->NextSiblingElement() ;
  }
}

/**
 * Process a prescription line check result 
 */
void
NSBdmDriver::processCheckPrescriptionLineResult(TiXmlElement* pEntryElement, ArrayCopyDrugs *pDrugs)
{
  if (((TiXmlElement*) NULL == pEntryElement) || ((ArrayCopyDrugs*) NULL == pDrugs))
    return ;

  // Get prescription line from id
  //
  string sPrescriptionLine = getTextForTag(pEntryElement, string("id")) ;
  if (string("") == sPrescriptionLine)
    return ;

  // Get the drug from the prescription line
  //
  NsSelectableDrug* pCurrentBamDrug = getPrescriptionLine(sPrescriptionLine, pDrugs) ;

  // If a Bam drug was found, initialize its prescription check information
  //
  if ((NsSelectableDrug*) NULL == pCurrentBamDrug)
    return ;

  pCurrentBamDrug->resetCheckResult() ;

  string sCISeverity       = getTextForTag(pEntryElement, string("vidal:maxContraIndicationSeverity")) ;
  string sAllergySeverity  = getTextForTag(pEntryElement, string("vidal:maxAllergySeverity")) ;
  string sInteractSeverity = getTextForTag(pEntryElement, string("vidal:maxDrugDrugInteractionsSeverity")) ;
  string sPhyChimSeverity  = getTextForTag(pEntryElement, string("vidal:maxPhysicoChemicalSeverity")) ;
  string sPrecautSeverity  = getTextForTag(pEntryElement, string("vidal:maxPrecautionSeverity")) ;
  string sRedundanSeverity = getTextForTag(pEntryElement, string("vidal:maxRedundantInteractionsSeverity")) ;
  string sPosologySeverity = getTextForTag(pEntryElement, string("vidal:maxPosologySeverity")) ;
  string sProcreatSeverity = getTextForTag(pEntryElement, string("vidal:maxProcreationSeverity")) ;

  pCurrentBamDrug->setContraIndicationSeverity(sCISeverity) ;
  pCurrentBamDrug->setAllergySeverity(sAllergySeverity) ;
  pCurrentBamDrug->setDrugDrugInteractionsSeverity(sInteractSeverity) ;
  pCurrentBamDrug->setPhysicoChemicalSeverity(sPhyChimSeverity) ;
  pCurrentBamDrug->setPrecautionSeverity(sPrecautSeverity) ;
  pCurrentBamDrug->setRedundantInteractionsSeverity(sRedundanSeverity) ;
  pCurrentBamDrug->setPosologySeverity(sPosologySeverity) ;
  pCurrentBamDrug->setProcreationSeverity(sProcreatSeverity) ;

  pCurrentBamDrug->setContraIndicationSeverityLevel(getAlertLevel(pEntryElement, string("vidal:maxContraIndicationSeverity"))) ;
  pCurrentBamDrug->setAllergySeverityLevel(getAlertLevel(pEntryElement, string("vidal:maxAllergySeverity"))) ;
  pCurrentBamDrug->setDrugDrugInteractionsSeverityLevel(getAlertLevel(pEntryElement, string("vidal:maxDrugDrugInteractionsSeverity"))) ;
  pCurrentBamDrug->setPhysicoChemicalSeverityLevel(getAlertLevel(pEntryElement, string("vidal:maxPhysicoChemicalSeverity"))) ;
  pCurrentBamDrug->setPrecautionSeverityLevel(getAlertLevel(pEntryElement, string("vidal:maxPrecautionSeverity"))) ;
  pCurrentBamDrug->setRedundantInteractionsSeverityLevel(getAlertLevel(pEntryElement, string("vidal:maxRedundantInteractionsSeverity"))) ;
  pCurrentBamDrug->setPosologySeverityLevel(getAlertLevel(pEntryElement, string("vidal:maxPosologySeverity"))) ;
  pCurrentBamDrug->setProcreationSeverityLevel(getAlertLevel(pEntryElement, string("vidal:maxProcreationSeverity"))) ;

  // Get links
  //
  TiXmlNode* pIdNode = pEntryElement->FirstChild("link") ;
  if ((TiXmlNode*) NULL == pIdNode)
    return ;

  TiXmlElement* pLinkAsElement = pIdNode->ToElement() ;
  while (pLinkAsElement)
  {
    string sTitle = string("") ;
    string sType  = string("") ;
    string sHref  = string("") ;
    string sRel   = string("") ;

    // Get attributes
    //
    TiXmlAttribute* pAttribute = pLinkAsElement->FirstAttribute() ;
    while (pAttribute)
    {
      string sName = pAttribute->Name() ;

      if      (string("rel") == sName)
        sRel  = pAttribute->Value() ;
      else if (string("href") == sName)
        sHref  = pAttribute->Value() ;
      else if (string("type") == sName)
        sType  = pAttribute->Value() ;
      else if (string("title") == sName)
        sTitle = pAttribute->Value() ;

      pAttribute = pAttribute->Next() ;
    }

    pCurrentBamDrug->addAlertLink(sTitle, sHref, sType, sRel) ;

    pLinkAsElement = pLinkAsElement->NextSiblingElement("link") ;
  }
}

/**
 * Process an alert from a check result
 */
void
NSBdmDriver::processCheckPrescriptionAlertResult(TiXmlElement* pEntryElement, ArrayCopyDrugs *pDrugs)
{
  if (((TiXmlElement*) NULL == pEntryElement) || ((ArrayCopyDrugs*) NULL == pDrugs))
    return ;

  if (pDrugs->empty())
    return ;

  NsDrugAlert alert ;

  alert.initializeType(getTextForTag(pEntryElement, string("vidal:type"))) ;
  alert.setTitle(getTextForTag(pEntryElement, string("title"))) ;
  alert.setID(getTextForTag(pEntryElement, string("id"))) ;
  alert.setContent(getTextForTag(pEntryElement, string("content"))) ;
  alert.setContentType(getAttributeForTag(pEntryElement, string("content"), string("type"))) ;
  alert.initializeSeverity(getTextForTag(pEntryElement, string("vidal:severity")))  ;
  alert.setDetail(getTextForTag(pEntryElement, string("vidal:detail"))) ;
  alert.setDetailType(getAttributeForTag(pEntryElement, string("vidal:detail"), string("type"))) ;

  // Get links
  //
  TiXmlNode* pIdNode = pEntryElement->FirstChild("link") ;
  if ((TiXmlNode*) NULL == pIdNode)
    return ;

  TiXmlElement* pLinkAsElement = pIdNode->ToElement() ;
  while (pLinkAsElement)
  {
    string sTitle = string("") ;
    string sType  = string("") ;
    string sHref  = string("") ;
    string sRel   = string("") ;

    // Get attributes
    //
    TiXmlAttribute* pAttribute = pLinkAsElement->FirstAttribute() ;
    while (pAttribute)
    {
      string sName = pAttribute->Name() ;

      if      (string("rel") == sName)
        sRel  = pAttribute->Value() ;
      else if (string("href") == sName)
        sHref  = pAttribute->Value() ;
      else if (string("type") == sName)
        sType  = pAttribute->Value() ;
      else if (string("title") == sName)
        sTitle = pAttribute->Value() ;

      pAttribute = pAttribute->Next() ;
    }

    // Inline link -> pointer on the considered product
    //
    // <link rel="inline" href="vidal://product/19649"/>
    //
    if (string("INLINE") == pseumaj(sRel))
    {
      size_t iRefLen = strlen(sHref.c_str()) ;
      if ((iRefLen > 16) && (string("vidal://product/") == string(sHref, 0, 16)))
      {
        string sProductId = string(sHref, 16, iRefLen - 16) ;
      }
    }

    alert.addLink(sTitle, sHref, sType, sRel) ;

    pLinkAsElement = pLinkAsElement->NextSiblingElement("link") ;
  }

  NsHtmlLinksArray* pLinksArray = alert.getLinks() ;
  if (pLinksArray->empty())
    return ;

  for (NsHtmlLinkIter it = pLinksArray->begin() ; pLinksArray->end() != it ; it++)
  {
    // Inline link -> pointer on the considered product
    //
    // <link rel="inline" href="vidal://product/19649"/>
    // <link rel="inline" href="vidal://prescription_line/1"/>
    //
    if ((*it)->getRelTitle() == NsHtmlLink::relInline)
    {
      string sHref = (*it)->getURL() ;
      size_t iRefLen = strlen(sHref.c_str()) ;

      if ((iRefLen > 26) && (string("vidal://prescription_line/") == string(sHref, 0, 26)))
      {
        // Get the drug from the prescription line
        //
        NsSelectableDrug* pBamDrug = getPrescriptionLine(sHref, pDrugs) ;
        if (pBamDrug)
          pBamDrug->addAlert(&alert) ;
      }
      else if ((iRefLen > 16) && (string("vidal://product/") == string(sHref, 0, 16)))
      {
        string sProductId = string(sHref, 16, iRefLen - 16) ;

        // Find the drug(s) for this product Id
        //
        for (drugsIter it = pDrugs->begin() ; pDrugs->end() != it ; it++)
        {
          NsSelectableDrug* pBamDrug = (*it)->getBamDrug() ;
          if (pBamDrug && (pBamDrug->getBdmID() == sProductId))
            pBamDrug->addAlert(&alert) ;
        }
      }
      else if ((iRefLen > 12) && (string("vidal://vmp/") == string(sHref, 0, 12)))
      {
        string sVmId = string(sHref, 12, iRefLen - 12) ;

        // Find the drug(s) for this virtual drug Id
        //
        for (drugsIter it = pDrugs->begin() ; pDrugs->end() != it ; it++)
        {
          NsSelectableDrug* pBamDrug = (*it)->getBamDrug() ;
          if (pBamDrug && (pBamDrug->getBdmVmID() == sVmId))
            pBamDrug->addAlert(&alert) ;
        }
      }
    }
  }
}

/**
 * Get the bdm drug from a link of the kind "vidal://prescription_line/1"
 */
NsSelectableDrug*
NSBdmDriver::getPrescriptionLine(const string sInlineRef, ArrayCopyDrugs *pDrugs)
{
  if (((ArrayCopyDrugs*) NULL == pDrugs) || (string("") == sInlineRef))
    return (NsSelectableDrug*) 0 ;

  size_t iRefLen = strlen(sInlineRef.c_str()) ;

  if ((iRefLen <= 26) || (string("vidal://prescription_line/") != string(sInlineRef, 0, 26)))
    return (NsSelectableDrug*) 0 ;

  string sPrescriptionLine = string(sInlineRef, 26, iRefLen - 26) ;

  // Find proper drug
  //
  if (string("") == sPrescriptionLine)
    return (NsSelectableDrug*) 0 ;

  for (drugsIter it = pDrugs->begin() ; pDrugs->end() != it ; it++)
  {
    NsSelectableDrug* pBamDrug = (*it)->getBamDrug() ;
    if (pBamDrug && (pBamDrug->getCheckPrescriptionIndex() == sPrescriptionLine))
      return pBamDrug ;
  }

  return (NsSelectableDrug*) 0 ;
}

/**
 *  Function that get a full html document from a partial url
 */
string
NSBdmDriver::getFullUrl(const string sPartialUrl)
{
  if ((string("") == sPartialUrl) || ((Rest*) NULL == _pRest))
    return string("") ;

  string sFullUrl = _pRest->build_uri(sPartialUrl) ;

  vector<Var> vars ;
  initVars(&vars) ;

  return _pRest->add_vars_to_uri(sFullUrl, vars) ;
}

/**
 * Get version Id
 */
string
NSBdmDriver::getBamVersionId()
{
  // Get the proper link
  //
  string sRequest = string("version") ;
  if (string("") == sRequest)
    return string("") ;

  vector<Var> vars ;
  initVars(&vars) ;

  string sMessage = string("Calling BDM with a GET on query: ") + sRequest ;
  pContexte->getSuperviseur()->trace(&sMessage, 1, NSSuper::trDetails) ;

  string sResult = _pRest->request(sRequest, string("GET"), vars) ;

  if (string("") == sResult)
    return string("") ;

  // Processing results
  //
  TiXmlDocument docForSearch ;
  docForSearch.Parse(sResult.c_str()) ;

  TiXmlHandle docHandle(&docForSearch) ;
  TiXmlElement* pEntryElement = docHandle.FirstChild("feed").FirstChild("entry").Element() ;

  string sVersionId = string("Base de donnée médicamenteuse Vidal") ;

  if (pEntryElement)
    sVersionId += string(" version ") + getTextForTag(pEntryElement, string("vidal:version")) ;
  else
    sVersionId += string(" version inconnue") ;

  return sVersionId ;
}

/**
 *  Get the text (converted in iso-8859-15) from first son with a given tag
 */
string
NSBdmDriver::getTextForTag(TiXmlElement* pEntryElement, const string sTag)
{
  if (((TiXmlElement*) NULL == pEntryElement) || (string("") == sTag))
    return string("") ;

  TiXmlNode* pIdNode = pEntryElement->FirstChild(sTag) ;
  if ((TiXmlNode*) NULL == pIdNode)
    return string("") ;

  return getTextForNode(pIdNode) ;
}

/**
 *  Get the text (converted in iso-8859-15) from a node
 */
string
NSBdmDriver::getTextForNode(TiXmlNode* pNode)
{
  if ((TiXmlNode*) NULL == pNode)
    return string("") ;

  TiXmlText* pIdText = pNode->FirstChild()->ToText() ;
  if (pIdText)
    return FromUTF8ToISO(string(pIdText->Value())) ;

  return string("") ;
}

/**
 *  Get the text (converted in iso-8859-15) from first son with a given tag
 */
string
NSBdmDriver::getAttributeForTag(TiXmlElement* pEntryElement, const string sTag, const string sAttributeName)
{
  if (((TiXmlElement*) NULL == pEntryElement) || (string("") == sTag))
    return string("") ;

  TiXmlNode* pIdNode = pEntryElement->FirstChild(sTag) ;
  if ((TiXmlNode*) NULL == pIdNode)
    return string("") ;

  TiXmlElement* pElement = pIdNode->ToElement() ;
  if ((TiXmlElement*) NULL == pElement)
    return string("") ;

  const char* pszAttribute = pElement->Attribute(sAttributeName) ;
  if ((char*) NULL == pszAttribute)
    return string("") ;

  string sAttribute = string(pszAttribute) ;

  return FromUTF8ToISO(sAttribute) ;
}

/**
 * Get the alert level of a tag in the form
 * <vidal:maxAllergySeverity severity=\"NO_ALERT\">bla</vidal:maxAllergySeverity>
 */
NsSelectableDrug::ALERTLEVEL
NSBdmDriver::getAlertLevel(TiXmlElement* pEntryElement, const string sTag)
{
  string sAttribute = getAttributeForTag(pEntryElement, sTag, string("severity")) ;
  if (string("") == sAttribute)
    return NsSelectableDrug::alertUndefined ;

  sAttribute = pseumaj(sAttribute) ;
  
  if (string("NO_ALERT") == sAttribute)
    return NsSelectableDrug::alertNone ;
  if (string("LEVEL_1") == sAttribute)
    return NsSelectableDrug::alertLevel1 ;
  if (string("LEVEL_2") == sAttribute)
    return NsSelectableDrug::alertLevel2 ;
  if (string("LEVEL_3") == sAttribute)
    return NsSelectableDrug::alertLevel3 ;
  if (string("LEVEL_4") == sAttribute)
    return NsSelectableDrug::alertLevel4 ;

  return NsSelectableDrug::alertUndefined ;
}

string
NSBdmDriver::GetCleanTag(const string sTag)
{
  if (string("") == sTag)
    return string("") ;

  string sModifiedTag = sTag ;

  if ((strlen(sTag.c_str()) > 6) && (string("vidal:") == string(sTag, 0, 6)))
    sModifiedTag = string(sModifiedTag, 6, strlen(sModifiedTag.c_str()) - 6) ;

  return FromUTF8ToISO(sModifiedTag) ;
}

/**
 *  Is a pair(ID, label) already present inside the array
 */
bool
NSBdmDriver::isEntryInArray(const NSBdmEntryArray* paListeArray, const string sID, const string sCode, const string sLabel) const
{
  if (((NSBdmEntryArray*) NULL == paListeArray) || paListeArray->empty())
    return false ;

  for (NSBdmEntryConstIter it = paListeArray->begin() ; paListeArray->end() != it ; it++)
    if (((*it)->getID() == sID) && ((*it)->getCode() == sCode) && ((*it)->getLabel() == sLabel))
      return true ;

  return false ;
}

/**
 * NsSelectableDrug
 *
 * Information from drug database that can be displayed for drug selection
 */

NsSelectableDrug::NsSelectableDrug()
{
  reset() ;
}

NsSelectableDrug::NsSelectableDrug(string sID, string sCIS, string sLabel, string sPrice)
{
  reset() ;

  _sBdmID = sID ;
  _sCIS   = sCIS ;
  _sLabel = sLabel ;
  _sPrice = sPrice ;
}

NsSelectableDrug::NsSelectableDrug(const NsSelectableDrug& rv)
{
  initFrom(rv) ;
}

bool
NsSelectableDrug::isConceptNeededForSecurityControl(const string sConcept)
{
  if ((string("") == sConcept) || _aIndicators.empty())
    return false ;

  string sMajConcept = pseumaj(sConcept) ;

  for (NsXmlEntryIter it = _aIndicators.begin() ; _aIndicators.end() != it ; it++)
    if ((pseumaj((*it)->getField()) == string("INDICATOR")) &&
        (pseumaj((*it)->getValue()) == sMajConcept))
      return true ;

  return false ;
}

NsSelectableDrug&
NsSelectableDrug::operator=(const NsSelectableDrug& src)
{
  if (&src == this)
		return *this ;

  initFrom(src) ;

  return *this ;
}

/**
 * Reset all elements
 */
void
NsSelectableDrug::reset()
{
  _sPrescribedNodeId      = string("") ;

  _sBdmID                 = string("") ;
  _sBdmVmID               = string("") ;
  _sLabel                 = string("") ;

  _sCIS                   = string("") ;
  _sATC                   = string("") ;
  _sMedicaBase            = string("") ;
  _sOnMarketDate          = string("") ;
  _iAmmType               = ammUndefined ;
  _sAmmLabel              = string("") ;
  _iMarketStatusType      = marketStatusUndefined ;
  _iMarketStatusLabel     = string("") ;
  _bWithoutPrescription   = boolUndefined ;
  _sCompanyLabel          = string("") ;
  _sCompanyBdmId          = string("") ;
  _iCompanyRoleType       = companyRoleUndefined ;

  _sListId                = string("") ;
  _sListLabel             = string("") ;

  _aComponents.vider() ;
  _aIndicators.vider() ;

  _sVirtualDrugBdmId      = string("") ;
  _sVirtualDrugLabel      = string("") ;

  _sGalenicFormBdmId      = string("") ;
  _sGalenicFormLabel      = string("") ;

  _sVigilanceWarning      = string("") ;

  _sMinUcdRangePrice      = string("") ;
  _sMaxUcdRangePrice      = string("") ;
  _sPrice                 = string("") ;
  _sRefundRate            = string("") ;

  _iDispensationPlace     = dispensationUndefined ;
  _iGenericType           = genericUndefined ;

  _bHorsGHS               = boolUndefined ;
  _bPrescribableByMidwife = boolUndefined ;
  _bDrugInSport           = boolUndefined ;
  _bBeCareful             = boolUndefined ;
  _bRetrocession          = boolUndefined ;
  _bSafetyAlert           = boolUndefined ;

  _aLinks.vider() ;

  resetCheckResult() ;
  resetCheckAlerts() ;
}

/**
 * Only reset elements involved in the prescription check process
 */
void
NsSelectableDrug::resetCheckResult()
{
  _sMaxContraIndicationSeverity           = string("") ;
  _iMaxContraIndicationSeverityLevel      = alertUndefined ;
  _sMaxAllergySeverity                    = string("") ;
  _iMaxAllergySeverityLevel               = alertUndefined ;
  _sMaxDrugDrugInteractionsSeverity       = string("") ;
  _iMaxDrugDrugInteractionsSeverityLevel  = alertUndefined ;
  _sMaxPhysicoChemicalSeverity            = string("") ;
  _iMaxPhysicoChemicalSeverityLevel       = alertUndefined ;
  _sMaxPrecautionSeverity                 = string("") ;
  _iMaxPrecautionSeverityLevel            = alertUndefined ;
  _sMaxRedundantInteractionsSeverity      = string("") ;
  _iMaxRedundantInteractionsSeverityLevel = alertUndefined ;
  _sMaxPosologySeverity                   = string("") ;
  _iMaxPosologySeverityLevel              = alertUndefined ;
  _sMaxProcreationSeverity                = string("") ;
  _iMaxProcreationSeverityLevel           = alertUndefined ;

  _aAlertLinks.vider() ;
}

void
NsSelectableDrug::resetCheckAlerts()
{
  _aAlerts.vider() ;
}

/**
 * Get the max alert level from prescription check
 */
NsSelectableDrug::ALERTLEVEL
NsSelectableDrug::getMaxAlertLevel()
{
  ALERTLEVEL iMaxAlertLevel = alertUndefined ;

  if (_iMaxContraIndicationSeverityLevel > iMaxAlertLevel)
    iMaxAlertLevel = _iMaxContraIndicationSeverityLevel ;
  if (_iMaxAllergySeverityLevel > iMaxAlertLevel)
    iMaxAlertLevel = _iMaxAllergySeverityLevel ;
  if (_iMaxDrugDrugInteractionsSeverityLevel > iMaxAlertLevel)
    iMaxAlertLevel = _iMaxDrugDrugInteractionsSeverityLevel ;
  if (_iMaxPhysicoChemicalSeverityLevel > iMaxAlertLevel)
    iMaxAlertLevel = _iMaxPhysicoChemicalSeverityLevel ;
  if (_iMaxPrecautionSeverityLevel > iMaxAlertLevel)
    iMaxAlertLevel = _iMaxPrecautionSeverityLevel ;
  if (_iMaxRedundantInteractionsSeverityLevel > iMaxAlertLevel)
    iMaxAlertLevel = _iMaxRedundantInteractionsSeverityLevel ;
  if (_iMaxPosologySeverityLevel > iMaxAlertLevel)
    iMaxAlertLevel = _iMaxPosologySeverityLevel ;
  if (_iMaxProcreationSeverityLevel > iMaxAlertLevel)
    iMaxAlertLevel = _iMaxProcreationSeverityLevel ;

  return iMaxAlertLevel ;
}

void
NsSelectableDrug::initFrom(const NsSelectableDrug& src)
{
  reset() ;

  _sPrescribedNodeId      = src._sPrescribedNodeId ;

  _sBdmID                 = src._sBdmID ;
  _sBdmVmID               = src._sBdmVmID ;
  _sLabel                 = src._sLabel ;

  _sCIS                   = src._sCIS ;
  _sATC                   = src._sATC ;
  _sMedicaBase            = src._sMedicaBase ;
  _sOnMarketDate          = src._sOnMarketDate ;
  _iAmmType               = src._iAmmType ;
  _sAmmLabel              = src._sAmmLabel ;
  _iMarketStatusType      = src._iMarketStatusType ;
  _iMarketStatusLabel     = src._iMarketStatusLabel ;
  _bWithoutPrescription   = src._bWithoutPrescription ;
  _sCompanyLabel          = src._sCompanyLabel ;
  _sCompanyBdmId          = src._sCompanyBdmId ;
  _iCompanyRoleType       = src._iCompanyRoleType ;

  _sListId                = src._sListId ;
  _sListLabel             = src._sListLabel ;

  _aComponents.vider() ;

  if (false == src._aComponents.empty())
    for (NsDrugComponentConstIter it = src._aComponents.begin() ; src._aComponents.end() != it ; it++)
      _aComponents.push_back(new NsDrugComponent(**it)) ;

  _aIndicators.vider() ;

  if (false == src._aIndicators.empty())
    for (NsXmlEntryConstIter it = src._aIndicators.begin() ; src._aIndicators.end() != it ; it++)
      _aIndicators.push_back(new NsXmlEntry(**it)) ;

  _sVirtualDrugBdmId      = src._sVirtualDrugBdmId ;
  _sVirtualDrugLabel      = src._sVirtualDrugLabel ;

  _sGalenicFormBdmId      = src._sGalenicFormBdmId ;
  _sGalenicFormLabel      = src._sGalenicFormLabel ;

  _sVigilanceWarning      = src._sVigilanceWarning ;

  _sMinUcdRangePrice      = src._sMinUcdRangePrice ;
  _sMaxUcdRangePrice      = src._sMaxUcdRangePrice ;
  _sPrice                 = src._sPrice ;
  _sRefundRate            = src._sRefundRate ;

  _iDispensationPlace     = src._iDispensationPlace ;
  _iGenericType           = src._iGenericType ;

  _bHorsGHS               = src._bHorsGHS ;
  _bPrescribableByMidwife = src._bPrescribableByMidwife ;
  _bDrugInSport           = src._bDrugInSport ;
  _bBeCareful             = src._bBeCareful ;
  _bRetrocession          = src._bRetrocession ;
  _bSafetyAlert           = src._bSafetyAlert ;

  _aLinks.vider() ;

  if (false == src._aLinks.empty())
    for (NsHtmlLinkConstIter it = src._aLinks.begin() ; src._aLinks.end() != it ; it++)
      _aLinks.push_back(new NsHtmlLink(**it)) ;

  _sMaxContraIndicationSeverity           = src._sMaxContraIndicationSeverity ;
  _iMaxContraIndicationSeverityLevel      = src._iMaxContraIndicationSeverityLevel ;
  _sMaxAllergySeverity                    = src._sMaxAllergySeverity ;
  _iMaxAllergySeverityLevel               = src._iMaxAllergySeverityLevel ;
  _sMaxDrugDrugInteractionsSeverity       = src._sMaxDrugDrugInteractionsSeverity ;
  _iMaxDrugDrugInteractionsSeverityLevel  = src._iMaxDrugDrugInteractionsSeverityLevel ;
  _sMaxPhysicoChemicalSeverity            = src._sMaxPhysicoChemicalSeverity ;
  _iMaxPhysicoChemicalSeverityLevel       = src._iMaxPhysicoChemicalSeverityLevel ;
  _sMaxPrecautionSeverity                 = src._sMaxPrecautionSeverity ;
  _iMaxPrecautionSeverityLevel            = src._iMaxPrecautionSeverityLevel ;
  _sMaxRedundantInteractionsSeverity      = src._sMaxRedundantInteractionsSeverity ;
  _iMaxRedundantInteractionsSeverityLevel = src._iMaxRedundantInteractionsSeverityLevel ;
  _sMaxPosologySeverity                   = src._sMaxPosologySeverity ;
  _iMaxPosologySeverityLevel              = src._iMaxPosologySeverityLevel ;
  _sMaxProcreationSeverity                = src._sMaxProcreationSeverity ;
  _iMaxProcreationSeverityLevel           = src._iMaxProcreationSeverityLevel ;

  _aAlertLinks.vider() ;

  if (false == src._aAlertLinks.empty())
    for (NsHtmlLinkConstIter it = src._aAlertLinks.begin() ; src._aAlertLinks.end() != it ; it++)
      _aLinks.push_back(new NsHtmlLink(**it)) ;

  _sCheckPrescriptionIndex = src._sCheckPrescriptionIndex ;
}

void
NsSelectableDrug::setOnMarketDate(string sd)
{
}

string
NsSelectableDrug::getOnMarketDate()
{
  return string("") ;
}

void
NsSelectableDrug::setAmmType(string sd)
{
}

void
NsSelectableDrug::setMarketStatusType(string sM)
{
  string sUpperCapText = pseumaj(sM) ;

  if      (string("AVAILABLE") == sUpperCapText)
    _iMarketStatusType = marketStatusAvailable ;
  else if (string("DELETED") == sUpperCapText)
    _iMarketStatusType = marketStatusDeleted ;
  else if (string("DELETED_ONEYEAR") == sUpperCapText)
    _iMarketStatusType = marketStatusDeletedOneYear ;
  else if (string("PHARMACO") == sUpperCapText)
    _iMarketStatusType = marketStatusPharmaco ;
  else if (string("PHARMACO_ONEYEAR") == sUpperCapText)
    _iMarketStatusType = marketStatusPharmacoOneYear ;
  else
    _iMarketStatusType = marketStatusUndefined ;
}

void
NsSelectableDrug::setCompanyRoleType(string sC)
{
  string sUpperCapText = pseumaj(sC) ;

  if      (string("OWNER") == sUpperCapText)
    _iCompanyRoleType = companyRoleOwner ;
  else
    _iCompanyRoleType = companyRoleUndefined ;
}

void
NsSelectableDrug::setDispensationPlace(string sD)
{
  string sUpperCapText = pseumaj(sD) ;

  if      (string("HOSPITAL") == sUpperCapText)
    _iDispensationPlace = dispensationHospitalOnly ;
  else if (string("PHARMACY") == sUpperCapText)
    _iDispensationPlace = dispensationPharmacy ;
  else if (string("NOT_HOMOGENEOUS") == sUpperCapText)
    _iDispensationPlace = dispensationNotHomogeneous ;
  else
    _iDispensationPlace = dispensationUndefined ;
}

string
NsSelectableDrug::getDispensationPlace(NSContexte* pContexte)
{
  switch(_iDispensationPlace)
  {
    case dispensationPharmacy       : return pContexte->getSuperviseur()->getText("drugInformation", "pharmacy") ;
    case dispensationHospitalOnly   : return pContexte->getSuperviseur()->getText("drugInformation", "hospital") ;
    case dispensationNotHomogeneous : return pContexte->getSuperviseur()->getText("drugInformation", "nonHomogeneous") ;
  }

  return string("?") ;
}

void
NsSelectableDrug::setGenericType(string sG)
{
}

bool
NsSelectableDrug::isNarcotic()
{
  if (_aIndicators.empty())
    return false ;

  for (NsXmlEntryIter it = _aIndicators.begin() ; _aIndicators.end() != it ; it++)
  {
    if (pseumaj((*it)->getField()) == string("INDICATOR"))
    {
      string sValue = (*it)->getValue() ;
      if ((string("15") == sValue) || (string("62") == sValue) || (string("63") == sValue))
        return true ;
    }
  }

  return false ;
}

string
NsSelectableDrug::getGenericType()
{
  return string("") ;
}

void
NsSelectableDrug::getActiveComponents(NsDrugComponentsArray* paComponents) const
{
  if (((NsDrugComponentsArray*) NULL == paComponents) || _aComponents.empty())
    return ;

  for (NsDrugComponentConstIter it = _aComponents.begin() ; _aComponents.end() != it ; it++)
    if ((*it)->getItemType() == NsDrugComponent::itemActivePrinciple)
      paComponents->push_back(new NsDrugComponent(**it)) ;
}

void
NsSelectableDrug::addLink(string sTitle, string sURL, string sType, string sRelType)
{
  _aLinks.push_back(new NsHtmlLink(sTitle, sURL, sType, sRelType)) ;
}

void
NsSelectableDrug::addAlertLink(string sTitle, string sURL, string sType, string sRelType)
{
  _aAlertLinks.push_back(new NsHtmlLink(sTitle, sURL, sType, sRelType)) ;
}

void
NsSelectableDrug::addAlert(NsDrugAlert* pAlert)
{
  if ((NsDrugAlert*) NULL == pAlert)
    return ;

  _aAlerts.push_back(new NsDrugAlert(*pAlert)) ;
}

string
NsSelectableDrug::getLinkFromTitle(const string sTitle) const
{
  if ((string("") == sTitle) || _aLinks.empty())
    return string("") ;

  string sUpperTitle = pseumaj(sTitle) ;

  for (NsHtmlLinkConstIter it = _aLinks.begin() ; _aLinks.end() != it ; it++)
    if (pseumaj((*it)->getTitle()) == sUpperTitle)
      return (*it)->getURL() ;

  return string("") ;
}

NsSelectableDrug::BOOLEANTYPE
NsSelectableDrug::getBool(string sB)
{
  if (IsYes(sB))
    return boolTrue ;
  if (IsNo(sB))
    return boolFalse ;

  return boolUndefined ;
}

string
NsSelectableDrug::getBoolLabel(NsSelectableDrug::BOOLEANTYPE iB) const
{
  switch(iB)
  {
    case boolFalse : return string("non") ;
    case boolTrue  : return string("oui") ;
  }

  return string("?") ;
}

int
NsSelectableDrug::operator==(const NsSelectableDrug& o)
{
  if ((_sBdmID == o._sBdmID) &&
      (_sCIS   == o._sCIS)   &&
      (_sLabel == o._sLabel) &&
      (_sPrice == o._sPrice))
    return 1 ;

  return 0 ;
}

/**
 * NSBdmEntry
 *
 * Information from drug database that can be displayed in a generic list
 */
NSBdmEntry::NSBdmEntry()
{
  _sID    = string("") ;
  _sCode  = string("") ;
  _sLabel = string("") ;
}

NSBdmEntry::NSBdmEntry(const string sI, const string sC, const string sL)
{
  _sID    = sI ;
  _sCode  = sC ;
  _sLabel = sL ;
}

NSBdmEntry::NSBdmEntry(const NSBdmEntry& rv)
{
  _sID    = rv._sID ;
  _sCode  = rv._sCode ;
  _sLabel = rv._sLabel ;
}

NSBdmEntry::~NSBdmEntry()
{
}

NSBdmEntry&
NSBdmEntry::operator=(const NSBdmEntry& src)
{
  if (&src == this)
		return *this ;

  _sID    = src._sID ;
  _sCode  = src._sCode ;
  _sLabel = src._sLabel ;

  return *this ;
}

int
NSBdmEntry::operator==(const NSBdmEntry& o)
{
  if ((_sID    == o._sID)    &&
      (_sCode  == o._sCode)  &&
      (_sLabel == o._sLabel))
    return 1 ;

  return 0 ;
}

/**
 * NsHtmlLink
 *
 * URL to an HTML page that give complementary information
 */
NsHtmlLink::NsHtmlLink()
{
  _sTitle   = string("") ;
  _sURL     = string("") ;
  _sType    = string("") ;
  _iRelType = relUndefined ;
}

NsHtmlLink::NsHtmlLink(string sTitle, string sURL, string sType, string sRelType)
{
  _sTitle = sTitle ;
  _sURL   = sURL ;
  _sType  = sType ;
  setRelTitle(sRelType) ;
}

NsHtmlLink::NsHtmlLink(string sTitle, string sURL, string sType, RELTYPE iRelType)
{
  _sTitle   = sTitle ;
  _sURL     = sURL ;
  _sType    = sType ;
  _iRelType = iRelType ;
}

NsHtmlLink::NsHtmlLink(const NsHtmlLink& rv)
{
  _sTitle   = rv._sTitle ;
  _sURL     = rv._sURL ;
  _sType    = rv._sType ;
  _iRelType = rv._iRelType ;
}

NsHtmlLink&
NsHtmlLink::operator=(const NsHtmlLink& src)
{
  if (&src == this)
		return *this ;

  _sTitle   = src._sTitle ;
  _sURL     = src._sURL ;
  _sType    = src._sType ;
  _iRelType = src._iRelType ;

  return *this ;
}

int
NsHtmlLink::operator==(const NsHtmlLink& o)
{
  if ((_sTitle   == o._sTitle) &&
      (_sURL     == o._sURL)   &&
      (_sType    == o._sType)  &&
      (_iRelType == o._iRelType))
    return 1 ;

  return 0 ;
}

void
NsHtmlLink::setRelTitle(string sT)
{
  string sUpperCapText = pseumaj(sT) ;

  if      (string("RELATED") == sUpperCapText)
    _iRelType = relRelated ;
  else if (string("ALTENATE") == sUpperCapText)
    _iRelType = relAlternate ;
  else if (string("INLINE") == sUpperCapText)
    _iRelType = relInline ;
  else
    _iRelType = relUndefined ;
}

/**
 * NsDrugComponent
 *
 * A drug component (active principle or not)
 */
NsDrugComponent::NsDrugComponent()
{
  _sBdmID    = string("") ;
  _sLabel    = string("") ;
  _iItemType = itemUndefined ;
  _iRanking  = -1 ;
}

NsDrugComponent::NsDrugComponent(string sBdmID, string sLabel)
{
  _sBdmID    = sBdmID ;
  _sLabel    = sLabel ;
  _iItemType = itemUndefined ;
  _iRanking  = -1 ;
}

NsDrugComponent::NsDrugComponent(const NsDrugComponent& rv)
{
  _sBdmID    = rv._sBdmID ;
  _sLabel    = rv._sLabel ;
  _iItemType = rv._iItemType ;
  _iRanking  = rv._iRanking ;
}

NsDrugComponent&
NsDrugComponent::operator=(const NsDrugComponent& src)
{
  if (&src == this)
		return *this ;

  _sBdmID    = src._sBdmID ;
  _sLabel    = src._sLabel ;
  _iItemType = src._iItemType ;
  _iRanking  = src._iRanking ;

  return *this ;
}

int
NsDrugComponent::operator==(const NsDrugComponent& o)
{
  if ((_sBdmID == o._sBdmID) &&
      (_sLabel == o._sLabel))
    return 1 ;

  return 0 ;
}

void
NsDrugComponent::setItemType(string sT)
{
  if (string("") == sT)
  {
    _iItemType = itemUndefined ;
    return ;
  }

  string sMajTitle = pseumaj(sT) ;

  if      (string("ACTIVE_PRINCIPLE") == sMajTitle)
    _iItemType = itemActivePrinciple ;
  else if (string("ACTIVE_EXCIPIENT") == sMajTitle)
    _iItemType = itemActiveExcipient ;
  else if (string("EXCIPIENT") == sMajTitle)
    _iItemType = itemExcipient ;
  else
    _iItemType = itemUndefined ;
}

void
NsDrugComponent::setRanking(string sR)
{
  if (string("") == sR)
  {
    _iRanking = -1 ;
    return ;
  }

  _iRanking = StringToInt(sR) ;
}

/**
 * NsDrugAlert
 *
 * Alert from tha prescription check process
 */
NsDrugAlert::NsDrugAlert()
{
  reset() ;
}

NsDrugAlert::NsDrugAlert(const NsDrugAlert& rv)
{
  _iType        = rv._iType ;
  _sType        = rv._sType ;
  _sTitle       = rv._sTitle ;
  _sID          = rv._sID ;
  _sContent     = rv._sContent ;
  _sContentType = rv._sContentType ;
  _iSeverity    = rv._iSeverity ;
  _sDetail      = rv._sDetail ;
  _sDetailType  = rv._sDetailType ;

  _aLinks       = rv._aLinks ;
}

NsDrugAlert&
NsDrugAlert::operator=(const NsDrugAlert& src)
{
  if (&src == this)
		return *this ;

  _iType        = src._iType ;
  _sType        = src._sType ;
  _sTitle       = src._sTitle ;
  _sID          = src._sID ;
  _sContent     = src._sContent ;
  _sContentType = src._sContentType ;
  _iSeverity    = src._iSeverity ;
  _sDetail      = src._sDetail ;
  _sDetailType  = src._sDetailType ;

  _aLinks       = src._aLinks ;

  return *this ;
}

int
NsDrugAlert::operator==(const NsDrugAlert& o)
{
  if (_sID == o._sID)
    return 1 ;
  return 0 ;
}

void
NsDrugAlert::reset()
{
  _iType        = typeUndefined ;
  _sType        = string("") ;
  _sTitle       = string("") ;
  _sID          = string("") ;
  _sContent     = string("") ;
  _sContentType = string("") ;
  _iSeverity    = severityUndefined ;
  _sDetail      = string("") ;
  _sDetailType  = string("") ;

  _aLinks.vider() ;
}

/**
 * Initialize the type
 */
void
NsDrugAlert::initializeType(const string sType)
{
  _sType = sType ;
  _iType = typeUndefined ;

  if (string("") == sType)
    return ;

  string sMajType = pseumaj(sType) ;

  if      (string("WARNING")                      == sMajType)
    _iType = typeWarning ;
  else if (string("PRECAUTION")                   == sMajType)
    _iType = typePrecaution ;
  else if (string("INDICATOR")                    == sMajType)
    _iType = typeIndicator ;
  else if (string("SIDE_EFFECT")                  == sMajType)
    _iType = typeSideEffect ;
  else if (string("POSOLOGY")                     == sMajType)
    _iType = typePosology ;
  else if (string("REDUNDANT_ACTIVE_INGREDIENT")  == sMajType)
    _iType = typeActiveIngredientRedundancy ;
  else if (string("PHYSICO-CHEMICAL-INTERACTION") == sMajType)
    _iType = typePhysicoChemicalInteraction ;
}

void
NsDrugAlert::initializeSeverity(const string sSeverity)
{
  _iSeverity = severityUndefined ;

  if (string("") == sSeverity)
    return ;

  string sMajSeverity = pseumaj(sSeverity) ;

  if      (string("INFO")    == sMajSeverity)
    _iSeverity = severityInfo ;
  else if (string("LEVEL_1") == sMajSeverity)
    _iSeverity = severityLevel1 ;
  else if (string("LEVEL_2") == sMajSeverity)
    _iSeverity = severityLevel2 ;
  else if (string("LEVEL_3") == sMajSeverity)
    _iSeverity = severityLevel3 ;
  else if (string("LEVEL_4") == sMajSeverity)
    _iSeverity = severityLevel4 ;
}

void
NsDrugAlert::addLink(string sTitle, string sURL, string sType, string sRelType)
{
  _aLinks.push_back(new NsHtmlLink(sTitle, sURL, sType, sRelType)) ;
}

/**
 * NsXmlBlock
 *
 * A set of tags and links that represents an information block
 */
NsXmlBlock::NsXmlBlock()
{
  _sTitle    = string("") ;
  _sUniqueId = string("") ;
}

NsXmlBlock::NsXmlBlock(const NsXmlBlock& rv)
{
  initFromBlock(rv) ;
}

NsXmlBlock&
NsXmlBlock::operator=(const NsXmlBlock& src)
{
  if (&src == this)
		return *this ;

  initFromBlock(src) ;

  return *this ;
}

void
NsXmlBlock::initFromBlock(const NsXmlBlock& rv)
{
  _sTitle    = rv._sTitle ;
  _sUniqueId = rv._sUniqueId ;

  if (false == rv._aEntries.empty())
    for (NsXmlEntryConstIter it = rv._aEntries.begin() ; rv._aEntries.end() != it ; it++)
      _aEntries.push_back(new NsXmlEntry(**it)) ;

  if (false == rv._aLinks.empty())
    for (NsHtmlLinkConstIter it = rv._aLinks.begin() ; rv._aLinks.end() != it ; it++)
      _aLinks.push_back(new NsHtmlLink(**it)) ;
}

int
NsXmlBlock::operator==(const NsXmlBlock& o)
{
  if (_sUniqueId == o._sUniqueId)
    return 1 ;

  return 0 ;
}

void
NsXmlBlock::addEntry(const NsXmlEntry &Entry)
{
  _aEntries.push_back(new NsXmlEntry(Entry)) ;
}

void
NsXmlBlock::addLink(const NsHtmlLink &Link)
{
  _aLinks.push_back(new NsHtmlLink(Link)) ;
}

/**
 * NsXmlEntry
 *
 * An entry is a tag, with its name, text and attributes
 */
NsXmlEntry::NsXmlEntry()
{
}

NsXmlEntry::NsXmlEntry(string sField, string sValue)
{
  _entry = NsXmlFieldValuePair(sField, sValue) ;
}

NsXmlEntry::NsXmlEntry(const NsXmlEntry& rv)
{
  initFromXmlEntry(rv) ;
}

NsXmlEntry&
NsXmlEntry::operator=(const NsXmlEntry& src)
{
  if (&src == this)
		return *this ;

  initFromXmlEntry(src) ;

  return *this ;
}

int
NsXmlEntry::operator==(const NsXmlEntry& o)
{
  if (_entry == o._entry)
    return 1 ;
  return 0 ;
}

void
NsXmlEntry::addAttribute(string sField, string sValue)
{
  _aAtributes.push_back(new NsXmlFieldValuePair(sField, sValue)) ;
}

void
NsXmlEntry::initFromXmlEntry(const NsXmlEntry& rv)
{
  _entry = rv._entry ;

  if (false == rv._aAtributes.empty())
    for (NsXmlFieldValuePairConstIter it = rv._aAtributes.begin() ; rv._aAtributes.end() != it ; it++)
      _aAtributes.push_back(new NsXmlFieldValuePair(**it)) ;
}

/**
 * NsXmlFieldValuePair
 *
 * An NsXmlFieldValuePair is generic value/pair information, either tag/text or
 * attribute name/value
 */

NsXmlFieldValuePair::NsXmlFieldValuePair()
{
  _sField = string("") ;
  _sValue = string("") ;
}

NsXmlFieldValuePair::NsXmlFieldValuePair(string sField, string sValue)
{
  _sField = sField ;
  _sValue = sValue ;
}

NsXmlFieldValuePair::NsXmlFieldValuePair(const NsXmlFieldValuePair& rv)
{
  _sField = rv._sField ;
  _sValue = rv._sValue ;
}

NsXmlFieldValuePair&
NsXmlFieldValuePair::operator=(const NsXmlFieldValuePair& src)
{
  if (&src == this)
		return *this ;

  _sField = src._sField ;
  _sValue = src._sValue ;

  return *this ;
}

int
NsXmlFieldValuePair::operator==(const NsXmlFieldValuePair& o)
{
  if ((_sField == o._sField) &&
      (_sValue == o._sValue))
    return 1 ;
  return 0 ;
}

/**
 *  Information to be sent to BAM to check prescription
 */
NSPrescriptionCheckingMessage::NSPrescriptionCheckingMessage()
{
}

NSPrescriptionCheckingMessage::NSPrescriptionCheckingMessage(const NSPrescriptionCheckingMessage& rv)
{
  _patientInformation = rv._patientInformation ;
  _aPrescriptionLines = rv._aPrescriptionLines ;
}

NSPrescriptionCheckingMessage::~NSPrescriptionCheckingMessage()
{
}

string
NSPrescriptionCheckingMessage::getPatientBlock()
{
  string sMessageToSend = string("  <patient>") + string(NewLineUnix) ;

  // Basic information
  //
  NSBdmEntryArray* pBasicInfo = _patientInformation.getBasicInformation() ;
  if (false == pBasicInfo->empty())
    for (NSBdmEntryConstIter it = pBasicInfo->begin() ; pBasicInfo->end() != it ; it++)
      sMessageToSend += string("    ") + getEntryBlock(*it) ;

  // Allergies
  //
  NSBdmEntryArray* pAllergies = _patientInformation.getAllergies() ;
  if (false == pAllergies->empty())
  {
    sMessageToSend += string("    <allergies>") + string(NewLineUnix) ;
    for (NSBdmEntryConstIter it = pAllergies->begin() ; pAllergies->end() != it ; it++)
      sMessageToSend += string("      ") + getEntryBlock(*it) ;
    sMessageToSend += string("    </allergies>") + string(NewLineUnix) ;
  }

  // Molecules
  //
  NSBdmEntryArray* pMolecules = _patientInformation.getMolecules() ;
  if (false == pMolecules->empty())
  {
    sMessageToSend += string("    <molecules>") + string(NewLineUnix) ;
    for (NSBdmEntryConstIter it = pMolecules->begin() ; pMolecules->end() != it ; it++)
      sMessageToSend += string("      ") + getEntryBlock(*it) ;
    sMessageToSend += string("    </molecules>") + string(NewLineUnix) ;
  }

  // Pathologies
  //
  NSBdmEntryArray* pPathologies = _patientInformation.getPathologies() ;
  if (false == pPathologies->empty())
  {
    sMessageToSend += string("    <pathologies>") + string(NewLineUnix) ;
    for (NSBdmEntryConstIter it = pPathologies->begin() ; pPathologies->end() != it ; it++)
      sMessageToSend += string("      ") + getEntryBlock(*it) ;
    sMessageToSend += string("    </pathologies>") + string(NewLineUnix) ;
  }

  sMessageToSend += string("  </patient>") + string(NewLineUnix) ;

  return sMessageToSend ;
}

string
NSPrescriptionCheckingMessage::getPrescriptionBlock()
{
  if (_aPrescriptionLines.empty())
    return string("") ;

  string sMessageToSend = string("  <prescription-lines>") + string(NewLineUnix) ;

  for (NSPresLineIter it = _aPrescriptionLines.begin() ; _aPrescriptionLines.end() != it ; it++)
    sMessageToSend += (*it)->getLineBlock() ;

  sMessageToSend += string("  </prescription-lines>") + string(NewLineUnix) ;

  return sMessageToSend ;
}

string
NSPrescriptionCheckingMessage::getEntryBlock(const NSBdmEntry* pEntry) const
{
  if ((NSBdmEntry*) NULL == pEntry)
    return string("") ;

  return string("<") + pEntry->getID() + string(">") +
         pEntry->getCode() +
         string("</") + pEntry->getID() + string(">") +
         string(NewLineUnix) ;
}

NSPrescriptionCheckingMessage&
NSPrescriptionCheckingMessage::operator=(const NSPrescriptionCheckingMessage& src)
{
  if (&src == this)
		return *this ;

  _patientInformation = src._patientInformation ;
  _aPrescriptionLines = src._aPrescriptionLines ;

  return *this ;
}

int
NSPrescriptionCheckingMessage::operator==(const NSPrescriptionCheckingMessage& o)
{
  if ((_patientInformation == o._patientInformation) &&
      (_aPrescriptionLines == o._aPrescriptionLines))
    return 1 ;
  return 0 ;
}

/**
 *  Patient description subset of information to be sent to BAM to check prescription
 */
NSPresCheckPatientIntformation::NSPresCheckPatientIntformation()
{
}

NSPresCheckPatientIntformation::NSPresCheckPatientIntformation(const NSPresCheckPatientIntformation& rv)
{
  _aBasicInformation = rv._aBasicInformation ;
  _aAllergies        = rv._aAllergies ;
  _aMolecules        = rv._aMolecules ;
  _aPathologies      = rv._aPathologies ;
}

NSPresCheckPatientIntformation::~NSPresCheckPatientIntformation()
{
}

NSPresCheckPatientIntformation&
NSPresCheckPatientIntformation::operator=(const NSPresCheckPatientIntformation& src)
{
  if (&src == this)
		return *this ;

  _aBasicInformation = src._aBasicInformation ;
  _aAllergies        = src._aAllergies ;
  _aMolecules        = src._aMolecules ;
  _aPathologies      = src._aPathologies ;

  return *this ;
}

int
NSPresCheckPatientIntformation::operator==(const NSPresCheckPatientIntformation& o)
{
  if ((_aBasicInformation == o._aBasicInformation) &&
      (_aAllergies        == o._aAllergies)        &&
      (_aMolecules        == o._aMolecules)        &&
      (_aPathologies      == o._aPathologies))
    return 1 ;

  return 0 ;
}

/**
 *  Line of prescription to be sent to BAM to check prescription
 */
NSPresCheckPrescriptionLine::NSPresCheckPrescriptionLine()
{
  reset() ;
}

NSPresCheckPrescriptionLine::NSPresCheckPrescriptionLine(const NSPresCheckPrescriptionLine& rv)
{
  initializeFrom(rv) ;
}

NSPresCheckPrescriptionLine::~NSPresCheckPrescriptionLine()
{
}

string
NSPresCheckPrescriptionLine::getLineBlock() const
{
  string sMessageToSend = string("    <prescription-line>") + string(NewLineUnix) ;

  sMessageToSend += getEntryBlock(string("drug"),          _sDrug) ;
  sMessageToSend += getEntryBlock(string("drugId"),        _sDrugId) ;
  sMessageToSend += getEntryBlock(string("drugType"),      _sDrugType) ;
  sMessageToSend += getEntryBlock(string("dose"),          _sDose) ;
  sMessageToSend += getEntryBlock(string("unitId"),        _sDoseUnit) ;
  sMessageToSend += getEntryBlock(string("duration"),      _sDuration) ;
  sMessageToSend += getEntryBlock(string("durationType"),  _sDurationType) ;
  sMessageToSend += getEntryBlock(string("frequencyType"), _sFrequencyType) ;

  if (false == _aRoutes.empty())
  {
    sMessageToSend += string("      <routes>") + string(NewLineUnix) ;

    for (vector<string>::const_iterator it = _aRoutes.begin() ; _aRoutes.end() != it ; it++)
      sMessageToSend += string("  ") + getEntryBlock(string("route"), *it) ;

    sMessageToSend += string("      </routes>") + string(NewLineUnix) ;
  }

  sMessageToSend += string("    </prescription-line>") + string(NewLineUnix) ;

  return sMessageToSend ;
}

string
NSPresCheckPrescriptionLine::getEntryBlock(const string sTag, const string sValue) const
{
  if ((string("") == sTag) || (string("") == sValue))
    return string("") ;

  return string("      <") + sTag + string(">") + sValue +
              string("</") + sTag + string(">") + string(NewLineUnix) ;
}

string
NSPresCheckPrescriptionLine::getDoseUnitAsText(NSContexte* pContexte)
{
  NSBdmDriver* pDriver = pContexte->getBdmDriver() ;
  if ((NSBdmDriver*) NULL == pDriver)
    return string("") ;

  NSBdmEntry* pBdmEntry = pDriver->getUnitForId(_sDoseUnit) ;
  if ((NSBdmEntry*) NULL == pBdmEntry)
    return string("") ;

  return pBdmEntry->getLabel() ;
}

string
NSPresCheckPrescriptionLine::getDurationTypeAsText(NSContexte* pContexte)
{
  return pContexte->getSuperviseur()->getText("drugAlertWindow", _sDurationType) ;
}

string
NSPresCheckPrescriptionLine::getFrequencyTypeAsText(NSContexte* pContexte)
{
  return pContexte->getSuperviseur()->getText("drugAlertWindow", _sFrequencyType) ;
}

void
NSPresCheckPrescriptionLine::initializeFrom(const NSPresCheckPrescriptionLine& rv)
{
  _sDrugLabel     = rv._sDrugLabel ;
  _sDrug          = rv._sDrug ;
  _sDrugId        = rv._sDrugId ;
  _sDrugType      = rv._sDrugType ;
  _sDose          = rv._sDose ;
  _sDoseUnit      = rv._sDoseUnit ;
  _sDuration      = rv._sDuration ;
  _sDurationType  = rv._sDurationType ;
  _sFrequencyType = rv._sFrequencyType ;

  if (false == _aRoutes.empty())
    for (vector<string>::iterator it = _aRoutes.begin() ; _aRoutes.end() != it ; it++)
      _aRoutes.push_back(string(*it)) ;
}

void
NSPresCheckPrescriptionLine::reset()
{
  _sDrugLabel     = string("") ;
  _sDrug          = string("") ;
  _sDrugId        = string("") ;
  _sDrugType      = string("") ;
  _sDose          = string("") ;
  _sDoseUnit      = string("") ;
  _sDuration      = string("") ;
  _sDurationType  = string("") ;
  _sFrequencyType = string("") ;

  if (false == _aRoutes.empty())
    _aRoutes.clear() ;
}

NSPresCheckPrescriptionLine&
NSPresCheckPrescriptionLine::operator=(const NSPresCheckPrescriptionLine& src)
{
  if (&src == this)
		return *this ;

  reset() ;
  initializeFrom(src) ;

  return *this ;
}

int
NSPresCheckPrescriptionLine::operator==(const NSPresCheckPrescriptionLine& o)
{
  if ((_sDrug          == o._sDrug)         &&
      (_sDrugId        == o._sDrugId)       &&
      (_sDrugType      == o._sDrugType)     &&
      (_sDose          == o._sDose)         &&
      (_sDoseUnit      == o._sDoseUnit)     &&
      (_sDuration      == o._sDuration)     &&
      (_sDurationType  == o._sDurationType) &&
      (_sFrequencyType == o._sFrequencyType))
    return 1 ;

  return 0 ;
}


