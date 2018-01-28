#ifndef __NSCONCLU_H
#define __NSCONCLU_H

#include <owl\dialog.h>
#include <owl\radiobut.h>

#include "nsbb\nsbb.rh"
#include "nsbb\nsbbitem.h"


//------------------------------------------------------------------------
// éléments pour la conclusion
//------------------------------------------------------------------------

#ifdef _INCLUS
class NSDialogWrapper;
#endif

#ifdef _INCLUS
class NSConclusion : public NSDialogWrapper
#else
class NSConclusion : public TDialog
#endif
{
	public :

   TResId				ressource;
   TWindow* 			AParent;
   BBItem*           pBBitemConclusion;

   TRadioButton*		pAvant;
   TRadioButton*		pApres;
   TRadioButton*		pPlace;


   //
	// Constructeurs 		Destructeur
   //
   NSConclusion(TWindow* parent, TResId resId, BBItem* pBBitem, TModule* module = 0);
    ~NSConclusion();

   NSConclusion( NSConclusion& src);
   // Surcharge de l'opérateur d'affectation
	NSConclusion& operator=(NSConclusion src);


   //
   // Opérateurs
   //
   void SetupWindow();
   void CmOk();
   void CmCancel();

   //table de réponses
   DECLARE_RESPONSE_TABLE(NSConclusion);
};



#endif
