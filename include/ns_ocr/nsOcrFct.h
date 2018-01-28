#ifndef __NSOCRFCT_H#define __NSOCRFCT_H

bool _export OCR_run(NSContexte far *pCtx, NSOCRbloc far *pBloc, NSBitmap far *pScrMap);bool _export OCR_learn(NSContexte far *pCtx, NSOCRbloc far *pBloc, NSBitmap far *pScrMap);

bool OCR_imageInit(NSContexte* pCtx, NSBitmap* pScrMap, NSOCRbloc* pBloc);
bool OCR_imageInit(NSContexte* pCtx, NSBitmap* pScrMap, NSOCRbloc* pBloc, pix** p);
bool OCR_rectInit(NSBitmap* pScrMap, NSOCRbloc* pBloc);
bool OCR_isPattern(NSBitmap* pScrMap, NSOCRbloc* pBloc, int iEdge);
bool NSOCRFindChars();
bool NSOCRcolonneVide(int iCol, int TopLine, int BottomLine);
bool OCR_fontLearn();

enum RECTSIDES { leftSide = 0, topSide, rightSide, bottomSide };

#endif

