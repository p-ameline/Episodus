/*
**
** BB1AppInst.h
**
*/

/*
** Blackboard objects created by BB1Class::MakeInstance(...), where the class is
** application-defined, are of class BB1AppInst.
*/

#ifndef _BB1AppInst_h
#define _BB1AppInst_h 1

class BB1Token ;

#ifdef __GNUC__
#pragma interface
#endif // __GNUC__

#include "BB1Object.h" // base type

class BB1AppInst : public BB1Object
{
  /* Allowed to call BB1AppInst::BB1AppInst(...) */
  friend class BB1BB ;

  public:
    // no public members

  private:
  
#ifdef __TCONTROLER__
    BB1AppInst(const string   sOName,
               AttValPair&    dispatch,
               BB1KB&         kb,
               BB1Class*      c,
               AVPSet*        pAttSpec  = (AVPSet*) 0,
               const LinkSet* pLinkSpec = (LinkSet*) 0) ;
#else
    BB1AppInst(const string	&oname,
               BB1KB		&kb,
               BB1Class		*c,
               const AVPSet	*attSpec = NULL,
               const LinkSet	*linkSpec = NULL) ;
#endif

    BB1AppInst(const BB1AppInst &bbAppInst) ;

    void Save(ofstream &KBstream) ;

  private:

    ~BB1AppInst() { } ;
} ;

#endif // _BB1AppInst_h

