#ifndef __DZZSPECI_H#define __DZZSPECI_H

#include "dcodeur\nsphrobj.h"

#include "dzz\dzzglob.h"
#include "dzz\dzzdecod.h"

//-----------------------------------------------------------------
// class decSpecialite
//
// Phrasing for drugs
//-----------------------------------------------------------------
class decSpecialite : public decLesion
{  public:

    decSpecialite(NSContexte* pCtx, bool bAllLettersSentences, int iDecodeType = dcPhrase) ;    decSpecialite(decodageBase* pBase, bool bAllLettersSentences, int iDecodeType = dcPhrase) ;

    // Lancement du décodage
    void decode(int colonne) ;
    void donnePhrase() ;

  protected:

    NSPhrasePrescript _DkdPrescript ;
    bool              _bAllLettersSentences ;

    string getVirtualDrug(const string sSpeciality) ;
    string getDrugRootName(const string sDrugCode, const string sDrugLabel) ;
};

//-----------------------------------------------------------------
// classe decPrescription
//
// Décodage des prescriptions
//-----------------------------------------------------------------
class decPrescription : public decodage
{
  public:

    decPrescription(NSContexte* pCtx, bool bAllLettersSentences) ;
    decPrescription(decodageBase* pBase, bool bAllLettersSentences) ;

    // Lancement du décodage
    void decode(int colonne) ;

    void setLibelle(string sLib) { _sLibelle = sLib ; }

  protected:

    bool   _bAllLettersSentences ;
    string _sLibelle ;
};

#endif  // __DZZSPECI_H

