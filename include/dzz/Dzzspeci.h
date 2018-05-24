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

    decSpecialite(NSContexte* pCtx, int iDecodeType = dcPhrase) ;    decSpecialite(decodageBase* pBase, int iDecodeType = dcPhrase) ;

    // Lancement du d�codage
    void decode(int colonne) ;
    void donnePhrase() ;

  protected:

    NSPhrasePrescript _DkdPrescript ;

    string getVirtualDrug(const string sSpeciality) ;
};

//-----------------------------------------------------------------
// classe decPrescription
//
// D�codage des prescriptions
//-----------------------------------------------------------------
class decPrescription : public decodage
{
  public:

    decPrescription(NSContexte* pCtx) ;
    decPrescription(decodageBase* pBase) ;

    // Lancement du d�codage
    void decode(int colonne) ;

    void setLibelle(string sLib) { sLibelle = sLib ; }

  protected:

    string sLibelle ;
};

#endif  // __DZZSPECI_H

