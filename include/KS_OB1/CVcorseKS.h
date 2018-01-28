// -----------------------------------------------------------------------------
// RVDB_CardioVasculaireKS.h
// -----------------------------------------------------------------------------
// $Revision: 1.3 $
// $Author: philippe $
// $Date: 2005/05/13 17:56:03 $
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// FLP  - décembre 2003
// -----------------------------------------------------------------------------

#ifndef __CVCORSEKS_H__
#define __CVCORSEKS_H__

#include <bwcc.h>

#include "ns_ob1\nsbbk.h"
#include "ns_ob1\InitStructure.h"  // for KSInitStructure
#include "ns_ob1\abstractKS.h"     // for Abtract
#include "ns_ob1\Transfer.h"        // for TRansfert

	// 0 : CHD (maladie coronarienne)
  // 1 : MI  (infarctus du myocarde)
  // 2 : CHD death
  // 3 : AVC (accident vasc cérébral)
  // 4 : CVD (maladie cardiovasculaire)
  // 5 : CVD death

	const double theta0[]        = {   0.9145,	3.4064,	 2.9851, -0.4312,	0.6536,	  0.8207 } ;
  const double theta1[]        = {	-0.2784, -0.8584,	-0.9142,       0, -0.2402, -0.4346 } ;
  const double beta0[]         = {  15.5305, 11.4712, 11.2889, 26.5116, 18.8144, -5.0385 } ;
  const double female[]        = {	28.4441, 10.5109,  0.2332,	0.2019,	-1.2146,	0.2243 } ;
  const double logAge[]        = {	-1.4792, -0.7965, -0.944,	 -2.3741,	-1.8443, 	8.237  } ;
  const double logAge2[]       = {        0,	     0,       0,	     0,       0, -1.2109 } ;
  const double logAgeFemale[]  = { -14.4588, -5.4216,	      0,       0,  0.3668,	     0 } ;
  const double logAge2Female[] = {	 1.8515,  0.7101,       0,       0,       0,       0 } ;
  const double logSBP[]        = {  -0.9119, -0.6623,	-0.588,	 -2.4643,	-1.4032, -0.8383 } ;
  const double tobacco[]       = {  -0.2767, -0.2675,	-0.1367, -0.3914,	-0.3899, -0.1618 } ;
  const double logTCH_DL_C[]   = {  -0.7181, -0.4277,	-0.3448, -0.0229,	-0.539,	 -0.3493 } ;
	const double diabetes[]	     = {  -0.1759, -0.1534,	-0.0474, -0.3087,	-0.3036, -0.0833 } ;
	const double diabetesFemale[]= {  -0.1999, -0.1165, -0.2233, -0.2627,	-0.1697, -0.2067 } ;
	const double ECG_LVH[]       = {  -0.5865,	     0,	-0.1237, -0.2355,	-0.3362, -0.2946 } ;
	const double ECG_LVH_Male[]  = {        0, -0.1588,       0,       0,       0,       0 } ;

class CVcorseKS : public AbstractKS
{
 public:
 	CVcorseKS() ;
	~CVcorseKS() {} ;

  int sexe ;
  int iAge ;
	double dPAS ;
	int iTabac ;  		//oui/non
	double CholesterolTot ;
	double HDL_Cholesterol	;
  double dGlycemie	;
  int Diabete ;			//oui=1/non
	int HVG_ECG	;    //oui=1/non
	int iFollowUp	;

  double riskDefault ;

	bool                TriggerCondition(const BB1Event& event) ;
  KsInitStructure*    Publication() ;
  ValidityContextType ContextValidity(const BB1BB& bb, Transfert* trans) ;
  bool                Precondition(const BB1BB& bb, Transfert* ksar) ;
	bool                ObviationCondition(const BB1BB& bb, BB1KSAR& ksar) ;
  Errcode             Action(BB1BB& bb, Transfert* trans,bool isOpportuniste) ;


  double					 Calcul1(int i) ;
  double					 Calcul2(int i) ;
  double					 Calcul3(int i) ;
  double					 Calcul4(int i) ;
  double					 ProbabilityCalcul(int i) ;
} ;

bool                _export init(string *pInit) ;
KsInitStructure*    _export Publication() ;
ValidityContextType _export ContextValidity(const BB1BB& bb, Transfert* trans);
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
  CVcorseKS        ks ;
#  endif
# else
  extern TModule*  pNSResModule ;
  extern CVcorseKS ks ;
# endif


#endif  // __CVCORSEKS_H__
