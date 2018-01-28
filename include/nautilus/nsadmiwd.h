// Dialogue de la fiche administrative RS Novembre 1997#ifndef __NSADMIN_H
#define __NSADMIN_H

#include <owl\dialog.h>
#include <owl\edit.h>
#include <owl\listbox.h>

#include "nsbb\nsednum.h"
#include "nscompta\nsvitale.h"
#include "nscompta\nscpta.h"
#include "nscompta\nsfse16.h"
#include "nscompta\nsfseuti.h"

#if defined(_DANSCPTADLL)
	#define _CLASSELEXI __export
#else
	#define _CLASSELEXI __import
#endif

// objet contenant les méthodes compta (pour un patient donné)
class NSComptaFct : public NSRoot
{
	public :
		NSComptaFct(NSContexte* pCtx, NSPersonInfo* pPat = 0) ;    ~NSComptaFct() ;

};

// objet contenant les méthodes compta (pour un patient donné)class _CLASSELEXI NSComptaPatient : public NSRoot
{
	public :
		NSPersonInfo* _pPatient ;		NSComptaPatient(NSContexte* pCtx, NSPersonInfo* pPat = 0) ;
    ~NSComptaPatient() ;

    // Creation / modification de la carte vitale du patient
    bool CherchePatEnCours(string& sNumSS) ;
    bool SaisieNumSS(string& sNumSS) ;
    bool ChercheNumSS(string& sNumSS, NSVitale1Info* pCarteInfo) ;
    bool InitDataCarteVitale(NSVitale1Info* pCarteInfo, bool bCreer) ;
    // Creation / modification de la fiche compt
    bool GetComptForDoc(string& sNumCompt, string sDocID, string sPatID = string("")) ;
    bool SetComptForDoc(string sNumCompt, string sDocID, string sPatID = string("")) ;
    bool GetDocsForCompt(string sNumCompt, NSComptDocuArray *pComptDocuArray) ;
    bool GetCcamInfoForCompt(string sNumCompt, NSFse16Array *paFseCCAM) ;
    bool ChercheNumCompt(string& sNumCompt) ;
    bool CreerFicheCompt(NSComptInfo* pComptInfo, NSFse1610Info* pFse1610Info, NSFse16Array* pFseArray = 0, string sDocID = string("")) ;
    void   InitComptInfoFromDocument(NSComptInfo* pComptInfo, NSFse16Array* pFseCcamArray, NVLdVTemps *pDate, const NSPatPathoArray *pPpt) ;
    string GetCcamFromDocument(const NSPatPathoArray *pPpt) ;
    void   InitCcamFromDocument(NSComptInfo* pComptInfo, NSFse16Array* pFseCcamArray, const NSPatPathoArray *pPpt) ;
    bool getComptInfo(string sNumCompt, NSComptInfo *pComptInfo) ;
    bool EditCompt(string sNumCompt, TWindow* pere, NSComptInfo *pComptInfo = 0, NSFse1610Info* pFse1610Info = 0, NSFse16Array* pFseArray = 0) ;
    bool DeleteCompt(string sNumCompt, bool bCallCptaDll = true) ;
    bool DeleteComptDocu(string sNumCompt, string sPatId, string sDocId) ;
    bool DeleteAllComptDocu(string sNumCompt) ;
    // points d'entrée
    void CmCarteVitale() ;
    bool CmFicheCompt(NSComptInfo* pComptInit = 0, NSFse1610Info* pFse1610Init = 0, NSFse16Array* pFseArray = 0) ;
    bool modifyFicheCompt(NSComptInfo* pComptInit, NSFse1610Info* pFse1610Init = 0, NSFse16Array* pFseArray = 0) ;
    void CmSituation() ;

    void Export(string sAction, string sNumCompt, string sPatientId) ;
};

#endif
