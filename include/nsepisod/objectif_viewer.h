#ifndef __OBJECTIFVIEWER_H
#define __OBJECTIFVIEWER_H

#include <owl\groupbox.h>
#include <owl\radiobut.h>
#include <owl\static.h>
#include "nsbb\nsednum.h"
#include "nsbb\nsedit.h"
#include "nsbb\nscomboutil.h"
#include "nautilus\nsldvgoal.h"

#if defined(_EPISODUSDLL)
	#define _EPISODUS __export
#else
	#define _EPISODUS __import
#endif

#define ONE_OBJECTIF            0
#define CYCLE_OBJECTIF          1
#define PERMANENT_OBJECTIF      2

#define LEFT    0
#define RIGHT   1


class ObjectifViewerDlg ;

class NSObjectifEditNum : public NSEditNum
{
	public:

		int                     iValue ;
		NS_CLASSLIB::TColor*    brushColorForward ;
		NS_CLASSLIB::TColor*    brushColorBack ;
		ObjectifViewerDlg*      pParent ;
		int                     posInObjectControlList;

		NSObjectifEditNum(NSContexte *pCtx, ObjectifViewerDlg* pNSUtilDialog, int resId, int listPos,
                 NS_CLASSLIB::TColor* forwardColor,  NS_CLASSLIB::TColor* backColor, int iTextLen = 255,
    			    int iDecimales = 0, string sValidator = "0-9.,");

		~NSObjectifEditNum();

		void    EvKillFocus(HWND hWndLostFocus) ;
		void    SetIntIntoEditNum(int iVal) ;
		void    SetDoubleIntoEditNum(double dVal) ;

	DECLARE_RESPONSE_TABLE(NSObjectifEditNum);
}  ;


class NSObjectifEditDateHeure : public NSUtilEditDateHeure
{
  public:

    NS_CLASSLIB::TColor* brushColorForward ;
    NS_CLASSLIB::TColor* brushColorBack ;
    ObjectifViewerDlg*   pParent ;
    int                  posInObjectControlList ;

    NSObjectifEditDateHeure(NSContexte *pCtx, ObjectifViewerDlg* pNSUtilDialog, int resId,
                 NS_CLASSLIB::TColor* forwardColor,  NS_CLASSLIB::TColor* backColor, UINT iTextLen = 19, bool b2000 = true) ;

    ~NSObjectifEditDateHeure() ;

    void EvKillFocus(HWND hWndLostFocus) ;

  DECLARE_RESPONSE_TABLE(NSObjectifEditDateHeure) ;
}  ;

class _EPISODUS ObjectifViewerDlg : public NSUtilDialog
{
  public:

    NSUtilLexique*                  pObjectifLexicon ;
    OWL::TEdit*                         pComplement ;
     //suivre l'objectif ou l'absence d'objectif
    OWL::TGroupBox*                 pObjGroup ;
    OWL::TRadioButton*              pPresence ;
    OWL::TRadioButton*              pAbsence ;

    NSUtilLexique*                  pUnitValue ;
    NSComboBox*                     pPeriodUnit ;
    TStatic*                        pStaticPeriod ;
    TStatic*                        pStaticMoments ;

    NSUtilEditDateHeure*            pStartDate ;
    NSUtilLexique*                  pStartEveniment ;

    NSUtilEditDateHeure*            pEndDate ;
    NSUtilLexique*                  pEndEveniment ;

    //lines
    TStatic*                        pStaticLines1 ;
    TStatic*                        pStaticLines2 ;
    TStatic*                        pStaticLines3 ;
    TStatic*                        pStaticLines4 ;
    TStatic*                        pStaticLines5 ;
    TStatic*                        pStaticLines6 ;

    //period management
    NSObjectifEditNum*              pPeriodIdealMin ;
    NSObjectifEditNum*              pPeriodIdealMax ;
    NSObjectifEditNum*              pPeriodRecommendMin ;
    NSObjectifEditNum*              pPeriodRecommendMax ;
    NSObjectifEditNum*              pPeriodAuthorizeMin ;
    NSObjectifEditNum*              pPeriodAuthorizeMax ;

    //date management
    NSObjectifEditDateHeure*        pDateIdealMin ;
    NSObjectifEditDateHeure*        pDateIdealMax ;
    NSObjectifEditDateHeure*        pDateRecommendMin ;
    NSObjectifEditDateHeure*        pDateRecommendMax ;
    NSObjectifEditDateHeure*        pDateAuthorizeMin ;
    NSObjectifEditDateHeure*        pDateAuthorizeMax ;

    //value management
    NSObjectifEditNum*              pValueIdealMin ;
    NSObjectifEditNum*              pValueIdealMax ;
    NSObjectifEditNum*              pValueRecommendMin ;
    NSObjectifEditNum*              pValueRecommendMax ;
    NSObjectifEditNum*              pValueAuthorizeMin ;
    NSObjectifEditNum*              pValueAuthorizeMax ;

    //mode
    OWL::TGroupBox*                  pMode ;
    OWL::TRadioButton*               pCycle ;
    OWL::TRadioButton*               pOne ;
    OWL::TRadioButton*               pPermanent ;


    bool    creatMod ;
    NSLdvGoal*      pObjectif;
   // ObjectifViewerDlg( TWindow* pere, NSContexte* pCtx, bool withCreat, NSPatPathoArray* pInitPatho) ;
    ObjectifViewerDlg( TWindow* pere, NSContexte* pCtx, bool withCreat, NSLdvGoal* pRefObj) ;
    ~ObjectifViewerDlg();

    void    CmOk();
    void    CmCancel();
    void    CmPunctual();
    void    CmCyclic();
    void    EvPaint();
    void    CmPermanent();

    void    EvClose();
    void    SetupWindow();

    void    Paint(TDC& pDc, bool erase, NS_CLASSLIB::TRect& RectAPeindre) ;

    void    ColorControlRect(int rect, NSObjectifEditNum *lastActivEdit,  NS_CLASSLIB::TColor* pCurrentColor, int lastPaintedRect) ;
    void    initColorControlRect(  TDC *pDc, NS_CLASSLIB::TPoint ptTopLeft, NS_CLASSLIB::TPoint ptBottomRight ) ;
        //void    RepaintRect(NSObjectifEditNum *lastEdit) ;

       /* void    getEditNumControlTree(NSEditNum* pControl, string codControl,
                            NSPatPathoArray* pPatho, int col, string unit);
        void    getEditDateControlTree(NSUtilEditDateHeure* pControl, string codControl,
                            NSPatPathoArray* pPatho, int col);  */
     //   void    getDataEditNumControl(NSEditNum* pControl, string codControl, string unit);
      //  void    getDataEditDateControlTree(NSUtilEditDateHeure* pControl, string codControl);

       /* void    getTreeCyclicObjectif(NSPatPathoArray* pPatho, int colonne) ;
        void    getTreePunctualObjectif(NSPatPathoArray* pPatho, int colonne) ;
        void    getTreePermanentObjectif(NSPatPathoArray* pPatho, int colonne) ;
        void    getTreeOpenMoment(NSPatPathoArray* pPatho, int colonne);
        void    getTreeCloseMoment(NSPatPathoArray* pPatho, int colonne); */

       // void    getDataCyclicObjectif() ;
      //  void    getDataPunctualObjectif() ;
      //  void    getDataPermanentObjectif() ;
        //save
    bool    getPeriodData() ;
    bool    getValueData();
    void    getDateData() ;
    void    getDataOpenMoment() ;
    void    getDataCloseMoment() ;
    //update
    void    setDate() ;
    void    setPeriode() ;
    void    setValue() ;
    bool    InitFromSavedObjectif() ;

  private :

    int     objectifType ;
    string  sOpenDate ;

   /* void    getPeriodTree(NSPatPathoArray* pPatho, int colonne) ;
    void    getDateTree(NSPatPathoArray* pPatho, int colonne) ;
    void    getValueTree(NSPatPathoArray* pPatho, int colonne) ;   */

    bool    hasControlValue(NSEditNum* pControl) ;
    bool    hasControlValue(NSUtilEditDateHeure* pControl) ;
    bool    hasControlValue(NSUtilLexique* pControl) ;

    void    ShowValue(int cmdShow);
    void    ShowPeriod(int cmdShow) ;
    void    ShowDate(int cmdShow) ;
    void    ShowLine(int cmdShow) ;
    void    decodeObjectif(type) ;

    void    getArrayNumControls(NSObjectifEditNum *pArray[6], int pos);
    void    getArrayDateControls(NSObjectifEditDateHeure *pArray[6]);

      /*  bool    testDraw(NSObjectifEditNum**  arrayControls);
        bool    testDraw(NSObjectifEditDateHeure**  arrayControls) ;  */

    void    repaintColorBar(TDC* pDc, NSObjectifEditNum** arrayControls,
                NS_CLASSLIB::TPoint ptTopLeft, NS_CLASSLIB::TPoint ptBottomRight) ;

    void    repaintColorBar(TDC* pDc, NSObjectifEditDateHeure** arrayControls,
                NS_CLASSLIB::TPoint ptTopLeft, NS_CLASSLIB::TPoint ptBottomRight) ;

    // typeLeft = 0 not visible, 1 numbers, 2 dates
    int                 typeLeft;

    NSObjectifEditNum*       arrayLeftNumControls[6] ;
    NSObjectifEditDateHeure* arrayLeftDateControls[6] ;

    NSObjectifEditNum*       arrayRightNumControls[6] ;

  DECLARE_RESPONSE_TABLE(ObjectifViewerDlg) ;
} ;

class ColorRect
{
  public :

    ColorRect(NS_CLASSLIB::TPoint topLeft, NS_CLASSLIB::TPoint bottomRight);
    ColorRect( NS_CLASSLIB::TPoint topLeft, NS_CLASSLIB::TPoint bottomRight,  NS_CLASSLIB::TColor* color );
    ColorRect() {} ;
    ~ColorRect() {} ;
    void SetRect(NS_CLASSLIB::TPoint ptTopLeft, NS_CLASSLIB::TPoint ptBottomRigh);
    void SetColor( NS_CLASSLIB::TColor* color)        {defaultColor = color;}
    void RectFillWithDefaultColor(TDC *pDc ) ;
    void RectFill(TDC *pDc, const NS_CLASSLIB::TColor& color);

  private :

    NS_CLASSLIB::TPoint ptTopLeft ;
    NS_CLASSLIB::TPoint ptBottomRight ;
    NS_CLASSLIB::TColor* defaultColor ;

    //	pDc->FillRect(ptTopLeft.x, ptTopLeft.y, ptTopLeft.x + 1, ptTopLeft.y + 5, OWL::TBrush pPinceauBlack->GetHandle())) ;
};

#endif   // #ifdef __NSEPIDIV_H

