// --------------------------------------------------------------------------// NSEPISODVIEW.H		Document/Vues Capture Données Utilisateur
// --------------------------------------------------------------------------
// Fabrice LE PERRU - Aout 2001
// source : Rémi SPAAK - Mai 1997
// --------------------------------------------------------------------------

#if !defined(__NS_SOAPVIEW_H)#define __NS_SOAPVIEW_H

#include <owl\docmanag.h>#include <owl\docview.h>
#include <owl\mdichild.h>
#include <owl\decframe.h>
#include <owl\statusba.h>
#include <owl\controlb.h>
#include <owl\buttonga.h>
#include <owl\applicat.h>
#include <owl\richedpr.h>
#include <owl\choosefo.h>
#include <owl\listwind.h>

#include "nautilus\nsepicap.h"
#include "nsepisod\eptables.h"#include "nsepisod\nsclasser.h"
#include "nsepisod\nssoapdiv.h"

#include "nssavoir\nspatho.h"#include "nssavoir\nspathor.h"

#include "nautilus\nsdocref.h"#include "nautilus\nautilus.rh"
#include "nautilus\richedap.h"
#include "nsbb\nsednum.h"

#define _NSSOAPVIEWCLASS _USERCLASS#define IDC_SOAPVIEW     0x0042

#define NS_EV_TLISTWIND_NOTIFY_BOOL(notifyCode, method)\  {notifyCode, UINT_MAX, (TAnyDispatcher)::B_LPARAM_Dispatch,\
  (TMyPMF)b_LISTWIND_NOTIFY_Sig(&TMyClass::method)}
#define NS_LVN_BEGINDRAG(method) NS_EV_TLISTWIND_NOTIFY_BOOL(LVN_BEGINDRAG, method)

// enum category_type {subjective = 0, objective, assesment, plan} ;
class NSSOAPListWindow;        // classe générique de documents texte nautilus
#define CLASSIF_PRINCIPLE_NBMAXSLOT 8
class  NSSOAPDocument : public TDocument, public NSRoot{
  public:

    NSEpisodus*       pEpisodus ;
    NSDocumentInfo*   pDocInfo ;    SOAPBasketArray*  pBasketList ;    SOAPObjectArray*  pTankObjectList ;

    PrinciplesArray*  pPrincipes ;
    OWL::TImageList*  pMainImgList ;    OWL::TImageList*  pSmImageList ;

    // Drag And Drop    bool              bDragAndDrop ;   // flag of drag-and-drop activation
    OWL::TImageList*  ImgDragDrop ;    // ImageList of the drag-and-drop
    NSSOAPListWindow* pSrcDragDrop ;   // ListWindow source
    NSSOAPListWindow* pDstDragDrop ;   // ListWindow destination
    SOAPObject*       pObject2Drop ;   // object to drop;

    NSSOAPDocument(TDocument *parent, NSContexte *pCtx = (NSContexte*) 0, NSDocumentInfo* pDocInf = (NSDocumentInfo*) 0) ;
    ~NSSOAPDocument();
    bool    enregistrer();    bool    createCSDocument();

    void    addToTank(SOAPObject* pObject);    void    addBasket(SOAPBasket* pBasket);
    void    removeBasket(SOAPBasket* pBasket);
    void    donneChamp(SOAPObject* pObject);
    string  donneCaseLexique(int index);
    int     donneIconIndex(string sCode);
    bool    acceptDragDrop(SOAPObject *pObject2Drop);
    bool    deleteItemDragDrop(SOAPObject *pObject2Drag);
    bool    floculer();
    void    creerChroniques();

    // Méthodes virtuelles de TDocument    bool    Open(int mode, const char far *path = 0)   {return true;}
    bool    Close()                                    {return true;}

    bool    IsOpen()                                   {return true;}
    bool    SetNProblems(int i) { nProblems = i ; return true ;};    bool    SetNCategory(int i) { if (i <= 4) {nCategory = i; return true ;} else return false;};

    int     GetNProblems()  { return nProblems ; };    int     GetNCategory()  { return nCategory ; };

    NSRefDocument* getDocument() { return pDocument ; }
    bool           setDocument(NSRefDocument* pDoc) ;
//        NSCutPaste* pBufDragDrop;
//        NSCutPaste* pBufCopie;

  protected:
    int            nProblems ;    int            nCategory ;

    NSRefDocument* pDocument ;
};
class NSSOAPView;class NSSOAPTankView;

class NSSOAPListWindow : public TListWindow{
  public:

    OWL::TWindow*            pParentWindow;
    NSSOAPDocument*          pDocSOAP;
    SOAPBasket*              pBasket;

    ClassificationPrinciple* pPrincipe;    SOAPObjectArray*         pObjectList;

    int                      iColumn;    int                      iLine;

    OWL::TImageList*         pLgImageList;    OWL::TImageList*         pSmImageList;

    NSSOAPListWindow();    NSSOAPListWindow(NSSOAPView *parent, int id, int x, int y, int w, int h, ClassificationPrinciple* pPrincipe, TModule* module = 0);
    NSSOAPListWindow(NSSOAPTankView* parent, int id, int x, int y, int w, int h, TModule* module = 0);

    ~NSSOAPListWindow() ;
    // réponse aux événements    bool    VnIsWindow(HWND hWnd);
//        void    EvLButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point);
    void    EvSetFocus(HWND hWndLostFocus);
    void    DispInfoSOAP(TLwDispInfoNotify& dispInfo);

    void    SetupWindow();
    bool    LvnBeginDrag(TLwNotify& nmHdr);    void    EvTimer(uint id);
    void    EvMouseMove(uint modKeys, NS_CLASSLIB::TPoint& point);
    void    EvLButtonDown( uint modKeys, NS_CLASSLIB::TPoint& pt);
    void    EvLButtonUp(uint modKeys, NS_CLASSLIB::TPoint& pt);
    void    EvRButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point);
    void    EvLButtonDblClk(uint modkeys, NS_CLASSLIB::TPoint& point);
    void    EvMove(ClassLib::TPoint& clientOrigin);
    void    EvWindowPosChanged(WINDOWPOS far& windowPos);

    void    EvCode();    void    EvPosition();
    void    EvDupliquer();
    void    EvCloner();
    void    EvLibelle();

    void    EvPosit(int iPosit);    void    EvPosit1() { EvPosit(1); }
    void    EvPosit2() { EvPosit(2); }
    void    EvPosit3() { EvPosit(3); }
    void    EvPosit4() { EvPosit(4); }
    void    EvPosit5() { EvPosit(5); }
    void    EvPosit6() { EvPosit(6); }
    void    EvPosit7() { EvPosit(7); }
    void    EvPosit8() { EvPosit(8); }
    void    EvPosit9() { EvPosit(9); }

    void    EvDupli(int iPosit);    void    EvDupli1() { EvDupli(1); }
    void    EvDupli2() { EvDupli(2); }
    void    EvDupli3() { EvDupli(3); }
    void    EvDupli4() { EvDupli(4); }
    void    EvDupli5() { EvDupli(5); }
    void    EvDupli6() { EvDupli(6); }
    void    EvDupli7() { EvDupli(7); }
    void    EvDupli8() { EvDupli(8); }
    void    EvDupli9() { EvDupli(9); }

    int     getFocusedItem();    int     getIndex(int iParam);

    bool    DeleteItemDragDrop(SOAPObject *pObject2Drag);      // efface la donnée de sa liste    void    logMessage(string sLogMessage);

    void    addObject(SOAPObject* pObject);    void    delObject(SOAPObject* pObject, bool bOldID = false);

  DECLARE_RESPONSE_TABLE(NSSOAPListWindow);
};

typedef vector<NSSOAPListWindow*>   ArraySOAPLW ;typedef ArraySOAPLW::iterator       ArraySOAPListWindowIter ;
typedef ArraySOAPLW::const_iterator ArraySOAPListWindowConstIter ;

class SOAPListWinArray : public ArraySOAPLW{
  public:

    SOAPListWinArray() ;    SOAPListWinArray(const SOAPListWinArray &rv) ;
    ~SOAPListWinArray() ;

    NSSOAPListWindow* trouveList(SOAPBasket* pBasket, ClassificationPrinciple* pPrincipe) ;
    void vider() ;
    SOAPListWinArray& operator=(const SOAPListWinArray& src) ;
  protected:
    bool bOwner ; // L'array est-elle propriétaire des objets référencés};

class NSSOAPView : public NSMUEView{
  public:

    // Constructeur - Destructeur    NSSOAPView(NSSOAPDocument& doc, TWindow *parent = 0, string sPreoccu = "");
    ~NSSOAPView();

    void CmOk() {}
    void CmCancel() {}

    // Redéfiniton des fonctions virtuelles de TView    static LPCSTR   StaticName() {return "NSSOAPView";}
    const char      *GetViewName() {return StaticName();}
    TWindow         *GetWindow();
    bool            CanClose();           // de TEditFile
    // bool         CanClear();           // de TEditFile
    bool            SetDocTitle(const char far* docname, int index)
    {
        return OWL::TWindow::SetDocTitle(docname, index);
    }
    void            setViewName() ;

    void            EvDestroy();    void            SetWindowPosit();
    void            RecordWindowPosit();

    void            SetupColumns();    void            DispInfoSOAP(TLwDispInfoNotify& dispInfo);
    void            EvSetFocus(HWND hWndLostFocus);
    void            EvMouseMove(uint modKeys, NS_CLASSLIB::TPoint& point);
                    // Fonctions de réponse aux evts
    void            EvSize(uint sizeType, ClassLib::TSize &size);
    void            EvMove(ClassLib::TPoint& clientOrigin);
    void            EvWindowPosChanged(WINDOWPOS far& windowPos);
    void            EvPaint();
    void            gotMoved();

    void            addColumn(SOAPBasket* pBasket = 0);    void            delColumn(SOAPBasket* pBasket);
    bool            noItem(int iCol);

    void            transmit();
    // void            addButton();
    void            logMessage(string sLogMessage);
    NSSOAPDocument*     getDoc()    { return pDocSOAP; }    SOAPListWinArray*   getLists()  { return pArraySOAPListWindows; }

  protected:
    // Variables
    bool                bFirstSetup;
    NSSOAPDocument*     pDocSOAP;	            // Pointeur sur document soap    SOAPListWinArray*   pArraySOAPListWindows;  // ListWindow
    int                 nSOAPWindowList;

    bool	            bSetupToolBar;
    // Remplissage de la vue    void FillViewData() ;
    void SetupWindow() ;
    void SetupToolBar() ;
    void SetupTxtBar() ;
    void InitFormatFont() ;

    // view event handlers    bool VnIsWindow(HWND hWnd) ;

    void EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point) ;    void EvChar(uint key, uint repeatCount, uint flags) ;

  DECLARE_RESPONSE_TABLE(NSSOAPView) ;};

class NSSOAPTankView : public NSMUEView{
  public:

    // Constructeur - Destructeur
    NSSOAPTankView(NSSOAPDocument& doc, TWindow *parent = 0, string sPreoccu = "");
    ~NSSOAPTankView();

    void addObject(SOAPObject* pObjectSOAP);    void floculer();
    void reposit();

    void CmOk() {}
    void CmCancel() {}

    // Redéfiniton des fonctions virtuelles de TView    static LPCSTR   StaticName() {return "NSSOAPTankView";}
    const char      *GetViewName() {return StaticName();}
    TWindow         *GetWindow();
    bool            CanClose();           // de TEditFile
    // bool         CanClear();           // de TEditFile
    bool            SetDocTitle(const char far* docname, int index)
    {
        return OWL::TWindow::SetDocTitle(docname, index);
    }
    void            setViewName() ;

    void            EvDestroy();    void            SetWindowPosit();
    void            RecordWindowPosit();

    // view event handlers    bool 	        VnIsWindow(HWND hWnd);

    void            DispInfoTank(TLwDispInfoNotify& dispInfo);    void            EvSetFocus(HWND hWndLostFocus);
    void            EvMouseMove(uint modKeys, NS_CLASSLIB::TPoint& point);

    NSSOAPListWindow  *pMyListWindow; // ListWindow
    NSSOAPDocument* getDoc() { return pDocSOAP; }
  protected:
    // Remplissage de la vue    void 	FillViewData();
    void 	SetupWindow();
    void    SetupToolBar();

    // Fonctions de réponse aux evts    void    EvSize(uint sizeType, ClassLib::TSize &size);

    // variables    NSSOAPDocument *pDocSOAP ;

    bool	       bSetupToolBar ;
  DECLARE_RESPONSE_TABLE(NSSOAPTankView) ;};

class NSSOAPLibelleDlg : public TDialog, public NSRoot{
  public:

    // Constructeur - Destructeur    NSSOAPLibelleDlg(TWindow *parent, NSContexte *pCtx, string *psLib) ;
    ~NSSOAPLibelleDlg() ;

    void    CmOk() ;    void    CmCancel() ;

    void    SetupWindow() ;
  protected:
    OWL::TEdit* pLibelEdit ;    string*     psLibelle ;

  DECLARE_RESPONSE_TABLE(NSSOAPLibelleDlg) ;};

class NSSOAPDlg : public TDialog, public NSRoot{
  public:

    // Constructeur - Destructeur    NSSOAPDlg(TWindow *parent, NSContexte *pCtx);
    NSSOAPDlg(TWindow *parent, NSContexte *pCtx, string *psdlg);
    ~NSSOAPDlg() ;

    void    CmOk();    void    CmCancel();
    void    CmConvert() ;

//        bool    newFicheClasser(NSEpiClasserData *pData) ;    bool    convert(string line) ;
    bool    newFicheClasserC2(string sCode, string sRelation) ;

  protected:
    OWL::TEdit *pSOAPEdit1 ;    OWL::TEdit *pSOAPEdit2 ;
    OWL::TEdit *pSOAPEdit3 ;
    string     *psSOAPEdit ;

  DECLARE_RESPONSE_TABLE(NSSOAPDlg) ;} ;

#endif		// __NS_SOAPVIEW_H
