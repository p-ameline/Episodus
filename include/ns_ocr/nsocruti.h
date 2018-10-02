//----------------------------------------------------------------------------//   Programme prot�g� par une licence Open Source GPL
//   NAUTILUS - www.nautilus-info.com
//----------------------------------------------------------------------------
#ifndef __NSOCRUTI_H
#define __NSOCRUTI_H

#include <owl/dialog.h>
#include <owl/scrollba.h>
#include <owl/groupbox.h>

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsglobalLus.h"
#else
  #include "partage/nsglobal.h"
#endif

#include "partage\ns_vector.h"
#include "nautilus\nssuper.h"
#include "nssavoir\nscaptur.h"
#include "nsldv\nsldvuti.h"

#if defined(_OCRDLL)
	#define _OCROBJ __export
#else
	#define _OCROBJ __import
#endif

class NSOCRbox
{
  public:

    int x0,x1,y0,y1,x,y,dots;   // xmin,xmax,ymin,ymax,reference-pixel,i-dots
    wchar_t c,ac;	            // detected (alternate) char
    char modifier;	            // default=0, see compose() in unicode.c
    int num;		            // same nummer = same char
    int line;		            // line number (points to struct tlines lines)
    int m1,m2,m3,m4;
    int iWidth, iHeigth;

    NS_CLASSLIB::TRect  BoxRect;
    string              sTexte;
    string              sPixels;

    NSOCRbox(int x0, int x1, int y0, int y1, int x, int y, int dots,
             wchar_t c, wchar_t ac, char modifier, int num, int line,
             int m1, int m2, int m3, int m4, int iW, int iH);
    NSOCRbox(string sPix, string sText);
    NSOCRbox(const NSOCRbox& rv);
    ~NSOCRbox();

    NSOCRbox& operator=(const NSOCRbox& src);
};

// d�finition d'un array de mod�lestypedef vector<NSOCRbox*>           OCRBoxArray ;
typedef OCRBoxArray::iterator       boxIter ;
typedef OCRBoxArray::const_iterator boxConstIter ;

class OCRBoxesArray : public OCRBoxArray
{
  public:

    // Constructeurs - Destructeur    OCRBoxesArray() : OCRBoxArray() {}
    OCRBoxesArray(const OCRBoxesArray& rv) ;
    ~OCRBoxesArray() ;

    void vider() ;
    bool charger(NSContexte* pCtx) ;
    bool sauver(NSContexte* pCtx) ;

    OCRBoxesArray& operator=(const OCRBoxesArray& src) ;
};

class NSOCRswap{
  public:

    string sMotif ;    string sRemplacement ;

    NSOCRswap(string sPix, string sText) ;
    NSOCRswap(const NSOCRswap& rv) ;
    ~NSOCRswap() ;

    NSOCRswap& operator=(const NSOCRswap& src) ;
};

// d�finition d'un array de mod�lestypedef vector<NSOCRswap*>         OCRSwArray ;
typedef OCRSwArray::iterator       swapIter ;
typedef OCRSwArray::const_iterator swapConstIter ;

class OCRSwapArray : public OCRSwArray{
  public:

    // Constructeurs - Destructeur
    OCRSwapArray() : OCRSwArray() {}
    OCRSwapArray(const OCRSwapArray& rv);
    ~OCRSwapArray();

    void    vider();
    bool    charger(NSContexte* pCtx);
    bool    sauver(NSContexte* pCtx);
    void    transformerTexte(string* pTexte);

    OCRSwapArray& operator=(const OCRSwapArray& src);
};

class NSOCRLearnDlg;
class NSOCRLearnGroupBox : public TGroupBox{
    public:

	    NSOCRLearnDlg* pMere;

	    NSOCRLearnGroupBox(NSOCRLearnDlg* parent, int resId);
	    ~NSOCRLearnGroupBox();

        void EvPaint();
        void Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectAPeindre);
        void EvLButtonDblClk(UINT modKeys, NS_CLASSLIB::TPoint& point);
        void ButtonDblClk();

    DECLARE_RESPONSE_TABLE(NSOCRLearnGroupBox);
};

class NSOCRVertScroll : public OWL::TScrollBar{
    public:

	    NSOCRLearnDlg* pMere;
        int            iPageStep;

	    NSOCRVertScroll(NSOCRLearnDlg* parent, int resId, TModule* module = 0);
	    ~NSOCRVertScroll() {};

        void FixeRange(int max, int taille);        void SBLineDown();	    void SBLineUp();
        void SBPageDown();
        void SBPageUp();
	    void PositionneCurseur();
};

class NSOCRHorizScroll : public OWL::TScrollBar{
    public:

	    NSOCRLearnDlg* pMere;
        int            iPageStep;

	    NSOCRHorizScroll(NSOCRLearnDlg* parent, int resId, TModule* module = 0);
	    ~NSOCRHorizScroll() {};

        void FixeRange(int max, int taille);
        void SBLineDown();
	    void SBLineUp();
        void SBPageDown();
        void SBPageUp();
	    void PositionneCurseur();

        void EvHScroll(uint scrollCode, uint thumbPos, HWND hWndCtl);
};

//// Objet "Boite de dialogue" utilis� pour l'apprentissage OCR
//
class NSOCRLearnDlg : public TDialog, public NSRoot
{
	public:

		NSOCRLearnGroupBox* pGroup;
        OWL::TEdit* 		pEditOCR;
        OWL::TEdit* 		pEditLib;
        NSOCRHorizScroll*   pHScroll;
        NSOCRVertScroll*    pVScroll;

		OCRBoxesArray       aBoxesArray;
        OCRSwapArray        aSwapArray;

		UINT				iCurrentBox;

        NSOCRbloc*          pOCRBloc;
        NSBitmap*           pScrMap;

        NSOCRbox*           pCurrentBox;

        NS_CLASSLIB::TPoint pointEnCours;

		NSOCRLearnDlg(TWindow*, NSContexte* pCtx, NSOCRbloc* pBloc, NSBitmap* pMap);
		~NSOCRLearnDlg();

		void SetupWindow();

		void CmSelect(WPARAM Cmd);
		void CmCancel();
        void CmOk();
        void EvPaint();
        void Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectAPeindre);
        void changeCurrentBox(NSOCRbox* pNewSelBox);
        void EvLButtonDblClk(UINT modKeys, NS_CLASSLIB::TPoint& point);

    DECLARE_RESPONSE_TABLE(NSOCRLearnDlg);
};

#endif // __NSOCRUTI_H
