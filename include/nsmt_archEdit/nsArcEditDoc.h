//----------------------------------------------------------------------------// ObjectWindows - (C) Copyright 1991, 1993 by Borland International
//   Tutorial application -- step12dv.cpp
//----------------------------------------------------------------------------
#ifndef __NSARCEDITDOC_H
#define __NSARCEDITDOC_H

// #include <owl\owlpch.h>
#include <owl\dc.h>

#if defined(_DANSDLL)
	#define _CSDOCCLASSE __import
#endif

#include <owl\docmanag.h>

class NSArcEditInterfaceView ;
class NSArcEditTreeView ;
class NSControle ;

#include "partage\nsglobal.h"
#include "nsbb\nsarc.h"
#include "nssavoir\nslocalise.h"

class NSArchEditDocument : public TDocument, public NSRoot
{
	public:

    NSArchEditDocument(TDocument* parent = 0, NSContexte* pCtx = 0) ;
		~NSArchEditDocument() ;

    void open(string sFileN) ;
    void save() ;

    nsarcParseur* getParser()   { return _pArchParser ; }
    string        getFileName() { return _sFileName ; }
    BBItem*       getRootBBItem() ;

    void          selectGuiObjectForLocalisation(string sLocalisation) ;
    NSControle*   getControlForLocalisation(string sLocalisation) ;
    void          createControlForLocalisation(string sLocalisation, string sLabel) ;

    Cdialogbox*   getDialogObject(string sLang, bool bCreateIfNecessary = true) ;
    void          createDialogObject(string sLang) ;

    void          createRootItem() ;

    void          setDialogParams() ;

    NSArcEditInterfaceView* getGuiView() ;
    NSArcEditTreeView*      getTreeView() ;

    bool          initLocalizationStrings() ;
    string        getText(string sChapter, string sCode) ;

  protected:

    string       _sFileName ;
    nsarcParseur *_pArchParser ;

    NSLocalChapterArray aLocalizationStrings ;
} ;

#endif // __NSARCEDITDOC_H

