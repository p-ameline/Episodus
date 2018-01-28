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

  sometimes I have written comments in german language, sorry for that

 - look for ??? for preliminary code
 - space: avX=22 11-13 (empirical estimated)
          avX=16  5-7
          avX= 7  5-6
         
 ToDo: - add filter (r/s mismatch) g300c1
       - better get_line2 function (problems on high resolution)
       - write parallelizable code!
       - learnmode (optimize filter)
       - use ispell for final control or if unsure
       - better line scanning (if not even)
       - step 5: same chars differ? => expert mode
       - chars dx>dy and above 50% hor-crossing > 4 is char-group ?
       - detect color of chars and underground
       - rotation of chars in the pixmap to avoid y-corrections
       - better word space calculation (look at the examples)
          (distance: left-left, middle-middle, left-right, thickness of e *0.75)

   GLOBAL DATA (mostly structures)
   - pix   : image - one byte per pixel  bits0-2=working
   - lines : rows of the text (points to pix)
   - box   : list of bounding box for character
   - obj   : objects (lines, splines, etc. building a character)
 */


#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#ifdef HAVE_WCHAR_H
#include <wchar.h>
#endif

#include "amiga.h"

#include "gocr_list.h"
#include "pgm2asc.h"
#include "pcx.h"        /* needed for writebmp (removed later) */
/* ocr1 is the test-engine - remember: this is development version */
#include "ocr1.h"
/* first engine */
#include "ocr0.h"
#include "otsu.h"

#include "gocr.h"

#define MaxBox (100*200)	// largest possible letter (buffersize)
#define MAX(a,b)			((a) >= (b) ? (a) : (b))

pix ppo;		/* pixmap for debugging output */
gocrList boxlist;
gocrList linelist;
struct environment env;
/* here the positions (frames) of lines are stored for further use */
struct tlines lines;

/* sometimes recognition function is called again and again, if result was 0
   n_run tells the pixel function to return alternative results */
/* static */
int n_run=0;  // num of run, if run_2 critical pattern get other results
              // used for 2nd try, pixel uses slower filter function etc.

// ------------------------ feature extraction -----------------
// -------------------------------------------------------------
// detect maximas in of line overlaps (return in %) and line coordinates
// this is for future use
#define HOR 1    // horizontal
#define VER 2    // vertical
#define RIS 3    // rising=steigend
#define FAL 4    // falling=fallend
struct tline line;

/* exchange two variables */
static void swap(int *a, int *b) { 
  int c = *a;
  *a = *b;
  *b = c;
}

// calculate the overlapping of the line (0-1) with black points 
// by recursive bisection 
// line: y=dy/dx*x+b, implicit form: d=F(x,y)=dy*x-dx*y+b*dx=0 
// incremental y(i+1)=m*(x(i)+1)+b, F(x+1,y+1)=f(F(x,y))
// ret & 1 => inverse pixel!
// d=2*F(x,y) integer numbers
int get_line(int x0, int y0, int x1, int y1, pix *p, int cs, int ret){
   int dx,dy,incrE,incrNE,d,x,y,r0,r1,ty,tx,
       *px,*py,*pdx,*pdy,*ptx,*pty,*px1;
   dx=abs(x1-x0); tx=((x1>x0)?1:-1);    // tx=x-spiegelung (new)
   dy=abs(y1-y0); ty=((y1>y0)?1:-1);	// ty=y-spiegelung (new)
   // rotate coordinate system if dy>dx
/*bbg: can be faster if instead of pointers we use the variables and swaps? */
   if(dx>dy){ pdx=&dx;pdy=&dy;px=&x;py=&y;ptx=&tx;pty=&ty;px1=&x1; }
   else     { pdx=&dy;pdy=&dx;px=&y;py=&x;ptx=&ty;pty=&tx;px1=&y1; }
   if( *ptx<0 ){ swap(&x0,&x1);swap(&y0,&y1);tx=-tx;ty=-ty; }
   d=((*pdy)<<1)-(*pdx); incrE=(*pdy)<<1; incrNE=((*pdy)-(*pdx))<<1;
   x=x0; y=y0; r0=r1=0; /* dd=tolerance (store max drift) */
   while( (*px)<=(*px1) ){
     if( ((pixel(p,x,y)<cs)?1:0)^(ret&1) ) r0++; else r1++;
     (*px)++; if( d<=0 ){ d+=incrE; } else { d+=incrNE; (*py)+=(*pty); }
   }
   return (r0*(ret&~1))/(r0+r1); // ret==100 => percentage %
}

// this function should detect whether a direct connection between points
//   exists or not, not finally implemented
// ret & 1 => inverse pixel!
// d=2*F(x,y) integer numbers, ideal line: ,I pixel: I@
//   ..@  @@@  .@.  ...,@2@. +1..+3 floodfill around line ???
//   ..@  .@@  .@.  ...,.@@@ +2..+4 <= that's not implemented yet
//   ..@  ..@  .@.  ...,.@@@ +2..+4
//   @.@  @..  .@.  ...,@@@. +1..+3
//   @.@  @@.  .@.  ...I@@@.  0..+3
//   @@@  @@@  .@.  ..@1@@..  0..+2
//   90%   0%  100%   90%     r1-r2
// I am not satisfied with it
int get_line2(int x0, int y0, int x1, int y1, pix *p, int cs, int ret){
   int dx,dy,incrE,incrNE,d,x,y,r0,r1,ty,tx,q,ddy,rx,ry,
       *px,*py,*pdx,*pdy,*ptx,*pty,*px1;
   dx=abs(x1-x0); tx=((x1>x0)?1:-1);    // tx=x-spiegelung (new)  
   dy=abs(y1-y0); ty=((y1>y0)?1:-1);	// ty=y-spiegelung (new)
   // rotate coordinate system if dy>dx
   if(dx>dy){ pdx=&dx;pdy=&dy;px=&x;py=&y;ptx=&tx;pty=&ty;px1=&x1;rx=1;ry=0; }
   else     { pdx=&dy;pdy=&dx;px=&y;py=&x;ptx=&ty;pty=&tx;px1=&y1;rx=0;ry=1; }
   if( *ptx<0 ){ swap(&x0,&x1);swap(&y0,&y1);tx=-tx;ty=-ty; }
   d=((*pdy)<<1)-(*pdx); incrE=(*pdy)<<1; incrNE=((*pdy)-(*pdx))<<1;  
   x=x0; y=y0; r0=r1=0; ddy=3; // tolerance = bit 1 + bit 0 = left+right
   // int t=(*pdx)/16,tl,tr;  // tolerance, left-,right delimiter
   while( (*px)<=(*px1) ){  // not finaly implemented
     q=((pixel(p,x,y)<cs)?1:0)^(ret&1);
     if ( !q ){		// tolerance one pixel perpenticular to the line
                        // what about 2 or more pixels tolerance???
       ddy&=(~1)|(((pixel(p,x+ry,y+rx)<cs)?1:0)^(ret&1));
       ddy&=(~2)|(((pixel(p,x-ry,y-rx)<cs)?1:0)^(ret&1))*2;
     } else ddy=3;
     if( ddy ) r0++; else r1++;
     (*px)++; if( d<=0 ){ d+=incrE; } else { d+=incrNE; (*py)+=(*pty); }
   }
   return (r0*(ret&~1))/(r0+r1); // ret==100 => percentage %
}

/* Look for dots in the rectangular region x0 <= x <= x1 and y0 <= y
 <= y1 in pixmap p.  The two low order bits in mask indicate the color
 of dots to look for: If mask==1 then look for black dots (where a
 pixel value less than cs is considered black).  If mask==2 then look
 for white dots.  If mask==3 then look for both black and white dots.
 If the dots are found, the corresponding bits are set in the returned
 value.  Heavily used by the engine ocr0*.cc */
char get_bw(int x0, int x1, int y0, int y1, pix * p, int cs, int mask) {
  char rc = 0;			// later with error < 2% (1 dot)
  int x, y;

  if (x0 < 0)        x0 = 0;
  if (x1 >= p->x)    x1 = p->x - 1;
  if (y0 < 0)        y0 = 0;
  if (y1 >= p->y)    y1 = p->y - 1;

  for ( y = y0; y <= y1; y++)
    for ( x = x0; x <= x1; x++) {
      rc |= ((pixel(p, x, y) < cs) ? 1 : 2);	// break if rc==3
      if ((rc & mask) == mask)
	return mask;		// break loop
    }
  return (rc & mask);
}

/* more general Mar2000 (x0,x1,y0,y1 instead of x0,y0,x1,y1! (history))
 * look for black crossings throw a line from x0,y0 to x1,y1 and count them
 * follow line and count crossings ([white]-black-transitions)
 *  ex: horizontal num_cross of 'm' would return 3 */
int num_cross(int x0, int x1, int y0, int y1, pix *p, int cs) {
  int rc = 0, col = 0, k, x, y, i, d;	// rc=crossings  col=0=white
  int dx = x1 - x0, dy = y1 - y0;

  d = MAX(abs(dx), abs(dy));
  for (i = 0, x = x0, y = y0; i <= d; i++) {
    if (d) {
      x = x0 + i * dx / d;
      y = y0 + i * dy / d;
    }
    k = ((pixel(p, x, y) < cs) ? 1 : 0);	// 0=white 1=black
    if (col == 0 && k == 1)
      rc++;
    col = k;

    /* Small chars : we could miss a crossing if we dont explore both pixels
     * when dx/d==1/2 or dy/d==1/2 */
    if (d == 2*dx)
    {
        k = ((pixel(p, x+1, y) < cs) ? 1 : 0);	// 0=white 1=black
        if (col == 0 && k == 1)
        {
            rc++;
            col = k;
        }
    }
    if (d == 2*dy)
    {
        k = ((pixel(p, x, y+1) < cs) ? 1 : 0);	// 0=white 1=black
        if (col == 0 && k == 1)
        {
            rc++;
            col = k;
        }
    }
  }
  return rc;
}

/* look for edges: follow a line from x0,y0 to x1,y1, record the
 * location of each transition, and return their number.
 * ex: horizontal num_cross of 'm' would return 6 */
int follow_path(int x0, int x1, int y0, int y1, pix *p, int cs, path_t *path) {
  int rc = 0, prev, x, y, i, d, color; // rc=crossings  col=0=white
  int dx = x1 - x0, dy = y1 - y0;

  d = MAX(abs(dx), abs(dy));
  prev = pixel(p, x0, y0) < cs;	// 0=white 1=black
  path->start = prev;
  for (i = 1, x = x0, y = y0; i <= d; i++) {
    if (d) {
      x = x0 + i * dx / d;
      y = y0 + i * dy / d;
    }
    color = pixel(p, x, y) < cs; // 0=white 1=black
    if (color != prev){
      if (rc>=path->max){
	int n=path->max*2+10;
	path->x = xrealloc(path->x, n*sizeof(int));
	path->y = xrealloc(path->y, n*sizeof(int));
	path->max = n;
      }
      path->x[rc]=x;
      path->y[rc]=y;
      rc++;
    }      
    prev = color;
  }
  path->num=rc;
  return rc;
}

void *xrealloc(void *ptr, size_t size){
  void *p;
  p = realloc(ptr, size);
  if (!p){
    fprintf(stderr, "insufficient memory");
    exit(1);
  }
  return p;
}

/*
 *  -------------------------------------------------------------
 *  mark edge-points
 *   - first move forward until b/w-edge
 *   - more than 2 pixel?
 *   - loop around
 *     - if forward    pixel : go up, rotate right
 *     - if forward no pixel : rotate left
 *   - stop if found first 2 pixel in same order
 *  go_along_the_right_wall strategy is very similar and used otherwhere
 *  --------------------------------------------------------------
 *  turmite game: inp: start-x,y, regel r_black=UP,r_white=RIght until border
 * 	       out: last-position
 * 
 *  could be used to extract more features:
 *   by counting stepps, dead-end streets ,xmax,ymax,ro-,ru-,lo-,lu-edges
 * 
 *   use this little animal to find features, I first was happy about it
 *    but now I prefer the loop() function 
 */

void turmite(pix *p, int *x, int *y,
	     int x0, int x1, int y0, int y1, int cs, int rw, int rb) {
  int r;
  if (outbounds(p, x0, y0))	// out of pixmap
    return;
  while (*x >= x0 && *y >= y0 && *x <= x1 && *y <= y1) {
    r = ((pixel(p, *x, *y) < cs) ? rb : rw);	// select rule 
    switch (r) {
      case UP: (*y)--; break;
      case DO: (*y)++; break;
      case RI: (*x)++; break;
      case LE: (*x)--; break;
      case ST:       break;
      default:       assert(0);
    }
    if( r==ST ) break;	/* leave the while-loop */
  }
}

/* search a way from p0 to p1 without crossing pixels of type t
 *  only two directions, useful to test if there is a gap 's'
 * labyrinth algorithm - do you know a faster way? */
int joined(pix *p, int x0, int y0, int x1, int y1, int cs){
  int t,r,x,y,dx,dy,xa,ya,xb,yb;
  x=x0;y=y0;dx=1;dy=0;
  if(x1>x0){xa=x0;xb=x1;} else {xb=x0;xa=x1;}
  if(y1>y0){ya=y0;yb=y1;} else {yb=y0;ya=y1;}
  t=((pixel(p,x,y)<cs)?1:0);
  for(;;){
    if( t==((pixel(p,x+dy,y-dx)<cs)?1:0)	// right free?
     && x+dy>=xa && x+dy<=xb && y-dx>=ya && y-dx<=yb) // wall
         { r=dy;dy=-dx;dx=r;x+=dx;y+=dy; } // rotate right and step forward
    else { r=dx;dx=-dy;dy=r; } // rotate left
    // fprintf(stderr," path xy %d-%d %d-%d %d %d  %d %d\n",xa,xb,ya,yb,x,y,dx,dy);
    if( x==x1 && y==y1 ) return 1;
    if( x==x0 && y==y0 && dx==1) return 0;
  }
  // return 0; // endless loop ?
}

/* move from x,y to direction r until pixel of color col is found
 *   or maximum of l steps
 * return the number of steps done */
int loop(pix *p,int x,int y,int l,int cs,int col, DIRECTION r){ 
  int i=0;
  if(x>=0 && y>=0 && x<p->x && y<p->y){
    switch (r) {
    case UP:
      for( ;i<l && y>=0;i++,y--)
	if( (pixel(p,x,y)<cs)^col )
	  break;
    case DO:
      for( ;i<l && y<p->y;i++,y++)
	if( (pixel(p,x,y)<cs)^col )
	  break;
    case LE:
      for( ;i<l && x>=0;i++,x--)
	if( (pixel(p,x,y)<cs)^col )
	  break;
    case RI:
      for( ;i<l && x<p->x;i++,x++)
	if( (pixel(p,x,y)<cs)^col )
	  break;
    default:;
    }
  }
  return i;
}

/* Given a point, frames a rectangle containing all points of the same
 * color surrounding it, and mark these points.
 *
 * looking for better algo: go horizontally and look for upper/lower non_marked_pixel/nopixel
 * use lowest three bits for mark
 *   - recursive version removed! AmigaOS has no Stack-OVL-Event
 * run around the chape using laby-robot
 * bad changes can lead to endless loop!
 *  - this is not absolutely sure but mostly works well
 */
int frame_nn(pix *p, int  x,  int  y,
             int *x0, int *x1, int *y0, int *y1,	// enlarge frame
             int cs, int r,int diag){

  int i, j, d, dx, ox, oy, od, nx, ny, rc = 0, rot = 0, x2 = x, y2 = y, ln;

  static const int d0[8][2] = { {0, -1} /* up   */, {-1, -1},
				{-1, 0} /* left */, {-1, 1},
				{0, 1} /* down  */, {1, 1},
				{1, 0} /* right */, {1, -1}};

    /* check bounds */
    if (outbounds(p, x, y))
        return 0;
    /* check if already marked */
    if (marked(p,x,y))
        return 0;

    i = ((pixel(p, x, y) < cs) ? 0 : 1);
    rc = 0;

    //g_debug(fprintf(stderr," start frame:");)
    /* repeat the algorithm from other border ???
     * if first loop was around inner border (use labyrinth algo)
     * does not work for  @.......@
     *  		          @@@.X.@@@ < start on X => only right loops
     * 		              ..@@@@@..
     *  to avoid this store leftmost position for second start
     *  or change algorithm */
    for (ln = 0; ln < 2 && rot >= 0; ln++)
    {  // repeat if right-loop
        //g_debug(fprintf(stderr," ln=%d diag=%d cs=%d x=%d y=%d - go to border\n",ln,diag,cs,x,y);)

        od=d=(8+4*ln-diag)&7; // start robot looks up, right is a wall
        // go to right (left) border
        if (ln==1) {
            x=x2;	y=y2;
        }
        /* start on leftmost position */
        for (dx = 1 - 2*ln; x + dx < p->x && x + dx >= 0 /* bounds */ &&
      	      	       i == ((pixel(p, x + dx, y) < cs) ? 0 : 1) /* color */;
	      	       x += dx);

        //g_debug(fprintf(stderr," ln=%d diag=%d cs=%d x=%d y=%d\n",ln,diag,cs,x,y);)

        /* robot stores start-position */
        ox = x;	oy = y;
        for (rot = 0; abs(rot) <= 64; ) 	/* for sure max. 8 spirals */
        {
            /* leftmost position */
            if (ln == 0 && x < x2)
            {
	            x2 = x; 	y2 = y;
            }

            //g_debug(fprintf(stderr," x=%3d y=%3d d=%d i=%d p=%3d rc=%d\n",x,y,d,i,pixel(p,x,y),rc);)

            if ( abs(d0[d][1]) ) 	/* mark left (right) pixels */
            {
	            for (j = 0, dx = d0[d][1]; x + j >= 0 && x + j < p->x
	              	    && i == ((pixel(p, x + j, y) < cs) ? 0 : 1); j += dx)
                {
	                if (!marked(p, x + j, y))
	                    rc++;
	                p->p[x + j + y * p->x] |= (r & 7);
	            }
            }
            /* look to the front of robot */
            nx = x + d0[d][0];
            ny = y + d0[d][1];
            /* if right is a wall */
            if ( outbounds(p, nx, ny) || i != ((pixel(p,nx,ny)<cs) ? 0 : 1) )
            {
	            /* rotate left */
                d=(d+2-diag) & 7; rot-=2-diag;
            }
            else 	/* if no wall, go forward and turn right (90 degrees) */
            {
                x=nx; y=ny; d=(d+6) & 7; rot+=2;
	            /* enlarge frame */
                if (x < *x0)
                    *x0 = x;
	            if (x > *x1)
                    *x1 = x;
	            if (y < *y0)
                    *y0 = y;
	            if (y > *y1)
                    *y1 = y;
            }
            if (x==ox && y==oy && d==od)
                break;	// round trip finished
        }
    }
    //g_debug(fprintf(stderr," rot=%d\n",rot);)

    /* Modif NAUTILUS                                       */
    /*  --@-     Starting from X ; then @ is not marked     */
    /*  -X--                                                */
    /* We simply check that X neighbours are checked        */

    for (j = 0; j < 8; j++)
    {
        nx = x + d0[j][0];
        ny = y + d0[j][1];
        if (!(outbounds(p, nx, ny)))
        {
            if ((i == ((pixel(p, nx, ny) < cs) ? 0 : 1)) && !marked(p, nx, ny))
                rc += frame_nn(p, nx, ny, x0, x1, y0, y1, cs, r, diag);
        }
    }

    /* end modif NAUTILUS */

    return rc;
}

/* mark neighbouring pixel of same color, return number
 * better with neighbours of same color (more general) ???
 * parameters: (&~7)-pixmap, start-point, critical_value, mark
 *  recursion is removed */
int mark_nn(pix * p, int x, int y, int cs, int r) {
  /* out of bounds or already marked? */
  if (outbounds(p, x, y) || marked(p, x, y))
    return 0;
  {
    int x0, x1, y0, y1;
    x0 = x1 = x;
    y0 = y1 = y;			// not used
    return frame_nn(p, x, y, &x0, &x1, &y0, &y1, cs, r, n_run & 1);	// using same scheme
  }
}


/* clear lowest 3 (marked) bits (they are used for marking) */
void clr_bits(pix * p, int x0, int x1, int y0, int y1) {
  int x, y;
  for ( y=y0; y <= y1; y++)
    for ( x=x0; x <= x1; x++)
      p->p[x+y*p->x] &= ~7;
}

/* look for white holes surrounded by black points
 * at the moment look for white point with black in all four directions
 * later: count only holes with vol>10% ??? */
int num_hole(int x0, int x1, int y0, int y1, pix * p, int cs) {
  int rc = 0, x, y;		// rc:1=hole
  pix b;			// temporary mini-page
  int dx = x1 - x0 + 1, dy = y1 - y0 + 1;
  unsigned char *buf;	//  2nd copy of picture, for working 

  b.p = buf = malloc( dx * dy * sizeof(unsigned char) );
  if( !buf ){
    fprintf( stderr, "\nFATAL: malloc failed, skip num_hole" );
    return 0;
  }
  if (copybox(p, x0, y0, dx, dy, &b, dx * dy))
    return -1;

  /* --- mark white-points connected with border */
  for (x = 0; x < b.x; x++) {
    if (pixel(&b, x, 0) >= cs)
      mark_nn(&b, x, 0, cs, AT);
    if (pixel(&b, x, b.y - 1) >= cs)
      mark_nn(&b, x, b.y - 1, cs, AT);
  }
  for (y = 0; y < b.y; y++) {
    if (pixel(&b, 0, y) >= cs)
      mark_nn(&b, 0, y, cs, AT);
    if (pixel(&b, b.x - 1, y) >= cs)
      mark_nn(&b, b.x - 1, y, cs, AT);
  }

  //g_debug(out_b(b,0,0,b.x,b.y,cs);)
  // --- look for unmarked white points => hole
  for (x = 0; x < b.x; x++)
    for (y = 0; y < b.y; y++)
      if (!marked(&b, x, y))	// unmarked
	if (pixel(&b, x, y) >= cs)	// hole found
	  if (mark_nn(&b, x, y, cs, AT) > 1 || dx * dy <= 40)
	    rc++;
  return rc;
}

/* count for black nonconnected objects --- used for i,auml,ouml,etc. */
int num_obj(int x0, int x1, int y0, int y1, pix * p, int cs) {
  int x, y, rc = 0;		// rc=num_obj
  unsigned char *buf; // 2nd copy of picture, for working
  pix b;

  b.p = buf = malloc( (x1-x0+1) * (y1-y0+1) * sizeof(unsigned char) );
  if( !buf ){
    fprintf( stderr, "\nFATAL: malloc failed, skip num_hole" );
    return 0;
  }
  if (copybox(p, x0, y0, x1 - x0 + 1, y1 - y0 + 1, &b, MaxBox))
    return -1;
  // --- mark black-points connected with neighbours
  for (x = 0; x < b.x; x++)
    for (y = 0; y < b.y; y++)
      if (pixel(&b, x, y) < cs)
	if (!marked(&b, x, y)) {
	  rc++;
	  mark_nn(&b, x, y, cs, AT);
	}
  return rc;
}

#if 0
// ----------------------------------------------------------------------
// first idea for making recognition based on probability
//  - start with a list of all possible chars
//  - call recognition_of_char(box *) 
//    - remove chars from list which could clearly excluded
//    - reduce probability of chars which have wrong features
//  - font types list could also build
// at the moment it is only an idea, I should put it to the todo list
//  
char *list="0123456789,.\0xe4\0xf6\0xfc"	// "a=228 o=246 u=252
           "abcdefghijklmnopqrstuvwxyz"
           "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
int  wert[100];
int  listlen=0,numrest=0;
// initialize a new character list (for future)
void ini_list(){ int i;
    for(i=0;list[i]!=0 && i<100;i++) wert[i]=0;
    numrest=listlen=i; } 
// exclude??? (for future) oh it was long time ago, I wrote that :/
void exclude(char *filt){ int i,j;
    for(j=0;filt[j]!=0 && j<100;j++)
    for(i=0;list[i]!=0 && i<100;i++)
    if( filt[j]==list[i] ) { if(!wert[i])numrest--; wert[i]++; } }
// get the result after all the work (for future)
char getresult(){ int i;
    if( numrest==1 )
    for(i=0;list[i]!=0 && i<100;i++) if(!wert[i]) return list[i];
    return '_';
 }
#endif

//  look at the environment of the pixel too (contrast etc.)
//   detailed analysis only of diff pixels!
//
// 100% * distance, 0 is best fit
// = similarity of two chars for recognition of garbled (verstuemmelter) chars
//   weight of pixels with only one same neighbour set to 0
//   look at contours too! v0.2.4: B==H
int distance( pix *p1, struct box *box1,
              pix *p2, struct box *box2, int cs){
   int rc=0,x,y,v1,v2,i1,i2,rgood=0,rbad=0,x1,y1,x2,y2,dx,dy,dx1,dy1,dx2,dy2;
   x1=box1->x0;y1=box1->y0;x2=box2->x0;y2=box2->y0;
   dx1=box1->x1-box1->x0+1; dx2=box2->x1-box2->x0+1; dx=((dx1>dx2)?dx1:dx2);
   dy1=box1->y1-box1->y0+1; dy2=box2->y1-box2->y0+1; dy=((dy1>dy2)?dy1:dy2);
   if(abs(dx1-dx2)>1+dx/16 || abs(dy1-dy2)>1+dy/16) return 100;
   // compare relations to baseline and upper line
   if(2*box1->y1>box1->m3+box1->m4 && 2*box2->y1<box2->m3+box2->m4) rbad+=128;
   if(2*box1->y0>box1->m1+box1->m2 && 2*box2->y0<box2->m1+box2->m2) rbad+=128;
   // compare pixels
   for( y=0;y<dy;y++ )
   for( x=0;x<dx;x++ ) {	// try global shift too ???
     v1     =((pixel(p1,x1+x  ,y1+y  )<cs)?1:0); i1=8;	// better gray?
     v2     =((pixel(p2,x2+x  ,y2+y  )<cs)?1:0); i2=8;	// better gray?
     if(v1==v2) { rgood+=16; continue; } // all things are right!
     // what about different pixel???
     // test overlap of surounding pixels ???
     v1=-1;
     for(i1=-1;i1<2;i1++)
     for(i2=-1;i2<2;i2++)if(i1!=0 || i2!=0){
       if( ((pixel(p1,x1+x+i1*(1+dx/32),y1+y+i2*(1+dy/32))<cs)?1:0)
         !=((pixel(p2,x2+x+i1*(1+dx/32),y2+y+i2*(1+dy/32))<cs)?1:0) ) v1++;
     }
     if(v1>0)rbad+=16*v1;
   }
   if(rgood+rbad) rc= 100*rbad/(rgood+rbad); else rc=99;
//   if(rc<10 && vvv){
//     fprintf(stderr," distance rc=%d\n",rc);
//     out_x(box1);out_x(box2);
//   }
   return rc;
}



// ============================= call OCR engine ================== ;)
wchar_t whatletter(struct box *box1, int cs){
   wchar_t bc=UNKNOWN;			// best letter
   wchar_t um=SPACE;			// umlaut? '" => modifier
   pix *p=box1->p;
   int	x,y,dots,xa,ya,x0,x1,y0,y1,dx,dy,i;
   static unsigned char buf[MaxBox];	// 2nd copy of picture, for working
   pix b;

   xa=box1->x; ya=box1->y;
   x0=box1->x0;y0=box1->y0;
   x1=box1->x1;y1=box1->y1;
   // int vol=(y1-y0+1)*(x1-x0+1);	// volume
   // crossed l-m , divided chars
   while( get_bw(x0,x1,y0,y0,p,cs,1)!=1  &&  y0+1<y1) y0++;
   while( get_bw(x0,x1,y1,y1,p,cs,1)!=1  &&  y0+1<y1) y1--;
   dx=x1-x0+1;
   dy=y1-y0+1;	// size

   // better to proof the white frame too!!! ????
   // --- test for german umlaut and points above, not robust enough???
   // if three chars are connected i-dots (ari) sometimes were not detected
   //  - therefore after division a test could be useful
   testumlaut(box1,cs,2,&um);
   dots=box1->dots;
   y0  =box1->y0;	// dots==2 => y0 below double dots
   dy  =y1-y0+1;

   // move upper and lower border (for divided letters)
   while( get_bw(x0,x1,y0,y0,p,cs,1)==0  &&  y0+1<y1) y0++;
   while( get_bw(x0,x1,y1,y1,p,cs,1)==0  &&  y0+1<y1) y1--;
   while( get_bw(x0,x0,y0,y1,p,cs,1)==0  &&  x0+1<x1) x0++;
   while( get_bw(x1,x1,y0,y1,p,cs,1)==0  &&  x0+1<x1) x1--;
   dx=x1-x0+1;
   dy=y1-y0+1;	// size
   box1->x0=x0;box1->y0=y0;	// set reduced frame
   box1->x1=x1;box1->y1=y1;

   // bad startpoint (probably from division)?
   if( xa<x0 || xa>x1 || ya<y0 || ya>y1
     || pixel(p,xa,ya)>=cs /* || 2*ya<y0+y1 */   ){
     // subfunction? also called after division of two glued chars?
     for(y=y1;y>=y0;y--) // low to high (not i-dot)
     for(x=(x0+x1)/2,i=0;x>=x0 && x<=x1;i++,x+=((2*i&2)-1)*i) /* is that ok? */
     if(pixel(p,x,y)<cs){ xa=x;ya=y;y=-1;break; }
     /* should box1->x,y be set? */
   }

   // ----- create char-only-box -------------------------------------
   if(dx*dy>MaxBox) 
     return PICTURE;
   b.p = buf;
   if( copybox(p,x0,y0,dx,dy,&b,MaxBox) ) 
     return bc;
   // clr_bits(b,0,b.x-1,0,b.y-1);
   // ------ use diagonal too (only 2nd run?) 
   n_run++; mark_nn(&b,xa-x0,ya-y0,cs,1); n_run--;
   // this routine is introduced because
   //   frame_nn do not work correctly sometimes (600dpi font0 \it Q)
   //   that should be changed - v0.2.7 Jul00
   for(x=1;x<b.x-1;x++)  // check if frame_nn has failed
   for(y=1;y<b.y-1;y++){
     if( (!marked(&b,x  ,y  ) && pixel(&b,x  ,y  )<cs ) )
     if( ( marked(&b,x-1,y  ) && pixel(&b,x-1,y  )<cs )
      || ( marked(&b,x+1,y  ) && pixel(&b,x+1,y  )<cs )
      || ( marked(&b,x  ,y-1) && pixel(&b,x  ,y-1)<cs )
      || ( marked(&b,x  ,y+1) && pixel(&b,x  ,y+1)<cs ) )
     mark_nn(&b,x,y,cs,1); // failure => repeat
   }

   for(x=0;x<b.x;x++) 
   for(y=0;y<b.y;y++) if(!marked(&b,x,y)) b.p[x+y*b.x] = 255&~7;

   if (bc == UNKNOWN)
     bc=ocr0(box1,&b,cs);
   // look for serifs and divide melted one (nmhk,wv)
   //    ##  #
   //    ##  #
   //  #########
   //      ^
   
   if (bc == UNKNOWN) {
     y=dy-1;
     x=loop(&b,0,y,dx,cs,0,RI);x1=loop(&b,x,y,dx,cs,1,RI);
     if(pixel(&b,x     ,y-1)>cs)
     if(pixel(&b,x+x1-1,y-1)>cs)
     if(pixel(&b,x+x1/2,y-1)>cs)
     if( num_cross(x+1,x+x1-2,y-2,y-2,&b,cs) > 1 )
     if( num_cross(x+1,x+x1/2,y-2,y-2,&b,cs) == 1 ){
       put(&b,x+x1/2,y,0,176);
       put(p,box1->x0+x+x1/2,box1->y0+y,0,176);
       bc=ocr0(box1,&b,cs);
     }
   }
   if (bc == UNKNOWN) {n_run+=1;bc=ocr0(box1,&b,cs);n_run-=1;}
   if (bc == UNKNOWN) {n_run+=2;bc=ocr0(box1,&b,cs);n_run-=2;}
   if (bc == UNKNOWN) {n_run+=3;bc=ocr0(box1,&b,cs);n_run-=3;}

   if( um ){
     bc = compose(bc, um );
   }
   // box1->c=bc; out_x(box1); // test

   return bc;
}

/*
** creates a list of boxes/frames around objects detected 
** on the pixmap p for further work
** returns number of boxes created.
** - by the way: get average X, Y (avX=sumX/numC,..)
*/
int scan_boxes( pix *p ){
  int x,y,x0,x1,y0,y1,dots,cs;
  struct box *box3;

  if (env.vvv)
    fprintf(stderr,"# scanning boxes");

  cs = env.cs;
  env.sumX = env.sumY = env.numC = 0;
  clr_bits(p,0,p->x-1,0,p->y-1);

  for(y=0; y < p->y; y++)     // step 2 gives speedup and should work too
    for(x=0; x < p->x; x++) { // NO - dust of size 1 is not removed !!!
      if( marked(p,x,y)       )  // marked
	continue;
      if( pixel (p,x,y) >= cs )  // no pixel
	continue;
      x0=x;      x1=x;
      y0=y;      y1=y;
      dots=0;	// box

      frame_nn(p,x,y,&x0,&x1,&y0,&y1,cs,AT,1);  // frame and mark nn-dots
      pixel_atp(p,x,y)|=M1;			// mark startpoint
      env.numC++; env.sumX+=x1-x0+1; env.sumY+=y1-y0+1;

      // --- insert in list
      box3 = (struct box *)malloc(sizeof(struct box));
      box3->x0=x0;     box3->x1=x1;
      box3->y0=y0;     box3->y1=y1;
      box3->x=x;       box3->y=y;
      box3->dots=dots; 
      box3->c=(((y1-y0+1)*(x1-x0+1)>=MaxBox)? PICTURE : UNKNOWN);
      box3->ac=UNKNOWN;
      box3->modifier='\0';
      box3->num=env.numC;
      box3->line=0;	// not used here
      box3->m1=0; box3->m2=0; box3->m3=0; box3->m4=0;
      box3->p=p;
      gocrlist_app(&boxlist, box3); 	// append to list
  }
  if(env.numC){ if(env.vvv)fprintf(stderr," %d\n",env.numC); }
  return env.numC;
}

/* compare ints for sorting.  Return -1, 0, or 1 according to
   whether *vr < *vs, vr == *vs, or *vr > *vs */
int 
intcompare (const void *vr, const void *vs)
{
  int *r=(int *)vr;
  int *s=(int *)vs;

  if (*r < *s) return -1;
  if (*r > *s) return 1;
  return 0;
}

 static int monospaced, pitch;
/*
  measure_pitch - detect monospaced font and measure the pitch
*/
void
measure_pitch(){
    int width, maxwidths=0, numwidths=0, n;
    struct box *box2;
    int prev=-1000, center, *widths=0;
    double v;
    /* add NAUTILUS */ int heigth, minwidth, maxheigth = 0; prev = 0;

    if(env.vvv){ fprintf(stderr,"# check for monospaced font ..."); }
    for_each_data(&boxlist)
    {
        box2 = (struct box *)gocrlist_get_current(&boxlist);
         center = box2->x0 + box2->x1; /* this doubles the calculated widths */
        width = center-prev;

        heigth = box2->m3 - box2->y0; // height above base line
        if (heigth > maxheigth)
            maxheigth = heigth;
        /* fonts are expected to be 20 to 60 pixels high, which is about
            15 to 50 pixels wide.  We allow some extra margin. */
        if (12 < width && width < 150)
        {
            if (numwidths >= maxwidths)
            {
	            n=maxwidths*2+10;
	            widths = xrealloc(widths, n*sizeof(int)); /* what if error ??? */
	            maxwidths = n;
            }
            widths[numwidths++] = width;
        }
        prev = center;
    } end_for_each(&boxlist);

    /* add NAUTILUS for small fonts */
    if (maxheigth < 20)
    {
        prev = 0;
        numwidths = 0;
        minwidth = maxheigth * 3/5;
        for_each_data(&boxlist)
        {
            box2 = (struct box *)gocrlist_get_current(&boxlist);
            center = box2->x0 + box2->x1; /* this doubles the calculated widths */
            width = center-prev;
            if (minwidth < width && width < 150)
            {
                if (numwidths >= maxwidths)
                {
	                n=maxwidths*2+10;
	                widths = xrealloc(widths, n*sizeof(int)); /* what if error ??? */
	                maxwidths = n;
                }
                widths[numwidths++] = width;
            }
            prev = center;
        } end_for_each(&boxlist);
    }

    if( !numwidths )
    {
        fprintf(stderr," no spaces found\n");
        return;
    }

    qsort (widths, numwidths, sizeof (int), intcompare);

    v = (widths[numwidths*7/10]-widths[numwidths/5])/(double)widths[numwidths/5];
    /* measurements showed v=.09 for Courier and .44 for Times-Roman */
    /* monospaced = (v < .22); */
    monospaced = (v < .15);  // Modif Nautilus
    pitch = widths[numwidths*2/5]/2; /* compensate for the factor of 2 */
    if(env.vvv){ fprintf(stderr," min=%d max=%d v=%f mono=%d pitch=%d\n",
        widths[0]/2,widths[numwidths-1]/2,v,monospaced,pitch); }
}

/* ---- glue broken chars ( before step1 ??? )  ---------------------------------
    use this carefully, do not destroy previous detection ~fi, broken K=k' g
    glue if boxes are near or diagonally connected 
    other strategy: mark boxes for deleting and delete in extra loop at end
    faster: check only next two following boxes because list is sorted!
*/
int glue_broken_chars( pix *pp ){
  int ii,y,cs,x0,y0,x1,y1;
  struct box *box2,*box4;
  cs=env.cs;
  {
    if(env.vvv){ fprintf(stderr,"# glue broken chars ..."); }
    ii=0;
    for_each_data(&boxlist)
    {
        box2 = (struct box *)gocrlist_get_current(&boxlist);
        x0 = box2->x0;
        x1 = box2->x1;
        y0 = box2->y0;
        y1 = box2->y1;

      // vertical broken (g965T umlauts etc.)
      // not: f,
/*    ~; ???
      if( 3*y0>box2->m2+2*box2->m3
       &&   y1>box2->m3
       &&   x1-x0 < y1-y0 ) continue; // ~komma
*/

        if( box2->m4>0 && y0>box2->m4   )
            continue; /* dust outside ? */
        if( box2->m1>0 && y0<box2->m1-(box2->m4-box2->m3) )
            continue;

        // check near larger boxes
        if( 2*(y1-y0) < box2->m4 - box2->m1 )  // only check fragments
            for_each_data(&boxlist)
            {
	            box4=(struct box *)gocrlist_get_current(&boxlist);
      	        if(box4 && box4!=box2 && box4->c != PICTURE )
      	            if( box4->line>=0 && box4->line==box2->line )
      	            {
                        if( /* umlaut "a "o "u, ij; box2 is the dot, box4 the body */
                            (    4*box4->x1>=3*x0+x1 	/* test if box4 is around box2 */
                                && 2*box4->x0<=x0+x1
                                && ( x1-x0 <= box4->x1-box4->x0+1 ) /* +1 for small chars */
                                &&     y1<=box2->m2
                                && ( y0+2>=box2->m1 || 4*(y1-y0)<box2->m4-box2->m1 )
                            ) || (	/* broken T */
                                3*(box2->x1 - box2->x0) > 2*env.avX
                                && 4*box4->x0>3*box2->x0+box2->x1
                                && 4*box4->x1<box2->x0+3*box2->x1
                            )
                            ||  /* !?; box2 is the dot, box4 the body */
                            (    2*box4->x1>=x0+x1 	/* test if box4 is around box2 */
                                && 2*box4->x0<=2*x1 /* +x0+1 Jan00 */
                                && ( x1-x0 <= box4->x1-box4->x0+2 )
                                &&   2*y0>=box2->m2+box2->m3
                                &&   4*y1>=box2->m2+3*box2->m3
                                &&   4*(y1-y0)<box2->m4-box2->m1
                                &&   8*box4->y1 < box4->m2+7*box4->m3 /* Jan00 */
                            )
                            ||  /* =; */
                            (    2*box4->x1>=x0+x1 	/* test if box4 is around box2 */
                                && 2*box4->x0<=2*x1 /* +x0+1 */
                                && ( x1-x0   <= box4->x1-box4->x0+4 )
                                && ( x1-x0+4 >= box4->x1-box4->x0   )
                                && 4*y0>=3*box2->m2+box2->m3
                                &&   y1< box2->m4
                                && 2*y1> box2->m3+box2->m2
                                &&   box4->y1 > box4->m2
                                && 4*box4->y1 < box4->m2+3*box4->m3
                            )
                        )
                        {  // fkt melt(box2,box4)
                            if( box4->x0<x0 )
                                x0=box2->x0=box4->x0;
                            if( box4->x1>x1 )
                                x1=box2->x1=box4->x1;
                            if( box4->y0<y0 )
                                y0=box2->y0=box4->y0;
                            if( box4->y1>y1 )
                                y1=box2->y1=box4->y1;
                            env.numC--;ii++;	// remove
	                        gocrlist_del(&boxlist, box4); /* ret&1: error-massage ??? */
	                        free(box4);
                        }
	                }
            } end_for_each(&boxlist);
        // horizontally broken w' K'
        if(     2*y1  <   (box2->m3+box2->m2) )
            if( 2*(y1-y0) <   (box2->m3+box2->m2) )	// fragment
                for_each_data(&boxlist)
                {
                    box4=(struct box *)gocrlist_get_current(&boxlist);
                    if(box4!=box2 && box4->c != PICTURE )
                    {
                        if( box4->line>=0 && box4->line==box2->line
                            && box4->x1>x0-3 && box4->x1<x0
                            && box4->x0+3*box4->x1<4*x0)
                            if( get_bw(x0  ,x0  ,y1,y1  ,pp,cs,1) == 1)
                                if( get_bw(x0-2,x0-1,y1,y1+2,pp,cs,1) == 1)
                                {  // fkt melt(box2,box4)
                                    put(pp,x0,y1+1,~(128+64),0);
                                    if( box4->x0<x0 )
                                        x0=box2->x0=box4->x0;
                                    if( box4->x1>x1 )
                                        x1=box2->x1=box4->x1;
                                    if( box4->y0<y0 )
                                        y0=box2->y0=box4->y0;
                                    if( box4->y1>y1 )
                                        y1=box2->y1=box4->y1;
                                    env.numC--;ii++;	// remove
                                    gocrlist_del(&boxlist, box4);
                                    free(box4);
                                }
                    }
                } end_for_each(&boxlist);
        // horizontally broken n h	(h=l_)		v0.2.5 Jun00
        if( abs(box2->m2-y0)<=(y1-y0)/8 )
            if( abs(box2->m3-y1)<=(y1-y0)/8 )
                if( num_cross(x0,         x1,(y0+  y1)/2,(y0+  y1)/2,pp,cs) == 1)
                    if( num_cross(x0,         x1,(y0+3*y1)/4,(y0+3*y1)/4,pp,cs) == 1)
                        if(    get_bw((3*x0+x1)/4,(3*x0+x1)/4,(3*y0+y1)/4,y1,pp,cs,1) == 0)
                            if(    get_bw(x0,(3*x0+x1)/4,(3*y0+y1)/4,(y0+3*y1)/4,pp,cs,1) == 0)
                                if(    get_bw(x0,         x0,         y0,(3*y0+y1)/4,pp,cs,1) == 1)
                                    for_each_data(&boxlist)
                                    {
	                                    box4=(struct box *)gocrlist_get_current(&boxlist);
      	                                if(box4!=box2 && box4->c != PICTURE )
	                                    {
                                            if( box4->line>=0 && box4->line==box2->line
                                                && box4->x1>x0-3 && box4->x1-2<x0
                                                && abs(box4->y1-box2->m3)<2)
      	                                    {  // fkt melt(box2,box4)
      	                                        y=loop(pp,x0,y0,y1-y0,cs,0,DO);
                                                if(2*y>y1-y0)
                                                    continue;
                                                put(pp,x0-1,y0+y  ,~(128+64),0);
                                                put(pp,x0-1,y0+y+1,~(128+64),0);
                                                if( box4->x0<x0 )
                                                    x0=box2->x0=box4->x0;
                                                if( box4->x1>x1 )
                                                    x1=box2->x1=box4->x1;
                                                if( box4->y0<y0 )
                                                    y0=box2->y0=box4->y0;
                                                if( box4->y1>y1 )
                                                    y1=box2->y1=box4->y1;
                                                env.numC--;ii++;	// remove
	                                            gocrlist_del(&boxlist, box4);
	                                            free(box4);
                                            }
      	                                }
                                    } end_for_each(&boxlist);
    } end_for_each(&boxlist);
    if(env.vvv)fprintf(stderr," %3d times glued, remaining boxes %d\n",ii,env.numC);
  }
  return 0;
}

/*
** this is a simple way to improve results on noisy images:
** - find similar chars (build cluster of same chars)
** - analyze clusters (could be used for generating unknown font-base)
** - the quality of the result depends mainly on the distance function
*/
  // ---- analyse boxes, compare chars, compress picture ------------
  // ToDo: - error-correction only on large chars!
int find_same_chars( pix *pp){
  int i,k,d,cs,dist,n1,dx; struct box *box2,*box3,*box4,*box5;
  pix p=(*pp);
  cs=env.cs;
  {
    if(env.vvv)fprintf(stderr,"# packing");
    i = gocrlist_total(&boxlist);
    for_each_data(&boxlist) {
      box4 = box2 = (struct box *)gocrlist_get_current(&boxlist);
      dist=1000;	// 100% maximum
      dx = box2->x1 - box2->x0 + 1;

      if(env.vvv)fprintf(stderr,"\r# packing %5d",i);
      if( dx>3 )
      for(box3=(struct box *)gocrlist_next(&boxlist,box2);box3;
	  box3=(struct box *)gocrlist_next(&boxlist,box3)) {
        if(box2->num!=box3->num){
          int d=distance(&p,box2,&p,box3,cs);
          if ( d<dist ) { dist=d; box4=box3; }	// best fit
          if ( d<5 ){   // good limit = 5% ???
            i--;n1=box3->num;		// set all num==box2.num to box2.num
	    for_each_data(&boxlist) {
	      box5=(struct box *)(struct box *)gocrlist_get_current(&boxlist);
	      if(box5!=box2)
              if( box5->num==n1 ) box5->num=box2->num;
	    } end_for_each(&boxlist);
          // out_b2(&p,box2->x0,box2->y0,dx,dy,cs,box5->x0,box5->y0);
          // fprintf(stderr," dist=%d\n",d);
          }
      	}
      }
      // nearest dist to box2 has box4
      //    out_b2(p,box2->x0,box2->y0,dx,dy,cs,box4->x0,box4->y0);
      //    fprintf(stderr," dist=%d\n",dist); 
    } end_for_each(&boxlist);
    k=0;
    if(env.vvv)fprintf(stderr," %d different chars",i);
    for_each_data(&boxlist) {
      struct box *box3,*box4;
      int j,dist;
      box2=(struct box *)gocrlist_get_current(&boxlist);
      for(box3=gocrlist_get_header(&boxlist);box3!=box2 && box3!=NULL;
	  box3=gocrlist_next(&boxlist, box3))
        if(box3->num==box2->num)break;
      if(box3!=box2 && box3!=NULL)continue;
      i++;
      // count number of same chars
      dist=0;box4=box2;
      
      for(box3=box2,j=0;box3;box3=gocrlist_next(&boxlist, box3)) {
	if(box3->num==box2->num){
          j++;
          d=distance(&p,box2,&p,box3,cs);
          if ( d>dist ) { dist=d; box4=box3; }	// worst fit
	}
      }
      if(env.vvv&8){
        out_b2(&p,box2->x0,box2->y0,box2->x1-box2->x0+1,box2->y1-box2->y0+1,cs,
                  box4->x0,box4->y0);
        fprintf(stderr," no %d char %4d %5d times maxdist=%d\n",i,box2->num,j,dist);
      }
      // calculate mean-char (error-correction)
      // ToDo: calculate maxdist in group 
      k+=j;
  //    if(j>1)
  //    out_b(p,box2->x0,box2->y0,box2->x1-box2->x0+1,box2->y1-box2->y0+1,cs);
      if(env.vvv&8)
      fprintf(stderr," no %d char %4d %5d times sum=%d\n",i,box2->num,j,k);   
    } end_for_each(&boxlist);
    if(env.vvv)fprintf(stderr," ok\n");
  }
  return 0; 
}

/*
** call the first engine for all boxes and set box->c=result;
**
*/
int char_recognition( pix *pp, int mo){
  int i,cs,x0,y0,x1,y1;
  struct box *box2, *box3;
  wchar_t cc;
  cs=env.cs;
  // ---- analyse boxes, find chars ---------------------------------
  if(env.vvv)fprintf(stderr,"# step 1: char recognition");
  i=0;
  for_each_data(&boxlist) {
    box2 = (struct box *)gocrlist_get_current(&boxlist);
    x0=box2->x0;x1=box2->x1;
    y0=box2->y0;y1=box2->y1;	// box

    cc=box2->c;  // only makes sense for gray-pictures!
    if(cc==UNKNOWN) 
      cc=whatletter(box2,cs   ); // 90%

    if(mo&2) 
      if(cc==UNKNOWN)
	cc=ocr_db(box2);

    if(env.vvv&8) {
      fprintf(stderr,"# code= %04lx %c\n",(long)cc,(char)((cc<255)?cc:'_')); 
      out_b(pp,x0,y0,x1-x0+1,y1-y0+1,cs);
    }
    
    box2->c=cc; 

    if(cc==UNKNOWN) // copy char
      for_each_data(&boxlist) {
        box3=(struct box *)gocrlist_get_current(&boxlist);
	if(box3->num==box2->num)
	  box3->c=cc;
      } end_for_each(&boxlist);
    if(cc==UNKNOWN)
	i++; 

  } end_for_each(&boxlist);
  if(env.vvv)fprintf(stderr,", %d chars unidentified\n",i);
  return 0;
}


/*
** compare unknown with known chars,
** very similar to the find_similar_char_function but here only to
** improve the result
*/
int compare_unknown_with_known_chars(pix * pp, int mo) {
  int i, cs = env.cs, dist, d;
  struct box *box2, *box3, *box4;
  wchar_t bc;
  i = 0;			// ---- ------------------------------- 
  if (env.vvv)
    fprintf(stderr, "# step 2: try to compare unknown with known chars");
  if (!(mo & 8))
  {
    for_each_data(&boxlist) {
      box2 = (struct box *)gocrlist_get_current(&boxlist);
      if (box2->c == UNKNOWN)
	if (box2->y1 - box2->y0 > 4 && box2->x1 - box2->x0 > 1) { // no dots!
	  box4 = (struct box *)gocrlist_get_header(&boxlist);;
	  dist = 1000;		/* 100% maximum */
	  bc = UNKNOWN;		/* best fit char */
	  for_each_data(&boxlist) {
	    box3 = (struct box *)gocrlist_get_current(&boxlist);
	    if (box3 != box2)
	      if (box3->c != UNKNOWN) {
		d = distance(pp, box2, pp, box3, cs);
		if (d < dist) {
		  dist = d;
		  bc = box3->c;
		  box4 = box3;
		}
	      }
	  } end_for_each(&boxlist);
	  if (dist < 10) {
	    box2->c = bc;
	    i++;
	  }			// limit as option???
	  //  => better max distance('e','e') ???
	  if (dist < 50 && (env.vvv & 7)) {	// only for debugging
	    fprintf(stderr, "\n# L%02d best fit was %04x %c %3d%% %d", box2->line,
		    (int)bc, (char)((bc<255)?bc:'_'), dist, i);
	    if ((env.vvv & 4) && dist < 10)
	      out_x2(box2, box4);
	  }
	}
    } end_for_each(&boxlist);
  }
  if (env.vvv)
    fprintf(stderr, " - found %d\n", i);
  return 0;
}

/*
// ---- divide overlapping chars which !strchr("_,.:;",c);
// completely changed at Mar2000
// division if dots>0 does not work properly! ???
//
// what about glued "be"?
// what about recursive division?
*/
int  try_to_divide_boxes( pix *pp, int mo){
  struct box *box2,*box3,boxa,boxb,boxc; int cs=env.cs;
#ifdef HAVE_WCHAR_H
  wchar_t c1,c2,c3,s1[]={ UNKNOWN, '_', '.', ',', '\'', '!', ';', '?', ':', '-', 
      '=', '(', ')' };	// not accepted chars
#else
  wchar_t c1,c2,c3;char *s1="_.,'!;?:-=()";	// not accepted chars
#endif
  int k2,x0,x1,y0,y1,x,x2;
  int i,ii,j,k,m,m1,m2,m3,i1,i2,i3,dx,dy;
  // pix p=(*pp); // remove!
  if(env.vvv)fprintf(stderr,"# step 3: try to divide unknown chars");
  if(!(mo&16))  // put this to the caller
  for_each_data(&boxlist) {
    box2 = (struct box *)gocrlist_get_current(&boxlist);
    if(box2->c==UNKNOWN && box2->x1-box2->x0>5 && box2->y1-box2->y0>4){
      c1=c2=c3=UNKNOWN; 
      x=x2=0;
      x0=box2->x0; x1=box2->x1;
      y0=box2->y0; y1=box2->y1;
      
      if( 2*y1 < box2->m3+box2->m4    /* baseline char ? */
       && num_cross(x0,x1,y1,y1,pp,cs)==1
       && loop(pp,x0,y1-(y1-y0)/32,x1-x0,cs,0,RI)
         +loop(pp,x1,y1-(y1-y0)/32,x1-x0,cs,0,LE) > (x1-x0+1)/2
        ) continue; /* do not try on bvdo"o etc. */

      // one vertical line can not be two glued chars
      if( num_cross(x0,x1,(  y1+y0)/2,(  y1+y0)/2,pp,cs)>1 )
      if( num_cross(x0,x1,(2*y1+y0)/3,(2*y1+y0)/3,pp,cs)>1   // 34 ?
       || num_cross(x0,x1,(3*y1+y0)/4,(3*y1+y0)/4,pp,cs)>1 )
      {	// doublet = 2 letters
        dx=(x1-x0)/32;
        dy=(y1-y0+1);
        if(env.vvv&2)fprintf(stderr," divide box: %4d %4d %3d %3d",x0,y0,x1-x0+1,y1-y0+1);
        if(env.vvv&4)out_b(pp,x0,y0,x1-x0+1,y1-y0+1,cs);
        m1=m2=m3=0; i1=i2=i3=0; // searching minima m1 m2 m3
        // it would be better if testing is only if most right and left char
        //   is has no horizontal gap (below m2) ex: be
        for(i=0;i<(x1-x0)/2-2;i++)   // rm <=> nn .@ mask? for better sorting
        for(ii=-1;ii<2;ii+=((i)?2:4)){ // left and right from middle
          x=(x1+x0)/2+ii*i;
          for(k=0,j=y0;j<=y1;j++) k+=((pixel(pp,x  ,j)<cs)?1:0);
          if(4*k>3*(y1-y0+1)) continue;	// do not divide across black line
          // do not try division right of r in case of glued ar
          if( ii>0 && num_cross(x+1,x1,(y1+y0)/2,(y1+y0)/2,pp,cs)==0 ) continue; 
          if( ii<0 && num_cross(x0,x-1,(y1+y0)/2,(y1+y0)/2,pp,cs)==0 ) continue;
          m=loop(pp,x  ,y0,y1-y0+1,cs,0,DO)
           +loop(pp,x  ,y1,y1-y0+1,cs,0,UP);
          k=loop(pp,x  ,y0,y1-y0+1,cs,0,DO)
           +loop(pp,x-1,y1,y1-y0+1,cs,0,UP); if(k>m) m=k;
          k=loop(pp,x-2,y1,y1-y0+1,cs,0,UP);
          if(2*k>y1-y0){
           k=(y1-y0)/2;
           k+=loop(pp,x  ,y0  ,y1-y0+1,cs,0,DO)
             +loop(pp,x-1,y1-k,y1-y0+1,cs,0,UP); if(k>m) m=k;
          }
          m*=8;  // pretty good!
          for(k2=0,j=y0;j<=y1;j++){
            k=        ((pixel(pp,x  ,j)<cs)?0:1); m+=4*k; // using gray ???
            if(!k) m+=((pixel(pp,x-1,j)<cs)?0:2);
            if(!k) m+=((pixel(pp,x+1,j)<cs)?0:2);
            if(!k) m+=((pixel(pp,x-2,j)<cs)?0:1);
            if(!k) m+=((pixel(pp,x+2,j)<cs)?0:1);
            if(k!=k2) m-=dy/4; k2=k;  // many b/w changes are bad!
          }
          // replace one of 3 maxima (nearest or lowest
          if( abs(i3-ii*i)<2+dx ){ if(m>m3) { m3=m;i3= ii*i; } } else
          if( abs(i2-ii*i)<2+dx ){ if(m>m2) { m2=m;i2= ii*i; } } else
          if( abs(i1-ii*i)<2+dx ){ if(m>m1) { m1=m;i1= ii*i; } } else
                                 { if(m>m3) { m3=m;i3= ii*i; } }
          // sort it:  m1 > m2 > m3
          if( m3>m2 ){ k=m2;m2=m3;m3=k; k=i2;i2=i3;i3=k; }
          if( m2>m1 ){ k=m1;m1=m2;m2=k; k=i1;i1=i2;i2=k; }
          if( m3>m2 ){ k=m2;m2=m3;m3=k; k=i2;i2=i3;i3=k; }

        }
        x=0;
        i1+=(x1+x0)/2;
        i2+=(x1+x0)/2;
        i3+=(x1+x0)/2;
        if(env.vvv&2)fprintf(stderr," x123= %d %d %d  m123= %d %d %d\n",i1-x0,i2-x0,i3-x0,m1,m2,m3);
        // removing ->dots if dot only above one char !!! ??? not implemented
        if( 2*m1>y1-y0 ) // minimum of white pixels should be found
        {
          boxa=*box2;boxb=*box2,boxc=*box2;	// copy contents
//          boxa.next=&boxb;boxb.pre=&boxa;   // new pointers
          x=i1;
          boxa.x=x0; boxa.y=y0;boxa.x1=x;
          boxb.x=x+1;boxb.y=y0;boxb.x0=x+1;
          c1=whatletter(&boxa,cs); // unknown startpos!
          c2=whatletter(&boxb,cs);
	  // boxa..c changed!!! dots should be modified!!!
#ifdef HAVE_WCHAR_H
	  if( wcschr(s1, c1)  || wcschr(s1, c2) ) x=0;
          if(env.vvv&2)fprintf(stderr," x c12 =%d %lc %lc\n",x-x0,c1,c2);
#else
          if( strchr(s1,(char) c1)  || strchr(s1,(char) c2) ) x=0;
          if(env.vvv&2)fprintf(stderr," x c12 =%d %c %c\n",x-x0,(char)c1,(char)c2);
#endif
        }
        if( 2*m2>y1-y0 )
        if(!x){
          boxa=*box2;boxb=*box2,boxc=*box2;	// copy contents
//          boxa.next=&boxb;boxb.pre=&boxa;   // new pointers
          x=i2;
          boxa.x=x0; boxa.y=y0;boxa.x1=x;
          boxb.x=x+1;boxb.y=y0;boxb.x0=x+1;
          c1=whatletter(&boxa,cs); // unknown startpos!
          c2=whatletter(&boxb,cs);
#ifdef HAVE_WCHAR_H
	  if( wcschr(s1, c1)  || wcschr(s1, c2) ) x=0;
          if(env.vvv&2)fprintf(stderr," x c12 =%d %lc %lc\n",x-x0,c1,c2);
#else
          if( strchr(s1,(char) c1)  || strchr(s1,(char) c2) ) x=0;
          if(env.vvv&2)fprintf(stderr," x c12 =%d %c %c\n",x-x0,(char)c1,(char)c2);
#endif
        }
        if( 2*m3>y1-y0 )
        if(!x){
          boxa=*box2;boxb=*box2,boxc=*box2;	// copy contents
//          boxa.next=&boxb;boxb.pre=&boxa;   // new pointers
          x=i3;
          boxa.x=x0; boxa.y=y0;boxa.x1=x;
          boxb.x=x+1;boxb.y=y0;boxb.x0=x+1;
          c1=whatletter(&boxa,cs); // unknown startpos!
          c2=whatletter(&boxb,cs);
#ifdef HAVE_WCHAR_H
	  if( wcschr(s1, c1)  || wcschr(s1, c2) ) x=0;
          if(env.vvv&2)fprintf(stderr," x c12 =%d %lc %lc\n",x-x0,c1,c2);
#else
          if( strchr(s1,(char) c1)  || strchr(s1,(char) c2) ) x=0;
          if(env.vvv&2)fprintf(stderr," x c12 =%d %c %c\n",x-x0,(char)c1,(char)c2);
#endif
        }
        if( 2*m2>y1-y0 )
        if(!x){
          boxa=*box2;boxb=*box2,boxc=*box2;	// copy contents
//          boxa.next=&boxb;boxb.pre=&boxa;   // new pointers
          x2=i2; x=i1; if(x>x2){ k=x;x=x2;x2=k; }
          boxa.x=x0;  boxa.y=y0;boxa.x1=x;
          boxb.x=x+1; boxb.y=y0;boxb.x0=x+1;boxb.x1=x2;
          boxc.x=x2+1;boxc.y=y0;boxc.x0=x2+1;
          c1=whatletter(&boxa,cs); // unknown startpos!
          c2=whatletter(&boxb,cs);
          c3=whatletter(&boxc,cs);
#ifdef HAVE_WCHAR_H
	  if( wcschr(s1, c1)  || wcschr(s1, c2) || wcschr(s1, c3) ) x=0;
          if(env.vvv&2)fprintf(stderr," x c123=%d %lc %lc %lc\n",x-x0,c1,c2,c3);
#else
          if( strchr(s1,(char) c1)  || strchr(s1,(char) c2) || strchr(s1,(char) c3) ) x=0;
          if(env.vvv&2)fprintf(stderr," x c123=%d %c %c %c\n",x-x0,(char)c1,(char)c2,(char)c3);
#endif
        }
        if( 2*m3>y1-y0 )
        if(!x){
          boxa=*box2;boxb=*box2,boxc=*box2;	// copy contents
//          boxa.next=&boxb;boxb.pre=&boxa;   // new pointers
          x2=i3; x=i1; if(x>x2){ k=x;x=x2;x2=k; }
          boxa.x=x0;  boxa.y=y0;boxa.x1=x;
          boxb.x=x+1; boxb.y=y0;boxb.x0=x+1;boxb.x1=x2;
          boxc.x=x2+1;boxc.y=y0;boxc.x0=x2+1;
          c1=whatletter(&boxa,cs); // unknown startpos!
          c2=whatletter(&boxb,cs);
          c3=whatletter(&boxc,cs);
#ifdef HAVE_WCHAR_H
	  if( wcschr(s1, c1)  || wcschr(s1, c2) || wcschr(s1, c3) ) x=0;
          if(env.vvv&2)fprintf(stderr," x c123=%d %lc %lc %lc\n",x-x0,c1,c2,c3);
#else
          if( strchr(s1,(char) c1)  || strchr(s1,(char) c2) || strchr(s1,(char) c3) ) x=0;
          if(env.vvv&2)fprintf(stderr," x c123=%d %c %c %c\n",x-x0,(char)c1,(char)c2,(char)c3);
#endif
        }
        if( 2*m3>y1-y0 )
        if(!x){
          boxa=*box2;boxb=*box2,boxc=*box2;	// copy contents
//          boxa.next=&boxb;boxb.pre=&boxa;   // new pointers
          x2=i3; x=i2; if(x>x2){ k=x;x=x2;x2=k; }
          boxa.x=x0;  boxa.y=y0;boxa.x1=x;
          boxb.x=x+1; boxb.y=y0;boxb.x0=x+1;boxb.x1=x2;
          boxc.x=x2+1;boxc.y=y0;boxc.x0=x2+1;
          c1=whatletter(&boxa,cs); // unknown startpos!
          c2=whatletter(&boxb,cs);
          c3=whatletter(&boxc,cs);
#ifdef HAVE_WCHAR_H
	  if( wcschr(s1, c1)  || wcschr(s1, c2) || wcschr(s1, c3) ) x=0;
          if(env.vvv&2)fprintf(stderr," x c123=%d %lc %lc %lc\n",x-x0,c1,c2,c3);
#else
          if( strchr(s1,(char) c1)  || strchr(s1,(char) c2) || strchr(s1,(char) c3) ) x=0;
          if(env.vvv&2)fprintf(stderr," x c123=%d %c %c %c\n",x-x0,(char)c1,(char)c2,(char)c3);
#endif
        }
      }
      if(x>x0 && x<x1){			// separate first
        box2->y0=boxb.y0;
        box2->y1=boxb.y1;
        // --- insert ind list
        box3=malloc_box(&boxa);  // *box2=>boxa,boxb 024a4
        box3->x1=x;   box3->c=c1;
        box2->x0=x+1; box2->c=c2;
	if( gocrlist_ins(&boxlist, box2, box3) ){ fprintf(stderr,"ERROR list_ins\n"); };
        env.numC++;
        if(x2>x && x2<x1){
          // --- insert in list
          box3=malloc_box(&boxb);
          box3->x1=x2;   box3->c=c2;
          box2->x0=x2+1; box2->c=c3;
	  gocrlist_ins(&boxlist, box2, box3);
          env.numC++;
        }
        continue;
      }
    }
  } end_for_each(&boxlist);
  if(env.vvv)fprintf(stderr,", numC %d\n",env.numC); 
  return 0;
}

  // ---- proof difficult chars Il1 by context view ----
  // context: separator, number, vowel, nonvowel, upper case ????
  // could be also used to find unknown chars if the environment (nonumbers)
  //   can be found in other places!
/* should be language-settable; Unicode compatible */
int context_correction(pix * pp) {
 // const static char
    // *l_vowel="aeiou", *l_Vowel="AEIOU",chars if the environment (nonumbers)
    // *l_nonvo = "bcdfghjklmnpqrstvwxyz";
  struct box *box4, *box3, *box2, *prev, *next;

  if (env.vvv)
    fprintf(stderr, "# step 4: context correction Il1 0O");

  for_each_data(&boxlist) {
    box2 = (struct box *)gocrlist_get_current(&boxlist);
    if (box2->c > 0xFF) continue; // temporary UNICODE fix 
    prev = (struct box *)gocrlist_get_cur_prev(&boxlist);
    next = (struct box *)gocrlist_get_cur_next(&boxlist);

    /* check for Il1| */
    /* bbg: not very good. Should add some tests to check if is preceded by '.',
     spelling, etc */
    if (strchr("Il1|", box2->c) && next && prev) {
//       if( strchr(" \n",prev->c)      // SPC
//        && strchr(" \n",next->c) ) box2->c='I'; else // bad idea! I have ...
      if (isalpha(next->c) && 
          ( prev->c == '\n' || 
	   ( prev->c == ' ' &&
	    ( box4=(struct box *)gocrlist_prev(&boxlist, prev)) &&
	      box4->c == '.' ) ) )
	box2->c = 'I';
      else if (isupper(next->c))
	box2->c = 'I';
      else if (islower(prev->c))
	box2->c = 'l';
      else if (isdigit(prev->c) || isdigit(next->c))
	box2->c = '1';
    }
    
    /* check for O0*/
    else if (strchr("O0", box2->c) && next && prev) {
      if (isspace(prev->c) && isalpha(next->c)) /* initial letter */
	box2->c = 'O';
      else if (isalpha(prev->c) && isalpha(next->c)) /* word in upper case */
	box2->c = 'O';
      else if (isdigit(prev->c) || isdigit(next->c))
	box2->c = '0';
    }

    /* was a space not found? xXx => x Xx ??? */
    if (isupper(box2->c) && next && prev) {
      if (islower(prev->c) && islower(next->c)
	  && 2 * (box2->x0 - prev->x1) > 3 * (next->x0 - box2->x1)) {
	struct box *box3 = malloc_box((struct box *) NULL);
	box3->x0 = prev->x1 + 2;
	box3->x1 = box2->x0 - 2;
	box3->y0 = box2->y0;
	box3->y1 = box2->y1;
	box3->x = box2->x0 - 1;
	box3->y = box2->y0;
	box3->dots = 0;
	box3->c = ' ';
	box3->num = 0;
	box3->line = prev->line;
	box3->m1 = box3->m2 = box3->m3 = box3->m4 = 0;
	box3->p = pp;
	gocrlist_ins(&boxlist, box2, box3);
      }
    }
    
    /* a space before punctuation? */
    if ( prev )
    if (prev->c == ' ' && strchr(".,;:!?)", box2->c))
      if (prev->x1 - prev->x0 < 2 * env.avX) {	// carefully on tables
	box3 = prev;
	gocrlist_del(&boxlist, box3);
	free(box3);
      }
  } end_for_each(&boxlist);
  if (env.vvv)
    fprintf(stderr, "\n");
  return 0;
}

  /* ---- insert spaces ---- */
int list_insert_spaces( pix *pp, int spc ) {
  int i=0; char cc; struct box *box2,*box3;
  if( env.vvv&1 ) fprintf(stderr,"# insert space between words ...");
  for_each_data(&boxlist) {
    box2 =(struct box *)gocrlist_get_current(&boxlist);
    cc=0;
    if((box3 = (struct box *)gocrlist_prev(&boxlist, box2))){
      if(box2->line!=box3->line ) {
        if(lines.m3[box2->line] > lines.m4[box3->line]) cc='\n'; // NL
        else cc=' ';
      }
      if(box2->x0 - box3->x1 > spc) cc=' ';     // SPC
    }
    if(box2==gocrlist_get_header(&boxlist) &&
	box2->x0-lines.x0[box2->line] > env.avX) cc=' ';
    if(cc){
      struct box *box4=(struct box *)gocrlist_prev(&boxlist, box2);
      box3=(struct box *)malloc(sizeof(struct box));
      if(box4)
	box3->x0=box4->x1+2;
      if(cc=='\n' || !box4)
	box3->x0=lines.x0[box2->line];
      box3->x1=box2->x0-2;
      box3->y0=box2->y0;         box3->y1=box2->y1;
      if(cc=='\n' && box4){
        box3->y0=box4->y1;	// better use lines.y1[box2->pre] ???
        box3->y1=box2->y0;
      }
      box3->x =box2->x0-1;       box3->y=box2->y0;
      box3->dots=0;              box3->c=cc;
      box3->modifier='\0';
      box3->num=0; box3->line=box2->line;
      box3->m1=0;  box3->m2=0;  box3->m3=0;  box3->m4=0;
      box3->p=pp;
      gocrlist_ins(&boxlist,box2,box3);
       i++;
    }
  } end_for_each(&boxlist);
  if( env.vvv&1 ) fprintf(stderr," found %d\n",i);
  return 0;
}


/*
   add infos where the box is positioned to the box
   this is useful for better recognition
*/
int  add_line_info(/* List *boxlist2 */){
  pix *pp=env.p;
  struct box *box2;
  int i,xx,m1,m2,m3,m4;
  if( env.vvv&1 ) fprintf(stderr,"# add line infos to boxes ...");
  for_each_data(&boxlist) {
    box2 =(struct box *)gocrlist_get_current(&boxlist);
    for(i=0;i<lines.num;i++)	// only in detected lines
    {
      // fprintf(stderr," test line %d\n",i);
      xx=(box2->x1+box2->x0)/2;
      m1=lines.m1[i]+lines.dy*xx/pp->x;
      m2=lines.m2[i]+lines.dy*xx/pp->x;
      m3=lines.m3[i]+lines.dy*xx/pp->x;
      m4=lines.m4[i]+lines.dy*xx/pp->x;
#if 0
      if( (box2->y0+box2->y1)/2 >= m1
       && (box2->y0+box2->y1)/2 <= m4 ){ /* found correct line */
#endif
      if( box2->x0 >= lines.x0[i]  &&  box2->x1 <= lines.x1[i] )
      if( box2->m2==0 || abs(box2->y0-box2->m2) > abs(box2->y0-m2) )
      { /* found nearest line */
        box2->m1=m1;
        box2->m2=m2;
        box2->m3=m3;
        box2->m4=m4;
        box2->line=i;
      }
    }
  } end_for_each(&boxlist);
  if( env.vvv&1 ) fprintf(stderr," done\n");
  return 0;
}

/*
   add infos where the box is positioned to the box
   this is useful for better recognition
*/
int  add_line_info_enlarge(int iStepUp, int iStepDown){
  pix *pp=env.p;
  struct box *box2;
  int i,xx,m1,m2,m3,m4;
  if( env.vvv&1 ) fprintf(stderr,"# add line infos to boxes ...");
  for_each_data(&boxlist) {
    box2 =(struct box *)gocrlist_get_current(&boxlist);
    for(i=0;i<lines.num;i++)	// only in detected lines
    {
      // fprintf(stderr," test line %d\n",i);
      xx=(box2->x1+box2->x0)/2;
      m1=lines.m1[i]+lines.dy*xx/pp->x;
      m2=lines.m2[i]+lines.dy*xx/pp->x;
      m3=lines.m3[i]+lines.dy*xx/pp->x;
      m4=lines.m4[i]+lines.dy*xx/pp->x;
#if 0
      if( (box2->y0+box2->y1)/2 >= m1
       && (box2->y0+box2->y1)/2 <= m4 ){ /* found correct line */
#endif
      if( box2->x0 >= lines.x0[i]  &&  box2->x1 <= lines.x1[i] )
      if( abs(box2->y0-box2->m2) >= abs(box2->y0-m2))
      { /* found nearest line */

        // if char is between m1 and m2, we assume the system falsely
        // thought it is a hchar type, and we grow up m1 and m2
        if ((box2->y0 >= box2->m1) && (box2->y0 < box2->m2))
        {
            xx = (4 * (box2->y1 - box2->y0)) / 10;
            box2->m2 = box2->y0;
            if (box2->y0 > xx)
                box2->m1 = box2->y0 - xx;
            else
                box2->m1 = 0;
        }
        box2->line=i;
      }
    }
  } end_for_each(&boxlist);
  if( env.vvv&1 ) fprintf(stderr," done\n");
  return 0;
}

/*
 *  bring the boxes in right order
 *  add_line_info must be executed first!
 */
int sort_box_func (const void *a, const void *b) {
  struct box *boxa, *boxb;

  boxa = (struct box *)a;
  boxb = (struct box *)b;

  if ( ( boxb->line < boxa->line ) || 
       ( boxb->line == boxa->line && boxb->x0 < boxa->x0 ) )
    return 1;
  return -1;
}    

// -------------------------------------------------------------
// ------             use this for entry from other programs
// include pnm.h pgm2asc.h
// -------------------------------------------------------------
// entry point for gocr.c or if it is used as lib
// better name is call_ocr ???
// args after pix *pp should be removed and new functions
//   set_option_mode(int mode), set_option_spacewidth() .... etc.
//   should be used instead, before calling pgm2asc(pix *pp)
//   ! change if you can ! - used by X11 frontend
//
int pgm2asc(
 pix *pp,			// see pnm.h, pixelbuffer and size
 int mo, int cs, int spc,	// mode=0 cs=0 spacewidth=0
 int dust_size, char *lc,	// dust_size=10 listchars="_"
 int vvv,			// verbose=0
 char* pText, int iMaxText,
 int bKillBox
 ){
  Box *b;
  int i;

  if(vvv) fprintf(stderr,"# db_path= %s\n",env.db_path);
  /* Initialize stuff */
  if ( !cs )
    cs = 128+32;
  env.cs=cs; env.avX=5; env.avY=8; env.p=pp; env.vvv=vvv; // default values
  env.mode=mo;
  gocrlist_init( &boxlist );
  gocrlist_init( &linelist );
  lines.dy=0; lines.num=0;

  ppo.p=NULL; ppo.x=ppo.y=0;

  /* ----- count colors ------ create histogram -------
     - this should be used to create a upper and lower limit for cs
     - cs is the optimum gray value between cs_min and cs_max
     - also inverse scans could be detected here later */
  otsu( pp->p,pp->y,pp->x, 2 + (env.vvv & 1) );
  if ( env.vvv&32 ) { // generate 2nd imagebuffer for debugging output
    ppo.p = (unsigned char *)malloc(pp->y * pp->x); 	// buffer
    assert(ppo.p);
    copybox(pp,0,0,pp->x,pp->y,&ppo,pp->x * pp->y);
  }
  
  /* load character data base */
  if ( mo&2 )
    load_db();

  /* this is first step for reorganize the PG
     ---- look for letters, put rectangular frames around letters
     letter = connected points near color F
     should be used by dust removing (faster) and line detection!
     ---- 0..cs = black letters, last change = Mai99 */
  
  scan_boxes( pp );
  if ( !env.numC ){ 
    fprintf( stderr,"# no boxes found - stopped\n" );
    if ( env.vvv&32 ) 
      writebmp( "out20.bmp",ppo,env.vvv ); /* colored should be better */
/***** should free stuff, etc) */
    return(1);
  }

  remove_dust( pp, dust_size ); /* from the &boxlist! */

  detect_pictures( ); /* mark pictures */

  remove_melted_serifs( pp ); /* make some corrections on pixmap */

  detect_rotation_angle( pp );

#if 1 		/* Rotate the whole picture! move boxes */
  if( lines.dy!=0 ){  // move down lowest first, move up highest first
    // in work! ??? (at end set dy=0) think on ppo!
  }
#endif

  detect_text_lines( pp, mo );

  // if(env.vvv&32) writebmp("out20.bmp",ppo,env.vvv); // colored should be better
  // erase box list, temporarily (later sorting and gluing)

  add_line_info(/* &boxlist */);
  gocrlist_sort(&boxlist, sort_box_func);

  remove_pictures( );

  glue_broken_chars( pp );

  remove_rest_of_dust( );

  measure_pitch( );

  if(mo&64) find_same_chars( pp );

  char_recognition( pp, mo);

  // begin Add Nautilus
  // add_line_info_enlarge(2, 0);
  // char_recognition( pp, mo);
  // end Add Nautilus

  // ----------- write out20.pgm -----------
  if(env.vvv&32) write_img("out20.bmp",&ppo,pp,1);

  compare_unknown_with_known_chars( pp, mo);

  try_to_divide_boxes( pp, mo);

  /* --- list output ---- for debugging --- */
  if( env.vvv&6 ) output_list( pp, lc);

    if ( spc==0 ){
        if ( monospaced )
            spc = pitch;
        else
        {
            if (env.avX >= 7)            /*  add by NAUTILUS */
                spc = (env.avX+18) / 4;
            else
            {
                spc = (env.avX+2) / 2;  /*  add by NAUTILUS */
                if (spc < 3)            /*  add by NAUTILUS */
                    spc = 3;            /*  add by NAUTILUS */
            }
        }
    if(env.vvv)fprintf(stderr,"# set space width to %d\n",spc);
  }

  /* ---- insert spaces ---- */
  list_insert_spaces( pp , spc );

  // ---- proof difficult chars Il1 by context view ----
  if(!(mo&32)) context_correction( pp );

  store_boxtree_lines( mo );

  // ---- frame-size-histogram
  // ---- (my own defined) distance between letters
  // ---- write internal picture of textsite
  // ----------- write out30.pgm -----------
  if( env.vvv&32 ) write_img("out30.bmp",&ppo,pp,2);

  if(env.vvv&32) free( ppo.p );

  i = 0;
  for_each_data(&boxlist) {
    if ((b = (Box *)gocrlist_get_current(&boxlist)))
    {
        if (i < iMaxText)
        {
            if      (b->c == UNKNOWN)
                pText[i++] = '_';
            else if (b->c != PICTURE)
                pText[i++] = b->c;
        }
        if (bKillBox)
            free(b);
    }
  } end_for_each(&boxlist);

  if (bKillBox)
    gocrlist_free(&boxlist);
  
  pText[i] = '\0';

  return 0; 	/* what should I return? error-state? num-of-chars? */
}

int
giveBoxData(int iNumBox, int* piBox, int* pNumLine, int* px0, int* px1, int* py0, int* py1,
            int* px, int* py, int* pdots, wchar_t* pc, wchar_t* pac, char* pmod,
            int* pm1, int* pm2, int* pm3, int* pm4, int* pWidth, int* pHeigth)
{
    int i, iX, ydeb;
    int iFound = 0;
    int iydebfound = 0;
    int iDot;
    Box *b;
    int cs = env.cs; // Seuil en dessous duquel un pixel est considéré comme noir

    i = 0;

    for_each_data(&boxlist)
    {
        if ((b = (Box *)gocrlist_get_current(&boxlist)))
        {
            if (i == iNumBox)
            {
                *px0        = b->x0;
                *px1        = b->x1;
                *py0        = b->y0;
                *py1        = b->y1;
                *px         = b->x;
                *py         = b->y;
                *pdots      = b->dots;
                *pc         = b->c;
                *pac        = b->ac;
                *pmod       = b->modifier;
                *piBox      = b->num;
                *pNumLine   = b->line;
                *pm1        = b->m1;
                *pm2        = b->m2;
                *pm3        = b->m3;
                *pm4        = b->m4;

                //
                // Calcul de la hauteur (y0-y1 oublie l'éventuel accent)
                //
                if (b->dots >= 1)
                {
                    for (ydeb = b->m1; (ydeb <= b->y0) && (iydebfound == 0); )
                    {
                        for (iX = b->x0; iX <= b->x1; iX++)
                        {
                            iDot = pixel(b->p, iX, ydeb);
                            if (iDot <= cs)
                                break;
                        }
                        if (iX <= b->x1)
                            iydebfound = 1;
                        else
                            ydeb++;
                    }
                }
                else
                    ydeb = b->y0;

                *pWidth     = b->x1 - b->x0 + 1;
                *pHeigth    = b->y1 - ydeb + 1;

                iFound = 1;

                //
                // Ne pas faire return ici, car si la liste ne passe pas par
                // end_for_each, elle passe en mode énumérations imbriquées
                // (level++) et explose rapidement
                //
                // Dont do return here ; if we dont leave with a end_for_each,
                // the list thinks we are including loops inside loops (level++)
                // and quickly explodes
                //
            }
        }
        i++;
    } end_for_each(&boxlist);

    return iFound;
}

int
giveBoxPixels(int iBox, char* pPixels)
{
    int i, j, k, ydeb;
    int iFound = 0;
    int iydebfound = 0;
    Box *b;
    int cs = env.cs; // Seuil en dessous duquel un pixel est considéré comme noir

    int iNum = 0;

    //
    // Attention, ne pas comparer sur b->num qui vaut 0 pour tous les espaces
    //

    for_each_data(&boxlist)
    {
        if ((b = (Box *)gocrlist_get_current(&boxlist)))
        {
            if (iNum == iBox)
            {
                // Attention, la fonction testumlaut supprime l'accent
                // au lieu de prendre b->y0 comme référence, on prend la
                // première ligne qui contient un point au dessous de b->m1

                if (b->dots >= 1)
                {
                    for (ydeb = b->m1; (ydeb <= b->y0) && (iydebfound == 0);)
                    {
                        for (i = b->x0; (i <= b->x1) && (pixel(b->p,i,ydeb) > cs); i++);
                        if (i <= b->x1)
                            iydebfound = 1;
                        else
                            ydeb++;
                    }
                }
                else
                    ydeb = b->y0;

                for (k = 0, j = ydeb/*b->y0*/; j <= b->y1; j++, k++)
                {
                    for (i = b->x0; i <= b->x1; i++, k++)
                    {
                        if (j == b->m1)
                        {
                            if (pixel(b->p,i,j) <= cs)
                                pPixels[k] = 'A';
                            else
                                pPixels[k] = 'a';
                        }
                        else if (j == b->m2)
                        {
                            if (pixel(b->p,i,j) <= cs)
                                pPixels[k] = 'B';
                            else
                                pPixels[k] = 'b';
                        }
                        else if (j == b->m3)
                        {
                            if (pixel(b->p,i,j) <= cs)
                                pPixels[k] = 'C';
                            else
                                pPixels[k] = 'c';
                        }
                        else if (j == b->m4)
                        {
                            if (pixel(b->p,i,j) <= cs)
                                pPixels[k] = 'D';
                            else
                                pPixels[k] = 'd';
                        }
                        else
                        {
                            if (pixel(b->p,i,j) <= cs)
                                pPixels[k] = '1';
                            else
                                pPixels[k] = '0';
                        }
                    }
                    pPixels[k] = ' ';
                }
                pPixels[k] = '\0';

                iFound = 1;

                //
                // Ne pas faire return ici, car si la liste ne passe pas par
                // end_for_each, elle passe en mode énumérations imbriquées
                // (level++) et explose rapidement
                //
                // Dont do return here ; if we dont leave with a end_for_each,
                // the list thinks we are including loops inside loops (level++)
                // and quickly explodes
                //
            }
        }
        iNum++;
    } end_for_each(&boxlist);

    return iFound;
}

int
deleteBoxData()
{
    Box *b;
    for_each_data(&boxlist)
    {
        if ((b = (Box *)gocrlist_get_current(&boxlist)))
            free(b);
    } end_for_each(&boxlist);
    gocrlist_free(&boxlist);

    return 0;
}
