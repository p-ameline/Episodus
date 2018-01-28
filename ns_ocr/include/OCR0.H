//#include "pgm2asc.h"
#include "gocr.h"

int set_only_numbers(int n);

// tests for umlaut
int testumlaut(struct box *box1, int cs, int m, wchar_t *modifier);
// detect chars
wchar_t ocr0(struct box *box1, pix  *b, int cs);
// detect numbers
wchar_t ocr0n(struct box *box1, pix  *b, int cs);
