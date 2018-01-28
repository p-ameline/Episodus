//---------------------------------------------------------------------------
//    NSANXARY.CPP
//    PA   juillet 94
//    Kaddachi Hafedh
//  Implémentation des Arrays d'objets annexes de NAUTILUS
//---------------------------------------------------------------------------

#include "nautilus\nssuper.h"

#include "partage\nsdivfct.h"
#include "nsoutil\nsvpatbd.h"
#include "partage\stringar.h"
#include "nsbb\nsbbsmal.h"

// -------------------------------------------------------------------------
// -------------------- METHODES DE NSPatPaDatArray ------------------------
// -------------------------------------------------------------------------
NSPatOldDatArray::NSPatOldDatArray(NSContexte* pCtx)
					 :NSPatOldDatInfoArray(), NSRoot(pCtx)
{}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSPatOldDatArray::NSPatOldDatArray(NSPatOldDatArray& rv)
					 :NSPatOldDatInfoArray(), NSRoot(rv.pContexte)
{
    for (PatOldDatIter i = rv.begin(); i != rv.end(); i++)
   	    push_back(new NSPatPaOldDatInfo (*(*i)));
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
void
NSPatOldDatArray::vider()
{
   if (empty())
   	return;

	for (PatOldDatIter i = begin(); i != end(); )
   {
   	delete *i;
      erase(i);
   }
}

NSPatOldDatArray::~NSPatOldDatArray()
{
	vider();
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSPatOldDatArray&
NSPatOldDatArray::operator=(NSPatOldDatArray src)
{
   PatOldDatIter i;
   //
   // Effacement des éléments déjà contenus dans le vecteur destination
   //
	vider();
	//
   // Copie et insertion des éléments de la source
   //
  	for (i = src.begin(); i != src.end(); i++)
   	push_back(new NSPatPaOldDatInfo(*(*i)));
	return *this;
}

//---------------------------------------------------------------------------
//  Ajout d'un élément à l'array
//---------------------------------------------------------------------------
bool
NSPatOldDatArray::ajouteElement(NSPatPaOldDatInfo* pPatPatho)
{
	push_back(pPatPatho);
   return true;
}

//***************************************************************************
// Implémentation des mèthodes NSPatPaDat
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur copie de NSPatOldDatData
//---------------------------------------------------------------------------
NSPatOldDatData::NSPatOldDatData(NSPatOldDatData& rv)
{
	strcpy(codePatient, 	rv.codePatient);
   strcpy(codeDocument, rv.codeDocument);
   strcpy(noeud, 			rv.noeud);
	strcpy(type,   	 	rv.type);
	strcpy(lexique, 	 	rv.lexique);
   strcpy(complement, 	rv.complement);
	strcpy(certitude,  	rv.certitude);
	strcpy(interet,    	rv.interet);
   strcpy(pluriel,    	rv.pluriel);
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation de NSPatOldDatData
//---------------------------------------------------------------------------
NSPatOldDatData&
NSPatOldDatData::operator=(NSPatOldDatData rv)
{
	strcpy(codePatient, 	rv.codePatient);
   strcpy(codeDocument, rv.codeDocument);
   strcpy(noeud, 			rv.noeud);
	strcpy(type,   	 	rv.type);
	strcpy(lexique, 	 	rv.lexique);
   strcpy(complement, 	rv.complement);
	strcpy(certitude,  	rv.certitude);
	strcpy(interet,    	rv.interet);
   strcpy(pluriel,    	rv.pluriel);

	return *this;
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison de NSPatOldDatData
//---------------------------------------------------------------------------
int
NSPatOldDatData::operator == (const NSPatOldDatData& o)
{
	if ((strcmp(codePatient,  o.codePatient) 	== 0) &&
       (strcmp(codeDocument, o.codeDocument) == 0) &&
       (strcmp(noeud, 		  o.noeud) 			== 0) &&
		 (strcmp(type,			  o.type) 			== 0) &&
		 (strcmp(lexique,   	  o.lexique) 		== 0) &&
		 (strcmp(complement,   o.complement) 	== 0) &&
       (strcmp(certitude, 	  o.certitude) 	== 0) &&
		 (strcmp(interet,   	  o.interet) 		== 0) &&
       (strcmp(pluriel,   	  o.pluriel) 		== 0)
   	)
		return 1;
	else
		return 0;
}

//---------------------------------------------------------------------------
//  Met à 0 les variables de NSPatOldDatData
//---------------------------------------------------------------------------
void
NSPatOldDatData::metAZero()
{
	//
	// Met les champs de données à zéro
	//
	memset(codePatient,	0, PAT_NSS_LEN + 1);
   memset(codeDocument, 0, DOC_CODE_DOCUM_LEN + 1);
   memset(noeud, 			0, PPD_NOEUD_LEN + 1);
   memset(type, 			0, BASE_TYPE_LEN + 1);
   memset(lexique, 		0, BASE_LEXIQUE_LEN + 1);
   memset(complement, 	0, BASE_COMPLEMENT_LEN + 1);
	memset(certitude, 	0, BASE_CERTITUDE_LEN + 1);
	memset(interet, 		0, BASE_INTERET_LEN + 1);
	memset(pluriel, 		0, BASE_PLURIEL_LEN + 1);
}

//---------------------------------------------------------------------------
//  Met à blanc les variables de NSPatOldDatData
//---------------------------------------------------------------------------
void
NSPatOldDatData::metABlanc()
{
	//
	// Met les champs de données à blanc
	//
   memset(codePatient,	' ', PAT_NSS_LEN);
   memset(codeDocument, ' ', DOC_CODE_DOCUM_LEN);
   memset(noeud, 			' ', PPD_NOEUD_LEN);
   memset(type, 			' ', BASE_TYPE_LEN);
   memset(lexique, 		' ', BASE_LEXIQUE_LEN);
   memset(complement, 	' ', BASE_COMPLEMENT_LEN);
	memset(certitude, 	' ', BASE_CERTITUDE_LEN);
	memset(interet, 		' ', BASE_INTERET_LEN);
	memset(pluriel, 		' ', BASE_PLURIEL_LEN);
}


//---------------------------------------------------------------------------
//  Retourne le type de l'élément : Actif ou Historique
//---------------------------------------------------------------------------
int
NSPatOldDatData::getType()
{
	switch (type[0])
   {
   	case 'A' : return ppdActif;
		case 'H' : return ppdHistorique;
	}
   return ppdIndef;
}

//---------------------------------------------------------------------------
//  Instancie le type de l'élément : Actif ou Historique
//---------------------------------------------------------------------------
void
NSPatOldDatData::setType(int newType)
{
	switch (newType)
   {
   	case ppdActif   	 : strcpy(type, "A"); break;
		case ppdHistorique : strcpy(type, "H"); break;
   }
}

/*************************************************************************/
// NSPatPaDat
/*************************************************************************/

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NSPatPaOldDat::NSPatPaOldDat(NSContexte* pCtx) : NSFiche(pCtx)
{
	pDonnees = new NSPatOldDatData();
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSPatPaOldDat::~NSPatPaOldDat()
{
	delete pDonnees;
}

//---------------------------------------------------------------------------
//  Transfère le contenu de pRecBuff dans les variables de la fiche
//---------------------------------------------------------------------------
void
NSPatPaOldDat::alimenteFiche()
{
	alimenteChamp(pDonnees->codePatient,  PPD_PAT_FIELD,			PAT_NSS_LEN);
   alimenteChamp(pDonnees->codeDocument, PPD_DOCU_FIELD,		 	DOC_CODE_DOCUM_LEN);
   alimenteChamp(pDonnees->noeud, 		  PPD_NOEUD_FIELD,		PPD_NOEUD_LEN);
   alimenteChamp(pDonnees->type,			  PPD_TYPE_FIELD,		 	BASE_TYPE_LEN);
   alimenteChamp(pDonnees->lexique,		  PPD_LEXIQUE_FIELD,	 	BASE_LEXIQUE_LEN);
   alimenteChamp(pDonnees->complement,	  PPD_COMPLEMENT_FIELD, BASE_COMPLEMENT_LEN);
   alimenteChamp(pDonnees->certitude,	  PPD_CERTITUDE_FIELD,	BASE_CERTITUDE_LEN);
   alimenteChamp(pDonnees->interet,		  PPD_INTERET_FIELD,	 	BASE_INTERET_LEN);
   alimenteChamp(pDonnees->pluriel,		  PPD_PLURIEL_FIELD,	 	BASE_PLURIEL_LEN);
}

//---------------------------------------------------------------------------
//  Transfère le contenu des valeurs de la fiche dans pRecBuff
//---------------------------------------------------------------------------
void
NSPatPaOldDat::videFiche()
{
   videChamp(pDonnees->codePatient,  PPD_PAT_FIELD,		  PAT_NSS_LEN);
   videChamp(pDonnees->codeDocument, PPD_DOCU_FIELD,		  DOC_CODE_DOCUM_LEN);
   videChamp(pDonnees->noeud, 		 PPD_NOEUD_FIELD,		  PPD_NOEUD_LEN);
   videChamp(pDonnees->type,			 PPD_TYPE_FIELD,		  BASE_TYPE_LEN);
   videChamp(pDonnees->lexique,		 PPD_LEXIQUE_FIELD,	  BASE_LEXIQUE_LEN);
   videChamp(pDonnees->complement,	 PPD_COMPLEMENT_FIELD, BASE_COMPLEMENT_LEN);
   videChamp(pDonnees->certitude,	 PPD_CERTITUDE_FIELD,  BASE_CERTITUDE_LEN);
   videChamp(pDonnees->interet,		 PPD_INTERET_FIELD,	  BASE_INTERET_LEN);
   videChamp(pDonnees->pluriel,		 PPD_PLURIEL_FIELD,	  BASE_PLURIEL_LEN);
}

//---------------------------------------------------------------------------
//  Ouvre la table
//---------------------------------------------------------------------------
DBIResult
NSPatPaOldDat::open()
{
	char tableName[] = "PATPADAT.DB";
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
NSPatPaOldDat::Create()
{
	return TRUE;
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
bool
NSPatPaOldDat::Modify()
{
	return TRUE;
}

//***************************************************************************
// Implémentation des méthodes NSPatPaOldDatInfo
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur par défaut
//---------------------------------------------------------------------------
NSPatPaOldDatInfo::NSPatPaOldDatInfo()
{
	// Crée l'objet de données
	pDonnees = new NSPatOldDatData();
}


//---------------------------------------------------------------------------
//  Constructeur à partir d'un NSPatPatho
//---------------------------------------------------------------------------
NSPatPaOldDatInfo::NSPatPaOldDatInfo(NSPatPaOldDat* pPatPaDat)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSPatOldDatData();
	//
	// Copie les valeurs du NSDocument
	//
	*pDonnees = *(pPatPaDat->pDonnees);
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSPatPaOldDatInfo::NSPatPaOldDatInfo(NSPatPaOldDatInfo& rv)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSPatOldDatData(*(rv.pDonnees));
	//
	// Copie les valeurs du NSPatPathoInfo d'origine
	//
	*pDonnees = *(rv.pDonnees);
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSPatPaOldDatInfo&
NSPatPaOldDatInfo::operator=(NSPatPaOldDatInfo src)
{
	*pDonnees = *(src.pDonnees);
	return *this;
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSPatPaOldDatInfo::operator == (const NSPatPaOldDatInfo& o)
{
	return (*pDonnees == *(o.pDonnees));
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison avec un NSPatPathoInfo
//---------------------------------------------------------------------------
int
NSPatPaOldDatInfo::operator == (const NSPatPathoInfo& o)
{
	if ((strcmp(pDonnees->lexique,    o.pDonnees->lexique) 	 == 0) &&
		 (strcmp(pDonnees->complement, o.pDonnees->complement) == 0) &&
       (strcmp(pDonnees->certitude,  o.pDonnees->certitude)  == 0) &&
		 (strcmp(pDonnees->interet,    o.pDonnees->interet) 	 == 0) &&
       (strcmp(pDonnees->pluriel,    o.pDonnees->pluriel) 	 == 0))
		return 1;
	else
		return 0;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSPatPaOldDatInfo::~NSPatPaOldDatInfo()
{
  	delete pDonnees;
}

// -------------------------------------------------------------------------
// -------------------- METHODES DE NSPatOldLocArray ------------------------
// -------------------------------------------------------------------------
NSPatOldLocArray::NSPatOldLocArray(NSContexte* pCtx)
					 :NSPatOldLocInfoArray(), NSRoot(pCtx)
{}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSPatOldLocArray::NSPatOldLocArray(NSPatOldLocArray& rv)
                 :NSPatOldLocInfoArray(), NSRoot(rv.pContexte)
{
	for (PatPaOldLocIter i = rv.begin(); i != rv.end(); i++)
        push_back(new NSPatOldLocInfo(*(*i)));
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
void
NSPatOldLocArray::vider()
{
   if (empty())
   	return;

	for (PatPaOldLocIter i = begin(); i != end(); )
   {
   	delete *i;
      erase(i);
   }
}

NSPatOldLocArray::~NSPatOldLocArray()
{
	vider();
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSPatOldLocArray&
NSPatOldLocArray::operator=(NSPatOldLocArray src)
{
   PatPaOldLocIter i;
   //
   // Effacement des éléments déjà contenus dans le vecteur destination
   //
	vider();
	//
   // Copie et insertion des éléments de la source
   //
  	for (i = src.begin(); i != src.end(); i++)
   	push_back(new NSPatOldLocInfo(*(*i)));
	return *this;
}

//---------------------------------------------------------------------------
//  Ajout d'un élément à l'array
//---------------------------------------------------------------------------
bool
NSPatOldLocArray::ajouteElement(NSPatOldLocInfo* pPatPatho)
{
	push_back(pPatPatho);
   return true;
}

//***************************************************************************
// Implémentation des mèthodes NSPatOldLoc
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur copie de NSPatOldLocData
//---------------------------------------------------------------------------
NSPatOldLocData::NSPatOldLocData(NSPatOldLocData& rv)
{
	strcpy(codePatient, 		 rv.codePatient);
   strcpy(codeDocument, 	 rv.codeDocument);
   strcpy(transaction, 		 rv.transaction);
   strcpy(noeud, 				 rv.noeud);
   strcpy(codeLocalisation, rv.codeLocalisation);
   strcpy(visible, 			 rv.visible);
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation de NSPatOldLocData
//---------------------------------------------------------------------------
NSPatOldLocData&
NSPatOldLocData::operator=(NSPatOldLocData rv)
{
	strcpy(codePatient, 		 rv.codePatient);
   strcpy(codeDocument, 	 rv.codeDocument);
   strcpy(transaction, 		 rv.transaction);
   strcpy(noeud, 				 rv.noeud);
   strcpy(codeLocalisation, rv.codeLocalisation);
   strcpy(visible, 			 rv.visible);

	return *this;
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison de NSPatOldLocData
//---------------------------------------------------------------------------
int
NSPatOldLocData::operator == (const NSPatOldLocData& o)
{
	if ((strcmp(codePatient,  		o.codePatient) 	  == 0) &&
       (strcmp(codeDocument, 		o.codeDocument) 	  == 0) &&
       (strcmp(transaction,  		o.transaction) 	  == 0) &&
       (strcmp(noeud, 		  		o.noeud) 			  == 0) &&
		 (strcmp(codeLocalisation,	o.codeLocalisation) == 0) &&
       (strcmp(visible,   	  		o.visible) 			  == 0)
   	)
		return 1;
	else
		return 0;
}

//---------------------------------------------------------------------------
//  Met à 0 les variables de NSPatOldLocData
//---------------------------------------------------------------------------
void
NSPatOldLocData::metAZero()
{
	//
	// Met les champs de données à zéro
	//
	memset(codePatient,		 0, PAT_NSS_LEN + 1);
   memset(codeDocument, 	 0, DOC_CODE_DOCUM_LEN + 1);
   memset(transaction, 		 0, TRAN_CODE_LEN + 1);
   memset(noeud, 				 0, PPD_NOEUD_LEN + 1);
   memset(codeLocalisation, 0, BASE_LOCALISATION_LEN + 1);
	memset(visible, 			 0, BASE_VISIBLE_LEN + 1);
}

//---------------------------------------------------------------------------
//  Met à blanc les variables de NSPatOldLocData
//---------------------------------------------------------------------------
void
NSPatOldLocData::metABlanc()
{
	//
	// Met les champs de données à blanc
	//
   memset(codePatient,		 ' ', PAT_NSS_LEN);
   memset(codeDocument, 	 ' ', DOC_CODE_DOCUM_LEN);
   memset(transaction, 		 ' ', TRAN_CODE_LEN);
   memset(noeud, 				 ' ', PPD_NOEUD_LEN);
   memset(codeLocalisation, ' ', BASE_LOCALISATION_LEN);
	memset(visible, 			 ' ', BASE_VISIBLE_LEN);
}

/*************************************************************************/
// NSPatOldLoc
/*************************************************************************/

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NSPatOldLoc::NSPatOldLoc(NSContexte* pCtx)
			  :NSFiche(pCtx)
{
	pDonnees = new NSPatOldLocData();
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSPatOldLoc::~NSPatOldLoc()
{
	delete pDonnees;
}

//---------------------------------------------------------------------------
//  Transfère le contenu de pRecBuff dans les variables de la fiche
//---------------------------------------------------------------------------
void
NSPatOldLoc::alimenteFiche()
{
	alimenteChamp(pDonnees->codePatient,  		PPL_PAT_FIELD,		 PAT_NSS_LEN);
   alimenteChamp(pDonnees->codeDocument, 		PPL_DOCU_FIELD,	 DOC_CODE_DOCUM_LEN);
   alimenteChamp(pDonnees->transaction,  		PPL_TRAN_FIELD,	 TRAN_CODE_LEN);
   alimenteChamp(pDonnees->noeud, 		  		PPL_NOEUD_FIELD,	 PPD_NOEUD_LEN);
   alimenteChamp(pDonnees->codeLocalisation,	PPL_LOC_FIELD,		 BASE_LOCALISATION_LEN);
   alimenteChamp(pDonnees->visible,		  		PPL_VISIBLE_FIELD, BASE_VISIBLE_LEN);
}

//---------------------------------------------------------------------------
//  Transfère le contenu des valeurs de la fiche dans pRecBuff
//---------------------------------------------------------------------------
void
NSPatOldLoc::videFiche()
{
   videChamp(pDonnees->codePatient,  	  PPL_PAT_FIELD,		PAT_NSS_LEN);
   videChamp(pDonnees->codeDocument, 	  PPL_DOCU_FIELD,		DOC_CODE_DOCUM_LEN);
   videChamp(pDonnees->transaction,  	  PPL_TRAN_FIELD,	 	TRAN_CODE_LEN);
   videChamp(pDonnees->noeud, 		  	  PPL_NOEUD_FIELD,	PPD_NOEUD_LEN);
   videChamp(pDonnees->codeLocalisation, PPL_LOC_FIELD,		BASE_LOCALISATION_LEN);
   videChamp(pDonnees->visible,		  	  PPL_VISIBLE_FIELD, BASE_VISIBLE_LEN);
}

//---------------------------------------------------------------------------
//  Ouvre la table
//---------------------------------------------------------------------------
DBIResult
NSPatOldLoc::open()
{
	char tableName[] = "PATPALOC.DB";
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
NSPatOldLoc::Create()
{
	return TRUE;
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
bool
NSPatOldLoc::Modify()
{
	return TRUE;
}

//***************************************************************************
// Implémentation des méthodes NSPatOldLocInfo
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur par défaut
//---------------------------------------------------------------------------
NSPatOldLocInfo::NSPatOldLocInfo()
{
	// Crée l'objet de données
	pDonnees = new NSPatOldLocData();
}


//---------------------------------------------------------------------------
//  Constructeur à partir d'un NSPatOldLoc
//---------------------------------------------------------------------------
NSPatOldLocInfo::NSPatOldLocInfo(NSPatOldLoc* pPatPaLoc)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSPatOldLocData();
	//
	// Copie les valeurs du NSDocument
	//
	*pDonnees = *(pPatPaLoc->pDonnees);
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSPatOldLocInfo::NSPatOldLocInfo(NSPatOldLocInfo& rv)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSPatOldLocData(*(rv.pDonnees));
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSPatOldLocInfo&
NSPatOldLocInfo::operator=(NSPatOldLocInfo src)
{
	*pDonnees = *(src.pDonnees);
	return *this;
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSPatOldLocInfo::operator == (const NSPatOldLocInfo& o)
{
	return (*pDonnees == *(o.pDonnees));
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSPatOldLocInfo::~NSPatOldLocInfo()
{
  	delete pDonnees;
}
