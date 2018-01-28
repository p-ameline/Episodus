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

#include "pgm2asc.h"
#include <string.h>

// ------------------ (&~7)-pixmap-functions ------------------------
 
/* test if pixel marked?
 * Returns: 0 if not marked, least 3 bits if marked.
 */
int marked (pix * p, int x, int y) {
  if (x < 0 || y < 0 || x >= p->x || y >= p->y)
    return 0;
  return (pixel_atp(p, x, y) & 7);
}

#define Nfilt3 6 /* number of 3x3 filter */
const char filt3[Nfilt3][9]={ 
  {0,0,0, 0,0,1, 1,0,0}, /* (-1,-1) (0,-1) (0,1)  (-1,0) (0,0) ... */
  {0,0,0, 1,0,1, 0,0,0},
  {1,0,0, 0,0,1, 0,0,0},
  {1,1,0, 0,1,0, 2,1,1},
  {0,0,1, 0,0,0, 2,1,0},
  {0,1,0, 0,0,0, 1,2,0}
};
/* 2=ignore_pixel, 0=white_background, 1=black_pixel */

/* this function is heavily used
 * test if pixel was set, remove low bits (marks) --- later with error-correction
 * result depends on n_run, if n_run>0 filter are used
 * Returns: pixel-color (without marks)
 */
int pixel(pix *p, int x, int y){
  int i;
  static char c33[9];
  
  if ( x < 0 || y < 0 || x >= p->x || y >= p->y ) 
    return 255 & ~7;

  if (n_run & 2) { /* use the filters (corection of errors) */
    memset(c33, 0, sizeof(c33));
    /* copy environment of a point (only highest bit)
       bbg: FASTER now. It has 4 ifs less at least, 8 at most. */
    if (x > 0) {	c33[3] = pixel_atp(p,x-1, y )>>7;
      if (y > 0)	c33[0] = pixel_atp(p,x-1,y-1)>>7;
      if (y+1 < p->y)	c33[6] = pixel_atp(p,x-1,y+1)>>7;
    }
    if (x+1 < p->x) {	c33[5] = pixel_atp(p,x+1, y )>>7;
      if (y > 0)	c33[2] = pixel_atp(p,x+1,y-1)>>7;
      if (y+1 < p->y)	c33[8] = pixel_atp(p,x+1,y+1)>>7;
    }
    if (y > 0)		c33[1] = pixel_atp(p, x ,y-1)>>7;
			c33[4] = pixel_atp(p, x , y )>>7;
    if (y+1 < p->y)	c33[7] = pixel_atp(p, x ,y+1)>>7;

    /* do filtering */
    for (i = 0; i < Nfilt3; i++)
      if( ( (filt3[i][0]>>1) || c33[0]!=(1 & filt3[i][0]) )
       && ( (filt3[i][1]>>1) || c33[1]!=(1 & filt3[i][1]) )
       && ( (filt3[i][2]>>1) || c33[2]!=(1 & filt3[i][2]) ) 
       && ( (filt3[i][3]>>1) || c33[3]!=(1 & filt3[i][3]) )
       && ( (filt3[i][4]>>1) || c33[4]!=(1 & filt3[i][4]) )
       && ( (filt3[i][5]>>1) || c33[5]!=(1 & filt3[i][5]) ) 
       && ( (filt3[i][6]>>1) || c33[6]!=(1 & filt3[i][6]) )
       && ( (filt3[i][7]>>1) || c33[7]!=(1 & filt3[i][7]) )
       && ( (filt3[i][8]>>1) || c33[8]!=(1 & filt3[i][8]) ) )
        return ((filt3[i][4])?env.cs:0);
    return pixel_atp(p, x, y) & ~7;
  }

  if ((n_run&1) && x > 0 && y > 0 && x+1 < p->x && y+1 < p->y){ 
    int r; // filter
    r = pixel_atp(p,x,y)&~7;
    /* {2,2,2, 2,0,1, 2,1,0} */
    if ((r&128) && (~pixel_atp(p,x+1, y )&128)
		&& (~pixel_atp(p, x ,y+1)&128)
		&& ( pixel_atp(p,x+1,y+1)&128)) 
	r = 64; /* faxfilter */

    else
    /* {2,2,2, 1,0,2, 0,1,2} */
    if ((r&128) && (~pixel_atp(p,x-1, y )&128)
		&& (~pixel_atp(p, x ,y+1)&128)
		&& ( pixel_atp(p,x-1,y+1)&128)) 
	r = 64; /* faxfilter */
    return r & ~7;
  }
  return (pixel_atp(p,x,y) & ~7);
}

/* modify pixel, test if out of range */
void put(pix * p, int x, int y, int ia, int io) {
  if (x < p->x && x >= 0 && y >= 0 && y < p->y)
    pixel_atp(p, x, y) = (pixel_atp(p, x, y) & ia) | io;
}
