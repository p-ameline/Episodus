#ifndef __NSDOCDLG_H__# define __NSDOCDLG_H__#if defined(_DANSNSDNDLL)	#define _CLASSENSDN __export
#else
	#define _CLASSENSDN __import
#endif# include <classlib\arrays.h>
# include <owl\dialog.h>
# include <owl\listbox.h>
# include <owl\combobox.h>
# include <owl\groupbox.h>
# include <owl\checkbox.h>
# include <owl\edit.h>
# include <owl\slider.h>
# include <owl\radiobut.h>

class NSDocumentData ;
class NSChemiseData ;

# include "ns_sgbd\nsannexe.h"
# include "nsdn\nsanxary.h"
# include "nsbb\nsednum.h"
# include "nsbb\nscomboutil.h"
# include "nsbb\nsutidlg.h"

# include "nssavoir\nsRightsManager.h"

// -----------------------------------------------------------------------------
//
// Objet "Boite de dialogue" utilisé pour enregistrer un document
//
// -----------------------------------------------------------------------------

class _CLASSENSDN EnregDocDialog : public NSUtilDialog
{
	public:

		EnregDocDialog(TWindow *, NSDocumentData *, string &sChemise, const string sRights, NSContexte *pCtx) ;
    EnregDocDialog(TWindow *, NSDocumentData *, string &sChemise, const string sRights, NSContexte *pCtx, TResId resID) ;
		virtual ~EnregDocDialog() ;

    void RemplirChemises() ;

    NSChemiseArray* getChemisesArray() { return &_aChemisesArray ; }

  protected:

    NSDocumentData      *_pDocData ;

		TEdit               *_pNomDoc ;
    NSUtilLexique       *_pType ;
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

		NSChemiseArray      _aChemisesArray ;

		int        	  	    _iCreation ;

		int 		   			    _iChemiseChoisie ;
		string             *_pCodeChemiseChoisie ;

		string					    _sRightsString ;
		NSRosace* 		 	    _pRosace ;

    void                 SetupWindow() ;

    NSHealthTeamMandate* GetMandate() ;
    void                 CreateControls() ;
    void                 InitializeRosace(const string sRights) ;

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

class _CLASSENSDN EnregDocDialogCombo : public EnregDocDialog
{
  public:

    EnregDocDialogCombo(TWindow *, NSDocumentData *, string &sChemise, string sRights, NSContexte *pCtx) ;
    ~EnregDocDialogCombo() ;

    void            SetupWindow() ;
    string          GetParamsFile() ;

  protected:

    TButton           *_pTypeGetCombo ;
    NSComboBox        *_pTypeFromList ;
    OWL::TGlyphButton *_pTypeEditList ;

    void CmCelectType() ;
    void CmEditTypes() ;
    void CmTypeChanged() ;

    bool GetTypes(VecteurString* pVect) ;
    bool SaveTypes(VecteurString* pVect) ;

  DECLARE_RESPONSE_TABLE(EnregDocDialogCombo) ;
} ;

// -----------------------------------------------------------------------------
//
// dialogue utilisé pour choisir un nouveau nom de chemise (ou defaut)
//
// -----------------------------------------------------------------------------
class _CLASSENSDN NomChemiseDialog : public NSUtilDialog
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
class _CLASSENSDN CreerSejourDialog : public NSUtilDialog
{
  public:

    TEdit               *_pNumSejour ;
    TEdit               *_pNumUF ;
    NSUtilEditDateHeure *_pDateDeb ;
    NSUtilEditDateHeure *_pDateFin ;

    string			    sNumUF ;

    string					sPrevNum ;
    string					sPrevCode ;

    CreerSejourDialog(TWindow *pere, NSContexte *pCtx, NSSejourInfo *pVenue, string sDefaultVenueId) ;
    ~CreerSejourDialog() ;

    void            SetupWindow() ;

    void            CmOk() ;
    void            CmCancel() ;

  protected:

    NSSejourInfo *_pEditedVenue ;

  DECLARE_RESPONSE_TABLE(CreerSejourDialog) ;
} ;

// -----------------------------------------------------------------------------
//
// Classe ListeSejoursDialog -> Séjours du patient
//
// -----------------------------------------------------------------------------
class _CLASSENSDN ListeSejoursDialog : public NSUtilDialog
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
class _CLASSENSDN EnregChemDialog : public NSUtilDialog
{
	public:

    NSChemiseData   *pChemData ;

    TEdit           *pNomChem ;
    TComboBox       *pArmoireBox ;
    TEdit           *pCommentaire ;

    // NSArmoireArray  *pArmoiresArray ;

    int        	    creation ;

    int 		   	    ArmoireChoisie ;
    char            *pCodeArmoireChoisie ;

    EnregChemDialog(TWindow *, NSChemiseData *, char *, NSContexte *pCtx) ;
    ~EnregChemDialog() ;

    void            SetupWindow() ;

    void            CmOk() ;
    void            CmCancel() ;
    void            CmNouvArmoire() ;

	DECLARE_RESPONSE_TABLE(EnregChemDialog) ;
} ;

class _CLASSENSDN NSEditDocTypesDlg : public NSUtilDialog
{
	public:

		NSEditDocTypesDlg(TWindow* pere, NSContexte* pCtx, VecteurString* paTypes) ;
		~NSEditDocTypesDlg() ;

  protected:

    NSUtilLexique* _pEdit ;
    TListWindow*   _pList ;
    VecteurString* _paTypes ;

    void SetupWindow() ;
    void InitList() ;
    void DisplayList() ;

    void CmUp() ;
    void CmDown() ;
    void CmInsert() ;
    void CmUpdate() ;
    void CmDelete() ;

    void CmOk() ;
    void CmCancel() ;

    int IndexItemSelect() ;

	DECLARE_RESPONSE_TABLE(NSEditDocTypesDlg) ;
};

#endif // __NSDOCDLG_H__

