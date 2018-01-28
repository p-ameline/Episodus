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
 
#ifndef OUTPUT_H
#define OUTPUT_H

#include <stdlib.h>
#include <stdio.h>
#include "pnm.h"
#include "gocr.h"
#include "gocr_list.h"

extern int n_run;
extern gocrList boxlist;

void out_b(pix *b, int x0, int y0, int dx, int dy, int cs );
void out_x(struct box *px);
void out_x2(struct box *box1,struct box *box2);
void out_b2(pix *b, int x0, int y0, int dx, int dy, int cs, int x1, int y1 );
int output_list(pix * pp, char *lc);
int write_img(char *fname, pix * ppo, pix * p, int opt);


#endif
