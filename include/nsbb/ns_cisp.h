// -----------------------------------------------------------------------------
// ns_cisp.h
// -----------------------------------------------------------------------------
// $Revision: 1.2 $
// $Author: pameline $
// $Date: 2009/07/23 17:03:17 $
// -----------------------------------------------------------------------------
// Ligne de vie - Interfaces CISP
// Ligne de vie - ICPC Interfaces
// -----------------------------------------------------------------------------
// PA  - juillet 2008
// ...
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Programme protégé par une licence Open Source GPL
// NAUTILUS - www.nautilus-info.com
// -----------------------------------------------------------------------------

#ifndef __NS_CISP_H
#define __NS_CISP_H

#include <owl\dialog.h>
#include <owl\slider.h>
#include <owl\gauge.h>
#include <owl\listwind.h>

#include "nsbb\nsutidlg.h"

//
// Evaluation du DUSOI
//
class _NSBBCLASSE NSDusoiDlg : public NSUtilDialog
{
 public:

  int* _piDusoi ;
  int* _piSymptoms ;
  int* _piComplication ;
  int* _piPrognosis ;
  int* _piTreatability ;

  int  iCalculatedDusoi ;

  int  iSymptomValue ;
  int  iComplicationValue ;
  int  iPrognosisValue ;
  int  iTreatabilityValue ;

  OWL::TStatic  *_pDusoiTxt ;
  OWL::TStatic  *_pDusoiValueTxt ;
  OWL::TStatic  *_pCourtesyTxt ;

  OWL::TStatic  *_pSymptomsText ;
  OWL::THSlider *_pSymptomsSlide ;
  OWL::TGauge   *_pSymptomsGauge ;
  OWL::TStatic  *_pSymptomsLabel0 ;
  OWL::TStatic  *_pSymptomsLabel1 ;
  OWL::TStatic  *_pSymptomsLabel2 ;
  OWL::TStatic  *_pSymptomsLabel3 ;
  OWL::TStatic  *_pSymptomsLabel4 ;

  OWL::TStatic  *_pComplicationsText ;
  OWL::THSlider *_pComplicationsSlide ;
  OWL::TGauge   *_pComplicationsGauge ;
  OWL::TStatic  *_pComplicationsLabel0 ;
  OWL::TStatic  *_pComplicationsLabel1 ;
  OWL::TStatic  *_pComplicationsLabel2 ;
  OWL::TStatic  *_pComplicationsLabel3 ;
  OWL::TStatic  *_pComplicationsLabel4 ;

  OWL::TStatic  *_pPrognosisText ;
  OWL::THSlider *_pPrognosisSlide ;
  OWL::TGauge   *_pPrognosisGauge ;
  OWL::TStatic  *_pPrognosisLabelExp ;
  OWL::TStatic  *_pPrognosisLabel0 ;
  OWL::TStatic  *_pPrognosisLabel1 ;
  OWL::TStatic  *_pPrognosisLabel2 ;
  OWL::TStatic  *_pPrognosisLabel3 ;
  OWL::TStatic  *_pPrognosisLabel4 ;
  OWL::TStatic  *_pPrognosisExplain ;

  OWL::TStatic  *_pTreatabilityText ;
  OWL::THSlider *_pTreatabilitySlide ;
  OWL::TGauge   *_pTreatabilityGauge ;
  OWL::TStatic  *_pTreatabilityLabelExp0 ;
  OWL::TStatic  *_pTreatabilityLabelExp1 ;
  OWL::TStatic  *_pTreatabilityLabel0 ;
  OWL::TStatic  *_pTreatabilityLabel1 ;
  OWL::TStatic  *_pTreatabilityLabel2 ;
  OWL::TStatic  *_pTreatabilityLabel3 ;
  OWL::TStatic  *_pTreatabilityLabel4 ;
  OWL::TStatic  *_pTreatabilityExplain0 ;
  OWL::TStatic  *_pTreatabilityExplain1 ;

  NSDusoiDlg(TWindow* pPere, NSContexte* pCtx, int* piScore, int* pSymptom, int* pComplication, int* pPrognosis, int* pTreatability) ;
  ~NSDusoiDlg() ;

  void CmOk() ;
  void CmCancel() ;
  void SetupWindow() ;

  void UpdateSymptoms(uint) ;
  void UpdateComplications(uint) ;
  void UpdatePrognosis(uint) ;
  void UpdateTreatability(uint) ;
  void UpdateGauge(uint, OWL::THSlider *pSlider, OWL::TGauge *pGauge) ;

  void InitSymptoms() ;
  void InitComplications() ;
  void InitPrognosis() ;
  void InitTreatability() ;
  void InitGauge(uint value, OWL::THSlider *pSlider, OWL::TGauge *pGauge) ;

  void Calculate() ;

  void initValueFromPointer(int &iValue, int* pPointer, int iMinVal, int iMaxVal) ;

	DECLARE_RESPONSE_TABLE(NSDusoiDlg) ;
};

#endif // __NS_CISP_H

