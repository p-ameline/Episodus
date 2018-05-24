#ifndef __NSPATHORBDM_H
#define __NSPATHORBDM_H

class NSEditBdm ;

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

class ChoixBdmDialog ;
//*******************************************************************
//						classe List   ListeBoxBdm
//*******************************************************************
class ListeBoxBdm : public OWL::TListBox
{
  public:

    ChoixBdmDialog* _pChoixBdmDialog ;

    ListeBoxBdm(TWindow* parent, int ressource, ChoixBdmDialog* pChoixBdm, TModule* module) ;
    ~ListeBoxBdm() ;

    void EvKeyDown(uint key, uint repeatCount, uint flags) ;
    void CmPathoDblClk() ;
    void LexiqueMedicament() ;
    void LexiqueGeneral() ;

  DECLARE_RESPONSE_TABLE(ListeBoxBdm) ;
};

//
// Objet "Boite de dialogue" utilise pour selectionner un élément de la BDM
//
class _NSOUTILCLASSE ChoixBdmDialog : public TDialog
{
  public:

    NSBdmDriver::BAMTABLETYPE _iTableType ;

    ListeBoxBdm*              _pListeBox ;
		NSBdmEntryArray           _aListeArray ;
		int 			                _iElementChoisi ;
		string			              _sAmmorce ;
    NSEditBdm*                _pEdit ;
    NSBdmDriver*              _pDriver ;
    bool 			                _bSetup ;

		ChoixBdmDialog(TWindow* parent, NSBdmDriver* pDriver, NSEditBdm* pEdit,
                   NSBdmDriver::BAMTABLETYPE iTableType,
                   TModule* module = (TModule*) 0) ;
		~ChoixBdmDialog() ;

		void SetupArray() ;
		void SetupListe() ;
		void SetupWindow() ;
		int  DonneAmmorce(string* pAmmorce) ;

		void CmSelectPatho(WPARAM Cmd) ;

		void CmPathoDblClk(WPARAM Cmd) ;

    string GetCode() ;

    void showDialog() ;
		void hideDialog() ;
    void InsererElementLexique() ; //inserer un element du lexique suite a la touche Inserer

    void FormerElement() ;         //selection de l'element precedent
    void NextElement() ;           //selection de l'element suivant

		void OuvreDocument(int NumDoc) ;
    void SelectCodeLexique(string sCodeLexique) ;
    void GetSelectedLabel(string* pChaineRecuperee) ;
    void EvKeyDown(uint key, uint repeatCount, uint flags) ;
    void ConvertitMajuscule(char* chaine) ;
    bool IsOpen() { return _bIsOpen ; }
    void setEdit(NSEditBdm* pE) ;
    void EvClose() ;
    void EvDestroy() ;
    void CmOk() ;
    void CmCancel() ;
    bool doesBelong(THandle hWndFocus) ;

    void ElementWasSelected() ;
    bool hasSelectedElement() ;

    bool isProperChar(char cChar) ;

    string getAmmorce()           { return _sAmmorce ; }
    void   setAmmorce(string sAm) { _sAmmorce = sAm ; }

    NSEditBdm* getEdit()          { return _pEdit ; }

  protected :

    TWindow* _pFenetreAppel ;  // HANDLE SUR LA BOITE DIALOGUE
    bool 	   _bIsOpen ;   	   // Boite de dialogue fermee ou ouverte ?

	DECLARE_RESPONSE_TABLE(ChoixBdmDialog) ;
};

#endif // __NSPATHORBDM_H

