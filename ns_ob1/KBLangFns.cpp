#include <iostream.h>
#include <string.h>     // for dupStr(...)


#ifndef _OWLDLL
#include "AttValPair.h" // for KBLang::ResetAttSpec(...)
#include "BB1BB.h"
#include "BB1Class.h"   // for classAction(...)
#include "BB1KB.h"      // for instAction(...)
#include "BB1Link.h"

#include "KBLangParser.h"
#include "KBLangFns.h"
#else
# include "ns_obk\AttValPair.h" // for KBLang::ResetAttSpec(...)
# include "ns_obk\BB1BB.h"
# include "ns_obk\BB1Class.h"   // for classAction(...)
# include "ns_obk\BB1KB.h"      // for instAction(...)
# include "ns_obk\BB1Link.h"

# include "ns_bbk\KBLangParser.h"
# include "ns_bbk\KBLangFns.h"
#endif

// extern FILE *yyin ;  /* Defined in KBLangLexer.c */
// extern FILE *yyout ; /* Defined in KBLangLexer.c */

char *dupStr(char *s)
{
  char *ds = new char[strlen(s)+1] ;
  strcpy(ds, s) ;
  return (ds) ;
}

#if defined(_MSC_VER)
int isatty(int i) { return(0) ; } // Defined in <unistd.h>; not available in MSVC
#endif

// -----------------------------------------------------------------------------
// The lexer returns a string with the quotes. Remove the quotes before
// duplication.
// -----------------------------------------------------------------------------

char    *unquote(char *s)
{
  int lastchar = strlen(s) - 1 ;
  if ((s[0] == '"') && ((s[lastchar]) == '"'))
  {
    s[lastchar] = '\0' ;
    s = s + 1 ;
  }
  return (dupStr(s)) ;
}

// -----------------------------------------------------------------------------
// In module "KBLangLexer.c", generated with lex from module "KBLangLexer.l",
// memory is allocated by several calls to function "strdup". Thus, memory must
// be freed according to the same protocol. Since this program uses libg++, the
// library function "strdup" will use "new" rather than "malloc" to allocate
// memory. Thus, this memory must be freed with "delete" rather than "free". Why
// does this return 0? It has to be used by the K&R .c file produced by lex from
// "KBLangLexer.l". It can't return void, and int is a good alternative.
// -----------------------------------------------------------------------------
// Some compilers require that a value be returned, and 0 is the optimal choice.
// -----------------------------------------------------------------------------

int freeStr(char *s)
{
  delete s ;
  return (0) ;
}

class   KBLang
{
  public:

    KBLang(BB1BB& BB) : bb(BB) {} ;
    BB1BB&		BB() const			 { return(bb) ; } ;
    BB1KB			*KB() const			 { return(kb) ; } ;
    const StringList *DirectSuperclasses() const	{ return &ds ; } ;
    const AVPSet		 *AttSpec() const		          { return &attSpec ; } ;
    const LinkSet		 *LinkSpec() const		        { return &linkSpec ; } ;
    // void		AddAVP(AttValPair *avp)		{ attSpec.add(avp) ; } ;
    void			AddAVP(AttValPair *avp)		{ attSpec.push_back(new AttValPair(*avp)) ; } ;
    void			AddDirectSuperclass(string dS)	{ ds.append(dS) ; } ;
    // void		AddLink(BB1Link *link)		{ linkSpec.add(link) ; } ;
    void			AddLink(BB1Link *link)		{ linkSpec.push_back(link) ; } ;
    void			Reset()				{ ResetDirectSuperclasses() ; ResetAttSpec() ; ResetLinkSpec() ; } ;
    void			SetKB(BB1KB *KB)		{ kb = KB ; } ;

  private:

    void			ResetDirectSuperclasses()	{ ds.clear() ; } ;
    void			ResetAttSpec()			      { ClearSetOfAVP(&attSpec) ; } ;
    void			ResetLinkSpec()			      { ClearSetOfBB1Link(&linkSpec) ; } ;

  private:

    BB1BB      &bb ;
    BB1KB      *kb ;
    StringList ds ;
    AVPSet     attSpec ;
    LinkSet    linkSpec ;
} *KBL ;

AttValPair *makeAttValPair(string& s, int atype, char *text)
{
  AttValPair *avp ;
  switch (atype)
  {
    case CHARTYPE:     { avp = new AttValPair(s, text[0]) ;              break ; }
    case DOUBLETYPE:   { avp = new AttValPair(s, atof(text)) ;           break ; }
    case INTTYPE:      { avp = new AttValPair(s, (int) atoi(text)) ;     break ; }
    case FLOATTYPE:    { avp = new AttValPair(s, (float) atof(text)) ;   break ; }
    case LONGTYPE:     { avp = new AttValPair(s, atoi(text)) ;           break ; }
    case SHORTTYPE:    { avp = new AttValPair(s, (short) atoi(text)) ;   break ; }
    case STRINGTYPE:   { avp = new AttValPair(s, string(text)) ;         break ; }
    default:           { avp = 0 ;                                       break ; }
  }
  return (avp) ;
};

int avpAction(char *attName, int atype, char *text)
{
  string s(attName) ;
  AttValPair *avp = makeAttValPair(s, atype, text) ;
  if (avp)
    { KBL->AddAVP(avp) ; }
  else
    { cout << "Unrecognized attribute type " << atype << "\n" ; }
  freeStr(attName) ; /* allocated for token IDENTIFIER */
  freeStr(text) ;    /* allocated for token CHARVAL/INTEGERVAL/REALVAL/STRINGVAL */
  return (0) ;
}

int classAction(char *className)
{
  //FIXME
 // KBL->BB().DefineClass(className, AttValPair("control", string("class")), KBL->DirectSuperclasses(), KBL->AttSpec()) ;
 // KBL->Reset() ;
 // freeStr(className) ; /* allocated for token IDENTIFIER */
  return (0) ;
}

int directSuperclassAction(char *ds)
{
  string  sds = string(ds) ;
  KBL->AddDirectSuperclass(sds) ;
  freeStr(ds) ; /* allocated for token IDENTIFIER */
  return (0) ;
}

int instAction(char *instName, char *className)
{
  if (KBL->KB())
  {
    BB1Class *cl = KBL->BB().ClassNamed(className) ;
    if (cl)
    {   //FIXME
  //    BB1Object *obj = cl->MakeInstance(instName, AttValPair("control", string("")), *(KBL->KB()), KBL->AttSpec(), KBL->LinkSpec()) ;
    }
    else
      cout << "Unable to find class " << className << "\n" ;
  }
  else
    cout << "No KB has been defined\n" ;
  KBL->Reset() ;
  freeStr(className) ; /* allocated for token IDENTIFIER */
  freeStr(instName) ;  /* allocated for token IDENTIFIER */
  return (0) ;
}

int kbAction(char *kbName)
{
  KBL->SetKB(KBL->BB().DefineKB(string(kbName))) ;
  freeStr(kbName) ; /* allocated for token IDENTIFIER */
  return (0) ;
}

int ksAction(char *ksName, char *ctlp, char *tc, char *pc, char *oc, char *ac, char *cg)
{
  if (KBL->KB())
  {
    BB1BB& bb = KBL->BB() ;
    //
    // FIXME
    //
   /* BB1KS *ks = bb.DefineKS(string(ksName),
			    *(KBL->KB()),
			    int(atoi(ctlp)),
			    bb.LookupTC(string(tc)),
			    bb.LookupPC(string(pc)),
			    bb.LookupOC(string(oc)),
					bb.LookupAC(string(ac)),
					KBL->AttSpec(),
					KBL->LinkSpec(),
        	bb.LookupCG(string(cg))) ;*/
	}
	else
		cout << "No KB has been defined\n" ;
	KBL->Reset() ;
	freeStr(ksName) ;	/* allocated for token IDENTIFIER */
	freeStr(ctlp) ;	/* allocated for token BOOLVAL */
	freeStr(tc) ;		/* allocated for token IDENTIFIER */
	freeStr(pc) ;		/* allocated for token IDENTIFIER */
	freeStr(oc) ;		/* allocated for token IDENTIFIER */
	freeStr(ac) ;		/* allocated for token IDENTIFIER */
	freeStr(cg) ;		/* allocated for token IDENTIFIER */
	return (0) ;
}

int linkAction(char *linkName, char *toObjectName)
{
	BB1Object *toObject = KBL->BB().ObjectNamed(toObjectName) ;
	if (toObject)
	{
		BB1Link *l = new BB1Link(linkName, *toObject) ;
		if (l)
			{ KBL->AddLink(l) ; }
	}
	else
		cout << "There is no object named " << toObjectName << "\n" ;
	freeStr(linkName) ;     /* allocated for token IDENTIFER} */
	freeStr(toObjectName) ; /* allocated for token LONG_NAME */
	return (0) ;
}

int relAction(char *ForwardRel, char *InverseRel, char *FromClass, char *ToClass)
{
	KBL->BB().DefineRelation(ForwardRel, InverseRel, FromClass, ToClass) ;
	freeStr(ForwardRel) ; /* allocated for token IDENTIFIER */
	freeStr(InverseRel) ; /* allocated for token IDENTIFIER */
	freeStr(FromClass) ;  /* allocated for token IDENTIFIER */
	freeStr(ToClass) ;    /* allocated for token IDENTIFIER */
	return (0) ;
}

/*
void BB1BB::LoadKBAux(const string& KBPathName)
{
	yyout = stdout ;
	KBL = new KBLang(*this) ;
	FILE *file = fopen((const char *) KBPathName.c_str(), "r") ;
	if (file)
		{ yyin = file ; }
	else
		{ cout << "Could not open KB file " << KBPathName << " for loading\n" ; }
	yyparse() ;
	delete KBL ;
	KBL = NULL ;
}
*/

