// -----------------------------------------------------------------------------// ns_html.h//
// Contient les définitions des objets HTML associés à chaque type NAUTILUS
//
// 1ere version : RS juin 97   Dernière modif : 23/07/97
// -----------------------------------------------------------------------------

#ifndef __NS_HTML_H# define __NS_HTML_H

// # include <owl/dialog.h>
// # include <owl/richedit.h>

class NSLdvPubli ;
class NSCoopChartDocument ;
class NSFactData ;

# include "nautilus\nsmodhtm.h"
# include "nsbb\nsRichEdit.h"

class NSHtmlHistoIndex : public NSModHtml
{
  public:

    NSBlocHtmlArray ligneBlocHtml ;

    NSHtmlHistoIndex(OperationType typeOp, NSContexte *pCtx) ;
    ~NSHtmlHistoIndex() ;

    void  ajouteBloc(NSHtml *ph) ;
    void  ajouteLigne(string sDocLabel, string sDocExportFile, string sDocId, string sDocDate, string sDocName) ;
} ;

class NSHtmlCR : public NSModHtml
{
  public:

    NSHtmlCR(OperationType typeOp, NSNoyauDocument *pDocNoy, NSContexte *pCtx, string sDest = "") ;
    NSHtmlCR(NSNoyauDocument *pDocNoy, NSContexte *pCtx) ;
    ~NSHtmlCR() ;

    HtmlTypes typeHtml(char cc) ; // type Html <- caractère type
    string    lireTexte();
    bool      lireFils(NSHtml *pere, HtmlTypes typeDebut, string sChemin) ;
    bool      lireCR(string sFichier) ;
    bool      buildHtmlTree(NSHtml *pCR) ;

  protected:

    string  _sSource ;
    size_t  _iCc ;
} ;

class NSHtmlCS : public NSModHtml
{
  public:

    NSHtmlCS(OperationType typeOp, NSNoyauDocument *pDocNoy, NSContexte *pCtx, string sDest = "") ;
    ~NSHtmlCS() ;
} ;

class NSHtmlCQ : public NSModHtml
{
  public:

    NSHtmlCQ(OperationType typeOp, NSNoyauDocument *pDocNoy, NSContexte *pCtx, string sDest = "") ;
    ~NSHtmlCQ() ;
} ;

class NSHtmlHisto : public NSModHtml
{
  public:

    NSHtmlHisto(OperationType typeOp, NSNoyauDocument *pDocNoy, NSContexte *pCtx, string sDest = "") ;
    ~NSHtmlHisto() ;
} ;

class NSHtmlQoL : public NSModHtml
{
  public:

    NSHtmlQoL(OperationType typeOp, NSNoyauDocument *pDocNoy, NSContexte *pCtx, string sDest = "") ;
    ~NSHtmlQoL() ;

    void buildHtml(NSCoopChartDocument *pQoLDoc) ;
} ;

class NSHtmlGoals : public NSModHtml
{
  public:

    NSBlocHtmlArray ligneBlocHtml ;
    NSLdvPubli      *_pPubliDriver ;

    NSHtmlGoals(OperationType typeOp, NSLdvDocument *pDocNoy, NSContexte *pCtx, NSLdvPubli *pPubliDriver, string sDest = "") ;
    ~NSHtmlGoals() ;

    void  ajouteBloc(NSHtml *ph) ;
    void  ajouteLignes() ;
} ;

class NSHtmlDrugs : public NSModHtml
{
  public:

    NSBlocHtmlArray ligneBlocHtml ;
    NSLdvPubli      *_pPubliDriver ;

    NSHtmlDrugs(OperationType typeOp, NSLdvDocument *pDocNoy, NSContexte *pCtx, NSLdvPubli *pPubliDriver, string sDest = "") ;
    ~NSHtmlDrugs() ;

    void  ajouteBloc(NSHtml *ph) ;
    void  ajouteLignes() ;
} ;

class NSHtmlSecu : public NSModHtml
{
  public:

    NSHtmlSecu(OperationType typeOp, NSNoyauDocument *pDocNoy, NSContexte *pCtx) ;
    ~NSHtmlSecu() ;
};

class NSHtmlConvoc : public NSModHtml
{
  public:

    NSHtmlConvoc(OperationType typeOp, NSNoyauDocument *pDocNoy, NSContexte *pCtx, string sDest = "") ;
    ~NSHtmlConvoc() ;
} ;
class NSHtmlAga : public NSModHtml
{
  public:

    NSBlocHtmlArray ligneBlocHtml;

    NSHtmlAga(OperationType typeOp, NSNoyauDocument *pDocNoy, NSContexte *pSuper, string sDest = "") ;
    ~NSHtmlAga() ;

    void  initTotaux() ;
    void  ajouteBloc(NSHtml *ph) ;
    void  ajouteLignes() ;
} ;

class NSHtmlReq : public NSModHtml
{
  public:

    NSBlocHtmlArray ligneBlocHtml;

    NSHtmlReq(OperationType typeOp, NSNoyauDocument *pDocNoy, NSContexte *pSuper, string sDest = "") ;
    ~NSHtmlReq() ;

    void  initTotaux() ;
    void  ajouteBloc(NSHtml *ph) ;
    void  ajouteLignes() ;
} ;

class NSHtmlDep : public NSModHtml
{
  public:

    NSBlocHtmlArray ligneBlocHtml ;

    NSHtmlDep(OperationType typeOp, NSNoyauDocument *pDocNoy, NSContexte *pSuper, string sDest = "") ;
    ~NSHtmlDep() ;

    void  initTotaux() ;    void  ajouteBloc(NSHtml *ph) ;
    void  ajouteLignes() ;
} ;
class NSHtmlImp : public NSModHtml{
  public:

    NSBlocHtmlArray 	ligneBlocHtml ;

    NSHtmlImp(OperationType typeOp, NSNoyauDocument *pDocNoy, NSContexte *pCtx, string sDest = "") ;
    ~NSHtmlImp() ;

    void  ajouteBloc(NSHtml *ph) ;
    void  ajouteLignes() ;
} ;

class NSHtmlLac : public NSModHtml
{
  public:

    NSBlocHtmlArray ligneBlocHtml ;

    NSHtmlLac(OperationType typeOp, NSNoyauDocument *pDocNoy, NSContexte *pCtx, string sDest = "") ;
    ~NSHtmlLac() ;

    void  ajouteBloc(NSHtml *ph) ;
    void  ajouteLignes() ;
} ;

class NSHtmlSac : public NSModHtml
{
  public:

    NSBlocHtmlArray 	ligneBlocHtml ;

    NSHtmlSac(OperationType typeOp, NSNoyauDocument *pDocNoy, NSContexte *pCtx, string sDest = "") ;
    ~NSHtmlSac() ;

    void  initTotaux() ;
    void  ajouteBloc(NSHtml *ph) ;
    void  ajouteLignes() ;
} ;

class NSHtmlSen : public NSModHtml
{
  public:

    NSHtmlSen(OperationType typeOp, NSNoyauDocument *pDocNoy, NSContexte *pCtx, string sDest = "") ;
    ~NSHtmlSen() ;

    void initTotaux() ;
    void addElement(string sLabel, int iAmount, string sCurrency) ;
} ;

class NSHtmlFact : public NSModHtml
{
  public:

    NSHtmlFact(OperationType typeOp, NSNoyauDocument *pDocNoy, NSContexte *pCtx, string sDest = "") ;
    ~NSHtmlFact() ;

    void InsertFactElements(const NSFactData* pData) ;
} ;

class NSHtmlPubli : public NSModHtml
{
  public:

    NSBlocHtmlArray ligneBlocHtml ;

    NSHtmlPubli(OperationType typeOp, NSNoyauDocument *pDocNoy, NSContexte *pCtx, string sDest = "") ;
    ~NSHtmlPubli() ;

    void  ajouteBloc(NSHtml *ph) ;
    void  ajouteLignes() ;
} ;

class NSHtmlRTF : public NSModHtml
{
  public:

    ConvertRTFDialog *_pDialog ;
    int				        _iAlignPrec ;

    NSHtmlRTF(OperationType typeOp, NSNoyauDocument *pDocNoy, string sFileName, NSContexte *pCtx, string sDest = string("")) ;
    ~NSHtmlRTF() ;

    bool      AnalyseXML(string sNomFichierXML) ;
    void	    RemplaceTagRTF(char far *texte, string sTag, string sValeur) ;
    void	    Convertir() ;
    void 	    ConvertRTF(NSHtml *pTexte) ;
    void 	    CreerFils(NSHtml *pere, PoliceParam pp, TextStyle style, string text) ;
    NSHtml    *CreerNewPara(NSHtml *pere, OWL::TParaFormat far pf) ;
    NSHtml    *CreerPara(NSHtml *pere, int indent, int align = 0) ;
    TextStyle TypeStyle(OWL::TCharFormat far cf) ;
    void      TypePolice(OWL::TCharFormat far cf, PoliceParam& pp) ;
} ;

class NSHtmlTXT : public NSModHtml
{
  public:

    NSHtmlTXT(OperationType typeOp, NSNoyauDocument *pDocNoy, const char *fichier, NSContexte *pCtx, string sDest = "") ;
    ~NSHtmlTXT() ;

    string StringTexteHtml(const char* fichier, bool bImport) ;
    void	 RemplaceTag(string& sTexte, string sTag, string sValeur) ;
    void   RemplaceDynamicTags(string& sTexte) ;
} ;

// -----------------------------------------------------------------------------

#endif    // __NS_HTML_H

