// -----------------------------------------------------------------------------// nscaptur.h// -----------------------------------------------------------------------------
// Contient les définitions des objets intervenant dans la capture
// -----------------------------------------------------------------------------
// PA Septembre 2001
// -----------------------------------------------------------------------------
#ifndef __NSCAPTUR_H__
# define __NSCAPTUR_H__

class NSPatPathoArray ;
class NSUtilLexique ;
class NSSejourInfo ;

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsglobalLus.h"
#else
  # include <owl/window.h>
  # include <owl/dialog.h>
  # include <owl/listbox.h>
  # include <owl/edit.h>

  # include <vcl/windows.hpp>

  #include "partage/nsglobal.h"
#endif

# include "partage\ns_vector.h"
# include "partage\NTArray.h"
// # include "nsbb\nsednum.h"

# if defined(_DANSLEXIDLL)
#  define _CLASSELEXI __export
# else
#  define _CLASSELEXI __import
# endif

# ifdef _EXT_CAPTURE
#  include "ns_capture\nscaptmanager.h"
# endif

# ifndef _EXT_CAPTURE
class NSOCRbloc ;

enum CAPTUREFROM { FromUnknown = 0, FromOutside, FromRechDialog, FromInPatientsList, FromMailBox} ;

// -----------------------------------------------------------------------------
// Objets de capture Episodus
// -----------------------------------------------------------------------------
class _CLASSELEXI NSCapture : public NSRoot
{
  private:

 	  static long lObjectCount ;

  public:

    string _sChemin ;
    string _sLibelle ;
    string _sUnit ;
    string _sDate ;

    string _sClassifier ;
    string _sClassifResultO ;
    string _sClassifResultP ;
    string _sClassifResultI ;
    string _sClassifResult3 ;

    NSOCRbloc* _pOCRBloc ;

    CAPTUREFROM _iCapturedFrom ;

    // Constructeurs
    NSCapture(NSContexte* pCtx) ;
    NSCapture(NSContexte* pCtx, string sChem, string sLibel, CAPTUREFROM iFrom, string sClassif = string(""), string sUnite = string(""), string sDataDate = string("")) ;
    NSCapture(NSContexte* pCtx, NSOCRbloc* pBloc) ;
    NSCapture(const NSCapture& rv) ;
    // Destructeur
    virtual ~NSCapture() { lObjectCount-- ; }

    bool prendChemin() ;

    NSCapture& operator=(const NSCapture& src) ;

    string      getChemin()         { return _sChemin ; }
    string      getLibelle()        { return _sLibelle ; }
    string      getUnit()           { return _sUnit ; }
    string      getDate()           { return _sDate ; }
    CAPTUREFROM getFrom()           { return _iCapturedFrom ; }
    string      getClassifier()     { return _sClassifier ; }
    string      getClassifResultO() { return _sClassifResultO ; }
    string      getClassifResultP() { return _sClassifResultP ; }
    string      getClassifResultI() { return _sClassifResultI ; }
    string      getClassifResult3() { return _sClassifResult3 ; }

    void setChemin(string sC)               { _sChemin = sC ; }
    void setLibelle(string sL)              { _sLibelle = sL ; }
    void setUnit(string sU)                 { _sUnit = sU ; }
    void setDate(string sD)                 { _sDate = sD ; }
    void setCapturedFrom(CAPTUREFROM iFrom) { _iCapturedFrom = iFrom ; }
    void setClassifier(string sC)           { _sClassifier = sC ; }
    void setClassifResultO(string sC)       { _sClassifResultO = sC ; }
    void setClassifResultP(string sC)       { _sClassifResultP = sC ; }
    void setClassifResultI(string sC)       { _sClassifResultI = sC ; }
    void setClassifResult3(string sC)       { _sClassifResult3 = sC ; }

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }
} ;

typedef vector<NSCapture *>             NSCaptureVector ;
typedef NSCaptureVector::iterator       CaptureIter ;
typedef NSCaptureVector::const_iterator CaptureCIter ;

class _CLASSELEXI NSCaptureArray : public NSCaptureVector, public NSRoot
{
  private:

 	  static long lObjectCount ;

  public:

    // Constructeurs
    NSCaptureArray(NSContexte* pCtx) ;
    NSCaptureArray(const NSCaptureArray& rv) ;
    // Destructeur
    virtual ~NSCaptureArray() ;

    int        trouveChemin(string sChem, string* pLib, int pos = 0) ;
    NSCapture* trouveCheminExact(string sChem, string* pLib, NSCapture* pStartPoint = (NSCapture*) 0) ;

    void       getIdentity(string &Nom, string &Prenom, bool bUsePatronym = true) ;
    void       getSejourInfo(NSSejourInfo *pSejour) ;

    void vider() ;
    void viderForOrigin(CAPTUREFROM iFrom) ;
    void ajouter(NSCapture* pNew) ;
    void append(NSCaptureArray* pNewCapture) ;

    void eraseEntry(string sPath) ;

    NSCaptureArray& operator=(const NSCaptureArray& src) ;

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }
} ;

// -----------------------------------------------------------------------------
// CONTEXTE DE CAPTURE (Chemin)
// -----------------------------------------------------------------------------
class NSCaptureContexte
{  public:

    string sChemin ;
    string sLibelle ;
    string sClassifier ;

    NSCaptureContexte(string sChem = string(""), string sLib = string(""), string sClassif = string("")) ;
    NSCaptureContexte(const NSCaptureContexte& rv) ;

    // Destructeur
    ~NSCaptureContexte() ;

    NSCaptureContexte& operator=(const NSCaptureContexte& src) ;
} ;

// -----------------------------------------------------------------------------
// Définition de NSProtocoleArray (Array de NSProtocole(s))
// -----------------------------------------------------------------------------
typedef vector<NSCaptureContexte *>     NSCaptCtxArray ;
typedef NSCaptCtxArray::iterator        IterCaptCtx ;
typedef NSCaptCtxArray::const_iterator  CIterCaptCtx ;

class _CLASSELEXI NSCaptureCtxArray : public NSCaptCtxArray, public NSRoot
{
  public:

    // Constructeurs
    NSCaptureCtxArray(NSContexte* pCtx) : NSCaptCtxArray(), NSRoot(pCtx) {}
    NSCaptureCtxArray(const NSCaptureCtxArray& rv) ;

    // Destructeur
    virtual ~NSCaptureCtxArray() ;
    void vider() ;

    bool charger() ;
} ;
#endif // !_EXT_CAPTURE#ifndef _ENTERPRISE_DLL// -----------------------------------------------------------------------------
// Objet "Boite de dialogue" utilisé pour sélectionner un contexte
// -----------------------------------------------------------------------------
class _CLASSELEXI ChoixCaptureContexte : public TDialog, public NSRoot
{
  public:

    NSCaptureContexte*  pCaptCtx ;
    OWL::TListBox* 		  pContexteBox ;
    OWL::TEdit* 		    pEditLib ;
    NSCaptureCtxArray   contexteArray ;
    UINT				        iContexteChoisi ;    string*             pCheminChoisi ;    string*             pClassifier ;
    string*             pLibelle ;

    bool                bCanChangeLib ;

    NSOCRbloc*          pOCRBloc ;

    ChoixCaptureContexte(TWindow*, NSContexte* pCtx, string* pChemin, string* pClassif, string* pLibel, bool bCanChangeLib, NSOCRbloc* pBloc = (NSOCRbloc*) 0) ;
    ~ChoixCaptureContexte() ;

    void SetupWindow() ;

    void CmSelectProto(WPARAM Cmd) ;
    void CmCancel() ;
    void CmOk() ;
    void CmProtoDblClk(WPARAM Cmd) ;
    void CmLearn() ;

    void OuvreDocument(int NumDoc) ;

  DECLARE_RESPONSE_TABLE(ChoixCaptureContexte) ;
} ;
#endif   // #ifndef _ENTERPRISE_DLL

// -----------------------------------------------------------------------------
// FORMAT DE CAPTURE
// -----------------------------------------------------------------------------
class NSCaptureFormat
{
  public:

    string sTitre ;
    string sExpression ;

    NSCaptureFormat(string sTit = "", string sExpres = "") ;
    NSCaptureFormat(const NSCaptureFormat& rv) ;

    // Destructeur
    ~NSCaptureFormat() ;

    NSCaptureFormat& operator=(const NSCaptureFormat& src) ;
} ;

// -----------------------------------------------------------------------------
// Définition de NSProtocoleArray (Array de NSProtocole(s))
// -----------------------------------------------------------------------------
typedef vector<NSCaptureFormat *>       NSCaptFmtArray ;
typedef NSCaptFmtArray::iterator        IterCaptFmt ;
typedef NSCaptFmtArray::const_iterator  CIterCaptFmt ;

class _CLASSELEXI NSCaptureFormatArray : public NSCaptFmtArray, public NSRoot
{
  public:

    // Constructeurs
    NSCaptureFormatArray(NSContexte* pCtx) : NSCaptFmtArray(), NSRoot(pCtx) {}
    NSCaptureFormatArray(const NSCaptureFormatArray& rv) ;

    // Destructeur
    virtual ~NSCaptureFormatArray() ;
    void vider() ;

    bool   charger() ;

    string getFormatExpression(string sFormatTitle) ;

    void   formatElement(string *psText, string sFormat, bool bForced = true) ;
    static void format(string *psText, string sFormat, bool bForced = true) ;
    static void formatForDisplay(string *psText, string sFormat, string sDisplayFormat, bool bForced = true) ;

    static void getNonBlankChars(string* psTarget, string* psText, size_t &iPosInText, size_t &iPosInExpr, size_t iPosEndSet) ;
    static void getMultipleNonBlankChars(string* psTarget, string* psText, size_t &iPosInText) ;
    void   preCompare(string* psText, string* psModel) ;
    static bool smartCompare(string* psText, string* psModel) ;
} ;

#ifndef _ENTERPRISE_DLL
// -----------------------------------------------------------------------------
// Objet "Boite de dialogue" utilisé pour sélectionner un format
// -----------------------------------------------------------------------------
class _CLASSELEXI ChoixCaptureFormat : public TDialog, public NSRoot
{
  public:

    NSCaptureFormat*        pCaptFmt ;
    OWL::TListBox* 		      pContexteBox ;
    OWL::TEdit* 		        pEditLib ;
    NSCaptureFormatArray    formatArray ;

    UINT				            iContexteChoisi ;

    string*                 pTitre ;
    string*                 pModele ;

    ChoixCaptureFormat(TWindow*, NSContexte* pCtx, string* pTitle, string* pModel) ;
    ~ChoixCaptureFormat() ;

    void SetupWindow() ;

    void CmSelectProto(WPARAM Cmd) ;
    void CmCancel() ;
    void CmOk() ;
    void CmProtoDblClk(WPARAM Cmd) ;

    void OuvreDocument(int NumDoc) ;

  DECLARE_RESPONSE_TABLE(ChoixCaptureFormat) ;
} ;

class _CLASSELEXI ChoixCaptureUnit : public TDialog, public NSRoot
{
  public:

    NSUtilLexique*  pType ;
    string* 	    pTypeUnit ;

    ChoixCaptureUnit(TWindow* pere, NSContexte* pCtx, string* pTypeUnite) ;    ~ChoixCaptureUnit() ;

    void CmOk() ;
    void CmCancel() ;
    void SetupWindow() ;

 DECLARE_RESPONSE_TABLE(ChoixCaptureUnit) ;
} ;

class _CLASSELEXI ChoixCaptureModelName : public TDialog, public NSRoot
{
  public:

    OWL::TEdit* pNameEdit ;
    string*     pNewName ;

    ChoixCaptureModelName(TWindow* pere, NSContexte* pCtx, string* pName) ;    ~ChoixCaptureModelName() ;

    void CmOk() ;
    void CmCancel() ;
    void SetupWindow() ;

  DECLARE_RESPONSE_TABLE(ChoixCaptureModelName) ;
} ;
#endif    // #ifndef _ENTERPRISE_DLL

//
// Partie "analysée" de la chaine de capture
//
class _CLASSELEXI flexObject
{
	public:

		flexObject(string sLibel, int iStartPos, int iEndPos, string sCod) ;
		flexObject(const flexObject &rv) ;
		~flexObject() ;

		flexObject& operator=(const flexObject& src) ;		bool        operator==(const flexObject& src) ;
		string     sLibelle ;       // libellé		string     sCode ;          // code Nautilus
		int        iStartPosition ; // position de départ dans la chaîne
		int        iEndPosition ;   // position de fin dans la chaîne

		NSCapture* pGenerated ;
};

typedef vector<flexObject*>         ArrayOfFlex ;
typedef ArrayOfFlex::iterator       FlexIter ;
typedef ArrayOfFlex::const_iterator FlexConstIter ;
typedef NTArray<flexObject>         FlexArray ;

class ParseElemArray ;

class _CLASSELEXI analysedCapture : public NSRoot
{
  private:

 	  static long lObjectCount ;

	public:

		string           _sPath ;       // Chemin des données capturées : ex GCONS1
    string           _sClassifier ; // Classification à utiliser
    string           _sUnit ;       // Unité

		string           _sTexte ;     // Texte capturé
		FlexArray        _aFlex ;      // Array de portions "analysées"
    bool             _bDisplayed ;

		NSOCRbloc*       _pBloc ;

    NSCapture*       _pPere ;
    analysedCapture* _pFather ;

    NSCaptureArray*  _pCaptureArray ;

    CAPTUREFROM      _iCapturedFrom ;

		analysedCapture(NSContexte *pCtx, string sText, NSCaptureArray* pCaptArray, CAPTUREFROM iFrom, string sPath = "", string sClassif = "", string sUnite = "") ;
		analysedCapture(NSContexte *pCtx, NSOCRbloc* pOcrBloc, NSCaptureArray* pCaptArray, CAPTUREFROM iFrom, string sPath = "", string sClassif = "", string sUnite = "") ;
		analysedCapture(NSContexte *pCtx, NSCaptureArray* pCaptArray, NSCapture* pCapture, analysedCapture* pCaptFather = 0) ;

    NSCapture*       analyse(analysedCapture** pSecondary) ;
    void             getSemanticParseElemArray(ParseElemArray* pRawTermsArray, ParseElemArray* pSemTermsArray) ;

    analysedCapture* findNumValues(ParseElemArray* pRawTermsArray, ParseElemArray* pSemTermsArray) ;
    string           getNumPattern(ParseElemArray* pRawTermsArray, ParseElemArray* pSemTermsArray, bool* pbFullSuccess = 0) ;
    string           getElementByTagInNumPattern(string sPattern, char cTag) ;
    void             numPatternToTree(string* pPattern, NSPatPathoArray* pPPT) ;
    bool             addToNumPattern(string* pPattern, string* pLibelle) ;
    analysedCapture* computeNumPattern(string* pPattern) ;

    void             findPathologies(ParseElemArray* pRawTermsArray, ParseElemArray* pSemTermsArray) ;

    void             getWaitingNumInfo(string* pPattern, string* pPath, string* pValue, string* pUnit) ;
    analysedCapture* treatWaitingNumInfo(string* pPath, string* pValue, string* pUnit) ;

		analysedCapture(const analysedCapture &rv) ;
		~analysedCapture() ;

    string          getText()         { return _sTexte ; }
    bool            isDisplayed()     { return _bDisplayed ; }
    NSCaptureArray* getCaptureArray() { return _pCaptureArray ; }
    NSOCRbloc*      getOcrBloc()      { return _pBloc ; }

    void   setDisplayed(bool bDisp) { _bDisplayed = bDisp ; }

		analysedCapture& operator=(const analysedCapture& src) ;		bool             operator==(const analysedCapture& src) ;    static long getNbInstance()  { return lObjectCount ; }    static void initNbInstance() { lObjectCount = 0 ; }};

typedef vector<analysedCapture*>      AnaCaptVector ;
typedef AnaCaptVector::iterator       AnaCaptIter ;
typedef AnaCaptVector::const_iterator AnaCaptConstIter ;
typedef NTArray<analysedCapture>      AnaCaptArray ;

//
// *************** CAPTURE MODELS ***************
//

# ifndef _EXT_CAPTURE

# define LABEL_CAPTURE_ENTRY          "entry"
# define LABEL_CAPTURE_COORDS         "coords"
# define LABEL_CAPTURE_COORDS_TOP     "top"
# define LABEL_CAPTURE_COORDS_LEFT    "left"
# define LABEL_CAPTURE_COORDS_RIGHT   "right"
# define LABEL_CAPTURE_COORDS_BOTTOM  "bottom"
# define LABEL_CAPTURE_CLASS          "class"
# define LABEL_CAPTURE_ORDER          "order"
# define LABEL_CAPTURE_LABEL          "label"
# define LABEL_CAPTURE_CORRESPONDCE   "correspondence"
# define LABEL_CAPTURE_CAPTURE_TYPE   "capture"
# define LABEL_CAPTURE_CAPTURE_SYSTEM "system"
# define LABEL_CAPTURE_CAPTURE_OCR    "OCR"
# define LABEL_CAPTURE_ANALYSE_TYPE   "analyse"
# define LABEL_CAPTURE_ANALYSE_DFLT   "default"
# define LABEL_CAPTURE_ANALYSE_PLACMT "placement"
# define LABEL_CAPTURE_ANALYSE_ORDER  "order"
# define LABEL_CAPTURE_ANALYSE_PANDO  "placement+order"
# define LABEL_CAPTURE_ANALYSE_LIBEL  "libelle"
# define LABEL_CAPTURE_ANALYSE_REL_O  "relativeOrder"
# define LABEL_CAPTURE_ANALYSE_REL_P  "relativePlacement"
# define LABEL_CAPTURE_ANALYSE_NEVER  "never"
# define LABEL_CAPTURE_CLASSIFY       "classify"
# define LABEL_CAPTURE_FORMAT         "format"
# define LABEL_CAPTURE_UNIT           "unit"

// -----------------------------------------------------------------------------
//
// Classe EpisodusElemData
//
// -----------------------------------------------------------------------------
#ifndef _ENTERPRISE_DLL
//class ChildEpisodusData
class _CLASSELEXI EpisodusElemData
{
  public:

    unsigned long   AppWindow ;
    unsigned long   AppWndChild ;
    string          sClassChild ;
    string          sTextChild ;
    string          sCorrespondence ;
    string          sClassifier ;
    string          sFormat ;
    string          sUnit ;
    ClassLib::TRect Placement ;

    enum TYPECAPTURE    { captureSystem = 1, captureOCR } ;    enum TYPEANALYSIS   { analyseDefault = 1, analysePlacement, analyseOrder, analysePlacementAndOrder, analyseLibelle, analyseRelativeOrder, analyseRelativePlacement, analyseNever } ;    int	            iModeCapture ;    int	            iModeAnalyse ;    int             iOrder ;    // Constructeurs - Destructeur
    EpisodusElemData() ;
    EpisodusElemData(const EpisodusElemData& rv) ;
    EpisodusElemData(EpisodusElemData *pNewEpisodusElemData) ;
    EpisodusElemData(string sXMLBaliseContenu, int iGivenOrder) ;
    EpisodusElemData(TWindow::THandle aWindow, TWindow::THandle aWndChild,
                     string aClassChild, string aTextChild,
                     ClassLib::TRect aPlacement, int iOrd,
                     string aCorrespondence = string(""),
                     int iModCapt = captureSystem,
                     int iModAnalys = analyseDefault,
                     string sClassifier = string(""),
                     string sFormatage = string(""), string sUnite = string("")) ;
    ~EpisodusElemData() ;

    bool    ImportModelXML(string sXMLBaliseContenu) ;

    // fonction de comparaison
    bool    CompareWithElementModel(EpisodusElemData *, int iDefaultAnalyse)  ;
    bool    CompareWithRelativeElementModel(EpisodusElemData * pModel, EpisodusElemData * pRepereCapture, EpisodusElemData * pRepereModel) ;

    EpisodusElemData& operator=(const EpisodusElemData& src) ;
} ;

// définition d'un array représentant la fenêtre de capture
typedef vector<EpisodusElemData *>        ArrayEpisodusData ;
typedef ArrayEpisodusData::iterator       ArrayEpisodusDataIter ;
typedef ArrayEpisodusData::const_iterator ArrayEpisodusDataCIter ;

class _CLASSELEXI EpisodusData : public ArrayEpisodusData
{
  public:

    enum TYPEANALYSE { byPlacement = 1, byOrder, byPlacementAndOrder } ;
    int     iModeAnalyse ;

    // Constructeurs - Destructeur
    EpisodusData() ;
    EpisodusData(const EpisodusData& rv) ;
    EpisodusData(EpisodusData *pNewEpisodusData) ;
    EpisodusData(string sXMLBaliseContenu) ;
    ~EpisodusData() ;

    void    Reorder() ;

    // Fonctions de Correspondance    bool    SetElement(size_t index, string sLabel) ;    bool    SetClassifElement(size_t index, string sLabel) ;
    bool    SetFormatElement(size_t index, string sLabel) ;
    bool    SetUnitElement(size_t index, string sLabel) ;
    bool    SetAnalyseTypeElement(size_t index, int iAnalyse) ;

    EpisodusElemData * getElement(size_t index) ;

    // fonction d'importation exportation
    bool    ExportModel(ofstream * pOutFile) ;
    bool    ImportModel() ;
    bool    ImportModel(string sXMLBaliseContenu) ;
    bool    Model2View(EpisodusData * pEpisodDataView) ;

    bool    SaveModel() ;

    // fonctions de comparaison
    bool    CompareWithModel(EpisodusData*, bool bExact) ;
    bool    captureSubModel(EpisodusData* pCompareData, NSCaptureArray* pCaptureArray, ArrayEpisodusData* pCapturedElements, ArrayEpisodusData* pModelElements) ;

    void    vider() ;

    EpisodusData& operator=(const EpisodusData& src) ;
} ;

typedef EpisodusData::iterator       EpisodusDataIter ;
typedef EpisodusData::const_iterator EpisodusDataCIter ;

class _CLASSELEXI EpisodusModel : public EpisodusData
{
  public:

    string      sNom ;
    bool        bModified ;
    EpisodusModel() ;    EpisodusModel(const EpisodusModel& rv) ;    EpisodusModel(EpisodusData *pNewEpisodusData) : EpisodusData(pNewEpisodusData) {}
    EpisodusModel(string sXMLBaliseContenu, string sXMLBaliseVariables) ;
    ~EpisodusModel() ;

    bool ExportModel(ofstream * pOutFile, string sType) ;
    bool useOCR() ;

    EpisodusModel& operator=(const EpisodusModel& src) ;
} ;

// définition d'un array de modèles
typedef vector<EpisodusModel *>                  ArrayEpisodusModelesData ;
typedef ArrayEpisodusModelesData::iterator       ArrayEpisodusModelIter ;
typedef ArrayEpisodusModelesData::const_iterator ArrayEpisodusModelCIter ;

class _CLASSELEXI ArrayEpisodusModelData : public ArrayEpisodusModelesData
{
  public:

    // Constructeurs - Destructeur
    ArrayEpisodusModelData() : ArrayEpisodusModelesData() {}
    ArrayEpisodusModelData(const ArrayEpisodusModelData& rv) ;
    ~ArrayEpisodusModelData() ;

    EpisodusModel* trouveModele(EpisodusData * pEpisodusData) ;
    EpisodusModel* findModelByName(string sModelName) ;
    // bool            captureSubModels(EpisodusData * pCapturedData, NSCaptureArray * pCaptureArray, NSEpisodus * pEpisod) ;

    bool    sauvegarder(ofstream *pOutFile, string sType) ;
    void    vider() ;    ArrayEpisodusModelData& operator=(const ArrayEpisodusModelData& src) ;} ;
#endif   // #ifndef _ENTERPRISE_DLL

# endif // !_EXT_CAPTURE

class _CLASSELEXI EpisodusModelXML
{
  public:

    EpisodusModelXML() {}
    ~EpisodusModelXML() {}
    bool ImportModelXML(string input, size_t pos = 0) ;

    vector<string> sArrayBalise ;
    vector<string> sArrayBaliseVariables ;
    vector<string> sArrayBaliseContenu ;
};

#endif // !__NSCAPTUR_H__

