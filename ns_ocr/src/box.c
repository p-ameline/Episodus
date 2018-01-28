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

 Joerg.Schulenburg@physik.uni-magdeburg.de 
 
 Problems:

 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "gocr.h"
#include "pgm2asc.h"

/* for sorting letters by position on the image
/ ToDo: - use function same line like this or include lines.m1 etc. */
int box_gt(struct box *box1, struct box *box2) {
 // box1 after box2 ?
  if (box1->line > box2->line)
    return 1;
  if (box1->line < box2->line)
    return 0;
  if (box1->x0 > box2->x1)	// before
    return 1;
  if (box1->x1 < box2->x0)	// before
    return 0;
  if (box1->x0 > box2->x0)	// before,  overlapping!
    return 1;

  return 0;
}

/* --- copy part of pix p into new pix b	---- len=10000
 * Returns: 0 on success, 1 on error.
 */
int copybox (pix * p, int x0, int y0, int dx, int dy, pix * b, int len) {
  int x, y;

  /* test boundaries */
  if (b->p == NULL || dx < 0 || dy < 0 || dx * dy > len) {
    fprintf(stderr, " error-copybox x=%5d %5d  d=%5d %5d\n", x0, y0, dx, dy);
    return 1;
  }

  b->x = dx;
  b->y = dy;
  b->bpp = 1;
#ifdef FASTER_INCOMPLETE
  for (y = 0; y < dy; y++)
    memcpy(&pixel_atp(b, 0, y), &pixel_atp(p, x0, y + y0 ), dx);
  // and unmark pixels
#else
  for (y = 0; y < dy; y++)
    for (x = 0; x < dx; x++)
      pixel_atp(b, x, y) = pixel(p, x + x0, y + y0);
#endif

  return 0;
}

/* get memory for box and initialize the memory */
struct box *malloc_box (struct box *inibox) {
  struct box *buf;

  buf = (struct box *) malloc(sizeof(struct box));
  if (!buf)
    return NULL;
  if (inibox)
    memcpy(buf, inibox, sizeof(struct box));
  return buf;
}
