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
*/

#include <stdlib.h>
#include <stdio.h>
#include "pgm2asc.h"
#include "gocr.h"

/* ---- remove dust ---------------------------------
   What is dust? I think, this is a very small pixel cluster without
   neighbours. Of course not all dust clusters can be detected correct.
   This feature should be possible to switch off via option.
   -> may be, all clusters should be stored here?
   speed is very slow, I know, but I am happy that it is working well
*/
int remove_dust( pix *pp, int dust_size ){
  // new dust removing
  int i,j,x,y,ds,x0,x1,y0,y1,df=100000,dd,cs,vvv=env.vvv; // dust found
  struct box *box2;
  i=df-1; // dustsize should be revers proportional to its number
  dd=dust_size/16+1;	// step
  cs=env.cs;
  for( ds=dd;i<df && ds<dust_size && i>0;ds+=dd )
  { df=i;i=0;
    if(vvv){ fprintf(stderr,"# searching dust size<%2d",ds+1);
             if(ds>2)fprintf(stderr," and remove detected dust");
             fprintf(stderr," ..."); 
    }
    if(ds==dd){
      for_each_data(&boxlist) {
        box2 = (struct box *)gocrlist_get_current(&boxlist);
        box2->dots = 0;
        for(x=box2->x0;x<=box2->x1;x++)
          for(y=box2->y0;y<=box2->y1;y++){
            if( pixel(pp,x,y)<cs ){ box2->dots++; }
          }
      } end_for_each(&boxlist);
    }
    i = 0;
    for_each_data(&boxlist) {
      box2 = (struct box *)gocrlist_get_current(&boxlist);
      x0=box2->x0;x1=box2->x1;y0=box2->y0;y1=box2->y1;	// box
      j=box2->dots;
      if(j<=ds-dd)      /* remove this */
      { /* realy ??? do not remove i-dots */
        if( get_bw((x0+x1)/2,(x0+x1)/2,y1+4*(y1-y0+1),y1+8*(y1-y0+1),pp,cs,2) == 0)
        continue;
        env.numC--;env.sumX-=x1-x0+1;env.sumY-=y1-y0+1;
        for(x=x0;x<=x1;x++)
        for(y=y0;y<=y1;y++){ put(pp,x,y,0,255&~7); }

	gocrlist_del(&boxlist,box2);	
	free(box2);
	continue;
      }
      if(j<=ds  ) i++; /* count as dust particle */
    } end_for_each(&boxlist);
    if(vvv)fprintf(stderr," %3d cluster detected\n",i);
  }
  /* reset dots to 0 */
  for_each_data(&boxlist) {
    Box *b = ((struct box *)gocrlist_get_current(&boxlist));
    b->dots = 0;
  } end_for_each(&boxlist);
  return 0;
}

/* ---- remove pictures ------------------------------------------
 *   may be, not deleting or moving to another list is much better!
 */
int remove_pictures( ){
  struct box *box4;
  int j=0;

  if (env.vvv)
    fprintf(stderr, "# remove pictures ??? ...");
  for_each_data(&boxlist) {
    box4 = (struct box *)gocrlist_get_current(&boxlist);
    if( box4->c==PICTURE ) {
          if(env.vvv) fprintf(stderr,"\n# ... found picture at %4d %4d size %4d %4d",
              box4->x0, box4->y0, box4->x1-box4->x0+1, box4->y1-box4->y0+1 );
    	  j++;
#if 0
	  gocrlist_del(&boxlist, box4);	/* delete or output <IMG=...> */
    	  free(box4);
#endif
    }
  } end_for_each(&boxlist);
  if (env.vvv)
    fprintf(stderr," %d cluster detected\n",j);
  return 0;
}

  /* ---- remove melted serifs --------------------------------- v0.2.5
                >>v<<
        ##########.########
        ###################  like X VW etc.
        ...###.......###...
        ...###......###....
  - can generate new boxes if two characters were glued
  */
int remove_melted_serifs( pix *pp ){
  int x,y,j1,i,ii,cs,x0,x1,xa,xb,y0,y1,vvv=env.vvv;
  struct box *box2, *box3;

  cs=env.cs; i=0; ii=0;
  if(vvv){ fprintf(stderr,"# searching melted serifs ..."); }
  for_each_data(&boxlist) {
    box2 = (struct box *)gocrlist_get_current(&boxlist);
    x0=box2->x0; x1=box2->x1; 
    y0=box2->y0; y1=box2->y1;	// box
    // upper serifs
    for(j1=x0;j1+4<x1;){
      j1+=loop(pp,j1,y0,x1-x0,cs,0,RI);
      x  =loop(pp,j1,y0,x1-x0,cs,1,RI); if(j1+x>x1+1) break;
      y  =y0+(y1-y0+1+4)/8; 
      if(x>5)
      if( num_cross(j1	       ,j1+x    ,y,y,pp,cs)   >= 2
       && num_cross(j1+x/4     ,j1+x-x/4,y,y,pp,cs)   >= 1  // ~7+serifs
      /* not a italic F with strong serifs */
       && num_cross(j1	       ,j1+x  ,y0+(y1-y0)/4,y0+(y1-y0)/4,pp,cs) >= 2
       && num_cross(j1	       ,j1+x/2,y,y,pp,cs)     == 1 )
      if(   get_bw(j1	       ,j1+0*x/8,y,y,pp,cs,1) == 0
       &&   get_bw(j1+x/2-2-0*x/8,j1+x/2,y,y,pp,cs,1) == 0
       &&   get_bw(j1+x  -2-0*x/8,j1+x  ,y,y,pp,cs,1) == 0 )
      {
	i++; // snip!
	for(y=0;y<(y1-y0+1+4)/8;y++)put(pp,j1+x/2,y0+y,255,128+64); // clear highest bit
	if(vvv&4){ 
	  fprintf(stderr,"\n"); 
	  out_x(box2);
	  fprintf(stderr,"# melted serifs corrected on %d %d",j1+x/2-x0,y);
	}
	for(xb=0,xa=0;xa<(x1-x0+4)/8;xa++){ /* detect vertical gap */
          if( loop(pp,j1+x/2-1-xa,y1,y1-y0,cs,0,UP) > (y1-y0+1)/2
           && loop(pp,j1+x/2-1,(y0+y1)/2,xa+1,cs,0,LE) >=xa ){ xb=-xa; break; }
          if( loop(pp,j1+x/2-1-xa,y1,y1-y0,cs,0,UP) > (y1-y0+1)/2 
           && loop(pp,j1+x/2-1,(y0+y1)/2,xa+1,cs,0,RI) >=xa ){ xb= xa; break; }
        }
	if( get_bw(j1+x/2   ,j1+x/2   ,y0,(y0+y1)/2,pp,cs,1) == 0
	 && get_bw(j1+x/2+xb,j1+x/2+xb,(y0+y1)/2,y1,pp,cs,1) == 0 )
	{ /* divide */
	  box3=malloc_box(box2);
	  box3->x0=j1+x/2+1;
	  box2->x1=j1+x/2-1; x1=box2->x1;
	  gocrlist_ins(&boxlist,box2,box3); env.numC++; ii++;
	  if(vvv&4) fprintf(stderr," => splitted");
	}
      }
      j1+=x;
    }
    // same on lower serifs -- change this later to better function
    for(j1=x0;j1<x1;){
      j1+=loop(pp,j1,y1  ,x1-x0,cs,0,RI);
      x  =loop(pp,j1,y1  ,x1-x0,cs,1,RI);	       if(j1+x>x1+1) break;
      y  =loop(pp,j1,y1-1,x1-x0,cs,1,RI); if(y>x) x=y; if(j1+x>x1+1) break;
      y  =y1-(y1-y0+1+4)/8; 
      if(x>5)
      if(num_cross(j1	       ,j1+x  ,y,y,pp,cs)   >= 2 )
      if(num_cross(j1	       ,j1+x/2,y,y,pp,cs)   == 1 )
      if(   get_bw(j1	       ,j1+0*x/8,y,y,pp,cs,1) == 0 )
      if(   get_bw(j1+x/2-2-0*x/8,j1+x/2,y,y,pp,cs,1) == 0 )
      if(   get_bw(j1+x  -2-0*x/8,j1+x  ,y,y,pp,cs,1) == 0 )
      {
	i++; // snip!
	for(y=0;y<(y1-y0+1+4)/8;y++)
	  put(pp,j1+x/2-1,y1-y,255,128+64); // clear highest bit
	if(vvv&4){ 
	  fprintf(stderr,"\n");
	  out_x(box2);
	  fprintf(stderr,"# melted serifs corrected on %d %d",j1+x/2-x0,y);
	}
	for(xb=0,xa=0;xa<(x1-x0+4)/8;xa++){ /* detect vertical gap */
          if( loop(pp,j1+x/2-1-xa,y0,y1-y0,cs,0,DO) > (y1-y0+1)/2
           && loop(pp,j1+x/2-1,(y0+y1)/2,xa+1,cs,0,LE) >=xa ){ xb=-xa; break; }
          if( loop(pp,j1+x/2-1-xa,y0,y1-y0,cs,0,DO) > (y1-y0+1)/2 
           && loop(pp,j1+x/2-1,(y0+y1)/2,xa+1,cs,0,RI) >=xa ){ xb= xa; break; }
        }
	if( get_bw(j1+x/2-1   ,j1+x/2-1   ,(y0+y1)/2,y1,pp,cs,1) == 0
	 && get_bw(j1+x/2-1+xb,j1+x/2-1+xb,y0,(y0+y1)/2,pp,cs,1) == 0 )
	{ /* divide */
	  box3=malloc_box(box2);
	  box3->x0=j1+x/2;
	  box2->x1=j1+x/2-2; x1=box2->x1;
	  gocrlist_ins(&boxlist,box2,box3); env.numC++; ii++;
	  if(vvv&4) fprintf(stderr," => splitted");
	}
      }
      j1+=x;
    }
  } end_for_each(&boxlist);
  if(vvv)fprintf(stderr," %3d cluster corrected, %d new boxes\n",i,ii);
  return 0;
}

int remove_rest_of_dust() {
  /* ---- remove ...  ---------------------------------
   */
  int i = 0, vvv = env.vvv, x0, x1, y0, y1;
  struct box *box2, *box4;

  if (vvv) {
    fprintf(stderr, "# detect dust2, ... ");
  }

  /* remove fragments from border */
  for_each_data(&boxlist) {
    box2 = (struct box *)gocrlist_get_current(&boxlist);
#ifdef USE_UNICODE
    if (box2->c == UNKNOWN) {
#else
    if (box2->c == '_') {
#endif
      x0 = box2->x0; x1 = box2->x1;
      y0 = box2->y0; y1 = box2->y1;	// box
      // box in char ???
      if (  env.numC * (y1 - y0 + 1) < 2 * env.sumY
        && (   ( y1 > box2->p->y-5 && y0 > box2->m4 )
            || ( y0 <            5 && y1 < box2->m1 ) )
         )	/* remove this */
      {
	 env.numC--;
	 i++;
	 gocrlist_del(&boxlist, box2);
	 free(box2);
      }
    }
  } end_for_each(&boxlist);
  for_each_data(&boxlist) {
    box2 = (struct box *)gocrlist_get_current(&boxlist);
    if (box2->c != '@') {
      x0 = box2->x0; x1 = box2->x1;
      y0 = box2->y0; y1 = box2->y1;	// box
      // box in char ???
      if ( env.numC * (x1 - x0 + 1) < 4 * env.sumX
        || env.numC * (y1 - y0 + 1) < 4 * env.sumY )	/* remove this */
	for_each_data(&boxlist) {
	  box4 = (struct box *)gocrlist_get_current(&boxlist);
	  if (box4 != box2) {
	    if ( box4->x0 >= x0 && box4->x1 <= x1
	      && box4->y0 >= y0 && box4->y1 <= y1 ) {	/* remove this */
	      env.numC--;
	      i++;
	      gocrlist_del(&boxlist, box4);
	      free(box4);
	    }
	  }
	} end_for_each(&boxlist);
    }
  } end_for_each(&boxlist);
  if (vvv)
    fprintf(stderr, " %3d boxes deleted, remaining boxes %d\n", i, env.numC);

  return 0;
}
