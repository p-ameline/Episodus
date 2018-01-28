// OCR engine (c) Joerg Schulenburg
// first engine: rule based --- numbers 0..9
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
#include "ocr0.h"
#include "ocr1.h"
#include "amiga.h"
#include "pnm.h"
#include "gocr.h"

#define MM	{ fprintf(stderr," mark %d\n",__LINE__); }

/* extern "C"{ */

// OCR engine ;)
wchar_t ocr0n(struct box *box1, pix  *b, int cs){
   pix  *p=(box1->p);
   int	d,x,y,x0=box1->x0,x1=box1->x1,y0=box1->y0,y1=box1->y1;
   int  dx=x1-x0+1,dy=y1-y0+1;	// size
   int  xa,xb,ya,yb, /* tmp-vars */
        i1,i2,i3,i4,i,j;
   wchar_t bc=UNKNOWN;				// bestletter
   // ad,ac will be used in future
   wchar_t ac=UNKNOWN;				// alternative char (2nd best)
   // int  ad=0;			// propability 0..100
   int hchar;		// char is higher than 'e'
   int gchar;		// char has ink lower than m3
   int dots=box1->dots;
   // --- hchar --- gchar -------------------------
   hchar=0;if( 2*y0<=2*box1->m2-(box1->m2-box1->m1) ) hchar=1;
   gchar=0;if( 2*y1>=2*box1->m3+(box1->m4-box1->m3) ) gchar=1;
   // --- test 5 near S ---------------------------------------------------
   if(hchar /* && (!gchar) */)  /* not stable */
   for(d=100;dx>2 && dy>4;){     // min 3x4
      if( num_cross(  dx/2,  dx/2,0,dy-1,b,cs)!=3
      &&  num_cross(5*dx/8,3*dx/8,0,dy-1,b,cs)!=3 ) break;
      // get the upper and lower hole koords, y around dy/4 ???
      x=5*dx/8;
      y  =loop(b,x,0,dy,cs,0,DO); if(y>dy/8) break;
      y +=loop(b,x,y,dy,cs,1,DO); if(y>dy/4) break;
      i1 =loop(b,x,y,dy,cs,0,DO)+y; if(y>5*dy/8) break;
      i3=y=(y+i1)/2;
      x  =loop(b,0,y,dx,cs,0,RI); if(x>3*dx/8) break;
      x +=loop(b,x,y,dx,cs,1,RI); if(x>  dx/2) break;
      i1 =loop(b,x,y,dx,cs,0,RI); i1=(i1+2*x)/2; // upper center x
      y=11*dy/16;
      x  =loop(b,dx-1  ,y,dx,cs,0,LE); if(x>dx/4) break;
      x +=loop(b,dx-1-x,y,dx,cs,1,LE); if(x>dx/2) break;
      i2 =loop(b,dx-1-x,y,dx,cs,0,LE); i2=dx-1-(i2+2*x)/2; // lower center x

// out_x(box1); fprintf(stderr,"i1,i3=%d,%d i2=%d\n",i1,i3,i2); MM;

      y  =loop(b,i1,0,dy,cs,0,DO);
      y +=loop(b,i1,y,dy,cs,1,DO);
      y  =(3*y+i3)/4;
      if( num_cross( i1, dx-1, y, y,b,cs)>0 ){ /* S or serif5 ? */
        y  =loop(b,i1  ,i3,dy,cs,0,DO);
        i  =loop(b,i1-1,i3,dy,cs,0,DO);
        if (y>i ) break; /* looks like S */
        y  =loop(b,i1  ,i3,dy,cs,0,UP);
        i  =loop(b,i1+1,i3,dy,cs,0,UP);
        if (i<y ) break; /* looks like S */
        x  =loop(b,dx-1,0,dx,cs,0,LE);
        i  =loop(b,dx-1,1,dx,cs,0,LE);
        if (x>i ) break; /* looks like S */
        if( num_cross(   0, dx/2, dy-1, dy-1,b,cs)>1 
         && num_cross( dx/2,dx-1,    0,    0,b,cs)>1 ) break; /* serifs */
        if ( loop(b,0,dy-1,dx,cs,0,RI)==0 ) break;

      }

      for(y=dy/4;y<3*dy/4;y++) // right gap?
      if( num_cross(i1,dx-1,dy/4,y,b,cs)==0 ) break;
      if( y==3*dy/4 ) break;

      for(y=dy/4;y<=11*dy/16;y++) // left gap?
      if( num_cross(0,i2,y,11*dy/16,b,cs)==0 ) break;
      if( y>11*dy/16 ) break;

      if( num_hole( x0, x1, y0, y1, p,cs) > 0 ) break;

      // sS5 \sl z  left upper v-bow ?
      for(x=dx,i=y=dy/4;y<dy/2;y++){
        j=loop(b,0,y,dx,cs,0,RI); if(j<x) { x=j; i=y; }
      } y=i;
      i1=loop(b,0,   dy/16     ,dx,cs,0,RI);
      i2=loop(b,0,(y+dy/16)/2  ,dx,cs,0,RI);
      i =loop(b,0,(y+dy/16)/2+1,dx,cs,0,RI); if( i>i2 ) i2=i;
      i3=loop(b,0, y  ,dx,cs,0,RI);
      i =loop(b,0, y-1,dx,cs,0,RI); if( i<i3 ) i3=i;
      if( 2*i2+1+dx/16 < i1+i3 ) break;

      return '5';
   }
   // --- test 1 ---------------------------------------------------
   if(/*hchar &&*/ !gchar) // Mar2000 modified I
   for(d=100;dy>4 && dy>dx && 2*dy>box1->m3-box1->m2;){     // min 3x4
      if( dots==1 ) break;

      for( i=dy/8,y=3*dy/8;y<dy-1 && i;y++ )	// 5% Fehlertoleranz ???
      if( num_cross(0, dx-1, y  , y  ,b,cs) != 1 ) i--;
      if( !i ) break;

      if( num_cross(0, dx-1, 0  , 0  ,b,cs) > 1
       && num_cross(0, dx-1, 1  , 1  ,b,cs) > 1 ) break; // ~/it_7

      // calculate upper and lower mass center (without lower serif)

      x =loop(b,0,7*dy/8-1,dx,cs,0,RI);   i2=x;
      x+=loop(b,x,7*dy/8-1,dx,cs,1,RI)-1; i2=(i2+x)/2;

      i1=loop(b,dx-1  ,1+0* dy/4,dx,cs,0,LE); i1=dx-1-i1-(x-i2)/2;

      x =(i1-i2+4)/8; i1+=x; i2-=x;
      
      if( get_line2(i1,0,i2,dy-1,b,cs,100)<95 ) break;
      // upper and lower width
      x =loop(b,(i1+i2)/2,dy/2,dx,cs,1,RI); i=x; i3=0;
      for(y=0;y<7*dy/8;y++)
        if( loop(b,i1+y*(i2-i1)/dy, y,dx,cs,1,RI)-i > 1+dx/8 ) break;
      if(y<7*dy/8) break;
// out_x(box1); printf(" i12=%d %d\n",i1,i2);
      x =loop(b,i2,dy-1,dx,cs,1,LE); j=x;
      x =loop(b,i2,dy-2,dx,cs,1,LE); if(x>j)j=x; i=j;
      x =loop(b,i2,dy-1,dx,cs,1,RI); j=x;
      x =loop(b,i2,dy-2,dx,cs,1,RI); if(x>j)j=x;
      if(abs(i-j)>1+dx/8) i3|=1;
      if(i3) break;        
      // out_x(box1);printf(" 11 i=%d j=%d dx=%d\n",i,j,dx);
      for(i=dx,j=y=0;y<3*dy/8;y++){
        x =loop(b,0,y,dx,cs,0,RI); if(x<i) { i=x;j=y; }
      }  
      if ( i1-i<7*dx/16 ) break; // 4*dx/8 => 7*dx/16
      x =loop(b,0,dy/2,dx,cs,0,RI);
      j =loop(b,x,dy/2,dx,cs,1,RI); if( j>x+(dy+16)/32 ) break; // ~l
      x =loop(b,0,0,dx,cs,0,RI); // straight line ???
      j =loop(b,0,1,dx,cs,0,RI);               if( j>x ) break; // ~l
      if( x==j ) j =loop(b,0,dy/8,dx,cs,0,RI); if( j>x ) break;
      if( x==j ) if( loop(b,0,dy/4,dx,cs,0,RI)>x ) break;  // ~l
      x=j;
//      j =loop(b,0,2,dx,cs,0,RI); if( j>=x ) break; x=j; // ~l
//      j =loop(b,0,   0,dx,cs,0,DO); if( !j  ) break; // ~7
      if( !hchar ) // ~ right part of n
      if( loop(b,dx-1,   1,dx,cs,0,LE)-dy/6
        > loop(b,dx-1,dy/4,dx,cs,0,LE)
       || get_bw(x1+1,x1+2,y0,y0+dy/8,p,cs,1)==1 ) break; // Mai00
      if( loop(b,dx-1,3*dy/4,dx,cs,0,LE) > dx/2
       && get_bw(x1-dx/4,x1,y1-1,y1,p,cs,1)==1 ) break; // ~z Jun00

      i=loop(b,  dx/8,0,dy,cs,0,DO);
      j=loop(b,3*dx/4,0,dy,cs,0,DO); if(j<dy/2 && j+dy/16>=i) break; // ~\tt l

      if(bc==UNKNOWN) 
	bc='1'; else ac='1';
      break;
   }
   // --- test 2 ---------------------------------------------------
   if(bc==UNKNOWN) 
   for(d=100;dx>2 && dy>4;){     // min 3x4
      if( get_bw(x0+dx/2, x0+dx/2 , y1-dy/5, y1     ,p,cs,1) != 1 ) break;
      if( get_bw(x0+dx/2, x0+dx/2 , y0     , y0+dy/5,p,cs,1) != 1 ) break;
      if( get_bw(x0+dx/8, x1-dx/3 , y1-dy/3, y1-dy/3,p,cs,1) != 1 ) break;

      if( get_bw(x1-dx/3, x1      , y0+dy/3 , y0+dy/3,p,cs,1) != 1 ) break;
      if( get_bw(x0     , x0+dx/9 , y0      , y0     ,p,cs,1) == 1 ) break;
      if( get_bw(x1-dx/9, x1      , y0      , y0     ,p,cs,1) == 1 ) break;
      if( get_bw(x0     , x0+dx/ 8, y1-dy/16, y1     ,p,cs,1) != 1 ) break;
      if( num_cross(x0, x1-dx/8, y0+dy/2, y0+dy/2, p,cs) != 1 ) break;
      // out_x(box1);

      for( x=x0+dx/4;x<x1-dx/6;x++ )		// C
      if( num_cross( x, x, y0, y1, p,cs) == 3 ) break;
      if( x>=x1-dx/6 ) break;

      for(i=1,y=y0;y<y0+dy/2;y++ )
      if( num_cross( x0, x1, y, y, p,cs) == 2 ) i=0;
      if( i ) break;

      for(i=1,y=y0+dy/5;y<y0+3*dy/4;y++ )
      if( get_bw( x0, x0+dx/3, y, y, p,cs,1) == 0 ) i=0;
      if( i ) break;

      x=x1-dx/3,y=y1;
      turmite(p,&x,&y,x0,x1,y0,y1,cs,UP,ST); if( y<y1-dy/5 ) break;
      turmite(p,&x,&y,x0,x1,y0,y1,cs,ST,UP); if( y<y1-dy/4 ) break;
      turmite(p,&x,&y,x0,x1,y0,y1,cs,UP,ST); if( y<y0+dy/3 ) break; y++;
      turmite(p,&x,&y,x0,x1,y0,y1,cs,RI,ST);
      if( x<x1 ){ x--; // hmm thick font and serifs
        turmite(p,&x,&y,x0,x1,y0,y1,cs,UP,ST); if( y<y0+dy/2 ) break; y++;
        turmite(p,&x,&y,x0,x1,y0,y1,cs,RI,ST);
        if( x<x1 ) break;
      }

      // test ob rechte Kante ansteigend
      for(x=0,y=dy/18;y<=dy/3;y++){ // rechts abfallende Kante/Rund?
        i=loop(p,x1,y0+y,dx,cs,0,LE); // use p (not b) for broken chars
        if( i<x ) break;	// rund
        if( i>x ) x=i;
      }
      if (y>dy/3 ) break; 	// z

      if( num_hole( x0, x1,      y0,      y1, p,cs) > 0 ) // there is no hole
      if( num_hole( x0, x0+dx/2, y0, y0+dy/2, p,cs) == 0 ) // except in some beauty fonts
      break;

      i1=loop(b,dx-1-dx/16,0,dy,cs,0,DO);  // Jul00
      i2=loop(b,     dx/ 2,0,dy,cs,0,DO); if( i2+dy/32>=i1 ) break; // ~z

      if(!hchar)
	if(bc!=UNKNOWN) { 
	  ac='2'; 
	  break; 
	}
      return '2';
   }
   // --- test 3 -------
   for(d=100;dx>4 && dy>3;){	// dy<=dx nicht perfekt! besser mittleres
				// min-suchen fuer m
      if( get_bw(x0+dx/2,x0+dx/2,y0,y0+dy/4,p,cs,1) == 0 ) break; // ~4
      if( get_bw(x0+dx/2,x0+dx/2,y1-dy/8,y1,p,cs,1) == 0 ) break; // ~4
      if( num_cross(x0+dx/2,x0+dx/2,y0     ,y1,p,cs) < 2 ) break;
      if( num_cross(x0+dx/4,x0+dx/4,y1-dy/2,y1,p,cs) == 0 ) break;
      if( loop(b,dx/2,  0   ,dy,cs,0,DO)>dy/4 ) break;
      if( loop(b,dx/2,  dy-1,dy,cs,0,UP)>dy/4 ) break;
      if( loop(b,dx-1,  dy/3,dy,cs,0,LE)>dy/4 ) break;
      if( loop(b,dx-1,2*dy/3,dy,cs,0,LE)>dy/4 ) break;
      for( i3=x=0,i1=y=dy/5;y<dy/2;y++ ){ // suche erstes >
        i=loop(b,0,y,dx,cs,0,RI);
        if( i>x ) { i3=x=i;i1=y; }
      } i3--; if( i3<dx/2 ) break;
      for( i4=x=0,i2=y=dy-1-dy/8;y>dy/2;y-- ){
        i=loop(b,0,y,dx,cs,0,RI);
        if( i>x ) { i4=x=i;i2=y; }
      } i4--; if( i4<dx/2 ) break;
      for( x=xa=0,ya=y=dy/4;y<3*dy/4;y++ ){  // right gap
        i=loop(b,dx-1,y,dx,cs,0,LE);
        if( i>=xa ) { xa=i;ya=y;x=xa+loop(b,dx-1-xa,y,dx,cs,1,LE); }
      } if(dy>3*dx) if( xa<2 && x-xa<dx/2 ) break; // ~]
      if( get_bw(i3,i3,i1,i2  ,b,cs,1) != 1 ) break;
      if( get_bw(i4,i4,i1,i2  ,b,cs,1) != 1 ) break;
      if( get_bw(i3,i3,0 ,i1  ,b,cs,1) != 1 ) break;
      if( get_bw(i4,i4,i1,dy-1,b,cs,1) != 1 ) break;  // m like
      if( num_hole( x0, x1     , y0, y1     , p,cs) > 0 ) // there is no hole
      if( num_hole( x0, x0+dx/2, y0, y0+dy/2, p,cs) == 0 ) // except in some beauty fonts
      break;
      // out_x(box1);

      if(bc!=UNKNOWN){ 
	ac='3';
	break; 
      }
      return '3';
   }
   // --- test 4 -------
   if(bc==UNKNOWN)
   for(d=100;dx>3 && dy>5;){     // dy>dx
      // upper width smaller than middle width
      if( loop(b,0     ,3*b->y/16,dx,cs,0,RI)
         +loop(b,b->x-1,3*b->y/16,dx,cs,0,LE)
       <= loop(b,b->x-1,2*b->y/4 ,dx,cs,0,LE)
         +loop(b,0     ,2*b->y/4 ,dx,cs,0,RI) ) break;
      // --- follow line from (1,0) to (0,.7)
//       out_x(box1);MM;
      x=0,y=0;
      turmite(b,&x,&y,0,dx-1,0,dy-1,cs,RI,ST);
      if( x+loop(b,x,0,dx,cs,1,RI)<dx/2 ) break;
      turmite(b,&x,&y,0,dx-1,0,dy-1,cs,DO,LE); if( x>=0 ) break;
      if( (y=loop(b,0,0,dy,cs,0,DO)) < dy/4 ) break;
      if( (y+loop(b,0,y,dy,cs,1,DO)) < dy/2 ) break;
      if( get_bw(x0     , x0+3*dx/8, y1-dy/6, y1-dy/6,p,cs,1) == 1 ) break;
      if( get_bw(x0+dx/2, x1     , y1-dy/3, y1-dy/3,p,cs,1) != 1 ) break;
      if( get_bw(x0+dx/2, x0+dx/2, y0+dy/3, y1-dy/5,p,cs,1) != 1 ) break;
      if( loop(b,b->x-1,  b->y/4,dx,cs,0,LE) >
          loop(b,b->x-1,2*b->y/4,dx,cs,0,LE)+1 ) break;
      for(i=dx/8+1,x=0;x<dx && i;x++){
        if( num_cross(x ,x   ,0  ,dy-1, b,cs) == 2 ) i--;
      } if( i ) break;
      for(i=dy/6+1,y=dy/4;y<dy && i;y++){
        if( num_cross(0 ,dx-1,y  ,y   , b,cs) == 2 ) i--;
      } if( i ) break;
      for(i=dy/10+1,y=dy-1-dy/4;y<dy && i;y++){
        if( num_cross(0   ,dx-1,y ,y  , b,cs) == 1 )
        if( num_cross(dx/2,dx-1,y ,y  , b,cs) == 1 ) i--;
      } if( i ) break;
      if( gchar )  // ~q
      if( loop(b,0,dy/16,dx,cs,0,RI)<dx/2 )
      if( num_hole ( x0, x1, y0, y1,p,cs) > 0 ) break; 

      i=loop(b,dx-1,dy-1,dx,cs,0,LE); // ~9
      if( i>3*dx/4
       && i-loop(b,dx-1,dy-1-dy/8,dx,cs,0,LE)>dx/4 ) break;

      if( num_cross(0 ,dx-1,dy/16 ,dy/16  , b,cs) == 2 // ~9
       && loop(b,dx-1,dy/16        ,dx,cs,0,LE)>
          loop(b,dx-1,dy/16+1+dy/32,dx,cs,0,LE) ) break;
      
      bc='4';
      break;
   }
   // --- test 6 -------
   for(d=100;dx>3 && dy>4;){     // dy>dx
      if( loop(b,   0,  dy/4,dx,cs,0,RI)>dx/2 ) break;
      if( loop(b,   0,  dy/2,dx,cs,0,RI)>dx/4 ) break;
      if( loop(b,   0,3*dy/4,dx,cs,0,RI)>dx/4 ) break;
      if( loop(b,dx-1,3*dy/4,dx,cs,0,LE)>dx/2 ) break;
      if( num_cross(x0+  dx/2,x0+  dx/2,y0     ,y1     ,p,cs) != 3
       && num_cross(x0+5*dx/8,x0+5*dx/8,y0     ,y1     ,p,cs) != 3 ) break;
      if( num_cross(x0     ,x1     ,y1-dy/4,y1-dy/4,p,cs) != 2 ) break;
      for( y=y0+dy/6;y<y0+dy/2;y++ ){
        x =loop(p,x1    ,y  ,dx,cs,0,LE); if( x>dx/2 ) break;
        x+=loop(p,x1-x+1,y-1,dx,cs,0,LE); if( x>dx/2 ) break;
      } if( y>=y0+dy/2 ) break;
      // --- follow line from left down to right up
      x=x0,y=y1-dy/3;
      turmite(p,&x,&y,x0,x1,y0,y1,cs,RI,ST); if( x>x0+dx/4 ) break;
      x=x1,y=y1-dy/3;
      turmite(p,&x,&y,x0,x1,y0,y1,cs,LE,ST); if( x<x1-dx/4 ) break;
      x=x0+dx/2,y=y0;
      turmite(p,&x,&y,x0,x1,y0,y1,cs,DO,ST); if( y>y0+dy/8 ) break;

      if( num_hole ( x0, x1, y0+dy/4, y1,p,cs) != 1 ) break; 
      if( num_hole ( x0, x1, y0     , y1,p,cs) != 1 ) break; 
//    out_x(box1); printf(" x0 y0 %d %d\n",x0,y0);
      i1=loop(b,0,dy/8     ,dx,cs,0,RI);
      i3=loop(b,0,dy-1-dy/8,dx,cs,0,RI);
      i2=loop(b,0,dy/2     ,dx,cs,0,RI); if(i1+i3-2*i2<1 && i1+i2+i3>0) break;  // convex from left
      for( x=dx,y=0;y<dy/4;y++ ){	// ~ b (serife?)
        i1=loop(b,0,y,dx,cs,0,RI);
        if(i1<x) x=i1; else if (i1>x) break;
      } if( y<dy/4 ) break;
      // ~& (with open upper loop)
      for( i=0,y=dy/2;y<dy;y++){
        if( num_cross(dx/2,dx-1,y,y,b,cs) > 1 ) i++; if( i>dy/8 ) break;
      } if( y<dy ) break;
      bc='6';
      break;
   }
   // --- test 7 ---------------------------------------------------
   if(dy>box1->m3-box1->m2) // too small
   if(hchar && !gchar)
   for(d=100;dx>2 && dy>4;){     // dx>1 dy>2*dx 
      if( loop(b,dx/2,0,dy,cs,0,DO)>dy/8 ) break;
      if( num_cross(0,dx-1,3*dy/4,3*dy/4,b,cs) != 1 ) break; // preselect
      for( yb=xb=y=0;y<dy/2;y++){ // upper h-line and gap
        j=loop(b,0,y,dx,cs,0,RI);if(xb>0 && j>dx/4) break; // gap after h-line
        j=loop(b,j,y,dx,cs,1,RI);if(j>xb){ xb=j;yb=y; }  // h-line
      } if( xb<dx/4 || y==dy/2 ) break;
      j=loop(b,0,dy/2,dx,cs,0,RI);
      j=loop(b,j,dy/2,dx,cs,1,RI); if(xb<2*j) break; // minimum thickness
      for(x=0;y<dy;y++){	// one v-line?
        if( num_cross(0,dx-1,y,y,b,cs) != 1 ) break;
        j=loop(b,dx-1,y,dx,cs,0,LE); if( j<x ) break; if( j-1>x ) x=j-1;
      } if( y<dy || x<dx/3 ) break;
      j =loop(b,dx-1,0,dy,cs,0,DO);  // ~T
      j+=loop(b,dx-1,j,dy,cs,1,DO);
      i =loop(b,dx-1,j,dx,cs,0,LE); if(j<dy/2 && i>j) break;
      return '7';
   }
   // --- test 8 ---------------------------------------------------
   // last change: May15th,2000 JS
   if(dy>20 || (hchar && !gchar)) // either all details are visible
   for(d=100;dx>2 && dy>4;){     //    or we need large height
      if( num_cross(x0,x1,y0+dy/4,y0+dy/4,p,cs) != 2 ) break; // ~gr (glued)
      if( num_cross(x0,x1,y1-dy/4,y1-dy/4,p,cs) != 2 ) break;
      if( get_bw(x0,x0+dx/4,y1-dy/4,y1-dy/4,p,cs,1) == 0 ) break; // ~9
      if( get_bw(x0,x0+dx/2,y0+dy/4,y0+dy/4,p,cs,1) == 0 ) break;
      if( get_bw(x0+dx/2,x0+dx/2,y0+dy/4,y1-dy/4,p,cs,1) == 0 ) break; // ~0
//    out_x(box1); printf(" x0 y0 %d %d\n",x0,y0);
      for( x=0,i=y=y0+dy/3;y<=y1-dy/3;y++){	// linke Kerbe
	j=loop(p,x0,y,dx,cs,0,RI); if( j>x ) { x=j; i=y; }
      } if(i>y0+dy/2+dy/20 || x<dx/8 || x>dx/2) break;	// no gB
      j =   loop(p,x1,y1-  dy/4,dx,cs,0,LE);
      if( j>loop(p,x1,y1-  dy/5,dx,cs,0,LE)
       && j>loop(p,x1,y1-2*dy/5,dx,cs,0,LE) ) break;	// &
      if( num_hole(x0,x1,y0,i+1   ,p,cs)!=1 )
      if( num_hole(x0,x1,y0,i+dy/8,p,cs)!=1 ) break;	// upper hole
      if( num_hole(x0,x1,i-1,y1,p,cs)!=1 ) break; i1=i; // lower hole
      for( x=0,i2=i=y=y0+dy/3;y<=y1-dy/3;y++){
	j=loop(p,x1,y,dx,cs,0,LE); if( j>=x ) i2=y;
        if( j>x ) { x=j; i=y; }
      }
      if( i>y0+dy/2+dy/10 ) break;
      // if( x<dx/8 ) break;
      if( x>dx/2 ) break;
      if( num_cross(x0,x1, i      , i      ,p,cs) != 1
       && num_cross(x0,x1, i+1    , i+1    ,p,cs) != 1
       && num_cross(x0,x1,(i+i2)/2,(i+i2)/2,p,cs) != 1 ) break; // no g
      if(abs(i1-i)>dy/10) break;
      // ~B ff
      for(i=dx,y=0;y<dy/8+2;y++){
        j=loop(b,0,y,dx,cs,0,RI); if( j<i ) i=j; if( j>i+dx/16 ) break;
      } if( y<dy/8+2 ) break;
      for(i=dx,y=0;y<dy/8+2;y++){
        j=loop(b,0,dy-1-y,dx,cs,0,RI);
        if( j<i ) i=j; if( j>i+dx/16 ) break;
      } if( y<dy/8+2 ) break;
      if(  dy>16 && num_cross(0,dx-1,dy-1,dy-1,p,cs) > 1
        && loop(b,0,dy-1,dx,cs,0,RI) <dx/8+1 ) break; // no fat serif S
      for( i=0,y=dy/2;y<dy;y++){
        if( num_cross(0,dx-1,y,y,b,cs) > 2 ) i++; if( i>dy/8 ) break;
      } if( y<dy ) break;
      if(bc==UNKNOWN) 
	return '8';
      else ac='8';
      break;
   }
   // --- test 9 \it g ---------------------------------------------------
   if(bc==UNKNOWN) 
   for(d=100;dx>2 && dy>4;){     // dx>1 dy>2*dx 
      // out_x(box1);printf(" 1 x0 y0 %d %d\n",x0,y0);
      if( num_cross(x0+  dx/2,x0+  dx/2,y0,y1,p,cs) != 3 // pre select
       && num_cross(x0+3*dx/8,x0+3*dx/8,y0,y1,p,cs) != 3
       && num_cross(x0+  dx/4,x1  -dx/4,y0,y1,p,cs) != 3 ) break;
      for( x=0,i=y=y0+dy/2;y<=y1-dy/4;y++){	// suche kerbe
	j=loop(p,x0,y,dx,cs,0,RI); 
        if( j>x ) { x=j; i=y; }
      }
      if( x<4*dx/8 ) break;
      if( num_cross(x0+dx/2,x1,i,y1,p,cs) != 1 ) break;
      if( num_hole(x0,x1,y0,i+1,p,cs)!=1 ) break;
      if( num_hole(x0,x1,i-1,y1,p,cs)!=0 ) break;
      if( loop(p,x0,y1  ,dy,cs,0,RI)>dx/3 &&
          loop(p,x0,y1-1,dy,cs,0,RI)>dx/3) break; // no q
      for( x=0,i=y=y0+dy/3;y<=y1-dy/3;y++){	// suche kerbe
	j=loop(p,x1,y,dx,cs,0,LE); 
        if( j>x ) { x=j; i=y; }
      } if( x>dx/2 ) break;		// no g
      i1=loop(b,dx-1,dy/8     ,dx,cs,0,LE); if(i1>dx/2) break;
      i3=loop(b,dx-1,dy-1-dy/8,dx,cs,0,LE);
      i2=loop(b,dx-1,dy/2     ,dx,cs,0,LE); if(i1+i3-2*i2<0) break; // konvex 
      i1=loop(b,dx-1,dy/4     ,dx,cs,0,LE); if(i1>dx/2) break;
      i3=loop(b,dx-1,dy-1-dy/8,dx,cs,0,LE);
      for(y=dy/4;y<dy-1-dy/4;y++){
        i2=loop(b,dx-1,y,dx,cs,0,LE);
        if(i1+i3-2*i2<-1) break;  // konvex from right ~g ~3
      } if(y<dy-1-dy/4) break;
      x=loop(b,dx  -1,6*dy/8,dx,cs,0,LE); if(x>0) x--; // robust
      y=loop(b,dx-x-1,  dy-1,dy,cs,0,UP);
      if(y<dy/8) break; // ~q (serif!)
      if((!gchar) &&  hchar) return '9';
      if( ac==UNKNOWN ) 
	ac='9'; // unsure
      break;
   }
   // 0 is same as O !?
   // --- test 0 (with a hole in it ) -----------------------------
   for(d=100;dx>2 && dy>3;){     // min 3x4
      if( get_bw(x0      , x0+dx/2,y0+dy/2 , y0+dy/2,p,cs,1) != 1 ) break;
//      out_b(b,0,0,dx,dy,cs);
      if( get_bw(x1-dx/2 , x1     ,y0+dy/2 , y0+dy/2,p,cs,1) != 1 ) break;
      if( get_bw(x0+dx/2 , x0+dx/2,y1-dy/2 , y1,     p,cs,1) != 1 ) break;
      if( get_bw(x0+dx/2 , x0+dx/2,y0      , y0+dy/2,p,cs,1) != 1 ) break;
      if( get_bw(x0+dx/2 , x0+dx/2,y0+dy/3 , y1-dy/3,p,cs,1) != 0 ) break;

      if( num_cross(x0+dx/2,x0+dx/2,y0      , y1     ,p,cs)  != 2 ) break;
      if( num_cross(x0+dx/3,x1-dx/3,y0      , y0     ,p,cs)  != 1 ) // AND
      if( num_cross(x0+dx/3,x1-dx/3,y0+1    , y0+1   ,p,cs)  != 1 ) break;
      if( num_cross(x0+dx/3,x1-dx/3,y1      , y1     ,p,cs)  != 1 ) // against "rauschen"
      if( num_cross(x0+dx/3,x1-dx/3,y1-1    , y1-1   ,p,cs)  != 1 ) break;
      if( num_cross(x0     ,x0     ,y0+dy/3 , y1-dy/3,p,cs)  != 1 )
      if( num_cross(x0+1   ,x0+1   ,y0+dy/3 , y1-dy/3,p,cs)  != 1 ) break;
      if( num_cross(x1     ,x1     ,y0+dy/3 , y1-dy/3,p,cs)  != 1 )
      if( num_cross(x1-1   ,x1-1   ,y0+dy/3 , y1-dy/3,p,cs)  != 1 ) break;
      if( num_hole(x0,x1,y0,y1,p,cs) != 1 ) break;
      
      if( loop(b,0   ,0   ,x1-x0,cs,0,RI)<=
          loop(b,0   ,2   ,x1-x0,cs,0,RI)  ) break;

      x=loop(b,dx-1,dy-1-dy/3,x1-x0,cs,0,LE);	// should be minimum
      for( y=dy-1-dy/3;y<dy;y++ ){
        i=loop(b,dx-1,y,x1-x0,cs,0,LE);
        if( i<x ) break; x=i;
      }
      if( y<dy ) break;

      // ~D
      if(     loop(b,0,     dy/16,dx,cs,0,RI)
         +    loop(b,0,dy-1-dy/16,dx,cs,0,RI)
         <= 2*loop(b,0,     dy/2 ,dx,cs,0,RI)+dx/8 ) break; // not konvex

      if( loop(b,dx-1,     dy/16,dx,cs,0,LE)>dx/8 )
      if( loop(b,0   ,     dy/16,dx,cs,0,RI)<dx/16 ) break;
      if( loop(b,dx-1,dy-1-dy/16,dx,cs,0,LE)>dx/8 )
      if( loop(b,0   ,dy-1-dy/16,dx,cs,0,RI)<dx/16 ) break;
      if( get_bw(x1-dx/32,x1,y0,y0+dy/32,p,cs,1) == 0
       && get_bw(x1-dx/32,x1,y1-dy/32,y1,p,cs,1) == 0
       && ( get_bw(x0,x0+dx/32,y0,y0+dy/32,p,cs,1) == 1
         || get_bw(x0,x0+dx/32,y1-dy/32,y1,p,cs,1) == 1 ) ) break; // ~D

       // italic a
      for(i=0,y=6*dy/8;y<dy;y++)
      if( num_cross(0,dx-1,y,y,b,cs) > 2 ) i++; else i--; if(i>0) break; // ~'a' \it a
      if( hchar ){ bc='0'; }
      break;
   } 
   // --- test 0 with a straight line in it -------------------
   for(d=100;dx>4 && dy>5;){  /* v0.3.1+ */
      if( get_bw(x0      , x0+dx/2,y0+dy/2 , y0+dy/2,p,cs,1) != 1 ) break;
      if( get_bw(x1-dx/2 , x1     ,y0+dy/2 , y0+dy/2,p,cs,1) != 1 ) break;
      if( get_bw(x0+dx/2 , x0+dx/2,y1-dy/2 , y1,     p,cs,1) != 1 ) break;
      if( get_bw(x0+dx/2 , x0+dx/2,y0      , y0+dy/2,p,cs,1) != 1 ) break;
      if( get_bw(x0+dx/2 , x0+dx/2,y0+dy/3 , y1-dy/3,p,cs,1) != 1 ) break;

      // out_x(box1); printf(" x0 y0 %d %d\n",x0,y0);
      if( num_cross(x0+dx/2,x0+dx/2,y0      , y1     ,p,cs)  != 3 ) break;
      if( num_cross(x0+dx/3,x1-dx/3,y0      , y0     ,p,cs)  != 1 ) // AND
      if( num_cross(x0+dx/3,x1-dx/3,y0+1    , y0+1   ,p,cs)  != 1 ) break;
      if( num_cross(x0+dx/3,x1-dx/3,y1      , y1     ,p,cs)  != 1 ) // against "rauschen"
      if( num_cross(x0+dx/3,x1-dx/3,y1-1    , y1-1   ,p,cs)  != 1 ) break;
      if( num_cross(x0     ,x0     ,y0+dy/3 , y1-dy/3,p,cs)  != 1 )
      if( num_cross(x0+1   ,x0+1   ,y0+dy/3 , y1-dy/3,p,cs)  != 1 ) break;
      if( num_cross(x1     ,x1     ,y0+dy/3 , y1-dy/3,p,cs)  != 1 )
      if( num_cross(x1-1   ,x1-1   ,y0+dy/3 , y1-dy/3,p,cs)  != 1 ) break;
      if( num_hole(x0,x1,y0,y1,p,cs) != 2 ) break;
      
      if( loop(b,0   ,0   ,x1-x0,cs,0,RI)<=
          loop(b,0   ,2   ,x1-x0,cs,0,RI)  ) break;

      x=loop(b,dx-1,dy-1-dy/3,x1-x0,cs,0,LE);	// should be minimum
      for( y=dy-1-dy/3;y<dy;y++ ){
        i=loop(b,dx-1,y,x1-x0,cs,0,LE);
        if( i<x ) break; x=i;
      }
      if( y<dy ) break;
      
      /* test for straight line */
      y =loop(b,dx/2,dy-1  ,y1-y0,cs,0,UP); if(y>dy/4) break;
      y+=loop(b,dx/2,dy-1-y,y1-y0,cs,1,UP); if(y>dy/4) break;
      y+=loop(b,dx/2,dy-1-y,y1-y0,cs,0,UP); if(3*y>2*dy) break;
      x =loop(b,dx/2,dy-y,dx/2,cs,0,RI); if(x==0) break;
      if( loop(b,dx/2+x-1,dy-y,y1-y0,cs,0,UP)==0 ) break;

       // $
      for(i=0,y=dy/4;y<dy-dy/4-1;y++)
      if( loop(b,   0,0,dx-1,cs,0,RI) > dx/4
       || loop(b,dx-1,0,dx-1,cs,0,LE) > dx/4 ) break;
      if( y<dy-dy/4-1 ) break;

      // ~D
      if(     loop(b,0,     dy/16,dx,cs,0,RI)
         +    loop(b,0,dy-1-dy/16,dx,cs,0,RI)
         <= 2*loop(b,0,     dy/2 ,dx,cs,0,RI)+dx/8 ) break; // not konvex

      if( loop(b,dx-1,     dy/16,dx,cs,0,LE)>dx/8 )
      if( loop(b,0   ,     dy/16,dx,cs,0,RI)<dx/16 ) break;
      if( loop(b,dx-1,dy-1-dy/16,dx,cs,0,LE)>dx/8 )
      if( loop(b,0   ,dy-1-dy/16,dx,cs,0,RI)<dx/16 ) break;
      if( get_bw(x1-dx/32,x1,y0,y0+dy/32,p,cs,1) == 0
       && get_bw(x1-dx/32,x1,y1-dy/32,y1,p,cs,1) == 0
       && ( get_bw(x0,x0+dx/32,y0,y0+dy/32,p,cs,1) == 1
         || get_bw(x0,x0+dx/32,y1-dy/32,y1,p,cs,1) == 1 ) ) break; // ~D

       // italic a
      for(i=0,y=6*dy/8;y<dy;y++)
      if( num_cross(0,dx-1,y,y,b,cs) > 2 ) i++; else i--; if(i>0) break; // ~'a' \it a
      if( hchar ){ bc='0'; }
      break;
   } 
   if(box1->ac==UNKNOWN)
     box1->ac=ac;
   return bc;
}
