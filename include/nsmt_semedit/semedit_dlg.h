#ifndef __SYSTOOLS_DLG_H#define __SYSTOOLS_DLG_H

class NSUtilLexique ;

#include <owl\dialog.h>
#include <owl\groupbox.h>
#include <owl\checkbox.h>

#include "nsepisod\flechiesDB.h"
#include "nsbb\nsutidlg.h"

//
// Objet "Boite de dialogue" utilisé pour sélectionner l'utilitaire
//
class NsSystoolsDialog : public NSUtilDialog
{
    public:

        OWL::TCheckBox*	  pFichEdit ;
        OWL::TCheckBox*	  pCodeLib ;
        OWL::TCheckBox*	  pVF ;
        OWL::TCheckBox*	  pPV ;
        OWL::TCheckBox*	  pTV ;

	    NsSystoolsDialog(TWindow* pere, NSContexte* pCtx) ;
	    ~NsSystoolsDialog() ;

	    void SetupWindow() ;

        void CmOk();
	    //void CmCancel();

        void lanceEditSem() ;
        void lanceFicheEdit() ;
        void lanceCodeLibelle() ;
        void lanceVF() ;
        void lancePV() ;
        void lanceTV() ;

    DECLARE_RESPONSE_TABLE(NsSystoolsDialog) ;
} ;

//---------------------------------------------------------------
//fixer le type de document
//---------------------------------------------------------------
class NSTermeLexique : public TDialog, public NSRoot
{
    public :

   	    NSUtilLexique*	  	pType ;
        string* 		    pTypeDocum ;

        NSTermeLexique(TWindow* pere, NSContexte* pCtx, string* pTypeDocument,                     TModule* module = 0) ;
   	    ~NSTermeLexique() ;

   	    void CmOk() ;        void CmCancel() ;
        void SetupWindow() ;

    DECLARE_RESPONSE_TABLE(NSTermeLexique) ;};

//---------------------------------------------------------------
//---------------------------------------------------------------
class NSCodeLibelle : public TDialog, public NSRoot
{
    public :

   	    OWL::TListBox*  pLibelles ;
        OWL::TEdit*     pAmmorce ;
        OWL::TButton*   pChercher ;

        OWL::TEdit*	    pOrphel ;
   	    OWL::TButton*   pCherchor ;

   	    NSCodeLibelle(TWindow* pere, NSContexte* pCtx) ;
   	    ~NSCodeLibelle() ;

   	    void CmOk() ;
   	    void CmCancel() ;
   	    void SetupWindow() ;

        void recherche() ;
        void rechOrph() ;

    DECLARE_RESPONSE_TABLE(NSCodeLibelle) ;
} ;

//---------------------------------------------------------------

class NSVraiFaux : public TDialog, public NSRoot{
    public :

   	    NSUtilLexique*  pItem1;
        NSUtilLexique*  pItem2;
        OWL::TEdit*     pRelation;

   	    NSVraiFaux(TWindow* pere, NSContexte* pCtx);
   	    ~NSVraiFaux();

    	void CmOk();
   	    void CmCancel();
   	    void SetupWindow();

    DECLARE_RESPONSE_TABLE(NSVraiFaux);};

//---------------------------------------------------------------//---------------------------------------------------------------
class NSPremierVrai : public TDialog, public NSRoot
{
    public :

   	    NSUtilLexique*  pItem1;        NSUtilLexique*  pItem2;
        OWL::TEdit*     pRelation;

   	    NSPremierVrai(TWindow* pere, NSContexte* pCtx);
   	    ~NSPremierVrai();

   	    void CmOk();
   	    void CmCancel();
   	    void SetupWindow();

    DECLARE_RESPONSE_TABLE(NSPremierVrai);
};

//---------------------------------------------------------------//---------------------------------------------------------------

class NSTousLesVrais : public TDialog, public NSRoot{
    public :

   	    NSUtilLexique*  pItem1;
        OWL::TEdit*     pRelation;

   	    NSTousLesVrais(TWindow* pere, NSContexte* pCtx);
   	    ~NSTousLesVrais();

   	    void CmOk();
   	    void CmCancel();
   	    void SetupWindow();

    DECLARE_RESPONSE_TABLE(NSTousLesVrais);
};

#endif  // __SYSTOOLS_DLG_H
