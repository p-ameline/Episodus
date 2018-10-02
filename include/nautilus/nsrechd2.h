#ifndef __NSRECHD2_H#define __NSRECHD2_H

//#include <classlib\arrays.h>

#include <owl\dialog.h>
#include <owl\listbox.h>
#include <owl\combobox.h>
#include <owl\groupbox.h>
#include <owl\checkbox.h>
#include <owl\edit.h>
#include <owl\scrollba.h>

//#include "nsbb\nsednum.h"
#include "nsdn\nsanxary.h"
#include "nsbb\nsutidlg.h"

//---------------------------------------------------------------------------
//  Classe NSModule
//---------------------------------------------------------------------------

//
// Taille des champs du fichier MODULES.LUS
//
#define MODU_CODE_LEN  	 1
#define MODU_LEXIQUE_LEN 6
#define MODU_COMPTE_LEN  2

//
// Objet donn�es
//
//class _TYPEDECLASSE NSModuleData
class NSModuleData
{
	public :

		//
		// Variables de stockage des valeurs
		//
		char code[MODU_CODE_LEN + 1] ;
		char lexique[MODU_LEXIQUE_LEN + 1] ;
		char compte[MODU_COMPTE_LEN + 1] ;

		string libelle ;
		string sImportDll ;

		void metAZero() ;

		void initialiseLibelle() ;

		NSModuleData() { metAZero() ; }
		NSModuleData(const NSModuleData& rv) ;

		NSModuleData& operator=(const NSModuleData& src) ;
		int 		      operator==(const NSModuleData& o) ;
};

//
// Objet "Info" correspondant � NSModule
// (destin� � �tre utilis� dans les containers)
//
//class _TYPEDECLASSE NSModuleInfo
class NSModuleInfo
{
	public:

		NSModuleInfo() ;
		NSModuleInfo(const NSModuleInfo& rv) ;
		~NSModuleInfo() ;
		NSModuleInfo& operator=(const NSModuleInfo& src) ;
		int           operator==(const NSModuleInfo& o) ;

    NSModuleData* getData() { return &_Donnees ; }

    string getCode()    { return string(_Donnees.code) ;    }
		string getLexique() { return string(_Donnees.lexique) ; }
		string getCompte()  { return string(_Donnees.compte) ;  }

    string getLabel()     { return _Donnees.libelle ; }
		string getImportDll() { return _Donnees.sImportDll ; }

    void   setCode(string sC)    { strcpy(_Donnees.code, sC.c_str()) ;    }
		void   setLexique(string sL) { strcpy(_Donnees.lexique, sL.c_str()) ; }
		void   setCompte(string sC)  { strcpy(_Donnees.compte, sC.c_str()) ;  }

    void   setLabel(string sL)     { _Donnees.libelle    = sL ; }
		void   setImportDll(string sI) { _Donnees.sImportDll = sI ; }

    void   metAZero() { _Donnees.metAZero() ; }

  protected:

    NSModuleData _Donnees ;
};

//
// D�finition de NSModuleArray (Array de NSModule(s))
//
//typedef TArrayAsVector<NSModuleInfo> NSModuInfoArray;

typedef vector<NSModuleInfo*> NSModuInfoArray ;
typedef NSModuInfoArray::iterator       NSModuInfoArrayIter ;
typedef NSModuInfoArray::const_iterator NSModuInfoArrayConstIter ;

class NSModuleArray : public NSModuInfoArray
{
	public :

		// Constructeurs
		NSModuleArray() : NSModuInfoArray() {}
		NSModuleArray(const NSModuleArray& rv) ;
		// Destructeur
		virtual ~NSModuleArray() ;

    NSModuleArray& operator=(const NSModuleArray& src) ;
		void vider() ;
};

//
// Objet "Info" correspondant � NSModule
// (destin� � �tre utilis� dans les containers)
//
//class _TYPEDECLASSE NSArchtypeInfo
class NSArchetypeInfo
{
	public:

		string _sArchetype ;
		string _sLibelle ;

		NSArchetypeInfo() ;
    NSArchetypeInfo(string sId, string sLabel) ;
		NSArchetypeInfo(const NSArchetypeInfo& rv) ;
		~NSArchetypeInfo() ;

		NSArchetypeInfo& operator=(const NSArchetypeInfo& src) ;
		int              operator==(const NSArchetypeInfo& o) ;

    string getArchetype()            { return _sArchetype ; }
    void   setArchetype(string sArc) { _sArchetype = sArc ; }

    string getLibelle()              { return _sLibelle ; }
    void   setLibelle(string sLib)   { _sLibelle = sLib ; }
};

//
// D�finition de NSModuleArray (Array de NSModule(s))
//
//typedef TArrayAsVector<NSModuleInfo> NSModuInfoArray;


typedef vector<NSArchetypeInfo*> NSArcheInfoArray ;
typedef NSArcheInfoArray::iterator       NSArchInfoArrayIter ;
typedef NSArcheInfoArray::const_iterator NSArchInfoArrayConstIter ;

class NSArchetypeArray : public NSArcheInfoArray
{
	public :

		// Constructeurs
		NSArchetypeArray() : NSArcheInfoArray() {}
		NSArchetypeArray(const NSArchetypeArray& rv) ;
		// Destructeur
		virtual ~NSArchetypeArray() ;
		void vider() ;

    NSArcheInfoArray& operator=(const NSArcheInfoArray& src) ;
};

//
// Objet "Boite de dialogue" utilis� pour choisir le type de document
// pour une consulation
class NSUtilLexique;
//---------------------------------------------------------------
//fixer le type de document
//---------------------------------------------------------------
#ifdef _INCLUS
class NSDialogWrapper;
#endif

#ifdef _INCLUS
class NSTypeDocument : public NSDialogWrapper
#else
class NSTypeDocument : public NSUtilDialog
#endif
{
	public :

		NSUtilLexique* pType ;
		string*        pTypeDocum ;
		OWL::TButton*  pConsultation ;

		NSTypeDocument(TWindow* pere, NSContexte* pCtx, string* pTypeDocument,
                       TModule* module = 0) ;
		~NSTypeDocument() ;

		void CmOk() ;
		void CmCancel() ;
		void SetupWindow() ;
		void ChoixConsultation() ;

	DECLARE_RESPONSE_TABLE(NSTypeDocument) ;
};

//
// Objet "Boite de dialogue" utilis� pour choisir le type de compte rendu
// � g�n�rer
//
//class _TYPEDECLASSE ChoixCRDialog : public TDialog
#ifdef _INCLUS
class NSDialogWrapper;
#endif

#ifdef _INCLUS
class  ChoixCRDialog : public NSDialogWrapper
#else
class  ChoixCRDialog : public NSUtilDialog
#endif
{
	public:

		char            lexiqueCR[MODU_LEXIQUE_LEN+1], Contexte ;

		OWL::TListBox* 	pModulesBox ;
		NSModuleArray*  pModulesArray ;
		OWL::TGroupBox* pContexteGroup ;
		OWL::TCheckBox* pContexteBox[2] ;
		int			    iModuleChoisi ;

		string* 		pChoisi ;
		string* 		pImportDll ;

		NSContexte* 	pContexte ;

#ifdef _INCLUS
		ChoixCRDialog(TWindow* , string* pChoix, NSContexte* pCtx, TModule* module = 0) ;
#else
		ChoixCRDialog(TWindow* , string* pChoix, string* pImport, NSContexte* pCtx) ;
#endif
		~ChoixCRDialog() ;

		void SetupWindow() ;
		void CmSelectModule(WPARAM Cmd) ;
		void CmOk() ;
		void CmCancel() ;
		void CmModuleDblClk(WPARAM Cmd) ;

	DECLARE_RESPONSE_TABLE(ChoixCRDialog) ;
};

// Dialog box to select an Archetype from referenced ones
//
class SelectReferencedArchetypeDialog : public NSUtilDialog
{
	public:

		OWL::TListBox* 	    _pModulesBox ;
		NSArchetypeArray*   _pModulesArray ;
		int			        		_iSelected ;

    string*             _pSelectedId ;
    string*             _pSelectedName ;

		SelectReferencedArchetypeDialog(TWindow* parent, string* pSelectedId, string* pSelectedName, NSContexte* pCtx) ;
		~SelectReferencedArchetypeDialog() ;

		void SetupWindow() ;
		void CmSelectModule(WPARAM Cmd) ;
		void CmOk() ;
		void CmCancel() ;
		void CmModuleDblClk(WPARAM Cmd) ;

    bool initArchetypesArray() ;

    void setSelectedInfo() ;

	DECLARE_RESPONSE_TABLE(SelectReferencedArchetypeDialog) ;
};

// Dialog box to rename an Archetype entry in archetypes list
//
class RenameArchetypeEntryDialog : public NSUtilDialog
{
	public:

		OWL::TEdit* _pEditBox ;
		int			    _iSelected ;

    string*     _pName ;

		RenameArchetypeEntryDialog(TWindow* parent, string* pName, NSContexte* pCtx) ;
		~RenameArchetypeEntryDialog() ;

		void SetupWindow() ;
		void CmOk() ;
		void CmCancel() ;

	DECLARE_RESPONSE_TABLE(RenameArchetypeEntryDialog) ;
};

class ChoixArchetypeDialog ;

// List box used to manage right click for ChoixArchetypeDialog
//
class ArchetypesListBox : OWL::TListBox
{
  public:

    ArchetypesListBox(NSContexte *pCtx,
                      ChoixArchetypeDialog* parent,
                      int             id,
                      int x, int y, int w, int h,
                      TModule*        module = 0) ;
    ArchetypesListBox(NSContexte *pCtx, ChoixArchetypeDialog* parent, int resourceId, TModule* module = 0) ;

    void EvRButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point) ;
    void CmAddArchetype() ;
    void CmEditName() ;

    virtual int AddString(const char far* str) { return OWL::TListBox::AddString(str) ; }
    virtual int GetSelIndex() const            { return OWL::TListBox::GetSelIndex() ; }
    virtual int SetSelIndex(int index)         { return OWL::TListBox::SetSelIndex(index) ; }
    virtual void  ClearList()                  { OWL::TListBox::ClearList() ; }

  protected:

    NSContexte           *pContexte ;
    ChoixArchetypeDialog *_pParent ;

  DECLARE_RESPONSE_TABLE(ArchetypesListBox) ;
};

// Dialog box used to open an Archetype from user's list
//
class ChoixArchetypeDialog : public NSUtilDialog
{
	public:

		string              _sArchetype ;

		ArchetypesListBox* 	_pModulesBox ;
		NSArchetypeArray*   _pModulesArray ;
		int			        		_iModuleChoisi ;

		string* 		    		_pChoisi ;

		ChoixArchetypeDialog(TWindow* , string* pChoix, NSContexte* pCtx) ;
		~ChoixArchetypeDialog() ;

		void SetupWindow() ;
		void CmSelectModule(WPARAM Cmd) ;
		void CmOk() ;
		void CmCancel() ;
		void CmModuleDblClk(WPARAM Cmd) ;
    void AddArchetype() ;
    void EditName() ;

    bool writeFile() ;
    bool readFile() ;

	DECLARE_RESPONSE_TABLE(ChoixArchetypeDialog) ;
};

#endif   // #ifndef __NSRECHD2_H

