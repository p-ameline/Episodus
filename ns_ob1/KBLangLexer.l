%{
char *dupStr(); /* Defined in KBLangFns.C */
char *unquote(); /* Defined in KBLangFns.C */
#include "KBLangParser.h"
#include "KBLangFns.h"
int lineCount = 1;
extern YYSTYPE yylval;
%}

id [a-zA-Z][a-zA-Z0-9]*

%s COMMENT_STATE

%%

^[ \t]*"/*"                          { BEGIN COMMENT_STATE; }
^[ \t]*"/*".*"*/"[ \t]*\n            { lineCount++; }
<COMMENT_STATE>"*/"[ \t]*\n          { lineCount++; BEGIN 0; }
<COMMENT_STATE>"*/"                  { BEGIN 0; }
<COMMENT_STATE>\n                    { lineCount++; }
<COMMENT_STATE>.\n                   { lineCount++; }
[ \t]                                { }
\n                                   { lineCount++; }
\(                                   { return('('); }
\)                                   { return(')'); }

KB                                   { return(KBKEY); }
KS                                   { return(KSKEY); }
CLASS                                { return(CLASSKEY); }
INSTANCE                             { return(INSTANCEKEY); }
RELATION                             { return(RELATIONKEY); }

char                                 { yylval.atype = CHARTYPE;   return(CHARTYPE); }
double                               { yylval.atype = DOUBLETYPE; return(DOUBLETYPE); }
float                                { yylval.atype = FLOATTYPE;  return(FLOATTYPE); }
int                                  { yylval.atype = INTTYPE;    return(INTTYPE); }
long                                 { yylval.atype = LONGTYPE;   return(LONGTYPE); }
short                                { yylval.atype = SHORTTYPE;  return(SHORTTYPE); }
String                               { yylval.atype = STRINGTYPE; return(STRINGTYPE); }

FALSE                                { yylval.name = dupStr("0"); return(BOOLVAL); }
TRUE                                 { yylval.name = dupStr("1"); return(BOOLVAL); }
{id}\.{id}                           { yylval.name = dupStr(yytext);
                                       return(LONG_NAME); }
{id}                                 { yylval.name = dupStr(yytext);
                                       return(IDENTIFIER); }

-?[0-9]+                             { yylval.name = dupStr(yytext); return(INTEGERVAL); }
-?([0-9]*\.[0-9]+([eE][-+]?[0-9]+)?) { yylval.name = dupStr(yytext); return(REALVAL); }
\'.\'                                { yylval.name = dupStr(yytext); return(CHARVAL); }
\'\\[nt0]\'                          { yylval.name = dupStr(yytext); return(CHARVAL); }
\"[^"\n]*\"                          { yylval.name = unquote(yytext);
                                       return(STRINGVAL); }
\"[^"\n]*\n                          { yylval.name = unquote(yytext); 
                                       lineCount++; return(STRINGVAL); }
.                                    { return(yytext[0]); }

%%

