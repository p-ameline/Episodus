%{
#include "KBLangFns.h"
/* Bison doesn't know about Win32 or MSVC */
#ifdef _MSC_VER
#include <malloc.h>
#endif
%}

%union {
  char *name;
  int atype;
}

%type <name> kbname
%type <name> classname
%type <name> instname
%type <name> attname
%type <name> linkname
%type <name> objname

%type <name> ctlp
%type <name> tc
%type <name> pc
%type <name> oc
%type <name> ac
%type <name> cg

%token <name> IDENTIFIER
%token <name> LONG_NAME

%token <name> BOOLVAL
%token <name> CHARVAL
%token <name> INTEGERVAL
%token <name> REALVAL
%token <name> STRINGVAL

%token <atype> CHARTYPE
%token <atype> FLOATTYPE
%token <atype> INTTYPE
%token <atype> DOUBLETYPE
%token <atype> LONGTYPE
%token <atype> SHORTTYPE
%token <atype> STRINGTYPE

%token KBKEY
%token KSKEY
%token CLASSKEY
%token INSTANCEKEY
%token RELATIONKEY
%token ENDOFFILE

%%

kb: /* nothing */
   | def kb 
   ;

def: '(' KBKEY kbname ')'                                 { kbAction($3); }
   | '(' CLASSKEY classname dslist avplist ')'            { classAction($3); }
   | '(' CLASSKEY classname dslist ')'                    { classAction($3); }
   | '(' CLASSKEY classname ')'                           { classAction($3); }
   | '(' RELATIONKEY linkname linkname classname classname ')' { relAction($3, $4, $5, $6); }
   | '(' INSTANCEKEY instname classname avplist linklist ')' { instAction($3, $4); }
   | '(' INSTANCEKEY instname classname avplist ')'          { instAction($3, $4); }
   | '(' INSTANCEKEY instname classname ')'                  { instAction($3, $4); }
   | '(' KSKEY instname ctlp tc pc oc ac avplist linklist cg ')' { ksAction($3, $4, $5, $6, $7, $8, $11); }
   | '(' KSKEY instname ctlp tc pc oc ac avplist linklist ')'    { ksAction($3, $4, $5, $6, $7, $8, dupStr("NULLCG")); }
   | '(' KSKEY instname ctlp tc pc oc ac avplist ')'             { ksAction($3, $4, $5, $6, $7, $8, dupStr("NULLCG")); }
   | '(' KSKEY instname ctlp tc pc oc ac ')'                     { ksAction($3, $4, $5, $6, $7, $8, dupStr("NULLCG")); }
   ;

ctlp: BOOLVAL                                             { $$ = $1; }
   ;

dslist: '(' dsseq ')'                                     { }
   ;

dsseq: /* nothing */                                      { }
   | classname dsseq                                      { directSuperclassAction($1); }
   ;

avplist: '(' avpseq ')'                                   { }
   ;

avpseq: /* nothing */                                     { }
   | avp avpseq                                           { }
   ;

avp: '(' attname CHARTYPE CHARVAL ')'                     { avpAction($2, $3, $4); }
   | '(' attname DOUBLETYPE REALVAL ')'                   { avpAction($2, $3, $4); }
   | '(' attname FLOATTYPE REALVAL ')'                    { avpAction($2, $3, $4); }
   | '(' attname INTTYPE  INTEGERVAL ')'                  { avpAction($2, $3, $4); }
   | '(' attname LONGTYPE INTEGERVAL ')'                  { avpAction($2, $3, $4); }
   | '(' attname SHORTTYPE INTEGERVAL ')'                 { avpAction($2, $3, $4); }
   | '(' attname STRINGTYPE STRINGVAL ')'                 { avpAction($2, $3, $4); }
   ;

linklist: '(' linkpairseq ')'                             { }
   ;

linkpairseq: /* nothing */                                { }
   | linkpair linkpairseq                                 { }
   ;

linkpair: '(' linkname objname ')'                        { linkAction($2, $3); }
   ;

objname: LONG_NAME                                        { $$ = $1; }
   ;

kbname: IDENTIFIER                                        { $$ = $1; }
   ;

classname: IDENTIFIER                                     { $$ = $1; }
   ;

instname: IDENTIFIER                                      { $$ = $1; }
   ;

attname: IDENTIFIER                                       { $$ = $1; }
   ;

linkname: IDENTIFIER                                      { $$ = $1; }
   ;

tc: IDENTIFIER                                            { $$ = $1; }
   ;

pc: IDENTIFIER                                            { $$ = $1; }
   ;

oc: IDENTIFIER                                            { $$ = $1; }
   ;

ac: IDENTIFIER                                            { $$ = $1; }
   ;

cg: IDENTIFIER                                            { $$ = $1; }
   ;

%%

int yyerror(char *s) {
  extern int lineCount;

  fprintf(stderr, "Load KB: %s", s);
  if (yytext) { fprintf(stderr, " %s", yytext); }
  fprintf(stderr, " on line %d\n", lineCount);
  return(0);
}

/* So that the applications don't have to use libl.a */

int yywrap() {
  return(1);
}

