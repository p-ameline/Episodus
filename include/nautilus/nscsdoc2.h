//----------------------------------------------------------------------------
// ObjectWindows - (C) Copyright 1991, 1993 by Borland International
//   Tutorial application -- step12dv.cpp
//----------------------------------------------------------------------------
#ifndef __NSCSDOC_H
#define __NSCSDOC_H

// #include <owl\owlpch.h>
#include <owl\dc.h>
#include <owl\inputdia.h>
#include <owl\chooseco.h>
#include <owl\gdiobjec.h>
#include <owl\docmanag.h>
#include <owl\filedoc.h>
#include <owl\listbox.h>
#include <classlib\arrays.h>
#include <classlib\heapsel.h>
//#include "heapsel.h"

#if defined(_DANSDLL)
	#define _CSDOCCLASSE __import
#endif

#include "nautilus\nsannexe.h"
#include "nautilus\nstxstyl.h"
#include "nautilus\nsdocref.h"
#include "nautilus\nsrechd2.h"
#include "nautilus\nscr_ama.h"
#include "nsbb\nsbbsmal.h"

bool nouveauCompteRendu(NSSuper *pSuper);
bool afficheCompteRendu(string *pCRString, NSSuper *pSuper);
bool afficheCompteRendu(NSDocumentInfo& Document, NSSuper *pSuper);


//class _DOCVIEWCLASS NSCRDocument : public NSRefDocument
//class NSCRDocument : public TOleDocument

class NSCSDocument : public TDocument, public NSRoot
{
  	public:
   
    char chAffiche[200], dateAffiche[20];
	 //string 			  sCompteRendu;
    NSPatPathoArray* pPatPathoArray;

	 NSSmallBrother* pBigBoss;
    NSDocumentInfo* pDocInfo;
    NSDocumentInfo* pHtmlInfo;
	 string 			  sTitre;

	 NSCSDocument(TDocument* parent, NSDocumentInfo* pDocumentInfo,
    					NSDocumentInfo* pDocHtmlInfo, NSSuper* pSuper);
	 NSCSDocument(TDocument* parent = 0, NSSuper* pSuper = 0);
	 NSCSDocument(NSCSDocument& rv);
	 ~NSCSDocument();
	 NSCSDocument& operator=(NSCSDocument& src);

    NSPatPathoArray* getPatPatho() { return pPatPathoArray;}

	 //void CreateArray(char *source);

	 // Méthodes de gestion des comptes rendus
	 //BOOL nouveauCR(TWindow* pFenetre);
	 bool enregistre();
	 // bool chercheDerniereFiche(NSCompteRendu* pCR);
	 bool enregistreCS(NSDocument* pNewDocument);

	 // implementation des méthodes virtuelles de TDocument
	 bool   Open(int mode, const char far* path=0);
	 bool   Close();
	 bool   IsOpen() { return !(pPatPathoArray->empty()); }
	 bool	  CanClose();
	 void	  SetTitle(LPCSTR title);
    //void	  	SetTitle(LPCSTR title);
    void   	SetDocName(char *nom);
    //bool		Commit(bool force);
   // bool		Revert(bool clear);

};

#endif // __NSCSDOC_H
