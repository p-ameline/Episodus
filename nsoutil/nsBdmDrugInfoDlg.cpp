#include <owl\applicat.h>
#include <owl\dialog.h>
#include <owl\button.h>
#include <stdio.h>
#include <string.h>

#include "nautilus\nssuper.h"
#include "nsoutil\nsoutil.h"
#include "nsoutil\nsBdmDrugInfoDlg.h"
#include "nsoutil\nsBdmInfoDlg.h"

#include "nsoutil\nsBdmDlg.rh"

// -----------------------------------------------------------------
//
//  Méthodes de NSDrugResearchDlg
//
// -----------------------------------------------------------------
// -----------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NSBdmDrugInfoDlg, NSGenericBdmInfoDlg)
END_RESPONSE_TABLE;

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NSBdmDrugInfoDlg::NSBdmDrugInfoDlg(TWindow* pere, NSContexte* pCtx, NsSelectableDrug* pDrugInfo)
                 :NSGenericBdmInfoDlg(pere, pCtx, "DRUGS_INFO_DLG")
{
  _pDrugInfo = pDrugInfo ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSBdmDrugInfoDlg::~NSBdmDrugInfoDlg()
{
}

//---------------------------------------------------------------------------
//  Initialise la boite de dialogue
//---------------------------------------------------------------------------
void
NSBdmDrugInfoDlg::SetupWindow()
{
  NSGenericBdmInfoDlg::SetupWindow() ;
}

void
NSBdmDrugInfoDlg::fillList()
{
  NsXmlEntriesArray aFieldsArray ;

  aFieldsArray.push_back(new NsXmlEntry(string("drugName"),              _pDrugInfo->getLabel())) ;
  aFieldsArray.push_back(new NsXmlEntry(string("virtualDrug"),           _pDrugInfo->getVirtualDrugLabel())) ;
  aFieldsArray.push_back(new NsXmlEntry(string("galenicForm"),           _pDrugInfo->getGalenicFormLabel())) ;
  aFieldsArray.push_back(new NsXmlEntry(string("cisCode"),               _pDrugInfo->getCIS())) ;
  aFieldsArray.push_back(new NsXmlEntry(string("marketStatusType"),      _pDrugInfo->getMarketStatusLabel())) ;
  aFieldsArray.push_back(new NsXmlEntry(string("onMarketDate"),          _pDrugInfo->getOnMarketDate())) ;
  aFieldsArray.push_back(new NsXmlEntry(string("ammType"),               _pDrugInfo->getAmmLabel())) ;
  aFieldsArray.push_back(new NsXmlEntry(string("minUcdRangePrice"),      _pDrugInfo->getMinUcdRangePrice())) ;
  aFieldsArray.push_back(new NsXmlEntry(string("maxUcdRangePrice"),      _pDrugInfo->getMaxUcdRangePrice())) ;
  aFieldsArray.push_back(new NsXmlEntry(string("refundRate"),            _pDrugInfo->getRefundRate())) ;
  aFieldsArray.push_back(new NsXmlEntry(string("withoutPrescription"),   _pDrugInfo->getWithoutPrescription())) ;
  aFieldsArray.push_back(new NsXmlEntry(string("prescribableByMidwife"), _pDrugInfo->getPrescribableByMidwife())) ;
  aFieldsArray.push_back(new NsXmlEntry(string("drugInSport"),           _pDrugInfo->getDrugInSport())) ;
  aFieldsArray.push_back(new NsXmlEntry(string("beCareful"),             _pDrugInfo->getBeCareful())) ;
  aFieldsArray.push_back(new NsXmlEntry(string("vigilanceWarning"),      _pDrugInfo->getVigilanceWarning())) ;
  aFieldsArray.push_back(new NsXmlEntry(string("secured"),               _pDrugInfo->getSafetyAlert())) ;
  aFieldsArray.push_back(new NsXmlEntry(string("horsGHS"),               _pDrugInfo->getHorsGHS())) ;
  aFieldsArray.push_back(new NsXmlEntry(string("companyName"),           _pDrugInfo->getCompanyLabel())) ;
  aFieldsArray.push_back(new NsXmlEntry(string("list"),                  _pDrugInfo->getListLabel())) ;
  aFieldsArray.push_back(new NsXmlEntry(string("dispensationPlace"),     _pDrugInfo->getDispensationPlace(pContexte))) ;
  aFieldsArray.push_back(new NsXmlEntry(string("genericType"),           _pDrugInfo->getGenericType())) ;
  aFieldsArray.push_back(new NsXmlEntry(string("retrocession"),          _pDrugInfo->getRetrocession())) ;

  _pInfoList->fillList(&aFieldsArray) ;
}

void
NSBdmDrugInfoDlg::activatedInformation(int /* iIndex */)
{
}

void
NSBdmDrugInfoDlg::openUrl(const string sUrl)
{
  if (string("") == sUrl)
    return ;

  if (isValidUrl(sUrl))
  {
    ::ShellExecute(pContexte->GetMainWindow()->HWindow,
                 "open",
                 sUrl.c_str(),
                 "",
                 "",
                 SW_SHOWNORMAL) ;

    return ;
  }

  NSBdmDriver* pDriver = pContexte->getBdmDriver() ;
  if ((NSBdmDriver*) NULL == pDriver)
    return ;

  NsXmlBlocksArray aBlocks ;

  pDriver->getBlocksFromUrl(sUrl, &aBlocks) ;

  if (aBlocks.empty())
    return ;

  NSBdmInfoDlg infoDlg(this, pContexte, *(aBlocks.begin())) ;
  infoDlg.Execute() ;
}

