// nsintradb.h//////////////////////////////////////////////////////////////////////////
// Header des bases intranet - Rémi SPAAK Aout 1997
//////////////////////////////////////////////////////////////////////////

#ifndef __NSVINTRAD_H
#define __NSVINTRAD_H

#if defined(_DANSNSDNDLL)
	#define _CLASSENSDN __export
#else
	#define _CLASSENSDN __import
#endif

// #include <owl\owlpch.h>
#include <owl\dc.h>
#include <owl\inputdia.h>
#include <owl\chooseco.h>
#include <owl\gdiobjec.h>
#include <owl\docmanag.h>
#include <owl\filedoc.h>
#include <owl\listbox.h>
#include "partage\nsperson.h"   // pour le code patient
#include "nssavoir\nspatbas.h"  // pour BASE_LEXI_LEN#include "nautilus\nsannexe.h"#include "nautilus\nstxstyl.h"#include "nsbb\ns_objs.h"	// pour OBJECT_ID_LEN
// tailles et indices des champs de TEMPLATE.DB///////////////////////////////////////////////

#define TMPL_CODE_LEN			OBJECT_ID_LEN
#define TMPL_TYPE_LEN		5
#define TMPL_UTIL_LEN		PAT_NSS_LEN
#define TMPL_ROOT_LEN       BASE_LEXIQUE_LEN
#define TMPL_COMPO_LEN		1#define TMPL_DEFAUT_LEN		1
#define TMPL_EN_TETE_LEN	255
#define TMPL_FICHIER_LEN	255
#define TMPL_LIBELLE_LEN	255

#define TMPL_CODE_FIELD 	1#define TMPL_TYPE_FIELD		2
#define TMPL_UTIL_FIELD     3
#define TMPL_ROOT_FIELD     4
#define TMPL_COMPO_FIELD	5#define TMPL_DEFAUT_FIELD	6
#define TMPL_EN_TETE_FIELD  7
#define TMPL_FICHIER_FIELD	8
#define TMPL_LIBELLE_FIELD	9

// tailles et indices des champs de TEMPUB.DB
///////////////////////////////////////////////

#define TMPUB_CODE_COMPO_LEN		3
#define TMPUB_TYPE_ACTION_LEN 		1
#define TMPUB_DEFAUT_LEN		1
#define TMPUB_CODE_TMPL_LEN		3

#define TMPUB_CODE_COMPO_FIELD		1
#define TMPUB_TYPE_ACTION_FIELD 	2
#define TMPUB_DEFAUT_FIELD		3
#define TMPUB_CODE_TMPL_FIELD		4

// tailles et indices des champs de WORKFLOW.DB
///////////////////////////////////////////////

#define WF_CODE_DOCU_LEN		11
#define WF_CODE_DEST_LEN 		3
#define WF_TYPE_PUBLI_LEN 		1
#define WF_DATE_PUBLI_LEN		8
#define WF_HEURE_PUBLI_LEN		6
#define WF_NB_PUBLI_LEN			2
#define WF_ENTET_PUBLI_LEN		35
#define WF_TMPL_PUBLI_LEN		35
#define WF_AR_PUBLI_LEN			1
#define WF_VALIDE_LEN			1

#define WF_CODE_DOCU_FIELD		1
#define WF_CODE_DEST_FIELD 		2
#define WF_TYPE_PUBLI_FIELD   	3
#define WF_DATE_PUBLI_FIELD		4
#define WF_HEURE_PUBLI_FIELD   	5
#define WF_NB_PUBLI_FIELD		6
#define WF_ENTET_PUBLI_FIELD  	7
#define WF_TMPL_PUBLI_FIELD		8
#define WF_AR_PUBLI_FIELD		9
#define WF_VALIDE_FIELD			10

// tailles et indices des champs de COMPHTML.DB
///////////////////////////////////////////////

#define CHTML_CODE_HTML_LEN 	11
#define CHTML_TYPE_COMP_LEN	3
#define CHTML_INDEX_COMP_LEN	3
#define CHTML_CODE_COMP_LEN   	11
#define CHTML_PRINCIPAL_LEN	1

#define CHTML_CODE_HTML_FIELD 	1
#define CHTML_TYPE_COMP_FIELD	2
#define CHTML_INDEX_COMP_FIELD	3
#define CHTML_CODE_COMP_FIELD   4
#define CHTML_PRINCIPAL_FIELD	5

// tailles et indices des champs de COURRIER.DB
///////////////////////////////////////////////

#ifndef _INCLUS
#define COUR_CODE_DOC_LEN	11
#define COUR_CODE_CORRESP_LEN	3
#define COUR_CODE_LETTRE_LEN	11
#else
#define COUR_CODE_DOC_LEN       (2 * ID_INCLUS_LEN) + 1
#define COUR_CODE_CORRESP_LEN   ID_INCLUS_LEN
#define COUR_CODE_LETTRE_LEN    ID_INCLUS_LEN + 5
#endif

#define COUR_CODE_DOC_FIELD	1
#define COUR_CODE_CORRESP_FIELD 2
#define COUR_CODE_LETTRE_FIELD  3

// tailles et indices des champs de FORMULE.DB
///////////////////////////////////////////////

#define FORM_CODE_LEN		3
#define FORM_TYPE_LEN           1
#define FORM_TEXTE1_LEN		255
#define FORM_TEXTE2_LEN		255
#define FORM_TEXTE3_LEN		255
#define FORM_TEXTE4_LEN		255

#define FORM_CODE_FIELD		1
#define FORM_TYPE_FIELD		2
#define FORM_TEXTE1_FIELD	3
#define FORM_TEXTE2_FIELD	4
#define FORM_TEXTE3_FIELD	5
#define FORM_TEXTE4_FIELD	6

////////////////////////////////////////////////////////////////////////////////
// DEFINITION DES CLASSES D'ACCES AUX BASES INTRANET
////////////////////////////////////////////////////////////////////////////////

// Base TEMPLATE.DB
///////////////////

#define NSV_TMPL_CODE_LEN 		 3
#define NSV_TMPL_TYPE_LEN		   5
#define NSV_TMPL_UTIL_LEN		   3
#define NSV_TMPL_ROOT_LEN      5#define NSV_TMPL_COMPO_LEN		 1#define NSV_TMPL_DEFAUT_LEN		 1
#define NSV_TMPL_EN_TETE_LEN	35
#define NSV_TMPL_FICHIER_LEN	35
#define NSV_TMPL_LIBELLE_LEN	35

#define NSV_TMPL_CODE_FIELD 	  1#define NSV_TMPL_TYPE_FIELD		  2
#define NSV_TMPL_UTIL_FIELD     3
#define NSV_TMPL_ROOT_FIELD     4
#define NSV_TMPL_COMPO_FIELD	  5#define NSV_TMPL_DEFAUT_FIELD	  6
#define NSV_TMPL_EN_TETE_FIELD  7
#define NSV_TMPL_FICHIER_FIELD	8
#define NSV_TMPL_LIBELLE_FIELD	9

// Base TEMPLATE.DB
///////////////////

class NSVTemplateData
{
	public :

		//
    // Variables de stockage des valeurs
    //
		char code[NSV_TMPL_CODE_LEN + 1] ;
		char type[NSV_TMPL_TYPE_LEN + 1] ;
		char util[NSV_TMPL_UTIL_LEN + 1] ;
		char root[NSV_TMPL_ROOT_LEN + 1] ;
		char compo[NSV_TMPL_COMPO_LEN + 1] ;
		char defaut[NSV_TMPL_DEFAUT_LEN + 1] ;
		char en_tete[NSV_TMPL_EN_TETE_LEN + 1] ;
		char fichier[NSV_TMPL_FICHIER_LEN + 1] ;
		char libelle[NSV_TMPL_LIBELLE_LEN + 1] ;

		NSVTemplateData() ;
		NSVTemplateData(const NSVTemplateData& rv) ;

		NSVTemplateData&	operator=(NSVTemplateData src) ;
		int 		   	operator==(const NSVTemplateData& o) ;
};

//
// Objet dérivé de NSFiche utilisé pour les opérations de base de données//
class NSVTemplate : public NSFiche
{
	public :

		//
    // Variables de stockage des valeurs
    //
    NSVTemplateData* pDonnees ;

    NSVTemplate(NSSuper* pSuper) ;
    ~NSVTemplate() ;

    DBIResult open(string pathbase) ;
    void alimenteFiche() ;
    void videFiche() ;
};

//
// Objet contenant uniquement un pointeur sur les donnees//
class NSVTemplateInfo
{
	public :

		//
    // Objet qui contient les données
    //
		NSVTemplateData* pDonnees ;
    string			     sNumCons ;

    NSVTemplateInfo() ;
		NSVTemplateInfo(NSVTemplate*) ;
		NSVTemplateInfo(const NSVTemplateInfo& rv) ;
    ~NSVTemplateInfo() ;

    NSVTemplateInfo& operator=(NSVTemplateInfo src) ;
		int operator == ( const NSVTemplateInfo& o ) ;
};

//
// Définition de NSTemplateArray (Array de NSTemplate)
//
typedef vector<NSVTemplateInfo*> NSVTemplateVector;
typedef NSVTemplateVector::iterator NSVTemplateIter;

class _CLASSENSDN NSVTemplateArray : public NSVTemplateVector
{
	public :

		// Constructeurs
		NSVTemplateArray() : NSVTemplateVector() {}
		NSVTemplateArray(NSVTemplateArray& rv);
		// Destructeur
		virtual ~NSVTemplateArray();

   		NSVTemplateArray& operator=(NSVTemplateArray src);
   		void vider();
};


//////////////////////////////////////////////////////////////////////////////
// Base TEMPUB.DB
//////////////////////////////////////////////////////////////////////////////

class _CLASSENSDN NSTempubData
{
  public :
	  //
	  // Variables de stockage des valeurs
	  //
     char code_compo[TMPUB_CODE_COMPO_LEN + 1];
     char type_action[TMPUB_TYPE_ACTION_LEN + 1];
     char defaut[TMPUB_DEFAUT_LEN + 1];
     char code_tmpl[TMPUB_CODE_TMPL_LEN + 1];

	  NSTempubData();
     NSTempubData(const NSTempubData& rv);

	  NSTempubData& operator=(NSTempubData src);
	  int 			  operator==(const NSTempubData& o);
};

//
// Objet dérivé de NSFiche utilisé pour les opérations de base de données
//
class _CLASSENSDN NSTempub : public NSFiche
{
  public :

	  //
	  // Variables de stockage des valeurs
	  //
	  NSTempubData* pDonnees;

	  NSTempub(NSSuper* pSuper);
	  ~NSTempub();

	  DBIResult open();
	  void alimenteFiche();
	  void videFiche();
};

//
// Objet contenant uniquement un pointeur sur les donnees
//
class _CLASSENSDN NSTempubInfo
{
	public :
	  	//
	  	// Objet qui contient les données
	  	//
	  	NSTempubData* pDonnees;

     	NSTempubInfo();
		NSTempubInfo(NSTempub*);
		NSTempubInfo(const NSTempubInfo& rv);
      ~NSTempubInfo();

      NSTempubInfo& operator=(NSTempubInfo src);
		int operator == ( const NSTempubInfo& o );
};

//
// Définition de NSTempubArray (Array de NSTempub)
//
typedef vector<NSTempubInfo*> NSTempubVector;
typedef NSTempubVector::iterator NSTempubIter;

class _CLASSENSDN NSTempubArray : public NSTempubVector
{
public :
	// Constructeurs
	NSTempubArray() : NSTempubVector() {}
	NSTempubArray(NSTempubArray& rv);
	// Destructeur
	virtual ~NSTempubArray();

   NSTempubArray& operator=(NSTempubArray src);
   void vider();
};

//////////////////////////////////////////////////////////////////////////////
// Base WORKFLOW.DB
//////////////////////////////////////////////////////////////////////////////

class _CLASSENSDN NSWorkflowData
{
  public :
	  //
	  // Variables de stockage des valeurs
	  //
     char code_docu[WF_CODE_DOCU_LEN + 1];
     char code_dest[WF_CODE_DEST_LEN + 1];
     char type_publi[WF_TYPE_PUBLI_LEN + 1];
     char date_publi[WF_DATE_PUBLI_LEN + 1];
     char heure_publi[WF_HEURE_PUBLI_LEN + 1];
     char nb_publi[WF_NB_PUBLI_LEN + 1];
     char entet_publi[WF_ENTET_PUBLI_LEN + 1];
     char tmpl_publi[WF_TMPL_PUBLI_LEN + 1];
     char ar_publi[WF_AR_PUBLI_LEN + 1];
     char valide[WF_VALIDE_LEN + 1];

	  NSWorkflowData();
     NSWorkflowData(const NSWorkflowData& rv);

	  NSWorkflowData& operator=(NSWorkflowData src);
	  int 			  operator==(const NSWorkflowData& o);
};

//
// Objet dérivé de NSFiche utilisé pour les opérations de base de données
//
class _CLASSENSDN NSWorkflow : public NSFiche
{
  public :

	  //
	  // Variables de stockage des valeurs
	  //
	  NSWorkflowData* pDonnees;

	  NSWorkflow(NSSuper* pSuper);
	  ~NSWorkflow();

	  DBIResult open();
	  void alimenteFiche();
	  void videFiche();
};

//
// Objet contenant uniquement un pointeur sur les donnees
//
class _CLASSENSDN NSWorkflowInfo
{
	public :
	  	//
	  	// Objet qui contient les données
	  	//
	  	NSWorkflowData* pDonnees;

     	NSWorkflowInfo();
		NSWorkflowInfo(NSWorkflow*);
		NSWorkflowInfo(const NSWorkflowInfo& rv);
      ~NSWorkflowInfo();

      NSWorkflowInfo& operator=(NSWorkflowInfo src);
		int operator == ( const NSWorkflowInfo& o );
};

//
// Définition de NSWorkflowArray (Array de NSWorkflow)
//
typedef vector<NSWorkflowInfo*> NSWorkflowVector;
typedef NSWorkflowVector::iterator NSWorkflowIter;

class _CLASSENSDN NSWorkflowArray : public NSWorkflowVector
{
public :
	// Constructeurs
	NSWorkflowArray() : NSWorkflowVector() {}
	NSWorkflowArray(NSWorkflowArray& rv);
	// Destructeur
	virtual ~NSWorkflowArray();

   NSWorkflowArray& operator=(NSWorkflowArray src);
   void vider();
};
//////////////////////////////////////////////////////////////////////////////// Base COMPHTML.DB
//////////////////////////////////////////////////////////////////////////////

class _CLASSENSDN NSVCompHtmlData{
  public :

  	//
	  // Variables de stockage des valeurs
	  //
    char code_html[CHTML_CODE_HTML_LEN + 1] ;
    char type_comp[CHTML_TYPE_COMP_LEN + 1] ;
    char index_comp[CHTML_INDEX_COMP_LEN + 1] ;
    char code_comp[CHTML_CODE_COMP_LEN + 1] ;
    char principal[CHTML_PRINCIPAL_LEN + 1] ;

	  NSVCompHtmlData() ;
    NSVCompHtmlData(const NSVCompHtmlData& rv) ;

	  NSVCompHtmlData& operator=(NSVCompHtmlData src) ;
	  int              operator==(const NSVCompHtmlData& o) ;
};

class NSVCompHtml : public NSFiche
{
	public :

	  //	  // Variables de stockage des valeurs
	  //

	  NSVCompHtmlData* pDonnees ;
	  NSVCompHtml(NSSuper* pSuper) ;	  ~NSVCompHtml() ;

	  DBIResult open(string pathbase) ;
	  void alimenteFiche() ;
	  void videFiche() ;};


//// Objet contenant uniquement un pointeur sur les donnees
//
class _CLASSENSDN NSVCompHtmlInfo
{
	public :

  	//
    // Objet qui contient les données
    //
    NSVCompHtmlData* pDonnees ;

    NSVCompHtmlInfo() ;
		NSVCompHtmlInfo(NSVCompHtml*) ;
		NSVCompHtmlInfo(const NSVCompHtmlInfo& rv) ;
    ~NSVCompHtmlInfo() ;

    NSVCompHtmlInfo& operator=(NSVCompHtmlInfo src) ;
		int operator == ( const NSVCompHtmlInfo& o ) ;
};

//// Définition de NSCompHtmlArray (Array de NSCompHtml)
//
typedef vector<NSVCompHtmlInfo*> NSVCompHtmlVector;
typedef NSVCompHtmlVector::iterator NSVCompHtmlIter;

class _CLASSENSDN NSVCompHtmlArray : public NSVCompHtmlVector{

	public :
	// Constructeurs	NSVCompHtmlArray() : NSVCompHtmlVector() {}
	NSVCompHtmlArray(NSVCompHtmlArray& rv);
	// Destructeur
	virtual ~NSVCompHtmlArray();

	NSVCompHtmlArray& operator=(NSVCompHtmlArray src);
	void vider();
};


//////////////////////////////////////////////////////////////////////////////
// Base COURRIER.DB
//////////////////////////////////////////////////////////////////////////////

class _CLASSENSDN NSCourrierData
{
  public :
	  //
	  // Variables de stockage des valeurs
	  //
     char code_doc[COUR_CODE_DOC_LEN + 1];
  	 char code_corresp[COUR_CODE_CORRESP_LEN + 1];
     char code_lettre[COUR_CODE_LETTRE_LEN + 1];

	  NSCourrierData();
     NSCourrierData(const NSCourrierData& rv);

	  NSCourrierData& operator=(NSCourrierData src);
	  int 			  operator==(const NSCourrierData& o);
};

//
// Objet dérivé de NSFiche utilisé pour les opérations de base de données
//
class _CLASSENSDN NSCourrier : public NSFiche
{
  public :

	  //
	  // Variables de stockage des valeurs
	  //
	  NSCourrierData* pDonnees;

	  NSCourrier(NSSuper* pSuper);
	  ~NSCourrier();

	  DBIResult open();
	  void alimenteFiche();
	  void videFiche();
};

//
// Objet contenant uniquement un pointeur sur les donnees
//
class _CLASSENSDN NSCourrierInfo
{
	public :
	  	//
	  	// Objet qui contient les données
	  	//
	  	NSCourrierData* pDonnees;

     	NSCourrierInfo();
		NSCourrierInfo(NSCourrier*);
		NSCourrierInfo(const NSCourrierInfo& rv);
      ~NSCourrierInfo();

      NSCourrierInfo& operator=(NSCourrierInfo src);
		int operator == ( const NSCourrierInfo& o );
};

//
// Définition de NSCourrierArray (Array de NSCourrier)
//
typedef vector<NSCourrierInfo*> NSCourrierVector;
typedef NSCourrierVector::iterator NSCourrierIter;

class _CLASSENSDN NSCourrierArray : public NSCourrierVector
{
public :
	// Constructeurs
	NSCourrierArray() : NSCourrierVector() {}
	NSCourrierArray(NSCourrierArray& rv);
	// Destructeur
	virtual ~NSCourrierArray();

   NSCourrierArray& operator=(NSCourrierArray src);
   void vider();
};
//////////////////////////////////////////////////////////////////////////////// Base FORMULE.DB
//////////////////////////////////////////////////////////////////////////////

/*
class _CLASSENSDN NSFormuleData

{
  public :
	  //
	  // Variables de stockage des valeurs
	  //
     char code[FORM_CODE_LEN + 1];
	 char type[FORM_TYPE_LEN + 1];
     char texte1[FORM_TEXTE1_LEN + 1];
     char texte2[FORM_TEXTE2_LEN + 1];
     char texte3[FORM_TEXTE3_LEN + 1];
     char texte4[FORM_TEXTE4_LEN + 1];

	  NSFormuleData();
     NSFormuleData(const NSFormuleData& rv);

	  NSFormuleData& operator=(NSFormuleData src);
	  int 			  operator==(const NSFormuleData& o);
};

//
// Objet dérivé de NSFiche utilisé pour les opérations de base de données
//
class _CLASSENSDN NSFormule : public NSFiche
{
  public :

	  //
	  // Variables de stockage des valeurs
	  //
	  NSFormuleData* pDonnees;

	  NSFormule(NSContexte* pCtx);
	  ~NSFormule();

	  DBIResult open();
	  void alimenteFiche();
	  void videFiche();
};

//
// Objet contenant uniquement un pointeur sur les donnees
//
class _CLASSENSDN NSFormuleInfo
{
	public :
	  	//
	  	// Objet qui contient les données
	  	//
	  	NSFormuleData* pDonnees;

     	NSFormuleInfo();
		NSFormuleInfo(NSFormule*);
		NSFormuleInfo(const NSFormuleInfo& rv);
      ~NSFormuleInfo();

      NSFormuleInfo& operator=(NSFormuleInfo src);
		int operator == ( const NSFormuleInfo& o );
};

//
// Définition de NSFormuleArray (Array de NSFormule)
//
typedef vector<NSFormuleInfo*> NSFormuleVector;
typedef NSFormuleVector::iterator NSFormuleIter;

class _CLASSENSDN NSFormuleArray : public NSFormuleVector
{
public :
	// Constructeurs
	NSFormuleArray() : NSFormuleVector() {}
	NSFormuleArray(NSFormuleArray& rv);
	// Destructeur
	virtual ~NSFormuleArray();

   NSFormuleArray& operator=(NSFormuleArray src);
   void vider();
};
*/
#endif // __NSVINTRAD_H
///////////////////////////// fin de nsv_intradb.h ///////////////////////////////
