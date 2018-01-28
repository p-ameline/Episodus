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

#include <string.h>
#include "unicode.h"
#include "output.h"
#include "pcx.h"

/* function is only for debugging and for developing
   it prints out a part of pixmap b at point x0,y0 to stderr
   using dots .,; if no pixel, and @xoO for pixels
   modify n_run and print out what would happen on 2nd, 3th loop!  */
void out_b(pix *b, int x0, int y0, int dx, int dy, int cs ){
  int x,y,x2,y2,tx,ty,n1;
  static const char c1[]=".,,,,,,;@1234xoO";
  tx=dx/80+1;
  ty=dy/40+1; // step, usually 1, but greater on large maps 
  fprintf(stderr,"# list pattern   x=%4d %4d d=%3d %3d t=%d %d\n",x0,y0,dx,dy,tx,ty);

  for(y=y0;y<y0+dy;y+=ty) { // reduce the output to max 78x40
    for(x=x0;x<x0+dx;x+=tx){ n1=0;
      for(y2=y;y2<y+ty && y2<y0+dy && n1==0;y2++) /* Mai2000 */
      for(x2=x;x2<x+tx && x2<x0+dx && n1==0;x2++)
#if 0
      if((pixel(b,x2,y2)<cs)){ n1=8+marked(b,x2,y2); }
#else
      {
        if((pixel(b,x2,y2)<cs)){ n1=8; }
        if(n_run==0){
          n_run++; if(!n1) if((pixel(b,x2,y2)<cs)){ n1= 9; }
          n_run++; if(!n1) if((pixel(b,x2,y2)<cs)){ n1=10; }
          n_run++; if(!n1) if((pixel(b,x2,y2)<cs)){ n1=11; }
          n_run=0;
        }
      }
#endif
      fprintf(stderr,"%c", c1[n1] );
    }
    if ( dx>0 ) fprintf(stderr,"\n");
  }
}

/* same as out_b, but for faster use, only a box as argument
 */
void out_x(struct box *px) {
  int y0;
  y0 = px->y0;
  fprintf(stderr, "# list box dots=%d c=%04x=%c line=%d m= %d %d %d %d r= %d %d\n",
	  px->dots, (int)px->c,
	  (char)((px->c>=32 && px->c<=127)?px->c:'_'), px->line,
	  px->m1 - y0, px->m2 - y0, px->m3 - y0, px->m4 - y0,
	  px->x - px->x0, px->y - y0);
  if( px->dots && 2*y0>px->m1+px->m2 && px->m1>0) y0=px->m1;
  out_b(px->p, px->x0, y0, px->x1 - px->x0 + 1, px->y1 - y0 + 1, env.cs);
}


/* print out two boxes side by side, for debugging comparision algos */
void out_x2(struct box *box1, struct box *box2){
  int x,y,i,tx,ty,dy;
  static char *c1="OXXXXxx@.,,,,,,,";
  pix *b=env.p;
  dy=(box1->y1-box1->y0+1);
  if(dy<box2->y1-box2->y0+1)dy=box2->y1-box2->y0+1;
  tx=(box1->x1-box1->x0)/40+1;
  ty=(box1->y1-box1->y0)/40+1; // step, usually 1, but greater on large maps 
  fprintf(stderr,"\n# list 2 patterns");
  for(i=0;i<dy;i+=ty) { // reduce the output to max 78x40???
    fprintf(stderr,"\n"); y=box1->y0+i;
    for(x=box1->x0;x<=box1->x1;x+=tx) 
    fprintf(stderr,"%c", c1[ ((pixel(b,x,y)<env.cs)?0:8)+marked(b,x,y) ] );
    fprintf(stderr,"  "); y=box2->y0+i;
    for(x=box2->x0;x<=box2->x1;x+=tx)
    fprintf(stderr,"%c", c1[ ((pixel(b,x,y)<env.cs)?0:8)+marked(b,x,y) ] );
  }
}

/* very similar to out_x2, should be removed later */
void out_b2(pix *b, int x0, int y0, int dx, int dy, int cs, int x1, int y1 ){
  int x,y; static char *c1="OXXXXxx@.,,,,,,,";
  fprintf(stderr," x=%5d %5d  d=%5d %5d\n",x0,y0,dx,dy);
  for(y=y0;y<y0+dy;y++) {
    for(x=x0;x<x0+dx;x++) 
    fprintf(stderr,"%c", c1[ ((pixel(b,x,y)      <cs)?0:8)+marked(b,x,y)       ] );
    if( dx>0 ) fprintf(stderr," = ");
    for(x=x1;x<x1+dx;x++) 
    fprintf(stderr,"%c", c1[ ((pixel(b,x,y+y1-y0)<cs)?0:8)+marked(b,x,y+y1-y0) ] );
    if ( dx>0 ) fprintf(stderr,"\n");
  }
}

/* ---- list output ---- for debugging --- */
int output_list(pix * pp, char *lc) {
  int i = 0, cs = env.cs;
  struct box *box2;

  for_each_data(&boxlist) {
    box2 = (struct box *) gocrlist_get_current(&boxlist);
    if (strchr(lc, box2->c)) {
      fprintf(stderr,
	      "# list shape %3d x=%4d %4d d=%3d %3d h=%d o=%d dots=%d %04x %c\n",
	      i, box2->x0, box2->y0,
	      box2->x1 - box2->x0 + 1,
	      box2->y1 - box2->y0 + 1,
	      num_hole(box2->x0, box2->x1, box2->y0, box2->y1, pp, cs),
	      num_obj( box2->x0, box2->x1, box2->y0, box2->y1, pp, cs),
	      box2->dots, (int)box2->c,   /* wchar_t -> char ???? */
	      (char)((box2->c>=32 && box2->c<=127)?box2->c:'_') );
      if (env.vvv & 4) {
	out_x(box2);
      }
    }
    i++;
  } end_for_each(&boxlist);
  return 0;
}

/* --- output of image incl. corored lines usefull for developers --- */
int write_img(char *fname, pix * ppo, pix * p, int opt) {
  struct box *box2;
  int x, y, ic;

  if( opt & 4 )		/* refresh debug image */
    for(y=0;y<p->y;y++)
      for(x=0;x<p->x;x++)
        ppo->p[x+(p->x)*y]=p->p[x+(p->x)*y]&192;

  /* for(x=0;x<ppo->x;x++)if((x&35)>32)
     put(ppo,x,lines.longest_line+dy*x/ppo.x,255,32); */

#if 0	/* struct tlines lines is not declared here, find elegant way */
  if( opt & 4 )
  {
      dy = lines.dy;
      for (i = 0; i < lines.num; i++) {	// mark lines
	for (x = lines.x0[i]; x < lines.x1[i]; x++) {
	  y = lines.m1[i];
	  if ((x & 7) == 4)    put(&ppo, x, y + dy * x / ppo.x, 255, 32);
	  y = lines.m2[i];
	  if ((x & 3) == 2)    put(&ppo, x, y + dy * x / ppo.x, 255, 32);
	  y = lines.m3[i];
	  if ((x & 1) == 1)    put(&ppo, x, y + dy * x / ppo.x, 255, 32);
	  y = lines.m4[i];
	  if ((x & 7) == 4)    put(&ppo, x, y + dy * x / ppo.x, 255, 32);
	}
      }
  }
#endif   

  ic = ((opt & 2) ? 1 : 2);
  for_each_data(&boxlist) {
    box2 = (struct box *) gocrlist_get_current(&boxlist);
    if (box2->c != ' ' && box2->c != '\n') {
      for (y = box2->y0; y <= box2->y1; y += ic)
	ppo->p[box2->x0 + y * p->x] |= 32;
      for (y = box2->y0; y <= box2->y1; y += ic)
	ppo->p[box2->x1 + y * p->x] |= 32;
      for (x = box2->x0; x <= box2->x1; x += ic)
	ppo->p[x + box2->y0 * p->x] |= 32;
      for (x = box2->x0; x <= box2->x1; x += ic)
	ppo->p[x + box2->y1 * p->x] |= 32;
#ifdef USE_UNICODE
      if (box2->c == UNKNOWN)
#else
      if (box2->c == '_')
#endif
	for (y = box2->y0 + 1; y < box2->y1; y++)
	  for (x = box2->x0 + 1; x < box2->x1; x++)
	    if ((1 & (x + y)) != 0 || ic == 1)
	      ppo->p[x + y * p->x] |= 16;
    }
  } end_for_each(&boxlist);
  if( strstr(fname,".pgm") ) writepgm(fname,ppo);
  else writebmp(fname, *ppo, env.vvv);	// colored should be better
  return 0;
}
