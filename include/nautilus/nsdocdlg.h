#ifndef __NSDOCDLG_H__# define __NSDOCDLG_H__# include <classlib\arrays.h>
# include <owl\dialog.h>
# include <owl\listbox.h>
# include <owl\combobox.h>
# include <owl\groupbox.h>
# include <owl\checkbox.h>
# include <owl\edit.h>
# include <owl\slider.h>

# include "nautilus\nsannexe.h"
# include "nautilus\nsanxary.h"
# include "nsbb\nsednum.h"
# include "nsbb\nsutidlg.h"

# include "nssavoir\nsRightsManager.h"

// -----------------------------------------------------------------------------
//
// Objet "Boite de dialogue" utilis� pour enregistrer un document
//
// -----------------------------------------------------------------------------

class EnregDocDialog : public NSUtilDialog
{
	public:

		NSDocumentData      *_pDocData ;

		TEdit               *_pNomDoc ;
		NSUtilEditDateHeure *_pDateCreation ;
		TRadioButton        *_pHistorique ;
		TComboBox           *_pChemiseBox ;
		TGroupBox           *_pInteretGroup ;
		TVSlider            *_pImportance ;
		TButton             *_pNSSlider0P ;
		TButton             *_pNSSlider1P ;
		TButton             *_pNSSlider2P ;
		TButton             *_pNSSlider3P ;
		TButton             *_pNSSlider4P ;
		TEdit               *_pCommentaire ;
		NSRosaceGroupBox    *_pRosaceGroup ;
    TCheckBox				    *_synchro_check ; // Checkbox pour dire que l'objet est synchronisable
    TCheckBox				    *_note_check ;    // CheckBox pour dire que l'objet est une note personnelle

		NSChemiseArray      *_pChemisesArray ;

		int        	  	    _creation ;

		int 		   			    _ChemiseChoisie ;
		char                *_pCodeChemiseChoisie ;

		string					    _sRightsString ;
		NSRosace* 		 	    _pRosace ;

		EnregDocDialog(TWindow *, NSDocumentData *, char *, string sRights, NSContexte *pCtx) ;
		~EnregDocDialog() ;

		void            SetupWindow() ;

		void            RemplirChemises() ;
		void            InitChemisesBox() ;
		void            CmHistorique() ;
		void            CmOk() ;
		void            CmCancel() ;
		void            CmNouvChem() ;

		void            TraitementInteret0P() ;
		void            TraitementInteret1P() ;
		void            TraitementInteret2P() ;
		void            TraitementInteret3P() ;
		void            TraitementInteret4P() ;

	DECLARE_RESPONSE_TABLE(EnregDocDialog) ;
} ;

// -----------------------------------------------------------------------------
//
// dialogue utilis� pour choisir un nouveau nom de chemise (ou defaut)
//
// -----------------------------------------------------------------------------
class NomChemiseDialog : public NSUtilDialog
{
 public:

  TEdit         *pNomChem ;
  TRadioButton  *pChemDefaut ;
  TRadioButton  *pChemNew ;

  string			  sNomChem ;

  NomChemiseDialog(TWindow *pere, NSContexte *pCtx) ;
  ~NomChemiseDialog() ;

  void          SetupWindow() ;

  void          CmClickChemDefaut() ;
  void          CmClickChemNew() ;
  void          CmChangeNomChem() ;

  void          CmOk() ;
  void          CmCancel() ;

  DECLARE_RESPONSE_TABLE(NomChemiseDialog) ;
} ;

// -----------------------------------------------------------------------------
//
// dialogue utilis� pour cr�er un nouveau S�jour
//
// -----------------------------------------------------------------------------
class CreerSejourDialog : public NSUtilDialog
{
 public:

  TEdit           *pNumSejour ;
  TEdit           *pNumUF ;
  NSUtilEditDate  *pDateDeb ;
  NSUtilEditDate  *pDateFin ;

  string			    sNumUF ;

  string					sPrevNum ;
  string					sPrevCode ;

  CreerSejourDialog(TWindow *pere, NSContexte *pCtx, string sNumero = "") ;
  ~CreerSejourDialog() ;

  void            SetupWindow() ;

  void            CmOk() ;
  void            CmCancel() ;

  DECLARE_RESPONSE_TABLE(CreerSejourDialog) ;
} ;

// -----------------------------------------------------------------------------
//
// Classe ListeSejoursDialog -> S�jours du patient
//
// -----------------------------------------------------------------------------
class ListeSejoursDialog : public NSUtilDialog
{
 public:

  OWL::TListBox   *pListe ;

  string          sEnCours ;

  ListeSejoursDialog(TWindow *pere, NSContexte *pCtx, TModule *pModule = 0) ;
  ~ListeSejoursDialog() ;

  void            SetupWindow() ;
  void            AfficheListe() ;

  void            CmOk() ;
  void            CmCancel() ;
  void            CmSupprimer() ;
  void            CmCreer() ;
  void            CmModifier() ;
  void            CmSelectSejour() ;
  void            CmSejourDblClk() ;

  DECLARE_RESPONSE_TABLE(ListeSejoursDialog) ;
} ;

// -----------------------------------------------------------------------------
//
// Objet "Boite de dialogue" utilis� pour cr�er une chemise
//
// -----------------------------------------------------------------------------
class EnregChemDialog : public NSUtilDialog
{
	public:

  		NSChemiseData   *pChemData ;

  		TEdit           *pNomChem ;
  		TComboBox       *pArmoireBox ;
  		TEdit           *pCommentaire ;

  		// NSArmoireArray  *pArmoiresArray ;

  		int        	    creation ;

  		int 		   	ArmoireChoisie ;
  		char            *pCodeArmoireChoisie ;

  		EnregChemDialog(TWindow *, NSChemiseData *, char *, NSContexte *pCtx) ;
  		~EnregChemDialog() ;

  		void            SetupWindow() ;

  		void            CmOk() ;
  		void            CmCancel() ;
  		void            CmNouvArmoire() ;

	DECLARE_RESPONSE_TABLE(EnregChemDialog) ;
} ;

#endif // __NSDOCDLG_H__

