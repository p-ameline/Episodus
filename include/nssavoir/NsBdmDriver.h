//---------------------------------------------------------------------------
//     NsBdmDriver.H
//
//  PA April 2018
//---------------------------------------------------------------------------
#ifndef __NSBDMDRIVER_H
#define __NSBDMDRIVER_H

// #include "nssavoir\nspathor.h"
//
#include <string>
#include <vector>

class ChoixBdmDialog ;
class TiXmlElement ;
class TiXmlNode ;
class NSPatPathoArray ;

using namespace std ;

#include "partage/nsglobal.h"
#include "partage/NTArray.h"
#include "curl/nsRest.h"

#ifndef __linux__
#if defined(_DANSLEXIDLL)
	#define _CLASSELEXI __export
#else
	#define _CLASSELEXI __import
#endif
#endif

/**
 * An NsXmlFieldValuePair is generic value/pair information, either tag/text or
 * attribute name/value
 */
class _CLASSELEXI NsXmlFieldValuePair
{
  public:

    NsXmlFieldValuePair() ;
    NsXmlFieldValuePair(string sField, string sValue) ;

    NsXmlFieldValuePair(const NsXmlFieldValuePair& rv) ;

    NsXmlFieldValuePair& operator=(const NsXmlFieldValuePair& src) ;
    int                  operator==(const NsXmlFieldValuePair& o) ;

    void    setField(string sF)  { _sField = sF ; }
    void    setValue(string sV)    { _sValue = sV ; }

    string  getField() const { return _sField ; }
    string  getValue() const { return _sValue ; }

  protected:

    string  _sField ;
    string  _sValue ;
} ;

typedef vector<NsXmlFieldValuePair*>                 NsXmlFieldValuePairsVector ;
typedef NsXmlFieldValuePairsVector::iterator         NsXmlFieldValuePairIter ;
typedef NsXmlFieldValuePairsVector::const_iterator   NsXmlFieldValuePairConstIter ;
typedef NsXmlFieldValuePairsVector::reverse_iterator NsXmlFieldValuePairReverseIter ;
typedef NTArray<NsXmlFieldValuePair>                 NsXmlFieldValuePairsArray ;

/**
 * An entry is a tag, with its name, text and attributes
 */
class _CLASSELEXI NsXmlEntry
{
  public:

    NsXmlEntry() ;
    NsXmlEntry(string sField, string sValue) ;

    NsXmlEntry(const NsXmlEntry& rv) ;

    NsXmlEntry& operator=(const NsXmlEntry& src) ;
    int         operator==(const NsXmlEntry& o) ;

    void    setField(string sF)  { _entry.setField(sF) ; }
    void    setValue(string sV)  { _entry.setValue(sV) ; }

    string  getField() const { return _entry.getField() ; }
    string  getValue() const { return _entry.getValue() ; }

    NsXmlFieldValuePairsArray* getAttributes() { return &_aAtributes ; }

    void    addAttribute(string sField, string sValue) ;

  protected:

    NsXmlFieldValuePair       _entry ;
    NsXmlFieldValuePairsArray _aAtributes ;

    void initFromXmlEntry(const NsXmlEntry& rv) ;
} ;

typedef vector<NsXmlEntry*>                  NsXmlEntriesVector ;
typedef NsXmlEntriesVector::iterator         NsXmlEntryIter ;
typedef NsXmlEntriesVector::const_iterator   NsXmlEntryConstIter ;
typedef NsXmlEntriesVector::reverse_iterator NsXmlEntryReverseIter ;
typedef NTArray<NsXmlEntry>                  NsXmlEntriesArray ;

/**
 * A link is an URL to an HTML page that give complementary information
 */
class _CLASSELEXI NsHtmlLink
{
  public:

    enum RELTYPE { relUndefined = 0, relAlternate, relRelated, relInline } ;

    NsHtmlLink() ;
    NsHtmlLink(string sTitle, string sURL, string sType, string sRelType) ;
    NsHtmlLink(string sTitle, string sURL, string sType, RELTYPE iRelType) ;

    NsHtmlLink(const NsHtmlLink& rv) ;

    NsHtmlLink& operator=(const NsHtmlLink& src) ;
    int         operator==(const NsHtmlLink& o) ;

    void    setTitle(string sT)  { _sTitle = sT ; }
    void    setURL(string sU)    { _sURL   = sU ; }
    void    setType(string sT)   { _sType  = sT ; }
    void    setRelTitle(string sT) ;

    string  getTitle() const { return _sTitle ; }
    string  getURL()   const { return _sURL ;   }
    string  getType()  const { return _sType ;  }
    RELTYPE getRelTitle() const { return _iRelType ;  }

  protected:

    string  _sTitle ;
    string  _sURL ;

    string  _sType ;
    RELTYPE _iRelType ;
} ;

typedef vector<NsHtmlLink*>                 NsHtmlLinksVector ;
typedef NsHtmlLinksVector::iterator         NsHtmlLinkIter ;
typedef NsHtmlLinksVector::const_iterator   NsHtmlLinkConstIter ;
typedef NsHtmlLinksVector::reverse_iterator NsHtmlLinkReverseIter ;
typedef NTArray<NsHtmlLink>                 NsHtmlLinksArray ;

/**
 * A set of tags and links that represents an information block
 */
class _CLASSELEXI NsXmlBlock
{
  public:

    NsXmlBlock() ;

    NsXmlBlock(const NsXmlBlock& rv) ;

    NsXmlBlock& operator=(const NsXmlBlock& src) ;
    int         operator==(const NsXmlBlock& o) ;

    string             getTitle()    const { return _sTitle ; }
    string             getUniqueId() const { return _sUniqueId ; }
    NsXmlEntriesArray* getEntries()        { return &_aEntries ; }
    NsHtmlLinksArray*  getLinks()          { return &_aLinks ; }

    void setTitle(const string sT)    { _sTitle    = sT ; }
    void setUniqueId(const string sU) { _sUniqueId = sU ; }
    void addEntry(const NsXmlEntry &Entry) ;
    void addLink(const NsHtmlLink &Link) ;

  protected:

    string            _sTitle ;
    string            _sUniqueId ;
    NsXmlEntriesArray _aEntries ;
    NsHtmlLinksArray  _aLinks ;

    void initFromBlock(const NsXmlBlock& rv) ;
} ;

typedef vector<NsXmlBlock*>                 NsXmlBlocksVector ;
typedef NsXmlBlocksVector::iterator         NsXmlBlockIter ;
typedef NsXmlBlocksVector::const_iterator   NsXmlBlockConstIter ;
typedef NsXmlBlocksVector::reverse_iterator NsXmlBlockReverseIter ;
typedef NTArray<NsXmlBlock>                 NsXmlBlocksArray ;

/**
 * A drug component (active principle or not)
 */
class _CLASSELEXI NsDrugComponent
{
  public:

    NsDrugComponent() ;
    NsDrugComponent(string sBdmID, string sLabel) ;

    NsDrugComponent(const NsDrugComponent& rv) ;

    NsDrugComponent& operator=(const NsDrugComponent& src) ;
    int              operator==(const NsDrugComponent& o) ;

  protected:

    string  _sBdmID ;
    string  _sLabel ;
} ;

typedef vector<NsDrugComponent*>                 NsDrugComponentsVector ;
typedef NsDrugComponentsVector::iterator         NsDrugComponentIter ;
typedef NsDrugComponentsVector::const_iterator   NsDrugComponentConstIter ;
typedef NsDrugComponentsVector::reverse_iterator NsDrugComponentReverseIter ;
typedef NTArray<NsDrugComponent>                 NsDrugComponentsArray ;

// A drug as to be selected in a list
//
class _CLASSELEXI NsSelectableDrug
{
  public:

    enum DISPENSATIONTYPE { dispensationUndefined = 0, dispensationPharmacy, dispensationHospitalOnly, dispensationNotHomogeneous } ;
    enum GENERICTYPE      { genericUndefined = 0, genericGeneric } ;
    enum AMMTYPE          { ammUndefined = 0, ammFrench } ;
    enum MARKETSTATUSTYPE { marketStatusUndefined = 0, marketStatusAvailable, marketStatusDeleted, marketStatusDeletedOneYear, marketStatusPharmaco, marketStatusPharmacoOneYear } ;
    enum COMPANYROLETYPE  { companyRoleUndefined = 0, companyRoleOwner } ;
    enum BOOLEANTYPE      { boolUndefined = -1, boolFalse = 0, boolTrue = 1  } ;

    NsSelectableDrug() ;
    NsSelectableDrug(string sID, string sCIS, string sLabel, string sPrice) ;

    NsSelectableDrug(const NsSelectableDrug& rv) ;

    NsSelectableDrug& operator=(const NsSelectableDrug& src) ;
    int               operator==(const NsSelectableDrug& o) ;

    string getBdmID()                 const { return _sBdmID ; }
    string getLabel()                 const { return _sLabel ; }
    string getCIS()                   const { return _sCIS ; }
    string getATC()                   const { return _sATC ; }
    string getOnMarketDate() ;
    string getAmmLabel()              const { return _sAmmLabel ; }
    string getMarketStatusLabel()     const { return _iMarketStatusLabel ; }
    string getWithoutPrescription()   const { return getBoolLabel(_bWithoutPrescription) ; }
    string getCompanyLabel()          const { return _sCompanyLabel ; }
    string getListLabel()             const { return _sListLabel ; }
    string getVirtualDrugLabel()      const { return _sVirtualDrugLabel ; }
    string getGalenicFormLabel()      const { return _sGalenicFormLabel ; }
    string getHorsGHS()               const { return getBoolLabel(_bHorsGHS) ; }
    string getBeCareful()             const { return getBoolLabel(_bBeCareful) ; }
    string getVigilanceWarning()      const { return _sVigilanceWarning ; }
    string getDrugInSport()           const { return getBoolLabel(_bDrugInSport) ; }
    string getPrescribableByMidwife() const { return getBoolLabel(_bPrescribableByMidwife) ; }
    string getMinUcdRangePrice()      const { return _sMinUcdRangePrice ; }
    string getMaxUcdRangePrice()      const { return _sMaxUcdRangePrice ; }
    string getRefundRate()            const { return _sRefundRate ; }
    string getDispensationPlace(NSContexte* pContexte) ;
    string getGenericType() ;
    string getRetrocession()          const { return getBoolLabel(_bRetrocession) ; }
    string getSafetyAlert()           const { return getBoolLabel(_bSafetyAlert) ; }

    string getPrice() const { return _sPrice ; }

    NsHtmlLinksArray*  getLinks()      { return &_aLinks ; }
    NsXmlEntriesArray* getIndicators() { return &_aIndicators ; }

    bool   isSexNeededForSecurityControl()       { return isConceptNeededForSecurityControl(string("Sexe")) ; }
    bool   isHeightNeededForSecurityControl()    { return isConceptNeededForSecurityControl(string("Taille")) ; }
    bool   isWeightNeededForSecurityControl()    { return isConceptNeededForSecurityControl(string("Poids")) ; }
    bool   isClearanceNeededForSecurityControl() { return isConceptNeededForSecurityControl(string("Renal")) ; }

    void   setBdmID(string sI)            { _sBdmID = sI ; }
    void   setCIS(string sC)              { _sCIS   = sC ; }
    void   setATC(string sA)              { _sATC   = sA ; }
    void   setLabel(string sL)            { _sLabel = sL ; }
    void   setOnMarketDate(string sd) ;
    void   setAmmType(string sd) ;
    void   setAmmLabel(string sL)         { _sAmmLabel = sL ; }
    void   setMarketStatusType(string sM) ;
    void   setMarketStatusLabel(string sL) { _iMarketStatusLabel = sL ; }
    void   setWithoutPrescription(string sB) { _bWithoutPrescription = getBool(sB) ; }
    void   setCompanyLabel(string sC)     { _sCompanyLabel = sC ; }
    void   setCompanyBdmId(string sC)     { _sCompanyBdmId = sC ; }
    void   setCompanyRoleType(string sC) ;
    void   setListId(string sL)           { _sListId    = sL ; }
    void   setListLabel(string sL)        { _sListLabel = sL ; }
    void   setVirtualDrudId(string sV)    { _sVirtualDrugBdmId = sV ; }
    void   setVirtualDrudLabel(string sV) { _sVirtualDrugLabel = sV ; }
    void   setGalenicFormId(string sG)    { _sGalenicFormBdmId = sG ; }
    void   setGalenicFormLabel(string sG) { _sGalenicFormLabel = sG ; }
    void   setVigilanceWarning(string sV) { _sVigilanceWarning = sV ; }
    void   setMinUcdRangePrice(string sV) { _sMinUcdRangePrice = sV ; }
    void   setMaxUcdRangePrice(string sV) { _sMaxUcdRangePrice = sV ; }
    void   setRefundRate(string sR)       { _sRefundRate = sR ; }
    void   setDispensationPlace(string sD) ;
    void   setGenericType(string sG) ;
    void   setHorsGHS(string sB)               { _bHorsGHS      = getBool(sB) ; }
    void   setPrescribableByMidwife(string sB) { _bPrescribableByMidwife = getBool(sB) ; }
    void   setDrugInSport(string sB)           { _bDrugInSport  = getBool(sB) ; }
    void   setBeCareful(string sB)             { _bBeCareful    = getBool(sB) ; }
    void   setRetrocession(string sB)          { _bRetrocession = getBool(sB) ; }
    void   setSafetyAlert(string sB)           { _bSafetyAlert  = getBool(sB) ; }

    void   addLink(string sTitle, string sURL, string sType, string sRelType) ;

  protected:

    string           _sBdmID ;
    string           _sLabel ;

    string           _sCIS ;
    string           _sATC ;
    string           _sOnMarketDate ;
    AMMTYPE          _iAmmType ;
    string           _sAmmLabel ;
    MARKETSTATUSTYPE _iMarketStatusType ;
    string           _iMarketStatusLabel ;
    BOOLEANTYPE      _bWithoutPrescription ;
    string           _sCompanyLabel ;
    string           _sCompanyBdmId ;
    COMPANYROLETYPE  _iCompanyRoleType ;

    string           _sListId ;
    string           _sListLabel ;               // Type "Liste 1"

    NsDrugComponentsArray _aActivePrinciples ;
    NsXmlEntriesArray     _aIndicators ;

    string           _sVirtualDrugBdmId ;
    string           _sVirtualDrugLabel ;

    string           _sGalenicFormBdmId ;
    string           _sGalenicFormLabel ;

    string           _sVigilanceWarning ;

    string           _sMinUcdRangePrice ;
    string           _sMaxUcdRangePrice ;
    string           _sPrice ;
    string           _sRefundRate ;

    DISPENSATIONTYPE _iDispensationPlace ;
    GENERICTYPE      _iGenericType ;

    BOOLEANTYPE      _bHorsGHS ;
    BOOLEANTYPE      _bPrescribableByMidwife ;
    BOOLEANTYPE      _bDrugInSport ;
    BOOLEANTYPE      _bBeCareful ;
    BOOLEANTYPE      _bRetrocession ;
    BOOLEANTYPE      _bSafetyAlert ;

    NsHtmlLinksArray _aLinks ;

    void        reset() ;
    void        initFrom(const NsSelectableDrug& src) ;
    BOOLEANTYPE getBool(string sB) ;
    string      getBoolLabel(BOOLEANTYPE iB) const ;

    bool        isConceptNeededForSecurityControl(const string sConcept) ;
} ;

typedef vector<NsSelectableDrug*>                 NsSelectableDrugsVector ;
typedef NsSelectableDrugsVector::iterator         NsSelectableDrugIter ;
typedef NsSelectableDrugsVector::const_iterator   NsSelectableDrugConstIter ;
typedef NsSelectableDrugsVector::reverse_iterator NsSelectableDrugReverseIter ;
typedef NTArray<NsSelectableDrug>                 NsSelectableDrugArray ;

// Element selected in a list (drug, indication, ATC code...)
//
class _CLASSELEXI NSBdmEntry
{
  public:

    // Constructors
    NSBdmEntry() ;
    NSBdmEntry(const string sI, const string sL) ;
    NSBdmEntry(const NSBdmEntry& rv) ;
    // Destructor
    ~NSBdmEntry() ;

    NSBdmEntry& operator=(const NSBdmEntry& src) ;
    int 			  operator==(const NSBdmEntry& o) ;

    string getID()    const { return _sID ;    }
    string getLabel() const { return _sLabel ; }

    void   setID(const string sI)    { _sID = sI ;    }
    void   setLabel(const string sL) { _sLabel = sL ; }

  protected:

    string _sID ;
    string _sLabel ;
};

typedef vector<NSBdmEntry*>                NSBdmEntryVector ;
typedef NSBdmEntryVector::iterator         NSBdmEntryIter ;
typedef NSBdmEntryVector::const_iterator   NSBdmEntryConstIter ;
typedef NSBdmEntryVector::reverse_iterator NSBdmEntryReverseIter ;
typedef NTArray<NSBdmEntry>                NSBdmEntryArray ;

// Patient description subset of information to be sent to BAM to check prescription
//
class _CLASSELEXI NSPresCheckPatientIntformation
{
  public:

    // Constructors
    NSPresCheckPatientIntformation() ;
    NSPresCheckPatientIntformation(const NSPresCheckPatientIntformation& rv) ;
    // Destructor
    ~NSPresCheckPatientIntformation() ;

    NSBdmEntryArray* getBasicInformation() { return &_aBasicInformation ; }
    NSBdmEntryArray* getAllergies()        { return &_aAllergies ;        }
    NSBdmEntryArray* getMolecules()        { return &_aMolecules ;        }
    NSBdmEntryArray* getPathologies()      { return &_aPathologies ;      }

    NSPresCheckPatientIntformation& operator=(const NSPresCheckPatientIntformation& src) ;
    int 			                      operator==(const NSPresCheckPatientIntformation& o) ;

  protected:

    NSBdmEntryArray _aBasicInformation ;
    NSBdmEntryArray _aAllergies ;
    NSBdmEntryArray _aMolecules ;
    NSBdmEntryArray _aPathologies ;
};

// Line of prescription to be sent to BAM to check prescription
//
class _CLASSELEXI NSPresCheckPrescriptionLine
{
  public:

    // Constructors
    NSPresCheckPrescriptionLine() ;
    NSPresCheckPrescriptionLine(const NSPresCheckPrescriptionLine& rv) ;
    // Destructor
    ~NSPresCheckPrescriptionLine() ;

    string getLineBlock() const ;

    NSPresCheckPrescriptionLine& operator=(const NSPresCheckPrescriptionLine& src) ;
    int 			                   operator==(const NSPresCheckPrescriptionLine& o) ;

    string getDrug()          { return _sDrug ;          }
    string getDrugId()        { return _sDrugId ;        }
    string getDrugType()      { return _sDrugType ;      }
    string getDose()          { return _sDose ;          }
    string getDoseUnit()      { return _sDoseUnit ;      }
    string getDuration()      { return _sDuration ;      }
    string getDurationType()  { return _sDurationType ;  }
    string getFrequencyType() { return _sFrequencyType ; }

    void   setDrug(string sD)          { _sDrug = sD ;          }
    void   setDrugId(string sD)        { _sDrugId = sD ;        }
    void   setDrugType(string sD)      { _sDrugType = sD ;      }
    void   setDose(string sD)          { _sDose = sD ;          }
    void   setDoseUnit(string sD)      { _sDoseUnit = sD ;      }
    void   setDuration(string sD)      { _sDuration = sD ;      }
    void   setDurationType(string sD)  { _sDurationType = sD ;  }
    void   setFrequencyType(string sD) { _sFrequencyType = sD ; }

  protected:

    string _sDrug ;          // vidal://package/idXXXX vidal://product/idXXX vidal://vmp/idXXX vidal://ucd/idXXX
                             // vidal://cip7/XXXX vidal://cip13/XXX vidal://ucd13/XXX vidal://cis/XXX
    string _sDrugId ;
    string _sDrugType ;      // COMMON_NAME_GROUP PRODUCT PACK UCD
    string _sDose ;
    string _sDoseUnit ;
    string _sDuration ;
    string _sDurationType ;  // MINUTE HOUR DAY WEEK MONTH YEAR
    string _sFrequencyType ; // THIS_DAY PER_DAY PER_24_HOURS PER_WEEK PER_MONTH PER_YEAR PER_2_DAYS PER_HOUR PER_MINUTE

    vector<string> _aRoutes ;

    void initializeFrom(const NSPresCheckPrescriptionLine& rv) ;
    void reset() ;

    string getEntryBlock(const string sTag, const string sValue) const ;
};

typedef vector<NSPresCheckPrescriptionLine*> NSPresLineVector ;
typedef NSPresLineVector::iterator           NSPresLineIter ;
typedef NSPresLineVector::const_iterator     NSPresLineConstIter ;
typedef NSPresLineVector::reverse_iterator   NSPresLineReverseIter ;
typedef NTArray<NSPresCheckPrescriptionLine> NSPresLineArray ;

// Information to be sent to BAM to check prescription
//
class _CLASSELEXI NSPrescriptionCheckingMessage
{
  public:

    // Constructors
    NSPrescriptionCheckingMessage() ;
    NSPrescriptionCheckingMessage(const NSPrescriptionCheckingMessage& rv) ;
    // Destructor
    ~NSPrescriptionCheckingMessage() ;

    NSPresCheckPatientIntformation* getPatientInformation() { return &_patientInformation ; }
    NSPresLineArray*                getPrescriptionLines()  { return &_aPrescriptionLines ; }

    string getPatientBlock() ;
    string getPrescriptionBlock() ;

    NSPrescriptionCheckingMessage& operator=(const NSPrescriptionCheckingMessage& src) ;
    int 			                     operator==(const NSPrescriptionCheckingMessage& o) ;

  protected:

    NSPresCheckPatientIntformation _patientInformation ;
    NSPresLineArray                _aPrescriptionLines ;

    string getEntryBlock(const NSBdmEntry* pEntry) const ;
};

bool
BdmEntrySortByLabelInf(const NSBdmEntry *pObj1, const NSBdmEntry *pObj2)
{
  if (((NSBdmEntry*) NULL == pObj1) || ((NSBdmEntry*) NULL == pObj2))
    return false ;

	return (pObj1->getLabel() < pObj2->getLabel()) ;
}

bool
BdmEntrySortByIdInf(const NSBdmEntry *pObj1, const NSBdmEntry *pObj2)
{
  if (((NSBdmEntry*) NULL == pObj1) || ((NSBdmEntry*) NULL == pObj2))
    return false ;

	return (pObj1->getID() < pObj2->getID()) ;
}

//
// Classe d'acces au dictionnaire
//
class _CLASSELEXI NSBdmDriver : public NSRoot
{
	public:

    enum BAMTABLETYPE { bamTableUndefined = 0, bamTableIndication, bamTableSubstance, bamTableATC, bamTableCim10 } ;

    // Constructor and destructor
    NSBdmDriver(NSContexte* pCtx) ;
    ~NSBdmDriver() ;

    // Inject user's IDs in vars
    void initVars(vector<Var>* paVars) ;

    // Get the Bam ID from CIS code
    //
    string getBamIdFromCisCode(const string sCisCode) ;

    // Get standard code from Bam ID
    //
    string getAtcCodeFromBamId(const string sBamId) ;

    // Function that get an html document that displays documents for a product
    //
    string getDocumentsForProduct(const string sCisCode) ;
    void   getBlocksForProduct(const string sCisCode, NsXmlBlocksArray* pBlocks) ;
    void   getBlocksFromUrl(const string sUrl, NsXmlBlocksArray* pBlocks) ;

    string addBlocksFromResults(NsXmlBlocksArray* pBlocks, const string* psResult) ;

    // Function that get a list of elements from their first characters
    //
    void getListForSeed(NSBdmEntryArray* paListeArray, string* pSeed, BAMTABLETYPE iTableType) ;

    // Functions involved in the prescription checking process
    //
    void initializeChecker(NSPrescriptionCheckingMessage *pMsg) ;
    void addPatientInfoToChecker(NSPrescriptionCheckingMessage *pMsg, const NSPatPathoArray* pInformation) ;
    void addPatientInfoToChecker(NSPrescriptionCheckingMessage *pMsg, const string sInformation, const string sValue) ;
    void addPathologyToChecker(NSPrescriptionCheckingMessage *pMsg, const string sIcdCode) ;
    void addPrecriptionLineToChecker(NSPrescriptionCheckingMessage *pMsg, const NSPresCheckPrescriptionLine* pLine) ;

    void checkPrescription(NSPrescriptionCheckingMessage *pMsg) ;

    // Function that get a list of drugs from a criteria (substance, ATC code...)
    //
    void getDrugsForCriteria(NsSelectableDrugArray* paDrugsList, string* pCode, BAMTABLETYPE iTableType) ;
    bool getDrugInformation(NsSelectableDrug* pDrugInformation, string* pCisCode) ;

    // Function that get a full html document from a partial url
    //
    string getFullUrl(const string sPartialUrl) ;

    ChoixBdmDialog* getBdmDialog()                     { return _pBdmDialog ; }
    void            setBdmDialog(ChoixBdmDialog* pDlg) { _pBdmDialog = pDlg ; }

    bool            isBusy() const { return _isBusy ; }

  protected :

    // Boite de dialogue de choix des elements du lexique
    ChoixBdmDialog* _pBdmDialog ;

    // Rest requests engine
    Rest*           _pRest ;

    bool            _isBusy ;

    // Functions that get a list of elements from their first characters
    void getElementsForSeed(NSBdmEntryArray* paListeArray, string* pSeed, const string sQuery, const string sIdTag, const string sLabelTag, const vector<Var>* paSpecificVars) ;
    void getIndicationsForSeed(NSBdmEntryArray* paListeArray, string* pSeed) ;
    void getSubstancesForSeed(NSBdmEntryArray* paListeArray, string* pSeed) ;
    void getAtcCodesForSeed(NSBdmEntryArray* paListeArray, string* pSeed) ;
    void getCim10CodesForSeed(NSBdmEntryArray* paListeArray, string* pSeed) ;
    void getUnitCodesForSeed(NSBdmEntryArray* paListeArray, string* pSeed) ;

    string fillArrayOfBdmEntriesFromResults(NSBdmEntryArray* paListeArray, const string* psResult, const string sIdTag, const string sLabelTag) ;

    // Functions that get a list of drugs from a criteria (substance, ATC code...)
    void getDrugsForSeed(NsSelectableDrugArray* paDrugsList, string* pSeed, const string sQuery, const vector<Var>* paSpecificVars) ;
    void getDrugsForIndication(NsSelectableDrugArray* paDrugsList, string* pCode) ;
    void getDrugsForSubstance(NsSelectableDrugArray* paDrugsList, string* pCode) ;
    void getDrugsForAtcCode(NsSelectableDrugArray* paDrugsList, string* pCode) ;

    string fillArrayOfDrugsFromResults(NsSelectableDrugArray* paDrugsList, const string* psResult) ;

    void   getSecurityIndicators(NsSelectableDrug* pDrugInformation) ;

    // Generic functions
    string getTextForTag(TiXmlElement* pEntryElement, const string sTag) ;
    string getTextForNode(TiXmlNode* pIdNode) ;
    string getAttributeForTag(TiXmlElement* pEntryElement, const string sTag, const string sAttributeName) ;

    string FromUTF8ToISO(const string sUTF8) ;
    string FromISOToUTF8(const string sISO) ;
    string GetCleanTag(const string sTag) ;

    bool   isEntryInArray(const NSBdmEntryArray* paListeArray, const string sID, const string sLabel) const ;
} ;

#endif

// __NSBDMDRIVER_H

