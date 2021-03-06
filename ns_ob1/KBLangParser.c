
/*  A Bison parser, made from KBLangParser.y
    by GNU Bison version 1.28  */

#define YYBISON 1  /* Identify Bison output.  */

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

#line 1 "KBLangParser.y"

#include "KBLangFns.h"
/* Bison doesn't know about Win32 or MSVC */
#ifdef _MSC_VER
#include <malloc.h>
#endif

#line 9 "KBLangParser.y"
typedef union {
  char *name;
  int atype;
} YYSTYPE;
#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		98
#define	YYFLAG		-32768
#define	YYNTBASE	25

#define YYTRANSLATE(x) ((unsigned)(x) <= 276 ? yytranslate[x] : 47)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,    23,
    24,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     3,     4,     5,     6,
     7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
    17,    18,    19,    20,    21,    22
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     1,     4,     9,    16,    22,    27,    35,    43,    50,
    56,    69,    81,    92,   102,   104,   108,   109,   112,   116,
   117,   120,   126,   132,   138,   144,   150,   156,   162,   166,
   167,   170,   175,   177,   179,   181,   183,   185,   187,   189,
   191,   193,   195
};

static const short yyrhs[] = {    -1,
    26,    25,     0,    23,    17,    37,    24,     0,    23,    19,
    38,    28,    30,    24,     0,    23,    19,    38,    28,    24,
     0,    23,    19,    38,    24,     0,    23,    21,    41,    41,
    38,    38,    24,     0,    23,    20,    39,    38,    30,    33,
    24,     0,    23,    20,    39,    38,    30,    24,     0,    23,
    20,    39,    38,    24,     0,    23,    18,    39,    27,    42,
    43,    44,    45,    30,    33,    46,    24,     0,    23,    18,
    39,    27,    42,    43,    44,    45,    30,    33,    24,     0,
    23,    18,    39,    27,    42,    43,    44,    45,    30,    24,
     0,    23,    18,    39,    27,    42,    43,    44,    45,    24,
     0,     5,     0,    23,    29,    24,     0,     0,    38,    29,
     0,    23,    31,    24,     0,     0,    32,    31,     0,    23,
    40,    10,     6,    24,     0,    23,    40,    13,     8,    24,
     0,    23,    40,    11,     8,    24,     0,    23,    40,    12,
     7,    24,     0,    23,    40,    14,     7,    24,     0,    23,
    40,    15,     7,    24,     0,    23,    40,    16,     9,    24,
     0,    23,    34,    24,     0,     0,    35,    34,     0,    23,
    41,    36,    24,     0,     4,     0,     3,     0,     3,     0,
     3,     0,     3,     0,     3,     0,     3,     0,     3,     0,
     3,     0,     3,     0,     3,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
    54,    55,    58,    59,    60,    61,    62,    63,    64,    65,
    66,    67,    68,    69,    72,    75,    78,    79,    82,    85,
    86,    89,    90,    91,    92,    93,    94,    95,    98,   101,
   102,   105,   108,   111,   114,   117,   120,   123,   126,   129,
   132,   135,   138
};
#endif


#if YYDEBUG != 0 || defined (YYERROR_VERBOSE)

static const char * const yytname[] = {   "$","error","$undefined.","IDENTIFIER",
"LONG_NAME","BOOLVAL","CHARVAL","INTEGERVAL","REALVAL","STRINGVAL","CHARTYPE",
"FLOATTYPE","INTTYPE","DOUBLETYPE","LONGTYPE","SHORTTYPE","STRINGTYPE","KBKEY",
"KSKEY","CLASSKEY","INSTANCEKEY","RELATIONKEY","ENDOFFILE","'('","')'","kb",
"def","ctlp","dslist","dsseq","avplist","avpseq","avp","linklist","linkpairseq",
"linkpair","objname","kbname","classname","instname","attname","linkname","tc",
"pc","oc","ac","cg", NULL
};
#endif

static const short yyr1[] = {     0,
    25,    25,    26,    26,    26,    26,    26,    26,    26,    26,
    26,    26,    26,    26,    27,    28,    29,    29,    30,    31,
    31,    32,    32,    32,    32,    32,    32,    32,    33,    34,
    34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
    44,    45,    46
};

static const short yyr2[] = {     0,
     0,     2,     4,     6,     5,     4,     7,     7,     6,     5,
    12,    11,    10,     9,     1,     3,     0,     2,     3,     0,
     2,     5,     5,     5,     5,     5,     5,     5,     3,     0,
     2,     4,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1
};

static const short yydefact[] = {     1,
     0,     1,     0,     0,     0,     0,     0,     2,    34,     0,
    36,     0,    35,     0,     0,    38,     0,     3,    15,     0,
    17,     6,     0,     0,     0,    39,     0,     0,    17,    20,
     5,     0,    10,     0,     0,    40,     0,    16,    18,     0,
     0,    20,     4,    30,     9,     0,     0,    41,     0,    37,
     0,    19,    21,     0,     0,    30,     8,     7,    42,     0,
     0,     0,     0,     0,     0,     0,     0,     0,    29,    31,
    14,     0,     0,     0,     0,     0,     0,     0,     0,    33,
     0,    13,     0,    22,    24,    25,    23,    26,    27,    28,
    32,    43,    12,     0,    11,     0,     0,     0
};

static const short yydefgoto[] = {     8,
     2,    20,    23,    28,    32,    41,    42,    46,    55,    56,
    81,    10,    29,    12,    51,    17,    27,    37,    49,    60,
    94
};

static const short yypact[] = {    -3,
    -5,    -3,    14,    15,    16,    15,    33,-32768,-32768,    17,
-32768,    34,-32768,    -1,    16,-32768,    33,-32768,-32768,    37,
    16,-32768,     2,     4,    16,-32768,    39,    19,    16,    21,
-32768,    22,-32768,     6,    16,-32768,    42,-32768,-32768,    44,
    24,    21,-32768,    26,-32768,    27,    28,-32768,    47,-32768,
    -6,-32768,-32768,    33,    29,    26,-32768,-32768,-32768,     9,
    48,    49,    51,    52,    54,    55,    46,    59,-32768,-32768,
-32768,    11,    32,    35,    40,    41,    43,    45,    50,-32768,
    53,-32768,     0,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,    56,-32768,    66,    68,-32768
};

static const short yypgoto[] = {    70,
-32768,-32768,-32768,    57,   -22,    30,-32768,     1,    20,-32768,
-32768,-32768,    -4,    65,-32768,   -17,-32768,-32768,-32768,-32768,
-32768
};


#define	YYLAST		86


static const short yytable[] = {    25,
    14,    34,    92,    61,    62,    63,    64,    65,    66,    67,
    24,     3,     4,     5,     6,     7,     9,    11,    13,     1,
    35,    21,    22,    93,    30,    31,    30,    33,    44,    45,
    47,    30,    71,    44,    82,    16,    68,    72,    19,    26,
    18,    36,    38,    40,    48,    43,    50,    52,    54,    59,
    57,    58,    69,    73,    79,    84,    74,    75,    85,    76,
    77,    78,    80,    86,    87,    97,    88,    98,    89,    96,
    15,    53,    83,    90,     0,    70,    91,     0,     0,    95,
     0,     0,     0,     0,     0,    39
};

static const short yycheck[] = {    17,
     5,    24,     3,    10,    11,    12,    13,    14,    15,    16,
    15,    17,    18,    19,    20,    21,     3,     3,     3,    23,
    25,    23,    24,    24,    23,    24,    23,    24,    23,    24,
    35,    23,    24,    23,    24,     3,    54,    60,     5,     3,
    24,     3,    24,    23,     3,    24,     3,    24,    23,     3,
    24,    24,    24,     6,     9,    24,     8,     7,    24,     8,
     7,     7,     4,    24,    24,     0,    24,     0,    24,     0,
     6,    42,    72,    24,    -1,    56,    24,    -1,    -1,    24,
    -1,    -1,    -1,    -1,    -1,    29
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "/usr/lib/bison.simple"
/* This file comes from bison-1.28.  */

/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

#ifndef YYSTACK_USE_ALLOCA
#ifdef alloca
#define YYSTACK_USE_ALLOCA
#else /* alloca not defined */
#ifdef __GNUC__
#define YYSTACK_USE_ALLOCA
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi) || (defined (__sun) && defined (__i386))
#define YYSTACK_USE_ALLOCA
#include <alloca.h>
#else /* not sparc */
/* We think this test detects Watcom and Microsoft C.  */
/* This used to test MSDOS, but that is a bad idea
   since that symbol is in the user namespace.  */
#if (defined (_MSDOS) || defined (_MSDOS_)) && !defined (__TURBOC__)
#if 0 /* No need for malloc.h, which pollutes the namespace;
	 instead, just don't use alloca.  */
#include <malloc.h>
#endif
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
/* I don't know what this was needed for, but it pollutes the namespace.
   So I turned it off.   rms, 2 May 1997.  */
/* #include <malloc.h>  */
 #pragma alloca
#define YYSTACK_USE_ALLOCA
#else /* not MSDOS, or __TURBOC__, or _AIX */
#if 0
#ifdef __hpux /* haible@ilog.fr says this works for HPUX 9.05 and up,
		 and on HPUX 10.  Eventually we can turn this on.  */
#define YYSTACK_USE_ALLOCA
#define alloca __builtin_alloca
#endif /* __hpux */
#endif
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc */
#endif /* not GNU C */
#endif /* alloca not defined */
#endif /* YYSTACK_USE_ALLOCA not defined */

#ifdef YYSTACK_USE_ALLOCA
#define YYSTACK_ALLOC alloca
#else
#define YYSTACK_ALLOC malloc
#endif

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	goto yyacceptlab
#define YYABORT 	goto yyabortlab
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    { yychar = (token), yylval = (value);			\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, &yylloc, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval, &yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	yychar;			/*  the lookahead symbol		*/
YYSTYPE	yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int yynerrs;			/*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
int yydebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/* Define __yy_memcpy.  Note that the size argument
   should be passed with type unsigned int, because that is what the non-GCC
   definitions require.  With GCC, __builtin_memcpy takes an arg
   of type size_t, but it can handle unsigned int.  */

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __yy_memcpy(TO,FROM,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (to, from, count)
     char *to;
     char *from;
     unsigned int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (char *to, char *from, unsigned int count)
{
  register char *t = to;
  register char *f = from;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif

#line 217 "/usr/lib/bison.simple"

/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
#ifdef __cplusplus
#define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#else /* not __cplusplus */
#define YYPARSE_PARAM_ARG YYPARSE_PARAM
#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
#endif /* not __cplusplus */
#else /* not YYPARSE_PARAM */
#define YYPARSE_PARAM_ARG
#define YYPARSE_PARAM_DECL
#endif /* not YYPARSE_PARAM */

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
#ifdef YYPARSE_PARAM
int yyparse (void *);
#else
int yyparse (void);
#endif
#endif

int
yyparse(YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;
  int yyfree_stacks = 0;

#ifdef YYPURE
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int yylen;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Starting parse\n");
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror("parser stack overflow");
	  if (yyfree_stacks)
	    {
	      free (yyss);
	      free (yyvs);
#ifdef YYLSP_NEEDED
	      free (yyls);
#endif
	    }
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
#ifndef YYSTACK_USE_ALLOCA
      yyfree_stacks = 1;
#endif
      yyss = (short *) YYSTACK_ALLOC (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss, (char *)yyss1,
		   size * (unsigned int) sizeof (*yyssp));
      yyvs = (YYSTYPE *) YYSTACK_ALLOC (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs, (char *)yyvs1,
		   size * (unsigned int) sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) YYSTACK_ALLOC (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls, (char *)yyls1,
		   size * (unsigned int) sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  goto yybackup;
 yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Reading a token: ");
#endif
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
      if (yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 3:
#line 58 "KBLangParser.y"
{ kbAction(yyvsp[-1].name); ;
    break;}
case 4:
#line 59 "KBLangParser.y"
{ classAction(yyvsp[-3].name); ;
    break;}
case 5:
#line 60 "KBLangParser.y"
{ classAction(yyvsp[-2].name); ;
    break;}
case 6:
#line 61 "KBLangParser.y"
{ classAction(yyvsp[-1].name); ;
    break;}
case 7:
#line 62 "KBLangParser.y"
{ relAction(yyvsp[-4].name, yyvsp[-3].name, yyvsp[-2].name, yyvsp[-1].name); ;
    break;}
case 8:
#line 63 "KBLangParser.y"
{ instAction(yyvsp[-4].name, yyvsp[-3].name); ;
    break;}
case 9:
#line 64 "KBLangParser.y"
{ instAction(yyvsp[-3].name, yyvsp[-2].name); ;
    break;}
case 10:
#line 65 "KBLangParser.y"
{ instAction(yyvsp[-2].name, yyvsp[-1].name); ;
    break;}
case 11:
#line 66 "KBLangParser.y"
{ ksAction(yyvsp[-9].name, yyvsp[-8].name, yyvsp[-7].name, yyvsp[-6].name, yyvsp[-5].name, yyvsp[-4].name, yyvsp[-1].name); ;
    break;}
case 12:
#line 67 "KBLangParser.y"
{ ksAction(yyvsp[-8].name, yyvsp[-7].name, yyvsp[-6].name, yyvsp[-5].name, yyvsp[-4].name, yyvsp[-3].name, dupStr("NULLCG")); ;
    break;}
case 13:
#line 68 "KBLangParser.y"
{ ksAction(yyvsp[-7].name, yyvsp[-6].name, yyvsp[-5].name, yyvsp[-4].name, yyvsp[-3].name, yyvsp[-2].name, dupStr("NULLCG")); ;
    break;}
case 14:
#line 69 "KBLangParser.y"
{ ksAction(yyvsp[-6].name, yyvsp[-5].name, yyvsp[-4].name, yyvsp[-3].name, yyvsp[-2].name, yyvsp[-1].name, dupStr("NULLCG")); ;
    break;}
case 15:
#line 72 "KBLangParser.y"
{ yyval.name = yyvsp[0].name; ;
    break;}
case 16:
#line 75 "KBLangParser.y"
{ ;
    break;}
case 17:
#line 78 "KBLangParser.y"
{ ;
    break;}
case 18:
#line 79 "KBLangParser.y"
{ directSuperclassAction(yyvsp[-1].name); ;
    break;}
case 19:
#line 82 "KBLangParser.y"
{ ;
    break;}
case 20:
#line 85 "KBLangParser.y"
{ ;
    break;}
case 21:
#line 86 "KBLangParser.y"
{ ;
    break;}
case 22:
#line 89 "KBLangParser.y"
{ avpAction(yyvsp[-3].name, yyvsp[-2].atype, yyvsp[-1].name); ;
    break;}
case 23:
#line 90 "KBLangParser.y"
{ avpAction(yyvsp[-3].name, yyvsp[-2].atype, yyvsp[-1].name); ;
    break;}
case 24:
#line 91 "KBLangParser.y"
{ avpAction(yyvsp[-3].name, yyvsp[-2].atype, yyvsp[-1].name); ;
    break;}
case 25:
#line 92 "KBLangParser.y"
{ avpAction(yyvsp[-3].name, yyvsp[-2].atype, yyvsp[-1].name); ;
    break;}
case 26:
#line 93 "KBLangParser.y"
{ avpAction(yyvsp[-3].name, yyvsp[-2].atype, yyvsp[-1].name); ;
    break;}
case 27:
#line 94 "KBLangParser.y"
{ avpAction(yyvsp[-3].name, yyvsp[-2].atype, yyvsp[-1].name); ;
    break;}
case 28:
#line 95 "KBLangParser.y"
{ avpAction(yyvsp[-3].name, yyvsp[-2].atype, yyvsp[-1].name); ;
    break;}
case 29:
#line 98 "KBLangParser.y"
{ ;
    break;}
case 30:
#line 101 "KBLangParser.y"
{ ;
    break;}
case 31:
#line 102 "KBLangParser.y"
{ ;
    break;}
case 32:
#line 105 "KBLangParser.y"
{ linkAction(yyvsp[-2].name, yyvsp[-1].name); ;
    break;}
case 33:
#line 108 "KBLangParser.y"
{ yyval.name = yyvsp[0].name; ;
    break;}
case 34:
#line 111 "KBLangParser.y"
{ yyval.name = yyvsp[0].name; ;
    break;}
case 35:
#line 114 "KBLangParser.y"
{ yyval.name = yyvsp[0].name; ;
    break;}
case 36:
#line 117 "KBLangParser.y"
{ yyval.name = yyvsp[0].name; ;
    break;}
case 37:
#line 120 "KBLangParser.y"
{ yyval.name = yyvsp[0].name; ;
    break;}
case 38:
#line 123 "KBLangParser.y"
{ yyval.name = yyvsp[0].name; ;
    break;}
case 39:
#line 126 "KBLangParser.y"
{ yyval.name = yyvsp[0].name; ;
    break;}
case 40:
#line 129 "KBLangParser.y"
{ yyval.name = yyvsp[0].name; ;
    break;}
case 41:
#line 132 "KBLangParser.y"
{ yyval.name = yyvsp[0].name; ;
    break;}
case 42:
#line 135 "KBLangParser.y"
{ yyval.name = yyvsp[0].name; ;
    break;}
case 43:
#line 138 "KBLangParser.y"
{ yyval.name = yyvsp[0].name; ;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */
#line 543 "/usr/lib/bison.simple"

  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      yyerror(msg);
	      free(msg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror("parse error");
    }

  goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

yyerrhandle:

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;

 yyacceptlab:
  /* YYACCEPT comes here.  */
  if (yyfree_stacks)
    {
      free (yyss);
      free (yyvs);
#ifdef YYLSP_NEEDED
      free (yyls);
#endif
    }
  return 0;

 yyabortlab:
  /* YYABORT comes here.  */
  if (yyfree_stacks)
    {
      free (yyss);
      free (yyvs);
#ifdef YYLSP_NEEDED
      free (yyls);
#endif
    }
  return 1;
}
#line 141 "KBLangParser.y"


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

