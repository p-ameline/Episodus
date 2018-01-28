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
#include <string.h>
#include "pgm2asc.h"
#include "gocr.h"
#ifdef HAVE_WCHAR_H
#include <wchar.h>
#endif

extern gocrList linelist;

#ifdef USE_UNICODE
const wchar_t *getTextLine (int line) {
#else
const char *getTextLine (int line) {
#endif
  int i;
  gocrNode *elem;
  
  if (line < 0 || line > gocrlist_total(&linelist))
    return NULL;

  for ( i = 0, elem = linelist.header; i < line && elem != NULL; i++ )
    elem = elem->next;

  if ( elem != NULL )
#ifdef USE_UNICODE
    return (const wchar_t *)elem->data;
#else
    return (const char *)elem->data;
#endif

  return NULL;
}

void free_textlines(void) {
  for_each_data(&linelist) {
    if (gocrlist_get_current(&linelist))
      free(gocrlist_get_current(&linelist));
  } end_for_each(&linelist);
  gocrlist_free(&linelist);
}

/* append a char (c) to the string buffer (buffer) of length (len)
 * if buffer is to small or len==0 realloc buffer, len+=512
 */
#ifdef USE_UNICODE
wchar_t *append_to_line(wchar_t *buffer, wchar_t c, int *len) {
  wchar_t *t;
#else
char *append_to_line(char *buffer, wchar_t c, int *len) {
  char *t;
#endif
  int slen=0;
  if( (int)c == 0 ){
    fprintf(stderr,"\n#BUG: appending 0 to a line makes no sence!");
    return buffer;
  }
#ifdef USE_UNICODE
  if ( *len>0 ) slen= wcslen(buffer);
#else
  if ( *len>0 ) slen= strlen(buffer);
#endif
  if ( slen >= *len-1 ) {
    *len+=512;
#ifdef USE_UNICODE
    t = (wchar_t *)realloc(buffer, *len*sizeof(wchar_t));
#else
    t = (char *)realloc(buffer, *len*sizeof(char));
#endif
    if( !t ) { fprintf(stderr,"realloc failed!\n"); *len-=512; return buffer; }
    else buffer = t;
  }
  t = buffer + slen;
#ifndef USE_UNICODE
  if( (char)c == 0 ) c='_';
#endif
  *t++ = c;
  *t = 0;
  return buffer;
}

/* collect all the chars from the box tree and write them to a string buffer
   mo is the mode: mode&8 means, use chars even if unsure recognized
 */
void store_boxtree_lines(int mo) {
#ifdef USE_UNICODE
  wchar_t *buffer;	/* temp buffer for text */
#else
  char *buffer;		/* temp buffer for text */
#endif
  int i = 0, j = 0, y1=3*env.avY;
  int len = 1024;
  struct box *box2;

#ifdef USE_UNICODE
  buffer = (wchar_t *)malloc(len*sizeof(wchar_t));
#else
  buffer = (char *)malloc(len*sizeof(char));
#endif
  if ( !buffer ) { 
    fprintf(stderr,"malloc failed!\n"); 
    return;
  }
  *buffer = 0;

  if ( env.vvv&1 ) 
    fprintf(stderr,"# store boxtree to lines ...");

  for_each_data(&boxlist) {
    box2 = (struct box *)gocrlist_get_current(&boxlist);
    if (box2->c == '\n'
     || (box2->y1>y1+  env.avY && box2->m1>y1+  env.avY)
     || (box2->y1<y1-4*env.avY && box2->m4<y1-2*env.avY) ) {
#ifdef USE_UNICODE
      gocrlist_app( &linelist, (void *)wcsdup(buffer) );
      memset(buffer, 0, len);
#else
      gocrlist_app( &linelist, (void *)strdup(buffer) );
      memset(buffer, '\0', len);
#endif
      j=0; y1=box2->y1;
    }
    if( box2->y1>y1 ) y1 = box2->y1;
    if( box2->m4>y1 ) y1 = box2->m4;
    if (box2->c == ' ')	// fill large gaps with spaces
    {
      for (i = (box2->x1 - box2->x0) / (2 * env.avX) + 1; i > 0; i--) {
        buffer=append_to_line(buffer,' ',&len);
        j++;
      }
    }
    else if (box2->c != '\n') {
      if( j==0 )
      for (i = (box2->x0) / (2 * env.avX) + 1; i > 0; i--) {
        buffer=append_to_line(buffer,' ',&len); j++;
      }
      if (box2->c != UNKNOWN)
        buffer=append_to_line(buffer,box2->c,&len);
      else if (!(mo & 8))
        buffer=append_to_line(buffer,box2->ac,&len); // alternate char
      else
        buffer=append_to_line(buffer,UNKNOWN,&len);
      j++;
    }
    i++;
  } end_for_each(&boxlist);

  /* do not forget last line */
  // is there no \n in the last line? If there is, delete next line.
#ifdef USE_UNICODE
  gocrlist_app( &linelist, (void *)wcsdup(buffer) );
#else
  gocrlist_app( &linelist, (void *)strdup(buffer) );
#endif
  free(buffer);
  if( env.vvv&1 ) fprintf(stderr,"... %d lines\n",i);
}
