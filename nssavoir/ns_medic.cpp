//---------------------------------------------------------------------------//    	NS_MEDIC.CPP
//		Implémentation des protocoles médicamenteux
//---------------------------------------------------------------------------
#include <utility.h>
#include <mem.h>
#include <string.h>
#include <cstring.h>

//using namespace std;

#include "nautilus\nssuper.h"
#include "nautilus\nsrechdl.h"
#include "partage\nsdivfct.h"

#include "nssavoir\ns_medic.h"

#include "nsbb\tagNames.h"

//***************************************************************************
// Implémentation des méthodes NSPrescription
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NSPrescriptionData::NSPrescriptionData()
{
	metAZero();
}

//---------------------------------------------------------------------------
//  Fonction : NSPrescriptionData::metAZero()
//  Met les champs de données à zéro
//---------------------------------------------------------------------------
void
NSPrescriptionData::metAZero()
{
	memset(code, 0, PRESC_CODE_LEN + 1) ;
  NSPathoBaseData::metAZero() ;
}

//---------------------------------------------------------------------------//  Constructeur copie
//---------------------------------------------------------------------------
NSPrescriptionData::NSPrescriptionData(NSPrescriptionData& rv)
                   :NSPathoBaseData(rv)
{
	strcpy(code, rv.code) ;
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
NSPrescriptionData::~NSPrescriptionData()
{
}

//---------------------------------------------------------------------------
//  Opérateur =
//---------------------------------------------------------------------------
NSPrescriptionData&
NSPrescriptionData::operator=(NSPrescriptionData src)
{
  if (&src == this)
		return *this ;

  NSPathoBaseData* pbdata1 = this ;
	NSPathoBaseData* pbdata2 = &src ;

	*pbdata1 = *pbdata2 ;

	strcpy(code, src.code) ;

	return *this ;
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSPrescriptionData::operator == (const NSPrescriptionData& o)
{
	if ((strcmp(code, o.code) == 0) &&
		  (_sCodeLocalisation == o._sCodeLocalisation))
		return 1 ;
	else
		return 0 ;
}

#ifndef N_TIERS

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NSPrescription::NSPrescription(NSContexte* pCtx)
			  		:NSFiche(pCtx)
{
	//
	// Crée un objet de données
	//
	pDonnees = new NSPrescriptionData();

}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSPrescription::NSPrescription(NSPrescription& rv)
			  		:NSFiche(rv.pContexte)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSPrescriptionData();
	//
	// Copie les valeurs du NSSavFicheInfo d'origine
	//
	*pDonnees = *(rv.pDonnees);
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSPrescription::~NSPrescription()
{
	delete pDonnees;
}

//---------------------------------------------------------------------------
//  Fonction : NSPrescription::alimenteFiche()
//  Transfère le contenu de pRecBuff dans les variables de la fiche
//---------------------------------------------------------------------------
void
NSPrescription::alimenteFiche()
{
   alimenteChamp(pDonnees->code, 		  		PRESC_CODE_FIELD, 		  PRESC_CODE_LEN);
	alimenteChamp(pDonnees->codeLocalisation, PRESC_LOCALISATION_FIELD, BASE_LOCALISATION_LEN);
	alimenteChamp(pDonnees->type, 		  		PRESC_TYPE_FIELD, 		  BASE_TYPE_LEN);
	alimenteChamp(pDonnees->lexique, 	  		PRESC_LEXIQUE_FIELD, 	  BASE_LEXIQUE_LEN);
   alimenteChamp(pDonnees->complement,   		PRESC_COMPLEMENT_FIELD,   BASE_COMPLEMENT_LEN);
	alimenteChamp(pDonnees->certitude, 	  		PRESC_CERTITUDE_FIELD, 	  BASE_CERTITUDE_LEN);
	alimenteChamp(pDonnees->interet, 	  		PRESC_INTERET_FIELD, 	  BASE_INTERET_LEN);
	alimenteChamp(pDonnees->pluriel, 	  		PRESC_PLURIEL_FIELD, 	  BASE_PLURIEL_LEN);
}

//---------------------------------------------------------------------------
//  Fonction : NSPrescription::videFiche()
//
//  Transfère le contenu des valeurs de la fiche dans pRecBuff
//---------------------------------------------------------------------------
void
NSPrescription::videFiche()
{
	videChamp(pDonnees->code, 		  		  PRESC_CODE_FIELD, 		  	 PRESC_CODE_LEN);
	videChamp(pDonnees->codeLocalisation, PRESC_LOCALISATION_FIELD, BASE_LOCALISATION_LEN);
	videChamp(pDonnees->type, 		  		  PRESC_TYPE_FIELD, 		  	 BASE_TYPE_LEN);
	videChamp(pDonnees->lexique, 	  		  PRESC_LEXIQUE_FIELD, 	  	 BASE_LEXIQUE_LEN);
   videChamp(pDonnees->complement,   	  PRESC_COMPLEMENT_FIELD,   BASE_COMPLEMENT_LEN);
	videChamp(pDonnees->certitude, 	  	  PRESC_CERTITUDE_FIELD, 	 BASE_CERTITUDE_LEN);
	videChamp(pDonnees->interet, 	  		  PRESC_INTERET_FIELD, 	  	 BASE_INTERET_LEN);
	videChamp(pDonnees->pluriel, 	  		  PRESC_PLURIEL_FIELD, 	  	 BASE_PLURIEL_LEN);
}

//---------------------------------------------------------------------------
//  Fonction :  	NSPrescription::getRecord()
//
//  Prend l'enregistrement en cours et assigne la valeur des
//  champs aux variables membres de la classe.
//
//  Returns:   	PXERR_, s'il y a lieu
//---------------------------------------------------------------------------
DBIResult
NSPrescription::getPatRecord()
{
	//
	// La table est-elle ouverte ?
	//
	if (!isOpen)
	  return(lastError = ERROR_TABLENOTOPEN);
	//
	// Appel de la classe de base pour récupérer l'enregistrement.
	//
	lastError = getDbiRecord(dbiWRITELOCK);

	return(lastError);
}

//---------------------------------------------------------------------------
//  Fonction : NSPrescription::open()
//
//  Ouvre la table primaire et les index secondaires
//
//  Retour :	PXERR_, s'il y a lieu
//---------------------------------------------------------------------------
DBIResult
NSPrescription::open()
{
	char tableName[] = "PRESCRIP.DB";
	//
	// Appelle la fonction open() de la classe de base pour ouvrir
	// l'index primaire
	//
	lastError = NSFiche::open(tableName, NSF_PARTAGE_GLOBAL);
	return(lastError);
}

//---------------------------------------------------------------------------
//  Function:  NSPrescription::Create()
//---------------------------------------------------------------------------
bool
NSPrescription::Create()
{
	return true;
}

//---------------------------------------------------------------------------
//  Function:  NSPrescription::Modify()
//
//---------------------------------------------------------------------------
bool
NSPrescription::Modify()
{
	return true;
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSPrescription&
NSPrescription::operator=(NSPrescription src)
{
	*pDonnees = *(src.pDonnees);
	return *this;
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSPrescription::operator == ( const NSPrescription& o )
{
	 return (*pDonnees == *(o.pDonnees));
}
#endif
//---------------------------------------------------------------------------
//  Constructeur par défaut
//---------------------------------------------------------------------------
NSPrescriInfo::NSPrescriInfo()
{
	// Crée l'objet de données
	pDonnees = new NSPrescriptionData();
}

//---------------------------------------------------------------------------
//  Constructeur à partir d'un NSPrescription
//---------------------------------------------------------------------------
#ifndef N_TIERS
NSPrescriInfo::NSPrescriInfo(NSPrescription* pPatho){
	//
	// Crée l'objet de données
	//
	pDonnees = new NSPrescriptionData();
	//
	// Copie les valeurs du NSDocument
	//
	*pDonnees = *(pPatho->pDonnees);
}
#endif

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSPrescriInfo::NSPrescriInfo(NSPrescriInfo& rv)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSPrescriptionData();
	//
	// Copie les valeurs du NSSavFicheInfo d'origine
	//
	*pDonnees = *(rv.pDonnees);
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSPrescriInfo::~NSPrescriInfo()
{
	delete pDonnees;
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSPrescriInfo&
NSPrescriInfo::operator=(NSPrescriInfo src)
{
	*pDonnees = *(src.pDonnees);
	return *this;
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSPrescriInfo::operator == ( const NSPrescriInfo& o )
{
	 return (*pDonnees == *(o.pDonnees));
}
//***************************************************************************// Implémentation des méthodes NSProtocole
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NSProtocoleData::NSProtocoleData()
{
	metAZero() ;
}

//---------------------------------------------------------------------------
//  Fonction : metAZero()
//  Met les champs de données à zéro
//---------------------------------------------------------------------------
void
NSProtocoleData::metAZero()
{
	memset(type, 0, BASE_LEXI_LEN) ;

  sObjectID = string("") ;
  sLibelle  = string("") ;

	// memset(utilisateur,	0, PROTO_UTILISATEUR_LEN + 1);
	// memset(code, 	   	0, PROTO_CODE_LEN + 1);
	// memset(libelle,    	0, PROTO_LIBELLE_LEN + 1);
	// memset(scenario,   	0, PROTO_SCENARIO_LEN + 1);
}

//---------------------------------------------------------------------------//  Constructeur copie
//---------------------------------------------------------------------------
NSProtocoleData::NSProtocoleData(NSProtocoleData& rv)
{
	strcpy(type, rv.type) ;

  sObjectID = rv.sObjectID ;
  sLibelle  = rv.sLibelle ;

	// strcpy(utilisateur,	rv.utilisateur);
	// strcpy(code, 	 	rv.code);
	// strcpy(libelle,   	rv.libelle);
	// strcpy(scenario,   	rv.scenario);
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
NSProtocoleData::~NSProtocoleData()
{
}

//---------------------------------------------------------------------------
//  Opérateur =
//---------------------------------------------------------------------------
NSProtocoleData&
NSProtocoleData::operator=(NSProtocoleData src)
{
	if (this == &src)
		return *this ;

	strcpy(type, src.type) ;

	sObjectID = src.sObjectID ;
  sLibelle  = src.sLibelle ;

	// strcpy(utilisateur,	src.utilisateur);
	// strcpy(code, 	 	src.code);
	// strcpy(libelle,   	src.libelle);
	// strcpy(scenario,    src.scenario);

	return *this ;
}

//---------------------------------------------------------------------------//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSProtocoleData::operator == (const NSProtocoleData& o)
{
	if ((strcmp(type, o.type) == 0) &&
   	 	(sObjectID == o.sObjectID)  &&
	    (sLibelle  == o.sLibelle))
		return 1 ;
	else
		return 0 ;
}

//---------------------------------------------------------------------------
//  Constructeur par défaut
//---------------------------------------------------------------------------
NSProtocoleInfo::NSProtocoleInfo()
{
	// Crée l'objet de données
	pDonnees       = new NSProtocoleData() ;
  pPatPathoArray = 0 ;
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSProtocoleInfo::NSProtocoleInfo(NSProtocoleInfo& rv)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSProtocoleData();
	//
	// Copie les valeurs du NSSavFicheInfo d'origine
	//
	*pDonnees = *(rv.pDonnees);

  if (rv.pPatPathoArray)
  	pPatPathoArray = new NSPatPathoArray(*(rv.pPatPathoArray)) ;
	else
  	pPatPathoArray = 0 ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSProtocoleInfo::~NSProtocoleInfo()
{
	delete pDonnees;
}

bool
NSProtocoleInfo::initialiseDepuisObjet(NSContexte* pCtx, string sObjId)
{
	pDonnees->sObjectID = string("") ;

	// ------------ Appel du pilote
  //
  NSDataGraph Graph(pCtx->getSuperviseur(), graphObject) ;
  NSBasicAttributeArray AttrArray ;
	AttrArray.push_back(new NSBasicAttribute(OBJECT, sObjId)) ;
  bool res = pCtx->getSuperviseur()->getPilot()->invokeService(NautilusPilot::SERV_SEARCH_OBJECT_FROM_ID.c_str(),
                                  &Graph,  &AttrArray) ;
  if (false == res)
  {
		string sErrorText = pCtx->getSuperviseur()->getText("NTIERS", "materialNotFound") ;
    erreur(sErrorText.c_str(), standardError, 0) ;
    return false ;
  }

  if (Graph.getTrees()->empty())
		return false ;

	if ((NSPatPathoArray*) NULL == pPatPathoArray)
		pPatPathoArray = new NSPatPathoArray(pCtx->getSuperviseur(), graphObject) ;
  else
  	pPatPathoArray->vider() ;

	NSPatPathoArray globalPpt(pCtx->getSuperviseur(), graphObject) ;
	NSDataTreeIter iterTree = Graph.getTrees()->begin() ;
	(*iterTree)->getRawPatPatho(&globalPpt) ;

  pDonnees->sObjectID = sObjId ;

  if (globalPpt.empty())
		return true ;

  int iColBase ;
  string sTemp ;
  string sNumSerie, sLibelle ;

  PatPathoIter iter = globalPpt.begin() ;

  string sElemLex = (*iter)->getLexiqueSens() ;
  strcpy(pDonnees->type, sElemLex.c_str()) ;

  iColBase = (*iter)->getColonne() ;
  iter++ ;
  while ((globalPpt.end() != iter) && ((*iter)->getColonne() > iColBase))
  {
    string sSens = (*iter)->getLexiqueSens() ;

    // libelle
    if (string("LNOMA") == sSens)
      pDonnees->sLibelle = globalPpt.getFreeTextValue(&iter) ;

    // date de mise en service
    else if (string("0PROT") == sSens)
    {
    	int iColProt = (*iter)->getColonne() ;

    	globalPpt.ExtrairePatPatho(iter, pPatPathoArray) ;

    	iter++ ;

      while ((globalPpt.end() != iter) && ((*iter)->getColonne() > iColProt))
      	iter++ ;
    }
    else
    	iter++ ;
  }

  return true ;
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSProtocoleInfo&
NSProtocoleInfo::operator=(NSProtocoleInfo src)
{
	if (this == &src)
		return *this ;

	*pDonnees = *(src.pDonnees) ;

  if (src.pPatPathoArray)
	{
  	if (pPatPathoArray)
    	*pPatPathoArray = *(src.pPatPathoArray) ;
    else
    	pPatPathoArray = new NSPatPathoArray(*(src.pPatPathoArray)) ;
  }
  else
  {
  	if (pPatPathoArray)
    	delete pPatPathoArray ;
    pPatPathoArray = 0 ;
	}

	return *this ;
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSProtocoleInfo::operator == ( const NSProtocoleInfo& o )
{
	 return (*pDonnees == *(o.pDonnees));
}

