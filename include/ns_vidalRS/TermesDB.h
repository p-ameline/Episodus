// -----------------------------------------------------------------------------////
//    Définition des objets de base de donnée
// -----------------------------------------------------------------------------

#ifndef __TERMESDB_H__# define __TERMESDB_H__

# ifndef _OWLDLL
#  include <stl.h>
# else
#  include <vector.h>
# endif

# include "ns_sgbd\nsfiche.h"

# ifdef _EPISODUSDLL
#  define _EPISODUS __export
# else
#  define _EPISODUS __import
# endif

// Taille des champs du fichier TermeRefIndic
# define TERMESREF_LIBELLE_LEN            255
# define TERMESREF_TEXT_LEN               255

// Indice des champs du fichier TermeRefIndic
# define TERMESREF_ID_FIELD			        1
# define TERMESREF_LIBELLE_FIELD            2
# define TERMESREF_TEXT_FIELD               3

//*******************************************
// Taille des champs du fichier dbo_GroupIndic
//# define GROUPINDIC_ID_LEN			          4
//# define GROUPINDIC_DICOTYPE_LEN                4
# define GROUPINDIC_CODE_LEN               255

// Indice des champs du fichier dbo_GroupIndic
# define GROUPINDIC_ID_FIELD			   	    1
# define GROUPINDIC_DICOTYPE_FIELD              2
# define GROUPINDIC_CODE_FIELD               3

//******************************************************

// Taille des champs du fichier GroupIndic_TermeRefIndic.DB

# define INDICTERME_TEXT_LEN               255

// Indice des champs du fichier GroupIndic_TermeRefIndic.DB
# define INDICTERME_IDGROUP_ID_FIELD		   	 1
# define INDICTERME_TERMEREF_ID_FIELD      	     2
# define INDICTERME_LIEN_ID_FIELD                3
# define INDICTERME_TEXT_FIELD                   4

//**********************************************************
// Taille des champs du fichier SynonyTermeRef.DB
# define SYNONY_LIBELLE_LEN            255
# define SYNONY_TEXT_LEN               255

// Indice des champs du fichier SynonyTermeRef.DB
//# define SYNONY_ID_FIELD			        1
# define SYNONY_ID_TERMRF_FIELD             1
# define SYNONY_LIBELLE_FIELD               2
# define SYNONY_TEXT_FIELD                  3
//*************************************************************

// Taille des champs du fichier Lexique.db
# define TERMELEXIQUE_LEXIQUE_LEN            6
# define TERMELEXIQUE_CLASS_LEN              2
//# define TERMELEXIQUE_CODE_LEN               255

// Indice des champs du fichier Lexique
# define TERMELEXIQUE_LEXIQUE_FIELD			 1
# define TERMELEXIQUE_CLASS_FIELD            2
# define TERMELEXIQUE_CODE_FIELD             3

//********************************************************
// Indice des champs du fichier SynonyTermeRefIndic_Rel.DB
# define TERMSYN_ID_TERM_FIELD			 1
# define TERMSYN_ID_SYN_FIELD            2
//********************************************************

//******************************************************

// Taille des champs du fichier LienTermerefES.DB

# define LIENSTERMESREF_TEXT_LEN               255

// Indice des champs du fichier LienTermerefES.DB
# define LIENSTERMESREF_PERE_ID_FIELD		   	     1
# define LIENSTERMESREF_ENFANT_ID_FIELD      	     2
# define LIENSTERMESREF_LIEN_ID_FIELD                3
# define LIENSTERMESREF_TEXT_FIELD                   4

//**********************************************************




// *************************** Table TermeRefIndic.DB ***********************
// Termes de référence
// **************************************************************************

// --------------------------------------------------------------------------
//  Classe NSTermeRefIndicData
//
// Objet contenant les données
//---------------------------------------------------------------------------
class NSTermeRefIndicData
{
 public:
	NSTermeRefIndicData() ;
	NSTermeRefIndicData (NSTermeRefIndicData & rv) ;
	~NSTermeRefIndicData() ;

	NSTermeRefIndicData&	operator=(NSTermeRefIndicData src) ;
	int						operator==(const NSTermeRefIndicData& o) ;

	//void 						metABlanc() ;
	void 						metAZero() ;

	// variables
	long int					id ;
	char						libelle[TERMESREF_LIBELLE_LEN + 1] ;
	char						text[TERMESREF_TEXT_LEN + 1] ;
} ;


class NSTermeRefIndicAct ;

// -----------------------------------------------------------------------------
//  Classe NSFTermeRefIndicInfo  (destinée à être stockée dans une Array)
// -----------------------------------------------------------------------------
class NSTermeRefIndicInfo
{
 public:
	NSTermeRefIndicInfo() ;
	NSTermeRefIndicInfo(NSTermeRefIndicAct *) ;
	NSTermeRefIndicInfo(NSTermeRefIndicInfo& rv) ;
	~NSTermeRefIndicInfo() ;

	NSTermeRefIndicInfo& operator=(NSTermeRefIndicInfo src) ;
	int 		      	operator==(const NSTermeRefIndicInfo& o) ;


	// variables
	NSTermeRefIndicData	*pDonnees ;
} ;


// -----------------------------------------------------------------------------
//  Classe NSTermeRefIndicAct
//
// Objet dérivé de NSFiche servant aux accès de base de données
// -----------------------------------------------------------------------------
class  NSTermeRefIndicAct : public NSTermeRefIndicInfo, public NSFiche
{
 public:

	NSTermeRefIndicAct(NSContexte *pCtx) ;
	NSTermeRefIndicAct(NSTermeRefIndicAct& rv) ;
	~NSTermeRefIndicAct() ;

   //	void					metABlanc() { pDonnees->metABlanc(); }
	void					alimenteFiche() ;
	void					videFiche() ;
	DBIResult			open(bool bOpenShared = true) ;

	virtual bool	Create() ;
	virtual bool	Modify() ;

	NSTermeRefIndicAct&		operator=(NSTermeRefIndicAct src) ;
	int 	      	operator==(const NSTermeRefIndicAct& o) ;
} ;



// ***************************Table dbo_GroupIndic.DB ***********************
// Groupes d'indication
// **************************************************************************

//---------------------------------------------------------------------------
//  Classe NSGroupIndicData
//
// Objet contenant les données
//---------------------------------------------------------------------------
class NSGroupIndicData
{
 public:
	NSGroupIndicData() ;
	NSGroupIndicData(NSGroupIndicData& rv) ;
	~NSGroupIndicData() ;

	NSGroupIndicData&	operator=(NSGroupIndicData src) ;
	int							operator==(const NSGroupIndicData& o) ;

	//void 						metABlanc() ;
	void 						metAZero() ;

	// variables
	long int					id ;
	long int					iType ;
	char						libelle[GROUPINDIC_CODE_LEN  + 1] ;
} ;


class NSGroupIndicAct ;

// -----------------------------------------------------------------------------
//  Classe NSGroupIndicInfo  (destinée à être stockée dans une Array)
// -----------------------------------------------------------------------------
class  NSGroupIndicInfo
{
 public:
	NSGroupIndicInfo() ;
	NSGroupIndicInfo(NSGroupIndicAct *) ;
	NSGroupIndicInfo(NSGroupIndicInfo& rv) ;
	~NSGroupIndicInfo() ;

	NSGroupIndicInfo& operator=(NSGroupIndicInfo src) ;
	int 		      	operator==(const NSGroupIndicInfo& o) ;

	// variables
	NSGroupIndicData	*pDonnees ;
} ;


// -----------------------------------------------------------------------------
//  Classe NSGroupIndicAct
//
// Objet dérivé de NSFiche servant aux accès de base de données
// -----------------------------------------------------------------------------
class NSGroupIndicAct : public NSGroupIndicInfo, public NSFiche
{
 public:

	NSGroupIndicAct(NSContexte *pCtx) ;
	NSGroupIndicAct(NSGroupIndicAct& rv) ;
	~NSGroupIndicAct() ;

   //	void					metABlanc() { pDonnees->metABlanc(); }
	void					alimenteFiche() ;
	void					videFiche() ;
	DBIResult			open(bool bOpenShared = true) ;

	virtual bool	Create() ;
	virtual bool	Modify() ;

	NSGroupIndicAct&		operator=(NSGroupIndicAct src) ;
	int 	      	operator==(const NSGroupIndicAct& o) ;
} ;


//***************************GroupIndic_TermeRefIndic.DB ********************
// Jointure Groupe d'indication - Terme de référence (en mode vrac)
// Jointure Groupe d'indication - Terme de référence en mode sémantique
// Jointure Terme de référence - Termes de référence
// **************************************************************************

//-----------------------------------------------------------------------------
//  Classe NSIndicTermeData
//
// Objet contenant les données
//---------------------------------------------------------------------------
class _EPISODUS NSIndicTermeData
{
 public:
	NSIndicTermeData() ;
	NSIndicTermeData(NSIndicTermeData& rv) ;
	~NSIndicTermeData() ;

	NSIndicTermeData&	operator=(NSIndicTermeData src) ;
	int				   	operator==(const NSIndicTermeData& o) ;

	//void 						metABlanc() ;
	void 						metAZero() ;

	// variables
	long int		   iGroupIndic;
    long int           iTermeRefIndic;
    long int           iLien;
	char			   text[INDICTERME_TEXT_LEN] ;

} ;


class NSIndicTermeAct ;

// -----------------------------------------------------------------------------
//  Classe NSIndicTermeInfo  (destinée à être stockée dans une Array)
// -----------------------------------------------------------------------------
class NSIndicTermeInfo
{
 public:
	NSIndicTermeInfo() ;
	NSIndicTermeInfo(NSIndicTermeAct *) ;
	NSIndicTermeInfo(NSIndicTermeInfo& rv) ;
	~NSIndicTermeInfo() ;

	NSIndicTermeInfo& operator=(NSIndicTermeInfo src) ;
	int 		      	operator==(const NSIndicTermeInfo& o) ;

	// variables
	NSIndicTermeData	*pDonnees ;
} ;


// -----------------------------------------------------------------------------
//  Classe NSGroupIndicAct
//
// Objet dérivé de NSFiche servant aux accès de base de données
// -----------------------------------------------------------------------------
class _EPISODUS NSIndicTermeAct : public NSIndicTermeInfo, public NSFiche
{
    public:

        enum FILETYPE { GITR_vrac = 1, GITR_sem, TRTR_sem } ;
        FILETYPE iFileType ;

        string  sFileName ;

	    NSIndicTermeAct(NSContexte *pCtx, FILETYPE iType) ;
	    NSIndicTermeAct(NSIndicTermeAct& rv) ;
	    ~NSIndicTermeAct() ;

	    void			    alimenteFiche() ;	    void			    videFiche() ;
	    DBIResult			open(bool bOpenShared = true) ;

        bool                bRelationExist(long int	iPere, long int iFrere) ;

	    virtual bool	    Create() ;
	    virtual bool	    Modify() ;

	    NSIndicTermeAct&    operator=(NSIndicTermeAct src) ;
	    int 	      	    operator==(const NSIndicTermeAct& o) ;
} ;


// **************************** Table SynonyTermeRef.DB *********************
// Synonymes
// **************************************************************************


// --------------------------------------------------------------------------
//  Classe NSSynonyData
//
// Objet contenant les données
//---------------------------------------------------------------------------
class _EPISODUS NSSynonyData
{
    public:

	    NSSynonyData() ;
	    NSSynonyData(NSSynonyData& rv) ;
	    ~NSSynonyData() ;

	    NSSynonyData&   operator =  (NSSynonyData src) ;
	    int				operator == (const NSSynonyData& o) ;

	    void 		    metAZero() ;

	    // variables
        //	long int    iSynony ;
        long int        iTermRef;
	    char		    libelle[SYNONY_LIBELLE_LEN + 1] ;
	    char		    text[SYNONY_TEXT_LEN + 1] ;
} ;


class NSSynonyAct ;

// -----------------------------------------------------------------------------
//  Classe NSSynonyInfo  (destinée à être stockée dans une Array)
// -----------------------------------------------------------------------------
class NSSynonyInfo
{
 public:
	NSSynonyInfo() ;
	NSSynonyInfo(NSSynonyAct *) ;
	NSSynonyInfo(NSSynonyInfo& rv) ;
	~NSSynonyInfo() ;

	NSSynonyInfo& operator=(NSSynonyInfo src) ;
	int 		  operator==(const NSSynonyInfo& o) ;


	// variables
	NSSynonyData	*pDonnees ;
} ;


// -----------------------------------------------------------------------------
//  Classe NSTermeRefIndicAct
//
// Objet dérivé de NSFiche servant aux accès de base de données
// -----------------------------------------------------------------------------
class NSSynonyAct : public NSSynonyInfo, public NSFiche
{
 public:

	NSSynonyAct(NSContexte *pCtx) ;
	NSSynonyAct(NSSynonyAct& rv) ;
	~NSSynonyAct() ;

   //	void					metABlanc() { pDonnees->metABlanc(); }
	void					alimenteFiche() ;
	void					videFiche() ;
	DBIResult			open(bool bOpenShared = true) ;

	virtual bool	Create() ;
	virtual bool	Modify() ;

	NSSynonyAct&		operator=(NSSynonyAct src) ;
	int 	      	operator==(const NSSynonyAct& o) ;
} ;




// **************************** LexiqueClass .DB ****************************
// Jointure Nautilus - Vidal
// **************************************************************************

// --------------------------------------------------------------------------
//  Classe NSLexiqueClassData
//
// Objet contenant les données
//---------------------------------------------------------------------------
class NSLexiqueClassData
{
 public:
	NSLexiqueClassData() ;
	NSLexiqueClassData(NSLexiqueClassData& rv) ;
	~NSLexiqueClassData() ;

	NSLexiqueClassData&	operator=(NSLexiqueClassData src) ;
	int						operator==(const NSLexiqueClassData& o) ;

	void 						metAZero() ;

	// variables
	//long int					id ;
	char						lexique[TERMELEXIQUE_LEXIQUE_LEN + 1] ;
	char						classe[TERMELEXIQUE_CLASS_LEN + 1] ;
    long int                    iCode;
} ;


class NSLexiqueClassAct ;

// -----------------------------------------------------------------------------
//  Classe NSFTermeRefIndicInfo  (destinée à être stockée dans une Array)
// -----------------------------------------------------------------------------
class _EPISODUS NSLexiqueClassInfo
{
 public:
	NSLexiqueClassInfo() ;
	NSLexiqueClassInfo(NSLexiqueClassAct *) ;
	NSLexiqueClassInfo(NSLexiqueClassInfo& rv) ;
	~NSLexiqueClassInfo() ;

	NSLexiqueClassInfo& operator=(NSLexiqueClassInfo src) ;
	int 		      	operator==(const NSLexiqueClassInfo& o) ;


	// variables
	NSLexiqueClassData	*pDonnees ;
} ;


// -----------------------------------------------------------------------------
//  Classe NSTermeRefIndicAct
//
// Objet dérivé de NSFiche servant aux accès de base de données
// -----------------------------------------------------------------------------
class _EPISODUS NSLexiqueClassAct : public NSLexiqueClassInfo, public NSFiche
{
 public:

	NSLexiqueClassAct(NSContexte *pCtx) ;
	NSLexiqueClassAct(NSLexiqueClassAct& rv) ;
	~NSLexiqueClassAct() ;

   //	void					metABlanc() { pDonnees->metABlanc(); }
	void					alimenteFiche() ;
	void					videFiche() ;
	DBIResult			open(bool bOpenShared = true) ;

	virtual bool	Create() ;
	virtual bool	Modify() ;

	NSLexiqueClassAct&		operator=(NSLexiqueClassAct src) ;
	int 	      	operator==(const NSLexiqueClassAct& o) ;
} ;


// **************************** SynonyTermeRefIndic_Rel.DB ******************
// Jointure Terme de référence - Synonymes
// **************************************************************************

// --------------------------------------------------------------------------
//  Classe NSTermSynData
//
// Objet contenant les données
//---------------------------------------------------------------------------
class NSTermSynData
{
 public:
	NSTermSynData() ;
	NSTermSynData(NSTermSynData& rv) ;
	~NSTermSynData() ;

	NSTermSynData&	operator=(NSTermSynData src) ;
	int						operator==(const NSTermSynData& o) ;

	void 						metAZero() ;

	// variables
	//long int					id ;
	long int 						iTerm ;
	long int 						iSyn ;

} ;


class NSTermSynAct ;

// -----------------------------------------------------------------------------
//  Classe NSFTermeRefIndicInfo  (destinée à être stockée dans une Array)
// -----------------------------------------------------------------------------
class _EPISODUS NSTermSynInfo
{
 public:
	NSTermSynInfo() ;
	NSTermSynInfo(NSTermSynAct *) ;
	NSTermSynInfo(NSTermSynInfo& rv) ;
	~NSTermSynInfo() ;

	NSTermSynInfo& operator=(NSTermSynInfo src) ;
	int 		      	operator==(const NSTermSynInfo& o) ;


	// variables
	NSTermSynData	*pDonnees ;
} ;


// -----------------------------------------------------------------------------
//  Classe NSTermSynAct
//
// Objet dérivé de NSFiche servant aux accès de base de données
// -----------------------------------------------------------------------------
class _EPISODUS NSTermSynAct : public NSTermSynInfo, public NSFiche
{
    public:

	    NSTermSynAct(NSContexte *pCtx) ;
	    NSTermSynAct(NSTermSynAct& rv) ;
	    ~NSTermSynAct() ;

	    void		    alimenteFiche() ;	    void		    videFiche() ;
	    DBIResult	    open(bool bOpenShared = true) ;

        bool            bRelationExist(long int	iPere, long int iFrere) ;

	    virtual bool	Create() ;
	    virtual bool	Modify() ;

	    NSTermSynAct&   operator=(NSTermSynAct src) ;
	    int 	      	operator==(const NSTermSynAct& o) ;
} ;

//*****************************************************************************


typedef vector<NSLexiqueClassInfo*> NSLexiqueClassInfoArray;



#endif

