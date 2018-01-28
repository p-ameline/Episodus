typedef union {
  char *name;
  int atype;
} YYSTYPE;
#define	IDENTIFIER	257
#define	LONG_NAME	258
#define	BOOLVAL	259
#define	CHARVAL	260
#define	INTEGERVAL	261
#define	REALVAL	262
#define	STRINGVAL	263
#define	CHARTYPE	264
#define	FLOATTYPE	265
#define	INTTYPE	266
#define	DOUBLETYPE	267
#define	LONGTYPE	268
#define	SHORTTYPE	269
#define	STRINGTYPE	270
#define	KBKEY	271
#define	KSKEY	272
#define	CLASSKEY	273
#define	INSTANCEKEY	274
#define	RELATIONKEY	275
#define	ENDOFFILE	276


extern YYSTYPE yylval;
