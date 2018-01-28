//---------------------------------------------------------------------------//     preparations_dlg.h
//     PA   octobre 2010
//  Contient les définitions des objets de Préparations
//---------------------------------------------------------------------------
#ifndef __PREPARATIONS_DLG_H
#define __PREPARATIONS_DLG_H

#include <owl\dialog.h>
#include <owl\listbox.h>
#include <owl\listwind.h>
#include <owl\groupbox.h>
#include <owl\radiobut.h>
#include <owl\static.h>

class NSUtilLexique ;
class NSEditNum ;
class NSUtilEdit2 ;

#include "partage\ns_vector.h"

#include "nsoutil\nsexport.h"
#include "nsbb\nsutidlg.h"

#include "nsmt_preparations\preparations.h"

//// Objet "Boite de dialogue" utilisé pour la création / modification des matériels
//
class CreerPreparationDialog : public NSUtilDialog
{
	public:

    NSUtilEdit2*	 _pLabel ;

		NSUtilLexique* _pType1 ;
    NSEditNum*     _pDose1 ;
    NSUtilLexique* _pUnit1 ;

    NSUtilLexique* _pType2 ;
    NSEditNum*     _pDose2 ;
    NSUtilLexique* _pUnit2 ;

    NSPreparationData*   _pData ;
    bool				         _bCreation ;
    NSPreparationsArray* _pPrepaArray ;

		CreerPreparationDialog(TWindow* pere, NSContexte* pCtx, NSPreparationsArray* pPreparationArray, bool bMode, TModule* module = 0) ;
    ~CreerPreparationDialog() ;

    void SetupWindow() ;

    void CmOk() ;
    void CmCancel() ;

	DECLARE_RESPONSE_TABLE(CreerPreparationDialog) ;
};
//
// Objet "Boite de dialogue" utilisé pour la liste des matériels
//
class NSListePrepaWindow ;

class ListePreparationDialog : public NSUtilDialog
{
	public:

		NSListePrepaWindow * pListePrepa ;
    NSPreparationsArray* pPrepaArray ;
    NSPreparationsArray* pPrepaAjoutArray ;
    NSPreparationsArray* pPrepaModifArray ;
    NSPreparationInfo*   pPrepaSelected ;

    int					 	       nbPrepa ;
    int						       nbPrepaAjout ;
    int					 	       iPrepaChoisi ;

    string               sDefaultPrepaID ;

    ListePreparationDialog(TWindow* pere, NSContexte* pCtx, TModule* module = 0) ;
    ~ListePreparationDialog() ;

    void SetupWindow() ;

    bool InitPrepaArray() ;

    bool ExisteDansAjouts(NSPreparationInfo* pMatInfo) ;
    void InitListe() ;
    void AfficheListe() ;    void EvRButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point) ;
    void EvRButtonDownOut(uint modkeys, NS_CLASSLIB::TPoint& point) ;
    bool InitDataPreparation(NSPreparationInfo* pMatInfo, bool bCreer) ;

    bool CreatePatpatho(NSPreparationInfo* pPrepaInfo, NSPatPathoArray* pPatPatho) ;
    bool UpdatePatpatho(NSPreparationInfo* pPrepaInfo, NSPatPathoArray* pPatPatho) ;
    void updateDrug(NSPatPathoArray* pPatPatho, PatPathoIter iterDrug, string sDrug, string sDose, string sUnit) ;
    void addDrug(NSPatPathoArray* pPatPatho, string sDrug, string sDose, string sUnit) ;

    bool CreerPreparation(NSPreparationInfo* pPrepaInfo) ;
    bool ModifierPreparation(NSPreparationInfo* pPrepaInfo) ;

    void CmCreer() ;
    void CmSupprimer() ;
    void CmModifier() ;
    void CmSetDefault() ;

    void CmOk() ;
    void CmCancel() ;

    void readDefaultPrepaID() ;
    void storeDefaultPrepaID() ;

	DECLARE_RESPONSE_TABLE(ListePreparationDialog) ;
};

//
// Objet "ListWindow" avec gestion du double-click (pour les matériels)
//
class NSListePrepaWindow : public TListWindow
{
	public:

		ListePreparationDialog* pDlg ;

		NSListePrepaWindow(ListePreparationDialog* pere, int resId) : TListWindow(pere, resId)
		{
    	pDlg = pere ;
    }
  	~NSListePrepaWindow() {}

		void EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point) ;
    void EvRButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point) ;
		int  IndexItemSelect() ;

	DECLARE_RESPONSE_TABLE(NSListePrepaWindow) ;
};

#endif    // __PREPARATIONS_DLG_H

