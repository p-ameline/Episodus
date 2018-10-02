#ifndef __NSPHROBJ_H#define __NSPHROBJ_H

// #include "nssavoir\nspatbas.h"
// #include "nssavoir\nspathor.h"
#include "dcodeur\nsphrase.h"
#include "dcodeur\nsdkd.h"
#include "dcodeur\nsgen.h"

#if defined(_DKD_DLL)
	#define _NSDKDCLASSE __export
#else
	#define _NSDKDCLASSE __import
#endif

class _NSDKDCLASSE NSPhraseObjet : public decodageBase
{
	public :

  	NSPatPathoInfo _Objet ;

virtual void decode(int colonne) ;
        void ammorce() ;

		NSPhraseObjet(NSContexte* pCtx, int iDecoType = dcPhrase, string sLangue = string("fr")) ;
		NSPhraseObjet(decodageBase* pBase, int iDecoType = dcPhrase, string sLangue = string("fr")) ;		NSPhraseObjet(const NSPhraseObjet& rv) ;
    virtual ~NSPhraseObjet() ;

		NSPhraseObjet& operator=(const NSPhraseObjet& src) ;
		int            operator==(const NSPhraseObjet& o) ;

    static long getNbInstance()  { return lBaseObjectCount ; }
    static void initNbInstance() { lBaseObjectCount = 0 ; }

	protected:

		int _iDcType ;

    static long lBaseObjectCount ;
};

typedef vector<NSPhraseObjet*> NSPhraObjArray ;
typedef NSPhraObjArray::iterator       iterPhraseObj ;
typedef NSPhraObjArray::const_iterator iterConstPhraseObj ;

class _NSDKDCLASSE NSPhraseObjArray : public NSPhraObjArray
{
	public :

    // Constructeurs
    NSPhraseObjArray() : NSPhraObjArray() {}
    NSPhraseObjArray(const NSPhraseObjArray& rv) ;
    // Destructeur
    virtual ~NSPhraseObjArray() ;
    NSPhraseObjArray& NSPhraseObjArray::operator=(const NSPhraseObjArray& src) ;
    //
    void vider() ;
};

class _NSDKDCLASSE NSPhraseLesion : public NSPhraseObjet
{
	public :

    NSPhraseObjArray MetaLocalisations ;  // Pour les d�fauts structurels,
                                         // ex "Epaississement de la paroi"
                                         // il peut exister une localisation
                                         // ex "au niveau de l'isthme"

    NSPhraseObjArray Aspect ;         // Description
    NSPhraseObjArray Dimensions ;     // Mesures X, Y, Z
    NSPhraseObjArray Localisations ;  // Organes ou parties d'organes
    NSPhraseObjArray Etiologies ;     // Causes
    NSPhraseObjArray Evoquant ;       // Evoquant ou De type
    NSPhraseObjArray Semiologie ;     // Sympt�mes

    NSPhraseObjArray SousLesions ;    // L�sions asoci�es

    NSPhraseObjArray Traitements ;    // Traitement
    NSPhraseObjArray CAS ;            // Conduite � suivre

    NSPhraseObjArray Temporalite ;    // Gestion du temps
    NSPhraseObjArray Garbage ;        // Objets inclassables

    void decode(int colonne) ;
    void metPhrase() ;

    NSPhraseLesion(NSContexte* pCtx, int iDecoType = dcPhrase, string sLangue = "fr") ;
    NSPhraseLesion(decodageBase* pBase, int iDecoType = dcPhrase, string sLangue = "fr") ;
    ~NSPhraseLesion() ;

    NSPhraseLesion(const NSPhraseLesion& rv) ;

    NSPhraseLesion& operator=(const NSPhraseLesion& src) ;
};

// Organe ou partie d'organe
//
class _NSDKDCLASSE NSPhraseOrgane : public NSPhraseObjet
{
	public :

    NSPhraseObjArray Dimensions ;     // Mesures X, Y, Z
    NSPhraseObjArray Lesions ;        // L�sions
    NSPhraseObjArray Aspect ;         // Description

    NSPhraseObjArray Temporalite ;   // Gestion du temps
    NSPhraseObjArray Garbage ;       // Objets inclassables

    void decode(int colonne) ;
    void metPhrase() ;

    NSPhraseOrgane(NSContexte* pCtx, int iDecoType = dcPhrase, string sLangue = "fr") ;
    NSPhraseOrgane(decodageBase* pBase, int iDecoType = dcPhrase, string sLangue = "fr") ;
    ~NSPhraseOrgane() ;

    NSPhraseOrgane(const NSPhraseOrgane& rv) ;

    NSPhraseOrgane& operator=(const NSPhraseOrgane& src) ;
};

class _NSDKDCLASSE NSPhraseGeste : public NSPhraseObjet
{
	public :

    NSPhraseObjArray Temporalite ;    // Gestion du temps
    NSPhraseObjArray Garbage ;        // Objets inclassables

    void decode(int colonne) ;
    void metPhrase() ;

    NSPhraseGeste(NSContexte* pCtx, int iDecoType = dcPhrase, string sLangue = "fr") ;
    NSPhraseGeste(decodageBase* pBase, int iDecoType = dcPhrase, string sLangue = "fr") ;
    ~NSPhraseGeste() ;

    NSPhraseGeste(const NSPhraseGeste& rv) ;

    NSPhraseGeste& operator=(const NSPhraseGeste& src) ;
};

class _NSDKDCLASSE NSPhrasePrescript : public NSPhraseObjet
{
	public :

    NSPhraseObjet    _Forme ;
    NSPhraseMotTime  _Dates ;

    NSPhraseObjArray _Administration ;    // Mode d'administration
    NSPhraseObjet    _Event ;             // en cas de douleur

    int              _iQuantitePhases ;
    NSPhraseObjArray _Phases ;            // Horaire et quantit� d'une prise

    NSPhraseObjArray _Garbage ;           // Objets inclassables
    NSPhraseMotArray _Details ;
    NSPhraseObjArray _FreeText ;

    string           _sNonSubstituable ;  // French only

    bool             _bNumsAsText ;

    void decode(int colonne) ;
    void metPhrase(string decDeb = string(""), string decFin = string(""), int sautLigne = 0) ;
    void metPhraseFreeText(string decDeb = string(""), string decFin = string(""), int sautLigne = 0) ;

    NSPhrasePrescript(NSContexte* pCtx, int iDecoType = dcPhrase, string sLangue = string("fr"), bool bAllLettersSentences = false) ;
    NSPhrasePrescript(decodageBase* pBase, int iDecoType = dcPhrase, string sLangue = string("fr"), bool bAllLettersSentences = false) ;
    ~NSPhrasePrescript() ;

    bool isNarcotic() ;

    NSPhrasePrescript(const NSPhrasePrescript& rv) ;

    NSPhrasePrescript& operator=(const NSPhrasePrescript& src) ;
};

class _NSDKDCLASSE NSPhraseTemporel : public NSPhraseObjet
{
	public :

    // enum TYPEEVENEMENT { TpsTypeUndefined = 0, TpsDuration, TpsDate} ;  // Dure 2h ou Dans 2h
    // TYPEEVENEMENT iTypeTps ;
    TIME_EVENT iTypeTps ;

    // enum TYPEFORME { TpsFormUndefined = 0, TpsInstant, TpsInterval} ; //
    // TYPEFORME iFormeTps ;
    TIME_FORM iFormeTps ;

    // enum TYPEREPERE { TpsFrameUndefined = 0, TpsPresent, TpsPast, TpsFutur} ;
    // TYPEREPERE iRepererTps ;
    TIME_FRAME iRepererTps ;

    NSPatPathoInfo   ValeurMin ;    // Valeur min
    bool             bMinNow ;      // De maintenant � plus tard
    NSPatPathoInfo   ValeurMax ;    // Valeur max
    bool             bMaxNow ;      // D'une date � maintenant
    NSPatPathoInfo   ValeurDuree ;  // Valeur max

    NSPhraseTemporel(NSContexte* pCtx, int iDecoType = dcPhrase, string sLangue = "fr") ;
    NSPhraseTemporel(decodageBase* pBase, int iDecoType = dcPhrase, string sLangue = "fr") ;
    ~NSPhraseTemporel() ;

    void decode(int colonne, bool bMinValue, TIME_EVENT iEvnt = TpsTypeUndefined ) ;  // previous default value was TpsDuree
    void decode(int colonne) { decode(colonne, true, TpsTypeUndefined) ; }
    void metPhrase() ;
    void initPhraseMotTime(NSPhraseMotTime* pPMT) ;

    NSPhraseTemporel(const NSPhraseTemporel& rv) ;

    NSPhraseTemporel& operator=(const NSPhraseTemporel& src) ;

    void   setTypeTps(TIME_EVENT iTypeTps)   { iTypeTps    = iTypeTps ;  }
    void   setFormeTps(TIME_FORM iFormeTps)  { iFormeTps   = iFormeTps ; }
    void   setRepererTps(TIME_FRAME iReTps)  { iRepererTps = iReTps ;    }
};

class _NSDKDCLASSE NSPhraseTempoCycle : public NSPhraseObjet
{
	public :

    enum TYPECYCLE { Regulier = 1, Libre } ;
    TYPECYCLE iTypeCycle ;

    NSPatPathoInfo  eventDuration ;     // dur�e de l'�v�nement
    NSPatPathoInfo  cycleDuration ;     // dur�e du cycle
    int             iEventNb ;          // Nombre d'�v�nements

    NSPhraseTempoCycle(NSContexte* pCtx, int iDecoType = dcPhrase, string sLangue = "fr") ;
    NSPhraseTempoCycle(decodageBase* pBase, int iDecoType = dcPhrase, string sLangue = "fr") ;
    ~NSPhraseTempoCycle() ;

    void decode(int colonne) ;
    void metPhrase() ;
    void initCycle(NSPhraseMotTimeCycle* pCycle) ;
    bool estVide() ;

    NSPhraseTempoCycle(const NSPhraseTempoCycle& rv) ;

    NSPhraseTempoCycle& operator=(const NSPhraseTempoCycle& src) ;
};

class _NSDKDCLASSE NSPhrasePhase : public NSPhraseObjet
{
	public :

    bool                isDuration ;
    NSPhraseTemporel    phrDuree ;
    int                 iNbRenouv ;

    NSPhraseMotTime     DatePhase ;
    NSPhraseMotTime     DateRenouv ;

    int                 iQuantiteCycles ;
    NSPhraseObjArray    Cycles ;

    NSPhrasePrescript*  pPrescript ;

    void decode(int colonne) ;
    void metPhrase() ;

    NSPhrasePhase(NSContexte* pCtx, int iDecoType = dcPhrase, string sLangue = "fr") ;
    NSPhrasePhase(decodageBase* pBase, int iDecoType = dcPhrase, string sLangue = "fr") ;
    ~NSPhrasePhase() ;

    NSPhrasePhase(const NSPhrasePhase& rv) ;

    NSPhrasePhase& operator=(const NSPhrasePhase& src) ;
};

class _NSDKDCLASSE NSPhraseCycle : public NSPhraseObjet
{
	public :

    NSPhraseObjArray    Cycle_circadien ;           // Horaire et quantit� d'une prise
    NSPhraseObjArray    Rythme ;

    void decode(int colonne) ;
    void metPhrase() ;

    NSPhraseCycle(NSContexte* pCtx, int iDecoType = dcPhrase, string sLangue = "fr") ;
    NSPhraseCycle(decodageBase* pBase, int iDecoType = dcPhrase, string sLangue = "fr") ;

    NSPhraseCycle(const NSPhraseCycle& rv) ;

    NSPhraseCycle& operator=(const NSPhraseCycle& src) ;
};

class _NSDKDCLASSE NSPhrasePrise : public NSPhraseObjet
{
	public :

    NSPatPathoInfo  nbDose ;        // Valeur  : 2
    NSPatPathoInfo  ValeurDose ;    // Valeur  : 2 fois 500 grammes
    NSPatPathoInfo  ObjetDose ;     // Forme   : 2 comprim�s

    NSPatPathoInfo  Moment ;        // le matin
    NSPhraseMotTime Temporalite ;   // A 2 heures, (2 heures apr�s le lever ?)
    NSPhraseTempoCycle TempoCycle ; // Cycle

    void decode(int colonne) ;
    void metPhrase() ;

    NSPhrasePrise(NSContexte* pCtx, int iDecoType = dcPhrase, string sLangue = "fr") ;
    NSPhrasePrise(decodageBase* pBase, int iDecoType = dcPhrase, string sLangue = "fr") ;

    NSPhrasePrise(const NSPhrasePrise& rv) ;

    NSPhrasePrise& operator=(const NSPhrasePrise& src) ;
};

/*
class _NSDKDCLASSE NSPhrasePrescript : public NSPhraseObjet
{
    public :

        NSPhraseObjet       Forme ;
        NSPhraseMotTime     Dates ;

        NSPhraseObjArray    Administration ;       // Mode d'administration
        NSPhraseObjArray    VoieAdministration ;   // Mode d'administration
        NSPhraseObjArray    Localisations ;        // Organes ou parties d'organes

        int                 iQuantiteJour ;
        NSPhraseObjArray    PrisesJour ;           // Horaire et quantit� d'une prise

        NSPhraseObjArray    Rythme;                // Rythme de prise

        NSPhraseObjArray    Temporalite;           // Gestion du temps
        NSPhraseObjArray    Garbage;               // Objets inclassables
        NSPhraseMotArray    Details;

        void decode(int colonne);
        void metPhrase(string decDeb = "", string decFin = "", int sautLigne = 0);

        NSPhrasePrescript(NSContexte* pCtx, int iDecoType = dcPhrase, string sLangue = "fr");
        NSPhrasePrescript(decodageBase* pBase, int iDecoType = dcPhrase, string sLangue = "fr");

        NSPhrasePrescript(NSPhrasePrescript& rv);

        NSPhrasePrescript& operator=(NSPhrasePrescript src);
};

class _NSDKDCLASSE NSPhrasePrise : public NSPhraseObjet
{
    public :

        NSPatPathoInfo   nbDose;        // Valeur  : 2
        NSPatPathoInfo   ValeurDose;    // Valeur  : 2 fois 500 grammes
        NSPatPathoInfo   ObjetDose;     // Forme   : 2 comprim�s

        NSPatPathoInfo   Moment ;       // le matin
        NSPhraseMotTime  Temporalite ;  // A 2 heures, (2 heures apr�s le lever ?)

        void decode(int colonne);
        void metPhrase();

        NSPhrasePrise(NSContexte* pCtx, int iDecoType = dcPhrase, string sLangue = "fr");
        NSPhrasePrise(decodageBase* pBase, int iDecoType = dcPhrase, string sLangue = "fr");

        NSPhrasePrise(NSPhrasePrise& rv);

        NSPhrasePrise& operator=(NSPhrasePrise src);
};
*/

class _NSDKDCLASSE NSPhraseQualificatif : public NSPhraseObjet
{
	public :

    NSPhraseObjArray Temporalite ;   // Gestion du temps
    NSPhraseObjArray Garbage ;       // Objets inclassables

    NSPhraseQualificatif(NSContexte* pCtx, int iDecoType = dcPhrase, string sLangue = "fr") ;
    NSPhraseQualificatif(decodageBase* pBase, int iDecoType = dcPhrase, string sLangue = "fr") ;

    NSPhraseQualificatif(const NSPhraseQualificatif& rv) ;

    NSPhraseQualificatif& operator=(const NSPhraseQualificatif& src) ;
};

class _NSDKDCLASSE NSPhraseBiometrie : public NSPhraseObjet
{
	public :

    NSPatPathoInfo   Valeur ;         // Valeur
    NSPhraseObjArray Normales ;       // Normales

    NSPhraseObjArray Methode ;        // Methode de calcul, conditions
    NSPhraseObjArray Localisation ;   // O�

    NSPhraseObjArray Temporalite ;   // Gestion du temps
    NSPhraseObjArray Garbage ;       // Objets inclassables

    NSPhraseBiometrie(NSContexte* pCtx, int iDecoType = dcPhrase, string sLangue = "fr");
    NSPhraseBiometrie(decodageBase* pBase, int iDecoType = dcPhrase, string sLangue = "fr");

    void decode(int colonne) ;
    void metPhrase() ;

    NSPhraseBiometrie(const NSPhraseBiometrie& rv) ;

    NSPhraseBiometrie& operator=(const NSPhraseBiometrie& src) ;
};

#endif  // __NSPHROBJ_H

