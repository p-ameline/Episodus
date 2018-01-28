//----------------------------------------------------------------------------//                  Ligne de vie - Vue du modèle Document/Vue
//----------------------------------------------------------------------------
#include <owl\pch.h>
#include <classlib\date.h>

#include "_gocr.h"
#include "gocr.h"

#include "pnm.h"
#include "pgm2asc.h"

#include "partage\nsdivfct.h"
#include "ns_ocr\nsocruti.h"
#include "ns_ocr\ns_ocr.h"
#include "ns_ocr\ns_ocr.rh"

// --------------------------------------------------------------------------
// -------------------------- METHODES DE NSOCRbox -------------------------
// --------------------------------------------------------------------------
NSOCRbox::NSOCRbox(int ix0, int ix1, int iy0, int iy1, int ix, int iy, int idots,
                   wchar_t cc, wchar_t cac, char cmodifier, int inum, int iline,
                   int im1, int im2, int im3, int im4, int iW, int iH)
{
    x0          = ix0;
    x1          = ix1;
    y0          = iy0;
    y1          = iy1;
    x           = ix;
    y           = iy;
    dots        = idots;
    c           = cc;
    ac          = cac;
    modifier    = cmodifier;
    num         = inum;
    line        = iline;
    m1          = im1;
    m2          = im2;
    m3          = im3;
    m4          = im4;
    iWidth      = iW;
    iHeigth     = iH;

    BoxRect     = NS_CLASSLIB::TRect(x0, y1 - iHeigth + 1, x1, y1);
    sTexte      = "";
    sPixels     = "";
}

NSOCRbox::NSOCRbox(string sPix, string sText)
{
    x0          = 0;
    x1          = 0;
    y0          = 0;
    y1          = 0;
    x           = 0;
    y           = 0;
    dots        = 0;
    c           = '\0';
    ac          = '\0';
    modifier    = '\0';
    num         = 0;
    line        = 0;
    m1          = 0;
    m2          = 0;
    m3          = 0;
    m4          = 0;
    iWidth      = 0;
    iHeigth     = 0;

    BoxRect     = NS_CLASSLIB::TRect(x0, y0, x1, y1);
    sTexte      = sText;
    sPixels     = sPix;
}
NSOCRbox::NSOCRbox(const NSOCRbox& rv){
  x0          = rv.x0;
  x1          = rv.x1;
  y0          = rv.y0;
  y1          = rv.y1;
  x           = rv.x;
  y           = rv.y;
  dots        = rv.dots;
  c           = rv.c;
  ac          = rv.ac;
  modifier    = rv.modifier;
  num         = rv.num;
  line        = rv.line;
  m1          = rv.m1;
  m2          = rv.m2;
  m3          = rv.m3;
  m4          = rv.m4;
  iWidth      = rv.iWidth;
  iHeigth     = rv.iHeigth;

  BoxRect = rv.BoxRect;
  sTexte  = rv.sTexte;
  sPixels = rv.sPixels;
}

NSOCRbox::~NSOCRbox(){
}

NSOCRbox&NSOCRbox::operator=(const NSOCRbox& src)
{
  if (this == &src)
  	return *this ;

  x0          = src.x0;
  x1          = src.x1;
  y0          = src.y0;
  y1          = src.y1;
  x           = src.x;
  y           = src.y;
  dots        = src.dots;
  c           = src.c;
  ac          = src.ac;
  modifier    = src.modifier;
  num         = src.num;
  line        = src.line;
  m1          = src.m1;
  m2          = src.m2;
  m3          = src.m3;
  m4          = src.m4;
  iWidth      = src.iWidth;
  iHeigth     = src.iHeigth;

  BoxRect     = src.BoxRect;
  sTexte      = src.sTexte;  sPixels     = src.sPixels;

  return *this;
}

// --------------------------------------------------------------------------// ------------------------ METHODES DE OCRBoxesArray -----------------------
// --------------------------------------------------------------------------

OCRBoxesArray::OCRBoxesArray(const OCRBoxesArray& rv)              :OCRBoxArray()
{
try
{
	if (false == rv.empty())
		for (boxConstIter i = rv.begin() ; rv.end() != i ; i++)
			push_back(new NSOCRbox(*(*i))) ;
}
catch (...)
{
	erreur("Exception OCRBoxesArray ctor.", standardError, 0) ;
}
}

voidOCRBoxesArray::vider()
{
  if (empty())
    return ;

  for (boxIter i = begin() ; end() != i ; )
  {
    delete *i ;
    erase(i) ;
  }
}

OCRBoxesArray::~OCRBoxesArray(){
	vider() ;
}

OCRBoxesArray&OCRBoxesArray::operator=(const OCRBoxesArray& src)
{
try
{
  if (this == &src)
    return *this ;

  vider() ;

  if (false == src.empty())
    for (boxConstIter i = src.begin() ; src.end() != i ; i++)
      push_back(new NSOCRbox(*(*i))) ;

  return *this ;
}
catch (...)
{
  erreur("Exception OCRBoxesArray::operator=.", standardError, 0) ;
  return *this ;
}
}

boolOCRBoxesArray::charger(NSContexte* pContexte)
{
try
{
  if (NULL == pContexte)
    return false ;

  ifstream inFile ;

  string sLang = "" ;

  string sFichierTempo = pContexte->PathName("FPER") + string("epi_ocr.dat") ;

  inFile.open(sFichierTempo.c_str()) ;
  if (!inFile)
  {
    // erreur("Erreur d'ouverture du fichier des formats de capture Episodus (captfrmt.dat).", 0, 0, pContexte->GetMainWindow()->GetHandle());
    return false ;
  }

  size_t  fin ;
  size_t  debut ;

  string  sTitre ;
  string  sLine ;

  int     inumLigne = 0 ;

  while (!inFile.eof())
  {
    getline(inFile, sLine) ;
    inumLigne++ ;

    bool bContinuer = true ;

    if (sLine == "")
      bContinuer = false ;

    if (bContinuer)
    {
      strip(sLine, stripBoth) ;
      if (sLine == "")
        bContinuer = false ;
    }

    if (bContinuer)
    {
      sTitre = string("") ;

      debut = sLine.find("[") ;
      if ((debut == NPOS) || (debut == strlen(sLine.c_str()) - 1))
      {
        erreur("Le fichier des modèles d'OCR Episodus (epi_ocr.dat) est défectueux.", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
        bContinuer = false ;
      }
    }

    if (bContinuer)
    {
      fin = sLine.find("]", debut) ;

      if ((fin == NPOS) || (fin == strlen(sLine.c_str()) - 1))
      {
        erreur("Le fichier des modèles d'OCR Episodus (epi_ocr.dat) est défectueux.", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
        bContinuer = false ;
      }
    }

    if (bContinuer)
    {
      sTitre  = string(sLine, debut+1, fin-debut-1) ;
      sLine   = string(sLine, fin+1, strlen(sLine.c_str())-fin-1) ;
    }

    if (bContinuer)
    {
      string sLineOld = "" ;
      while (sLineOld != sLine)
      {
        sLineOld = sLine ;
        strip(sLine, stripBoth) ;
        strip(sLine, stripBoth, '/t') ;
      }
      if (sLine == "")
        bContinuer = false ;
    }

    if (bContinuer)
      push_back(new NSOCRbox(sTitre, sLine)) ;
  }

  inFile.close() ;
  return true ;
}
catch (...)
{
  erreur("Exception : il est impossible de charger les modèles d'OCR Episodus.", standardError, 0) ;
  return false ;
}
}

boolOCRBoxesArray::sauver(NSContexte* pContexte)
{
try
{
  if (empty())
    return true ;

  ofstream outFile ;

  string sLang = "";
  if ((pContexte) && (pContexte->getUtilisateur()))
    sLang = pContexte->getUtilisateur()->donneLang() ;

  string sFichier = pContexte->PathName("FPER") + string("epi_ocr.dat") ;

  outFile.open(sFichier.c_str(), ios::out) ;
  if (!outFile)
  {
    string sErrMess = pContexte->getSuperviseur()->getText("fileErrors", "errorOpeningOutputFile") ;
    sErrMess += string(" ") + sFichier ;
    erreur(sErrMess.c_str(), standardError, 0) ;
    return false ;
  }

  for (boxIter i = begin(); i != end(); i++)
    if (((*i)->sTexte != "") && ((*i)->sPixels != ""))
      outFile << string("[") + (*i)->sPixels + string("] ") + (*i)->sTexte + string("\n") ;

  outFile.close() ;
  return true ;
}
catch (...)
{
  erreur("Exception : il est impossible d'enregistrer les modèles d'OCR Episodus.", standardError, 0) ;
  return false ;
}
}

// --------------------------------------------------------------------------

// ------------------------ METHODES DE OCRSwapArray -----------------------
// --------------------------------------------------------------------------

NSOCRswap::NSOCRswap(string sPix, string sText)
{
    sMotif          = sPix;
    sRemplacement   = sText;
}
NSOCRswap::NSOCRswap(const NSOCRswap& rv){
  sMotif          = rv.sMotif;
  sRemplacement   = rv.sRemplacement;
}

NSOCRswap::~NSOCRswap(){
}

NSOCRswap&NSOCRswap::operator=(const NSOCRswap& src)
{
  if (this == &src)
  	return *this ;

  sMotif          = src.sMotif;  sRemplacement   = src.sRemplacement;

  return *this;}


void
OCRSwapArray::transformerTexte(string* pTexte)
{
    if (empty())
        return;


    for (swapIter i = begin(); i != end(); i++)
    {
        size_t iPos = pTexte->find((*i)->sMotif);
        int iTailleMotif = strlen((*i)->sMotif.c_str());

        while (iPos != NPOS)
        {
            string sAvant = "";
            string sApres = "";

            if (iPos > 0)
                sAvant = string(*pTexte, 0, iPos);
            if (strlen(pTexte->c_str()) > iPos + iTailleMotif)
                sApres = string(*pTexte, iPos + iTailleMotif, strlen(pTexte->c_str()) - iPos - iTailleMotif);

            *pTexte = sAvant + (*i)->sRemplacement + sApres;

            iPos = pTexte->find((*i)->sMotif);
        }
    }
}
OCRSwapArray::OCRSwapArray(const OCRSwapArray& rv)             :OCRSwArray()
{
try
{
	if (false == rv.empty())
		for (swapConstIter i = rv.begin() ; rv.end() != i ; i++)
			push_back(new NSOCRswap(*(*i)));
}
catch (...)
{
	erreur("Exception OCRSwapArray copy ctor.", standardError, 0) ;
}
}

voidOCRSwapArray::vider()
{
    if (empty())
        return;

    for (swapIter i = begin(); i != end(); )
    {
   	    delete *i;
        erase(i);
    }
}

OCRSwapArray::~OCRSwapArray()
{
	vider();
}

OCRSwapArray&OCRSwapArray::operator=(const OCRSwapArray& src)
{
try
{
	if (this == &src)
		return *this ;

	vider() ;

	if (false == src.empty())		for (swapConstIter i = src.begin() ; src.end() != i ; i++)
			push_back(new NSOCRswap(*(*i))) ;

	return *this ;}
catch (...)
{
	erreur("Exception OCRSwapArray operator=.", standardError, 0) ;
	return *this ;
}
}

boolOCRSwapArray::charger(NSContexte* pContexte)
{
try
{
  ifstream inFile ;


    string sLang = "";

    string sFichierTempo = pContexte->PathName("FPER") + string("epi_ocr.swp");

    inFile.open(sFichierTempo.c_str());
    if (!inFile)
    {
   	    // erreur("Erreur d'ouverture du fichier des formats de capture Episodus (captfrmt.dat).", 0, 0, pContexte->GetMainWindow()->GetHandle());
        return false;
    }

    size_t  fin;
    size_t  debut;

    string  sTitre;
    string  sLine;

    int     inumLigne = 0;

    while (!inFile.eof())
    {
        getline(inFile, sLine);
        inumLigne++;

        bool bContinuer = true;

        if (sLine == "")
            bContinuer = false;

        if (bContinuer)
        {
            strip(sLine, stripBoth);
            if (sLine == "")
                bContinuer = false;
        }

        if (bContinuer)
        {
            sTitre  = "";

            debut = sLine.find("[");
            if ((debut == NPOS) || (debut == strlen(sLine.c_str()) - 1))
            {
                erreur("Le fichier des échanges d'OCR Episodus (epi_ocr.swp) est défectueux.", standardError, 0, pContexte->GetMainWindow()->GetHandle());
                bContinuer = false;
            }
        }

        if (bContinuer)
        {
            fin = sLine.find("]", debut);

            if ((fin == NPOS) || (fin == strlen(sLine.c_str()) - 1))
            {
                erreur("Le fichier des échanges d'OCR Episodus (epi_ocr.swp) est défectueux.", standardError, 0, pContexte->GetMainWindow()->GetHandle());
                bContinuer = false;
            }
        }

        if (bContinuer)
        {
            sTitre  = string(sLine, debut+1, fin-debut-1);
            sLine   = string(sLine, fin+1, strlen(sLine.c_str())-fin-1);
        }

        if (bContinuer)
        {
            string sLineOld = "";
            while (sLineOld != sLine)
            {
                sLineOld = sLine;
                strip(sLine, stripBoth);
                strip(sLine, stripBoth, '/t');
            }
            if (sLine == "")
                bContinuer = false;
        }

        if (bContinuer)
            push_back(new NSOCRswap(sTitre, sLine));
    }

    inFile.close();
    return true;
}catch (...)
{
  erreur("Exception : il est impossible de charger les modèles d'échanges OCR Episodus.", standardError, 0) ;
  return false ;
}
}

boolOCRSwapArray::sauver(NSContexte* pContexte)
{
try
{
    if (empty())
        return true;

    ofstream    outFile;

    string sLang = "";
    if ((pContexte) && (pContexte->getUtilisateur()))
        sLang = pContexte->getUtilisateur()->donneLang();

    string sFichier = pContexte->PathName("FPER") + string("epi_ocr.swp");

    outFile.open(sFichier.c_str(), ios::out);
    if (!outFile)
    {
        string sErrMess = string("Erreur d'ouverture en écriture du fichier ")
                                                                    + sFichier;
        erreur(sErrMess.c_str(), standardError, 0) ;
   		return false;
   	}

    for (swapIter i = begin(); i != end(); i++)
    {
        if (((*i)->sMotif != "") && ((*i)->sRemplacement != ""))
            outFile << string("[") + (*i)->sMotif + string("] ") + (*i)->sRemplacement + string("\n");
    }
    outFile.close();
    return true;
}
catch (...)
{
  erreur("Exception : il est impossible d'enregistrer les modèles d'échanges OCR Episodus.", standardError, 0) ;
  return false ;
}
}

// -----------------------------------------------------------------
//
//  Méthodes de ChoixCaptureContexte
//
// -----------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NSOCRLearnDlg, TDialog)
    EV_COMMAND(IDOK, CmOk),
    EV_WM_PAINT,
    EV_WM_LBUTTONDBLCLK,
	// EV_CHILD_NOTIFY_AND_CODE(IDC_CAPTCTXBOX, LBN_SELCHANGE, CmSelectProto),
	// EV_CHILD_NOTIFY_AND_CODE(IDC_CAPTCTXBOX, LBN_DBLCLK, 	CmProtoDblClk),
    // EV_CHILD_NOTIFY(IDC_LEARN, BN_CLICKED, CmLearn),
END_RESPONSE_TABLE;

NSOCRLearnDlg::NSOCRLearnDlg(TWindow* pere, NSContexte* pCtx,
                             NSOCRbloc* pBloc, NSBitmap* pMap)
              :TDialog(pere, "IDD_OCR_LEARN", pNSResModule), NSRoot(pCtx)
{
	pGroup          = new NSOCRLearnGroupBox(this, IDC_GROUP);
    pEditLib        = new TEdit(this, IDC_CHARS);
    pEditOCR        = new TEdit(this, IDC_RECONNU);
    pHScroll        = new NSOCRHorizScroll(this, IDC_OCRHSCROLL, pNSResModule);
    pVScroll        = new NSOCRVertScroll(this, IDC_OCRVSCROLL, pNSResModule);

    pOCRBloc        = pBloc;
    pScrMap         = pMap;

    aBoxesArray.charger(pCtx);
    aSwapArray.charger(pCtx);

    iCurrentBox     = -1;
    pCurrentBox     = 0;

    pointEnCours    = NS_CLASSLIB::TPoint(0, 0);


    bool bChargerBox = true;
    int  iNumBox = 0;
    while (bChargerBox)
    {
        int     ix0, ix1, iy0, iy1, ix, iy, idots, iline, inum;
        wchar_t cc, cac;
        char    cmodif;
        int     im1, im2, im3, im4;
        int     iBoxWidth;
        int     iBoxHeigth;

        int iResu = giveBoxData(iNumBox, &inum, &iline, &ix0, &ix1, &iy0, &iy1,
                                  &ix, &iy, &idots, &cc, &cac, &cmodif,
                                  &im1, &im2, &im3, &im4, &iBoxWidth, &iBoxHeigth);

        if (iResu != 0)
        {
            char* pPixels = new char[(iBoxHeigth * (iBoxWidth + 1)) + 1];

            int iResuPix = giveBoxPixels(iNumBox, pPixels);
            string sOCRPix = string(pPixels);

            if (iResuPix != 0)
            {
                NSOCRbox* pBox = new NSOCRbox(ix0, ix1, iy0, iy1, ix, iy, idots,
                                                cc, cac, cmodif, inum, iline,
                                                im1, im2, im3, im4, iBoxWidth,
                                                iBoxHeigth);
                pBox->sPixels = sOCRPix;

                aBoxesArray.push_back(new NSOCRbox(*pBox));
                delete pBox;

                iNumBox++;
            }
            delete[] pPixels;
        }
        else
            bChargerBox = false;
    }
}

//---------------------------------------------------------------------------
//  Destructeur.
//---------------------------------------------------------------------------
NSOCRLearnDlg::~NSOCRLearnDlg()
{
	delete pGroup;
    delete pEditLib;
    delete pEditOCR;
}

//---------------------------------------------------------------------------
//  Initialisation
//---------------------------------------------------------------------------
void
NSOCRLearnDlg::SetupWindow()
{
	TDialog::SetupWindow();
    NS_CLASSLIB::TRect grpRect = pGroup->GetWindowRect();

    pHScroll->FixeRange(pOCRBloc->BlocOCR.Width(), pOCRBloc->BlocOCR.Width() / grpRect.Width());
    pVScroll->FixeRange(pOCRBloc->BlocOCR.Height(), pOCRBloc->BlocOCR.Height() / grpRect.Height());
}
voidNSOCRLearnDlg::EvPaint()
{
  DefaultProcessing() ;
  return ;

/*
  TPaintDC Dc(*this);  NS_CLASSLIB::TRect& rect = *(NS_CLASSLIB::TRect*)&Dc.Ps.rcPaint;
  Paint(Dc, true, rect);
  //TWindow::EvPaint();
  DefaultProcessing();
  // bPaint = true;
*/
}

voidNSOCRLearnDlg::Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectAPeindre)
{
  TDialog::Paint(dc, erase, RectAPeindre) ;

  if (NULL == pScrMap)
    return ;

  NS_CLASSLIB::TRect GrpRect = pGroup->GetClientRect() ;
  NS_CLASSLIB::TPoint pointHautGauche = pOCRBloc->BlocOCR.TopLeft() ;

  pScrMap->Affiche(&dc, &RectAPeindre, &pointHautGauche, 2) ;
}

void NSOCRLearnDlg::CmCancel()
{
  TDialog::CmCancel() ;
}


void
NSOCRLearnDlg::CmOk()
{
    changeCurrentBox(NULL);

    for (boxIter boxIt = aBoxesArray.begin();
                                    boxIt != aBoxesArray.end();)
    {
        if ((*boxIt)->sTexte == "")
        {
            delete *boxIt;
            aBoxesArray.erase(boxIt);
        }
        else
            boxIt++;
    }

    aBoxesArray.sauver(pContexte);

	TDialog::CmOk();
}

void
NSOCRLearnDlg::changeCurrentBox(NSOCRbox* pNewSelBox)
{
    if (pCurrentBox)
    {
        char szNewTexte[51];
        pEditLib->GetText(szNewTexte, 50);
        pCurrentBox->sTexte = szNewTexte;
    }
    pCurrentBox = pNewSelBox;
    if (pCurrentBox)
    {
        if (pCurrentBox->c == UNKNOWN)
            pEditOCR->SetText("Inconnu");
        else
        {
            char szOcr[2];
            szOcr[0] = pCurrentBox->c;
            szOcr[1] = '\0';
            pEditOCR->SetText(szOcr);
        }

        if (pCurrentBox->sTexte == "")
            pCurrentBox->sTexte = string(1, pCurrentBox->c);
        pEditLib->SetText(pCurrentBox->sTexte.c_str());
    }
}
voidNSOCRLearnDlg::EvLButtonDblClk(UINT /* modKeys */, NS_CLASSLIB::TPoint& point)
{
  //
  // Manoeuvre très sale - Dirty way
  //
  NS_CLASSLIB::TPoint bitmapPoint = NS_CLASSLIB::TPoint(point.X() - 9,
                                                            point.Y() - 11) ;
/*  NS_CLASSLIB::TRect dlgRect;  NS_CLASSLIB::TRect grpRect;
  GetWindowRect(dlgRect);
  pGroup->GetWindowRect(grpRect);

  NS_CLASSLIB::TPoint bitmapPoint
            = NS_CLASSLIB::TPoint(point.X() + dlgRect.Left() - grpRect.Left(),
                                    point.Y() + dlgRect.Top() - grpRect.Top());
*/

  if (false == aBoxesArray.empty())
  {
    for (boxIter boxIt = aBoxesArray.begin();
                                    boxIt != aBoxesArray.end(); boxIt++)
    {

      NS_CLASSLIB::TRect compareRect =
                            NS_CLASSLIB::TRect((*boxIt)->BoxRect.Left() * 2,
                                                (*boxIt)->BoxRect.Top() * 2,
                                                (*boxIt)->BoxRect.Right() * 2,
                                                (*boxIt)->BoxRect.Bottom() * 2);

      // Attention : Contains ne marche pas lorsque le rectangle est
      // réduit à un pixel
      // if ((*boxIt)->BoxRect.Contains(bitmapPoint))

      if ((bitmapPoint.X() >= compareRect.Left()) &&
          (bitmapPoint.X() <= compareRect.Right()) &&
          (bitmapPoint.Y() >= compareRect.Top()) &&
          (bitmapPoint.Y() <= compareRect.Bottom()))
      {
        changeCurrentBox(*boxIt) ;
        return ;
      }
    }
  }
}

DEFINE_RESPONSE_TABLE1(NSOCRLearnGroupBox, TGroupBox)
    EV_WM_PAINT,
    EV_WM_LBUTTONDBLCLK,
    EV_NOTIFY_AT_CHILD(WM_LBUTTONDBLCLK, ButtonDblClk),
	//EV_NOTIFY_AT_CHILD(WM_KILLFOCUS, EvKillFocus),
	//EV_WM_KILLFOCUS,
END_RESPONSE_TABLE;

//
// Constructeur
//
NSOCRLearnGroupBox::NSOCRLearnGroupBox(NSOCRLearnDlg* parent, int resId)
                   :TGroupBox(parent, resId)
{
    pMere = parent;
    DisableTransfer();
}

//
// Destructeur
//
NSOCRLearnGroupBox::~NSOCRLearnGroupBox()
{
}

void
NSOCRLearnGroupBox::EvPaint()
{
    TPaintDC Dc(*this);
    NS_CLASSLIB::TRect& rect = *(NS_CLASSLIB::TRect*)&Dc.Ps.rcPaint;

    Paint(Dc, true, rect);

    //TWindow::EvPaint();
    DefaultProcessing();
    // bPaint = true;
}

void
NSOCRLearnGroupBox::Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectAPeindre)
{
    TGroupBox::Paint(dc, erase, RectAPeindre);

    if (!(pMere->pScrMap))
        return;

    int iZoomFactor = 2;
    //
    // Affichage du domaine sélectionné   pointEnCours
    //
    NS_CLASSLIB::TPoint pointHautGauche = pMere->pOCRBloc->BlocOCR.TopLeft();
    pointHautGauche.Offset(pMere->pointEnCours.X(), pMere->pointEnCours.Y());

    pMere->pScrMap->Affiche(&dc, &RectAPeindre, &pointHautGauche, iZoomFactor);

    //
    // Affichage des box
    //
    if (!(pMere->aBoxesArray.empty()))
    {
        for (boxIter boxIt = pMere->aBoxesArray.begin();
                                    boxIt != pMere->aBoxesArray.end(); boxIt++)
        {
            NS_CLASSLIB::TRect affiRect =
                        NS_CLASSLIB::TRect((*boxIt)->BoxRect.Left() - pMere->pointEnCours.X() - 1,
                                           (*boxIt)->BoxRect.Top() - pMere->pointEnCours.Y() - 1,
                                           (*boxIt)->BoxRect.Right() - pMere->pointEnCours.X() + 1,
                                           (*boxIt)->BoxRect.Bottom() - pMere->pointEnCours.Y() + 1);

            if (RectAPeindre.Touches(affiRect))
            {

                NS_CLASSLIB::TRect SelRect =
                        NS_CLASSLIB::TRect(iZoomFactor * affiRect.Left(),
                                           iZoomFactor * affiRect.Top(),
                                           iZoomFactor * affiRect.Right(),
                                           iZoomFactor * affiRect.Bottom());
                dc.DrawFocusRect(SelRect);
            }
        }
    }
}
voidNSOCRLearnGroupBox::EvLButtonDblClk(UINT /* modKeys */, NS_CLASSLIB::TPoint& point)
{
  if (false == pMere->aBoxesArray.empty())
  {
    for (boxIter boxIt = pMere->aBoxesArray.begin();
                                    boxIt != pMere->aBoxesArray.end(); boxIt++)
    {
      NS_CLASSLIB::TRect compareRect =
                            NS_CLASSLIB::TRect((*boxIt)->BoxRect.Left() * 2,
                                                (*boxIt)->BoxRect.Top() * 2,
                                                (*boxIt)->BoxRect.Right() * 2,
                                                (*boxIt)->BoxRect.Bottom() * 2);

      if (compareRect.Contains(point))
      {
        pMere->changeCurrentBox(*boxIt) ;
        return ;
      }
    }
  }
}


void
NSOCRLearnGroupBox::ButtonDblClk()
{

    if (!(pMere->aBoxesArray.empty()))
    {
        for (boxIter boxIt = pMere->aBoxesArray.begin();
                                    boxIt != pMere->aBoxesArray.end(); boxIt++)
        {
                pMere->changeCurrentBox(*boxIt);
                return;
        }
    }
}

//-------------------------------------------------------------------------
//
//                                 NSOCRVertScroll
//
//-------------------------------------------------------------------------

NSOCRVertScroll::NSOCRVertScroll(NSOCRLearnDlg* parent, int resourceId, TModule* module)
                :TScrollBar(parent, resourceId, module)
{
    pMere = parent;
}

void
NSOCRVertScroll::FixeRange(int max, int taille)
{
   iPageStep = taille;

   SCROLLINFO scInfo;
   scInfo.cbSize = (UINT) sizeof(scInfo);
   scInfo.fMask = SIF_ALL;
   scInfo.nMin = 1;
   scInfo.nMax = max;
   scInfo.nPage = (UINT) (taille);
   scInfo.nPos = 1;

   SetScrollInfo(&scInfo, TRUE);
}

//
// Flêche basse
//
void
NSOCRVertScroll::SBLineUp()
{
	// Transmet à la boite de Dialogue
	TScrollBar::SBLineUp();
	pMere->pointEnCours.Offset(0, -1);
    pMere->pGroup->Invalidate();
}

//
// Flêche haute
//
void
NSOCRVertScroll::SBLineDown()
{
	// Transmet à la boite de Dialogue
	TScrollBar::SBLineDown();
	pMere->pointEnCours.Offset(0, 1);
    pMere->pGroup->Invalidate();
}

void
NSOCRVertScroll::SBPageDown()
{
    TScrollBar::SBPageDown();
    pMere->pointEnCours.Offset(0, iPageStep);
    pMere->pGroup->Invalidate();
}

void
NSOCRVertScroll::SBPageUp()
{
    TScrollBar::SBPageUp();
    pMere->pointEnCours.Offset(0, -iPageStep);
    pMere->pGroup->Invalidate();
}

void
NSOCRVertScroll::PositionneCurseur()
{
    /*

	//
	// Prise du nombre total de d'enregistrement et de la position actuelle
	//
	int NumEnCours;

	//
	// Positionnement du curseur
	//
    pListeDlg->pPatEnCours->donneNumRecEnCours(&NumEnCours);
	SetPosition(NumEnCours); */
}

//-------------------------------------------------------------------------
//
//                                 NSOCRHorizScroll
//
//-------------------------------------------------------------------------

NSOCRHorizScroll::NSOCRHorizScroll(NSOCRLearnDlg* parent, int resourceId, TModule* module)
                :TScrollBar(parent, resourceId, module)
{
    pMere = parent;
}

void
NSOCRHorizScroll::FixeRange(int max, int taille)
{
   iPageStep = taille;

   SCROLLINFO scInfo;
   scInfo.cbSize = (UINT) sizeof(scInfo);
   scInfo.fMask = SIF_ALL;
   scInfo.nMin = 1;
   scInfo.nMax = max;
   scInfo.nPage = (UINT) (taille);
   scInfo.nPos = 1;

   SetScrollInfo(&scInfo, TRUE);
}

//
// Flêche gauche
//
void
NSOCRHorizScroll::SBLineUp()
{
    if (pMere->pointEnCours.X() <= 0)
        return;
	// Transmet à la boite de Dialogue
	TScrollBar::SBLineUp();
    pMere->pointEnCours.Offset(-1, 0);
    pMere->pGroup->Invalidate();
}

//
// Flêche droite
//
void
NSOCRHorizScroll::SBLineDown()
{
	// Transmet à la boite de Dialogue
	TScrollBar::SBLineDown();
	pMere->pointEnCours.Offset(1, 0);
    pMere->pGroup->Invalidate();
}

void
NSOCRHorizScroll::SBPageDown()
{
   TScrollBar::SBPageDown();
   pMere->pointEnCours.Offset(iPageStep, 0);
   pMere->pGroup->Invalidate();
}

void
NSOCRHorizScroll::SBPageUp()
{
    TScrollBar::SBPageUp();
    pMere->pointEnCours.Offset(-iPageStep, 0);
    pMere->pGroup->Invalidate();
}

void
NSOCRHorizScroll::PositionneCurseur()
{
    /*
	//
	// Prise du nombre total de d'enregistrement et de la position actuelle
	//
	int NumEnCours;

	//
	// Positionnement du curseur
	//
    pListeDlg->pPatEnCours->donneNumRecEnCours(&NumEnCours);
	SetPosition(NumEnCours); */
}


