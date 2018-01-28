// nsdb_intra.cpp//////////////////////////////////////////////////////////////////////////
// Fichier des bases intranet - Rémi SPAAK Aout 1997
// Description des classes d'acces aux bases :
// template.db; tempub.db; comphtml.db; workflow.db.
//////////////////////////////////////////////////////////////////////////

#ifndef _ENTERPRISE_DLL
  #include <owl\owlpch.h>
  #include <owl\dc.h>
  #include <owl\inputdia.h>
  #include <owl\chooseco.h>
  #include <owl\gdiobjec.h>
  #include <owl\docmanag.h>
  #include <owl\filedoc.h>
  #include <iostream.h>
  #include <cstring.h>

  #include "partage\nsdivfct.h"
#else
  #include "enterpriseLus/nsdivfctForCgi.h"
#endif

#include "nsdn\nsintrad.h"

#include "pilot\NautilusPilot.hpp"
#include "nsbb\tagNames.h"

//---------------------------------------------------------------------------//  Classe NSTemplateData
//---------------------------------------------------------------------------

NSTemplateData::NSTemplateData()
{
	sCode    = string("") ;
	sType    = string("") ;
	sUser    = string("") ;
	sRoot    = string("") ;
	sCompo   = string("") ;
	sDefaut  = string("") ;
	sEnTete  = string("") ;
	sFichier = string("") ;
	sLibelle = string("") ;
}

//---------------------------------------------------------------------------//  Fonction:		NSTemplateData::NSTemplateData(const NSTemplateData& rv)
//  Description:	Constructeur copie
//  Retour:			Rien
//---------------------------------------------------------------------------
NSTemplateData::NSTemplateData(const NSTemplateData& rv)
{
	sCode    = rv.sCode ;
	sType    = rv.sType ;
	sUser    = rv.sUser ;
	sRoot    = rv.sRoot ;
	sCompo   = rv.sCompo ;
	sDefaut  = rv.sDefaut ;
	sEnTete  = rv.sEnTete ;
	sFichier = rv.sFichier ;
	sLibelle = rv.sLibelle ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSTemplateData::operator=(NSTemplateData src)
//  Description:	Opérateur =
//  Retour:			Référence sur l'objet cible
//---------------------------------------------------------------------------
NSTemplateData&
NSTemplateData::operator=(const NSTemplateData& src)
{
	if (&src == this)
		return *this ;

	sCode    = src.sCode ;
	sType    = src.sType ;
	sUser    = src.sUser ;
	sRoot    = src.sRoot ;
	sCompo   = src.sCompo ;
	sDefaut  = src.sDefaut ;
	sEnTete  = src.sEnTete ;
	sFichier = src.sFichier ;
	sLibelle = src.sLibelle ;

	return *this;
}

//---------------------------------------------------------------------------
//  Fonction:		NSTemplateData::operator==(const NSTemplateData& o)
//  Description:	Opérateur de comparaison
//  Retour:			0 ou 1
//---------------------------------------------------------------------------
int
NSTemplateData::operator == ( const NSTemplateData& o )
{
	if ((sCode    == o.sCode   ) &&
	    (sType    == o.sType   ) &&
	    (sUser    == o.sUser   ) &&
	    (sRoot    == o.sRoot   ) &&
	    (sCompo   == o.sCompo  ) &&
	    (sDefaut  == o.sDefaut ) &&
	    (sEnTete  == o.sEnTete ) &&
	    (sFichier == o.sFichier) &&
	    (sLibelle == o.sLibelle))
		return 1;
	else
		return 0;
}

//---------------------------------------------------------------------------
//  Classe NSTemplateInfo
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  Fonction:		NSTemplateInfo::NSTemplateInfo()
//  Description:	Constructeur par défaut
//  Retour:			Rien
//---------------------------------------------------------------------------
NSTemplateInfo::NSTemplateInfo()
{
	// Crée l'objet de données
	pDonnees = new NSTemplateData() ;
}

NSTemplateInfo::NSTemplateInfo(NSBasicAttributeArray *pTmplArray)
{
	pDonnees = new NSTemplateData() ;

	string sTraitType = string("_0OTPL") + string("_0TYPE");
  string sTraitOper = string("_0OTPL") + string("_DOPER");
  string sTraitCons = string("_0OTPL") + string("_LNUCO");
  string sTraitRoot = string("_0OTPL") + string("_0TYPC");
  string sTraitComp = string("_0OTPL") + string("_0COMD");
  string sTraitDefa = string("_0OTPL") + string("_0DEFA");
  string sTraitFich = string("_0OTPL") + string("_0TPL0");
  string sTraitEnte = string("_0OTPL") + string("_0ENTE");
  string sTraitLibe = string("_0OTPL") + string("_0INTI");

  setCode(pTmplArray->getAttributeValue(OBJECT)) ;
  setType(pTmplArray->getAttributeValue(sTraitType)) ;
  setUser(pTmplArray->getAttributeValue(sTraitOper)) ;
  setRoot(pTmplArray->getAttributeValue(sTraitRoot)) ;
  setCompo(pTmplArray->getAttributeValue(sTraitComp)) ;
  setDefaut(pTmplArray->getAttributeValue(sTraitDefa)) ;
  setEnTete(pTmplArray->getAttributeValue(sTraitEnte)) ;
  setFichier(pTmplArray->getAttributeValue(sTraitFich)) ;
  setLibelle(pTmplArray->getAttributeValue(sTraitLibe)) ;

  sNumCons = pTmplArray->getAttributeValue(sTraitCons) ;
}

NSTemplateInfo::~NSTemplateInfo(){
	delete pDonnees ;
}

NSTemplateInfo::NSTemplateInfo(const NSTemplateInfo& rv)
{
	pDonnees = new NSTemplateData(*(rv.pDonnees)) ;
}

//-------------------------------------------------------------------------
//----------------------- METHODES DE NSTemplateArray ---------------------
//-------------------------------------------------------------------------

// constructeur copie
NSTemplateArray::NSTemplateArray(const NSTemplateArray& rv)                :NSTemplateVector()
{
  if (false == rv.empty())
    for (NSTemplateConstIter it = rv.begin() ; rv.end() != it ; it++)
      push_back(new NSTemplateInfo(*(*it))) ;
}

// fonction vider()
void
NSTemplateArray::vider()
{
	for (NSTemplateIter it = begin() ; end() != it ; )
	{
  	delete *it ;
    erase(it) ;
	}
}

// destructeur
NSTemplateArray::~NSTemplateArray()
{
	vider() ;
}

//---------------------------------------------------------------------------
//  Fonction	 : NSTemplateArray::operator=(NSTemplateArray src)
//
//  Description : Opérateur d'affectation
//---------------------------------------------------------------------------
NSTemplateArray&
NSTemplateArray::operator=(const NSTemplateArray& src)
{
	if (this == &src)
		return *this ;

	vider() ;

  if (false == src.empty())
	  for (NSTemplateConstIter it = src.begin() ; src.end() != it ; it++)
		  push_back(new NSTemplateInfo(*(*it))) ;

	return *this ;
}

//===========================================================================
// CLASSE NSTEMPUB
//---------------------------------------------------------------------------
//  Classe NSTempubData
//---------------------------------------------------------------------------

/*

NSTempubData::NSTempubData(){
	// met les champs de donnees a zero

   memset(code_compo,		0,	TMPUB_CODE_COMPO_LEN + 1);
   memset(type_action,		0, TMPUB_TYPE_ACTION_LEN + 1);
   memset(defaut,				0, TMPUB_DEFAUT_LEN + 1);
   memset(code_tmpl,   		0,	TMPUB_CODE_TMPL_LEN + 1);
}

//---------------------------------------------------------------------------
//  Fonction:		NSTempubData::NSTempubData(const NSTempubData& rv)
//  Description:	Constructeur copie
//  Retour:			Rien
//---------------------------------------------------------------------------
NSTempubData::NSTempubData(const NSTempubData& rv)
{
   strcpy(code_compo,   	rv.code_compo);
	strcpy(type_action, 		rv.type_action);
	strcpy(defaut, 			rv.defaut);
	strcpy(code_tmpl, 		rv.code_tmpl);
}

//---------------------------------------------------------------------------
//  Fonction:		NSTempubData::operator=(NSTempubData src)
//  Description:	Opérateur =
//  Retour:			Référence sur l'objet cible
//---------------------------------------------------------------------------
NSTempubData&
NSTempubData::operator=(NSTempubData src)
{
   strcpy(code_compo,   	src.code_compo);
	strcpy(type_action, 		src.type_action);
	strcpy(defaut, 			src.defaut);
	strcpy(code_tmpl,		 	src.code_tmpl);

	return *this;
}

//---------------------------------------------------------------------------
//  Fonction:		NSTempubData::operator==(const NSTempubData& o)
//  Description:	Opérateur de comparaison
//  Retour:			0 ou 1
//---------------------------------------------------------------------------
int
NSTempubData::operator == ( const NSTempubData& o )
{

	if ((strcmp(code_compo, 	o.code_compo) 	== 0) &&
		 (strcmp(type_action,   o.type_action) == 0) &&
		 (strcmp(defaut, 			o.defaut) 		== 0) &&
       (strcmp(code_tmpl,		o.code_tmpl)	== 0))
		return 1;
	else
		return 0;
}

//---------------------------------------------------------------------------
//  Classe NSTempub
//---------------------------------------------------------------------------

NSTempub::NSTempub(NSContexte* pCtx) : NSFiche(pCtx)
{
	// cree un objet de donnees
    pDonnees = new NSTempubData();
}

NSTempub::~NSTempub()
{
	delete pDonnees;
   pDonnees = 0;
}

// Ouverture et fermeture de la base des templates
//////////////////////////////////////////////////

DBIResult
NSTempub::open()
{
  char tableName[] = "TEMPUB.DB";
  //
  // Appelle la fonction open() de la classe de base pour ouvrir
  // l'index primaire
  //
  lastError = NSFiche::open(tableName, NSF_PARTAGE_GLOBAL);
  return(lastError);
}

// alimenteFiche
/////////////////////////////////////////////////////////////////////////

void
NSTempub::alimenteFiche()
{
   alimenteChamp(pDonnees->code_compo,		TMPUB_CODE_COMPO_FIELD, 	TMPUB_CODE_COMPO_LEN);
   alimenteChamp(pDonnees->type_action,	TMPUB_TYPE_ACTION_FIELD, 	TMPUB_TYPE_ACTION_LEN);
   alimenteChamp(pDonnees->defaut,			TMPUB_DEFAUT_FIELD, 			TMPUB_DEFAUT_LEN);
	alimenteChamp(pDonnees->code_tmpl,		TMPUB_CODE_TMPL_FIELD, 		TMPUB_CODE_TMPL_LEN);
}

// videFiche
/////////////////////////////////////////////////////////////////////////

void
NSTempub::videFiche()
{
  	videChamp(pDonnees->code_compo,		TMPUB_CODE_COMPO_FIELD, 	TMPUB_CODE_COMPO_LEN);
   videChamp(pDonnees->type_action,		TMPUB_TYPE_ACTION_FIELD, 	TMPUB_TYPE_ACTION_LEN);
   videChamp(pDonnees->defaut,			TMPUB_DEFAUT_FIELD, 			TMPUB_DEFAUT_LEN);
	videChamp(pDonnees->code_tmpl,		TMPUB_CODE_TMPL_FIELD, 		TMPUB_CODE_TMPL_LEN);
}

//---------------------------------------------------------------------------
//  Classe NSTempubInfo
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  Fonction:		NSTempubInfo::NSTempubInfo()
//  Description:	Constructeur par défaut
//  Retour:			Rien
//---------------------------------------------------------------------------
NSTempubInfo::NSTempubInfo()
{
	// Crée l'objet de données
	pDonnees = new NSTempubData();
}

//---------------------------------------------------------------------------
//  Fonction:		NSTempubInfo::NSTempubInfo(NSTempub*)
//  Description:	Constructeur à partir d'un NSTempub
//  Retour:			Rien
//---------------------------------------------------------------------------
NSTempubInfo::NSTempubInfo(NSTempub* pTempub)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSTempubData();
	//
	// Copie les valeurs du NSCorrespondant
	//
	*pDonnees = *(pTempub->pDonnees);
}

//---------------------------------------------------------------------------
//  Fonction:		NSTempubInfo::~NSTempubInfo()
//  Description:	Destructeur
//  Retour:			Rien
//---------------------------------------------------------------------------
NSTempubInfo::~NSTempubInfo()
{
	delete pDonnees;
}

//---------------------------------------------------------------------------
//  Fonction:		NSTempubInfo::NSTempubInfo(const NSTempubInfo& rv)
//  Description:	Constructeur copie
//  Retour:			Rien
//---------------------------------------------------------------------------
NSTempubInfo::NSTempubInfo(const NSTempubInfo& rv)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSTempubData();
	//
	// Copie les valeurs du NSTempubInfo d'origine
	//
	*pDonnees = *(rv.pDonnees);
}

//---------------------------------------------------------------------------
//  Fonction:		NSTempubInfo::operator=(NSTempubInfo src)
//  Description:	Opérateur d'affectation
//  Retour:			Référence de l'objet cible
//---------------------------------------------------------------------------
NSTempubInfo&
NSTempubInfo::operator=(NSTempubInfo src)
{
	*pDonnees = *(src.pDonnees);
	return *this;
}

//---------------------------------------------------------------------------
//  Fonction:		NSTempubInfo::operator==(NSTempubInfo src)
//  Description:	Opérateur de comparaison
//  Retour:			Référence de l'objet cible
//---------------------------------------------------------------------------
int NSTempubInfo::operator == ( const NSTempubInfo& o )
{
	 return (*pDonnees == *(o.pDonnees));
}

//-------------------------------------------------------------------------
//----------------------- METHODES DE NSTempubArray ---------------------
//-------------------------------------------------------------------------
// constructeur copie

NSTempubArray::NSTempubArray(NSTempubArray& rv)
					  : NSTempubVector()
{
	if (!(rv.empty()))
	   for (NSTempubIter i = rv.begin(); i != rv.end(); i++)
			 	push_back(new NSTempubInfo(*(*i)));
}

// fonction vider()
void
NSTempubArray::vider()
{
	for (NSTempubIter i = begin(); i != end(); )
   {
   	delete *i;
      erase(i);
   }
}

// destructeur
NSTempubArray::~NSTempubArray()
{
	vider();
}

//---------------------------------------------------------------------------
//  Fonction	 : NSTempubArray::operator=(NSTempubArray src)
//
//  Description : Opérateur d'affectation
//---------------------------------------------------------------------------
NSTempubArray&
NSTempubArray::operator=(NSTempubArray src)
{
	vider();
   for (NSTempubIter i = src.begin(); i != src.end(); i++)
			 	push_back(new NSTempubInfo(*(*i)));
	return *this;
}

*/

//===========================================================================
// CLASSE NSWORKFLOW
//---------------------------------------------------------------------------
//  Classe NSWorkflowData
//---------------------------------------------------------------------------

/*

NSWorkflowData::NSWorkflowData(){
	// met les champs de donnees a zero

	memset(code_docu,		0, WF_CODE_DOCU_LEN + 1);
   memset(code_dest,		0, WF_CODE_DEST_LEN + 1);
   memset(type_publi,	0, WF_TYPE_PUBLI_LEN + 1);
   memset(date_publi,	0, WF_DATE_PUBLI_LEN + 1);
   memset(heure_publi,	0,	WF_HEURE_PUBLI_LEN + 1);
   memset(nb_publi,		0, WF_NB_PUBLI_LEN + 1);
   memset(entet_publi,  0, WF_ENTET_PUBLI_LEN + 1);
   memset(tmpl_publi,	0, WF_TMPL_PUBLI_LEN + 1);
   memset(ar_publi,		0, WF_AR_PUBLI_LEN + 1);
   memset(valide,			0, WF_VALIDE_LEN + 1);
}

//---------------------------------------------------------------------------
//  Fonction:		NSWorkflowData::NSWorkflowData(const NSWorkflowData& rv)
//  Description:	Constructeur copie
//  Retour:			Rien
//---------------------------------------------------------------------------
NSWorkflowData::NSWorkflowData(const NSWorkflowData& rv)
{
   strcpy(code_docu,		rv.code_docu);
   strcpy(code_dest,		rv.code_dest);
   strcpy(type_publi,	rv.type_publi);
   strcpy(date_publi,	rv.date_publi);
   strcpy(heure_publi,	rv.heure_publi);
   strcpy(nb_publi,		rv.nb_publi);
   strcpy(entet_publi,	rv.entet_publi);
   strcpy(tmpl_publi,	rv.tmpl_publi);
   strcpy(ar_publi,		rv.ar_publi);
   strcpy(valide,			rv.valide);
}

//---------------------------------------------------------------------------
//  Fonction:		NSWorkflowData::operator=(NSWorkflowData src)
//  Description:	Opérateur =
//  Retour:			Référence sur l'objet cible
//---------------------------------------------------------------------------
NSWorkflowData&
NSWorkflowData::operator=(NSWorkflowData src)
{
   strcpy(code_docu,		src.code_docu);
   strcpy(code_dest,		src.code_dest);
   strcpy(type_publi,	src.type_publi);
   strcpy(date_publi,	src.date_publi);
   strcpy(heure_publi,	src.heure_publi);
   strcpy(nb_publi,		src.nb_publi);
   strcpy(entet_publi,	src.entet_publi);
   strcpy(tmpl_publi,	src.tmpl_publi);
   strcpy(ar_publi,		src.ar_publi);
   strcpy(valide,			src.valide);

	return *this;
}

//---------------------------------------------------------------------------
//  Fonction:		NSWorkflowData::operator==(const NSWorkflowData& o)
//  Description:	Opérateur de comparaison
//  Retour:			0 ou 1
//---------------------------------------------------------------------------
int
NSWorkflowData::operator == ( const NSWorkflowData& o )
{

	if ((strcmp(code_docu, 		o.code_docu) 	== 0) &&
   	 (strcmp(code_dest,		o.code_dest)   == 0) &&
		 (strcmp(type_publi,   	o.type_publi) 	== 0) &&
		 (strcmp(date_publi, 	o.date_publi) 	== 0) &&
       (strcmp(heure_publi,	o.heure_publi) == 0) &&
       (strcmp(nb_publi,		o.nb_publi)    == 0) &&
       (strcmp(entet_publi,		o.entet_publi)  == 0) &&
       (strcmp(tmpl_publi,		o.tmpl_publi)	== 0) &&
       (strcmp(ar_publi,		o.ar_publi)		== 0) &&
       (strcmp(valide,			o.valide)		== 0))
		return 1;
	else
		return 0;
}

//---------------------------------------------------------------------------
//  Classe NSWorkflow
//---------------------------------------------------------------------------

NSWorkflow::NSWorkflow(NSContexte* pCtx) : NSFiche(pCtx)
{
	// cree un objet de donnees
    pDonnees = new NSWorkflowData();
}

NSWorkflow::~NSWorkflow()
{
	delete pDonnees;
   pDonnees = 0;
}

DBIResult
NSWorkflow::open()
{
  char tableName[] = "WORKFLOW.DB";
  //
  // Appelle la fonction open() de la classe de base pour ouvrir
  // l'index primaire
  //
  lastError = NSFiche::open(tableName, NSF_PARTAGE_GLOBAL);
  return(lastError);
}

// alimenteFiche
/////////////////////////////////////////////////////////////////////////

void
NSWorkflow::alimenteFiche()
{
	alimenteChamp(pDonnees->code_docu,		WF_CODE_DOCU_FIELD,		WF_CODE_DOCU_LEN);
   	alimenteChamp(pDonnees->code_dest,		WF_CODE_DEST_FIELD,		WF_CODE_DEST_LEN);
   	alimenteChamp(pDonnees->type_publi,		WF_TYPE_PUBLI_FIELD,	WF_TYPE_PUBLI_LEN);
   	alimenteChamp(pDonnees->date_publi,		WF_DATE_PUBLI_FIELD,	WF_DATE_PUBLI_LEN);
   	alimenteChamp(pDonnees->heure_publi,	WF_HEURE_PUBLI_FIELD,	WF_HEURE_PUBLI_LEN);
   	alimenteChamp(pDonnees->nb_publi,		WF_NB_PUBLI_FIELD,		WF_NB_PUBLI_LEN);
    alimenteChamp(pDonnees->entet_publi,	WF_ENTET_PUBLI_FIELD,   WF_ENTET_PUBLI_LEN);
   	alimenteChamp(pDonnees->tmpl_publi,		WF_TMPL_PUBLI_FIELD,	WF_TMPL_PUBLI_LEN);
   	alimenteChamp(pDonnees->ar_publi,		WF_AR_PUBLI_FIELD,		WF_AR_PUBLI_LEN);
   	alimenteChamp(pDonnees->valide,			WF_VALIDE_FIELD,		WF_VALIDE_LEN);
}

// videFiche
/////////////////////////////////////////////////////////////////////////

void
NSWorkflow::videFiche()
{
  	videChamp(pDonnees->code_docu,		WF_CODE_DOCU_FIELD,		WF_CODE_DOCU_LEN);
   	videChamp(pDonnees->code_dest,		WF_CODE_DEST_FIELD,		WF_CODE_DEST_LEN);
   	videChamp(pDonnees->type_publi,		WF_TYPE_PUBLI_FIELD,	WF_TYPE_PUBLI_LEN);
   	videChamp(pDonnees->date_publi,		WF_DATE_PUBLI_FIELD,	WF_DATE_PUBLI_LEN);
   	videChamp(pDonnees->heure_publi,	WF_HEURE_PUBLI_FIELD,	WF_HEURE_PUBLI_LEN);
   	videChamp(pDonnees->nb_publi,		WF_NB_PUBLI_FIELD,		WF_NB_PUBLI_LEN);
    videChamp(pDonnees->entet_publi,	WF_ENTET_PUBLI_FIELD,	WF_ENTET_PUBLI_LEN);
   	videChamp(pDonnees->tmpl_publi,		WF_TMPL_PUBLI_FIELD,	WF_TMPL_PUBLI_LEN);
   	videChamp(pDonnees->ar_publi,		WF_AR_PUBLI_FIELD,		WF_AR_PUBLI_LEN);
   	videChamp(pDonnees->valide,			WF_VALIDE_FIELD,		WF_VALIDE_LEN);
}

//---------------------------------------------------------------------------
//  Classe NSWorkflowInfo
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  Fonction:		NSWorkflowInfo::NSWorkflowInfo()
//  Description:	Constructeur par défaut
//  Retour:			Rien
//---------------------------------------------------------------------------
NSWorkflowInfo::NSWorkflowInfo()
{
	// Crée l'objet de données
	pDonnees = new NSWorkflowData();
}

//---------------------------------------------------------------------------
//  Fonction:		NSWorkflowInfo::NSWorkflowInfo(NSWorkflow*)
//  Description:	Constructeur à partir d'un NSWorkflow
//  Retour:			Rien
//---------------------------------------------------------------------------
NSWorkflowInfo::NSWorkflowInfo(NSWorkflow* pWorkflow)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSWorkflowData();
	//
	// Copie les valeurs du NSCorrespondant
	//
	*pDonnees = *(pWorkflow->pDonnees);
}

//---------------------------------------------------------------------------
//  Fonction:		NSWorkflowInfo::~NSWorkflowInfo()
//  Description:	Destructeur
//  Retour:			Rien
//---------------------------------------------------------------------------
NSWorkflowInfo::~NSWorkflowInfo()
{
	delete pDonnees;
}

//---------------------------------------------------------------------------
//  Fonction:		NSWorkflowInfo::NSWorkflowInfo(const NSWorkflowInfo& rv)
//  Description:	Constructeur copie
//  Retour:			Rien
//---------------------------------------------------------------------------
NSWorkflowInfo::NSWorkflowInfo(const NSWorkflowInfo& rv)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSWorkflowData();
	//
	// Copie les valeurs du NSTempubInfo d'origine
	//
	*pDonnees = *(rv.pDonnees);
}

//---------------------------------------------------------------------------
//  Fonction:		NSWorkflowInfo::operator=(NSWorkflowInfo src)
//  Description:	Opérateur d'affectation
//  Retour:			Référence de l'objet cible
//---------------------------------------------------------------------------
NSWorkflowInfo&
NSWorkflowInfo::operator=(NSWorkflowInfo src)
{
	*pDonnees = *(src.pDonnees);
	return *this;
}

//---------------------------------------------------------------------------
//  Fonction:		NSWorkflowInfo::operator==(NSWorkflowInfo src)
//  Description:	Opérateur de comparaison
//  Retour:			Référence de l'objet cible
//---------------------------------------------------------------------------
int NSWorkflowInfo::operator == ( const NSWorkflowInfo& o )
{
	 return (*pDonnees == *(o.pDonnees));
}

//-------------------------------------------------------------------------
//----------------------- METHODES DE NSWorkflowArray ---------------------
//-------------------------------------------------------------------------

// constructeur copie

NSWorkflowArray::NSWorkflowArray(NSWorkflowArray& rv)
					  : NSWorkflowVector()
{
	if (!(rv.empty()))
	   for (NSWorkflowIter i = rv.begin(); i != rv.end(); i++)
			 	push_back(new NSWorkflowInfo(*(*i)));
}

// fonction vider()
void
NSWorkflowArray::vider()
{	for (NSWorkflowIter i = begin(); i != end(); )
   {
   	delete *i;
      erase(i);
   }
}

// destructeur
NSWorkflowArray::~NSWorkflowArray()
{
	vider();
}

//---------------------------------------------------------------------------
//  Fonction	 : NSWorkflowArray::operator=(NSWorkflowArray src)
//
//  Description : Opérateur d'affectation
//---------------------------------------------------------------------------
NSWorkflowArray&
NSWorkflowArray::operator=(NSWorkflowArray src)
{
	vider();
   for (NSWorkflowIter i = src.begin(); i != src.end(); i++)
			 	push_back(new NSWorkflowInfo(*(*i)));
	return *this;
}

*/

//===========================================================================
// CLASSE NSCompHtml
//---------------------------------------------------------------------------
//  Classe NSCompHtmlData
//---------------------------------------------------------------------------

/*

NSCompHtmlData::NSCompHtmlData(){	// met les champs de donnees a zero

   memset(code_html,			0,	CHTML_CODE_HTML_LEN + 1);
   memset(type_comp,			0, CHTML_TYPE_COMP_LEN + 1);
   memset(index_comp,		0, CHTML_INDEX_COMP_LEN + 1);
   memset(code_comp,   		0,	CHTML_CODE_COMP_LEN + 1);
   memset(principal,   		0,	CHTML_PRINCIPAL_LEN + 1);
}

//---------------------------------------------------------------------------//  Fonction:		NSCompHtmlData::NSCompHtmlData(const NSCompHtmlData& rv)
//  Description:	Constructeur copie
//  Retour:			Rien
//---------------------------------------------------------------------------
NSCompHtmlData::NSCompHtmlData(const NSCompHtmlData& rv)
{
   strcpy(code_html,   	rv.code_html);
	strcpy(type_comp, 	rv.type_comp);
	strcpy(index_comp, 	rv.index_comp);
	strcpy(code_comp, 	rv.code_comp);
   strcpy(principal,		rv.principal);
}

//---------------------------------------------------------------------------
//  Fonction:		NSCompHtmlData::operator=(NSCompHtmlData src)
//  Description:	Opérateur =
//  Retour:			Référence sur l'objet cible
//---------------------------------------------------------------------------
NSCompHtmlData&
NSCompHtmlData::operator=(NSCompHtmlData src)
{
   strcpy(code_html,   		src.code_html);
	strcpy(type_comp, 		src.type_comp);
	strcpy(index_comp, 		src.index_comp);
	strcpy(code_comp,		 	src.code_comp);
   strcpy(principal,			src.principal);

	return *this;
}

//---------------------------------------------------------------------------
//  Fonction:		NSCompHtmlData::operator==(const NSCompHtmlData& o)
//  Description:	Opérateur de comparaison
//  Retour:			0 ou 1
//---------------------------------------------------------------------------
int
NSCompHtmlData::operator == ( const NSCompHtmlData& o )
{

	if ((strcmp(code_html, 		o.code_html) 	== 0) &&
		 (strcmp(type_comp,   	o.type_comp) 	== 0) &&
		 (strcmp(index_comp, 	o.index_comp) 	== 0) &&
       (strcmp(code_comp, 		o.code_comp) 	== 0) &&
       (strcmp(principal,		o.principal)	== 0))
		return 1;
	else
		return 0;
}

//---------------------------------------------------------------------------
//  Classe NSCompHtml
//---------------------------------------------------------------------------

NSCompHtml::NSCompHtml(NSContexte* pCtx) : NSFiche(pCtx)
{
	// cree un objet de donnees
    pDonnees = new NSCompHtmlData();
}

NSCompHtml::~NSCompHtml()
{
	delete pDonnees;
   pDonnees = 0;
}

// Ouverture et fermeture de la base des templates
//////////////////////////////////////////////////

DBIResult
NSCompHtml::open()
{
  char tableName[] = "COMPHTML.DB";
  //
  // Appelle la fonction open() de la classe de base pour ouvrir
  // l'index primaire
  //
  lastError = NSFiche::open(tableName, NSF_PARTAGE_GLOBAL);
  return(lastError);
}

// alimenteFiche
/////////////////////////////////////////////////////////////////////////

void
NSCompHtml::alimenteFiche()
{
	alimenteChamp(pDonnees->code_html,		CHTML_CODE_HTML_FIELD,		CHTML_CODE_HTML_LEN);
   alimenteChamp(pDonnees->type_comp,		CHTML_TYPE_COMP_FIELD,		CHTML_TYPE_COMP_LEN);
   alimenteChamp(pDonnees->index_comp,		CHTML_INDEX_COMP_FIELD,		CHTML_INDEX_COMP_LEN);
   alimenteChamp(pDonnees->code_comp,		CHTML_CODE_COMP_FIELD,		CHTML_CODE_COMP_LEN);
   alimenteChamp(pDonnees->principal,		CHTML_PRINCIPAL_FIELD,		CHTML_PRINCIPAL_LEN);
}

// videFiche
/////////////////////////////////////////////////////////////////////////

void
NSCompHtml::videFiche()
{
  	videChamp(pDonnees->code_html,		CHTML_CODE_HTML_FIELD,		CHTML_CODE_HTML_LEN);
   videChamp(pDonnees->type_comp,		CHTML_TYPE_COMP_FIELD,		CHTML_TYPE_COMP_LEN);
   videChamp(pDonnees->index_comp,		CHTML_INDEX_COMP_FIELD,		CHTML_INDEX_COMP_LEN);
   videChamp(pDonnees->code_comp,		CHTML_CODE_COMP_FIELD,		CHTML_CODE_COMP_LEN);
   videChamp(pDonnees->principal,		CHTML_PRINCIPAL_FIELD,		CHTML_PRINCIPAL_LEN);
}

//---------------------------------------------------------------------------
//  Classe NSCompHtmlInfo
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  Fonction:		NSCompHtmlInfo::NSCompHtmlInfo()
//  Description:	Constructeur par défaut
//  Retour:			Rien
//---------------------------------------------------------------------------
NSCompHtmlInfo::NSCompHtmlInfo()
{
	// Crée l'objet de données
	pDonnees = new NSCompHtmlData();
}

//---------------------------------------------------------------------------
//  Fonction:		NSCompHtmlInfo::NSCompHtmlInfo(NSCompHtml*)
//  Description:	Constructeur à partir d'un NSCompHtml
//  Retour:			Rien
//---------------------------------------------------------------------------
NSCompHtmlInfo::NSCompHtmlInfo(NSCompHtml* pCompHtml)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSCompHtmlData();
	//
	// Copie les valeurs du NSCompHtml
	//
	*pDonnees = *(pCompHtml->pDonnees);
}

//---------------------------------------------------------------------------
//  Fonction:		NSCompHtmlInfo::~NSCompHtmlInfo()
//  Description:	Destructeur
//  Retour:			Rien
//---------------------------------------------------------------------------
NSCompHtmlInfo::~NSCompHtmlInfo()
{
	delete pDonnees;
}

//---------------------------------------------------------------------------
//  Fonction:		NSCompHtmlInfo::NSCompHtmlInfo(const NSCompHtmlInfo& rv)
//  Description:	Constructeur copie
//  Retour:			Rien
//---------------------------------------------------------------------------
NSCompHtmlInfo::NSCompHtmlInfo(const NSCompHtmlInfo& rv)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSCompHtmlData();
	//
	// Copie les valeurs du NSTempubInfo d'origine
	//
	*pDonnees = *(rv.pDonnees);
}

//---------------------------------------------------------------------------
//  Fonction:		NSCompHtmlInfo::operator=(NSCompHtmlInfo src)
//  Description:	Opérateur d'affectation
//  Retour:			Référence de l'objet cible
//---------------------------------------------------------------------------
NSCompHtmlInfo&
NSCompHtmlInfo::operator=(NSCompHtmlInfo src)
{
	*pDonnees = *(src.pDonnees);
	return *this;
}

//---------------------------------------------------------------------------
//  Fonction:		NSCompHtmlInfo::operator==(NSCompHtmlInfo src)
//  Description:	Opérateur de comparaison
//  Retour:			Référence de l'objet cible
//---------------------------------------------------------------------------
int NSCompHtmlInfo::operator == ( const NSCompHtmlInfo& o )
{
	 return (*pDonnees == *(o.pDonnees));
}

//-------------------------------------------------------------------------
//----------------------- METHODES DE NSCompHtmlArray ---------------------
//-------------------------------------------------------------------------

// constructeur copie

NSCompHtmlArray::NSCompHtmlArray(NSCompHtmlArray& rv)
					  : NSCompHtmlVector()
{
	if (!(rv.empty()))
	   for (NSCompHtmlIter i = rv.begin(); i != rv.end(); i++)
			 	push_back(new NSCompHtmlInfo(*(*i)));
}

// fonction vider()
void
NSCompHtmlArray::vider()
{
	for (NSCompHtmlIter i = begin(); i != end(); )
   {
   	delete *i;
      erase(i);
   }
}

// destructeur
NSCompHtmlArray::~NSCompHtmlArray()
{
	vider();
}

//---------------------------------------------------------------------------
//  Fonction	 : NSCompHtmlArray::operator=(NSCompHtmlArray src)
//
//  Description : Opérateur d'affectation
//---------------------------------------------------------------------------
NSCompHtmlArray&
NSCompHtmlArray::operator=(NSCompHtmlArray src)
{
	vider();
   for (NSCompHtmlIter i = src.begin(); i != src.end(); i++)
			 	push_back(new NSCompHtmlInfo(*(*i)));
	return *this;
}

*/

/*
//===========================================================================// CLASSE NSCourrier
//---------------------------------------------------------------------------
//  Classe NSCourrierData
//---------------------------------------------------------------------------

NSCourrierData::NSCourrierData()
{
	// met les champs de donnees a zero

   memset(code_doc,		0,	COUR_CODE_DOC_LEN + 1);
   memset(code_corresp, 0,	COUR_CODE_CORRESP_LEN + 1);
   memset(code_lettre,	0,	COUR_CODE_LETTRE_LEN + 1);
}

//---------------------------------------------------------------------------
//  Fonction:		NSCourrierData::NSCourrierData(const NSCourrierData& rv)
//  Description:	Constructeur copie
//  Retour:			Rien
//---------------------------------------------------------------------------
NSCourrierData::NSCourrierData(const NSCourrierData& rv)
{
   	strcpy(code_doc,   		rv.code_doc);
    strcpy(code_corresp,    rv.code_corresp);
    strcpy(code_lettre,		rv.code_lettre);
}

//---------------------------------------------------------------------------
//  Fonction:		NSCourrierData::operator=(NSCourrierData src)
//  Description:	Opérateur =
//  Retour:			Référence sur l'objet cible
//---------------------------------------------------------------------------
NSCourrierData&
NSCourrierData::operator=(NSCourrierData src)
{
   	strcpy(code_doc,   		src.code_doc);
    strcpy(code_corresp,	src.code_corresp);
    strcpy(code_lettre,		src.code_lettre);

	return *this;
}

//---------------------------------------------------------------------------
//  Fonction:		NSCourrierData::operator==(const NSCourrierData& o)
//  Description:	Opérateur de comparaison
//  Retour:			0 ou 1
//---------------------------------------------------------------------------
int
NSCourrierData::operator == ( const NSCourrierData& o )
{
	if ((strcmp(code_doc, 		o.code_doc) 	== 0) &&
    	(strcmp(code_corresp,	o.code_corresp)	== 0) &&
       	(strcmp(code_lettre,	o.code_lettre)	== 0))
		return 1;
	else
		return 0;
}

//---------------------------------------------------------------------------
//  Classe NSCourrier
//---------------------------------------------------------------------------

NSCourrier::NSCourrier(NSContexte* pCtx) : NSFiche(pCtx)
{
	// cree un objet de donnees
    pDonnees = new NSCourrierData();
}

NSCourrier::~NSCourrier()
{
	delete pDonnees;
   pDonnees = 0;
}

// Ouverture et fermeture de la base des templates
//////////////////////////////////////////////////

DBIResult
NSCourrier::open()
{
#ifndef _INCLUS
  char tableName[] = "COURRIER.DB";
#else
  char tableName[] = "COURRIERINCLUS.DB";
#endif
  //
  // Appelle la fonction open() de la classe de base pour ouvrir
  // l'index primaire
  //
  lastError = NSFiche::open(tableName, NSF_PARTAGE_GLOBAL);
  return(lastError);
}

// alimenteFiche
/////////////////////////////////////////////////////////////////////////

void
NSCourrier::alimenteFiche()
{
	alimenteChamp(pDonnees->code_doc,		COUR_CODE_DOC_FIELD,		COUR_CODE_DOC_LEN);
    alimenteChamp(pDonnees->code_corresp,	COUR_CODE_CORRESP_FIELD,	COUR_CODE_CORRESP_LEN);
    alimenteChamp(pDonnees->code_lettre,	COUR_CODE_LETTRE_FIELD,		COUR_CODE_LETTRE_LEN);
}

// videFiche
/////////////////////////////////////////////////////////////////////////

void
NSCourrier::videFiche()
{
  	videChamp(pDonnees->code_doc,		COUR_CODE_DOC_FIELD,		COUR_CODE_DOC_LEN);
    videChamp(pDonnees->code_corresp,	COUR_CODE_CORRESP_FIELD,	COUR_CODE_CORRESP_LEN);
    videChamp(pDonnees->code_lettre,	COUR_CODE_LETTRE_FIELD,		COUR_CODE_LETTRE_LEN);
}

//---------------------------------------------------------------------------
//  Classe NSCourrierInfo
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  Fonction:		NSCourrierInfo::NSCourrierInfo()
//  Description:	Constructeur par défaut
//  Retour:			Rien
//---------------------------------------------------------------------------
NSCourrierInfo::NSCourrierInfo()
{
	// Crée l'objet de données
	pDonnees = new NSCourrierData();
}

//---------------------------------------------------------------------------
//  Fonction:		NSCourrierInfo::NSCourrierInfo(NSCourrier*)
//  Description:	Constructeur à partir d'un NSCourrier
//  Retour:			Rien
//---------------------------------------------------------------------------
NSCourrierInfo::NSCourrierInfo(NSCourrier* pCourrier)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSCourrierData();
	//
	// Copie les valeurs du NSCourrier
	//
	*pDonnees = *(pCourrier->pDonnees);
}

//---------------------------------------------------------------------------
//  Fonction:		NSCourrierInfo::~NSCourrierInfo()
//  Description:	Destructeur
//  Retour:			Rien
//---------------------------------------------------------------------------
NSCourrierInfo::~NSCourrierInfo()
{
	delete pDonnees;
}

//---------------------------------------------------------------------------
//  Fonction:		NSCourrierInfo::NSCourrierInfo(const NSCourrierInfo& rv)
//  Description:	Constructeur copie
//  Retour:			Rien
//---------------------------------------------------------------------------
NSCourrierInfo::NSCourrierInfo(const NSCourrierInfo& rv)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSCourrierData();
	//
	// Copie les valeurs du NSTempubInfo d'origine
	//
	*pDonnees = *(rv.pDonnees);
}

//---------------------------------------------------------------------------
//  Fonction:		NSCourrierInfo::operator=(NSCourrierInfo src)
//  Description:	Opérateur d'affectation
//  Retour:			Référence de l'objet cible
//---------------------------------------------------------------------------
NSCourrierInfo&
NSCourrierInfo::operator=(NSCourrierInfo src)
{
	*pDonnees = *(src.pDonnees);
	return *this;
}

//---------------------------------------------------------------------------
//  Fonction:		NSCourrierInfo::operator==(NSCourrierInfo src)
//  Description:	Opérateur de comparaison
//  Retour:			Référence de l'objet cible
//---------------------------------------------------------------------------
int NSCourrierInfo::operator == ( const NSCourrierInfo& o )
{
	 return (*pDonnees == *(o.pDonnees));
}

//-------------------------------------------------------------------------
//----------------------- METHODES DE NSCourrierArray ---------------------
//-------------------------------------------------------------------------

// constructeur copie

NSCourrierArray::NSCourrierArray(NSCourrierArray& rv)
					  : NSCourrierVector()
{
	if (!(rv.empty()))
	   for (NSCourrierIter i = rv.begin(); i != rv.end(); i++)
			 	push_back(new NSCourrierInfo(*(*i)));
}

// fonction vider()
void
NSCourrierArray::vider()
{
	for (NSCourrierIter i = begin(); i != end(); )
   {
   	delete *i;
      erase(i);
   }
}

// destructeur
NSCourrierArray::~NSCourrierArray()
{
	vider();
}

//---------------------------------------------------------------------------
//  Fonction	 : NSCourrierArray::operator=(NSCourrierArray src)
//
//  Description : Opérateur d'affectation
//---------------------------------------------------------------------------
NSCourrierArray&
NSCourrierArray::operator=(NSCourrierArray src)
{
	vider();
   for (NSCourrierIter i = src.begin(); i != src.end(); i++)
			 	push_back(new NSCourrierInfo(*(*i)));
	return *this;
}

*/
//===========================================================================// CLASSE NSFormule
//---------------------------------------------------------------------------
//  Classe NSFormuleData
//---------------------------------------------------------------------------

NSFormuleData::NSFormuleData()
{
	// met les champs de donnees a zero
	metAZero() ;
}

void
NSFormuleData::metAZero()
{
	memset(code,   0, FORM_CODE_LEN + 1) ;
  memset(type,   0, FORM_TYPE_LEN + 1) ;
  memset(texte1, 0, FORM_TEXTE1_LEN + 1) ;
  memset(texte2, 0, FORM_TEXTE2_LEN + 1) ;
  memset(texte3, 0, FORM_TEXTE3_LEN + 1) ;
  memset(texte4, 0, FORM_TEXTE4_LEN + 1) ;
}

//---------------------------------------------------------------------------//  Fonction:		NSFormuleData::NSFormuleData(const NSFormuleData& rv)
//  Description:	Constructeur copie
//  Retour:			Rien
//---------------------------------------------------------------------------
NSFormuleData::NSFormuleData(const NSFormuleData& rv)
{
	metAZero() ;

	strcpy(code,   rv.code) ;
	strcpy(type,   rv.type) ;
	strcpy(texte1, rv.texte1) ;
	strcpy(texte2, rv.texte2) ;
	strcpy(texte3, rv.texte3) ;
	strcpy(texte4, rv.texte4) ;
}

//---------------------------------------------------------------------------//  Fonction:		NSFormuleData::operator=(NSFormuleData src)
//  Description:	Opérateur =
//  Retour:			Référence sur l'objet cible
//---------------------------------------------------------------------------
NSFormuleData&
NSFormuleData::operator=(const NSFormuleData& src)
{
	if (this == &src)
		return *this ;

	metAZero() ;

	strcpy(code,   src.code) ;
	strcpy(type,   src.type) ;
	strcpy(texte1, src.texte1) ;
	strcpy(texte2, src.texte2) ;
	strcpy(texte3, src.texte3) ;
	strcpy(texte4, src.texte4) ;

	return *this ;
}

//---------------------------------------------------------------------------//  Fonction:		NSFormuleData::operator==(const NSFormuleData& o)
//  Description:	Opérateur de comparaison
//  Retour:			0 ou 1
//---------------------------------------------------------------------------
int
NSFormuleData::operator==(const NSFormuleData& o)
{
	if ((strcmp(code,   o.code)   == 0) &&
    	(strcmp(type,   o.type)   == 0) &&
      (strcmp(texte1, o.texte1) == 0) &&
      (strcmp(texte2, o.texte2) == 0) &&
      (strcmp(texte3, o.texte3) == 0) &&
      (strcmp(texte4, o.texte4) == 0))
		return 1 ;
	else
		return 0 ;
}

//---------------------------------------------------------------------------//  Classe NSFormule
//---------------------------------------------------------------------------

NSFormule::NSFormule(NSSuper* pSuper)
#ifndef _ENTERPRISE_DLL
          :NSFiche(pSuper)
#else
          :NSSuperRoot(pSuper)
#endif
{
	// cree un objet de donnees
	pDonnees = new NSFormuleData() ;
}

NSFormule::~NSFormule(){
	delete pDonnees ;
	pDonnees = 0 ;
}

// Ouverture et fermeture de la base des templates//////////////////////////////////////////////////
#ifndef _ENTERPRISE_DLL
DBIResult
NSFormule::open()
{
  char tableName[] = "FORMULE.DB" ;
  //
  // Appelle la fonction open() de la classe de base pour ouvrir
  // l'index primaire
  //
  lastError = NSFiche::open(tableName, NSF_PARTAGE_GLOBAL) ;

  return(lastError) ;
}
#endif
// alimenteFiche/////////////////////////////////////////////////////////////////////////
void
NSFormule::alimenteFiche()
{
#ifndef _ENTERPRISE_DLL
	alimenteChamp(pDonnees->code,   FORM_CODE_FIELD,   FORM_CODE_LEN) ;
	alimenteChamp(pDonnees->type,   FORM_TYPE_FIELD,   FORM_TYPE_LEN) ;
	alimenteChamp(pDonnees->texte1, FORM_TEXTE1_FIELD, FORM_TEXTE1_LEN) ;
	alimenteChamp(pDonnees->texte2, FORM_TEXTE2_FIELD, FORM_TEXTE2_LEN) ;
	alimenteChamp(pDonnees->texte3, FORM_TEXTE3_FIELD, FORM_TEXTE3_LEN) ;
	alimenteChamp(pDonnees->texte4, FORM_TEXTE4_FIELD, FORM_TEXTE4_LEN) ;
#endif
}

// videFiche/////////////////////////////////////////////////////////////////////////
void
NSFormule::videFiche()
{
#ifndef _ENTERPRISE_DLL
	videChamp(pDonnees->code,   FORM_CODE_FIELD,   FORM_CODE_LEN) ;
	videChamp(pDonnees->type,   FORM_TYPE_FIELD,   FORM_TYPE_LEN) ;
	videChamp(pDonnees->texte1, FORM_TEXTE1_FIELD, FORM_TEXTE1_LEN) ;
	videChamp(pDonnees->texte2, FORM_TEXTE2_FIELD, FORM_TEXTE2_LEN) ;
	videChamp(pDonnees->texte3, FORM_TEXTE3_FIELD, FORM_TEXTE3_LEN) ;
	videChamp(pDonnees->texte4, FORM_TEXTE4_FIELD, FORM_TEXTE4_LEN) ;
#endif
}

//---------------------------------------------------------------------------//  Classe NSFormuleInfo
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  Fonction:		NSFormuleInfo::NSFormuleInfo()
//  Description:	Constructeur par défaut
//  Retour:			Rien
//---------------------------------------------------------------------------
NSFormuleInfo::NSFormuleInfo()
{
	// Crée l'objet de données
	pDonnees = new NSFormuleData() ;
}

//---------------------------------------------------------------------------//  Fonction:		NSFormuleInfo::NSFormuleInfo(NSFormule*)
//  Description:	Constructeur à partir d'un NSFormule
//  Retour:			Rien
//---------------------------------------------------------------------------
NSFormuleInfo::NSFormuleInfo(const NSFormule* pFormule)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSFormuleData() ;
	//
	// Copie les valeurs du NSFormule
	//
	*pDonnees = *(pFormule->pDonnees) ;
}

//---------------------------------------------------------------------------//  Fonction:		NSFormuleInfo::~NSFormuleInfo()
//  Description:	Destructeur
//  Retour:			Rien
//---------------------------------------------------------------------------
NSFormuleInfo::~NSFormuleInfo()
{
	delete pDonnees ;
}

//---------------------------------------------------------------------------//  Fonction:		NSFormuleInfo::NSFormuleInfo(const NSFormuleInfo& rv)
//  Description:	Constructeur copie
//  Retour:			Rien
//---------------------------------------------------------------------------
NSFormuleInfo::NSFormuleInfo(const NSFormuleInfo& rv)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSFormuleData() ;
	//
	// Copie les valeurs du NSTempubInfo d'origine
	//
	*pDonnees = *(rv.pDonnees) ;
}

//---------------------------------------------------------------------------//  Fonction:		NSFormuleInfo::operator=(NSFormuleInfo src)
//  Description:	Opérateur d'affectation
//  Retour:			Référence de l'objet cible
//---------------------------------------------------------------------------
NSFormuleInfo&
NSFormuleInfo::operator=(const NSFormuleInfo& src)
{
	if (this == &src)
		return *this ;

	*pDonnees = *(src.pDonnees) ;

	return *this ;
}

//---------------------------------------------------------------------------//  Fonction:		NSFormuleInfo::operator==(NSFormuleInfo src)
//  Description:	Opérateur de comparaison
//  Retour:			Référence de l'objet cible
//---------------------------------------------------------------------------
int NSFormuleInfo::operator==(const NSFormuleInfo& o)
{
	return (*pDonnees == *(o.pDonnees)) ;
}

//-------------------------------------------------------------------------//----------------------- METHODES DE NSFormuleArray ---------------------
//-------------------------------------------------------------------------

// constructeur copie
NSFormuleArray::NSFormuleArray(const NSFormuleArray& rv)
               :NSFormuleVector()
{
	if (false == rv.empty())
		for (NSFormuleConstIter it = rv.begin() ; rv.end() != it ; it++)
			push_back(new NSFormuleInfo(*(*it))) ;
}

// fonction vider()void
NSFormuleArray::vider()
{
	if (false == empty())
		for (NSFormuleIter it = begin() ; end() != it ; )
		{
			delete *it ;
			erase(it) ;
		}
}

// destructeurNSFormuleArray::~NSFormuleArray()
{
	vider() ;
}

//---------------------------------------------------------------------------//  Fonction	 : NSFormuleArray::operator=(NSFormuleArray src)
//
//  Description : Opérateur d'affectation
//---------------------------------------------------------------------------
NSFormuleArray&
NSFormuleArray::operator=(const NSFormuleArray& src)
{
	if (this == &src)
		return *this ;

	vider() ;

  if (false == src.empty())
	  for (NSFormuleConstIter it = src.begin() ; src.end() != it ; it++)
		  push_back(new NSFormuleInfo(*(*it))) ;

	return *this ;
}

/////////////////////////// fin de nsintradb.cpp ///////////////////////////////
