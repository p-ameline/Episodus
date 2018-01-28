//---------------------------------------------------------------------------//    NS_OBJS.CPP
//    PA   septembre 2003
//  Gestion des "choses"
//---------------------------------------------------------------------------
#include <utility.h>
#include <mem.h>
#include <string.h>
#include <cstring.h>

#include "partage\nsdivfct.h"
#include "nsbb\ns_objs.h"

// -------------------------------------------------------------------------
// -------------------- METHODES DE NSPatPaDatArray ------------------------
// -------------------------------------------------------------------------
NSObjectDatArray::NSObjectDatArray(NSContexte* pCtx)
                :NSObjectDatInfoArray(), NSRoot(pCtx)
{}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSObjectDatArray::NSObjectDatArray(NSObjectDatArray& rv)
                :NSObjectDatInfoArray(), NSRoot(rv.pContexte)
{
try
{
	if (!(rv.empty()))
		for (ObjectDatIter i = rv.begin(); i != rv.end(); i++)
			push_back(new NSObjectDatInfo (*(*i))) ;
}
catch (...)
{
	erreur("Exception NSObjectDatArray ctor.", standardError, 0) ;
}
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
void
NSObjectDatArray::vider()
{
	if (empty())
		return ;

	for (ObjectDatIter i = begin(); i != end(); )
	{
		delete *i ;
		erase(i) ;
	}
}

NSObjectDatArray::~NSObjectDatArray()
{
	vider() ;
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSObjectDatArray&
NSObjectDatArray::operator=(NSObjectDatArray src)
{
try
{
	if (this == &src)
		return *this ;

	ObjectDatIter i ;
	//
	// Effacement des éléments déjà contenus dans le vecteur destination
	//
	vider() ;
	//
	// Copie et insertion des éléments de la source
	//
	if (!(src.empty()))
  	for (i = src.begin(); i != src.end(); i++)
    	push_back(new NSObjectDatInfo(*(*i))) ;

	return *this ;
}
catch (...)
{
	erreur("Exception NSObjectDatArray = operator.", standardError, 0) ;
	return *this ;
}
}

//---------------------------------------------------------------------------//  Ajout d'un élément à l'array
//---------------------------------------------------------------------------
bool
NSObjectDatArray::ajouteElement(NSObjectDatInfo* pObj)
{
	push_back(pObj);
    return true;
}

//***************************************************************************
// Implémentation des mèthodes NSObjectData
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur copie de NSObjectData
//---------------------------------------------------------------------------
NSObjectData::NSObjectData(NSObjectData& rv)
{
	strcpy(objectID,    rv.objectID);
    strcpy(noeud, 	    rv.noeud);
	strcpy(type,   	    rv.type);
	strcpy(lexique,     rv.lexique);
    strcpy(complement,  rv.complement);
	strcpy(certitude,   rv.certitude);
    strcpy(pluriel,     rv.pluriel);
    strcpy(tran_new,    rv.tran_new);
    strcpy(tran_del,    rv.tran_del);
    strcpy(unit, 	    rv.unit);
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation de NSObjectData
//---------------------------------------------------------------------------
NSObjectData&
NSObjectData::operator=(NSObjectData rv)
{
	strcpy(objectID,    rv.objectID);
    strcpy(noeud, 	    rv.noeud);
	strcpy(type,   	    rv.type);
	strcpy(lexique,     rv.lexique);
    strcpy(complement,  rv.complement);
	strcpy(certitude,   rv.certitude);
    strcpy(pluriel,     rv.pluriel);
    strcpy(tran_new,    rv.tran_new);
    strcpy(tran_del,    rv.tran_del);
    strcpy(unit, 	    rv.unit);

	return *this;
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison de NSObjectData
//---------------------------------------------------------------------------
int
NSObjectData::operator == (const NSObjectData& o)
{
	if ((strcmp(objectID,   o.objectID)     == 0) &&
        (strcmp(noeud, 	    o.noeud) 	    == 0) &&
        (strcmp(type,	    o.type) 	    == 0) &&
        (strcmp(lexique,    o.lexique) 	    == 0) &&
        (strcmp(complement, o.complement)   == 0) &&
        (strcmp(unit,       o.unit)         == 0) &&
        (strcmp(certitude,  o.certitude)    == 0) &&
        (strcmp(pluriel,    o.pluriel) 	    == 0))
        return 1;
	else
		return 0;
}
//---------------------------------------------------------------------------
//  Met à 0 les variables de NSObjectData//---------------------------------------------------------------------------
void
NSObjectData::metAZero()
{
	//
	// Met les champs de données à zéro
	//
	memset(objectID,	 0, OBJECT_ID_LEN + 1);
    memset(noeud, 		 0, PPD_NOEUD_LEN + 1);
    memset(type, 		 0, BASE_TYPE_LEN + 1);
    memset(lexique, 	 0, BASE_LEXIQUE_LEN + 1);
    memset(complement, 	 0, BASE_COMPLEMENT_LEN + 1);
	memset(certitude, 	 0, BASE_CERTITUDE_LEN + 1);
	memset(pluriel, 	 0, BASE_PLURIEL_LEN + 1);
    memset(tran_new, 	 0, TRAN_CODE_LEN + 1);
    memset(tran_del, 	 0, TRAN_CODE_LEN + 1);
    memset(unit, 	     0, BASE_UNIT_LEN + 1);
}

/*************************************************************************/
// NSPatPaDat
/*************************************************************************/

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NSObjectDat::NSObjectDat(NSContexte* pCtx) : NSFiche(pCtx)
{
try
{
	pDonnees = new NSObjectData() ;
}
catch (...)
{
	erreur("Exception NSObjectDat ctor.", standardError, 0) ;
}
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
NSObjectDat::~NSObjectDat()
{
	delete pDonnees;
}

//---------------------------------------------------------------------------
//  Transfère le contenu de pRecBuff dans les variables de la fiche
//---------------------------------------------------------------------------
void
NSObjectDat::alimenteFiche()
{
	alimenteChamp(pDonnees->objectID,   OBD_OBJECT_FIELD,	    OBJECT_ID_LEN);
    alimenteChamp(pDonnees->noeud, 	    OBD_NOEUD_FIELD,		PPD_NOEUD_LEN);
    alimenteChamp(pDonnees->type,	    OBD_TYPE_FIELD,		    BASE_TYPE_LEN);
    alimenteChamp(pDonnees->lexique,    OBD_LEXIQUE_FIELD,	    BASE_LEXIQUE_LEN);
    alimenteChamp(pDonnees->complement, OBD_COMPLEMENT_FIELD,   BASE_COMPLEMENT_LEN);
    alimenteChamp(pDonnees->certitude,  OBD_CERTITUDE_FIELD,    BASE_CERTITUDE_LEN);
    alimenteChamp(pDonnees->pluriel,    OBD_PLURIEL_FIELD,	    BASE_PLURIEL_LEN);
    alimenteChamp(pDonnees->tran_new,   OBD_TRAN_NEW_FIELD,	    TRAN_CODE_LEN);
    alimenteChamp(pDonnees->tran_del,   OBD_TRAN_DEL_FIELD,	    TRAN_CODE_LEN);
    alimenteChamp(pDonnees->unit,	    OBD_UNIT_FIELD,	        BASE_UNIT_LEN);
}

//---------------------------------------------------------------------------
//  Transfère le contenu des valeurs de la fiche dans pRecBuff
//---------------------------------------------------------------------------
void
NSObjectDat::videFiche()
{
    videChamp(pDonnees->objectID,   OBD_OBJECT_FIELD,		OBJECT_ID_LEN);
    videChamp(pDonnees->noeud, 	    OBD_NOEUD_FIELD,		PPD_NOEUD_LEN);
    videChamp(pDonnees->type,	    OBD_TYPE_FIELD,		    BASE_TYPE_LEN);
    videChamp(pDonnees->lexique,    OBD_LEXIQUE_FIELD,	    BASE_LEXIQUE_LEN);
    videChamp(pDonnees->complement, OBD_COMPLEMENT_FIELD,   BASE_COMPLEMENT_LEN);
    videChamp(pDonnees->certitude,  OBD_CERTITUDE_FIELD,    BASE_CERTITUDE_LEN);
    videChamp(pDonnees->pluriel,    OBD_PLURIEL_FIELD,	    BASE_PLURIEL_LEN);
    videChamp(pDonnees->tran_new,   OBD_TRAN_NEW_FIELD,	    TRAN_CODE_LEN);
    videChamp(pDonnees->tran_del,   OBD_TRAN_DEL_FIELD,	    TRAN_CODE_LEN);
    videChamp(pDonnees->unit,	    OBD_UNIT_FIELD,	        BASE_UNIT_LEN);
}

//---------------------------------------------------------------------------
//  Ouvre la table
//---------------------------------------------------------------------------
DBIResult
NSObjectDat::open()
{
    char tableName[] = "NSMDATOBJ.DB";
	//
	// Appelle la fonction open() de la classe de base pour ouvrir
	// l'index primaire
	//
	lastError = NSFiche::open(tableName, NSF_PARTAGE_GLOBAL);
	return(lastError);
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
bool
NSObjectDat::Create()
{
	return TRUE;
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
bool
NSObjectDat::Modify()
{
	return TRUE;
}

//***************************************************************************
// Implémentation des méthodes NSPatPaDatInfo
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur par défaut
//---------------------------------------------------------------------------
NSObjectDatInfo::NSObjectDatInfo()
{
try
{
	// Crée l'objet de données
	pDonnees = new NSObjectData() ;
}
catch (...)
{
	erreur("Exception NSObjectDatInfo ctor.", standardError, 0) ;
}
}

//---------------------------------------------------------------------------//  Constructeur à partir d'un NSPatPatho
//---------------------------------------------------------------------------
NSObjectDatInfo::NSObjectDatInfo(NSObjectDat* pObjDat)
{
try
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSObjectData() ;
	//
	// Copie les valeurs du NSDocument
	//
	*pDonnees = *(pObjDat->pDonnees) ;
}
catch (...)
{
	erreur("Exception NSObjectDatInfo ctor(NSObjectDat*).", standardError, 0) ;
}
}

//---------------------------------------------------------------------------//  Constructeur copie
//---------------------------------------------------------------------------
NSObjectDatInfo::NSObjectDatInfo(NSObjectDatInfo& rv)
{
try
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSObjectData(*(rv.pDonnees)) ;
}
catch (...)
{
	erreur("Exception NSObjectDatInfo copy ctor.", standardError, 0) ;
}
}

//---------------------------------------------------------------------------//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSObjectDatInfo&
NSObjectDatInfo::operator=(NSObjectDatInfo src)
{
	if (this == &src)
		return *this ;
	*pDonnees = *(src.pDonnees) ;
	return *this ;
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSObjectDatInfo::operator == (const NSObjectDatInfo& o)
{
	return (*pDonnees == *(o.pDonnees));
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison avec un NSPatPathoInfo
//---------------------------------------------------------------------------
int
NSObjectDatInfo::operator == (const NSPatPathoInfo& o)
{
	if ((strcmp(pDonnees->noeud,      o.pDonnees->noeud) 	  == 0) &&
        (strcmp(pDonnees->lexique,    o.pDonnees->lexique) 	  == 0) &&
        (strcmp(pDonnees->complement, o.pDonnees->complement) == 0) &&
        (strcmp(pDonnees->certitude,  o.pDonnees->certitude)  == 0) &&
        (strcmp(pDonnees->pluriel,    o.pDonnees->pluriel) 	  == 0))
		return 1;
	else
		return 0;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSObjectDatInfo::~NSObjectDatInfo()
{
  	delete pDonnees;
}

// -------------------------------------------------------------------------
// -------------------- METHODES DE NSPatPaLocArray ------------------------
// -------------------------------------------------------------------------
NSObjectLocArray::NSObjectLocArray(NSContexte* pCtx)
                 :NSObjectLocInfoArray(), NSRoot(pCtx)
{}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSObjectLocArray::NSObjectLocArray(NSObjectLocArray& rv)
                 :NSObjectLocInfoArray(), NSRoot(rv.pContexte)
{
try
{
	if (!(rv.empty()))
		for (ObjectLocIter i = rv.begin(); i != rv.end(); i++)
			push_back(new NSObjectLocInfo(*(*i))) ;
}
catch (...)
{
	erreur("Exception NSObjectLocArray copy ctor.", standardError, 0) ;
}
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
void
NSObjectLocArray::vider()
{
	if (empty())
		return ;

	for (ObjectLocIter i = begin(); i != end(); )
	{
		delete *i ;
		erase(i) ;
	}
}
NSObjectLocArray::~NSObjectLocArray(){
	vider() ;
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSObjectLocArray&
NSObjectLocArray::operator=(NSObjectLocArray src)
{
try
{
	if (this == &src)
		return *this ;

	ObjectLocIter i ;
	//
	// Effacement des éléments déjà contenus dans le vecteur destination
	//
	vider() ;
	//
	// Copie et insertion des éléments de la source
	//
	if (!(src.empty()))
		for (i = src.begin(); i != src.end(); i++)
			push_back(new NSObjectLocInfo(*(*i))) ;

	return *this ;
}
catch (...)
{
	erreur("Exception NSObjectLocArray = operator.", standardError, 0) ;
	return *this ;
}
}

//---------------------------------------------------------------------------//  Ajout d'un élément à l'array
//---------------------------------------------------------------------------
bool
NSObjectLocArray::ajouteElement(NSObjectLocInfo* pObj)
{
	push_back(pObj);
    return true;
}

//***************************************************************************
// Implémentation des mèthodes NSObjectLoc
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur copie de NSObjectLocData
//---------------------------------------------------------------------------
NSObjectLocData::NSObjectLocData(NSObjectLocData& rv)
{
	strcpy(objectID, 	        rv.objectID) ;
    strcpy(transaction, 	    rv.transaction) ;
    strcpy(noeud, 			    rv.noeud) ;
    strcpy(codeLocalisation,    rv.codeLocalisation) ;
    strcpy(visible, 		    rv.visible) ;
    strcpy(interet, 		    rv.interet) ;
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation de NSPatPaLocData
//---------------------------------------------------------------------------
NSObjectLocData&
NSObjectLocData::operator=(NSObjectLocData rv)
{
	strcpy(objectID, 	        rv.objectID) ;
    strcpy(transaction, 	    rv.transaction) ;
    strcpy(noeud, 			    rv.noeud) ;
    strcpy(codeLocalisation,    rv.codeLocalisation) ;
    strcpy(visible, 		    rv.visible) ;
    strcpy(interet, 		    rv.interet) ;

	return *this;
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison de NSPatPaLocData
//---------------------------------------------------------------------------
int
NSObjectLocData::operator == (const NSObjectLocData& o)
{
	if ((strcmp(objectID,  		    o.objectID) 	    == 0) &&
        (strcmp(transaction,  		o.transaction) 	    == 0) &&
        (strcmp(noeud, 		  		o.noeud) 			== 0) &&
        (strcmp(codeLocalisation,	o.codeLocalisation) == 0) &&
        (strcmp(visible,   	  		o.visible) 			== 0))
		return 1;
	else
		return 0;
}

//---------------------------------------------------------------------------
//  Met à 0 les variables de NSPatPaLocData
//---------------------------------------------------------------------------
void
NSObjectLocData::metAZero()
{
	//
	// Met les champs de données à zéro
	//
    memset(objectID,		 0, OBJECT_ID_LEN + 1);
    memset(transaction, 	 0, TRAN_CODE_LEN + 1);
    memset(noeud, 			 0, PPD_NOEUD_LEN + 1);
    memset(codeLocalisation, 0, BASE_LOCALISATION_LEN + 1);
	memset(visible, 		 0, BASE_VISIBLE_LEN + 1);
    memset(interet, 	     0, BASE_INTERET_LEN + 1);
}

/*************************************************************************/
// NSObjectLoc
/*************************************************************************/

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NSObjectLoc::NSObjectLoc(NSContexte* pCtx)
            :NSFiche(pCtx)
{
try
{
	pDonnees = new NSObjectLocData() ;
}
catch (...)
{
	erreur("Exception NSObjectLoc ctor.", standardError, 0) ;
}
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSObjectLoc::~NSObjectLoc()
{
	delete pDonnees;
}

//---------------------------------------------------------------------------
//  Transfère le contenu de pRecBuff dans les variables de la fiche
//---------------------------------------------------------------------------
void
NSObjectLoc::alimenteFiche()
{
    alimenteChamp(pDonnees->objectID,  	        OBL_OBJECT_FIELD,   OBJECT_ID_LEN);
    alimenteChamp(pDonnees->transaction,  	    OBL_TRAN_FIELD,	    TRAN_CODE_LEN);
    alimenteChamp(pDonnees->noeud, 		  	    OBL_NOEUD_FIELD,    PPD_NOEUD_LEN);
    alimenteChamp(pDonnees->codeLocalisation,   OBL_LOC_FIELD,	    BASE_LOCALISATION_LEN);
    alimenteChamp(pDonnees->visible,		    OBL_VISIBLE_FIELD,  BASE_VISIBLE_LEN);
    alimenteChamp(pDonnees->interet,	        OBL_INTERET_FIELD,  BASE_INTERET_LEN);
}

//---------------------------------------------------------------------------
//  Transfère le contenu des valeurs de la fiche dans pRecBuff
//---------------------------------------------------------------------------
void
NSObjectLoc::videFiche()
{
    videChamp(pDonnees->objectID,  	        OBL_OBJECT_FIELD,   OBJECT_ID_LEN);
    videChamp(pDonnees->transaction,  	    OBL_TRAN_FIELD,	    TRAN_CODE_LEN);
    videChamp(pDonnees->noeud, 		  	    OBL_NOEUD_FIELD,    PPD_NOEUD_LEN);
    videChamp(pDonnees->codeLocalisation,   OBL_LOC_FIELD,	    BASE_LOCALISATION_LEN);
    videChamp(pDonnees->visible,		    OBL_VISIBLE_FIELD,  BASE_VISIBLE_LEN);
    videChamp(pDonnees->interet,	        OBL_INTERET_FIELD,  BASE_INTERET_LEN);
}

//---------------------------------------------------------------------------
//  Ouvre la table
//---------------------------------------------------------------------------
DBIResult
NSObjectLoc::open()
{
    char tableName[] = "NSMLOCOBJ.DB";
	//
	// Appelle la fonction open() de la classe de base pour ouvrir
	// l'index primaire
	//
	lastError = NSFiche::open(tableName, NSF_PARTAGE_GLOBAL);
	return(lastError);
}

//---------------------------------------------------------------------------////---------------------------------------------------------------------------
bool
NSObjectLoc::Create()
{
    return TRUE;
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
bool
NSObjectLoc::Modify()
{
    return TRUE;
}

//***************************************************************************
// Implémentation des méthodes NSPatPaLocInfo
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur par défaut
//---------------------------------------------------------------------------
NSObjectLocInfo::NSObjectLocInfo()
{
try
{
	// Crée l'objet de données
	pDonnees = new NSObjectLocData();
}
catch (...)
{
    erreur("Exception NSObjectLocInfo ctor.", standardError, 0);
}
}

//---------------------------------------------------------------------------//  Constructeur à partir d'un NSPatPaLoc
//---------------------------------------------------------------------------
NSObjectLocInfo::NSObjectLocInfo(NSObjectLoc* pPatPaLoc)
{
try
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSObjectLocData() ;
	//
	// Copie les valeurs du NSDocument
	//
	*pDonnees = *(pPatPaLoc->pDonnees) ;
}
catch (...)
{
	erreur("Exception NSObjectLocInfo ctor(NSPatLoc*).", standardError, 0) ;
}
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSObjectLocInfo::NSObjectLocInfo(NSObjectLocInfo& rv)
{
try
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSObjectLocData(*(rv.pDonnees)) ;
}
catch (...)
{
	erreur("Exception NSObjectLocInfo copy ctor.", standardError, 0) ;
}
}

//---------------------------------------------------------------------------//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSObjectLocInfo&
NSObjectLocInfo::operator=(NSObjectLocInfo src)
{
	*pDonnees = *(src.pDonnees);
	return *this;
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSObjectLocInfo::operator == (const NSObjectLocInfo& o)
{
	return (*pDonnees == *(o.pDonnees));
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSObjectLocInfo::~NSObjectLocInfo()
{
  	delete pDonnees;
}

//***************************************************************************
// 				  Implémentation des mèthodes NSObjectTLibreData
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSObjectTLibreData::NSObjectTLibreData(NSObjectTLibreData& rv)
{
    strcpy(object,  rv.object);
    strcpy(code, 	rv.code);
    strcpy(suivant, rv.suivant);
    strcpy(texte, 	rv.texte);
}

NSObjectTLibreData::~NSObjectTLibreData()
{
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSObjectTLibreData&
NSObjectTLibreData::operator=(NSObjectTLibreData rv)
{
    strcpy(object,  rv.object);
    strcpy(code, 	rv.code);
    strcpy(suivant, rv.suivant);
    strcpy(texte, 	rv.texte);

	return *this;
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSObjectTLibreData::operator == ( const NSObjectTLibreData& o )
{
    if ((strcmp(code, 	 o.code) 	== 0) &&
        (strcmp(object,  o.object) == 0) &&
        (strcmp(texte, 	 o.texte) 	== 0) &&
        (strcmp(suivant, o.suivant) == 0))
		return 1;
	else
		return 0;
}

//---------------------------------------------------------------------------//  Met à 0 les variables.
//---------------------------------------------------------------------------
void
NSObjectTLibreData::metAZero()
{
	//
	// Met les champs de données à zéro
	//
    memset(object,  0, OBJECT_ID_LEN      + 1);
	memset(code,	0, TLIBRE_CODE_LEN    + 1);
    memset(texte, 	0, TLIBRE_TEXTE_LEN   + 1);
    memset(suivant, 0, TLIBRE_SUIVANT_LEN + 1);
}

//---------------------------------------------------------------------------//  NSObjectTLibreData::metABlanc()
//---------------------------------------------------------------------------
void
NSObjectTLibreData::metABlanc()
{
	//
	// Met les champs de données à blanc
	//
    memset(object,  ' ', OBJECT_ID_LEN);
	memset(code,	' ', TLIBRE_CODE_LEN);
    memset(texte, 	' ', TLIBRE_TEXTE_LEN);
    memset(suivant, ' ', TLIBRE_SUIVANT_LEN);
}

//***************************************************************************
// 				Implémentation des méthodes NSObjectTLibreInfo
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur par défaut
//---------------------------------------------------------------------------
NSObjectTLibreInfo::NSObjectTLibreInfo()
{
    // Crée l'objet de données
	pDonnees = new NSObjectTLibreData();
}

//---------------------------------------------------------------------------
//  Constructeur à partir d'un NSObjectTLibre (objet dérivant de NSFiche)
//---------------------------------------------------------------------------
NSObjectTLibreInfo::NSObjectTLibreInfo(NSObjectTLibre* pNSTextelibre)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSObjectTLibreData();
	//
	// Copie les valeurs du NSDocument
	//
	*pDonnees = *(pNSTextelibre->pDonnees);
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSObjectTLibreInfo::NSObjectTLibreInfo(NSObjectTLibreInfo& rv)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSObjectTLibreData();
	//
	// Copie les valeurs du NSObjectTLibreInfo d'origine
	//
	*pDonnees = *(rv.pDonnees);
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
NSObjectTLibreInfo::~NSObjectTLibreInfo()
{
    if (pDonnees)
        delete pDonnees;
}

//---------------------------------------------------------------------------//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSObjectTLibreInfo&
NSObjectTLibreInfo::operator=(NSObjectTLibreInfo src)
{
    *pDonnees = *(src.pDonnees);
	return *this;
}

//---------------------------------------------------------------------------//  Opérateur de comparaison
//---------------------------------------------------------------------------
int NSObjectTLibreInfo::operator == ( const NSObjectTLibreInfo& o )
{
	return (*pDonnees == *(o.pDonnees));
}

//***************************************************************************
//				  Implémentation des mèthodes NSObjectTLibre
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NSObjectTLibre::NSObjectTLibre(NSContexte* pCtx) : NSFiche(pCtx)
{
    pDonnees = new NSObjectTLibreData();
}

//---------------------------------------------------------------------------//  Destructeur.
//---------------------------------------------------------------------------
NSObjectTLibre::~NSObjectTLibre()
{
    if (pDonnees)
   	    delete pDonnees;
}

//---------------------------------------------------------------------------//  Transfère le contenu de pRecBuff dans les variables de la fiche
//---------------------------------------------------------------------------
void
NSObjectTLibre::alimenteFiche()
{
    alimenteChamp(pDonnees->object,  OBJTLIBRE_OBJECT_FIELD,  OBJECT_ID_LEN);	alimenteChamp(pDonnees->code,    OBJTLIBRE_CODE_FIELD,    TLIBRE_CODE_LEN);    alimenteChamp(pDonnees->texte,   OBJTLIBRE_TEXTE_FIELD,   TLIBRE_TEXTE_LEN);
    alimenteChamp(pDonnees->suivant, OBJTLIBRE_SUIVANT_FIELD, TLIBRE_SUIVANT_LEN);
}

//---------------------------------------------------------------------------//  Transfère le contenu des valeurs de la fiche dans pRecBuff
//---------------------------------------------------------------------------
void
NSObjectTLibre::videFiche()
{
    videChamp(pDonnees->object,  OBJTLIBRE_OBJECT_FIELD,    OBJECT_ID_LEN);
	videChamp(pDonnees->code,    OBJTLIBRE_CODE_FIELD,      TLIBRE_CODE_LEN);
    videChamp(pDonnees->texte,   OBJTLIBRE_TEXTE_FIELD,     TLIBRE_TEXTE_LEN);
    videChamp(pDonnees->suivant, OBJTLIBRE_SUIVANT_FIELD,   TLIBRE_SUIVANT_LEN);
}

//---------------------------------------------------------------------------//  Ouvre la table primaire
//---------------------------------------------------------------------------
DBIResult
NSObjectTLibre::open()
{
    char tableName[] = "NSMTLIBOBJ.DB";
	//	// Appelle la fonction open() de la classe de base pour ouvrir
	// l'index primaire
	//
	lastError = NSFiche::open(tableName, NSF_PARTAGE_GLOBAL);
	return(lastError);
}

//***************************************************************************
// 				  Implémentation des méthodes NSObjectTLibreBase
//***************************************************************************
NSObjectTLibreBase::NSObjectTLibreBase(NSContexte* pCtx)
             :NSRoot(pCtx)
{
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSObjectTLibreBase::NSObjectTLibreBase(NSObjectTLibreBase& rv)
             :NSRoot(rv.pContexte)
{}

NSObjectTLibreBase::~NSObjectTLibreBase()
{}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSObjectTLibreBase&
NSObjectTLibreBase::operator=(NSObjectTLibreBase rv)
{
    return *this;
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSObjectTLibreBase::operator == (const NSObjectTLibreBase& o )
{
    return 1;
}

//---------------------------------------------------------------------------
//  Création du curseur d'acces à la base
//---------------------------------------------------------------------------
bool
NSObjectTLibreBase::creerCurseur()
{
try
{
	pNSTexteLibre = new NSObjectTLibre(pContexte) ;
	return true ;
}
catch (...)
{
	erreur("Exception NSObjectTLibreBase::creerCurseur", standardError, 0) ;
	return false ;
}
}

//---------------------------------------------------------------------------// Calcule le code à donner au nouveau texte libre
//---------------------------------------------------------------------------
bool
NSObjectTLibreBase::IncrementeCode(string* pTexte)
{
    int i = strlen(pTexte->c_str()) ;

    if (i == 1)
        return false;

    bool tourner = true;
    while ((tourner) && (i > 1))    {
        i--;
   	    if ((((*pTexte)[i] >= '0') && ((*pTexte)[i] < '9')) ||
      	    (((*pTexte)[i] >= 'A') && ((*pTexte)[i] < 'Z')))
        {
            (*pTexte)[i] = char((*pTexte)[i] + 1);
            tourner = false;
        }
        else if ((*pTexte)[i] == '9')
        {
            (*pTexte)[i] = 'A';
            tourner = false;
        }
        else
            (*pTexte)[i] = '0';
    }

    return(!tourner);
}

//----------------------------------------------------------------------------//  Insertion d'un nouveau texte dans la base de textes libres
//
//  sTexte : string qui contient le texte à stocker
//	pCode  : code de la fiche qui va contenir le texte
//	         le code doit être différent de "00000"
//----------------------------------------------------------------------------

boolNSObjectTLibreBase::InsereTexte(string sCodeObj, string *pCode, string sTexte)
{
    //le nouveau code est le dernier + 1
    string code;
    string suivant;
    string sObj;

try{
    // Pour éviter de stocker des textes libres vides
    // on considère par convention qu'un texte libre vide
    // est associé à un code vide.
    if (sTexte == "")
    {
        *pCode = "";
        return true;
    }

    if (!(creerCurseur()))
        return false;

	pNSTexteLibre->lastError = pNSTexteLibre->open();
    if (pNSTexteLibre->lastError != DBIERR_NONE)    {
        erreur("1.Erreur à l'ouverture de la base des textes libres objet.", standardError, pNSTexteLibre->lastError);
        delete pNSTexteLibre;
        pNSTexteLibre = 0;
		return false;
    }

    bool blocage = true;
    while (blocage)    {
   	    // blocage de la base en écriture
   	    pNSTexteLibre->lastError = DbiAcqTableLock(pNSTexteLibre->PrendCurseur(), dbiWRITELOCK);

   	    if (pNSTexteLibre->lastError == DBIERR_LOCKED)            blocage = true;
        else
            blocage = false;
    }

    if (pNSTexteLibre->lastError != DBIERR_NONE)	{
        erreur("2.Erreur au blocage de la base des textes libres objet.", standardError, pNSTexteLibre->lastError);
        delete pNSTexteLibre;
        pNSTexteLibre = 0;
		return false;
    }

    sObj = sCodeObj;
    // on regarde si le texte n'existe pas déjà, auquel cas
    // on peut réatribuer son code.
    int premier = 0;
    int dernier;
    string sTL;
    string sTLTrouve;
    string sCodeTrouve;

    if (strlen(sTexte.c_str()) <= TLIBRE_TEXTE_LEN)
        dernier = strlen(sTexte.c_str());
    else
        dernier = TLIBRE_TEXTE_LEN;

    // on crée la chaine amorce
    sTL = string(sTexte, premier, dernier);

    CURProps curProps;
    DBIResult lastError = DbiGetCursorProps(pNSTexteLibre->PrendCurseur(), curProps) ;
    Byte* pIndexRec = new Byte[curProps.iRecBufSize] ;
    memset(pIndexRec, 0, curProps.iRecBufSize) ;
    DbiPutField(pNSTexteLibre->PrendCurseur(), OBJTLIBRE_OBJECT_FIELD, pIndexRec, (Byte*) sObj.c_str()) ;
    DbiPutField(pNSTexteLibre->PrendCurseur(), OBJTLIBRE_TEXTE_FIELD,  pIndexRec, (Byte*) sTL.c_str()) ;

    pNSTexteLibre->lastError = pNSTexteLibre->chercheClefComposite("TEXTE_INDEX",
                                                                    NODEFAULTINDEX,
                                                                    keySEARCHEQ,
                                                                    dbiWRITELOCK,
                                                                    pIndexRec) ;
    delete[] pIndexRec ;
    if ((pNSTexteLibre->lastError != DBIERR_NONE) && (pNSTexteLibre->lastError != DBIERR_RECNOTFOUND))
    {
        erreur("Le fichier des textes libres objet est endommagé.", standardError, pNSTexteLibre->lastError);
        pNSTexteLibre->close() ;
        delete pNSTexteLibre ;
        return false;
    }

    // Si on ne trouve pas l'amorce, on sort.
    if (pNSTexteLibre->lastError == DBIERR_NONE)
    {
        pNSTexteLibre->lastError = pNSTexteLibre->getRecord();
        if (pNSTexteLibre->lastError != DBIERR_NONE)
        {
            erreur("Le fichier des textes libres objet est endommagé.", standardError, pNSTexteLibre->lastError);
            pNSTexteLibre->close() ;
            delete pNSTexteLibre;
            return false;
        }

        // Sert à la recherche du texte
        NSObjectTLibreBase* pNSTlibre = new NSObjectTLibreBase(pContexte);

        do
        {
            if ((string(pNSTexteLibre->pDonnees->object) != sObj) ||
                (string(pNSTexteLibre->pDonnees->texte) != sTL))
                break ;

            sCodeTrouve = string(pNSTexteLibre->pDonnees->code);

            if (!pNSTlibre->RecupereTexte(sObj, sCodeTrouve, &sTLTrouve))
            {
		        erreur("Le fichier des textes libres objet est endommagé.", standardError, pNSTexteLibre->lastError);
		        pNSTexteLibre->close() ;
		        delete pNSTexteLibre;
                delete pNSTlibre;
		        return false;
            }

            // Si on trouve le même texte libre dans la base, on renvoie son code
            if (sTLTrouve == sTexte)
            {
		        *pCode = sCodeTrouve;
		        pNSTexteLibre->close() ;
		        delete pNSTexteLibre;
                delete pNSTlibre;
		        return true;
            }

            pNSTexteLibre->lastError = pNSTexteLibre->suivant(dbiWRITELOCK);

		    if ((pNSTexteLibre->lastError != DBIERR_NONE) &&
			    (pNSTexteLibre->lastError != DBIERR_EOF))
            {
			    erreur("Erreur à l'accès au texte libre objet suivant.", 0, pNSTexteLibre->lastError);
                pNSTexteLibre->close() ;
		        delete pNSTexteLibre;
                delete pNSTlibre;
                return false ;
            }
		    else if (pNSTexteLibre->lastError == DBIERR_NONE)
		    {
			    pNSTexteLibre->lastError = pNSTexteLibre->getRecord();
			    if (pNSTexteLibre->lastError != DBIERR_NONE)
                {
				    erreur("Erreur à la lecture d'un texte libre objet.", 0, pNSTexteLibre->lastError);
                    pNSTexteLibre->close() ;
		            delete pNSTexteLibre;
                    delete pNSTlibre;
                    return false ;
                }
            }
            else
                break ;
        }
        while (1);

        delete pNSTlibre;
    }

    string codeTL;
    bool bTrouve = false;

	char PremOrdre[TLIBRE_CODE_LEN + 1], DernOrdre[TLIBRE_CODE_LEN +1 ];
	//    // Préparation des compteurs mini et maxi
	//

    int i;
    PremOrdre[0] = '_';
    DernOrdre[0] = '_';

	for (i = 1; i < TLIBRE_CODE_LEN; i++)	{
		PremOrdre[i] = '0';
		DernOrdre[i] = 'z';
	}

	PremOrdre[i] 	= '\0';	DernOrdre[i] 	= '\0';

	//	// On tente de se positionner sur le dernier code possible : "zzz"
	//
    codeTL = sObj + string(DernOrdre);

	pNSTexteLibre->lastError = pNSTexteLibre->chercheClef(&codeTL,                                                  "",
                                                  0,
                                                  keySEARCHGEQ,
                                                  dbiWRITELOCK);

	//	// Si on se trouve positionné en fin de fichier on recule
	//

	if (pNSTexteLibre->lastError == DBIERR_EOF)		pNSTexteLibre->lastError = pNSTexteLibre->precedent(dbiWRITELOCK);
	//
	// Toute autre erreur est anormale
	//
	else if (pNSTexteLibre->lastError != DBIERR_NONE)
	{
		erreur("Le fichier des textes libres objet est endommagé.", 0, pNSTexteLibre->lastError);
        pNSTexteLibre->close();
		delete pNSTexteLibre;
		return false;
	}

	//
	// On récupère l'enregistrement
	//

	pNSTexteLibre->lastError = pNSTexteLibre->getRecord();
	//	// Si on est en fin ou début de fichier, c'est qu'il est vide
	//

	if ((pNSTexteLibre->lastError == DBIERR_BOF) ||		 (pNSTexteLibre->lastError == DBIERR_EOF) ||
		 (pNSTexteLibre->lastError == DBIERR_NOCURRREC))
	{
		*pCode = string(PremOrdre);
		bTrouve = true;
	}

    if (!bTrouve)
    {
	    //	    // Toute autre erreur est anormale
	    //

	    if (pNSTexteLibre->lastError != DBIERR_NONE)	    {
		    erreur("Le fichier des textes libres objet est endommagé.", 0, pNSTexteLibre->lastError);
            pNSTexteLibre->close();
		    delete pNSTexteLibre;
		    return false;
        }

	    //	    // Si le TL trouvé n'appartient pas au patient, on recule d'un cran
	    //

	    if (strcmp(pNSTexteLibre->pDonnees->object, sObj.c_str()) != 0)	    {
		    pNSTexteLibre->lastError = pNSTexteLibre->precedent(dbiWRITELOCK);

		    //		    // Si on est en début de fichier, c'est qu'il n'y a pas de TL pour ce patient
		    //

		    if (pNSTexteLibre->lastError == DBIERR_BOF)		    {
			    *pCode = string(PremOrdre);
			    bTrouve = true;
		    }

            if (!bTrouve)
            {
		        //		        // Toute autre erreur est anormale
		        //

		        if (pNSTexteLibre->lastError == DBIERR_NONE)			        pNSTexteLibre->lastError = pNSTexteLibre->getRecord();

		        if (pNSTexteLibre->lastError != DBIERR_NONE)		        {
			        erreur("Le fichier des textes libres objet est endommagé.", 0, pNSTexteLibre->lastError);
                    pNSTexteLibre->close();
			        delete pNSTexteLibre;
			        return false;
		        }

		        //		        // Si le TL trouvé n'appartient pas au patient, il n'y a pas
		        // encore de TL pour ce patient
		        //

		        if (strcmp(pNSTexteLibre->pDonnees->object,								sObj.c_str()) != 0)
		        {
			        *pCode = string(PremOrdre);
		        }
                else
                {
                    *pCode = string(pNSTexteLibre->pDonnees->code);
                }
	        }
        }
        else
        {
            *pCode = string(pNSTexteLibre->pDonnees->code);
        }
    }

	//	// On est positionné sur le dernier TL du patient, que l'on doit
	// incrémenter pour avoir le nouveau code
	//
    if (!IncrementeCode(pCode))
    {
        erreur("Erreur à l'incrémentation du code textelibre objet.", 0, pNSTexteLibre->lastError);
        pNSTexteLibre->close();
        delete pNSTexteLibre;
        pNSTexteLibre = 0;
        return false;
    }

    code = *pCode;
    //    // Décomposition de sTexte en une liste chainée d'enregistrements de blocs
    // de TLIBRE_TEXTE_LEN caractères
    //

    size_t  bloc, j, h ;    int     k = 1 ;
    bool continuer = true;
    if (strlen(sTexte.c_str()) <= TLIBRE_TEXTE_LEN /*40*/)    {
   	    bloc = strlen(sTexte.c_str());
        suivant = string(TLIBRE_CODE_LEN, '0'); // pas de suivant
    }
    else
    {
   	    bloc = TLIBRE_TEXTE_LEN;
        suivant = *pCode;
        code    = *pCode;
        IncrementeCode(&suivant);
    }

    while (continuer)    {
        pNSTexteLibre->metABlanc();

        strcpy(pNSTexteLibre->pDonnees->object, sObj.c_str());
        strcpy(pNSTexteLibre->pDonnees->code, code.c_str());
        strcpy(pNSTexteLibre->pDonnees->suivant, suivant.c_str());

        int t = (k - 1) * TLIBRE_TEXTE_LEN;
        for (j = t ; j < bloc ; j++)            pNSTexteLibre->pDonnees->texte[j - t] = sTexte[j];

        pNSTexteLibre->lastError = pNSTexteLibre->appendRecord();
        if (pNSTexteLibre->lastError != DBIERR_NONE)        {
      	    erreur("5.Erreur à l'ajout de la fiche textelibre objet.", 0, pNSTexteLibre->lastError);
            pNSTexteLibre->close();
            delete pNSTexteLibre;
            pNSTexteLibre = 0;
            return false;
        }

        h = bloc;
        k++;   //nombre de blocs
        bloc = k * TLIBRE_TEXTE_LEN;
        if (!IncrementeCode(&code))        {            erreur("Erreur à l'incrémentation du code textelibre objet.", 0, pNSTexteLibre->lastError);            pNSTexteLibre->close();
            delete pNSTexteLibre;
            pNSTexteLibre = 0;
            return false;        }        suivant = code;

        if (!IncrementeCode(&suivant))
        {
            erreur("Erreur à l'incrémentation du code suivant textelibre objet.", 0, pNSTexteLibre->lastError);
            pNSTexteLibre->close();
            delete pNSTexteLibre;
            pNSTexteLibre = 0;
            return false;
        }

        if (bloc == strlen(sTexte.c_str()))        {
      	    suivant = string(TLIBRE_CODE_LEN, '0'); // pas de suivant
            continuer = false;
        }

        if (bloc > strlen(sTexte.c_str()))      	    continuer = false;
    }

    // si le reste de sTexte est inférieur à 40 caractéres
    if (h < strlen(sTexte.c_str()))    {
        suivant = string(TLIBRE_CODE_LEN, '0'); // pas de suivant
        pNSTexteLibre->metABlanc();

        strcpy(pNSTexteLibre->pDonnees->object, sObj.c_str());
        strcpy(pNSTexteLibre->pDonnees->code,  code.c_str());
        strcpy(pNSTexteLibre->pDonnees->suivant, suivant.c_str() );

        for (j = h; j <strlen(sTexte.c_str()); j++)            pNSTexteLibre->pDonnees->texte[j - h] = sTexte[j];

        pNSTexteLibre->lastError = pNSTexteLibre->appendRecord();
        if (pNSTexteLibre->lastError != DBIERR_NONE)        {
      	    erreur("6.Erreur à l'ajout de la fiche textelibre objet.", 0, pNSTexteLibre->lastError);
            pNSTexteLibre->close();
            delete pNSTexteLibre;
            pNSTexteLibre = 0;
            return false;
        }
    }

    pNSTexteLibre->close();    delete pNSTexteLibre;
    pNSTexteLibre = 0;
    return true;
}
catch (...)
{
    erreur("Exception (insert ObjectTL)", 0, 0);
    return false;
}
}


//----------------------------------------------------------------------------//  Mettre le contenu de la fiche de code sCode dans pTexte
//	le code doit être différent de "00000"
//----------------------------------------------------------------------------

boolNSObjectTLibreBase::RecupereTexte(string sCodeObj, string sCode, string* pTexte)
{
try
{
    // Pour éviter de stocker des textes libres vides
    // on considère par convention qu'un texte libre vide
    // est associé à un code vide.
    if (sCode == "")
    {
        *pTexte = "";
        return true;
    }

    if (!(creerCurseur()))
        return false;

    pNSTexteLibre->lastError = pNSTexteLibre->open();
    if (pNSTexteLibre->lastError != DBIERR_NONE)	{
        erreur("7.Erreur à l'ouverture de la base des textes libres objet.", 0, pNSTexteLibre->lastError);
        delete pNSTexteLibre;
		return false;
    }

    string codeTL = sCodeObj + sCode;

	pNSTexteLibre->lastError = pNSTexteLibre->chercheClef(&codeTL,                                                          "",
                                                          0,
                                                          keySEARCHEQ,
                                                          dbiWRITELOCK);

    if (pNSTexteLibre->lastError != DBIERR_NONE)    {
   	    erreur("8.Erreur à recherche des textes libres objet.", 0, pNSTexteLibre->lastError);
        pNSTexteLibre->close();
        delete pNSTexteLibre;
   	    return false;
    }

    pNSTexteLibre->lastError = pNSTexteLibre->getRecord();
    if (pNSTexteLibre->lastError != DBIERR_NONE)    {
        erreur("9.Le fichier de la base des textes libres objet est inutilisable.", 0, pNSTexteLibre->lastError);
        pNSTexteLibre->close();
        delete pNSTexteLibre;
        return false;
    }

	(*pTexte) = (string(pNSTexteLibre->pDonnees->texte).c_str());    (*pTexte) += string(TLIBRE_TEXTE_LEN - strlen(pNSTexteLibre->pDonnees->texte), ' ');

    while (strcmp(pNSTexteLibre->pDonnees->suivant, string(TLIBRE_CODE_LEN, '0').c_str()))    {
        codeTL = sCodeObj + string(pNSTexteLibre->pDonnees->suivant);

   	    pNSTexteLibre->lastError = pNSTexteLibre->chercheClef(&codeTL,
                                                              "",
                                                              0,
                                                              keySEARCHEQ,
                                                              dbiWRITELOCK);

        if (pNSTexteLibre->lastError != DBIERR_NONE)        {
      	    erreur("10.Erreur à la recherche des textes libres objet.", 0, pNSTexteLibre->lastError);
            pNSTexteLibre->close();
            delete pNSTexteLibre;
   		    return false;
        }

   	    pNSTexteLibre->lastError = pNSTexteLibre->getRecord();
   	    if (pNSTexteLibre->lastError != DBIERR_NONE)   	    {
   		    erreur("11.Le fichier des textes libres objet est inutilisable.", 0, pNSTexteLibre->lastError);
            pNSTexteLibre->close();
            delete pNSTexteLibre;
      	    return false;
   	    }

   	    (*pTexte) += (string(pNSTexteLibre->pDonnees->texte).c_str());        (*pTexte) += string(TLIBRE_TEXTE_LEN - strlen(pNSTexteLibre->pDonnees->texte), ' ');
    }

    int Compteur = strlen(pTexte->c_str()) - 1;
    while (((*pTexte)[Compteur] == ' ') && (Compteur > 0))   	    Compteur--;

    *pTexte = string(*pTexte, 0, Compteur + 1);
    pNSTexteLibre->close();    delete pNSTexteLibre;
    return true;
}
catch (...)
{
    erreur("Exception (recup ObjectTL)", 0, 0);
    return false;
}
}


