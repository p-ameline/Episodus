#include <windows.h>#include <owl/window.h>
#include <list.h>

#include "_gocr.h"
#include "gocr.h"

#include "pnm.h"
#include "pgm2asc.h"

#include "partage\ns_vector.h"
#include "nautilus\nssuper.h"
#include "nautilus\nsepicap.h"
#include "nautilus\nsepisodview.h"
#include "ns_ocr\nsocrfct.h"
#include "ns_ocr\nsocruti.h"

//
// Implémentation de la méthode execute de NSDlgFonction
//
bool
OCR_run(NSContexte* pCtx, NSOCRbloc* pBloc, NSBitmap* pScrMap)
{
	// Technique avec pix

	pix* p = 0 ;

	if (!(OCR_imageInit(pCtx, pScrMap, pBloc, &p)))
  	return false ;

	int mode       = 0 ;
	int cs         = 0 ;
	int spaceWidth = 0 ;
	int dust_size  = 0 ;
	int verbose    = 0 ;

	//
	// Chargement des modèles
	//
    /*
    OCRBoxesArray* pBoxArray = new OCRBoxesArray;
    pBoxArray->charger(pCtx);
    if (!(pBoxArray->empty()))
    {
        for (boxIter boxIt = pBoxArray->begin();
                                    boxIt != pBoxArray->end(); boxIt++)
            ajouteModele((*boxIt)->x0, (*boxIt)->x1, (*boxIt)->y0, (*boxIt)->y1,
                         (*boxIt)->x, (*boxIt)->y, (*boxIt)->dots,
        wchar_t c,ac;	            // detected (alternate) char
        char modifier;	            // default=0, see compose() in unicode.c
        int num;		            // same nummer = same char
        int line;		            // line number (points to struct tlines lines)
        int m1,m2,m3,m4;
        int iWidth, iHeigth;

        NS_CLASSLIB::TRect  BoxRect;
        string              sTexte;
        string              sPixels;

        }
    } */

	char listchars[2] ;
	strcpy(listchars, "_") ;

	char result[1024] ;	int iResult = pgm2asc(p, mode, cs, spaceWidth, dust_size, listchars, verbose, result, 1024, 0) ;

	string sTexte = string(result) ;

	// ****** Provisoire : doit être fait au dessus *************
	OCRBoxesArray BoxArray ;
	BoxArray.charger(pCtx) ;
	// **********************************************************

	if (false == BoxArray.empty())
	{
  	bool bEtudierBox = true ;
    int  iNumBox     = 0 ;
    unsigned int iPosit = 0 ;
    while (bEtudierBox)    {
    	int     ix0, ix1, iy0, iy1, ix, iy, idots, iline, inum ;
      wchar_t cc, cac ;
      char    cmodif ;
      int     im1, im2, im3, im4 ;
      int     iBoxWidth, iBoxHeigth ;
      int iResu = giveBoxData(iNumBox, &inum, &iline, &ix0, &ix1, &iy0, &iy1,
                              &ix, &iy, &idots, &cc, &cac, &cmodif,
                              &im1, &im2, &im3, &im4, &iBoxWidth, &iBoxHeigth) ;

			if ((iResu != 0) && (cc != PICTURE) && (cc != '\n'))
      {
      	//int iBoxWidth   = ix1 - ix0 + 1;
        //int iBoxHeigth  = iy1 - iy0 + 1;

        if ((iBoxWidth > 0) && (iBoxHeigth > 0))
        {
        	char* pBoxPixels = new char[(iBoxHeigth * (iBoxWidth + 1)) + 1] ;

          int iResuPix = giveBoxPixels(iNumBox, pBoxPixels) ;
          if (iResuPix)
          {          	bool bModified = false ;
            //
            // Première passe : on utilise les informations de positionnement
            //
            for (boxIter boxIt = BoxArray.begin();
                                    boxIt != BoxArray.end(); boxIt++)
            {
            	if (((*boxIt)->sPixels == string(pBoxPixels)) &&
                                (iPosit < strlen(sTexte.c_str())))
              {
              	sTexte = string(sTexte, 0, iPosit) + (*boxIt)->sTexte +
                         string(sTexte, iPosit+1, strlen(sTexte.c_str()) - iPosit - 1) ;
                iPosit += strlen((*boxIt)->sTexte.c_str()) - 1 ;
                bModified = true ;
              }
            }
            //
            // Deuxième passe : sans utiliser les informations de positionnement
            //
            if (!bModified)
            {
            	string sCaptured = string(pBoxPixels) ;
              for (size_t i = 0; i <= strlen(sCaptured.c_str()); i++)
              {
              	switch (sCaptured[i])
                {
                	case 'A' :
                  case 'B' :
                  case 'C' :
                  case 'D' : sCaptured[i] = '1' ; break ;
                  case 'a' :
                  case 'b' :
                  case 'c' :
                  case 'd' : sCaptured[i] = '0' ; break ;
                }
              }
              string sModel ;
              for (boxIter boxIt = BoxArray.begin();
                                    boxIt != BoxArray.end(); boxIt++)
              {
              	sModel = (*boxIt)->sPixels ;

                for (size_t i = 0; i <= strlen(sModel.c_str()); i++)
                {
                	switch (sModel[i])
                  {
                  	case 'A' :                    case 'B' :
                    case 'C' :
                    case 'D' : sModel[i] = '1' ; break ;
                    case 'a' :
                    case 'b' :
                    case 'c' :
                    case 'd' : sModel[i] = '0' ; break ;
                  }
                }

                if ((sModel == sCaptured) && (iPosit < strlen(sTexte.c_str())))
                {
                	sTexte = string(sTexte, 0, iPosit) + (*boxIt)->sTexte +
                           string(sTexte, iPosit+1, strlen(sTexte.c_str()) - iPosit - 1) ;
                  iPosit += strlen((*boxIt)->sTexte.c_str()) - 1 ;
                  bModified = true ;
                }
              }
            }
          }

          delete[] pBoxPixels ;
        }

        iPosit++ ;
      }
      else if (cc == '\n')
      {
      	sTexte = string(sTexte, 0, iPosit) + string(1, '\r') + string(1, '\n') +
                 string(sTexte, iPosit+1, strlen(sTexte.c_str()) - iPosit - 1) ;
        iPosit += 2 ;
      }
      else
      	bEtudierBox = false ;

      iNumBox++ ;
    }
  }

  //
  // Changement de blocs de textes par d'autres
  //
  OCRSwapArray SwapArray ;
  SwapArray.charger(pCtx) ;
  SwapArray.transformerTexte(&sTexte) ;

	pBloc->sTexte = sTexte ;

	deleteBoxData() ;

	return true ;

/*
    gocr_Block block;

    // Initialisation du moteur    //
    if (gocr_init(0, NULL) != 0)
    {
        // *psErreur = string("Erreur d'initialisation du moteur d'OCR");
        return false;
    }

    // Ne donne que les messages d'erreur
    //
    gocr_setAttribute(VERBOSE, (void *)1);

    if (!(OCR_imageInit(pCtx, pScrMap, pBloc)))
    {
        gocr_finalize();
        return false;
    }

    block.x0 = block.y0 = 0;
    block.x1 = currentimage->width-1;
    block.y1 = currentimage->height-1;
    block.t = gocr_blockTypeGetByName("TEXT");
    gocr_blockAdd(&block);

    currentblock = &block;

    NSOCRFindChars();

    //------------------------- deleted
    gocr_charBegin();
    gocr_charSetRect(GOCR_SET, 11, 0, 20, 16);
    gocr_charEnd();

    //gocr_printBox(currentbox);

    gocr_charBegin();
    gocr_charSetAllNearPixels(GOCR_SET, 4, 9, 4);
    gocr_charEnd();

    //gocr_printBox(currentbox);

    gocr_charBegin();
    gocr_charSetAllNearPixels(GOCR_SET, 24, 2, 8);
    gocr_charEnd();

    //gocr_printBox(currentbox);
    //gocr_mainImageWriteWithData("charfind.ppm");
    //------------------------- !deleted
	gocr_imageClose() ;
	gocr_finalize() ;

	return true ;
*/
}
boolOCR_learn(NSContexte far *pCtx, NSOCRbloc far *pBloc, NSBitmap far *pScrMap)
{
// Technique avec pix

  pix* p = 0 ;

  if (!(OCR_imageInit(pCtx, pScrMap, pBloc, &p)))
  	return false ;

  int mode       = 0 ;
  int cs         = 0 ;
  int spaceWidth = 0 ;
  int dust_size  = 0 ;
  int verbose    = 0 ;
  char listchars[2] ;  strcpy(listchars, "_") ;

  char result[1024] ;
  int iResult ;
  iResult = pgm2asc(p, mode, cs, spaceWidth, dust_size, listchars, verbose, result, 1024, 0) ;

  NSOCRLearnDlg* pLearnDlg = new NSOCRLearnDlg(0, pCtx, pBloc, pScrMap) ;
  pLearnDlg->Execute() ;
  delete pLearnDlg ;

  deleteBoxData() ;

  return true ;
}

//-------------------------------------------------------------------------
//
//
//-------------------------------------------------------------------------
bool
OCR_imageInit(NSContexte* /* pCtx */, NSBitmap* pScrMap, NSOCRbloc* pBloc)
{
  // Si une image est déjà ouverte, on la ferme
  //
  if (currentimage)
		gocr_imageClose() ;

  if ((NULL == pScrMap) || (NULL == pBloc))    return false ;

  NS_CLASSLIB::TRect* pRect = &(pBloc->BlocRect) ;
  // allocate memory  currentimage = (gocr_Image *) malloc(sizeof(gocr_Image)) ;
  if (NULL == currentimage)
    return false ;

  currentimage->type = GOCR_BW ;
  // allocation mémoire pour les données de l'image  //
  // Attention width = right - left, mais le nombre total de points
  // est right - left + 1 ; idem pour height
  //
  currentimage->width  = pRect->Width() + 1 ;
  currentimage->height = pRect->Height() + 1 ;
  if (datamalloc(currentimage) == -1)
  {
    free(currentimage) ;
    return false ;
  }

  // remplissage de l'image
  bool bExist ;

  for (int iLigne = pRect->Top(); iLigne <= pRect->Bottom(); iLigne++)
  {
    for (int iCol = pRect->Left(); iCol <= pRect->Right(); iCol++)
    {
      if ((pScrMap->getColor(NS_CLASSLIB::TPoint(iCol, iLigne), &bExist) == pBloc->bgColor) || (!bExist))
        gocr_imagePixelSetBW(currentimage, iCol - pRect->Left(),
                                                       iLigne - pRect->Top(),
                                                       GOCR_WHITE) ;
      else
        gocr_imagePixelSetBW(currentimage, iCol - pRect->Left(),
                                                       iLigne - pRect->Top(),
                                                       GOCR_BLACK) ;
    }
  }
  currentimage->mask = NULL ;
  currentimage->sons = 0 ;
  currentimage->parent = NULL ;

  return true ;

    /*
    NSEpisodus* pEpisod = pCtx->getSuperviseur()->pEpisodus;
    if (!pEpisod)
        return false;
    PixelArray* pPixMap = pEpisod->getScreenMap();
    if (pPixMap->empty())
        return true;

    // allocate memory
	currentimage = (gocr_Image *) malloc(sizeof(gocr_Image));
	if (currentimage == NULL)
		return false;

    currentimage->type = GOCR_COLOR;

    // Recherche des dimensions de la capture
    //
    int iMaxX = 0;
    int iMaxY = 0;
    pixelIter it = pPixMap->begin();
    for ( ; it != pPixMap->end(); it++)
    {
        if (iMaxX < (*it)->getPoint().X())
            iMaxX = (*it)->getPoint().X();
        if (iMaxY < (*it)->getPoint().Y())
            iMaxY = (*it)->getPoint().Y();
    }

    // allocation mémoire pour les données de l'image
	currentimage->width = iMaxX;
	currentimage->height = iMaxY;
	if (datamalloc(currentimage) == -1)
    {
		free(currentimage);
		return false;
	}

    // remplissage de l'image
    for (it = pPixMap->begin(); it != pPixMap->end(); it++)
        gocr_imagePixelSetColor(currentimage, (*it)->getPoint().X(),
                                              (*it)->getPoint().Y(),
                                              (*it)->getTuple());

    //currentimage->filename = strdup(filename);	currentimage->mask = NULL;
	currentimage->sons = 0;
	currentimage->parent = NULL;

	_gocr_thresholdGrayToBW(currentimage);

    return false; */
}

boolOCR_imageInit(NSContexte* /* pCtx */, NSBitmap* pScrMap, NSOCRbloc* pBloc, pix** p)
{
  //
  // Si une image est déjà ouverte, on la ferme
  // If an image is already opened, we close it
  //
  if (!(*p))
    *p = (pix *)malloc(sizeof(pix)) ;

  if ((NULL == pScrMap) || (NULL == pBloc))
    return false ;

  //
  // Calcul du rectangle utile :
  //  1) trouver le plus petit rectangle qui contient le texte
  //  2) l'entourer d'un rectangle d'un pixel couleur du fond, afin de
  //      faciliter la reconnaissance des caractères qui affleurent le
  //      rectangle
  //
  // Finding the "usefull" rectangle :
  //  1) finding the smallest rectangle that contains the text
  //  2) add a one pixel border, in order to ease recognition for chars that
  //      touch the rectangle
  //
  if (false == OCR_rectInit(pScrMap, pBloc))
    return false ;

  pBloc->BlocOCR = pBloc->BlocOCR.InflatedBy(1, 1) ;

  NS_CLASSLIB::TRect* pRect = &(pBloc->BlocOCR) ;

  //
  // Attention width = right - left, mais le nombre total de points
  // est right - left + 1 ; idem pour height
  //
  (*p)->x     = pRect->Width() + 1;
  (*p)->y     = pRect->Height() + 1;
  (*p)->bpp   = 1;

  if (NULL == ((*p)->p = (unsigned char *)malloc(((*p)->x)*((*p)->y))) )
    return false ;

  //
  // remplissage de l'image - Filling the image
  //
  bool bExist ;

  int iL, iC ;
  int iW = pRect->Width() + 1 ;

  for (int iLigne = pRect->Top(); iLigne <= pRect->Bottom(); iLigne++)
  {
    iL = iLigne - pRect->Top() ;
    for (int iCol = pRect->Left(); iCol <= pRect->Right(); iCol++)
    {
      iC = iCol - pRect->Left() ;
      // White border rectangle
      if ((iLigne == pRect->Top()) || (iLigne == pRect->Bottom()) ||
              (iCol == pRect->Left()) || (iCol == pRect->Right()))
        (*p)->p[(iL*iW)+iC] = 255 ;
      // Ordinary points
      else if ((pScrMap->getColor(NS_CLASSLIB::TPoint(iCol, iLigne), &bExist) == pBloc->bgColor) || (!bExist))
        (*p)->p[(iL*iW)+iC] = 255 ;
      else
        (*p)->p[(iL*iW)+iC] = 0 ;
    }
  }
  return true ;
}

////  Trouver le plus petit rectangle qui contient le texte
//  Finding the smallest rectangle that contains the text
//
bool OCR_rectInit(NSBitmap* pScrMap, NSOCRbloc* pBloc)
{
    pBloc->BlocOCR = pBloc->BlocRect;

    bool bExist;
    bool bShorten;
    NS_CLASSLIB::TColor edgeColor;
    int i;
    int iMin;
    int iMax;

    //
    // Détection des bords "3D"
    //

    //
    // Bord gauche et horizontales
    //
    bShorten = true;

    while (bShorten)
    {
        iMin = pBloc->BlocOCR.Top();
        iMax = pBloc->BlocOCR.Bottom();

        int iMiddle = iMin + (iMax - iMin) / 2;
        int iLeft = pBloc->BlocOCR.Left();

        edgeColor = pScrMap->getColor(iLeft, iMiddle, &bExist);
        int iEdgeTop;
        int iEdgeBottom;

        if (bExist && (edgeColor != pBloc->bgColor))
        {
            // Recherche en descendant
            for (i = iMiddle + 1; i <= iMax; i++)
                if ((pScrMap->getColor(iLeft, i, &bExist) != edgeColor) || (!bExist))
                    break;
            iEdgeBottom = i;

            // Recherche en montant
            for (i = iMiddle - 1; i >= iMin; i--)
                if ((pScrMap->getColor(iLeft, i, &bExist) != edgeColor) || (!bExist))
                    break;
            iEdgeTop = i;

            bShorten = false;

            if ((iEdgeBottom > iMax) || (iEdgeTop < iMin))
            {
                // Le bord se continue jusqu'en bas, se prolonge-t'il par
                // une ligne continue horizontale sur presque toute la longueur
                //
                if (iEdgeBottom > iMax)
                {
                    for (i = pBloc->BlocOCR.Left()+1; i <= pBloc->BlocOCR.Right(); i++)
                        if ((pScrMap->getColor(i, iMax, &bExist) != edgeColor) || (!bExist))
                            break;
                    if (i >= pBloc->BlocOCR.Right()-2)
                    {
                        pBloc->BlocOCR = NS_CLASSLIB::TRect(iLeft + 1,
                                                pBloc->BlocOCR.Top(),
                                                pBloc->BlocOCR.Right(),
                                                pBloc->BlocOCR.Bottom()-1);
                        bShorten = true;
                    }
                }
                //
                // Le bord se continue jusqu'en haut, se prolonge-t'il par
                // une ligne continue horizontale sur presque toute la longueur
                //
                if (iEdgeTop < iMin)
                {
                    for (i = pBloc->BlocOCR.Left()+1; i <= pBloc->BlocOCR.Right(); i++)
                        if ((pScrMap->getColor(i, iMin, &bExist) != edgeColor) || (!bExist))
                            break;
                    if (i >= pBloc->BlocOCR.Right()-2)
                    {
                        pBloc->BlocOCR = NS_CLASSLIB::TRect(iLeft + 1,
                                                pBloc->BlocOCR.Top()+1,
                                                pBloc->BlocOCR.Right(),
                                                pBloc->BlocOCR.Bottom());
                        bShorten = true;
                    }
                }
            }
        }
        else
            bShorten = false;
    }

    //
    // Bord droit et horizontales
    //
    bShorten = true;

    while (bShorten)
    {
        iMin = pBloc->BlocOCR.Top();
        iMax = pBloc->BlocOCR.Bottom();

        int iMiddle = iMin + (iMax - iMin) / 2;
        int iRight = pBloc->BlocOCR.Right();

        edgeColor = pScrMap->getColor(iRight, iMiddle, &bExist);
        int iEdgeTop;
        int iEdgeBottom;

        if (bExist && (edgeColor != pBloc->bgColor))
        {
            // Recherche en descendant
            for (i = iMiddle + 1; i <= iMax; i++)
                if ((pScrMap->getColor(iRight, i, &bExist) != edgeColor) || (!bExist))
                    break;
            iEdgeBottom = i;

            // Recherche en montant
            for (i = iMiddle - 1; i >= iMin; i--)
                if ((pScrMap->getColor(iRight, i, &bExist) != edgeColor) || (!bExist))
                    break;
            iEdgeTop = i;

            bShorten = false;

            if ((iEdgeBottom > iMax) || (iEdgeTop < iMin))
            {
                // Le bord se continue jusqu'en bas, se prolonge-t'il par
                // une ligne continue horizontale sur presque toute la longueur
                //
                if (iEdgeBottom > iMax)
                {
                    for (i = pBloc->BlocOCR.Right()-1; i >= pBloc->BlocOCR.Left(); i--)
                        if ((pScrMap->getColor(i, iMax, &bExist) != edgeColor) || (!bExist))
                            break;
                    if (i <= pBloc->BlocOCR.Left()+2)
                    {
                        pBloc->BlocOCR = NS_CLASSLIB::TRect(
                                                pBloc->BlocOCR.Left(),
                                                pBloc->BlocOCR.Top(),
                                                iRight-1,
                                                pBloc->BlocOCR.Bottom()-1);
                        bShorten = true;
                    }
                }
                //
                // Le bord se continue jusqu'en haut, se prolonge-t'il par
                // une ligne continue horizontale sur presque toute la longueur
                //
                if (iEdgeTop < iMin)
                {
                    for (i = pBloc->BlocOCR.Right()-1; i >= pBloc->BlocOCR.Left(); i--)
                        if ((pScrMap->getColor(i, iMin, &bExist) != edgeColor) || (!bExist))
                            break;
                    if (i <= pBloc->BlocOCR.Left()+2)
                    {
                        pBloc->BlocOCR = NS_CLASSLIB::TRect(
                                                pBloc->BlocOCR.Left(),
                                                pBloc->BlocOCR.Top()+1,
                                                iRight-1,
                                                pBloc->BlocOCR.Bottom());
                        bShorten = true;
                    }
                }
            }
        }
        else
            bShorten = false;
    }

    //
    // On cherche le bord gauche - finding left edge
    //
    bShorten = true;

    iMin = pBloc->BlocOCR.Top();
    iMax = pBloc->BlocOCR.Bottom();
    while (bShorten)
    {
        int iLeft = pBloc->BlocOCR.Left();
        for (i = iMin; i <= iMax; i++)
            if ((pScrMap->getColor(iLeft, i, &bExist) != pBloc->bgColor) || (!bExist))
                    break;

        if (i <= iMax)
        {
            if (iMax - iMin > 10)
                bShorten = OCR_isPattern(pScrMap, pBloc, leftSide);
            else
                bShorten = false;
        }
        else if (iLeft >= pBloc->BlocOCR.Right())
            return false;

        if (bShorten)
            pBloc->BlocOCR = NS_CLASSLIB::TRect(iLeft + 1,
                                                pBloc->BlocOCR.Top(),
                                                pBloc->BlocOCR.Right(),
                                                pBloc->BlocOCR.Bottom());
    }
    //
    // bord droit - right edge
    //
    bShorten = true;
    while (bShorten)
    {
        int iRight = pBloc->BlocOCR.Right();
        for (i = iMin; i <= iMax; i++)
            if ((pScrMap->getColor(iRight, i, &bExist) != pBloc->bgColor) || (!bExist))
                    break;

        if (i <= iMax)
        {
            if (iMax - iMin > 10)
                bShorten = OCR_isPattern(pScrMap, pBloc, rightSide);
            else
                bShorten = false;
        }
        else if (iRight <= pBloc->BlocOCR.Left()) // useless
            return false;

        if (bShorten)
            pBloc->BlocOCR = NS_CLASSLIB::TRect(pBloc->BlocOCR.Left(),
                                                pBloc->BlocOCR.Top(),
                                                iRight - 1,
                                                pBloc->BlocOCR.Bottom());
    }
    //
    // haut - top
    //
    iMin = pBloc->BlocOCR.Left();
    iMax = pBloc->BlocOCR.Right();
    bShorten = true;
    while (bShorten)
    {
        int iTop = pBloc->BlocOCR.Top();
        for (i = iMin; i <= iMax; i++)
            if ((pScrMap->getColor(i, iTop, &bExist) != pBloc->bgColor) || (!bExist))
                    break;

        if (i <= iMax)
        {
            if (iMax - iMin > 10)
                bShorten = OCR_isPattern(pScrMap, pBloc, topSide);
            else
                bShorten = false;
        }        else if (iTop >= pBloc->BlocOCR.Bottom()) // useless
            return false;

        if (bShorten)
            pBloc->BlocOCR = NS_CLASSLIB::TRect(pBloc->BlocOCR.Left(),
                                                iTop + 1,
                                                pBloc->BlocOCR.Right(),
                                                pBloc->BlocOCR.Bottom());
    }
    //
    // Bas - Bottom
    //
    bShorten = true;
    while (bShorten)
    {
        int iBottom = pBloc->BlocOCR.Bottom();
        for (i = iMin; i <= iMax; i++)
            if ((pScrMap->getColor(i, iBottom, &bExist) != pBloc->bgColor) || (!bExist))
                    break;

        if (i <= iMax)
        {
            if (iMax - iMin > 10)
                bShorten = OCR_isPattern(pScrMap, pBloc, bottomSide);
            else
                bShorten = false;
        }
        else if (iBottom <= pBloc->BlocOCR.Top()) // useless
            return false;

        if (bShorten)
            pBloc->BlocOCR = NS_CLASSLIB::TRect(pBloc->BlocOCR.Left(),
                                                pBloc->BlocOCR.Top(),
                                                pBloc->BlocOCR.Right(),
                                                iBottom - 1);
    }
    return true;
}

bool OCR_isPattern(NSBitmap* pScrMap, NSOCRbloc* pBloc, int iEdge){
    int iMinX = pBloc->BlocOCR.Left();
    int iMaxX = pBloc->BlocOCR.Right();
    int iMinY = pBloc->BlocOCR.Top();
    int iMaxY = pBloc->BlocOCR.Bottom();
    int iX, iY;
    int iDeltaX, iDeltaY;

    int iMinPatX, iMaxPatX;
    int iMinPatY, iMaxPatY;

    switch(iEdge)
    {
        case leftSide :
            iMinPatX = pBloc->BlocOCR.Left();
            iMaxPatX = pBloc->BlocOCR.Left();
            iMinPatY = iMinY + ((iMaxY - iMinY) / 2);
            iMaxPatY = iMinPatY;
            iX = pBloc->BlocOCR.Left();
            iY = iMinPatY;
            iDeltaX = 0;
            iDeltaY = 1;
            break;
        case rightSide :
            iMinPatX = pBloc->BlocOCR.Right();
            iMaxPatX = pBloc->BlocOCR.Right();
            iMinPatY = iMinY + ((iMaxY - iMinY) / 2);
            iMaxPatY = iMinPatY;
            iX = pBloc->BlocOCR.Right();
            iY = iMinPatY;
            iDeltaX = 0;
            iDeltaY = 1;
            break;
        case topSide :
            iMinPatX = iMinX + ((iMaxX - iMinX) / 2);
            iMaxPatX = iMinPatX;
            iMinPatY = pBloc->BlocOCR.Top();
            iMaxPatY = pBloc->BlocOCR.Top();
            iX = iMinPatX;
            iY = pBloc->BlocOCR.Top();
            iDeltaX = 1;
            iDeltaY = 0;
            break;
        case bottomSide :
            iMinPatX = iMinX + ((iMaxX - iMinX) / 2);
            iMaxPatX = iMinPatX;
            iMinPatY = pBloc->BlocOCR.Bottom();
            iMaxPatY = pBloc->BlocOCR.Bottom();
            iX = iMinPatX;
            iY = pBloc->BlocOCR.Bottom();
            iDeltaX = 1;
            iDeltaY = 0;
            break;
    }

    bool bExist;
    NS_CLASSLIB::TColor startColor = pScrMap->getColor(iX, iY, &bExist);
    if (!bExist)
        return false;

    //
    // Premier secteur : iMinpat = point le plus haut de la couleur de départ,
    //                   iMaxpat = point le plus bas
    //
    while ( (iMinPatX + iDeltaY > iMinX * iDeltaX) && (iMinPatY + iDeltaX > iMinY * iDeltaY) &&
            (pScrMap->getColor(iMinPatX - iDeltaX, iMinPatY - iDeltaY, &bExist) == startColor) &&
            (bExist))
    {
        iMinPatX -= iDeltaX;
        iMinPatY -= iDeltaY;
    }

    while ( (iMaxPatX * iDeltaX < iMaxX) && (iMaxPatY * iDeltaY < iMaxY) &&
            (pScrMap->getColor(iMaxPatX + iDeltaX, iMaxPatY + iDeltaY, &bExist) == startColor) &&
            (bExist))
    {
        iMaxPatX += iDeltaX;
        iMaxPatY += iDeltaY;
    }

    if ((iMaxPatX * iDeltaX >= iMaxX) || (iMaxPatY * iDeltaY >= iMaxY))
        return false;

    iMaxPatX += iDeltaX;
    iMaxPatY += iDeltaY;

    //
    // Second secteur, jusqu'à retrouver la couleur de départ
    // iMaxpat = point le plus bas du motif, juste avant de retrouver la couleur
    // de départ
    //
    // On pourraît améliorer en testant un motif comportant un seul secteur de
    // la couleur de départ, puis deux motifs, puis trois motifs...
    //
    while ( (iMaxPatX < iMaxX) && (iMaxPatY < iMaxY) &&
            (pScrMap->getColor(iMaxPatX + iDeltaX, iMaxPatY + iDeltaY, &bExist) != startColor) &&
            (bExist))
    {
        iMaxPatX += iDeltaX;
        iMaxPatY += iDeltaY;
    }

    int iReqPatn = 3;

    int iPatternWidth = (iMaxPatX - iMinPatX) * iDeltaX + (iMaxPatY - iMinPatY) * iDeltaY + 1;

    //
    // A-t-on la place de caser le bon nombre de patterns ?
    //
    if (((iMinPatX + iReqPatn * iPatternWidth) * iDeltaX > iMaxX) ||
        ((iMinPatY + iReqPatn * iPatternWidth) * iDeltaY > iMaxY))
        return false;

    //
    // On regarde si les points qui suivent reproduisent le pattern
    //
    for (int iNumPattern = 1; iNumPattern <= iReqPatn; iNumPattern++)
    {
        for (int j = (iMinPatX*iDeltaX)+(iMinPatY*iDeltaY); j <= (iMaxPatX*iDeltaX)+(iMaxPatY*iDeltaY); j++)
        {
            int k = j + iPatternWidth * iNumPattern;

            int iX0 = iX * iDeltaY + j * iDeltaX;
            int iX1 = iX * iDeltaY + k * iDeltaX;

            int iY0 = iY * iDeltaX + j * iDeltaY;
            int iY1 = iY * iDeltaX + k * iDeltaY;

            if ((pScrMap->getColor(iX0, iY0, &bExist) !=
                                    pScrMap->getColor(iX1, iY1, &bExist)) ||
                                                                    (!bExist))
                return false;
        }
    }
    return true;
}

bool NSOCRFindChars(){
  // int iXpos = 0 ;
  bool bChar = false ;

  ClassLib::TRect rectLigne = ClassLib::TRect(0,                      // left
                                              currentimage->height-1, // top
                                              currentimage->width-1,  // right
                                              0                       // bottom                                              ) ;

  int iLeftCol  = 0 ;    // Colonne de gauche de la boite en cours
  int iRightCol = 0 ;    // Colonne de droite de la boite en cours

  // On cherche les boites qui contiennent les caractères
  //
  for (int i = rectLigne.Left(); i < rectLigne.Right(); i++)
  {
  	//
    // Si on est sur une colonne "blanche" :
    // - si une boite était en cours, on la déclare
    // - sinon, on passe à la colonne suivante
    //
    if (NSOCRcolonneVide(i, rectLigne.Bottom(), rectLigne.Top()))
    {
    	if (bChar)
      {
      	gocr_charBegin() ;
        gocr_charSetRect(GOCR_SET, iLeftCol, rectLigne.Bottom(),
                                              iRightCol, rectLigne.Top()) ;
        gocr_charEnd() ;

        // gocr_runModule(charRecognizer, currentbox);
        // gocr_runModule(contextCorrection, currentbox);

        box charBox ;

        bChar = false ;
      }
    }
    //
    // La colonne n'est pas vide :
    // - si une boite était en cours, on passe à la colonne suivante
    // - sinon on démarre une nouvelle boite
    //
    else
    {
    	if (false == bChar)
      {
      	bChar = true ;
        iLeftCol = i ;
      }
      else
      	iRightCol = i ;
    }
  }
  //
  // On traite l'éventuelle dernière boîte
  //
  if (bChar)
  {
  	gocr_charBegin() ;
    gocr_charSetRect(GOCR_SET, iLeftCol, rectLigne.Bottom(),
                                          iRightCol, rectLigne.Top()) ;
    gocr_charEnd() ;
  }

  return true ;
}


bool NSOCRcolonneVide(int iCol, int TopLine, int BottomLine)
{
    for (int i = TopLine; i <= BottomLine; i++)
        if (gocr_imagePixelGetBW(currentimage, iCol, i) == GOCR_BLACK)
            return false;

    return true;
}

bool OCR_fontLearn()
{
    TMemoryDC* pMemDC = new TMemoryDC(/**pWinDC*/);

    LOGFONT logFont;
    strcpy(logFont.lfFaceName, "System");

    TFont* pPolice = new TFont(&logFont);
	pMemDC->SelectObject(*pPolice);



    // ClassLib::TSize GetTextExtent(const char far* string, int stringLen);

    delete pPolice;

    delete pMemDC;

    return true;
}
