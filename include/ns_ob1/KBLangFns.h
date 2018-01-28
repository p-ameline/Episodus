/*
**
** KBLangFns.h
**
*/

/*
** This file will be #included by KBLangLexer.l, KBLangParser.y, and KBLangFns.c
** The first two are K&R c, the third c++
*/

#ifndef _parse_h
# define _parse_h 1

# include <stdio.h>		/* For definition of FILE , used in LoadKB */
# include <stdlib.h>

int		freeStr() ;		/* LSB: no args for cc, args for Symantec C++ */

int		yyerror(char *s) ;	/* Defined in KBLangParser.y */
int		yylex() ;		/* Defined in KBLangLexer.c */
int		yylook() ;		/* Defined in KBLangLexer.c */
int		yyparse() ;		/* Defined in KBLangParser.c */

extern "C"
{
  int		yywrap() ;		/* Defined in KBLangLexer.c */
}

int		isatty(int i) ;		/* Defined in KBLangLexer.c */
extern char	*yytext ;		/* Defined in KBLangLexer.c */
extern int	lineCount ;		/* Defined in KBLangLexer.l */

/*
** These functions are defined in KBLangFns.C (a c++ file), but called from
** KBLangParser.y (a yacc file) and hence from KBLangParser.c
*/

char		*dupStr(char *s) ;
char		*unquote(char *s) ;
int		yyback(int *p, int m) ;
int		yyoutput(int c) ;
int		avpAction(char *attName, int atype, char *text) ;
int		classAction(char *className) ;
int		directSuperclassAction(char *ds) ;
int		instAction(char *instName, char *className) ;
int		kbAction(char *kbName) ;
int		ksAction(char *ksName, char *ctlp, char *tc, char *pc, char *oc, char *ac, char *cg) ;
int		linkAction(char *linkName, char *toObjectName) ;
int		relAction(char *ForwardRel, char *InverseRel, char *FromClass, char *ToClass) ;

#endif // _parse_h
