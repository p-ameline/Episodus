//----------------------------------------------------------------------------// ObjectWindows - (C) Copyright 1991, 1993 by Borland International
//   Tutorial application -- step12dv.cpp
//----------------------------------------------------------------------------
#ifndef __NSCSDOC_H
#define __NSCSDOC_H

// #include <owl\owlpch.h>
#include <owl\dc.h>

#if defined(_DANSDLL)
	#define _CSDOCCLASSE __import
#endif

#include "ns_sgbd\nsannexe.h"
#include "nautilus\nsdocref.h"
#include "nsbb\nspatpat.h"

bool nouveauCompteRendu(NSContexte *pCtx);
bool afficheCompteRendu(string *pCRString, NSContexte *pCtx);
bool afficheCompteRendu(NSDocumentInfo& Document, NSContexte *pCtx);

class NSHtml;

class NSCSDocument : public NSRefDocument
{
	public:

		string 	_sTitre ;
		string 	_sTypeDocument ; //type document
	 	NSHtml*	_pHtmlCS ;			 // html repr�sentant la TreeView

	 	NSCSDocument(TDocument* parent, NSDocumentInfo* pDocumentInfo,
    					   NSDocumentInfo* pDocHtmlInfo,
                 NSContexte* pCtx, string sTypeDocument, bool bROnly) ;
	 	NSCSDocument(TDocument* parent = 0, NSContexte* pCtx = 0, string sTypeDocument = "") ;
	 	NSCSDocument(const NSCSDocument& rv) ;
	 	~NSCSDocument() ;
	 	NSCSDocument& operator=(const NSCSDocument& src) ;

		// NSPatPathoArray* getPatPatho() { return &_PatPathoArray ; }

		bool	GenereHtml(string sPathHtml, string& sNomHtml,
                     OperationType typeOperation,
                     string sAdresseCorresp = string(""),
                     string sPathDest = string("")) ;
    bool  GenereHtmlText(string& sHtmlText) ;
    bool  GenereRawText(string& sRawText) ;
    void  GenereRawTextForBloc(string& sRawText, NSHtmlArray *pFilsArray, string sDecal) ;

	 	// M�thodes de gestion des comptes rendus
	 	//BOOL nouveauCR(TWindow* pFenetre);
	 	bool    enregistre(bool bVerbose = true, bool bSoaping = true) ;
	 	// bool chercheDerniereFiche(NSCompteRendu* pCR);
	 	// bool    enregistreCS(NSDocument* pNewDocument) ;
    string  InitIntituleNoBirth() ;
		void    activate() ;

    void    invalidateViews(string sReason) ;

    NSHtml*	getAsHtml() { return _pHtmlCS ; }

	 	// implementation des m�thodes virtuelles de TDocument
	 	bool    Open(int mode, const char far* path=0) ;
	 	bool    Close() ;
	 	bool    IsOpen() { return (false == _PatPathoArray.empty()) ; }
	 	void    SetTitle(LPCSTR title) ;
		void    SetDocName(char *nom) ;
} ;

#endif // __NSCSDOC_H

