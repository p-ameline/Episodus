#if !defined(NSFICHEEDIT_H)#define NSFICHEEDIT_H

#include <owl\dialog.h>#include <owl\radiobut.h>
#include <owl\edit.h>
#include <owl\checkbox.h>
#include <owl\groupbox.h>
#include <owl\listbox.h>
#include <owl\scrollba.h>
#include <owl\panespli.h>
#include <owl\listwind.h>

#include "nsbb\nsednum.h"#include "nsbb\nstrewi.h"
#include "nsbb\nsbbsmal.h"
#include "nsbb\nsarc.h"#include "nsoutil\nssavary.h"
#include "nsoutil\nsexport.h"

#define ID_TREE 100#define ID_LIST 101

//
// class NSsmEditeur : 	fenêtre divisée en 2 parties : la première contient
//								la patpatho de sLexiquePilote et la deuxième contient
//								les équivalents sémantiques de sLexiquePilote.
//
// ~~~~~ ~~~~~~~~~~~~~
//
class NSsmEditeur : public NSRoot, public TPaneSplitter
{ public:

	//
	// Data members
  //
  string 				sLexiquePilote ; //élément lexique choisi par l'utilisateur

  NSPatPathoArray* 	pPatPathoArray ;
  NSSavoirArray*		pSavoirArray ;

  NSSmallBrother* 	pBigBoss ;

  NSTreeWindow* 		pNSTreeWindow ;
  NSPaneListWindow* 	pListWind ;

  NSsmEditeur(TWindow* parent, NSContexte* pCtx, string sLexiquePilot) ;
  ~NSsmEditeur() ;

  void SetupWindow() ;
  void CleanupWindow() ;
  void EvClose() ;

	void CmEnregistre() ;
  bool enregPatPatho() ;

  void initPatPatho() ;
  void initSavoir() ;

  void AfficheListe() ;
  void LvnGetDispInfo(TLwDispInfoNotify& dispInfo) ;

  void donneRelation(string* pQualificatif, string* pQualifie, string* pEtiquette) ;

  DECLARE_RESPONSE_TABLE(NSsmEditeur) ;
};
#endif // NSFICHEEDIT_H
