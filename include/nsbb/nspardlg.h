// nspardlg.h#ifndef __NSPARDLG_H
#define __NSPARDLG_H
#include <owl/dialog.h>
#include <owl/static.h>
#include <owl/radiobut.h>
#include <owl/slider.h>
#include <owl/radiobut.h>

#include "nsbb\nspardlg.rh"
#include "nsbb\nsbbtran.h"
#include "nsbb\nstrnode.h"

#include "nsbb\nsutiDlg.h"

//------------------------------------------------------------------------
//------------------------------------------------------------------------
class NSSliderStatic : public TStatic
{
	public :

   	TSlider* pSlider ;
   	int			ressource ;
   	TWindow* AParent ;
   	int 			valeur ;
   	//
		// Constructeurs 		Destructeur
   	//
   	NSSliderStatic(TWindow* parent, int resId, TSlider* pOintSlider, int val ,
                   uint texlen = 0, TModule* module = 0);

   	~NSSliderStatic();
   	NSSliderStatic(const NSSliderStatic& src);
   	// Surcharge de l'opérateur d'affectation
		NSSliderStatic& operator=(const NSSliderStatic& src);
   	//
   	// Opérateurs
   	//
   	void SetupWindow();
   	void EvStnClicked();
   	//table de réponses
	DECLARE_RESPONSE_TABLE(NSSliderStatic) ;
};

//------------------------------------------------------------------------
// paramètres des éléments Patpatho
//------------------------------------------------------------------------
class Parametre : public TDialog
{
	public :

  	TResId				ressource ;   	TWindow* 			AParent ;
   	Message*      pPatpatho ;//patpatho

   	TRadioButton* pPlurielDes ;
   	TRadioButton* pPlurielLes ;
   	TRadioButton* pSingulier ;

   	TSlider*      pCertitude ;
   	TSlider*      pImportance ;

   	string sPlurielDlg ;
   	string sInteretDlg;
   	string sCertitudeDlg ;
   	NSTreeNode* pNSTreeNodeSelection ;

   	TButton*   pNSSlider0 ;
   	TButton*   pNSSlider25 ;
   	TButton*   pNSSlider50 ;
   	TButton*   pNSSlider75 ;
   	TButton*   pNSSlider100 ;
   	TButton*   pNSSliderAbsent ;
   	TButton*   pNSSliderPresent ;
   	TButton*   pNSSliderEventuel ;
   	TButton*   pNSSlider0P ;
   	TButton*   pNSSlider1P ;
   	TButton*   pNSSlider2P ;
   	TButton*   pNSSlider3P ;
   	TButton*   pNSSlider4P ;

   	//
		// Constructeurs 		Destructeur
   	//
   	Parametre(TWindow* parent, TResId resId, Message *pPatpath,
					   NSTreeNode* pNSTreeNode, TModule* module = 0) ;
    ~Parametre() ;

   	Parametre(const Parametre& src) ;
   	// Surcharge de l'opérateur d'affectation
		Parametre& operator=(const Parametre &src) ;

   	//
   	// Opérateurs
   	//
   	void SetupWindow() ;
   	void CmOk() ;
   	void CmCancel() ;

   	void TraitementCertitude0Val() ;
   	void TraitementCertitude25Val() ;
   	void TraitementCertitude50Val() ;
   	void TraitementCertitude75Val() ;
   	void TraitementCertitude100Val() ;
   	void TraitementCertitudeAbsentVal() ;
   	void TraitementCertitudePresentVal() ;
   	void TraitementCertitudeEventuelVal() ;

   	void TraitementInteret0P() ;
   	void TraitementInteret1P() ;
   	void TraitementInteret2P() ;
   	void TraitementInteret3P() ;
   	void TraitementInteret4P() ;

	//table de réponses	DECLARE_RESPONSE_TABLE(Parametre) ;
};

// -----------------------------------------------------------------------------
//
// What to edit?
//
// -----------------------------------------------------------------------------
class whatToEditDialog : public NSUtilDialog
{
 public:

  OWL::TRadioButton* _pEditPersonalData ;
	OWL::TRadioButton* _pEditProfessionalData ;
	OWL::TRadioButton* _pEditUserData ;

  string _sRoles ;

  whatToEditDialog(TWindow *, string sRole, NSContexte *pCtx) ;
  ~whatToEditDialog() ;

  void        SetupWindow() ;
  void        CmOk() ;
  void        CmCancel() ;

  bool isPersonalDataActive() ;
  bool isProfessionalDataActive() ;
  bool isUserDataActive() ;

  DECLARE_RESPONSE_TABLE(whatToEditDialog) ;
} ;

#endif
