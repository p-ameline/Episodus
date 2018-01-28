//---------------------------------------------------------------------------
// nscqdoc.h : header des documents formulaire
//---------------------------------------------------------------------------
#ifndef __NSCQDOC_H
#define __NSCQDOC_H

class nsarcParseur ;

// #include <owl\owlpch.h>
#include <owl\dc.h>

#include "nautilus\nsannexe.h"
#include "nautilus\nsdocref.h"

class NSCQDocument : public NSRefDocument
{
	public:

		enum TYPEDOCARC { dpio = 0, standard, creatpat, creatadr } ;

	 	NSCQDocument(TDocument* parent, NSDocumentInfo* pDocumentInfo,
    					NSDocumentInfo* pDocHtmlInfo, NSContexte* pCtx, bool bROnly,
              string sDefaultArc = string(""), bool bDefArch = false) ;
	 	NSCQDocument(TDocument* parent = 0, NSContexte* pCtx = 0, string sArc = string(""),
              TYPEDOCARC iTypeFormulaire = dpio, string sDefaultArc = string(""),
              bool bDefArch = false) ;
#ifdef __OB1__
    NSCQDocument(TDocument* parent, NSContexte* pCtx, BB1BBInterfaceForKs* pKSdesc) ;
#endif
	 	NSCQDocument(const NSCQDocument& rv) ;
	 	~NSCQDocument() ;
	 	NSCQDocument& operator=(const NSCQDocument& src) ;

		// NSPatPathoArray* getPatPatho() { return &_PatPathoArray ; }

		bool	GenereHtml(string sPathHtml, string& sNomHtml, OperationType typeOperation,
    						string sAdresseCorresp = "", string sPathDest = "") ;
    bool  GenereHtmlText(string& sHtmlText) ;

    string  InitIntituleNoBirth() ;
    string  GetDocLabel() ;

	 	// Méthodes de gestion des formulaires
		bool    ouvreArchetype() ;
	 	bool    enregistre(bool bVerbose = true, bool bSoaping = true) ;
		string  donneTexte() ;
		void    genereHtmlView() ;
		void    remplaceTagsChemin(string& sHtml) ;

    string  getHelpUrl() ;

    void    connectToHealthConcern() ;
    void    openReferential() ;

	 	// implementation des méthodes virtuelles de TDocument
	 	bool   Open(int mode, const char far* path=0) ;
	 	bool   Close() ;
	 	bool   IsOpen() { return (false == _PatPathoArray.empty()) ; }
	 	void   SetTitle(LPCSTR title) ;
		// void   SetDocName(char *nom);

    string findTitle() ;

    string     getSemDocType() const { return _sSemDocType ; }
    LDVFRAME   getFrame()      const { return _iFrame ; }

    TYPEDOCARC getDocType()    const { return _iTypeDoc ; }
    string     getHtmlView()   const { return _sHtmlView ; }
    string     getArchetype()  const { return _sArchetype ; }

    string     getDefaultArchetype() const { return _sDefaultArchetype ; }

    nsarcParseur* getParser()  const { return _pParseur ; }

    bool       isCreated()     const { return _bCreate ; }
    void       setCreated(bool bCr)  { _bCreate = bCr ; }

    bool       isParsed()      const { return _bParsingOk ; }

    BB1BBInterfaceForKs* getKsDescriptor() const { return _pKSdescriptor ; }

  protected:

    nsarcParseur* _pParseur ;
		string        _sArchetype ;
    string        _sDefaultArchetype ;
    bool          _bForceDefaultArch ;
#ifdef __OB1__
    BB1BBInterfaceForKs* _pKSdescriptor ;
#endif

    bool       _bCreate ;
		bool       _bParsingOk ;
		string     _sHtmlView ;

    string     _sSemDocType ;
    LDVFRAME   _iFrame ;
    TYPEDOCARC _iTypeDoc ;
};

#endif

