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
// Objet "Boite de dialogue" utilisé pour enregistrer un document
//
// -----------------------------------------------------------------------------

class EnregDocDialog : public NSUtilDialog
{
	public:

		NSDocumentData  *pDocData ;

		TEdit           *pNomDoc ;
		NSUtilEditDateHeure *pDateCreation ;
		TRadioButton    *pHistorique ;
		TComboBox       *pChemiseBox ;
		TGroupBox       *pInteretGroup ;
		TVSlider        *pImportance ;
		TButton         *pNSSlider0P ;
		TButton         *pNSSlider1P ;
		TButton         *pNSSlider2P ;
		TButton         *pNSSlider3P ;
		TButton         *pNSSlider4P ;
		TEdit           *pCommentaire ;
		NSRosaceGroupBox *pRosaceGroup ;
    TCheckBox				 *_synchro_check ; // Checkbox pour dire que l'objet est synchronisable
    TCheckBox				 *_note_check ;    // CheckBox pour dire que l'objet est une note personnelle

		NSChemiseArray  *pChemisesArray ;

		int        	  	creation ;

		int 		   			ChemiseChoisie ;
		char            *pCodeChemiseChoisie ;

		string					sRightsString ;
		NSRosace* 		 	pRosace ;

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
// dialogue utilisé pour choisir un nouveau nom de chemise (ou defaut)
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
// dialogue utilisé pour créer un nouveau Séjour
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
// Classe ListeSejoursDialog -> Séjours du patient
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
// Objet "Boite de dialogue" utilisé pour créer une chemise
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

