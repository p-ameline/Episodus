#ifndef __NSBDMLISTDLG_H
#define __NSBDMLISTDLG_H

#include <vector>

#ifndef __linux__
#include <owl/dialog.h>
#include <owl/listbox.h>
#include <owl/static.h>
#endif

#include "nsoutil/nsexport.h"
#include "nssavoir/nsBdmDriver.h"

#ifndef __linux__
#if defined(_DANSLEXIDLL)
    #define _CLASSELEXI __export
#else
	#define _CLASSELEXI __import
#endif
#endif

using namespace std ;

class ChoixBdmListDialog ;

//*******************************************************************
//						classe List   ListeBoxBdm
//*******************************************************************
class ListeBoxBdmList : public OWL::TListBox
{
  public:

    ListeBoxBdmList(ChoixBdmListDialog* pChoixBdmDlg, int ressource) ;
    ~ListeBoxBdmList() ;

    void SetupWindow() ;

  protected:

    ChoixBdmListDialog* _pChoixBdmListDialog ;

  DECLARE_RESPONSE_TABLE(ListeBoxBdmList) ;
};

//
// Contrôle Edit avec accès au dictionnaire
//
class _NSOUTILCLASSE NSEditBdmList : public OWL::TEdit
{
  public:

    //
    // Constructeur et destructeur
    //
    NSEditBdmList(ChoixBdmListDialog* parent, int resourceId, uint textLimit = 0) ;
    ~NSEditBdmList() ;

    void   SetupWindow() ;

    void   EvChar(uint key, uint repeatCount, uint flags) ;

  protected :

    ChoixBdmListDialog* _pBdmListDialog ;

  DECLARE_RESPONSE_TABLE(NSEditBdmList) ;
};

//
// Objet "Boite de dialogue" utilise pour selectionner un élément de la BDM
//
class _NSOUTILCLASSE ChoixBdmListDialog : public TDialog
{
  public:

		ChoixBdmListDialog(TWindow* parent, NSBdmDriver* pDriver,
                       NSBdmDriver::BAMTABLETYPE iTableType,
                       NSBdmEntry* pSelectedElement,
                       string	sSeed = string("")) ;
		~ChoixBdmListDialog() ;

    void SetupWindow() ;

		void SetupArray() ;
		void FillList() ;

		void CmPathoDblClk(WPARAM Cmd) ;

    void CmOk() ;
    void CmCancel() ;

  protected :

    NSBdmDriver::BAMTABLETYPE _iTableType ;

    NSEditBdmList*            _pEdit ;
    ListeBoxBdmList*          _pListeBox ;
    OWL::TButton*             _pSearchButton ;

		NSBdmEntryArray           _aListeArray ;
    NSBdmEntry*               _pSelectedElement ;
		string			              _sSeed ;

    NSBdmDriver*              _pDriver ;

    void        CmSearch() ;
    string      GetText() ;

    NSBdmEntry* getSelectedElement() ;

	DECLARE_RESPONSE_TABLE(ChoixBdmListDialog) ;
};

#endif  // __NSBDMLISTDLG_H

