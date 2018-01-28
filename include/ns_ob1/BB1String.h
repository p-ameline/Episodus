/*
**
** BB1String.h
**
*/

// This may look like C code, but it is really -*- C++ -*-
/* 
Copyright (C) 1988 Free Software Foundation
    written by Doug Lea (dl@rocky.oswego.edu)

This file is part of the GNU C++ Library.  This library is free
software; you can redistribute it and/or modify it under the terms of
the GNU Library General Public License as published by the Free
Software Foundation; either version 2 of the License, or (at your
option) any later version.  This library is distributed in the hope
that it will be useful, but WITHOUT ANY WARRANTY; without even the
implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the GNU Library General Public License for more details.
You should have received a copy of the GNU Library General Public
License along with this library; if not, write to the Free Software
Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
*/


#ifndef _BB1String_h
# define _BB1String_h 1

# ifdef __GNUC__
#  pragma interface
# endif

# include <iostream.h> // for operator<<

struct StrRep                     // internal BB1String representations
{
  unsigned short    len;         // BB1String length 
  unsigned short    sz;          // allocated space
  char              s[1];        // the BB1String starts here 
                                 // (at least 1 char for trailing null)
                                 // allocated & expanded via non-public fcts
};

// primitive ops on StrReps -- nearly all BB1String fns go through these.

StrRep*     Salloc(StrRep*, const char*, int, int);
StrRep*     Scopy(StrRep*, const StrRep*);
StrRep*     Sresize(StrRep*, int);
StrRep*     Scat(StrRep*, const char*, int, const char*, int);
StrRep*     Scat(StrRep*, const char*, int,const char*,int, const char*,int);
StrRep*     Sprepend(StrRep*, const char*, int);
StrRep*     Sreverse(const StrRep*, StrRep*);
StrRep*     Supcase(const StrRep*, StrRep*);
StrRep*     Sdowncase(const StrRep*, StrRep*);
StrRep*     Scapitalize(const StrRep*, StrRep*);

// These classes need to be defined in the order given

class BB1String;
class BB1SubString;

class BB1SubString
{
  friend class      BB1String;
protected:

  BB1String&		S;        // The BB1String I'm a BB1SubString of
  unsigned short    pos;      // starting position in S's rep
  unsigned short    len;      // length of BB1SubString

  void              assign(const StrRep*, const char*, int = -1);
                    BB1SubString(BB1String& x, int p, int l);
                    BB1SubString(const BB1SubString& x);

public:

// Note there are no public constructors. BB1SubStrings are always
// created via BB1String operations

                   ~BB1SubString();

  BB1SubString&        operator =  (const BB1String&     y);
  BB1SubString&        operator =  (const BB1SubString&  y);
  BB1SubString&        operator =  (const char* t);
  BB1SubString&        operator =  (char        c);

// return 1 if target appears anywhere in BB1SubString; else 0

  int               contains(char        c) const;
  int               contains(const BB1String&     y) const;
  int               contains(const BB1SubString&  y) const;
  int               contains(const char* t) const;

// IO 

  friend ostream&   operator<<(ostream& s, const BB1SubString& x);

// status

  unsigned int      length() const;
  int               empty() const;
  const char*       chars() const;

  int               OK() const; 

};


class BB1String
{
  friend class      BB1SubString;

protected:
  StrRep*           rep;   // BB1Strings are pointers to their representations

// some helper functions

  int               search(int, int, const char*, int = -1) const;
  int               search(int, int, char) const;
  int               match(int, int, int, const char*, int = -1) const;
  int               _gsub(const char*, int, const char* ,int);

public:
  BB1SubString         _substr(int, int); // temporary; to make ~cait/agent work

// constructors & assignment

                    BB1String();
                    BB1String(const BB1String& x);
                    BB1String(const BB1SubString&  x);
                    BB1String(const char* t);
                    BB1String(const char* t, int len);
                    BB1String(char c);

                    ~BB1String();

  BB1String&           operator =  (const BB1String&     y);
  BB1String&           operator =  (const char* y);
  BB1String&           operator =  (char        c);
  BB1String&           operator =  (const BB1SubString&  y);

// concatenation

  BB1String&           operator += (const BB1String&     y); 
  BB1String&           operator += (const BB1SubString&  y);
  BB1String&           operator += (const char* t);
  BB1String&           operator += (char        c);

  void              prepend(const BB1String&     y); 
  void              prepend(const BB1SubString&  y);
  void              prepend(const char* t);
  void              prepend(char        c);


// procedural versions:
// concatenate first 2 args, store result in last arg

  friend void     cat(const BB1String&, const BB1String&, BB1String&);
  friend void     cat(const BB1String&, const BB1SubString&, BB1String&);
  friend void     cat(const BB1String&, const char*, BB1String&);
  friend void     cat(const BB1String&, char, BB1String&);

  friend void     cat(const BB1SubString&, const BB1String&, BB1String&);
  friend void     cat(const BB1SubString&, const BB1SubString&, BB1String&);
  friend void     cat(const BB1SubString&, const char*, BB1String&);
  friend void     cat(const BB1SubString&, char, BB1String&);

  friend void     cat(const char*, const BB1String&, BB1String&);
  friend void     cat(const char*, const BB1SubString&, BB1String&);
  friend void     cat(const char*, const char*, BB1String&);
  friend void     cat(const char*, char, BB1String&);

// double concatenation, by request. (yes, there are too many versions, 
// but if one is supported, then the others should be too...)
// Concatenate first 3 args, store in last arg

  friend void     cat(const BB1String&,const BB1String&, const BB1String&,BB1String&);
  friend void     cat(const BB1String&,const BB1String&,const BB1SubString&,BB1String&);
  friend void     cat(const BB1String&,const BB1String&, const char*, BB1String&);
  friend void     cat(const BB1String&,const BB1String&, char, BB1String&);
  friend void     cat(const BB1String&,const BB1SubString&,const BB1String&,BB1String&);
  friend void     cat(const BB1String&,const BB1SubString&,const BB1SubString&,BB1String&);
  friend void     cat(const BB1String&,const BB1SubString&, const char*, BB1String&);
  friend void     cat(const BB1String&,const BB1SubString&, char, BB1String&);
  friend void     cat(const BB1String&,const char*, const BB1String&,    BB1String&);
  friend void     cat(const BB1String&,const char*, const BB1SubString&, BB1String&);
  friend void     cat(const BB1String&,const char*, const char*, BB1String&);
  friend void     cat(const BB1String&,const char*, char, BB1String&);

  friend void     cat(const char*, const BB1String&, const BB1String&,BB1String&);
  friend void     cat(const char*,const BB1String&,const BB1SubString&,BB1String&);
  friend void     cat(const char*,const BB1String&, const char*, BB1String&);
  friend void     cat(const char*,const BB1String&, char, BB1String&);
  friend void     cat(const char*,const BB1SubString&,const BB1String&,BB1String&);
  friend void     cat(const char*,const BB1SubString&,const BB1SubString&,BB1String&);
  friend void     cat(const char*,const BB1SubString&, const char*, BB1String&);
  friend void     cat(const char*,const BB1SubString&, char, BB1String&);
  friend void     cat(const char*,const char*, const BB1String&,    BB1String&);
  friend void     cat(const char*,const char*, const BB1SubString&, BB1String&);
  friend void     cat(const char*,const char*, const char*, BB1String&);
  friend void     cat(const char*,const char*, char, BB1String&);


// searching & matching

// return position of target in BB1String or -1 for failure

  int               index(char        c, int startpos = 0) const;      
  int               index(const BB1String&     y, int startpos = 0) const;      
  int               index(const BB1SubString&  y, int startpos = 0) const;      
  int               index(const char* t, int startpos = 0) const;  

// return 1 if target appears anyhere in BB1String; else 0

  int               contains(char        c) const;
  int               contains(const BB1String&     y) const;
  int               contains(const BB1SubString&  y) const;
  int               contains(const char* t) const;

// return 1 if target appears anywhere after position pos 
// (or before, if pos is negative) in BB1String; else 0

  int               contains(char        c, int pos) const;
  int               contains(const BB1String&     y, int pos) const;
  int               contains(const BB1SubString&  y, int pos) const;
  int               contains(const char* t, int pos) const;

// return 1 if target appears at position pos in BB1String; else 0

  int               matches(char        c, int pos = 0) const;
  int               matches(const BB1String&     y, int pos = 0) const;
  int               matches(const BB1SubString&  y, int pos = 0) const;
  int               matches(const char* t, int pos = 0) const;

//  return number of occurences of target in BB1String

  int               freq(char        c) const; 
  int               freq(const BB1String&     y) const;
  int               freq(const BB1SubString&  y) const;
  int               freq(const char* t) const;

// BB1SubString extraction

// Note that you can't take a BB1SubString of a const BB1String, since
// this leaves open the possiblility of indirectly modifying the
// BB1String through the BB1SubString

  BB1SubString         at(int         pos, int len);
  BB1SubString         operator () (int         pos, int len); // synonym for at

  BB1SubString         at(const BB1String&     x, int startpos = 0); 
  BB1SubString         at(const BB1SubString&  x, int startpos = 0); 
  BB1SubString         at(const char* t, int startpos = 0);
  BB1SubString         at(char        c, int startpos = 0);

  BB1SubString         before(int          pos);
  BB1SubString         before(const BB1String&      x, int startpos = 0);
  BB1SubString         before(const BB1SubString&   x, int startpos = 0);
  BB1SubString         before(const char*  t, int startpos = 0);
  BB1SubString         before(char         c, int startpos = 0);

  BB1SubString         through(int          pos);
  BB1SubString         through(const BB1String&      x, int startpos = 0);
  BB1SubString         through(const BB1SubString&   x, int startpos = 0);
  BB1SubString         through(const char*  t, int startpos = 0);
  BB1SubString         through(char         c, int startpos = 0);

  BB1SubString         from(int          pos);
  BB1SubString         from(const BB1String&      x, int startpos = 0);
  BB1SubString         from(const BB1SubString&   x, int startpos = 0);
  BB1SubString         from(const char*  t, int startpos = 0);
  BB1SubString         from(char         c, int startpos = 0);

  BB1SubString         after(int         pos);
  BB1SubString         after(const BB1String&     x, int startpos = 0);
  BB1SubString         after(const BB1SubString&  x, int startpos = 0);
  BB1SubString         after(const char* t, int startpos = 0);
  BB1SubString         after(char        c, int startpos = 0);


// deletion

// delete len chars starting at pos
  void              del(int         pos, int len);

// delete the first occurrence of target after startpos

  void              del(const BB1String&     y, int startpos = 0);
  void              del(const BB1SubString&  y, int startpos = 0);
  void              del(const char* t, int startpos = 0);
  void              del(char        c, int startpos = 0);

// global substitution: substitute all occurrences of pat with repl

  int               gsub(const BB1String&     pat, const BB1String&     repl);
  int               gsub(const BB1SubString&  pat, const BB1String&     repl);
  int               gsub(const char* pat, const BB1String&     repl);
  int               gsub(const char* pat, const char* repl);

// friends & utilities

// split BB1String into array res at separators; return number of elements

  friend int        split(const BB1String& x, BB1String res[], int maxn, 
                          const BB1String& sep);

  friend BB1String     common_prefix(const BB1String& x, const BB1String& y, 
                                  int startpos = 0);
  friend BB1String     common_suffix(const BB1String& x, const BB1String& y, 
                                  int startpos = -1);
  friend BB1String     replicate(char        c, int n);
  friend BB1String     replicate(const BB1String&     y, int n);
  friend BB1String     join(BB1String src[], int n, const BB1String& sep);

// simple builtin transformations

  friend BB1String     reverse(const BB1String& x);
  friend BB1String     upcase(const BB1String& x);
  friend BB1String     downcase(const BB1String& x);
  friend BB1String     capitalize(const BB1String& x);

// in-place versions of above

  void              reverse();
  void              upcase();
  void              downcase();
  void              capitalize();

// element extraction

  char&             operator [] (int i);
  char              elem(int i) const;
  char              firstchar() const;
  char              lastchar() const;

// conversion

                    operator const char*() const;
  const char*       chars() const;


// IO

  friend ostream&   operator<<(ostream& s, const BB1String& x);
  friend ostream&   operator<<(ostream& s, const BB1SubString& x);
  friend istream&   operator>>(istream& s, BB1String& x);

  friend int        readline(istream& s, BB1String& x, 
                             char terminator = '\n',
                             int discard_terminator = 1);

// status

  unsigned int      length() const;
  int               empty() const;

// preallocate some space for BB1String
  void              alloc(int newsize);

// report current allocation (not length!)

  int               allocation() const;


  void     error(const char* msg) const;

  int               OK() const;
};

typedef BB1String StrTmp; // for backward compatibility

// other externs

int        compare(const BB1String&    x, const BB1String&     y);
int        compare(const BB1String&    x, const BB1SubString&  y);
int        compare(const BB1String&    x, const char* y);
int        compare(const BB1SubString& x, const BB1String&     y);
int        compare(const BB1SubString& x, const BB1SubString&  y);
int        compare(const BB1SubString& x, const char* y);
int        fcompare(const BB1String&   x, const BB1String&     y); // ignore case

extern StrRep  _nilStrRep;
extern BB1String _nilBB1String;

// other inlines

inline BB1String operator + (const BB1String& x, const BB1String& y);
inline BB1String operator + (const BB1String& x, const BB1SubString& y);
inline BB1String operator + (const BB1String& x, const char* y);
inline BB1String operator + (const BB1String& x, char y);
inline BB1String operator + (const BB1SubString& x, const BB1String& y);
inline BB1String operator + (const BB1SubString& x, const BB1SubString& y);
inline BB1String operator + (const BB1SubString& x, const char* y);
inline BB1String operator + (const BB1SubString& x, char y);
inline BB1String operator + (const char* x, const BB1String& y);
inline BB1String operator + (const char* x, const BB1SubString& y);

inline int operator==(const BB1String& x, const BB1String& y); 
inline int operator!=(const BB1String& x, const BB1String& y);
inline int operator> (const BB1String& x, const BB1String& y);
inline int operator>=(const BB1String& x, const BB1String& y);
inline int operator< (const BB1String& x, const BB1String& y);
inline int operator<=(const BB1String& x, const BB1String& y);
inline int operator==(const BB1String& x, const BB1SubString&  y);
inline int operator!=(const BB1String& x, const BB1SubString&  y);
inline int operator> (const BB1String& x, const BB1SubString&  y);
inline int operator>=(const BB1String& x, const BB1SubString&  y);
inline int operator< (const BB1String& x, const BB1SubString&  y);
inline int operator<=(const BB1String& x, const BB1SubString&  y);
inline int operator==(const BB1String& x, const char* t);
inline int operator!=(const BB1String& x, const char* t);
inline int operator> (const BB1String& x, const char* t);
inline int operator>=(const BB1String& x, const char* t);
inline int operator< (const BB1String& x, const char* t);
inline int operator<=(const BB1String& x, const char* t);
inline int operator==(const BB1SubString& x, const BB1String& y);
inline int operator!=(const BB1SubString& x, const BB1String& y);
inline int operator> (const BB1SubString& x, const BB1String& y);
inline int operator>=(const BB1SubString& x, const BB1String& y);
inline int operator< (const BB1SubString& x, const BB1String& y);
inline int operator<=(const BB1SubString& x, const BB1String& y);
inline int operator==(const BB1SubString& x, const BB1SubString&  y);
inline int operator!=(const BB1SubString& x, const BB1SubString&  y);
inline int operator> (const BB1SubString& x, const BB1SubString&  y);
inline int operator>=(const BB1SubString& x, const BB1SubString&  y);
inline int operator< (const BB1SubString& x, const BB1SubString&  y);
inline int operator<=(const BB1SubString& x, const BB1SubString&  y);
inline int operator==(const BB1SubString& x, const char* t);
inline int operator!=(const BB1SubString& x, const char* t);
inline int operator> (const BB1SubString& x, const char* t);
inline int operator>=(const BB1SubString& x, const char* t);
inline int operator< (const BB1SubString& x, const char* t);
inline int operator<=(const BB1SubString& x, const char* t);


// status reports, needed before defining other things

inline unsigned int BB1String::length() const {  return rep->len; }
inline int         BB1String::empty() const { return rep->len == 0; }
inline const char* BB1String::chars() const { return &(rep->s[0]); }
inline int         BB1String::allocation() const { return rep->sz; }
inline void        BB1String::alloc(int newsize) { rep = Sresize(rep, newsize); }

inline unsigned int BB1SubString::length() const { return len; }
inline int         BB1SubString::empty() const { return len == 0; }
inline const char* BB1SubString::chars() const { return &(S.rep->s[pos]); }


// constructors

inline BB1String::BB1String() 
  : rep(&_nilStrRep) {}
inline BB1String::BB1String(const BB1String& x) 
  : rep(Scopy(0, x.rep)) {}
inline BB1String::BB1String(const char* t) 
  : rep(Salloc(0, t, -1, -1)) {}
inline BB1String::BB1String(const char* t, int tlen)
  : rep(Salloc(0, t, tlen, tlen)) {}
inline BB1String::BB1String(const BB1SubString& y)
  : rep(Salloc(0, y.chars(), y.length(), y.length())) {}
inline BB1String::BB1String(char c) 
  : rep(Salloc(0, &c, 1, 1)) {}

inline BB1String::~BB1String() { if (rep != &_nilStrRep) delete rep; }

inline BB1SubString::BB1SubString(const BB1SubString& x)
  :S(x.S), pos(x.pos), len(x.len) {}
inline BB1SubString::BB1SubString(BB1String& x, int first, int l)
  :S(x), pos(first), len(l) {}

inline BB1SubString::~BB1SubString() {}

// assignment

inline BB1String& BB1String::operator =  (const BB1String& y)
{ 
  rep = Scopy(rep, y.rep);
  return *this;
}

inline BB1String& BB1String::operator=(const char* t)
{
  rep = Salloc(rep, t, -1, -1);
  return *this;
}

inline BB1String& BB1String::operator=(const BB1SubString&  y)
{
  rep = Salloc(rep, y.chars(), y.length(), y.length());
  return *this;
}

inline BB1String& BB1String::operator=(char c)
{
  rep = Salloc(rep, &c, 1, 1);
  return *this;
}


inline BB1SubString& BB1SubString::operator = (const char* ys)
{
  assign(0, ys);
  return *this;
}

inline BB1SubString& BB1SubString::operator = (char ch)
{
  assign(0, &ch, 1);
  return *this;
}

inline BB1SubString& BB1SubString::operator = (const BB1String& y)
{
  assign(y.rep, y.chars(), y.length());
  return *this;
}

inline BB1SubString& BB1SubString::operator = (const BB1SubString& y)
{
  assign(y.S.rep, y.chars(), y.length());
  return *this;
}

// Zillions of cats...

inline void cat(const BB1String& x, const BB1String& y, BB1String& r)
{
  r.rep = Scat(r.rep, x.chars(), x.length(), y.chars(), y.length());
}

inline void cat(const BB1String& x, const BB1SubString& y, BB1String& r)
{
  r.rep = Scat(r.rep, x.chars(), x.length(), y.chars(), y.length());
}

inline void cat(const BB1String& x, const char* y, BB1String& r)
{
  r.rep = Scat(r.rep, x.chars(), x.length(), y, -1);
}

inline void cat(const BB1String& x, char y, BB1String& r)
{
  r.rep = Scat(r.rep, x.chars(), x.length(), &y, 1);
}

inline void cat(const BB1SubString& x, const BB1String& y, BB1String& r)
{
  r.rep = Scat(r.rep, x.chars(), x.length(), y.chars(), y.length());
}

inline void cat(const BB1SubString& x, const BB1SubString& y, BB1String& r)
{
  r.rep = Scat(r.rep, x.chars(), x.length(), y.chars(), y.length());
}

inline void cat(const BB1SubString& x, const char* y, BB1String& r)
{
  r.rep = Scat(r.rep, x.chars(), x.length(), y, -1);
}

inline void cat(const BB1SubString& x, char y, BB1String& r)
{
  r.rep = Scat(r.rep, x.chars(), x.length(), &y, 1);
}

inline void cat(const char* x, const BB1String& y, BB1String& r)
{
  r.rep = Scat(r.rep, x, -1, y.chars(), y.length());
}

inline void cat(const char* x, const BB1SubString& y, BB1String& r)
{
  r.rep = Scat(r.rep, x, -1, y.chars(), y.length());
}

inline void cat(const char* x, const char* y, BB1String& r)
{
  r.rep = Scat(r.rep, x, -1, y, -1);
}

inline void cat(const char* x, char y, BB1String& r)
{
  r.rep = Scat(r.rep, x, -1, &y, 1);
}

inline void cat(const BB1String& a, const BB1String& x, const BB1String& y, BB1String& r)
{
  r.rep = Scat(r.rep, a.chars(), a.length(), x.chars(), x.length(), y.chars(), y.length());
}

inline void cat(const BB1String& a, const BB1String& x, const BB1SubString& y, BB1String& r)
{
  r.rep = Scat(r.rep, a.chars(), a.length(), x.chars(), x.length(), y.chars(), y.length());
}

inline void cat(const BB1String& a, const BB1String& x, const char* y, BB1String& r)
{
  r.rep = Scat(r.rep, a.chars(), a.length(), x.chars(), x.length(), y, -1);
}

inline void cat(const BB1String& a, const BB1String& x, char y, BB1String& r)
{
  r.rep = Scat(r.rep, a.chars(), a.length(), x.chars(), x.length(), &y, 1);
}

inline void cat(const BB1String& a, const BB1SubString& x, const BB1String& y, BB1String& r)
{
  r.rep = Scat(r.rep, a.chars(), a.length(), x.chars(), x.length(), y.chars(), y.length());
}

inline void cat(const BB1String& a, const BB1SubString& x, const BB1SubString& y, BB1String& r)
{
  r.rep = Scat(r.rep, a.chars(), a.length(), x.chars(), x.length(), y.chars(), y.length());
}

inline void cat(const BB1String& a, const BB1SubString& x, const char* y, BB1String& r)
{
  r.rep = Scat(r.rep, a.chars(), a.length(), x.chars(), x.length(), y, -1);
}

inline void cat(const BB1String& a, const BB1SubString& x, char y, BB1String& r)
{
  r.rep = Scat(r.rep, a.chars(), a.length(), x.chars(), x.length(), &y, 1);
}

inline void cat(const BB1String& a, const char* x, const BB1String& y, BB1String& r)
{
  r.rep = Scat(r.rep, a.chars(), a.length(), x, -1, y.chars(), y.length());
}

inline void cat(const BB1String& a, const char* x, const BB1SubString& y, BB1String& r)
{
  r.rep = Scat(r.rep, a.chars(), a.length(), x, -1, y.chars(), y.length());
}

inline void cat(const BB1String& a, const char* x, const char* y, BB1String& r)
{
  r.rep = Scat(r.rep, a.chars(), a.length(), x, -1, y, -1);
}

inline void cat(const BB1String& a, const char* x, char y, BB1String& r)
{
  r.rep = Scat(r.rep, a.chars(), a.length(), x, -1, &y, 1);
}


inline void cat(const char* a, const BB1String& x, const BB1String& y, BB1String& r)
{
  r.rep = Scat(r.rep, a, -1, x.chars(), x.length(), y.chars(), y.length());
}

inline void cat(const char* a, const BB1String& x, const BB1SubString& y, BB1String& r)
{
  r.rep = Scat(r.rep, a, -1, x.chars(), x.length(), y.chars(), y.length());
}

inline void cat(const char* a, const BB1String& x, const char* y, BB1String& r)
{
  r.rep = Scat(r.rep, a, -1, x.chars(), x.length(), y, -1);
}

inline void cat(const char* a, const BB1String& x, char y, BB1String& r)
{
  r.rep = Scat(r.rep, a, -1, x.chars(), x.length(), &y, 1);
}

inline void cat(const char* a, const BB1SubString& x, const BB1String& y, BB1String& r)
{
  r.rep = Scat(r.rep, a, -1, x.chars(), x.length(), y.chars(), y.length());
}

inline void cat(const char* a, const BB1SubString& x, const BB1SubString& y, BB1String& r)
{
  r.rep = Scat(r.rep, a, -1, x.chars(), x.length(), y.chars(), y.length());
}

inline void cat(const char* a, const BB1SubString& x, const char* y, BB1String& r)
{
  r.rep = Scat(r.rep, a, -1, x.chars(), x.length(), y, -1);
}

inline void cat(const char* a, const BB1SubString& x, char y, BB1String& r)
{
  r.rep = Scat(r.rep, a, -1, x.chars(), x.length(), &y, 1);
}

inline void cat(const char* a, const char* x, const BB1String& y, BB1String& r)
{
  r.rep = Scat(r.rep, a, -1, x, -1, y.chars(), y.length());
}

inline void cat(const char* a, const char* x, const BB1SubString& y, BB1String& r)
{
  r.rep = Scat(r.rep, a, -1, x, -1, y.chars(), y.length());
}

inline void cat(const char* a, const char* x, const char* y, BB1String& r)
{
  r.rep = Scat(r.rep, a, -1, x, -1, y, -1);
}

inline void cat(const char* a, const char* x, char y, BB1String& r)
{
  r.rep = Scat(r.rep, a, -1, x, -1, &y, 1);
}


// operator versions

inline BB1String& BB1String::operator +=(const BB1String& y)
{
  cat(*this, y, *this);
  return *this;
}

inline BB1String& BB1String::operator +=(const BB1SubString& y)
{
  cat(*this, y, *this);
  return *this;
}

inline BB1String& BB1String::operator += (const char* y)
{
  cat(*this, y, *this);
  return *this;
}

inline BB1String& BB1String:: operator +=(char y)
{
  cat(*this, y, *this);
  return *this;
}

// constructive concatenation

#if defined(__GNUG__) && !defined(_G_NO_NRV)

inline BB1String operator + (const BB1String& x, const BB1String& y) return r;
{
  cat(x, y, r);
}

inline BB1String operator + (const BB1String& x, const BB1SubString& y) return r;
{
  cat(x, y, r);
}

inline BB1String operator + (const BB1String& x, const char* y) return r;
{
  cat(x, y, r);
}

inline BB1String operator + (const BB1String& x, char y) return r;
{
  cat(x, y, r);
}

inline BB1String operator + (const BB1SubString& x, const BB1String& y) return r;
{
  cat(x, y, r);
}

inline BB1String operator + (const BB1SubString& x, const BB1SubString& y) return r;
{
  cat(x, y, r);
}

inline BB1String operator + (const BB1SubString& x, const char* y) return r;
{
  cat(x, y, r);
}

inline BB1String operator + (const BB1SubString& x, char y) return r;
{
  cat(x, y, r);
}

inline BB1String operator + (const char* x, const BB1String& y) return r;
{
  cat(x, y, r);
}

inline BB1String operator + (const char* x, const BB1SubString& y) return r;
{
  cat(x, y, r);
}

inline BB1String reverse(const BB1String& x) return r;
{
  r.rep = Sreverse(x.rep, r.rep);
}

inline BB1String upcase(const BB1String& x) return r;
{
  r.rep = Supcase(x.rep, r.rep);
}

inline BB1String downcase(const BB1String& x) return r;
{
  r.rep = Sdowncase(x.rep, r.rep);
}

inline BB1String capitalize(const BB1String& x) return r;
{
  r.rep = Scapitalize(x.rep, r.rep);
}

#else /* NO_NRV */

inline BB1String operator + (const BB1String& x, const BB1String& y)
{
  BB1String r;  cat(x, y, r);  return r;
}

inline BB1String operator + (const BB1String& x, const BB1SubString& y) 
{
  BB1String r; cat(x, y, r); return r;
}

inline BB1String operator + (const BB1String& x, const char* y) 
{
  BB1String r; cat(x, y, r); return r;
}

inline BB1String operator + (const BB1String& x, char y) 
{
  BB1String r; cat(x, y, r); return r;
}

inline BB1String operator + (const BB1SubString& x, const BB1String& y) 
{
  BB1String r; cat(x, y, r); return r;
}

inline BB1String operator + (const BB1SubString& x, const BB1SubString& y) 
{
  BB1String r; cat(x, y, r); return r;
}

inline BB1String operator + (const BB1SubString& x, const char* y) 
{
  BB1String r; cat(x, y, r); return r;
}

inline BB1String operator + (const BB1SubString& x, char y) 
{
  BB1String r; cat(x, y, r); return r;
}

inline BB1String operator + (const char* x, const BB1String& y) 
{
  BB1String r; cat(x, y, r); return r;
}

inline BB1String operator + (const char* x, const BB1SubString& y) 
{
  BB1String r; cat(x, y, r); return r;
}

inline BB1String reverse(const BB1String& x) 
{
  BB1String r; r.rep = Sreverse(x.rep, r.rep); return r;
}

inline BB1String upcase(const BB1String& x) 
{
  BB1String r; r.rep = Supcase(x.rep, r.rep); return r;
}

inline BB1String downcase(const BB1String& x) 
{
  BB1String r; r.rep = Sdowncase(x.rep, r.rep); return r;
}

inline BB1String capitalize(const BB1String& x) 
{
  BB1String r; r.rep = Scapitalize(x.rep, r.rep); return r;
}

#endif

// prepend

inline void BB1String::prepend(const BB1String& y)
{
  rep = Sprepend(rep, y.chars(), y.length());
}

inline void BB1String::prepend(const char* y)
{
  rep = Sprepend(rep, y, -1); 
}

inline void BB1String::prepend(char y)
{
  rep = Sprepend(rep, &y, 1); 
}

inline void BB1String::prepend(const BB1SubString& y)
{
  rep = Sprepend(rep, y.chars(), y.length());
}

// misc transformations


inline void BB1String::reverse()
{
  rep = Sreverse(rep, rep);
}


inline void BB1String::upcase()
{
  rep = Supcase(rep, rep);
}


inline void BB1String::downcase()
{
  rep = Sdowncase(rep, rep);
}


inline void BB1String::capitalize()
{
  rep = Scapitalize(rep, rep);
}

// element extraction

inline char&  BB1String::operator [] (int i) 
{ 
  if (((unsigned)i) >= length()) error("invalid index");
  return rep->s[i];
}

inline char  BB1String::elem (int i) const
{ 
  if (((unsigned)i) >= length()) error("invalid index");
  return rep->s[i];
}

inline char  BB1String::firstchar() const
{ 
  return elem(0);
}

inline char  BB1String::lastchar() const
{ 
  return elem(length() - 1);
}

// searching

inline int BB1String::index(char c, int startpos) const
{
  return search(startpos, length(), c);
}

inline int BB1String::index(const char* t, int startpos) const
{   
  return search(startpos, length(), t);
}

inline int BB1String::index(const BB1String& y, int startpos) const
{   
  return search(startpos, length(), y.chars(), y.length());
}

inline int BB1String::index(const BB1SubString& y, int startpos) const
{   
  return search(startpos, length(), y.chars(), y.length());
}

inline int BB1String::contains(char c) const
{
  return search(0, length(), c) >= 0;
}

inline int BB1String::contains(const char* t) const
{   
  return search(0, length(), t) >= 0;
}

inline int BB1String::contains(const BB1String& y) const
{   
  return search(0, length(), y.chars(), y.length()) >= 0;
}

inline int BB1String::contains(const BB1SubString& y) const
{   
  return search(0, length(), y.chars(), y.length()) >= 0;
}

inline int BB1String::contains(char c, int p) const
{
  return match(p, length(), 0, &c, 1) >= 0;
}

inline int BB1String::contains(const char* t, int p) const
{
  return match(p, length(), 0, t) >= 0;
}

inline int BB1String::contains(const BB1String& y, int p) const
{
  return match(p, length(), 0, y.chars(), y.length()) >= 0;
}

inline int BB1String::contains(const BB1SubString& y, int p) const
{
  return match(p, length(), 0, y.chars(), y.length()) >= 0;
}


inline int BB1String::matches(const BB1SubString& y, int p) const
{
  return match(p, length(), 1, y.chars(), y.length()) >= 0;
}

inline int BB1String::matches(const BB1String& y, int p) const
{
  return match(p, length(), 1, y.chars(), y.length()) >= 0;
}

inline int BB1String::matches(const char* t, int p) const
{
  return match(p, length(), 1, t) >= 0;
}

inline int BB1String::matches(char c, int p) const
{
  return match(p, length(), 1, &c, 1) >= 0;
}


inline int BB1SubString::contains(const char* t) const
{   
  return S.search(pos, pos+len, t) >= 0;
}

inline int BB1SubString::contains(const BB1String& y) const
{   
  return S.search(pos, pos+len, y.chars(), y.length()) >= 0;
}

inline int BB1SubString::contains(const BB1SubString&  y) const
{   
  return S.search(pos, pos+len, y.chars(), y.length()) >= 0;
}

inline int BB1SubString::contains(char c) const
{
  return S.search(pos, pos+len, c) >= 0;
}


inline int BB1String::gsub(const BB1String& pat, const BB1String& r)
{
  return _gsub(pat.chars(), pat.length(), r.chars(), r.length());
}

inline int BB1String::gsub(const BB1SubString&  pat, const BB1String& r)
{
  return _gsub(pat.chars(), pat.length(), r.chars(), r.length());
}


inline int BB1String::gsub(const char* pat, const BB1String& r)
{
  return _gsub(pat, -1, r.chars(), r.length());
}

inline int BB1String::gsub(const char* pat, const char* r)
{
  return _gsub(pat, -1, r, -1);
}



inline  ostream& operator<<(ostream& s, const BB1String& x)
{
   s << x.chars(); return s;
}

// a zillion comparison operators

inline int operator==(const BB1String& x, const BB1String& y) 
{
  return compare(x, y) == 0; 
}

inline int operator!=(const BB1String& x, const BB1String& y)
{
  return compare(x, y) != 0; 
}

inline int operator>(const BB1String& x, const BB1String& y)
{
  return compare(x, y) > 0; 
}

inline int operator>=(const BB1String& x, const BB1String& y)
{
  return compare(x, y) >= 0; 
}

inline int operator<(const BB1String& x, const BB1String& y)
{
  return compare(x, y) < 0; 
}

inline int operator<=(const BB1String& x, const BB1String& y)
{
  return compare(x, y) <= 0; 
}

inline int operator==(const BB1String& x, const BB1SubString&  y) 
{
  return compare(x, y) == 0; 
}

inline int operator!=(const BB1String& x, const BB1SubString&  y)
{
  return compare(x, y) != 0; 
}

inline int operator>(const BB1String& x, const BB1SubString&  y)      
{
  return compare(x, y) > 0; 
}

inline int operator>=(const BB1String& x, const BB1SubString&  y)
{
  return compare(x, y) >= 0; 
}

inline int operator<(const BB1String& x, const BB1SubString&  y) 
{
  return compare(x, y) < 0; 
}

inline int operator<=(const BB1String& x, const BB1SubString&  y)
{
  return compare(x, y) <= 0; 
}

inline int operator==(const BB1String& x, const char* t) 
{
  return compare(x, t) == 0; 
}

inline int operator!=(const BB1String& x, const char* t) 
{
  return compare(x, t) != 0; 
}

inline int operator>(const BB1String& x, const char* t)  
{
  return compare(x, t) > 0; 
}

inline int operator>=(const BB1String& x, const char* t) 
{
  return compare(x, t) >= 0; 
}

inline int operator<(const BB1String& x, const char* t)  
{
  return compare(x, t) < 0; 
}

inline int operator<=(const BB1String& x, const char* t) 
{
  return compare(x, t) <= 0; 
}

inline int operator==(const BB1SubString& x, const BB1String& y) 
{
  return compare(y, x) == 0; 
}

inline int operator!=(const BB1SubString& x, const BB1String& y)
{
  return compare(y, x) != 0;
}

inline int operator>(const BB1SubString& x, const BB1String& y)      
{
  return compare(y, x) < 0;
}

inline int operator>=(const BB1SubString& x, const BB1String& y)     
{
  return compare(y, x) <= 0;
}

inline int operator<(const BB1SubString& x, const BB1String& y)      
{
  return compare(y, x) > 0;
}

inline int operator<=(const BB1SubString& x, const BB1String& y)     
{
  return compare(y, x) >= 0;
}

inline int operator==(const BB1SubString& x, const BB1SubString&  y) 
{
  return compare(x, y) == 0; 
}

inline int operator!=(const BB1SubString& x, const BB1SubString&  y)
{
  return compare(x, y) != 0;
}

inline int operator>(const BB1SubString& x, const BB1SubString&  y)      
{
  return compare(x, y) > 0;
}

inline int operator>=(const BB1SubString& x, const BB1SubString&  y)
{
  return compare(x, y) >= 0;
}

inline int operator<(const BB1SubString& x, const BB1SubString&  y) 
{
  return compare(x, y) < 0;
}

inline int operator<=(const BB1SubString& x, const BB1SubString&  y)
{
  return compare(x, y) <= 0;
}

inline int operator==(const BB1SubString& x, const char* t) 
{
  return compare(x, t) == 0; 
}

inline int operator!=(const BB1SubString& x, const char* t) 
{
  return compare(x, t) != 0;
}

inline int operator>(const BB1SubString& x, const char* t)  
{
  return compare(x, t) > 0; 
}

inline int operator>=(const BB1SubString& x, const char* t) 
{
  return compare(x, t) >= 0; 
}

inline int operator<(const BB1SubString& x, const char* t)  
{
  return compare(x, t) < 0; 
}

inline int operator<=(const BB1SubString& x, const char* t) 
{
  return compare(x, t) <= 0; 
}


// a helper needed by at, before, etc.

inline BB1SubString BB1String::_substr(int first, int l)
{
  if (first < 0 || (unsigned)(first + l) > length() )
    return BB1SubString(_nilBB1String, 0, 0) ;
  else 
    return BB1SubString(*this, first, l);
}

#endif
