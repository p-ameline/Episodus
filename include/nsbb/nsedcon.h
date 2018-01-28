/**********************************************************************///  permettre l'édition de champ numérique pour un NSTreeNode
/**********************************************************************/
#ifndef __NSEDCON_H
#define __NSEDCON_H

class NSUtilLexique ;
class ChampEdition ;

#include <owl\dialog.h>
#include <owl\radiobut.h>

#include "nsbb\nsedilex.h"
#include "nsbb\nsedit.h"
#include "nsbb\nsdico.h"

//
//Valeur chiffrée
//

class NSHistoryEdit : public OWL::TEdit, public NSRoot
{
	public:

		string  sItemValue ;
    string  sUnit ;
    string  sPath ;
    bool    bHistoryOpen ;

    ChampEdition* pPere ;

    NSHistoryEdit(ChampEdition* parent, int resId, NSContexte* pCtx, string sValue, string sValueUnit, string sValuePath) ;

    void    SetupWindow() ;

    void    EvKeyDown(uint key, uint repeatcount, uint flags) ;

	DECLARE_RESPONSE_TABLE(NSHistoryEdit) ;
} ;

class ChampEdition : public TDialog, public NSRoot
{
	public:

		//OWL::TEdit*		   pValeurExacte ;	//valeur exacte
		NSHistoryEdit*	    pValeurExacte ;	//valeur exacte
		OWL::TEdit*		    	pValeurSup ;		//valeur sup
		OWL::TEdit*		    	pValeurInf ;		//valeur inf
		OWL::TRadioButton*  pStock ;         //valeur stockée
		OWL::TRadioButton* 	pDynamique ;     // recalculée dynamiquement

		string              sEtiquette ;

		NSUtilLexique*	    pUnite ;
    string			    		sTypeCalcul ;
    string			    		sValeurExacte ;
    string			    		sValeurSup ;
    string			    		sValeurInf ;
    string			    		sUnite ;
    NSEditDico*		    	pEditDico ;
    bool			    			bCalcul ;
    TWindow* 		    		AParent ;
    int 			    			MaxInPut ;
    int		   		    		ressource ;
    NS_CLASSLIB::TRect  rectEdition ; //rectangle d'édition du NSTreeNode

		ChampEdition(TWindow* pere, TResId res, int MaxEntree, NSContexte* pCtx,
                     NS_CLASSLIB::TRect rectEdition, TModule* module,
                     bool bCalcul = false) ;
		~ChampEdition() ;

    void   CmOk() ;
    void   CmCancel() ;
    void   SetupWindow() ;

    void   ValeurSuperieure() ;
    void   ValeurExacte() ;
    void   ValeurInferieure() ;

    void   setUnit(string sNewUnit) ;
    string getUnit() ;

    void   Statique() ;
    void   Dynamique() ;

  DECLARE_RESPONSE_TABLE(ChampEdition) ;
};

//
//Date ou heure
//
class NSDate : public TDialog, public NSRoot
{
	public:

		string 			    sValeur ;
    NSEditDate*         pDate ;
    NSEditHeure*        pHeure ;
    TWindow* 		    AParent ;
    int		   	        ressource ;
    NS_CLASSLIB::TRect  rectEdition ; //rectangle d'édition du NSTreeNode

    bool                bOkToClose ;

		NSDate(TWindow* parent, TResId ressource, NSContexte* pCtx,
                        string sType, NS_CLASSLIB::TRect rectEdit,
                        TModule* module) ;
		~NSDate() ;

    void CmOk() ;
    void CmCancel() ;
    void SetupWindow() ;
    bool CanClose() ;
    void Initialise(string sUnite) ;
    void Nettoyer() ;

	DECLARE_RESPONSE_TABLE(NSDate) ;
};

//poistion drag and drop et copie

class PositionDragAndDrop : public TDialog, public NSRoot
{
	public:

    enum DRAGTYPE { dragNotSpecified = 0, dragBefore, dragAfter, dragAsSon } ;

		PositionDragAndDrop(TWindow* parent, TResId ressource, NSContexte* pCtx,
                            TModule* module) ;
		~PositionDragAndDrop() ;

    DRAGTYPE getDragType()   { return iDragType ; }
    void     resetDragType() { iDragType = dragNotSpecified ; }

  protected:

    OWL::TRadioButton* pApres ;
    OWL::TRadioButton* pAvant ;
    OWL::TRadioButton* pFils ;

    DRAGTYPE iDragType ;

    void Avant() ;
    void Apres() ;
    void Fils() ;
    void CmOk() ;
    void SetupWindow() ;

	DECLARE_RESPONSE_TABLE(PositionDragAndDrop) ;
};

#endif  // __NSEDCON_H

