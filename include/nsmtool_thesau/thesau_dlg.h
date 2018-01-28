#ifndef __VIDAL_DLG_H#define __VIDAL_DLG_H

#include <owl\edit.h>
#include <owl\dialog.h>
#include <owl\groupbox.h>
#include <owl\checkbox.h>

#include "nsepisod\flechiesDB.h"
#include "ns_vidalRS\termesdb.h"

//
// Objet "Boite de dialogue" utilisé pour sélectionner l'utilitaire
//
class NsVidalDialog : public NSUtilDialog
{
    public:

        OWL::TCheckBox*	  pPostProcess ;
        OWL::TCheckBox*	  pImportSem ;
        OWL::TCheckBox*	  pImportLex ;
        OWL::TCheckBox*	  pPreProcess ;

        NSLexiqueClassAct*  pLexiqueClass ;
        NSTermeRefIndicAct* pTermeRef ;
        NSTermSynAct*       pRefSyn ;

        NSIndicTermeAct*    pGIndic_TRef_Vrac ;
        NSIndicTermeAct*    pGIndic_TRef_Sem ;
        NSIndicTermeAct*    pTRefTRef ;

        ofstream            logFile ;

	    NsVidalDialog(TWindow* pere, NSContexte* pCtx) ;
	    ~NsVidalDialog() ;

	    void SetupWindow() ;

        //void CmOk();
	    //void CmCancel();

        void vidalPostProcess() ;
        void importSemantique() ;
        void importLexique() ;
        void vidalPreProcess() ;

        int  vidalTrouveCode(string code, string group, bool bCreationCode) ;
        int  vidalTrouveCodeSens(string code, string group, bool bCreationCode) ;
        void vidalGetOneLevel(string sQualifie, VecteurString* pEquivalentItem) ;
        void connecteAGI(VecteurString aVecteurString, long int iCodeVidal, VecteurString* pHistorique) ;
        void connecteARef(VecteurString aVecteurString, long int iCodeVidal, VecteurString* pHistorique, long int iGroupeIndic) ;
        void createSynonyms(string sCodeSens, string sCodeRef, long int iCodeRef) ;

        void chercherSynony(string &lib, int iCodRef, string &sCodeSyn, NSFlechies* pFlechies) ;

    DECLARE_RESPONSE_TABLE(NsVidalDialog) ;
} ;

class NsImportSemDlg : public NSUtilDialog
{
    public:

        bool impTourner ;

		OWL::TEdit* pEditNum ;        OWL::TEdit* pEditQualified ;
        OWL::TEdit* pEditLink ;
        OWL::TEdit* pEditQualifier ;

		NsImportSemDlg(TWindow* pere, NSContexte* pCtx) ;		~NsImportSemDlg() ;

		void SetupWindow() ;
        void demarreCvt() ;        void stoppeCvt() ;

        void afficheNum(string sNum) ;
        void afficheImport(string sQufied, string sLink, string sQufier) ;

    DECLARE_RESPONSE_TABLE(NsImportSemDlg) ;};

#endif  // __VIDAL_DLG_H
