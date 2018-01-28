// -----------------------------------------------------------------------------
// ns_vidal.h
// -----------------------------------------------------------------------------
// $Revision: 1.1 $
// $Author: pameline $
// $Date: 2015/02/07 14:44:30 $
// -----------------------------------------------------------------------------
// Import / Export for Schiller's SEMA2 format regarding ECG
// -----------------------------------------------------------------------------
// PA - juillet 2010
// -----------------------------------------------------------------------------

#ifndef __SCHILLEREXP_H__
#define __SCHILLEREXP_H__

#include "nsbb\nspatpat.h"

//
// Objet "Boite de dialogue" utilisé pour sélectionner l'utilitaire
//
class NsSchillerExpAnalyser : public NSRoot
{
  public:

    NsSchillerExpAnalyser(NSContexte* pCtx) ;
    ~NsSchillerExpAnalyser() ;

    bool   load(string sFileFullName) ;

    string getLexForRecType(string sInfoVal) ;

    void   addDateAndId() ;
    void   addBiometricalValue(string sConcept, string sValue, string sUnit) ;
    void   addDescriptionValue(string sConcept, string sValue, string sUnit) ;
    void   addValue(string sChapter, string sConcept, string sValue, string sUnit) ;
    void   addDescriptionText(string sText) ;

    string           getPatientId()    { return _sPatientId ; }
    // NSPatPathoArray* getPatPatho()     { return &Patpatho ; }
    void             setPatPatho(const NSPatPathoArray* pPpt) ;
    void             initFromPatPatho(NSPatPathoArray* pPpt) const ;
    string           getAttachedFile() { return _sAttachedFile ; }
    string           getExamId() ;

  protected:

    string _sTypeOfRecording ;
    
    string _sPatientId ;
    string _sPatientLastName ;
    string _sPatientFirsName ;
    string _sPatientBirthDate ;
    string _sPatientGender ;

    string _sExaminationDate ;
    string _sExaminationTime ;

    string _sAttachedFile ;

    NSPatPathoArray Patpatho ;

    map<string, string> _aMedicalData ;
} ;

#endif    // __SCHILLEREXP_H__

