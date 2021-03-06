#ifndef __DECODER_H#define __DECODER_H

class ClasseStringVector ;

#include "dcodeur\nsdkd.h"
//-----------------------------------------------------------------// classe lot
//
// El�ment individuel de d�codage
//-----------------------------------------------------------------
class NSlotArray ;

class lot : public NSRoot
{
  public :

    // T�te
    lot*   pLotPere ;
    string sConnection ;

    // Corps
    /*string sLesion;             // L�sion
      string sLocalisation;       // Organe
      string sGeste; */             // Traitement
    string sObjet ;

    string sDate ;
    string sHeure ;
    string sValeurChiffree ;

    string sQualifiant ;

    // Queue
    NSlotArray* pLotsLies ;

    lot(NSContexte* pCtx) ;
    lot(const lot& rv) ;
    //
    // Destructeur
    //
    ~lot() ;

    void ajouteDate(string sDate) ;
    void ajouteHeure(string sHeure) ;
    void ajouteValeurChiffree(string sValeurChiffree) ;
    void ajouteQualifiant(string sQualifiant) ;

    void setObjet(string sObj)          { sObjet      = sObj ; }
    void setLotPere(lot* pLot)          { pLotPere    = pLot ; }
    void setConnection(string sConnect) { sConnection = sConnect ; }

    string donneObjet()  { return sObjet ; }
};

//-----------------------------------------------------------------
// classe decodage
//
// Classe de base des classes de d�codage
//-----------------------------------------------------------------
class _NSDKDCLASSE GlobalDkd : public decodageBase
{
  public:

    string              sChemin ;
    NSPatPathoArray*    pSousPatPatho ;
    ClasseStringVector* pVect ;

    string              sLangue ;

    GlobalDkd(NSContexte* pCtx, string sLangue, string sChem = "",
                  NSPatPathoArray* pSPP = 0, ClasseStringVector* pCSV = 0) ;

    void   setChemin(const string sChem) 				      { sChemin = sChem ; }
    void   setSsPatPatho(NSPatPathoArray* pSPP) 		  { pSousPatPatho = pSPP ; }
    void   setStringVect(ClasseStringVector* pStVect) { pVect = pStVect ; }

    bool   decode(int precision) ;
    bool   decodeSpecifique(int precision) ;

    bool   decodeGenerique(int precision) ;
    void   assembleLots(int colonne, lot* pLotPere) ;
    void   decodeLots(lot* pLot, string sObjetMaitre, GENRE iGenreMaitre) ;

    bool   decodeNoeud(string sCheminLocal = "", bool bClassifJustLabel = false) ;

    string decodeMot(string* pCode, GENRE iGenreMaitre, GENRE* iGenreMot, int* iCertMot) ;
    string decodeGroupe(string* pCode, GENRE iGenre) ;
    string decodeGroupeDecode(string* pCode, GENRE iGenre) ;

    bool   decodeNum(string* sLibelle, gereNum* pNum) ;
    bool   decodeDate(string* sLibelle, gereDate* pDate) ;
    bool   decodeHeure(string* sLibelle, gereHeure* pHeure) ;

    bool   CommenceParVoyelle(const string* pLib) const ;    string donneLibelleAdjectif(string sLabel, int iCertitude) ;
    string donneLibelleNom(string sLabel, int iCertitude) ;
    string donneLibelleAutre(string sLabel, int iCertitude) ;
};

//VECTEUR DE PATPATHO
typedef vector<lot*> NSLotsArray ;
typedef NSLotsArray::iterator       LotIter ;
typedef NSLotsArray::const_iterator LotConstIter ;

class NSlotArray : public NSLotsArray, public NSRoot
{
  public :

    //
    // Constructeurs
    //
    NSlotArray(NSContexte* pCtx = 0) ;
    NSlotArray(const NSlotArray& rv) ;
    //
    // Destructeur
    //
    ~NSlotArray() ;

    void vider() ;
    //
    // Op�rateurs
    //
    NSlotArray& NSlotArray::operator=(const NSlotArray& src) ;
};

#endif  // __DECODER_H

