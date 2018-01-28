#ifndef __DZZDECOD_H#define __DZZDECOD_H

class NSDocumentData ;
#include "dcodeur\nsdkd.h"
//-----------------------------------------------------------------// classe decodage
//// Classe de base des classes de décodage
//-----------------------------------------------------------------
class decodage : public decodageBase
{
  public:
    // Données statiques    //
    static string _sExamen ;   // Type d'examen
    static int    _iCompteur ; // Compteur (ex : pour numéroter les precriptions)

    static int             _iNiveau ;
    static NSDocumentData* _pDocument ;

    enum DECODENIVEAU { iTitre = 0, iChapitre, iSsChapitre, iTexte, iConclusion } ;

    // Méthodes
    //
    decodage(NSContexte* pCtx)    : decodageBase(pCtx) {}
    decodage(decodageBase* pBase) : decodageBase(pBase) {}
    decodage(const decodage& rv)  : decodageBase(rv.pContexte) {} ;
    virtual ~decodage() {}

    void BioLibre(int colonne, string decDeb = string(""), string decFin = string(""), int sautLigne = 0) ;
    void lesionVide(string phrase, int colonne)  ;
    void debutPhrase() { setDcodeur(string("")) ; }
    void finPhrase()   { setDcodeur(string("")) ; }
    // Debrou

    void entete(gereDate* pDateExam);};

//-----------------------------------------------------------------// classe decCavitesVG
//
// Assure le décodage du ventricule gauche
//-----------------------------------------------------------------
class decGeneral : public decodage
{
  public:

    // Constructeur    decGeneral(NSContexte* pCtx) ;    decGeneral(decodageBase* pBase) ;
    ~decGeneral() {}

    // Lancement du décodage    void decode() ;

    void versi(int colonne) ;};

#endif  // __DZZDECOD_H
