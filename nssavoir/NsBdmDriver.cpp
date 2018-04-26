//---------------------------------------------------------------------------
//
//  		                        BDM query objects
//
//---------------------------------------------------------------------------

#include <stdlib.h>

#include "nssavoir/NsBdmDriver.h"
#include "nautilus/nssuper.h"
#include "curl/nsRest.h"
#include "nsutil/tinyxml.h"

bool
BdmEntrySortByLabelInf(const NSBdmEntry *pObj1, const NSBdmEntry *pObj2)
{
  if (((NSBdmEntry*) NULL == pObj1) || ((NSBdmEntry*) NULL == pObj2))
    return false ;

	return (pObj1->getLabel() < pObj2->getLabel()) ;
}

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
 * Get the BAM Identifier from the CIS code
 */
string
NSBdmDriver::getBamIdFromCisCode(const string sCisCode)
{
  if (string("") == sCisCode)
    return string("") ;

  vector<Var> aSpecificVars ;
  aSpecificVars.push_back(Var(string("code"),   sCisCode)) ;
  aSpecificVars.push_back(Var(string("filter"), string("PRODUCT"))) ;

  NsSelectableDrugArray aDrugsList ;

  string sSeed = string("") ;

  getDrugsForSeed(&aDrugsList, &sSeed, string("search"), &aSpecificVars) ;

  if (aDrugsList.size() != 1)
    return string("") ;

  return aDrugsList[0]->getBdmID() ;
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
  if (((NsXmlBlocksArray*) NULL == pBlocks) || (string("") == sUrl))
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

  if (aDrugsList.size() != 1)
    return false ;

  *pDrugInformation = *(aDrugsList[0]) ;

  return true ;
}

/**
 * Fills a list from the content of a table that fits a given seed
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
  }

  _isBusy = false ;
}

void
NSBdmDriver::getIndicationsForSeed(NSBdmEntryArray* paListeArray, string* pSeed)
{
  getElementsForSeed(paListeArray, pSeed, string("indications"),
                                          string("vidal:id"),
                                          string("vidal:name"),
                                          (vector<Var>*) 0) ;
}

void
NSBdmDriver::getSubstancesForSeed(NSBdmEntryArray* paListeArray, string* pSeed)
{
  getElementsForSeed(paListeArray, pSeed, string("molecules/active-substances"),
                                          string("vidal:id"),
                                          string("vidal:fullName"),
                                          (vector<Var>*) 0) ;
}

void
NSBdmDriver::getAtcCodesForSeed(NSBdmEntryArray* paListeArray, string* pSeed)
{
  if (((NSBdmEntryArray*) NULL == paListeArray) || ((string*) NULL == pSeed))
    return ;

  vector<Var> aSpecificVars ;
  aSpecificVars.push_back(Var(string("filter"), string("atc_classification"))) ;

  getElementsForSeed(paListeArray, pSeed, string("search"),
                                          string("id"),
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
}

void
NSBdmDriver::getElementsForSeed(NSBdmEntryArray* paListeArray, string* pSeed, const string sQuery, const string sIdTag, const string sLabelTag, const vector<Var>* paSpecificVars)
{
  if ((NSBdmEntryArray*) NULL == paListeArray)
    return ;

  paListeArray->vider() ;

  if (((string*) NULL == pSeed) || (string("") == *pSeed))
    return ;

  if (strlen(pSeed->c_str()) < 3)
    return ;

  // Preparing request
  //
  vector<Var> vars ;
  initVars(&vars) ;
  vars.push_back(Var(string("q"), *pSeed)) ;

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

    sRequest = fillArrayOfBdmEntriesFromResults(paListeArray, &sResult, sIdTag, sLabelTag) ;

    if (string("") == sRequest)
      bExistNextPage = false ;
    else
      initVars(&vars) ;
  }

  if (false == paListeArray->empty())
    sort(paListeArray->begin(), paListeArray->end(), BdmEntrySortByLabelInf) ;
}

string
NSBdmDriver::fillArrayOfBdmEntriesFromResults(NSBdmEntryArray* paListeArray, const string* psResult, const string sIdTag, const string sLabelTag)
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
    string sID    = getTextForTag(pEntryElement, sIdTag) ;
    string sLabel = getTextForTag(pEntryElement, sLabelTag) ;

    if ((string("") != sID) && (string("") != sLabel))
      paListeArray->push_back(new NSBdmEntry(sID, sLabel)) ;

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

  if (string("") == sQuery)
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
      NsSelectableDrug* pDrug = new NsSelectableDrug(sVidalID, sCodeCis, sLabel, sMinPrice + string(" - ") + sMaxPrice) ;

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
 *  Function that get a full html document from a partial url
 */
string
NSBdmDriver::getFullUrl(const string sPartialUrl)
{
  if (string("") == sPartialUrl)
    return string("") ;

  string sFullUrl = _pRest->build_uri(sPartialUrl) ;

  vector<Var> vars ;
  initVars(&vars) ;

  return _pRest->add_vars_to_uri(sFullUrl, vars) ;
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

  string sAttribute = pIdNode->ToElement()->Attribute(sAttributeName) ;

  return FromUTF8ToISO(sAttribute) ;
}

/**
 * Convert from utf8 to local iso-8859-15 equivalent charset
 */
string
NSBdmDriver::FromUTF8ToISO(const string sUTF8)
{
	// Shortcut if empty
	if (string("") == sUTF8)
		return string("") ;

  size_t iUTF8Len = strlen(sUTF8.c_str()) ;

  char* pIn  = new char[iUTF8Len + 1] ;
  strcpy(pIn, sUTF8.c_str()) ;

  char* pOut = new char[iUTF8Len + 1] ;

	const char* p = pIn ;
	// const char* q = pIn + iUTF8Len ;

	unsigned long charlen = 0 ;
	wchar_t       wc      = 0 ;

  int iCursor = 0 ;
	// while (p < q)
  for (size_t i = 0 ; i < iUTF8Len ; i++)
	{
		unsigned char mask = 0x3f;

    // If we are not already processing a multichar UTF8 char
		if (charlen == 0)
		{
			// Determine length of utf8 encoded wchar_t
			if ((*p & 0xf0 ) == 0xe0)
			{
				charlen = 3;
				mask = 0x0f;
			}
			else if ((*p & 0xe0 ) == 0xc0)
			{
				charlen = 2;
				mask = 0x1f;
			}
			else
			{
				charlen = 1;
				mask = 0x7f;
			}

			// Reset char
			wc = 0;
		}

		// Convert the byte
		wc <<= 6;
		wc |= (*p & mask);

		// Bump ptr
		p++;

		// Reduce byte remaining count and write it out if done
		if (0 == --charlen)
		{
			// Special for Euro
			if (wc == 0x20AC)
				// Use iso-8859-15 code for Euro
				pOut[iCursor++] = 0xA4 ;
			else if (wc > 0x00FF)
				pOut[iCursor++] = '?' ;
			else
				pOut[iCursor++] = wc & 0x00FF ;
		}
	}

  pOut[iCursor] = '\0' ;

  string sOut = string(pOut) ;

  delete[] pOut ;
  delete[] pIn ;

  return sOut ;
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

NsSelectableDrug&
NsSelectableDrug::operator=(const NsSelectableDrug& src)
{
  if (&src == this)
		return *this ;

  initFrom(src) ;

  return *this ;
}

void
NsSelectableDrug::reset()
{
  _sBdmID                 = string("") ;
  _sLabel                 = string("") ;

  _sCIS                   = string("") ;
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

  _aActivePrinciples.vider() ;

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
}

void
NsSelectableDrug::initFrom(const NsSelectableDrug& src)
{
  reset() ;

  _sBdmID                 = src._sBdmID ;
  _sLabel                 = src._sLabel ;

  _sCIS                   = src._sCIS ;
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

  if (false == src._aActivePrinciples.empty())
    for (NsDrugComponentConstIter it = src._aActivePrinciples.begin() ; src._aActivePrinciples.end() != it ; it++)
      _aActivePrinciples.push_back(new NsDrugComponent(**it)) ;

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

  if (false == src._aLinks.empty())
    for (NsHtmlLinkConstIter it = src._aLinks.begin() ; src._aLinks.end() != it ; it++)
      _aLinks.push_back(new NsHtmlLink(**it)) ;
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

string
NsSelectableDrug::getGenericType()
{
  return string("") ;
}

void
NsSelectableDrug::addLink(string sTitle, string sURL, string sType, string sRelType)
{
  _aLinks.push_back(new NsHtmlLink(sTitle, sURL, sType, sRelType)) ;
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
  _sLabel = string("") ;
}

NSBdmEntry::NSBdmEntry(const string sI, const string sL)
{
  _sID    = sI ;
  _sLabel = sL ;
}

NSBdmEntry::NSBdmEntry(const NSBdmEntry& rv)
{
  _sID    = rv._sID ;
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
  _sLabel = src._sLabel ;

  return *this ;
}

int
NSBdmEntry::operator==(const NSBdmEntry& o)
{
  if ((_sID    == o._sID)    &&
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
  _sBdmID = string("") ;
  _sLabel = string("") ;
}

NsDrugComponent::NsDrugComponent(string sBdmID, string sLabel)
{
  _sBdmID = sBdmID ;
  _sLabel = sLabel ;
}

NsDrugComponent::NsDrugComponent(const NsDrugComponent& rv)
{
  _sBdmID = rv._sBdmID ;
  _sLabel = rv._sLabel ;
}

NsDrugComponent&
NsDrugComponent::operator=(const NsDrugComponent& src)
{
  if (&src == this)
		return *this ;

  _sBdmID = src._sBdmID ;
  _sLabel = src._sLabel ;

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

