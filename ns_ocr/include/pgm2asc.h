/*
This is a Optical-Character-Recognition program
Copyright (C) 2000  Joerg Schulenburg

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

 Joerg.Schulenburg@physik.uni-magdeburg.de */
 
#ifndef PGM2ASC_H
#define PGM2ASC_H 1

#include "pnm.h"
#include "output.h"
#include "gocr_list.h"
#include "unicode.h"

#ifdef __cplusplus
extern "C" {
#endif

#define pixel_at(pic, xx, yy)           (pic).p[(xx)+((yy)*((pic).x))]
#define pixel_atp(pic, xx, yy)          (pic)->p[(xx)+((yy)*((pic)->x))]

/* declared in box.c */
int box_gt(struct box *box1, struct box *box2);
struct box *malloc_box( struct box *inibox );
int copybox( pix *p, int x0, int y0, int dx, int dy, pix *b, int len);

/* declared in database.c */
int load_db(void);
wchar_t ocr_db(struct box *box1);

/* declared in detect.c */
int detect_lines1(pix * p, int x0, int y0, int dx, int dy);
int detect_lines2(pix *p,int x0,int y0,int dx,int dy,int r);
int detect_rotation_angle(pix *pp);
int detect_text_lines(pix * pp, int mo);
int detect_pictures(void);

// Add Nautilus
int add_line_info_enlarge(int iStepUp, int iStepDown);

/* declared in lines.c */
void store_boxtree_lines( int mo );
   /* free memory for internal stored textlines.
    * Needs to be called _after_ having retrieved the text.
    * After freeing, no call to getTextLine is possible any
    * more
    */
void free_textlines( void );

   /* get result of ocr for a given line number.
    * If the line is out of range, the function returns 0,
    * otherwise a pointer to a complete line.
    */
#ifdef USE_UNICODE
const wchar_t *getTextLine( int );
#else
const char *getTextLine( int );
#endif

/* declared in remove.c */
int remove_dust( pix *pp, int dust_size );
int remove_pictures( );
int remove_melted_serifs( pix *pp );
int remove_rest_of_dust();

/* declared in pixel.c */
int marked(pix * p, int x, int y);
int pixel(pix *p, int x, int y);
void put(pix * p, int x, int y, int ia, int io);

   /* start ocr on a image in pix */
   int pgm2asc(
	       pix *pp,			// see pnm.h, pixelbuffer and size
	       int mo, int cs, int spc,	// mode=0 cs=0 spacewidth=0
	       int dust_size, char *lc,	// dust_size=10 listchars="_"
	       int vvv,			// verbose=0
           char* pText, int iMaxText, int bKillBox);

int
giveBoxData(int iNumBox, int* piBox, int* pNumLine, int* px0, int* px1, int* py0, int* py1,
            int* px, int* py, int* pdots, wchar_t* pc, wchar_t* pac, char* pmod,
            int* pm1, int* pm2, int* pm3, int* pm4, int* pWidth, int* pHeigth);

int giveBoxPixels(int iBox, char* pPixels);

int deleteBoxData();

#ifdef __cplusplus
}
#endif

#endif
