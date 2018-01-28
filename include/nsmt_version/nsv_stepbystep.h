//---------------------------------------------------------------------------
#ifndef __NSV_STEPBYSTEP_H
#define __NSV_STEPBYSTEP_H

#include <owl/edit.h>

#include "nsmt_version\nsv_upgrade.h"

//
// Objet "Boite de dialogue" utilisé pour créer une équipe de santé
//
class NsvHealthTeamDialog : public NSUtilDialog
{
	public:

		OWL::TStatic*	pNom ;
		OWL::TStatic*	pPrenom ;

		OWL::TButton*	pChercherPatient ;
		OWL::TButton*	pCreerHealthTeam ;

		string _sPatCode ;

		NsvHealthTeamDialog(TWindow* pere, NSContexte* pCtx) ;
		~NsvHealthTeamDialog() ;

		void SetupWindow() ;
		void GetPatient() ;
		void CreateHealthTeam() ;
    bool CreateHealthTeam_ReadModel(string *pFileContent) ;
    bool CreateHealthTeam_WriteFile(string *pFileContent) ;
    bool CreateHealthTeam_Execute() ;

	DECLARE_RESPONSE_TABLE(NsvHealthTeamDialog) ;
};

//
// Objet "Boite de dialogue" utilisé pour transférer les documents d'un
// patient de l'ancien Nautilus vers un patient du nouveau Nautilus
//
class NsvDocByDocDialog : public NSUtilDialog
{
	public:

		OWL::TStatic* pNomNew ;
		OWL::TStatic* pPrenomNew ;
		OWL::TButton* pChercherNewPatient ;

    OWL::TEdit*   pOldNss ;
    OWL::TStatic* pNomOld ;
		OWL::TStatic* pPrenomOld ;
    OWL::TButton* pChercherOldPatient ;

		OWL::TButton*	pGetDocuments ;

		string _sNewPatCode ;
    string _sOldPatCode ;

    NSPatInfo *_pPatEncours ;

    NSVUpgrade *_pNSVUpgrade ;

    NSDocumentInfo *_pLibChem ;

		NsvDocByDocDialog(TWindow* pere, NSContexte* pCtx, NSVUpgrade* pNSVUpgrade) ;
		~NsvDocByDocDialog() ;

		void SetupWindow() ;
		void GetNewPatient() ;
    void GetOldPatient() ;
    void UpdateChemises() ;
		void MoveDocuments() ;
    void CommitChanges() ;

	DECLARE_RESPONSE_TABLE(NsvDocByDocDialog) ;
};

//
// Objet "Boite de dialogue" utilisé pour réintégrer un doublon
//
class NsvDoublonsDialog : public NSUtilDialog
{
	public:

    OWL::TEdit*   pOldNss ;
		OWL::TStatic*	pNom ;
		OWL::TStatic*	pPrenom ;
    OWL::TEdit*   pNewPrenom ;

		OWL::TButton*	pChercherPatient ;
		OWL::TButton*	pCreerDoublon ;

		string _sOldPatCode ;

    NSVUpgrade *_pNSVUpgrade ;

		NsvDoublonsDialog(TWindow* pere, NSContexte* pCtx, NSVUpgrade* pNSVUpgrade) ;
		~NsvDoublonsDialog() ;

		void SetupWindow() ;
		void GetOldPatient() ;
		void CreateDoublon() ;
    bool CreateDoublon_CheckTranscode() ;
    bool CreateDoublon_EraseTranscode() ;
    bool CreateDoublon_StartReprise(string sNewFirstName = string("")) ;

	DECLARE_RESPONSE_TABLE(NsvDoublonsDialog) ;
};

//
// Objet "Boite de dialogue" utilisé pour réintégrer un doublon
//
class NsvCptaDoublonsDialog : public NSUtilDialog
{
	public:

    OWL::TEdit*   pOldNss ;
		OWL::TStatic*	pNom ;
		OWL::TStatic*	pPrenom ;

		OWL::TButton*	pChercherPatient ;
		OWL::TButton*	pCreerDoublon ;

		string _sOldPatCode ;

    NSVUpgrade *_pNSVUpgrade ;

		NsvCptaDoublonsDialog(TWindow* pere, NSContexte* pCtx, NSVUpgrade* pNSVUpgrade) ;
		~NsvCptaDoublonsDialog() ;

		void SetupWindow() ;
		void GetOldPatient() ;
		void CreateDoublon() ;
    bool CreateDoublon_CheckTranscode() ;
    bool CreateDoublon_EraseTranscode() ;
    bool CreateDoublon_StartReprise() ;

	DECLARE_RESPONSE_TABLE(NsvCptaDoublonsDialog) ;
};

#endif

