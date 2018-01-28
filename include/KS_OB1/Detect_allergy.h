// -----------------------------------------------------------------------------
// Detect_allergy.h
// -----------------------------------------------------------------------------
// $Revision: 1.3 $
// $Author: philippe $
// $Date: 2005/05/13 17:56:03 $
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// PA  - décembre 2010
// -----------------------------------------------------------------------------

#ifndef __DETECT_ALLERGY_H__
#define __DETECT_ALLERGY_H__

#include <bwcc.h>

#include "ns_ob1\nsbbk.h"
#include "ns_ob1\InitStructure.h"  // for KSInitStructure
#include "ns_ob1\abstractKS.h"     // for Abtract
#include "ns_ob1\Transfer.h"        // for TRansfert

class DetectAllergyKS : public AbstractKS
{
 public:

	DetectAllergyKS() ;
	~DetectAllergyKS() {} ;

	bool                TriggerCondition(const BB1Event& event) ;
  KsInitStructure*    Publication() ;
  ValidityContextType ContextValidity(const BB1BB& bb, Transfert* trans) ;
  bool                Precondition(const BB1BB& bb, Transfert* ksar) ;
	bool                ObviationCondition(const BB1BB& bb, BB1KSAR& ksar) ;
  Errcode             Action(BB1BB& bb, Transfert* trans,bool isOpportuniste) ;

  string           getNewDrugSens(BB1BB& bb, Transfert* trans) ;
  bool             getAllergiesArray(BB1BB& bb, NSPatPathoArray *pPpt) ;
} ;

bool                _export init(string *pInit) ;
KsInitStructure*    _export Publication() ;
ValidityContextType _export ContextValidity(const BB1BB& bb, Transfert* trans) ;
bool                _export KSTriggerCondition(const BB1Event& event) ;
bool                _export KSPrecondition(const BB1BB& bb, Transfert* ksar) ;
bool                _export KSObviationCondition(const BB1BB& bb, BB1KSAR& ksar) ;
Errcode             _export KSAction(BB1BB& bb, Transfert* trans, bool isOpportuniste) ;

# ifdef _MAIN
#  ifndef __MAIN
  extern TModule   *pNSResModule ;
#  else
  static TModule   *pResMod ;
  TModule          *pNSResModule ;
  DetectAllergyKS  ks ;
#  endif
# else
  extern TModule*        pNSResModule ;
  extern DetectAllergyKS ks ;
# endif


#endif  // __DETECT_ALLERGY_H__
