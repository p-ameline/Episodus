//----------------------------------------------------------------------------
//   Programme protege par une licence Open Source GPL
//   Philippe Ameline
//----------------------------------------------------------------------------
#ifndef __NSLDVSTRUCTS_H
#define __NSLDVSTRUCTS_H

#ifndef __linux__
#if defined(_EPISODUSDLL)
	#define _EPISODUS __export
#else
	#define _EPISODUS __import
#endif
#endif

#include "nsbb\nspatpat.h"

class _EPISODUS NewConcernInfo : public NSSuperRoot
{
  public:

    // Constructeurs - Destructeur
    NewConcernInfo(NSSuper* pSuper) ;
    NewConcernInfo(const NewConcernInfo& rv) ;
    ~NewConcernInfo() ;

    NewConcernInfo& operator=(const NewConcernInfo& src) ;

    LDVFRAME getFrame()     { return _iFrame ; }
    string   getLexique()   { return _sLexique ; }
    string   getFreeText()  { return _sTL ; }
    string   getStartDate() { return _sDateDeb ; }
    string   getEndDate()   { return _sDateFin ; }
    int      getSeverity()  { return _iSeverity ; }
    string   getCocCode()   { return _sCocCode ; }
    string   getCimCode()   { return _sCimCode ; }

    NSPatPathoArray* getDetails() { return &_pptDetails ; }

    void setFrame(const LDVFRAME iFrame) { _iFrame = iFrame ; }
    void setLexique(const string sL)     { _sLexique = sL ; }
    void setFreeText(const string sFT)   { _sTL = sFT ; }
    void setStartDate(const string sSD)  { _sDateDeb = sSD ; }
    void setEndDate(const string sED)    { _sDateFin = sED ; }
    void setSeverity(const int iS)       { _iSeverity = iS ; }
    void setCocCode(const string sCC)    { _sCocCode = sCC ; }
    void setCimCode(const string sCC)    { _sCimCode = sCC ; }

    void setDetails(const NSPatPathoArray& ppt) { _pptDetails = ppt ; }

  protected:

    LDVFRAME _iFrame ;
    string   _sLexique ;
    string   _sTL ;
    string   _sDateDeb ;
    string   _sDateFin ;
    int      _iSeverity ;
    string   _sCocCode ;
    string   _sCimCode ;

    NSPatPathoArray _pptDetails ;
};

#endif // __NSLDVSTRUCTS_H

