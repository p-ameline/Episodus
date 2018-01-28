/* (c) Joerg Schulenburg 2000
    v0.1.0 initial version (stdin added)
    v0.2.0 popen added
    v0.2.7 review by Bruno Barberi Gnecco
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
//#include <unistd.h>

#include "pnm.h"
#ifdef HAVE_PAM_H
# include <pam.h>
#endif
#ifdef HAVE_PNM_H
# include <pnm.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
#else
# include <ctype.h>
#endif

#define EE()         fprintf(stderr,"\nERROR "__FILE__" L%d: ",__LINE__)
#define E0(x0)       {EE();fprintf(stderr,x0 "\n");      }
#define F0(x0)       {EE();fprintf(stderr,x0 "\n");      exit(1);}
#define F1(x0,x1)    {EE();fprintf(stderr,x0 "\n",x1);   exit(1);}
#define F2(x0,x1,x2) {EE();fprintf(stderr,x0 "\n",x1,x2);exit(1);}

/*
    feel free to expand this list of usable converting programs
    Note 1: the last field must be NULL.
    Note 2: "smaller" extensions must come later: ".pnm.gz" must come
       before ".pnm".
 */
char *xlist[]={
  ".pnm.gz",	"gzip -cd",
  ".pbm.gz",	"gzip -cd",
  ".pgm.gz",	"gzip -cd",
  ".ppm.gz",	"gzip -cd",
  ".pnm.bz2",	"bzip2 -cd",
  ".pbm.bz2",	"bzip2 -cd",
  ".pgm.bz2",	"bzip2 -cd",
  ".ppm.bz2",	"bzip2 -cd",
  ".jpg", 	"djpeg -gray -pnm",
  ".jpeg",	"djpeg -gray -pnm",
  ".gif",	"giftopnm",
  ".bmp",	"bmptoppm",
  ".tiff",	"tifftopnm",
  ".png",	"pngtopnm",
  NULL
};

/* return a pointer to command converting file to pnm or NULL */
char *testsuffix(char *name){
  int i;

  for(i = 0; xlist[i] != NULL; i += 2 ) {
    if(strstr(name, xlist[i]) != NULL)
      return xlist[i+1];
  }
  return NULL;
}

#ifdef HAVE_PNM_H
/* js: I need pgm format output from pnm input
       This routine fails if input is pbm.
        Bruno, could this be changed?
 */
void readpnm(char *name, pix * p, int vvv) {
  FILE *fp;
  char *pip;
  int i, j;
#ifdef HAVE_PAM_H
  struct pam inpam;
  tuple *tuplerow;
#else
  xel *data;
  xelval maxval;
  int format;
#endif

  /* open file; test if conversion is needed. */
  if (name[0] == '-' && name[1] == '\0') {
    fp = stdin;
  }
  else {
    pip = testsuffix(name);
    if (!pip) {
      fp = fopen(name, "rb");
      if (!fp)
	F1("opening file %s", name);
    }
    else {
      char *buf = (char *)malloc((strlen(pip)+strlen(name)+2)*
	  sizeof(char));
      sprintf(buf, "%s %s", pip, name);
      if (vvv) {
	fprintf(stderr, "# popen( %s )\n", buf);
      }
#if defined(__USE_POSIX2)
      fp = popen(buf, "r");
#else
      F0("sorry, you must compile with __USE_POSIX2 to use pipes");
#endif
      if (!fp)
	F1("opening pipe %s", buf);
    }
  }

  /* test for p, if NULL, alloc it. */
  if (!p)
    p = (pix *)malloc(sizeof(pix));
/*  else if (p->p)
    free( p->p ); */

  /* read pgm */
#ifdef HAVE_PAM_H
  pnm_readpaminit(fp, &inpam, sizeof(inpam));
  p->x = inpam.width;
  p->y = inpam.height;
  if ( !(p->p = (unsigned char *)malloc(p->x*p->y)) )
    F1("Error at malloc: p->p: %d bytes", p->x*p->y);
  tuplerow = pnm_allocpamrow(&inpam);
  for ( i=0; i < inpam.height; i++ ) {
    pnm_readpamrow(&inpam, tuplerow);
    for ( j = 0; j < inpam.width; j++ )
      p->p[i*inpam.width+j] = tuplerow[j][0];
  }
  pnm_freepamrow(tuplerow);
#else
  pnm_readpnminit(fp, &(p->x), &(p->y), &maxval, &format);
  if ( !(p->p = (unsigned char *)malloc(p->x*p->y)) )
    F0("Error at malloc: p->p");
  data = pnm_allocrow(p->x);
  for ( i=0; i < p->y; i++ ) {
    pnm_readpnmrow(fp, data, p->x, maxval, format);
    for ( j = 0; j < p->x; j++ )
      p->p[i*p->x+j] = PNM_GET1(data[j]);
  }
  pnm_freerow(data);
#endif
  fclose(fp);

  p->bpp = 1;
}

#endif /* HAVE_PNM_H */

char read_char(FILE *f1){	// filter #-comments
  char c;
  int  m;
  for(m=0;;){
    c=fgetc(f1);
    if( feof(f1)   )		E0("read feof");
    if( ferror(f1) )		F0("read ferror");
    if( c == '#'  )		{ m = 1; continue; }
    if( m ==  0   )		return c;
    if( c == '\n' )		m = 0;
  }
}

void readpgm(char *name,pix *p,int vvv){	// raw-ppm
  char c1,c2,*pip;     int nx,ny,nc,mod,i,j,k;	// buffer
  FILE *f1;            unsigned char *pic;
  char buf[512];
  int lx,ly,dx;

  pip=NULL;
  if(name[0]=='-' && name[1]==0){
    f1=stdin;  /* is this correct ??? */
  } else {
    pip=testsuffix(name);
    if(!pip){
      f1=fopen(name,"rb"); if(!f1)F1("opening file %s",name);
    } else {
      sprintf(buf,"%s %s",pip,name); /* how to prevent OVL ? */
      if(vvv){ fprintf(stderr,"# popen( %s )\n",buf); }
#if defined(__USE_POSIX2)
      f1=popen(buf,"r");
#else
      F0("sorry, you must compile with __USE_POSIX2 to use pipes");
#endif
      if(!f1)F1("opening pipe %s",buf);
    }
  }
  c1=fgetc(f1); if(feof(f1))F0("read");
  c2=fgetc(f1); if(feof(f1))F0("read");
  if(c1!='P')F0("first byte should be P in PNM-files");
  if(c2!='5' && c2!='4' && c2!='6')// F0("no PNM-RAW"); // PGM-RAW,PBM-RAW,PPM-RAW
  if(c2!='1' && c2!='2' && c2!='3')F0("no PNM");
  nx=ny=nc=0; if( c2=='4' || c2=='1') nc=1;
  for(mod=0;((c2=='5' || c2=='2') && (mod&7)<6)
        ||  ((c2=='6' || c2=='3') && (mod&7)<6)		
        ||  ((c2=='4' || c2=='1') && (mod&7)<4);)		
  {						// mode: 0,2,4=[ |\t|\r|\n] 
  						//   1=nx 3=ny 5=nc 8-13=#rem
    c1=read_char(f1);				// frueher: # mod|=8
    if( (mod & 1)==0 )				// whitespaces
    if( !isspace(c1) ) mod++;
    if( (mod & 1)==1 ) {
      if( !isdigit(c1) ) {
        if( !isspace(c1) )F0("unexpected character");
        mod++; }
      else if(mod==1) nx=nx*10+c1-'0';
      else if(mod==3) ny=ny*10+c1-'0';
      else if(mod==5) nc=nc*10+c1-'0';
    }
  }
  if(nc>=256)F0("nc >= 256"); // realy RAW?
  if(vvv)
  fprintf(stderr,"# PNM P%c x=%d y=%d c=%d head=%ld",c2,nx,ny,nc,ftell(f1));
  if( c2=='4' && (nx&7)!=0 ){
    /* nx=(nx+7)&~7;*/ if(vvv)fprintf(stderr," PBM2PGM nx %d",(nx+7)&~7);
  }
  fflush(stdout);
  pic=(unsigned char *)malloc( nx*ny*((c2=='6')?3:1) );
  if(pic==NULL)F0("memory failed");				// no memory
  if( c2=='5' )
    if(ny!=(int)fread(pic,   nx,ny,f1))F0("read");	// read all lines
  if( c2=='2' )for(mod=j=i=0;i<nx*ny && !feof(f1);){		// not very clever
    c1=read_char(f1);
    if( !isdigit(c1) ) {  if( !isspace(c1) )F0("unexpected char");
      if(1&mod) { pic[i]=j*255/nc; i++; j=0; mod=0; }  }
    else { j=j*10+c1-'0'; mod=1; };
  }
  if( c2=='1' )for(mod=j=i=0,nc=255;i<nx*ny && !feof(f1);){	// PBM-ASCII 0001100
    c1=read_char(f1);
    if( isdigit(c1) ) { pic[i]=((c1=='0')?255:0); i++; }
    else if( !isspace(c1) )F0("unexpected char");
  }
  if( c2=='3' )for(mod=k=j=i=0;i<nx*ny*3 && !feof(f1);){
    c1=read_char(f1);
    if( !isdigit(c1) ) {  if( !isspace(c1) )F0("unexpected char");
      if(1&mod) { k+=j; if(mod==5){ pic[i]=k/3; i++; }
        j=0; mod=(mod+1)%6; }  }
    else { j=j*10+c1-'0'; if(!(mod&1)) mod++; };
  }
  if( c2=='6' ){
    if(ny!=(int)fread(pic, 3*nx,ny,f1))F0("read");	// read all lines
    for(ly=0;ly<ny;ly++)
    for(lx=0;lx<nx;lx++)
    pic[lx+ly*nx]=( pic[0+3*lx+3*ly*nx]
                   +pic[1+3*lx+3*ly*nx]
                   +pic[2+3*lx+3*ly*nx] ) / 3;
  }
  if( c2=='4' ){				// PBM-RAW
    dx=(nx+7)&~7;				// dx (mod 8)
    if(ny!=(int)fread(pic,dx>>3,ny,f1))F0("read");	// read all bytes
    for(ly=ny-1;ly>=0;ly--)
    for(lx=nx-1;lx>=0;lx--)
    pic[lx+ly*nx]=( (128 & pic[(lx+ly*dx)>>3]<<(lx & 7)) ? 0 : 255 );
#if 0
    // using gray values for fuzzy logic
    for(ly=ny-2;ly>0;ly--)
    for(lx=nx-2;lx>0;lx--){				// takes time!
      pic[lx+ly*nx]=
            ((pic[ lx   + ly   *nx]>128)?192:64)	// reduce contrast
           +((pic[(lx+1)+(ly  )*nx]>128)?  8:-8)	// max 4*8+4*4=48;
           +((pic[(lx-1)+(ly  )*nx]>128)?  8:-8)
           +((pic[(lx  )+(ly+1)*nx]>128)?  8:-8)
           +((pic[(lx  )+(ly-1)*nx]>128)?  8:-8)
           +((pic[(lx+1)+(ly+1)*nx]>128)?  4:-4)
           +((pic[(lx-1)+(ly+1)*nx]>128)?  4:-4)
           +((pic[(lx+1)+(ly-1)*nx]>128)?  4:-4)
           +((pic[(lx-1)+(ly-1)*nx]>128)?  4:-4);
    }
#endif
    nc=255;
  }
  if(name[0]=='-' && name[1]==0){
    f1=NULL;
  } else {
    if(!pip) fclose(f1);
#if defined(__USE_POSIX2)
    else     pclose(f1);	/* close pipe (Jul00) */
#endif
  }
  p->p=pic;  p->x=nx;  p->y=ny; p->bpp=1;
  if(vvv)fprintf(stderr,"\n");
}

#ifdef HAVE_PNM_H
int writepgmfile(FILE *fp, pix *p) {
  int i, j;
  gray *data;

  pgm_writepgminit(fp, p->x, p->y, PGM_MAXMAXVAL, 0);
  data = pgm_allocrow(p->x);
  for (i = 0; i < p->y; i++) {
    for (j = 0; j < p->x; j++) 
      data[j] = p->p[i*p->x+j];
    pgm_writepgmrow(fp, data, p->x, PGM_MAXMAXVAL, 0);
  }
  pgm_freerow(data);
  return 0;
}

int writepgm(char *nam, pix *p) {
  FILE *fp;

  fp = fopen(nam, "wb");
  if (!fp)
    F0("open");			// open-error
  writepgmfile(fp, p);
  fclose(fp);
  return 0;
}

int writeppm(char *nam, pix *p) {
  FILE *fp;char buf[1024];
  
  /* unsecure because possible buf overflow */
  sprintf(buf,"pgmtoppm rgbi:/1.0/1.0/1.0 >%s",nam);
  fp=popen(buf,"w");if(!fp)F0("open");
  writepgmfile(fp, p);
  pclose(fp);
  return 0;
}

int writepbm(char *nam, pix *p) {
  FILE *fp;char buf[1024];
  
  /* unsecure because possible buf overflow */
  sprintf(buf,"pgmtopbm -threshold -value 0.5 >%s",nam);
  fp=popen(buf,"w");if(!fp)F0("open");
  writepgmfile(fp, p);
  pclose(fp);
  return 0;
}
#else
int writepgm(char *nam,pix *p){// P5 raw-pgm
  FILE *f1;int a,x,y;
  f1=fopen(nam,"wb");if(!f1)F0("open");		// open-error
  fprintf(f1,"P5\n%d %d\n255\n",p->x,p->y);
  if(p->bpp==3)
  for(y=0;y<p->y;y++)
  for(x=0;x<p->x;x++){	// set bit
    a=x+y*p->x;
    p->p[a]=(p->p[3*a+0]+p->p[3*a+1]+p->p[3*a+2])/3;
  }
  if(p->y!=(int)fwrite(p->p,p->x,p->y,f1))F0("write");	// write all lines
  fclose(f1);
  return 0;
}

// pgm2ppm, use last 3 bits for farbcoding 
int writeppm(char *nam,pix *p){// P6 raw-ppm
  FILE *f1;int x,y; unsigned char rgb[3];
  f1=fopen(nam,"wb");if(!f1)F0("open");		// open-error
  fprintf(f1,"P6\n%d %d\n255\n",p->x,p->y);
  if( p->bpp==1 )
  for(y=0;y<p->y;y++)
  for(x=0;x<p->x;x++){
    rgb[0]=rgb[1]=rgb[2]=p->p[x+y*p->x];
    if(p->p[x+y*p->x]&4) rgb[0]=64;
    if(p->p[x+y*p->x]&2) rgb[1]=64;
    if(p->p[x+y*p->x]&1) rgb[2]=64;
    if( 1!=(int)fwrite(rgb,3,1,f1) )F0("write");
  }
  if( p->bpp==3 )
  if( p->y!=(int)fwrite(p->p,3*p->x,p->y,f1) )F0("write");
  fclose(f1);
  return 0;
}

// high bit = first, 
int writepbm(char *nam,pix *p){// P4 raw-pbm
  FILE *f1;int x,y,a,b,dx,i;
  dx=(p->x+7)&~7;	// enlarge to a factor of 8
  for(y=0;y<p->y;y++)
  for(x=0;x<p->x;x++){	// set bit
    a=(x+y*dx)>>3;b=7-(x&7);	// adress an bitisnumber
    i=x+y*p->x;
    if(p->bpp==3) i=(p->p[3*i+0]+p->p[3*i+1]+p->p[3*i+2])/3;
    else         i= p->p[  i  ];
    i=((i>127)?0:1);
    p->p[a]=(p->p[a] & (~1<<b)) | i<<b;
  }
  f1=fopen(nam,"wb");if(!f1)F0("open");		// open-error
  fprintf(f1,"P4\n%d %d\n",p->x,p->y);
  if(p->y!=(int)fwrite(p->p,dx>>3,p->y,f1))F0("write");	// write all lines
  fclose(f1);
  return 0;
}
// ------------------------------------------------------------------------
#endif
