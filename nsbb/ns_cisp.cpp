// -----------------------------------------------------------------------------
// ns_cisp.cpp
// -----------------------------------------------------------------------------
// $Revision: 1.2 $
// $Author: pameline $
// $Date: 2009/07/04 09:15:49 $
// -----------------------------------------------------------------------------
// Ligne de vie - Interfaces CISP
// Ligne de vie - ICPC Interfaces
// -----------------------------------------------------------------------------
// PA  - juillet 2008
// ...
// -----------------------------------------------------------------------------

#include <owl\owlpch.h>
#include <owl\dc.h>
#include <owl\gauge.h>

#include "nautilus\nssuper.h"
#include "nsbb\nsbb.h"
#include "nsbb\ns_cisp.h"
#include "nsbb\ns_cisp.rh"

// -----------------------------------------------------------------
//
//  Méthodes de NSDusoiDlg
//
// -----------------------------------------------------------------
// -----------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NSDusoiDlg, NSUtilDialog)
  EV_COMMAND(IDOK,                        CmOk),
  EV_COMMAND(IDCANCEL,                    CmCancel),
  EV_CHILD_NOTIFY_ALL_CODES(SYMPTOMS_SLIDER, UpdateSymptoms),
  EV_CHILD_NOTIFY_ALL_CODES(COMPLICA_SLIDER, UpdateComplications),
  EV_CHILD_NOTIFY_ALL_CODES(PROGNOSI_SLIDER, UpdatePrognosis),
  EV_CHILD_NOTIFY_ALL_CODES(TREATABI_SLIDER, UpdateTreatability),
END_RESPONSE_TABLE;

NSDusoiDlg::NSDusoiDlg(TWindow* pPere, NSContexte* pCtx, int* piScore, int* pSymptom, int* pComplication, int* pPrognosis, int* pTreatability)
           :NSUtilDialog(pPere, pCtx, "DUSOI", pNSDLLModule)
{
try
{
  _piDusoi = piScore ;

  _piSymptoms     = pSymptom ;
  _piComplication = pComplication ;
  _piPrognosis    = pPrognosis ;
  _piTreatability = pTreatability ;

  initValueFromPointer(iCalculatedDusoi,   _piDusoi,        0, 100) ;
  initValueFromPointer(iSymptomValue,      _piSymptoms,     0, 4) ;
  initValueFromPointer(iComplicationValue, _piComplication, 0, 4) ;
  initValueFromPointer(iPrognosisValue,    _piPrognosis,    0, 4) ;
  initValueFromPointer(iTreatabilityValue, _piTreatability, 0, 4) ;

  _pDusoiTxt      = new OWL::TStatic(this, DUSOI_TXT) ;
  _pDusoiValueTxt = new OWL::TStatic(this, DUSOI_VALUE) ;
  _pCourtesyTxt   = new OWL::TStatic(this, DUSOI_COURTESY) ;

  _pSymptomsText   = new OWL::TStatic(this, SYMPTOMS_TEXT) ;
  _pSymptomsSlide  = new OWL::THSlider(this, SYMPTOMS_SLIDER) ;
  _pSymptomsGauge  = 0 ; // new OWL::TGauge(this, "%d%%", SYMPTOMS_GAUGE, 12, 72, 200, 15) ;
  _pSymptomsLabel0 = new OWL::TStatic(this, SYMPTOMS_LABEL0) ;
  _pSymptomsLabel1 = new OWL::TStatic(this, SYMPTOMS_LABEL1) ;
  _pSymptomsLabel2 = new OWL::TStatic(this, SYMPTOMS_LABEL2) ;
  _pSymptomsLabel3 = new OWL::TStatic(this, SYMPTOMS_LABEL3) ;
  _pSymptomsLabel4 = new OWL::TStatic(this, SYMPTOMS_LABEL4) ;

  _pComplicationsText   = new OWL::TStatic(this, COMPLICA_TEXT) ;
  _pComplicationsSlide  = new OWL::THSlider(this, COMPLICA_SLIDER) ;
  _pComplicationsGauge  = 0 ; // new OWL::TGauge(this, "%d%%", COMPLICA_GAUGE, 12, 120, 200, 15) ;
  _pComplicationsLabel0 = new OWL::TStatic(this, COMPLICA_LABEL0) ;
  _pComplicationsLabel1 = new OWL::TStatic(this, COMPLICA_LABEL1) ;
  _pComplicationsLabel2 = new OWL::TStatic(this, COMPLICA_LABEL2) ;
  _pComplicationsLabel3 = new OWL::TStatic(this, COMPLICA_LABEL3) ;
  _pComplicationsLabel4 = new OWL::TStatic(this, COMPLICA_LABEL4) ;

  _pPrognosisText    = new OWL::TStatic(this, PROGNOSI_TEXT) ;
  _pPrognosisSlide   = new OWL::THSlider(this, PROGNOSI_SLIDER) ;
  _pPrognosisGauge   = 0 ; // new OWL::TGauge(this, "%d%%", PROGNOSI_GAUGE, 12, 176, 200, 15) ;
  _pPrognosisLabel0  = new OWL::TStatic(this, PROGNOSI_LABEL0) ;
  _pPrognosisLabel1  = new OWL::TStatic(this, PROGNOSI_LABEL1) ;
  _pPrognosisLabel2  = new OWL::TStatic(this, PROGNOSI_LABEL2) ;
  _pPrognosisLabel3  = new OWL::TStatic(this, PROGNOSI_LABEL3) ;
  _pPrognosisLabel4  = new OWL::TStatic(this, PROGNOSI_LABEL4) ;
  _pPrognosisExplain = new OWL::TStatic(this, PROGNOSI_EXPL) ;

  _pTreatabilityText     = new OWL::TStatic(this, TREATABI_TEXT) ;
  _pTreatabilitySlide    = new OWL::THSlider(this, TREATABI_SLIDER) ;
  _pTreatabilityGauge    = 0 ; // new OWL::TGauge(this, "%d%%", TREATABI_GAUGE, 12, 232, 200, 15) ;
  _pTreatabilityLabel0   = new OWL::TStatic(this, TREATABI_LABEL0) ;
  _pTreatabilityLabel1   = new OWL::TStatic(this, TREATABI_LABEL1) ;
  _pTreatabilityLabel2   = new OWL::TStatic(this, TREATABI_LABEL2) ;
  _pTreatabilityLabel3   = new OWL::TStatic(this, TREATABI_LABEL3) ;
  _pTreatabilityLabel4   = new OWL::TStatic(this, TREATABI_LABEL4) ;
  _pTreatabilityExplain0 = new OWL::TStatic(this, TREATABI_EXPL0) ;
  _pTreatabilityExplain1 = new OWL::TStatic(this, TREATABI_EXPL1) ;
}
catch (...)
{
	erreur("Exception NSDusoiDlg ctor.",  standardError, 0) ;
}
}

NSDusoiDlg::~NSDusoiDlg()
{
  delete _pDusoiTxt ;
  delete _pDusoiValueTxt ;
  delete _pCourtesyTxt ;

  delete _pSymptomsText ;
  delete _pSymptomsSlide ;
//  delete _pSymptomsGauge ;
  delete _pSymptomsLabel0 ;
  delete _pSymptomsLabel1 ;
  delete _pSymptomsLabel2 ;
  delete _pSymptomsLabel3 ;
  delete _pSymptomsLabel4 ;

  delete _pComplicationsText ;
  delete _pComplicationsSlide ;
//  delete _pComplicationsGauge ;
  delete _pComplicationsLabel0 ;
  delete _pComplicationsLabel1 ;
  delete _pComplicationsLabel2 ;
  delete _pComplicationsLabel3 ;
  delete _pComplicationsLabel4 ;

  delete _pPrognosisText ;
  delete _pPrognosisSlide ;
//  delete _pPrognosisGauge ;
  delete _pPrognosisLabel0 ;
  delete _pPrognosisLabel1 ;
  delete _pPrognosisLabel2 ;
  delete _pPrognosisLabel3 ;
  delete _pPrognosisLabel4 ;
  delete _pPrognosisExplain ;

  delete _pTreatabilityText ;
  delete _pTreatabilitySlide ;
//  delete _pTreatabilityGauge ;
  delete _pTreatabilityLabel0 ;
  delete _pTreatabilityLabel1 ;
  delete _pTreatabilityLabel2 ;
  delete _pTreatabilityLabel3 ;
  delete _pTreatabilityLabel4 ;
  delete _pTreatabilityExplain0 ;
  delete _pTreatabilityExplain1 ;
}

void
NSDusoiDlg::SetupWindow()
{
	NSUtilDialog::SetupWindow() ;

	InitSymptoms() ;
  InitComplications() ;
  InitPrognosis() ;
  InitTreatability() ;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void
NSDusoiDlg::CmOk()
{
try
{
  if (NULL != _piDusoi)
    *_piDusoi = iCalculatedDusoi ;

  if (NULL != _piSymptoms)
    *_piSymptoms = iSymptomValue ;

  if (NULL != _piComplication)
    *_piComplication = iComplicationValue ;

  if (NULL != _piPrognosis)
    *_piPrognosis = iPrognosisValue ;

  if (NULL != _piTreatability)
    *_piTreatability = iTreatabilityValue ;

	CloseWindow(IDOK) ;
}
catch (...)
{
  erreur("Exception NSDusoiDlg::CmOk.", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void
NSDusoiDlg::CmCancel()
{
	Destroy(IDCANCEL) ;
}

void
NSDusoiDlg::UpdateSymptoms(uint iVal)
{
  UpdateGauge(iVal, _pSymptomsSlide, _pSymptomsGauge) ;
}

void
NSDusoiDlg::UpdateComplications(uint iVal)
{
  UpdateGauge(iVal, _pComplicationsSlide, _pComplicationsGauge) ;
}

void
NSDusoiDlg::UpdatePrognosis(uint iVal)
{
  UpdateGauge(iVal, _pPrognosisSlide, _pPrognosisGauge) ;
}

void
NSDusoiDlg::UpdateTreatability(uint iVal)
{
  UpdateGauge(iVal, _pTreatabilitySlide, _pTreatabilityGauge) ;
}

void
NSDusoiDlg::UpdateGauge(uint, OWL::THSlider *pSlider, OWL::TGauge *pGauge)
{
  int setting = pSlider->GetPosition() ;

  if (NULL != pGauge)
  {
    if      (0 == setting )
		  pGauge->SetColor(NS_CLASSLIB::TColor(128, 128, 128)) ;   // Gray
	  else if (1 == setting)
		  pGauge->SetColor(NS_CLASSLIB::TColor(0, 0, 128)) ;   // Blue
	  else if (2 == setting)
		  pGauge->SetColor(NS_CLASSLIB::TColor(0, 128, 0)) ;   // Green
	  else if (3 == setting)
		  pGauge->SetColor(NS_CLASSLIB::TColor(128, 128, 0)) ; // Yellow
	  else
		  pGauge->SetColor(NS_CLASSLIB::TColor(128, 0, 0)) ;   // Red

    pGauge->SetValue(setting) ;
  }

  Calculate() ;
}

void
NSDusoiDlg::Calculate()
{
  iSymptomValue      = _pSymptomsSlide->GetPosition() ;
  iComplicationValue = _pComplicationsSlide->GetPosition() ;
  iPrognosisValue    = _pPrognosisSlide->GetPosition() ;
  iTreatabilityValue = _pTreatabilitySlide->GetPosition() ;

  int iSomme = iSymptomValue + iComplicationValue + iPrognosisValue + iTreatabilityValue ;
  double dDusoi100 = double(iSomme) * 100 / 16 ;
  string sDusoi100 = DoubleToString(&dDusoi100, -1, 0) ;

  _pDusoiValueTxt->SetText(sDusoi100.c_str()) ;

  iCalculatedDusoi = atoi(sDusoi100.c_str()) ;
}

void
NSDusoiDlg::InitSymptoms()
{
  InitGauge(iSymptomValue, _pSymptomsSlide, _pSymptomsGauge) ;
}

void
NSDusoiDlg::InitComplications()
{
  InitGauge(iComplicationValue, _pComplicationsSlide, _pComplicationsGauge) ;
}

void
NSDusoiDlg::InitPrognosis()
{
  InitGauge(iPrognosisValue, _pPrognosisSlide, _pPrognosisGauge) ;
}

void
NSDusoiDlg::InitTreatability()
{
  InitGauge(iTreatabilityValue, _pTreatabilitySlide, _pTreatabilityGauge) ;
}

void
NSDusoiDlg::InitGauge(uint iValue, OWL::THSlider *pSlider, OWL::TGauge *pGauge)
{
  pSlider->SetRange(0, 4) ;     //fixer min et max
	pSlider->SetRuler(1, true) ; //espacement entre deux graduations
	pSlider->SetPosition(iValue) ;

  if (NULL != pGauge)
    pGauge->SetValue(iValue) ;
}

void
NSDusoiDlg::initValueFromPointer(int &iValue, int* pPointer, int iMinVal, int iMaxVal)
{
  iValue = 0 ;
  if ((NULL == pPointer) || (*pPointer < iMinVal) || (*pPointer > iMaxVal))
    return ;
  iValue = *pPointer ;
}

// fin de ns_cisp.cpp
/////////////////////////////////////////////////////

