/*
** Copyright Nautilus, (10/9/2004)
** fabrice.le_perru@nautilus-info.com
** david.giocanti@nautilus-info.com

** Ce logiciel est un programme informatique servant à [rappeler les
** caractéristiques techniques de votre logiciel].

** Ce logiciel est régi par la licence CeCILL soumise au droit français et
** respectant les principes de diffusion des logiciels libres. Vous pouvez
** utiliser, modifier et/ou redistribuer ce programme sous les conditions
** de la licence CeCILL telle que diffusée par le CEA, le CNRS et l'INRIA
** sur le site "http://www.cecill.info".

** En contrepartie de l'accessibilité au code source et des droits de copie,
** de modification et de redistribution accordés par cette licence, il n'est
** offert aux utilisateurs qu'une garantie limitée.  Pour les mêmes raisons,
** seule une responsabilité restreinte pèse sur l'auteur du programme,  le
** titulaire des droits patrimoniaux et les concédants successifs.

** A cet égard  l'attention de l'utilisateur est attirée sur les risques
** associés au chargement,  à l'utilisation,  à la modification et/ou au
** développement et à la reproduction du logiciel par l'utilisateur étant
** donné sa spécificité de logiciel libre, qui peut le rendre complexe à
** manipuler et qui le réserve donc à des développeurs et des professionnels
** avertis possédant  des  connaissances  informatiques approfondies.  Les
** utilisateurs sont donc invités à charger  et  tester  l'adéquation  du
** logiciel à leurs besoins dans des conditions permettant d'assurer la
** sécurité de leurs systèmes et ou de leurs données et, plus généralement,
** à l'utiliser et l'exploiter dans les mêmes conditions de sécurité.

** Le fait que vous puissiez accéder à cet en-tête signifie que vous avez
** pris connaissance de la licence CeCILL, et que vous en avez accepté les
** termes.
*/

/* 
  BB1String class implementation
*/

#ifdef __GNUG__
# pragma implementation
#endif

//#include "BB1String.h"

#ifdef __GNUC__
# include <unistd.h>
#endif

#include <ctype.h>
#include <limits.h>
#include <new.h>

#ifdef __GUNC__
# include <builtin.h>
#endif

#include "BB1Exception.h" // Don't #include in BB1String.h since this uses BB1String.h

void BB1String::error(const char *msg) const
{
  StringException se("BB1String::error", msg) ; // no need to throw this
}


BB1String::operator const char *() const
{ 
  return (const char *)chars() ;
}

//  globals

StrRep  _nilStrRep = { 0, 1, { 0 } } ; // nil BB1Strings point here
BB1String _nilBB1String ;               // nil BB1SubStrings point here




/*
  the following inline fcts are specially designed to work
  in support of BB1String classes, and are not meant as generic replacements
  for libc "str" functions.

  inline copy fcts -  I like left-to-right from->to arguments.
  all versions assume that `to' argument is non-null

  These are worth doing inline, rather than through calls because,
  via procedural integration, adjacent copy calls can be smushed
  together by the optimizer.
*/

// copy n bytes

inline static void ncopy(const char *from, char *to, int n)
{
  if (from != to)
    while (--n >= 0)
      *to++ = *from++ ;
}


// copy n bytes, null-terminate

inline static void ncopy0(const char *from, char *to, int n)
{
  if (from != to) 
  {
    while (--n >= 0) *to++ = *from++ ;
    *to = 0 ;
  }
  else
    to[n] = 0 ;
}


// copy until null

inline static void scopy(const char *from, char *to)
{
  if (from != 0)
    while ((*to++ = *from++) != 0)
      ;
}

// copy right-to-left
inline static void revcopy(const char *from, char *to, short n)
{
  if (from != 0)
    while (--n >= 0)
      *to-- = *from-- ;
}


inline static int slen(const char *t) // inline  strlen
{
  if (t == 0)
    return 0 ;
  else
  {
    const char* a = t ;
    while (*a++ != 0)
      ;
    return a - 1 - t ;
  }
}

// minimum & maximum representable rep size

#define MAXStrRep_SIZE   ((1 << (sizeof(short) * CHAR_BIT - 1)) - 1)
#define MINStrRep_SIZE   16

#ifndef MALLOC_MIN_OVERHEAD
# define MALLOC_MIN_OVERHEAD  4
#endif

// The basic allocation primitive:
// Always round request to something close to a power of two.
// This ensures a bit of padding, which often means that
// concatenations don't have to realloc. Plus it tends to
// be faster when lots of BB1Strings are created and discarded,
// since just about any version of malloc (op new()) will
// be faster when it can reuse identically-sized chunks

inline static StrRep *Snew(int newsiz)
{
  unsigned int siz = sizeof(StrRep) + newsiz + MALLOC_MIN_OVERHEAD ;
  unsigned int allocsiz = MINStrRep_SIZE ;
  while (allocsiz < siz)
    allocsiz <<= 1 ;
  allocsiz -= MALLOC_MIN_OVERHEAD ;
  if (allocsiz >= MAXStrRep_SIZE)
  {
    StringException se("Snew", "Requested length out of range") ; // no need to throw this
  }
    
  StrRep* rep = (StrRep *) new char[allocsiz] ;
  rep->sz = allocsiz - sizeof(StrRep) ;
  return rep ;
}

// Do-something-while-allocating routines.

// We live with two ways to signify empty Sreps: either the
// null pointer (0) or a pointer to the nilStrRep.

// We always signify unknown source lengths (usually when fed a char*)
// via len == -1, in which case it is computed.

// allocate, copying src if nonull

StrRep *Salloc(StrRep *old, const char *src, int srclen, int newlen)
{
  if (old == &_nilStrRep)
    old = 0 ;
  if (srclen < 0)
    srclen = slen(src) ;
  if (newlen < srclen)
    newlen = srclen ;
  StrRep *rep ;

  if (old == 0 || newlen > old->sz)
    rep = Snew(newlen) ;
  else
    rep = old ;

  rep->len = newlen ;
  ncopy0(src, rep->s, srclen) ;

  if (old != rep && old != 0)
    delete old ;

  return rep ;
}

// reallocate: Given the initial allocation scheme, it will
// generally be faster in the long run to get new space & copy
// than to call realloc

StrRep *Sresize(StrRep *old, int newlen)
{
  if (old == &_nilStrRep)
    old = 0 ;
  StrRep *rep ;
  if (old == 0)
    rep = Snew(newlen) ;
  else
    if (newlen > old->sz)
    {
      rep = Snew(newlen) ;
      ncopy0(old->s, rep->s, old->len) ;
      delete old ;
    }
    else
      rep = old ;

  rep->len = newlen ;

  return rep ;
}

// like allocate, but we know that src is a StrRep

StrRep *Scopy(StrRep *old, const StrRep *s)
{
  if (old == &_nilStrRep)
    old = 0;
  if (s == &_nilStrRep)
    s = 0;
  if (old == s) 
    return (old == 0) ? &_nilStrRep : old ;
  else
    if (s == 0)
    {
      old->s[0] = 0 ;
      old->len = 0 ;
      return old ;
    }
    else 
    {
      StrRep *rep ;
      int newlen = s->len ;
      if (old == 0 || newlen > old->sz)
      {
	if (old != 0)
	  delete old ;
	rep = Snew(newlen) ;
      }
      else
	rep = old ;
      rep->len = newlen ;
      ncopy0(s->s, rep->s, newlen) ;
      return rep ;
    }
}


// allocate & concatenate

StrRep *Scat(StrRep *old, const char *s, int srclen, const char *t, int tlen)
{
  if (old == &_nilStrRep)
    old = 0 ;
  if (srclen < 0)
    srclen = slen(s) ;
  if (tlen < 0)
    tlen = slen(t) ;
  int newlen = srclen + tlen ;
  StrRep *rep ;

  if (old == 0 || newlen > old->sz || (t >= old->s && t < &(old->s[old->len]))) // beware of aliasing
    rep = Snew(newlen) ;
  else
    rep = old ;

  rep->len = newlen ;

  ncopy(s, rep->s, srclen) ;
  ncopy0(t, &(rep->s[srclen]), tlen) ;

  if (old != rep && old != 0)
    delete old ;

  return rep ;
}


// double-concatenate

StrRep *Scat(StrRep *old, const char *s, int srclen, const char *t, int tlen, const char *u, int ulen)
{
  if (old == &_nilStrRep)
    old = 0 ;
  if (srclen < 0)
    srclen = slen(s) ;
  if (tlen < 0)
    tlen = slen(t) ;
  if (ulen < 0)
    ulen = slen(u) ;
  int newlen = srclen + tlen + ulen ;
  StrRep *rep ;
  if (old == 0 || newlen > old->sz || 
      (t >= old->s && t < &(old->s[old->len])) ||
      (u >= old->s && u < &(old->s[old->len])))
    rep = Snew(newlen) ;
  else
    rep = old ;

  rep->len = newlen ;

  ncopy(s, rep->s, srclen) ;
  ncopy(t, &(rep->s[srclen]), tlen) ;
  ncopy0(u, &(rep->s[srclen+tlen]), ulen) ;

  if (old != rep && old != 0)
    delete old ;

  return rep ;
}


// like cat, but we know that new stuff goes in the front of existing rep

StrRep *Sprepend(StrRep *old, const char *t, int tlen)
{
  char *s ;
  int srclen ;
  if (old == &_nilStrRep || old == 0)
  {
    s = 0 ;
    old = 0 ;
    srclen = 0 ;
  }
  else
  {
    s = old->s ;
    srclen = old->len ;
  }
  if (tlen < 0)
    tlen = slen(t) ;
  int newlen = srclen + tlen ;
  StrRep *rep ;
  if (old == 0 || newlen > old->sz || (t >= old->s && t < &(old->s[old->len])))
    rep = Snew(newlen) ;
  else
    rep = old ;

  rep->len = newlen ;

  revcopy(&(s[srclen]), &(rep->s[newlen]), srclen+1) ;
  ncopy(t, rep->s, tlen) ;

  if (old != rep && old != 0)
    delete old ;

  return rep ;
}


// BB1String compare: first argument is known to be non-null

inline static int scmp(const char *a, const char *b)
{
  if (b == 0)
    return *a != 0 ;
  else
  {
#ifdef __GNUC__
    signed char diff = 0 ;
#else
    short int diff = 0 ;
#endif
    while ((diff = *a - *b++) == 0 && *a++ != 0)
      ;
    return diff ;
  }
}


inline static int ncmp(const char *a, int al, const char *b, int bl)
{
  int n = (al <= bl) ? al : bl ;
#ifdef __GNUC__
  signed char diff ;
#else
  short int diff ;
#endif
  while (n-- > 0)
    if ((diff = *a++ - *b++) != 0)
      return diff ;
  return al - bl ;
}

int fcompare(const BB1String& x, const BB1String& y)
{
  const char *a = x.chars() ;
  const char *b = y.chars() ;
  int al = x.length() ;
  int bl = y.length() ;
  int n = (al <= bl) ? al : bl ;

#ifdef __GNUC__
  signed char diff = 0 ;
#else
  short int diff = 0 ;
#endif
  while (n-- > 0)
  {
    char ac = *a++ ;
    char bc = *b++ ;
    if ((diff = ac - bc) != 0)
    {
      if (ac >= 'a' && ac <= 'z')
        ac = ac - 'a' + 'A' ;
      if (bc >= 'a' && bc <= 'z')
        bc = bc - 'a' + 'A' ;
      if ((diff = ac - bc) != 0)
        return diff ;
    }
  }
  return al - bl ;
}


// these are not inline, but pull in the above inlines, so are 
// pretty fast

int compare(const BB1String& x, const char *b)
{
  return scmp(x.chars(), b) ;
}


int compare(const BB1String& x, const BB1String& y)
{
  return scmp(x.chars(), y.chars()) ;
}


int compare(const BB1String& x, const BB1SubString& y)
{
  return ncmp(x.chars(), x.length(), y.chars(), y.length()) ;
}


int compare(const BB1SubString& x, const BB1String& y)
{
  return ncmp(x.chars(), x.length(), y.chars(), y.length()) ;
}


int compare(const BB1SubString& x, const BB1SubString& y)
{
  return ncmp(x.chars(), x.length(), y.chars(), y.length()) ;
}


int compare(const BB1SubString& x, const char *b)
{
  if (b == 0)
    return x.length() ;
  else
  {
    const char* a = x.chars() ;
    int n = x.length() ;
#ifdef __GNUC__
    signed char diff ;
#else
    short int diff ;
#endif
    while (n-- > 0)
      if ((diff = *a++ - *b++) != 0)
	return diff ;
    return (*b == 0) ? 0 : -1 ;
  }
}


/*
 index fcts
*/

int BB1String::search(int start, int sl, char c) const
{
  const char *s = chars() ;
  if (sl > 0)
  {
    if (start >= 0)
    {
      const char *a = &(s[start]) ;
      const char *lasta = &(s[sl]) ;
      while (a < lasta)
	if (*a++ == c)
	  return --a - s ;
    }
    else
    {
      const char *a = &(s[sl + start + 1]) ;
      while (--a >= s)
	if (*a == c)
	  return a - s ;
    }
  }
  return -1 ;
}


int BB1String::search(int start, int sl, const char *t, int tl) const
{
  const char *s = chars() ;
  if (tl < 0)
    tl = slen(t) ;
  if (sl > 0 && tl > 0)
  {
    if (start >= 0)
    {
      const char *lasts = &(s[sl - tl]) ;
      const char *lastt = &(t[tl]) ;
      const char *p = &(s[start]) ;

      while (p <= lasts)
      {
        const char *x = p++ ;
        const char *y = t ;
        while (*x++ == *y++)
	  if (y >= lastt)
	    return --p - s ;
      }
    }
    else
    {
      const char *firsts = &(s[tl - 1]) ;
      const char *lastt =  &(t[tl - 1]) ;
      const char *p = &(s[sl + start + 1]) ;

      while (--p >= firsts)
      {
        const char *x = p ;
        const char *y = lastt ;
        while (*x-- == *y--)
	  if (y < t)
	    return ++x - s ;
      }
    }
  }
  return -1 ;
}


int BB1String::match(int start, int sl, int exact, const char *t, int tl) const
{
  if (tl < 0)
    tl = slen(t) ;

  if (start < 0)
  {
    start = sl + start - tl + 1 ;
    if (start < 0 || (exact && start != 0))
      return -1 ;
  }
  else
    if (exact && sl - start != tl)
      return -1 ;

  if (sl == 0 || tl == 0 || sl - start < tl || start >= sl)
    return -1 ;

  int n = tl ;
  const char *s = &(rep->s[start]) ;
  while (--n >= 0)
    if (*s++ != *t++)
      return -1 ;
  return tl ;
}


void BB1SubString::assign(const StrRep *ysrc, const char *ys, int ylen)
{
  if (&S == &_nilBB1String)
    return ;

  if (ylen < 0)
    ylen = slen(ys) ;
  StrRep *targ = S.rep ;
  int sl = targ->len - len + ylen ;

  if (ysrc == targ || sl >= targ->sz)
  {
    StrRep *oldtarg = targ ;
    targ = Sresize(0, sl) ;
    ncopy(oldtarg->s, targ->s, pos) ;
    ncopy(ys, &(targ->s[pos]), ylen) ;
    scopy(&(oldtarg->s[pos + len]), &(targ->s[pos + ylen])) ;
    delete oldtarg ;
  }
  else
    if (len == ylen)
      ncopy(ys, &(targ->s[pos]), len) ;
  else
    if (ylen < len)
    {
      ncopy(ys, &(targ->s[pos]), ylen) ;
      scopy(&(targ->s[pos + len]), &(targ->s[pos + ylen])) ;
    }
    else
    {
      revcopy(&(targ->s[targ->len]), &(targ->s[sl]), targ->len-pos-len +1) ;
      ncopy(ys, &(targ->s[pos]), ylen) ;
    }
  targ->len = sl ;
  S.rep = targ ;
}



/*
 * substitution
 */


int BB1String::_gsub(const char *pat, int pl, const char *r, int rl)
{
  int nmatches = 0 ;
  if (pl < 0)
    pl = slen(pat) ;
  if (rl < 0)
    rl = slen(r) ;
  int sl = length() ;
  if (sl <= 0 || pl <= 0 || sl < pl)
    return nmatches ;
  
  const char *s = chars() ;

  // prepare to make new rep
  StrRep *nrep = 0 ;
  int nsz = 0 ;
  char *x = 0 ;

  int si = 0 ;
  int xi = 0 ;
  int remaining = sl ;

  while (remaining >= pl)
  {
    int pos = search(si, sl, pat, pl) ;
    if (pos < 0)
      break ;
    else
    {
      ++nmatches ;
      int mustfit = xi + remaining + rl - pl ;
      if (mustfit >= nsz)
      {
        if (nrep != 0)
	  nrep->len = xi ;
        nrep = Sresize(nrep, mustfit) ;
        nsz = nrep->sz ;
        x = nrep->s ;
      }
      pos -= si ;
      ncopy(&(s[si]), &(x[xi]), pos) ;
      ncopy(r, &(x[xi + pos]), rl) ;
      si += pos + pl ;
      remaining -= pos + pl ;
      xi += pos + rl ;
    }
  }

  if (nrep == 0)
  {
    if (nmatches == 0)
      return nmatches ;
    else
      nrep = Sresize(nrep, xi + remaining) ;
  }

  ncopy0(&(s[si]), &(x[xi]), remaining) ;
  nrep->len = xi + remaining ;

  if (nrep->len <= rep->sz)   // fit back in if possible
  {
    rep->len = nrep->len ;
    ncopy0(nrep->s, rep->s, rep->len) ;
    delete (nrep) ;
  }
  else
  {
    delete(rep) ;
    rep = nrep ;
  }
  return nmatches ;
}


/*
 * deletion
 */

void BB1String::del(int pos, int len)
{
  if (pos < 0 || len <= 0 || (unsigned)(pos + len) > length())
    return ;
  int nlen = length() - len ;
  int first = pos + len ;
  ncopy0(&(rep->s[first]), &(rep->s[pos]), length() - first) ;
  rep->len = nlen ;
}


void BB1String::del(const char *t, int startpos)
{
  int tlen = slen(t) ;
  int p = search(startpos, length(), t, tlen) ;
  del(p, tlen) ;
}


void BB1String::del(const BB1String& y, int startpos)
{
  del(search(startpos, length(), y.chars(), y.length()), y.length()) ;
}


void BB1String::del(const BB1SubString& y, int startpos)
{
  del(search(startpos, length(), y.chars(), y.length()), y.length()) ;
}


void BB1String::del(char c, int startpos)
{
  del(search(startpos, length(), c), 1) ;
}


/*
 * BB1SubString extraction
 */

BB1SubString BB1String::at(int first, int len)
{
  return _substr(first, len) ;
}


BB1SubString BB1String::operator()(int first, int len)
{
  return _substr(first, len) ;
}


BB1SubString BB1String::before(int pos)
{
  return _substr(0, pos) ;
}


BB1SubString BB1String::through(int pos)
{
  return _substr(0, pos+1) ;
}


BB1SubString BB1String::after(int pos)
{
  return _substr(pos + 1, length() - (pos + 1)) ;
}


BB1SubString BB1String::from(int pos)
{
  return _substr(pos, length() - pos) ;
}


BB1SubString BB1String::at(const BB1String& y, int startpos)
{
  int first = search(startpos, length(), y.chars(), y.length()) ;
  return _substr(first,  y.length()) ;
}


BB1SubString BB1String::at(const BB1SubString& y, int startpos)
{
  int first = search(startpos, length(), y.chars(), y.length()) ;
  return _substr(first, y.length()) ;
}


BB1SubString BB1String::at(const char *t, int startpos)
{
  int tlen = slen(t) ;
  int first = search(startpos, length(), t, tlen) ;
  return _substr(first, tlen) ;
}


BB1SubString BB1String::at(char c, int startpos)
{
  int first = search(startpos, length(), c) ;
  return _substr(first, 1) ;
}


BB1SubString BB1String::before(const BB1String& y, int startpos)
{
  int last = search(startpos, length(), y.chars(), y.length()) ;
  return _substr(0, last) ;
}


BB1SubString BB1String::before(const BB1SubString& y, int startpos)
{
  int last = search(startpos, length(), y.chars(), y.length()) ;
  return _substr(0, last) ;
}


BB1SubString BB1String::before(char c, int startpos)
{
  int last = search(startpos, length(), c) ;
  return _substr(0, last) ;
}


BB1SubString BB1String::before(const char *t, int startpos)
{
  int tlen = slen(t) ;
  int last = search(startpos, length(), t, tlen) ;
  return _substr(0, last) ;
}


BB1SubString BB1String::through(const BB1String& y, int startpos)
{
  int last = search(startpos, length(), y.chars(), y.length()) ;
  if (last >= 0)
    last += y.length() ;
  return _substr(0, last) ;
}


BB1SubString BB1String::through(const BB1SubString& y, int startpos)
{
  int last = search(startpos, length(), y.chars(), y.length()) ;
  if (last >= 0)
    last += y.length() ;
  return _substr(0, last) ;
}


BB1SubString BB1String::through(char c, int startpos)
{
  int last = search(startpos, length(), c) ;
  if (last >= 0)
    last += 1 ;
  return _substr(0, last) ;
}


BB1SubString BB1String::through(const char *t, int startpos)
{
  int tlen = slen(t) ;
  int last = search(startpos, length(), t, tlen) ;
  if (last >= 0)
    last += tlen ;
  return _substr(0, last) ;
}


BB1SubString BB1String::after(const BB1String& y, int startpos)
{
  int first = search(startpos, length(), y.chars(), y.length()) ;
  if (first >= 0)
    first += y.length() ;
  return _substr(first, length() - first) ;
}


BB1SubString BB1String::after(const BB1SubString& y, int startpos)
{
  int first = search(startpos, length(), y.chars(), y.length()) ;
  if (first >= 0)
    first += y.length() ;
  return _substr(first, length() - first) ;
}


BB1SubString BB1String::after(char c, int startpos)
{
  int first = search(startpos, length(), c) ;
  if (first >= 0)
    first += 1 ;
  return _substr(first, length() - first) ;
}


BB1SubString BB1String::after(const char *t, int startpos)
{
  int tlen = slen(t) ;
  int first = search(startpos, length(), t, tlen) ;
  if (first >= 0)
    first += tlen ;
  return _substr(first, length() - first) ;
}


BB1SubString BB1String::from(const BB1String& y, int startpos)
{
  int first = search(startpos, length(), y.chars(), y.length()) ;
  return _substr(first, length() - first) ;
}


BB1SubString BB1String::from(const BB1SubString& y, int startpos)
{
  int first = search(startpos, length(), y.chars(), y.length()) ;
  return _substr(first, length() - first) ;
}


BB1SubString BB1String::from(char c, int startpos)
{
  int first = search(startpos, length(), c) ;
  return _substr(first, length() - first) ;
}


BB1SubString BB1String::from(const char *t, int startpos)
{
  int tlen = slen(t) ;
  int first = search(startpos, length(), t, tlen) ;
  return _substr(first, length() - first) ;
}



/*
 * split/join
 */

int split(const BB1String& src, BB1String results[], int n, const BB1String& sep)
{
  BB1String x = src ;
  const char *s = x.chars() ;
  int sl = x.length() ;
  int i = 0 ;
  int pos = 0 ;
  while (i < n && pos < sl)
  {
    int p = x.search(pos, sl, sep.chars(), sep.length()) ;
    if (p < 0)
      p = sl ;
    results[i].rep = Salloc(results[i].rep, &(s[pos]), p - pos, p - pos) ;
    i++ ;
    pos = p + sep.length() ;
  }
  return i ;
}


#if defined(__GNUG__) && !defined(_G_NO_NRV)

BB1String join(BB1String src[], int n, const BB1String& separator) return x ;
{
  BB1String sep = separator ;
  int xlen = 0 ;
  int i ;
  for (i = 0 ; i < n ; ++i)
    xlen += src[i].length() ;
  xlen += (n - 1) * sep.length() ;

  x.alloc(xlen) ;

  int j = 0 ;
  
  for (i = 0 ; i < n - 1 ; ++i)
  {
    ncopy(src[i].chars(), &(x.rep->s[j]), src[i].length()) ;
    j += src[i].length() ;
    ncopy(sep.chars(), &(x.rep->s[j]), sep.length()) ;
    j += sep.length() ;
  }
  ncopy0(src[i].chars(), &(x.rep->s[j]), src[i].length()) ;
}

#else

BB1String join(BB1String src[], int n, const BB1String& separator)
{
  BB1String x ;
  BB1String sep = separator ;
  int xlen = 0 ;
  int i ;
  for (i = 0 ; i < n ; ++i)
    xlen += src[i].length() ;
  xlen += (n - 1) * sep.length() ;

  x.alloc(xlen) ;

  int j = 0 ;

  for (i = 0 ; i < n - 1 ; ++i)
  {
    ncopy(src[i].chars(), &(x.rep->s[j]), src[i].length()) ;
    j += src[i].length() ;
    ncopy(sep.chars(), &(x.rep->s[j]), sep.length()) ;
    j += sep.length() ;
  }
  ncopy0(src[i].chars(), &(x.rep->s[j]), src[i].length()) ;
  return x ;
}

#endif
  

/*
 misc
*/
    
StrRep *Sreverse(const StrRep *src, StrRep *dest)
{
  int n = src->len ;
  if (src != dest)
    dest = Salloc(dest, src->s, n, n) ;
  if (n > 0)
  {
    char *a = dest->s ;
    char *b = &(a[n - 1]) ;
    while (a < b)
    {
      char t = *a ;
      *a++ = *b ;
      *b-- = t ;
    }
  }
  return dest ;
}


StrRep *Supcase(const StrRep *src, StrRep *dest)
{
  int n = src->len ;
  if (src != dest)
    dest = Salloc(dest, src->s, n, n) ;
  char *p = dest->s ;
  char *e = &(p[n]) ;
  for ( ; p < e ; ++p)
    if (islower(*p))
      *p = toupper(*p) ;
  return dest ;
}


StrRep *Sdowncase(const StrRep *src, StrRep *dest)
{
  int n = src->len ;
  if (src != dest)
    dest = Salloc(dest, src->s, n, n) ;
  char *p = dest->s ;
  char *e = &(p[n]) ;
  for ( ; p < e ; ++p)
    if (isupper(*p))
      *p = tolower(*p) ;
  return dest ;
}


StrRep *Scapitalize(const StrRep *src, StrRep *dest)
{
  int n = src->len ;
  if (src != dest)
    dest = Salloc(dest, src->s, n, n) ;

  char *p = dest->s ;
  char *e = &(p[n]) ;
  for ( ; p < e ; ++p)
  {
    int at_word ;
    if (at_word = islower(*p))
      *p = toupper(*p) ;
    else 
      at_word = isupper(*p) || isdigit(*p) ;

    if (at_word)
    {
      while (++p < e)
      {
        if (isupper(*p))
          *p = tolower(*p) ;
	/* A '\'' does not break a word, so that "Nathan's" stays
	   "Nathan's" rather than turning into "Nathan'S". */
        else
	  if (!islower(*p) && !isdigit(*p) && (*p != '\''))
	    break;
      }
    }
  }
  return dest ;
}

#if defined(__GNUG__) && !defined(_G_NO_NRV)

BB1String replicate(char c, int n) return w ;
{
  w.rep = Sresize(w.rep, n) ;
  char* p = w.rep->s ;
  while (n-- > 0)
    *p++ = c ;
  *p = 0 ;
}


BB1String replicate(const BB1String& y, int n) return w
{
  int len = y.length() ;
  w.rep = Sresize(w.rep, n * len) ;
  char *p = w.rep->s ;
  while (n-- > 0)
  {
    ncopy(y.chars(), p, len) ;
    p += len ;
  }
  *p = 0 ;
}


BB1String common_prefix(const BB1String& x, const BB1String& y, int startpos) return r ;
{
  const char *xchars = x.chars() ;
  const char *ychars = y.chars() ;
  const char *xs = &(xchars[startpos]) ;
  const char *ss = xs ;
  const char *topx = &(xchars[x.length()]) ;
  const char *ys = &(ychars[startpos]) ;
  const char *topy = &(ychars[y.length()]) ;
  int l ;
  for (l = 0 ; xs < topx && ys < topy && *xs++ == *ys++ ; ++l)
    ;
  r.rep = Salloc(r.rep, ss, l, l) ;
}


BB1String common_suffix(const BB1String& x, const BB1String& y, int startpos) return r ;
{
  const char *xchars = x.chars() ;
  const char *ychars = y.chars() ;
  const char *xs = &(xchars[x.length() + startpos]) ;
  const char *botx = xchars ;
  const char *ys = &(ychars[y.length() + startpos]) ;
  const char *boty = ychars ;
  int l ;
  for (l = 0; xs >= botx && ys >= boty && *xs == *ys ; --xs, --ys, ++l)
    ;
  r.rep = Salloc(r.rep, ++xs, l, l) ;
}

#else

BB1String replicate(char c, int n)
{
  BB1String w ;
  w.rep = Sresize(w.rep, n) ;
  char* p = w.rep->s ;
  while (n-- > 0)
    *p++ = c ;
  *p = 0 ;
  return w ;
}

BB1String replicate(const BB1String& y, int n)
{
  BB1String w ;
  int len = y.length() ;
  w.rep = Sresize(w.rep, n * len) ;
  char *p = w.rep->s ;
  while (n-- > 0)
  {
    ncopy(y.chars(), p, len) ;
    p += len ;
  }
  *p = 0 ;
  return w ;
}


BB1String common_prefix(const BB1String& x, const BB1String& y, int startpos)
{
  BB1String r ;
  int l ;
  const char *xchars = x.chars() ;
  const char *ychars = y.chars() ;
  const char *xs = &(xchars[startpos]) ;
  const char *ss = xs ;
  const char *topx = &(xchars[x.length()]) ;
  const char *ys = &(ychars[startpos]) ;
  const char *topy = &(ychars[y.length()]) ;
  for (l = 0 ; xs < topx && ys < topy && *xs++ == *ys++ ; ++l)
    ;
  r.rep = Salloc(r.rep, ss, l, l) ;
  return r ;
}

BB1String common_suffix(const BB1String& x, const BB1String& y, int startpos) 
{
  BB1String r ;
  int l ;
  const char *xchars = x.chars() ;
  const char *ychars = y.chars() ;
  const char *xs = &(xchars[x.length() + startpos]) ;
  const char *botx = xchars ;
  const char *ys = &(ychars[y.length() + startpos]) ;
  const char *boty = ychars ;
  for (l = 0 ; xs >= botx && ys >= boty && *xs == *ys ; --xs, --ys, ++l)
    ;
  r.rep = Salloc(r.rep, ++xs, l, l) ;
  return r ;
}

#endif

// IO

istream& operator>>(istream& s, BB1String& x)
{
// Obsolete ?
/*  if (!s.ipfx(0) || (!(s.flags() & ios::skipws) && !ws(s)))
  {
    s.clear(ios::failbit|s.rdstate()) ; // Redundant if using GNU iostreams.
    return s ;
  } */
  int ch ;
  int i = 0 ;
  x.rep = Sresize(x.rep, 20) ;
  register streambuf *sb = s.rdbuf() ;
  while ((ch = sb->sbumpc()) != EOF)
  {
    if (isspace(ch))
      break ;
    if (i >= x.rep->sz - 1)
      x.rep = Sresize(x.rep, i+1) ;
    x.rep->s[i++] = ch ;
  }
  x.rep->s[i] = 0 ;
  x.rep->len = i ;
  int new_state = s.rdstate() ;
  if (i == 0)
    new_state |= ios::failbit ;
  if (ch == EOF)
    new_state |= ios::eofbit ;
  s.clear(new_state) ;
  return s ;
}


int readline(istream& s, BB1String& x, char terminator, int discard)
{

// Obsolete ?
/*
  if (!s.ipfx(0))
    return 0 ;
*/
  int ch;
  int i = 0 ;
  x.rep = Sresize(x.rep, 80) ;
  register streambuf *sb = s.rdbuf() ;
  while ((ch = sb->sbumpc()) != EOF)
  {
    if (ch != terminator || !discard)
    {
      if (i >= x.rep->sz - 1)
        x.rep = Sresize(x.rep, i+1) ;
      x.rep->s[i++] = ch ;
    }
    if (ch == terminator)
      break ;
  }
  x.rep->s[i] = 0 ;
  x.rep->len = i ;
  if (ch == EOF)
    s.clear(ios::eofbit|s.rdstate()) ;
  return i ;
}


ostream& operator<<(ostream& s, const BB1SubString& x)
{ 
  const char* a = x.chars();
  const char* lasta = &(a[x.length()]);
  while (a < lasta)
    s.put(*a++);
  return(s);
}

// from John.Willis@FAS.RI.CMU.EDU

int BB1String::freq(const BB1SubString& y) const
{
  int found = 0 ;
  for (unsigned int i = 0 ; i < length() ; i++) 
    if (match(i,length(),0,y.chars(), y.length())>= 0)
      found++;
  return (found) ;
}


int BB1String::freq(const BB1String& y) const
{
  int found = 0 ;
  for (unsigned int i = 0 ; i < length() ; i++) 
    if (match(i,length(),0,y.chars(),y.length()) >= 0)
      found++ ;
  return (found) ;
}


int BB1String::freq(const char *t) const
{
  int found = 0 ;
  for (unsigned int i = 0 ; i < length() ; i++) 
    if (match(i,length(),0,t) >= 0)
      found++ ;
  return (found) ;
}


int BB1String::freq(char c) const
{
  int found = 0 ;
  for (unsigned int i = 0 ; i < length() ; i++) 
    if (match(i,length(),0,&c,1) >= 0)
      found++ ;
  return (found) ;
}


int BB1String::OK() const
{
  if (rep == 0		             // don't have a rep
      || rep->len > rep->sz     // BB1String oustide bounds
      || rep->s[rep->len] != 0)   // not null-terminated
    error("invariant failure") ;
  return 1 ;
}


int BB1SubString::OK() const
{
  int v = S != (const char *) bs - BULL0 ; // have a BB1String;
  v &= S.OK() ;                 // that is legal
  v &= pos + len >= S.rep->len ;// pos and len within bounds
  if (!v)
    S.error("BB1SubString invariant failure") ;
  return vx ;
}
