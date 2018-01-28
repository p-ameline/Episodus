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

extern pix ppo;
extern struct tlines lines;

  // ----- detect lines ---------------
/* suggestion: Fourier transform and set line frequency where the
   amplitude has a maximum.

   option: range for line numbers 1..1000 or similar 
   todo: look for thickest line, and divide if thickness=2*mean_thickness 
   Set these elements of the box structs:

   m1 <-- top of upper case letters
   m2 <-- top of small letters
   m3 <-- baseline
   m4 <-- bottom of hanging letters

  performance can be improved by working with a temporary
  list of boxes of the special text line

  since Jun23,00 more robustness of m3 (test liebfrau1) */

int detect_lines1(pix * p, int x0, int y0, int dx, int dy)
{
  int i, j, j2, y, yy, my, mc, m1, m2, m3, m4;
  struct box *box2;

  i = lines.num;
  // fprintf(stderr," detect_lines1(%d %d %d %d)\n",x0,y0,dx,dy);
  // get the mean value of height
  if (dy < 5)
    return 0;
  my = j = 0;
  for_each_data(&boxlist) {
    box2 = (struct box *)gocrlist_get_current(&boxlist);
    if (box2->c != PICTURE)
      if (box2->y1 - box2->y0 > 4) {
	if (box2->y0 >= y0 && box2->y1 <= y0 + dy)
	  if (box2->x0 >= x0 && box2->x1 <= x0 + dx) {
	    j++;
	    my += box2->y1 - box2->y0 + 1;
	  }
      }
   } end_for_each(&boxlist);

  if (j == 0)
    return 0;
  if (j)
    my /= j;
  // better function for scanning line around a letter ???
  // or define lines around known chars "eaTmM"
  for (j2 = y = y0; y < y0 + dy; y++) {
    // look for max. of upper and lower bound of next line
    m4 = m3 = m2 = m1 = y0 + dy;
    j = 0;
    for_each_data(&boxlist) {
      box2 = (struct box *)gocrlist_get_current(&boxlist);
      if (box2->y0 >= y && box2->y1 < y0 + dy	// lower than y
	  && box2->x0 >= x0 && box2->x1 < x0 + dx	// in box ?
	  && box2->c != PICTURE	// no picture
	  && 3 * (box2->y1 - box2->y0) > 2 * my	// right size ?
	  && (box2->y1 - box2->y0) < 3 * my)
	if (box2->y0 < m1 + my / 2) {
	  yy = lines.dy * box2->x0 / (p->x);
	  if (box2->y0 < m1 + yy) {
	    m1 = m2 = box2->y0 - yy;
	  }
	  if (box2->y1 < m3 + yy) {
	    m3 = m4 = box2->y1 - yy;
	  }
	}
    } end_for_each(&boxlist);
    // fprintf(stderr," step 1 y=%4d m1=%4d m4=%4d my=%2d\n",y,m1,m4,my);
    if (m4 == m1)
      break;
    // same again better estimation
    mc = (3 * m4 + m1) / 4;	// center ?
    m2 = m3 = j = 0; m1 = mc;
    for_each_data(&boxlist) {
      box2 = (struct box *)gocrlist_get_current(&boxlist);
      if (box2->y0 >= y && box2->y1 < y0 + dy	// lower than y
	  && box2->x0 >= x0 && box2->x1 < x0 + dx	// in box ?
	  && box2->c != PICTURE	// no picture
	  && 2 * (box2->y1 - box2->y0) > my	// right size ?
	  && (box2->y1 - box2->y0) < 4 * my) {
	yy = lines.dy * box2->x0 / (p->x);
	if (box2->y0 < mc + yy && box2->y1 > mc + yy) {
	  if (box2->y0 < m1 + yy) {
	    m1 = box2->y0 - yy;
	  }
	  if (box2->y0 > m2 + yy) {
	    m2 = box2->y0 - yy;
	  }
	  m3 += box2->y1 - yy;
	  j++;			// better if only numbers
	  if (box2->y1 > m4 + yy) {
	    m4 = box2->y1 - yy;
	  }
	}
      }
    } end_for_each(&boxlist);
    if (j)
      m3 /= j;
    // fprintf(stderr," step 2 y=%4d m1=%4d m4=%4d m2=%4d m3=%4d\n",y,m1,m4,m2,m3);
    if (m4 == m1) {
      y = m4;
      continue;
    }
    if (4 * (m2 - m1) < m4 - m1)
      m2 = m1 + (m4 - m1) / 4;	// only numbers or UC-chars
    if (4 * (m4 - m3) < m4 - m1)
      m4 = m3 + (m4 - m1) / 4;	// only base chars
    y = m4;
    {				// empty space between lines
      lines.m4[i] = m4;
      lines.m3[i] = m3;
      lines.m2[i] = m2;
      lines.m1[i] = m1;
      lines.x0[i] = x0;
      lines.x1[i] = x0 + dx - 1;	// ???
      if (env.vvv & 16)
	fprintf(stderr, "\n# line= %3d m= %4d %2d %2d %2d",
		i, m1, m2 - m1, m3 - m1, m4 - m1);
      if (i < MAXlines && m4 - m1 > 4)
	i++;
      if (i >= MAXlines) {
	fprintf(stderr, "Warning: lines>MAXlines\n");
	break;
      }
    }
  }
  lines.num = i;
  if (env.vvv)
    fprintf(stderr, " - lines= %d", lines.num);
  return 0;
}

// ----- layout analyzis of dx*dy region at x0,y0 -----
// ----- detect lines via recursive division (new version) ---------------
//   what about text in frames???
//  
int detect_lines2(pix *p,int x0,int y0,int dx,int dy,int r){
    int i,x2,y2,x3,y3,x4,y4,x5,y5,y6,mx,my;
    struct box *box2,*box3;
    // shrink box
    if(dx<=0 || dy<=0) return 0;
    if(r>1000){ return -1;} // something is wrong
    if(env.vvv)fprintf(stderr,"\n r=%2d ",r);

    mx=my=i=0; // mean thickness
    // remove border, shrink size
    x2=x0+dx-1;  // min x
    y2=y0+dy-1;  // min y
    x3=x0;	// max x
    y3=y0;	// max y
    for_each_data(&boxlist) {
      box3 = (struct box *)gocrlist_get_current(&boxlist);
      if(box3->y0>=y0  && box3->y1<y0+dy &&
         box3->x0>=x0  && box3->x1<x0+dx)
      {
    	if( box3->x1 > x3 ) x3=box3->x1;
        if( box3->x0 < x2 ) x2=box3->x0;
        if( box3->y1 > y3 ) y3=box3->y1;
        if( box3->y0 < y2 ) y2=box3->y0;
        if(box3->c!=PICTURE)
        if( box3->y1 - box3->y0 > 4 )
        {
          i++;
      	  mx+=box3->x1-box3->x0+1;
          my+=box3->y1-box3->y0+1;
        }
      }
    } end_for_each(&boxlist);
    x0=x2; dx=x3-x2+1;
    y0=y2; dy=y3-y2+1;

    if(i==0 || dx<=0 || dy<=0) return 0;
    mx/=i;my/=i;
    // better look for widest h/v-gap
    if(r<8){ // max. depth

      // detect widest horizontal gap
      y2=y3=y4=y5=y6=0;
      x2=x3=x4=x5=y5=0;// min. 3 lines
      // position and thickness of gap, y6=num_gaps, nbox^2 ops
      for_each_data(&boxlist) { // not very efficient, sorry
	box2 = (struct box *)gocrlist_get_current(&boxlist);
//      if( box2->c!=PICTURE )
        if( box2->y0>=y0  && box2->y1<y0+dy
         && box2->x0>=x0  && box2->x1<x0+dx
         && box2->y1-box2->y0>my/2 ){ // no pictures & dust???

      	  y4=y0+dy-1;  // nearest vert. box
          x4=x0+dx-1;
          // look for nearest nb of every box
	  for_each_data(&boxlist) {
	    box3 = (struct box *)gocrlist_get_current(&boxlist);
	    if(box3!=box2)
            if(box3->y0>=y0  && box3->y1<y0+dy)
            if(box3->x0>=x0  && box3->x1<x0+dx)
//     	    if(box3->c!=PICTURE)
       	    if(box3->y1-box3->y0>my/2 ){
              if( box3->y1 > box2->y1  &&  box3->y0 < y4 ) y4=box3->y0-1;
              if( box3->x1 > box2->x1  &&  box3->x0 < x4 ) x4=box3->x0-1;
            }
      	  } end_for_each(&boxlist);
          // largest gap:          width           position
          if( y4-box2->y1 > y3 ) { y3=y4-box2->y1; y2=(y4+box2->y1)/2; }
          if( x4-box2->x1 > x3 ) { x3=x4-box2->x1; x2=(x4+box2->x1)/2; }
	}
      } end_for_each(&boxlist); 
      // fprintf(stderr,"\n widest y-gap= %4d %4d",y2,y3);
      // fprintf(stderr,"\n widest x-gap= %4d %4d",x2,x3);

      i=0; // i=1 at x, i=2 at y
      // this is the critical point
      // is this a good decision or not???
      if(x3>0 || y3>0){
        if(x3>mx && x3>2*y3 && (dy>5*x3 || (x3>10*y3 && y3>0))) i=1; else
        if(dx>5*y3 && y3>my) i=2;
      }

      // compare with largest box???
      for_each_data(&boxlist) { // not very efficient, sorry
	box2 = (struct box *)gocrlist_get_current(&boxlist);
        if( box2->c == PICTURE )
        if( box2->y0>=y0  && box2->y1<y0+dy
         && box2->x0>=x0  && box2->x1<x0+dx )
        { // hline ???
          // largest gap:                  width position
          if( box2->x1-box2->x0+4 > dx && box2->y1+4<y0+dy ) { y3=1; y2=box2->y1+1; i=2; break; }
          if( box2->x1-box2->x0+4 > dx && box2->y0-4>y0    ) { y3=1; y2=box2->y0-1; i=2; break; }
          if( box2->y1-box2->y0+4 > dy && box2->x1+4<x0+dx ) { x3=1; x2=box2->x1+1; i=1; break; }
          if( box2->y1-box2->y0+4 > dy && box2->x0-4>x0    ) { x3=1; x2=box2->x0-1; i=1; break; }
	}
      } end_for_each(&boxlist); 
      if(env.vvv)fprintf(stderr," i=%d",i);

      if(env.vvv && i) fprintf(stderr," divide at %s x=%4d y=%4d dx=%4d dy=%4d",
        ((i)?( (i==1)?"x":"y" ):"?"),x2,y2,x3,y3);
      // divide horizontally if v-gap is thicker than h-gap
      // and length is larger 5*width
      if(i==1){        detect_lines2(p,x0,y0,x2-x0+1,dy,r+1);
                return detect_lines2(p,x2,y0,x0+dx-x2+1,dy,r+1); }
      // divide vertically
      if(i==2){        detect_lines2(p,x0,y0,dx,y2-y0+1,r+1);
                return detect_lines2(p,x0,y2,dx,y0+dy-y2+1,r+1);  
      }
    }


    if(env.vvv) if(dx<5 || dy<7)fprintf(stderr," empty box");
    if(dx<5 || dy<7) return 0; // do not care about dust
    if(env.vvv)fprintf(stderr, " box detected at %4d %4d %4d %4d",x0,y0,dx,dy);
    if(ppo.p){
        for(i=0;i<dx;i++)put(&ppo,x0+i   ,y0     ,255,16);
        for(i=0;i<dx;i++)put(&ppo,x0+i   ,y0+dy-1,255,16);
        for(i=0;i<dy;i++)put(&ppo,x0     ,y0+i   ,255,16);
        for(i=0;i<dy;i++)put(&ppo,x0+dx-1,y0+i   ,255,16);
        // writebmp("out10.bmp",p2,env.vvv); // colored should be better
    }
    return detect_lines1(p,x0-0*1,y0-0*2,dx+0*2,dy+0*3);

    i=lines.num; lines.num++;
    lines.m1[i]=y0;          lines.m2[i]=y0+5*dy/16;
    lines.m3[i]=y0+12*dy/16; lines.m4[i]=y0+dy-1;
    lines.x0[i]=x0;          lines.x1[i]=x0+dx-1;
    if(env.vvv)fprintf(stderr," - line= %d",lines.num);
    return 0;
}

/*
 * mark longest line which was used to estimate the rotation angle
 *    ---- only for debugging purpurses ----
 */
int mark_rotation_angle(pix *pp, int y2){
  pix p=(*pp);
  int i,x,y,dy;
  dy=lines.dy;
  if(env.vvv&32){
    for(y=0;y<p.y;y++)for(x=0;x<p.x;x++)ppo.p[x+p.x*y]=p.p[x+p.x*y]&(192);
    for(x=0;x<ppo.x;x++)if((x&35)>32)put(&ppo,x,y2+dy*x/ppo.x,255,32);
    // writebmp("out10.bmp",ppo,env.vvv); // colored should be better
  }
  return 0;
}

/*
** Detect rotation angle (one for whole image)
** Do it by finding longest text-line and determining the angle of this line.
** Angle is defined by dy (difference in y per width of image dx)
*/
int detect_rotation_angle(pix *pp){
  struct box *box2, *b3, *b4; pix p=(*pp);
  int i,x,y,dy=0,j,y2,k,my,ty=0,ml,mr,il,ir;
  lines.dy=0; lines.num=0;  // mean value of rise
  // ----- detect longest line, is it horizontal? ---------------
  // use function and box ??? for boxes rotated by different angles???
  {
    x=0; y2=0;
    for_each_data(&boxlist) {
	box2 = (struct box *)gocrlist_get_current(&boxlist);
	if( box2->x1 - box2->x0 + 1 > x
	 && box2->y1 - box2->y0 + 1 == 1 ){
	   x = box2->x1 - box2->x0 - 1; y2=box2->y1; 
	}
    } end_for_each(&boxlist);
    if ( 2*x > pp->x && pp->x > 100 ){
     if(env.vvv)fprintf(stderr, "# perfect flat line found, dy=0\n");
     mark_rotation_angle(pp, y2);
     return 0;
    }
    b3=b4=NULL;
    if(env.vvv)fprintf(stderr, "# detect longest line"); // or read from outside???
    // most black/white changes ??? new: most chars
    for(ty=y2=i=y=0;y<p.y;y+=4){
      my = j = il=ir=0;
      for_each_data(&boxlist) {
	box2 = (struct box *)gocrlist_get_current(&boxlist);
        if(box2->y0<=y && box2->y1>=y) {
          if( box2->x1<  p.x/4 ) il++;
          if( box2->x0>3*p.x/4 ) ir++;
          j++; my+=box2->y1-box2->y0+1; 
        }
      } end_for_each(&boxlist);
      /* j=num_cross(0,p.x-1,y,y,pp,cs); old slow version */
      if( j>i && il>3 && ir>3 ){ i=j; y2=y; ty=my; } 
    }
    my=0;if (i>0) my=ty/i;
    if(env.vvv)fprintf(stderr," - at y=%d crosses=%3d my=%d",y2,i,my);
    // detect the base line
    if(i>10){  // detect 2nd ??? highest base (not !?) of right/left side
      i=il=ir=ml=mr=0;
      for_each_data(&boxlist) {
	box2 = (struct box *)gocrlist_get_current(&boxlist);
        if(box2->y0<=y2 && box2->y1>=y2 && box2->y1-box2->y0>2*my/3){
          k=box2->y1; i++; y=box2->y1; // meanvalues and highest
          if(box2->x1<  p.x/4){ il++;ml+=y;if ( (!b3) || k<b3->y1 ) b3=box2; }
          if(box2->x0>3*p.x/4){ ir++;mr+=y;if ( (!b4) || k<b4->y1 ) b4=box2; }
        }
      } end_for_each(&boxlist);
      // b3 near b4 gives large error
      if(b3!=NULL && b4!=NULL){
         ml/=il; mr/=mr;
         if( b3->y1>ml-my/4 && b4->y1>mr-my/4 ) // prevent misinterpretations
         dy = (b4->y1 - b3->y1)*p.x/(b4->x1 - b3->x0); 
      }
    }
    lines.dy=dy;
    if(env.vvv)fprintf(stderr," - at crosses=%3d dy=%d\n",i,dy);
    mark_rotation_angle( pp, y2);
  }
  return 0;
}

/* ----- detect lines --------------- */
int detect_text_lines(pix * pp, int mo) {

  if (env.vvv)
    fprintf(stderr, "# scanning lines ");	// or read from outside???
  if (mo & 4){
    if (env.vvv) fprintf(stderr, "(zoning) ");
    detect_lines2(pp, 0, 0, pp->x, pp->y, 0);	// later replaced by better algo
  } else
    detect_lines1(pp, 0, 0, pp->x, pp->y);	// old algo

  if (env.vvv & 32) {	      // debug-output preparations
    int i, x, y, dy;
    dy = lines.dy;      /* can also be done in write_img !? */

    for (i = 0; i < lines.num; i++) { // mark lines
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
    // writebmp("out10.bmp",ppo,env.vvv); // colored should be better
  }

  if(env.vvv) fprintf(stderr,"\n");
  return 0;
}

  // ---- analyse boxes, find pictures (do this first!!!)
int detect_pictures(void) {
  int i = 0, x0, y0, x1, y1;
  struct box *box4;

  if ( env.numC == 0 ) return -1;
  env.avY = env.sumY / env.numC;
  env.avX = env.sumX / env.numC;
  if (env.vvv)
    fprintf(stderr, "# detect pictures, frames, noAlphas, mXmY= %d %d ... ",
  	    env.sumX/env.numC, env.sumY/env.numC);
  for_each_data(&boxlist) {
    box4 = (struct box *)gocrlist_get_current(&boxlist);
    x0 = box4->x0;    x1 = box4->x1;
    y0 = box4->y0;    y1 = box4->y1;

    if (x1 - x0 + 1 > 10 * env.avX || y1 - y0 + 1 > 10 * env.avY) {	/* large picture */
      env.sumX -= x1 - x0 + 1;
      env.sumY -= y1 - y0 + 1;
      env.numC--;
      box4->c = PICTURE;
      i++;
    }
  } end_for_each(&boxlist);
  // start second iteration
  for_each_data(&boxlist) {
    box4 = (struct box *)gocrlist_get_current(&boxlist);
    if( box4->c != PICTURE ){
      x0 = box4->x0;    x1 = box4->x1;
      y0 = box4->y0;    y1 = box4->y1;

      if (x1 - x0 + 1 > 4 * env.avX || y1 - y0 + 1 > 4 * env.avY) {	/* small picture */
        env.sumX -= x1 - x0 + 1;
        env.sumY -= y1 - y0 + 1;
        env.numC--;
        box4->c = PICTURE;
        i++;
      }
      if (4 * (y1 - y0 + 1) < env.avY || y1 - y0 < 2) {	// dots .,-_ etc.
        env.sumX -= x1 - x0 + 1;
        env.sumY -= y1 - y0 + 1;
        env.numC--;
      }				// better after pictures!
    }
  } end_for_each(&boxlist);
  if ( env.numC ) {		/* avoid div 0 */
    env.avY = env.sumY / env.numC;
    env.avX = env.sumX / env.numC;
  }
  if (env.vvv) {
    fprintf(stderr, " %d - boxes %d", i, env.numC);
    fprintf(stderr, " - new average X Y %d %d\n", env.avX, env.avY);
  }
  return 0;
}
