//---------------------------------------------------------------------------//    NSPERSON.CPP
//    KRZISCH PH.   janvier 92
//  Implémentation de objets NAUTILUS
//---------------------------------------------------------------------------
#include <utility.h>
#include <mem.h>
#include <string.h>
#include <cstring.h>

//using namespace std;

#include "nautilus\nssuper.h"
#include "partage\nsdivfct.h"
#include "nscompta\nsFSE16.h"
#include "nscompta\nsfseuti.h"

#include "nscompta\nsCptaRef.rh"
#include "partage\nsCIM10.rh"

//***************************************************************************
// Implémentation des méthodes NSFse1610
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NSFse1610Data::NSFse1610Data()
{
	//
	// Met les champs de données à zéro
	//
	metAZero();
}

//---------------------------------------------------------------------------//  Constructeur copie
//---------------------------------------------------------------------------
NSFse1610Data::NSFse1610Data(const NSFse1610Data& rv)
{
	strcpy(numcompt, 		rv.numcompt);
	strcpy(numprest,  	rv.numprest);
	strcpy(date, 	 		  rv.date);
	strcpy(lieu,   			rv.lieu);
	strcpy(code,  			rv.code);
	strcpy(complmnt, 		rv.complmnt);
  strcpy(montant_f,   rv.montant_f);
  strcpy(montant_e,   rv.montant_e);
  strcpy(qualif,   		rv.qualif);
  strcpy(coeff,   		rv.coeff);
  strcpy(divis,			  rv.divis);
  strcpy(quantite,   	rv.quantite);
  strcpy(denombr,   	rv.denombr);
  strcpy(prix_unit_f, rv.prix_unit_f);
  strcpy(prix_unit_e, rv.prix_unit_e);
  strcpy(base_rmb,   	rv.base_rmb);
  strcpy(taux,   			rv.taux);
  strcpy(rmb_amo,   	rv.rmb_amo);
  strcpy(rmb_amc,   	rv.rmb_amc);
  strcpy(rmo,   			rv.rmo);
  strcpy(code_exo,   	rv.code_exo);
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
NSFse1610Data::~NSFse1610Data()
{
}

//---------------------------------------------------------------------------
//  Opérateur =
//---------------------------------------------------------------------------
NSFse1610Data&
NSFse1610Data::operator=(const NSFse1610Data& src)
{
  if (this == &src)
  	return *this ;

	strcpy(numcompt, 		src.numcompt);
	strcpy(numprest,  	src.numprest);
	strcpy(date, 	 		  src.date);
	strcpy(lieu,   			src.lieu);
	strcpy(code,  			src.code);
	strcpy(complmnt, 		src.complmnt);
  strcpy(montant_f,   src.montant_f);
  strcpy(montant_e,   src.montant_e);
  strcpy(qualif,   		src.qualif);
  strcpy(coeff,   		src.coeff);
  strcpy(divis,			  src.divis);
  strcpy(quantite,   	src.quantite);
  strcpy(denombr,   	src.denombr);
  strcpy(prix_unit_f, src.prix_unit_f);
  strcpy(prix_unit_e, src.prix_unit_e);
  strcpy(base_rmb,   	src.base_rmb);
  strcpy(taux,   			src.taux);
  strcpy(rmb_amo,   	src.rmb_amo);
  strcpy(rmb_amc,   	src.rmb_amc);
  strcpy(rmo,   			src.rmo);
  strcpy(code_exo,   	src.code_exo);

	return *this ;
}


//---------------------------------------------------------------------------//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSFse1610Data::operator == ( const NSFse1610Data& o )
{
	if ((strcmp(numcompt, o.numcompt) == 0))
		return 1;
	else
		return 0;
}

//---------------------------------------------------------------------------
//  Met à zéro les variables de la fiche
//---------------------------------------------------------------------------
void
NSFse1610Data::metAZero()
{
	//
	// Met les champs de données à zéro
	//
	memset(numcompt, 	  	0, FSE_NUMCOMPT_LEN + 1);
	memset(numprest,   		0, FSE_NUMPREST_LEN + 1);
	memset(date, 	 	  	  0, FSE1610_DATE_LEN + 1);
	memset(lieu,   			  0, FSE1610_LIEU_LEN + 1);
	memset(code,  			  0, FSE1610_CODE_LEN + 1);
	memset(complmnt, 		  0, FSE1610_COMPLMNT_LEN + 1);
  memset(montant_f, 		0, FSE1610_MONTANT_F_LEN + 1);
  memset(montant_e, 		0, FSE1610_MONTANT_E_LEN + 1);
  memset(qualif, 	  		0, FSE1610_QUALIF_LEN + 1);
  memset(coeff, 	  		0, FSE1610_COEFF_LEN + 1);
  memset(divis,			    0, FSE1610_DIVIS_LEN + 1);
  memset(quantite, 	  	0, FSE1610_QUANTITE_LEN + 1);
  memset(denombr, 	  	0, FSE1610_DENOMBR_LEN + 1);
  memset(prix_unit_f,		0, FSE1610_PRIX_UNIT_F_LEN + 1);
  memset(prix_unit_e, 	0, FSE1610_PRIX_UNIT_E_LEN + 1);
  memset(base_rmb, 	  	0, FSE1610_BASE_RMB_LEN + 1);
  memset(taux, 	  		  0, FSE1610_TAUX_LEN + 1);
  memset(rmb_amo, 	  	0, FSE1610_RMB_AMO_LEN + 1);
  memset(rmb_amc, 	  	0, FSE1610_RMB_AMC_LEN + 1);
  memset(rmo, 	  		  0, FSE1610_RMO_LEN + 1);
  memset(code_exo, 	  	0, FSE1610_CODE_EXO_LEN + 1);
}

//---------------------------------------------------------------------------//  Constructeur
//---------------------------------------------------------------------------
NSFse1610::NSFse1610(NSSuper* pSuper)
          :NSFiche(pSuper)
{
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSFse1610::NSFse1610(const NSFse1610& rv)
          :NSFiche(rv._pSuper)
{
  _Donnees = rv._Donnees ;
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
NSFse1610::~NSFse1610()
{
}

//---------------------------------------------------------------------------//  Fonction :  	NSFse1610::alimenteFiche()
//
//  Description : Transfère le contenu de pRecBuff dans les variables de
//             	la fiche
//---------------------------------------------------------------------------
void
NSFse1610::alimenteFiche()
{
  alimenteChamp(_Donnees.numcompt,    FSE1610_NUMCOMPT_FIELD,  	 FSE_NUMCOMPT_LEN);
  alimenteChamp(_Donnees.numprest,    FSE1610_NUMPREST_FIELD, 	 FSE_NUMPREST_LEN);
  alimenteChamp(_Donnees.date,        FSE1610_DATE_FIELD, 	  	 FSE1610_DATE_LEN);
  alimenteChamp(_Donnees.lieu,        FSE1610_LIEU_FIELD,			   FSE1610_LIEU_LEN);
  alimenteChamp(_Donnees.code,        FSE1610_CODE_FIELD, 		   FSE1610_CODE_LEN);
  alimenteChamp(_Donnees.complmnt,    FSE1610_COMPLMNT_FIELD, 	 FSE1610_COMPLMNT_LEN);
  alimenteChamp(_Donnees.montant_f,   FSE1610_MONTANT_F_FIELD,   FSE1610_MONTANT_F_LEN);
  alimenteChamp(_Donnees.montant_e,   FSE1610_MONTANT_E_FIELD,   FSE1610_MONTANT_E_LEN);
  alimenteChamp(_Donnees.qualif,      FSE1610_QUALIF_FIELD,  		 FSE1610_QUALIF_LEN);
  alimenteChamp(_Donnees.coeff,       FSE1610_COEFF_FIELD,  		 FSE1610_COEFF_LEN);
  alimenteChamp(_Donnees.divis,       FSE1610_DIVIS_FIELD,		   FSE1610_DIVIS_LEN);
  alimenteChamp(_Donnees.quantite,    FSE1610_QUANTITE_FIELD,  	 FSE1610_QUANTITE_LEN);
  alimenteChamp(_Donnees.denombr,     FSE1610_DENOMBR_FIELD,  	 FSE1610_DENOMBR_LEN);
  alimenteChamp(_Donnees.prix_unit_f, FSE1610_PRIX_UNIT_F_FIELD, FSE1610_PRIX_UNIT_F_LEN);
  alimenteChamp(_Donnees.prix_unit_e, FSE1610_PRIX_UNIT_E_FIELD, FSE1610_PRIX_UNIT_E_LEN);
  alimenteChamp(_Donnees.base_rmb,    FSE1610_BASE_RMB_FIELD,  	 FSE1610_BASE_RMB_LEN);
  alimenteChamp(_Donnees.taux,        FSE1610_TAUX_FIELD,  		   FSE1610_TAUX_LEN);
  alimenteChamp(_Donnees.rmb_amo,     FSE1610_RMB_AMO_FIELD,  	 FSE1610_RMB_AMO_LEN);
  alimenteChamp(_Donnees.rmb_amc,     FSE1610_RMB_AMC_FIELD,  	 FSE1610_RMB_AMC_LEN);
  alimenteChamp(_Donnees.rmo,         FSE1610_RMO_FIELD,  		   FSE1610_RMO_LEN);
  alimenteChamp(_Donnees.code_exo,    FSE1610_CODE_EXO_FIELD,  	 FSE1610_CODE_EXO_LEN);
}

//---------------------------------------------------------------------------//  Fonction :   	NSFse1610::videFiche()
//
//  Description : Transfère le contenu des valeurs de la fiche dans pRecBuff
//---------------------------------------------------------------------------
void
NSFse1610::videFiche()
{
  videChamp(_Donnees.numcompt, 	 FSE1610_NUMCOMPT_FIELD,  	FSE_NUMCOMPT_LEN);
  videChamp(_Donnees.numprest, 	 FSE1610_NUMPREST_FIELD, 	  FSE_NUMPREST_LEN);
  videChamp(_Donnees.date, 	 		 FSE1610_DATE_FIELD, 	  	  FSE1610_DATE_LEN);
  videChamp(_Donnees.lieu, 	 		 FSE1610_LIEU_FIELD,			  FSE1610_LIEU_LEN);
  videChamp(_Donnees.code,  			 FSE1610_CODE_FIELD, 		    FSE1610_CODE_LEN);
  videChamp(_Donnees.complmnt, 	 FSE1610_COMPLMNT_FIELD, 	  FSE1610_COMPLMNT_LEN);
  videChamp(_Donnees.montant_f, 	 FSE1610_MONTANT_F_FIELD,  	FSE1610_MONTANT_F_LEN);
  videChamp(_Donnees.montant_e, 	 FSE1610_MONTANT_E_FIELD,  	FSE1610_MONTANT_E_LEN);
  videChamp(_Donnees.qualif, 	   FSE1610_QUALIF_FIELD,  		FSE1610_QUALIF_LEN);
  videChamp(_Donnees.coeff, 	  	 FSE1610_COEFF_FIELD,  		  FSE1610_COEFF_LEN);
  videChamp(_Donnees.divis,			 FSE1610_DIVIS_FIELD,		    FSE1610_DIVIS_LEN);
  videChamp(_Donnees.quantite, 	 FSE1610_QUANTITE_FIELD,  	FSE1610_QUANTITE_LEN);
  videChamp(_Donnees.denombr, 		 FSE1610_DENOMBR_FIELD,  	  FSE1610_DENOMBR_LEN);
  videChamp(_Donnees.prix_unit_f, FSE1610_PRIX_UNIT_F_FIELD, FSE1610_PRIX_UNIT_F_LEN);
  videChamp(_Donnees.prix_unit_e, FSE1610_PRIX_UNIT_E_FIELD, FSE1610_PRIX_UNIT_E_LEN);
  videChamp(_Donnees.base_rmb, 	 FSE1610_BASE_RMB_FIELD,  	FSE1610_BASE_RMB_LEN);
  videChamp(_Donnees.taux, 	  	 FSE1610_TAUX_FIELD,  		  FSE1610_TAUX_LEN);
  videChamp(_Donnees.rmb_amo, 	   FSE1610_RMB_AMO_FIELD,  	  FSE1610_RMB_AMO_LEN);
  videChamp(_Donnees.rmb_amc, 	   FSE1610_RMB_AMC_FIELD,  	  FSE1610_RMB_AMC_LEN);
  videChamp(_Donnees.rmo, 	  	 	 FSE1610_RMO_FIELD,  		    FSE1610_RMO_LEN);
  videChamp(_Donnees.code_exo, 	 FSE1610_CODE_EXO_FIELD,  	FSE1610_CODE_EXO_LEN);
}

//---------------------------------------------------------------------------//  Fonction :  	NSFse1610::open()
//
//  Description :	Ouvre la table primaire et les index secondaires
//
//  Returns :		PXERR_, s'il y a lieu
//---------------------------------------------------------------------------
DBIResult
NSFse1610::open()
{
	char tableName[] = "FSE1610.DB" ;
	//
	// Appelle la fonction open() de la classe de base pour ouvrir
	// l'index primaire
	//
	lastError = NSFiche::open(tableName, NSF_COMPTA) ;
	return(lastError) ;
}

//---------------------------------------------------------------------------
//  Function:  NSFse1610::Create()
//---------------------------------------------------------------------------
bool
NSFse1610::Create()
{
	return true ;
}

//---------------------------------------------------------------------------
//  Function:  NSFse1610::Modify()
//---------------------------------------------------------------------------
bool
NSFse1610::Modify()
{
	return true ;
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSFse1610&
NSFse1610::operator=(const NSFse1610& src)
{
  if (this == &src)
  	return *this ;

  _Donnees = src._Donnees ;

	return *this ;}

//---------------------------------------------------------------------------//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSFse1610::operator == (const NSFse1610& o)
{
  return (_Donnees == o._Donnees) ;
}

//---------------------------------------------------------------------------//  Fonction:		NSFse1610Info::NSFse1610Info()
//
//  Description:	Constructeur par défaut
//---------------------------------------------------------------------------
NSFse1610Info::NSFse1610Info()
{
}

//---------------------------------------------------------------------------//  Fonction:		NSFse1610Info::NSFse1610Info(NSFse1610*)
//
//  Description:	Constructeur à partir d'un NSFse1610
//---------------------------------------------------------------------------
NSFse1610Info::NSFse1610Info(const NSFse1610* pFse1610)
{
  if (pFse1610)
	  _Donnees = pFse1610->_Donnees ;
}

//---------------------------------------------------------------------------//  Constructeur copie
//---------------------------------------------------------------------------
NSFse1610Info::NSFse1610Info(const NSFse1610Info& rv)
{
  _Donnees = rv._Donnees ;
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
NSFse1610Info::~NSFse1610Info()
{
}

//---------------------------------------------------------------------------//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSFse1610Info&
NSFse1610Info::operator=(const NSFse1610Info& src)
{
  if (this == &src)
  	return *this ;

  _Donnees = src._Donnees ;
	return *this ;}

//---------------------------------------------------------------------------//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSFse1610Info::operator == (const NSFse1610Info& o)
{
  return (_Donnees == o._Donnees) ;
}

//***************************************************************************// Implémentation des méthodes NSFse1620
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NSFse1620Data::NSFse1620Data()
{
	//
	// Met les champs de données à zéro
	//
	metAZero() ;
}

//---------------------------------------------------------------------------//  Constructeur copie
//---------------------------------------------------------------------------
NSFse1620Data::NSFse1620Data(const NSFse1620Data& rv)
{
	strcpy(numcompt,			rv.numcompt);
	strcpy(numprest,			rv.numprest);
	strcpy(date_depl, 		rv.date_depl);
	strcpy(code_ifd,   		rv.code_ifd);
  strcpy(montant_ifd_f,	rv.montant_ifd_f);
	strcpy(montant_ifd_e, rv.montant_ifd_e);
	strcpy(quantite, 			rv.quantite);
  strcpy(prix_unit_f,		rv.prix_unit_f);
  strcpy(prix_unit_e,		rv.prix_unit_e);
  strcpy(base_rmb, 			rv.base_rmb);
  strcpy(taux, 				  rv.taux);
  strcpy(rmb_amo, 			rv.rmb_amo);
  strcpy(rmb_amc, 			rv.rmb_amc);
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSFse1620Data::~NSFse1620Data()
{
}

//---------------------------------------------------------------------------
//  Opérateur =
//---------------------------------------------------------------------------
NSFse1620Data&
NSFse1620Data::operator=(const NSFse1620Data& src)
{
  if (this == &src)
  	return *this ;

	strcpy(numcompt,			src.numcompt);	strcpy(numprest,			src.numprest);
	strcpy(date_depl, 		src.date_depl);
	strcpy(code_ifd,   		src.code_ifd);
  strcpy(montant_ifd_f,	src.montant_ifd_f);
	strcpy(montant_ifd_e, src.montant_ifd_e);
	strcpy(quantite, 			src.quantite);
  strcpy(prix_unit_f,		src.prix_unit_f);
  strcpy(prix_unit_e,		src.prix_unit_e);
  strcpy(base_rmb, 			src.base_rmb);
  strcpy(taux, 				  src.taux);
  strcpy(rmb_amo, 			src.rmb_amo);
  strcpy(rmb_amc, 			src.rmb_amc);

	return *this ;
}

//---------------------------------------------------------------------------//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSFse1620Data::operator == ( const NSFse1620Data& o )
{
	if ((strcmp(numcompt, o.numcompt) == 0))
		return 1;
	else
		return 0;
}

//---------------------------------------------------------------------------
//  Met à zéro les variables de la fiche
//---------------------------------------------------------------------------
void
NSFse1620Data::metAZero()
{
	//
	// Met les champs de données à zéro
	//
	memset(numcompt, 	  	0, FSE_NUMCOMPT_LEN + 1);
	memset(numprest,   		0, FSE_NUMPREST_LEN + 1);
	memset(date_depl, 		0, FSE1620_DATE_DEPL_LEN + 1);
	memset(code_ifd,   		0, FSE1620_CODE_IFD_LEN + 1);
	memset(montant_ifd_f, 0, FSE1620_MONTANT_IFD_F_LEN + 1);
	memset(montant_ifd_e, 0, FSE1620_MONTANT_IFD_E_LEN + 1);
  memset(quantite,			0, FSE1620_QUANTITE_LEN + 1);
  memset(prix_unit_f,		0, FSE1620_PRIX_UNIT_F_LEN + 1);
  memset(prix_unit_e,		0, FSE1620_PRIX_UNIT_E_LEN + 1);
  memset(base_rmb,			0, FSE1620_BASE_RMB_LEN + 1);
  memset(taux,				  0, FSE1620_TAUX_LEN + 1);
  memset(rmb_amo,			  0, FSE1620_RMB_AMO_LEN + 1);
  memset(rmb_amc,			  0, FSE1620_RMB_AMC_LEN + 1);
}

//---------------------------------------------------------------------------//  Constructeur
//---------------------------------------------------------------------------
NSFse1620::NSFse1620(NSSuper* pSuper) : NSFiche(pSuper)
{
}

//---------------------------------------------------------------------------//  Constructeur copie
//---------------------------------------------------------------------------
NSFse1620::NSFse1620(const NSFse1620& rv)
          :NSFiche(rv._pSuper)
{
  _Donnees = rv._Donnees ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSFse1620::~NSFse1620()
{
}

//---------------------------------------------------------------------------
//  Fonction :  	NSFse1620::alimenteFiche()
//
//  Description : Transfère le contenu de pRecBuff dans les variables de
//             	la fiche
//---------------------------------------------------------------------------
void
NSFse1620::alimenteFiche()
{
  alimenteChamp(_Donnees.numcompt, 	    FSE1620_NUMCOMPT_FIELD,			 FSE_NUMCOMPT_LEN);
  alimenteChamp(_Donnees.numprest,		  FSE1620_NUMPREST_FIELD, 		 FSE_NUMPREST_LEN);
  alimenteChamp(_Donnees.date_depl, 		FSE1620_DATE_DEPL_FIELD, 		 FSE1620_DATE_DEPL_LEN);
  alimenteChamp(_Donnees.code_ifd, 	 	  FSE1620_CODE_IFD_FIELD,			 FSE1620_CODE_IFD_LEN);
  alimenteChamp(_Donnees.montant_ifd_f, FSE1620_MONTANT_IFD_F_FIELD, FSE1620_MONTANT_IFD_F_LEN);
  alimenteChamp(_Donnees.montant_ifd_e, FSE1620_MONTANT_IFD_E_FIELD, FSE1620_MONTANT_IFD_E_LEN);
  alimenteChamp(_Donnees.quantite,		  FSE1620_QUANTITE_FIELD,			 FSE1620_QUANTITE_LEN);
  alimenteChamp(_Donnees.prix_unit_f,	  FSE1620_PRIX_UNIT_F_FIELD,	 FSE1620_PRIX_UNIT_F_LEN);
  alimenteChamp(_Donnees.prix_unit_e,	  FSE1620_PRIX_UNIT_E_FIELD,	 FSE1620_PRIX_UNIT_E_LEN);
  alimenteChamp(_Donnees.base_rmb,		  FSE1620_BASE_RMB_FIELD,			 FSE1620_BASE_RMB_LEN);
  alimenteChamp(_Donnees.taux,				  FSE1620_TAUX_FIELD,				   FSE1620_TAUX_LEN);
  alimenteChamp(_Donnees.rmb_amo,			  FSE1620_RMB_AMO_FIELD,			 FSE1620_RMB_AMO_LEN);
  alimenteChamp(_Donnees.rmb_amc,			  FSE1620_RMB_AMC_FIELD,			 FSE1620_RMB_AMC_LEN);
}

//---------------------------------------------------------------------------//  Fonction :   	NSFse1620::videFiche()
//
//  Description : Transfère le contenu des valeurs de la fiche dans pRecBuff
//---------------------------------------------------------------------------
void
NSFse1620::videFiche()
{
  videChamp(_Donnees.numcompt, 	    FSE1620_NUMCOMPT_FIELD,			 FSE_NUMCOMPT_LEN);
  videChamp(_Donnees.numprest,			FSE1620_NUMPREST_FIELD, 		 FSE_NUMPREST_LEN);
  videChamp(_Donnees.date_depl, 		FSE1620_DATE_DEPL_FIELD, 		 FSE1620_DATE_DEPL_LEN);
  videChamp(_Donnees.code_ifd, 	 	  FSE1620_CODE_IFD_FIELD,			 FSE1620_CODE_IFD_LEN);
  videChamp(_Donnees.montant_ifd_f, FSE1620_MONTANT_IFD_F_FIELD, FSE1620_MONTANT_IFD_F_LEN);
  videChamp(_Donnees.montant_ifd_e, FSE1620_MONTANT_IFD_E_FIELD, FSE1620_MONTANT_IFD_E_LEN);
  videChamp(_Donnees.quantite,			FSE1620_QUANTITE_FIELD,			 FSE1620_QUANTITE_LEN);
  videChamp(_Donnees.prix_unit_f,	  FSE1620_PRIX_UNIT_F_FIELD,	 FSE1620_PRIX_UNIT_F_LEN);
  videChamp(_Donnees.prix_unit_e,	  FSE1620_PRIX_UNIT_E_FIELD,	 FSE1620_PRIX_UNIT_E_LEN);
  videChamp(_Donnees.base_rmb,			FSE1620_BASE_RMB_FIELD,			 FSE1620_BASE_RMB_LEN);
  videChamp(_Donnees.taux,				  FSE1620_TAUX_FIELD,				   FSE1620_TAUX_LEN);
  videChamp(_Donnees.rmb_amo,			  FSE1620_RMB_AMO_FIELD,			 FSE1620_RMB_AMO_LEN);
  videChamp(_Donnees.rmb_amc,			  FSE1620_RMB_AMC_FIELD,			 FSE1620_RMB_AMC_LEN);
}

//---------------------------------------------------------------------------//  Fonction :  	NSFse1620::open()
//
//  Description :	Ouvre la table primaire et les index secondaires
//
//  Returns :		PXERR_, s'il y a lieu
//---------------------------------------------------------------------------
DBIResult
NSFse1620::open()
{
	char tableName[] = "FSE1620.DB";
	//
	// Appelle la fonction open() de la classe de base pour ouvrir
	// l'index primaire
	//
	lastError = NSFiche::open(tableName, NSF_COMPTA);
	return(lastError);
}

//---------------------------------------------------------------------------
//  Function:  NSFse1620::Create()
//---------------------------------------------------------------------------
bool
NSFse1620::Create()
{
	return true;
}

//---------------------------------------------------------------------------
//  Function:  NSFse1620::Modify()
//---------------------------------------------------------------------------
bool
NSFse1620::Modify()
{
	return true;
}

//---------------------------------------------------------------------------//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSFse1620&
NSFse1620::operator=(const NSFse1620& src)
{
  if (this == &src)
  	return *this ;

	_Donnees = src._Donnees ;
	return *this ;}

//---------------------------------------------------------------------------//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSFse1620::operator==(const NSFse1620& o)
{
  return (_Donnees == o._Donnees) ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSFse1620Info::NSFse1620Info()
//
//  Description:	Constructeur par défaut
//---------------------------------------------------------------------------
NSFse1620Info::NSFse1620Info()
{
}

//---------------------------------------------------------------------------
//  Fonction:		NSFse1620Info::NSFse1620Info(NSFse1620*)
//
//  Description:	Constructeur à partir d'un NSFse1620
//---------------------------------------------------------------------------
NSFse1620Info::NSFse1620Info(const NSFse1620* pFse1620)
{
  if (pFse1620)
    _Donnees = pFse1620->_Donnees ;
}

//---------------------------------------------------------------------------//  Constructeur copie
//---------------------------------------------------------------------------
NSFse1620Info::NSFse1620Info(const NSFse1620Info& rv)
{
  _Donnees = rv._Donnees ;
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
NSFse1620Info::~NSFse1620Info()
{
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSFse1620Info&
NSFse1620Info::operator=(const NSFse1620Info& src)
{
  if (this == &src)
  	return *this ;

  _Donnees = src._Donnees ;
	return *this ;}

//---------------------------------------------------------------------------//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSFse1620Info::operator==(const NSFse1620Info& o)
{
  return (_Donnees == o._Donnees) ;
}

//***************************************************************************// Implémentation des méthodes NSFse1630
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NSFse1630Data::NSFse1630Data()
{
	//
	// Met les champs de données à zéro
	//
	metAZero() ;
}

//---------------------------------------------------------------------------//  Constructeur copie
//---------------------------------------------------------------------------
NSFse1630Data::NSFse1630Data(const NSFse1630Data& rv)
{
  strcpy(numcompt,		 rv.numcompt);
	strcpy(numprest,		 rv.numprest);
	strcpy(date_depl, 	 rv.date_depl);
	strcpy(code_ik,   	 rv.code_ik);
  strcpy(nbre_km,			 rv.nbre_km);
  strcpy(montant_ik_f, rv.montant_ik_f);
	strcpy(montant_ik_e, rv.montant_ik_e);
  strcpy(prix_unit_f,	 rv.prix_unit_f);
  strcpy(prix_unit_e,	 rv.prix_unit_e);
  strcpy(base_rmb, 		 rv.base_rmb);
  strcpy(taux, 				 rv.taux);
  strcpy(rmb_amo, 		 rv.rmb_amo);
  strcpy(rmb_amc, 		 rv.rmb_amc);
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
NSFse1630Data::~NSFse1630Data()
{
}

//---------------------------------------------------------------------------
//  Opérateur =
//---------------------------------------------------------------------------
NSFse1630Data&
NSFse1630Data::operator=(const NSFse1630Data& src)
{
  if (this == &src)
  	return *this ;

	strcpy(numcompt,		 src.numcompt);	strcpy(numprest,		 src.numprest);
	strcpy(date_depl, 	 src.date_depl);
	strcpy(code_ik,   	 src.code_ik);
  strcpy(nbre_km,			 src.nbre_km);
  strcpy(montant_ik_f, src.montant_ik_f);
	strcpy(montant_ik_e, src.montant_ik_e);
  strcpy(prix_unit_f,	 src.prix_unit_f);
  strcpy(prix_unit_e,	 src.prix_unit_e);
  strcpy(base_rmb, 		 src.base_rmb);
  strcpy(taux, 			   src.taux);
  strcpy(rmb_amo, 		 src.rmb_amo);
  strcpy(rmb_amc, 		 src.rmb_amc);

	return *this ;
}

//---------------------------------------------------------------------------//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSFse1630Data::operator==(const NSFse1630Data& o)
{
	if ((strcmp(numcompt, o.numcompt) == 0))
		return 1 ;
	else
		return 0 ;
}

//---------------------------------------------------------------------------//  Met à zéro les variables de la fiche
//---------------------------------------------------------------------------
void
NSFse1630Data::metAZero()
{
	//
	// Met les champs de données à zéro
	//
	memset(numcompt, 	  	0, FSE_NUMCOMPT_LEN + 1);
	memset(numprest,   		0, FSE_NUMPREST_LEN + 1);
	memset(date_depl, 		0, FSE1630_DATE_DEPL_LEN + 1);
	memset(code_ik,   		0, FSE1630_CODE_IK_LEN + 1);
  memset(nbre_km,			  0, FSE1630_NBRE_KM_LEN + 1);
	memset(montant_ik_f,  0, FSE1630_MONTANT_IK_F_LEN + 1);
	memset(montant_ik_e, 	0, FSE1630_MONTANT_IK_E_LEN + 1);
  memset(prix_unit_f,		0, FSE1630_PRIX_UNIT_F_LEN + 1);
  memset(prix_unit_e,		0, FSE1630_PRIX_UNIT_E_LEN + 1);
  memset(base_rmb,			0, FSE1630_BASE_RMB_LEN + 1);
  memset(taux,				  0, FSE1630_TAUX_LEN + 1);
  memset(rmb_amo,			  0, FSE1630_RMB_AMO_LEN + 1);
  memset(rmb_amc,			  0, FSE1630_RMB_AMC_LEN + 1);
}

//---------------------------------------------------------------------------//  Constructeur
//---------------------------------------------------------------------------
NSFse1630::NSFse1630(NSSuper* pSuper) : NSFiche(pSuper)
{
}

//---------------------------------------------------------------------------//  Constructeur copie
//---------------------------------------------------------------------------
NSFse1630::NSFse1630(const NSFse1630& rv)
          :NSFiche(rv._pSuper)
{
  _Donnees = rv._Donnees ;
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
NSFse1630::~NSFse1630()
{
}

//---------------------------------------------------------------------------//  Fonction :  	NSFse1630::alimenteFiche()
//
//  Description : Transfère le contenu de pRecBuff dans les variables de
//             	la fiche
//---------------------------------------------------------------------------
void
NSFse1630::alimenteFiche()
{
  alimenteChamp(_Donnees.numcompt, 	   FSE1630_NUMCOMPT_FIELD,		 FSE_NUMCOMPT_LEN);
  alimenteChamp(_Donnees.numprest,		 FSE1630_NUMPREST_FIELD, 		 FSE_NUMPREST_LEN);
  alimenteChamp(_Donnees.date_depl, 	 FSE1630_DATE_DEPL_FIELD, 	 FSE1630_DATE_DEPL_LEN);
  alimenteChamp(_Donnees.code_ik, 	 	 FSE1630_CODE_IK_FIELD,			 FSE1630_CODE_IK_LEN);
  alimenteChamp(_Donnees.nbre_km, 	 	 FSE1630_NBRE_KM_FIELD,			 FSE1630_NBRE_KM_LEN);
  alimenteChamp(_Donnees.montant_ik_f, FSE1630_MONTANT_IK_F_FIELD, FSE1630_MONTANT_IK_F_LEN);
  alimenteChamp(_Donnees.montant_ik_e, FSE1630_MONTANT_IK_E_FIELD, FSE1630_MONTANT_IK_E_LEN);
  alimenteChamp(_Donnees.prix_unit_f,	 FSE1630_PRIX_UNIT_F_FIELD,	 FSE1630_PRIX_UNIT_F_LEN);
  alimenteChamp(_Donnees.prix_unit_e,	 FSE1630_PRIX_UNIT_E_FIELD,	 FSE1630_PRIX_UNIT_E_LEN);
  alimenteChamp(_Donnees.base_rmb,		 FSE1630_BASE_RMB_FIELD,		 FSE1630_BASE_RMB_LEN);
  alimenteChamp(_Donnees.taux,				 FSE1630_TAUX_FIELD,				 FSE1630_TAUX_LEN);
  alimenteChamp(_Donnees.rmb_amo,			 FSE1630_RMB_AMO_FIELD,			 FSE1630_RMB_AMO_LEN);
  alimenteChamp(_Donnees.rmb_amc,			 FSE1630_RMB_AMC_FIELD,			 FSE1630_RMB_AMC_LEN);
}

//---------------------------------------------------------------------------//  Fonction :   	NSFse1630::videFiche()
//
//  Description : Transfère le contenu des valeurs de la fiche dans pRecBuff
//---------------------------------------------------------------------------
void
NSFse1630::videFiche()
{
  videChamp(_Donnees.numcompt, 	   FSE1630_NUMCOMPT_FIELD,		 FSE_NUMCOMPT_LEN);
  videChamp(_Donnees.numprest,		 FSE1630_NUMPREST_FIELD, 		 FSE_NUMPREST_LEN);
  videChamp(_Donnees.date_depl, 	 FSE1630_DATE_DEPL_FIELD, 	 FSE1630_DATE_DEPL_LEN);
  videChamp(_Donnees.code_ik, 	 	 FSE1630_CODE_IK_FIELD,			 FSE1630_CODE_IK_LEN);
  videChamp(_Donnees.nbre_km, 	 	 FSE1630_NBRE_KM_FIELD,			 FSE1630_NBRE_KM_LEN);
  videChamp(_Donnees.montant_ik_f, FSE1630_MONTANT_IK_F_FIELD, FSE1630_MONTANT_IK_F_LEN);
  videChamp(_Donnees.montant_ik_e, FSE1630_MONTANT_IK_E_FIELD, FSE1630_MONTANT_IK_E_LEN);
  videChamp(_Donnees.prix_unit_f,	 FSE1630_PRIX_UNIT_F_FIELD,	 FSE1630_PRIX_UNIT_F_LEN);
  videChamp(_Donnees.prix_unit_e,	 FSE1630_PRIX_UNIT_E_FIELD,	 FSE1630_PRIX_UNIT_E_LEN);
  videChamp(_Donnees.base_rmb,		 FSE1630_BASE_RMB_FIELD,		 FSE1630_BASE_RMB_LEN);
  videChamp(_Donnees.taux,				 FSE1630_TAUX_FIELD,				 FSE1630_TAUX_LEN);
  videChamp(_Donnees.rmb_amo,			 FSE1630_RMB_AMO_FIELD,			 FSE1630_RMB_AMO_LEN);
  videChamp(_Donnees.rmb_amc,			 FSE1630_RMB_AMC_FIELD,			 FSE1630_RMB_AMC_LEN);
}

//---------------------------------------------------------------------------//  Fonction :  	NSFse1630::open()
//
//  Description :	Ouvre la table primaire et les index secondaires
//
//  Returns :		PXERR_, s'il y a lieu
//---------------------------------------------------------------------------
DBIResult
NSFse1630::open()
{
	char tableName[] = "FSE1630.DB";
	//
	// Appelle la fonction open() de la classe de base pour ouvrir
	// l'index primaire
	//
	lastError = NSFiche::open(tableName, NSF_COMPTA);
	return(lastError);
}

//---------------------------------------------------------------------------
//  Function:  NSFse1630::Create()
//---------------------------------------------------------------------------
bool
NSFse1630::Create()
{
	return true;
}

//---------------------------------------------------------------------------
//  Function:  NSFse1630::Modify()
//---------------------------------------------------------------------------
bool
NSFse1630::Modify()
{
	return true;
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSFse1630&
NSFse1630::operator=(const NSFse1630& src)
{
  if (this == &src)
  	return *this ;

	_Donnees = src._Donnees ;
	return *this ;}

//---------------------------------------------------------------------------//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSFse1630::operator == (const NSFse1630& o)
{
  return (_Donnees == o._Donnees) ;
}

//---------------------------------------------------------------------------//  Fonction:		NSFse1630Info::NSFse1630Info()
//
//  Description:	Constructeur par défaut
//---------------------------------------------------------------------------
NSFse1630Info::NSFse1630Info()
{
}

//---------------------------------------------------------------------------
//  Fonction:		NSFse1630Info::NSFse1630Info(NSFse1630*)
//
//  Description:	Constructeur à partir d'un NSFse1630
//---------------------------------------------------------------------------
NSFse1630Info::NSFse1630Info(const NSFse1630* pFse1630)
{
  if (pFse1630)
    _Donnees = pFse1630->_Donnees ;
}

//---------------------------------------------------------------------------//  Constructeur copie
//---------------------------------------------------------------------------
NSFse1630Info::NSFse1630Info(const NSFse1630Info& rv)
{
  _Donnees = rv._Donnees ;
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
NSFse1630Info::~NSFse1630Info()
{
}

//---------------------------------------------------------------------------//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSFse1630Info&
NSFse1630Info::operator=(const NSFse1630Info& src)
{
  if (this == &src)
  	return *this ;

	_Donnees = src._Donnees ;
	return *this ;}

//---------------------------------------------------------------------------//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSFse1630Info::operator==(const NSFse1630Info& o)
{
  return (_Donnees == o._Donnees) ;
}

//***************************************************************************
// Implémentation des méthodes NSFseCCAM
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NSFseCCAMData::NSFseCCAMData()
{
	//
	// Met les champs de données à zéro
	//
	metAZero() ;
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSFseCCAMData::NSFseCCAMData(const NSFseCCAMData& rv)
{
	strcpy(numcompt, 		  rv.numcompt) ;
	strcpy(numprest,  		rv.numprest) ;
	strcpy(date, 	 		    rv.date) ;
	strcpy(code,  			  rv.code) ;
	strcpy(pourcent,      rv.pourcent) ;	strcpy(modificateurs, rv.modificateurs) ;	strcpy(montant_e,   	rv.montant_e) ;
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
NSFseCCAMData::~NSFseCCAMData()
{
}

//---------------------------------------------------------------------------
//  Opérateur =
//---------------------------------------------------------------------------
NSFseCCAMData&
NSFseCCAMData::operator=(const NSFseCCAMData& src)
{
	if (this == &src)
		return *this ;

	strcpy(numcompt, 		  src.numcompt) ;
	strcpy(numprest,  		src.numprest) ;
	strcpy(date, 	 		    src.date) ;
	strcpy(code,  			  src.code) ;
	strcpy(pourcent,      src.pourcent) ;	strcpy(modificateurs, src.modificateurs) ;	strcpy(montant_e,   	src.montant_e) ;

	return *this ;}

//---------------------------------------------------------------------------//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSFseCCAMData::operator==(const NSFseCCAMData& o)
{
	if ((strcmp(numcompt, o.numcompt) == 0))
		return 1;
	else
		return 0;
}

//---------------------------------------------------------------------------
//  Met à zéro les variables de la fiche
//---------------------------------------------------------------------------
void
NSFseCCAMData::metAZero()
{
	//
	// Met les champs de données à zéro
	//
	memset(numcompt, 	  	0, FSE_NUMCOMPT_LEN + 1) ;
	memset(numprest,   		0, FSE_NUMPREST_LEN + 1) ;
	memset(date, 	 	  	  0, FSECCAM_DATE_LEN + 1) ;
	memset(code,  			  0, FSECCAM_CODE_LEN + 1) ;
	memset(pourcent,      0, FSECCAM_POURCENT_LEN + 1) ;	memset(modificateurs, 0, FSECCAM_MODIFICATEURS_LEN + 1) ;	memset(montant_e, 		0, FSECCAM_MONTANT_E_LEN + 1) ;
}

//---------------------------------------------------------------------------//  Constructeur
//---------------------------------------------------------------------------
NSFseCCAM::NSFseCCAM(NSSuper* pSuper)
          :NSFiche(pSuper)
{
}

//---------------------------------------------------------------------------//  Constructeur copie
//---------------------------------------------------------------------------
NSFseCCAM::NSFseCCAM(const NSFseCCAM& rv)
          :NSFiche(rv._pSuper)
{
  _Donnees = rv._Donnees ;
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
NSFseCCAM::~NSFseCCAM()
{
}

//---------------------------------------------------------------------------//  Fonction :  	NSFseCCAM::alimenteFiche()
//
//  Description : Transfère le contenu de pRecBuff dans les variables de
//             	la fiche
//---------------------------------------------------------------------------
void
NSFseCCAM::alimenteFiche()
{
	alimenteChamp(_Donnees.numcompt,      FSECCAM_NUMCOMPT_FIELD,      FSE_NUMCOMPT_LEN) ;
	alimenteChamp(_Donnees.numprest,      FSECCAM_NUMPREST_FIELD,      FSE_NUMPREST_LEN) ;
	alimenteChamp(_Donnees.date,          FSECCAM_DATE_FIELD,          FSECCAM_DATE_LEN) ;
	alimenteChamp(_Donnees.code,          FSECCAM_CODE_FIELD,          FSECCAM_CODE_LEN) ;
	alimenteChamp(_Donnees.pourcent,      FSECCAM_POURCENT_FIELD,      FSECCAM_POURCENT_LEN) ;	alimenteChamp(_Donnees.modificateurs, FSECCAM_MODIFICATEURS_FIELD, FSECCAM_MODIFICATEURS_LEN) ;	alimenteChamp(_Donnees.montant_e,     FSECCAM_MONTANT_E_FIELD,     FSECCAM_MONTANT_E_LEN) ;
}

//---------------------------------------------------------------------------//  Fonction :   	NSFseCCAM::videFiche()
//
//  Description : Transfère le contenu des valeurs de la fiche dans pRecBuff
//---------------------------------------------------------------------------
void
NSFseCCAM::videFiche()
{
	videChamp(_Donnees.numcompt, 	    FSECCAM_NUMCOMPT_FIELD,  	   FSE_NUMCOMPT_LEN) ;
	videChamp(_Donnees.numprest, 		  FSECCAM_NUMPREST_FIELD, 	   FSE_NUMPREST_LEN) ;
	videChamp(_Donnees.date, 	 		    FSECCAM_DATE_FIELD, 	  	   FSECCAM_DATE_LEN) ;
	videChamp(_Donnees.code,  			  FSECCAM_CODE_FIELD, 		     FSECCAM_CODE_LEN) ;
	videChamp(_Donnees.pourcent,      FSECCAM_POURCENT_FIELD,      FSECCAM_POURCENT_LEN) ;	videChamp(_Donnees.modificateurs, FSECCAM_MODIFICATEURS_FIELD, FSECCAM_MODIFICATEURS_LEN) ;	videChamp(_Donnees.montant_e, 		FSECCAM_MONTANT_E_FIELD,  	 FSECCAM_MONTANT_E_LEN) ;
}

//---------------------------------------------------------------------------//  Fonction :  	NSFseCCAM::open()
//
//  Description :	Ouvre la table primaire et les index secondaires
//
//  Returns :		PXERR_, s'il y a lieu
//---------------------------------------------------------------------------
DBIResult
NSFseCCAM::open()
{
	char tableName[] = "FSECCAM.DB" ;
	//
	// Appelle la fonction open() de la classe de base pour ouvrir
	// l'index primaire
	//
	lastError = NSFiche::open(tableName, NSF_COMPTA) ;
	return(lastError) ;
}

//---------------------------------------------------------------------------//  Function:  NSFseCCAM::Create()
//---------------------------------------------------------------------------
bool
NSFseCCAM::Create()
{
	return true ;
}

//---------------------------------------------------------------------------//  Function:  NSFseCCAM::Modify()
//---------------------------------------------------------------------------
bool
NSFseCCAM::Modify()
{
	return true ;
}

bool
NSFseCCAM::initArrayForCompt(string sNumCompt, NSFse16Array *paFseCCAM)
{
  if ((string("") == sNumCompt) || (NULL == paFseCCAM))
    return false ;

  lastError = open() ;
  if (DBIERR_NONE != lastError)
  {
   	erreur("Erreur à l'ouverture de la base FseCCAM.", standardError, lastError) ;
    return false ;
  }

  lastError = chercheClef(&sNumCompt,
                          "",
                          0,
                          keySEARCHGEQ,
                          dbiWRITELOCK) ;

  if (DBIERR_BOF == lastError)	// cas fichier vide
  {
  	close() ;
    return true ;		// le tableau est vide
  }

  if ((DBIERR_NONE != lastError) && (DBIERR_EOF != lastError))
  {
  	erreur("Erreur à la recherche d'une fiche FseCCAM.", standardError, lastError) ;
    close() ;
    return false ;
  }

  while (DBIERR_EOF != lastError)
  {
  	lastError = getRecord() ;
    if (DBIERR_NONE != lastError)
    {
    	erreur("Erreur à la lecture d'une fiche FseCCAM.", standardError, lastError) ;
      close() ;
      return false ;
    }

    // condition d'arret
    if (string(_Donnees.numcompt) != sNumCompt)
    	break ;

    // on remplit le tableau
    paFseCCAM->push_back(new NSBlocFse16(this)) ;

    // ... on passe au composant suivant
    lastError = suivant(dbiWRITELOCK) ;
    if ((DBIERR_NONE != lastError) && (DBIERR_EOF != lastError))
    {
    	erreur("Erreur d'acces à une fiche FseCCAM.", standardError, lastError) ;
      close() ;
      return false ;
    }
  } // fin du while (recherche des composants images)

  // on ferme la base FseCCAM
  lastError = close() ;
  if (DBIERR_NONE != lastError)
  {
   	erreur("Erreur de fermeture du fichier FseCCAM.", standardError, lastError) ;
    return false ;
  }

	return true ;
}

//---------------------------------------------------------------------------//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSFseCCAM&
NSFseCCAM::operator=(const NSFseCCAM& src)
{
	if (this == &src)
		return *this ;

	_Donnees = src._Donnees ;

	return *this ;
}

//---------------------------------------------------------------------------//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSFseCCAM::operator == (const NSFseCCAM& o)
{
	return (_Donnees == o._Donnees) ;
}

//---------------------------------------------------------------------------//  Fonction:		NSFseCCAMInfo::NSFseCCAMInfo()
//
//  Description:	Constructeur par défaut
//---------------------------------------------------------------------------
NSFseCCAMInfo::NSFseCCAMInfo()
{
}

//---------------------------------------------------------------------------//  Description:	Constructeur à partir d'un NSFseCCAM
//---------------------------------------------------------------------------
NSFseCCAMInfo::NSFseCCAMInfo(const NSFseCCAM* pFseCCAM)
{
  if (pFseCCAM)
    _Donnees = pFseCCAM->_Donnees ;
}

//---------------------------------------------------------------------------//  Constructeur copie
//---------------------------------------------------------------------------
NSFseCCAMInfo::NSFseCCAMInfo(const NSFseCCAMInfo& rv)
{
  _Donnees = rv._Donnees ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSFseCCAMInfo::~NSFseCCAMInfo()
{
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSFseCCAMInfo&
NSFseCCAMInfo::operator=(const NSFseCCAMInfo& src)
{
  if (this == &src)
  	return *this ;

  _Donnees = src._Donnees ;

	return *this ;
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSFseCCAMInfo::operator==(const NSFseCCAMInfo& o)
{
	 return (_Donnees == o._Donnees) ;
}
voidNSFseCCAMInfo::initFromCCAMinfo(const NSCcamInfo* pCCAMInfo){  if ((NSCcamInfo*) NULL == pCCAMInfo)    return ;	strcpy(_Donnees.code,      pCCAMInfo->getCode().c_str()) ;	strcpy(_Donnees.montant_e, pCCAMInfo->getPrix().c_str()) ;}//***************************************************************************// Implémentation des méthodes NSFseLibre
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NSFseLibreData::NSFseLibreData()
{
	//
	// Met les champs de données à zéro
	//
	metAZero() ;
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSFseLibreData::NSFseLibreData(const NSFseLibreData& rv)
{
	strcpy(numcompt, 	rv.numcompt) ;
	strcpy(numprest,  rv.numprest) ;
	strcpy(date, 	 		rv.date) ;
	strcpy(code,  		rv.code) ;
	strcpy(pourcent,  rv.pourcent) ;  strcpy(montant_f, rv.montant_f) ;	strcpy(montant_e, rv.montant_e) ;}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
NSFseLibreData::~NSFseLibreData()
{
}

//---------------------------------------------------------------------------
//  Opérateur =
//---------------------------------------------------------------------------
NSFseLibreData&
NSFseLibreData::operator=(const NSFseLibreData& src)
{
	if (this == &src)
		return *this ;

	strcpy(numcompt, 	src.numcompt) ;
	strcpy(numprest,  src.numprest) ;
	strcpy(date, 	 		src.date) ;
	strcpy(code,  		src.code) ;
	strcpy(pourcent,  src.pourcent) ;	strcpy(montant_e, src.montant_e) ;  strcpy(montant_f, src.montant_f) ;

	return *this ;}

//---------------------------------------------------------------------------//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSFseLibreData::operator==(const NSFseLibreData& o)
{
	if ((strcmp(numcompt, o.numcompt) == 0))
		return 1;
	else
		return 0;
}

//---------------------------------------------------------------------------
//  Met à zéro les variables de la fiche
//---------------------------------------------------------------------------
void
NSFseLibreData::metAZero()
{
	//
	// Met les champs de données à zéro
	//
	memset(numcompt, 	0, FSE_NUMCOMPT_LEN + 1) ;
	memset(numprest,  0, FSE_NUMPREST_LEN + 1) ;
	memset(date, 	 	  0, FSELIBRE_DATE_LEN + 1) ;
	memset(code,  		0, FSELIBRE_CODE_LEN + 1) ;
	memset(pourcent,  0, FSELIBRE_POURCENT_LEN + 1) ;	memset(montant_e, 0, FSELIBRE_MONTANT_LEN + 1) ;}
//---------------------------------------------------------------------------//  Constructeur
//---------------------------------------------------------------------------
NSFseLibre::NSFseLibre(NSSuper* pSuper)
           :NSFiche(pSuper)
{
}

//---------------------------------------------------------------------------//  Constructeur copie
//---------------------------------------------------------------------------
NSFseLibre::NSFseLibre(const NSFseLibre& rv)
           :NSFiche(rv._pSuper)
{
  _Donnees = rv._Donnees ;
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
NSFseLibre::~NSFseLibre()
{
}

//---------------------------------------------------------------------------//  Fonction :  	NSFseCCAM::alimenteFiche()
//
//  Description : Transfère le contenu de pRecBuff dans les variables de
//             	la fiche
//---------------------------------------------------------------------------
void
NSFseLibre::alimenteFiche()
{
	alimenteChamp(_Donnees.numcompt,  FSELIBRE_NUMCOMPT_FIELD,  FSE_NUMCOMPT_LEN) ;
	alimenteChamp(_Donnees.numprest,  FSELIBRE_NUMPREST_FIELD,  FSE_NUMPREST_LEN) ;
	alimenteChamp(_Donnees.date,      FSELIBRE_DATE_FIELD,      FSELIBRE_DATE_LEN) ;
	alimenteChamp(_Donnees.code,      FSELIBRE_CODE_FIELD,      FSELIBRE_CODE_LEN) ;
	alimenteChamp(_Donnees.pourcent,  FSELIBRE_POURCENT_FIELD,  FSELIBRE_POURCENT_LEN) ;	alimenteChamp(_Donnees.montant_e, FSELIBRE_MONTANT_E_FIELD, FSELIBRE_MONTANT_LEN) ;  alimenteChamp(_Donnees.montant_f, FSELIBRE_MONTANT_F_FIELD, FSELIBRE_MONTANT_LEN) ;}

//---------------------------------------------------------------------------//  Fonction :   	NSFseCCAM::videFiche()
//
//  Description : Transfère le contenu des valeurs de la fiche dans pRecBuff
//---------------------------------------------------------------------------
void
NSFseLibre::videFiche()
{
	videChamp(_Donnees.numcompt, 	FSELIBRE_NUMCOMPT_FIELD,  FSE_NUMCOMPT_LEN) ;
	videChamp(_Donnees.numprest, 	FSELIBRE_NUMPREST_FIELD,  FSE_NUMPREST_LEN) ;
	videChamp(_Donnees.date, 	 		FSELIBRE_DATE_FIELD, 	    FSELIBRE_DATE_LEN) ;
	videChamp(_Donnees.code,  		FSELIBRE_CODE_FIELD, 		  FSELIBRE_CODE_LEN) ;
	videChamp(_Donnees.pourcent,  FSELIBRE_POURCENT_FIELD,  FSELIBRE_POURCENT_LEN) ;	videChamp(_Donnees.montant_e, FSELIBRE_MONTANT_E_FIELD, FSELIBRE_MONTANT_LEN) ;  videChamp(_Donnees.montant_f, FSELIBRE_MONTANT_F_FIELD, FSELIBRE_MONTANT_LEN) ;}

//---------------------------------------------------------------------------//  Fonction :  	NSFseCCAM::open()
//
//  Description :	Ouvre la table primaire et les index secondaires
//
//  Returns :		PXERR_, s'il y a lieu
//---------------------------------------------------------------------------
DBIResult
NSFseLibre::open()
{
	char tableName[] = "FSELIBRE.DB" ;
	//
	// Appelle la fonction open() de la classe de base pour ouvrir
	// l'index primaire
	//
	lastError = NSFiche::open(tableName, NSF_COMPTA) ;
	return(lastError) ;
}

//---------------------------------------------------------------------------//  Function:  NSFseCCAM::Create()
//---------------------------------------------------------------------------
bool
NSFseLibre::Create()
{
	return true ;
}

//---------------------------------------------------------------------------//  Function:  NSFseCCAM::Modify()
//---------------------------------------------------------------------------
bool
NSFseLibre::Modify()
{
	return true ;
}

bool
NSFseLibre::initArrayForCompt(string sNumCompt, NSFse16Array *paFseCCAM)
{
  if ((string("") == sNumCompt) || ((NSFse16Array*) NULL == paFseCCAM))
    return false ;

  lastError = open() ;
  if (DBIERR_NONE != lastError)
  {
   	erreur("Erreur à l'ouverture de la base FseLibre.", standardError, lastError) ;
    return false ;
  }

  lastError = chercheClef(&sNumCompt,
                          "",
                          0,
                          keySEARCHGEQ,
                          dbiWRITELOCK) ;

  if (DBIERR_BOF == lastError)	// cas fichier vide
  {
  	close() ;
    return true ;		// le tableau est vide
  }

  if ((DBIERR_NONE != lastError) && (DBIERR_EOF != lastError))
  {
  	erreur("Erreur à la recherche d'une fiche FseLibre.", standardError, lastError) ;
    close() ;
    return false ;
  }

  while (DBIERR_EOF != lastError)
  {
  	lastError = getRecord() ;
    if (DBIERR_NONE != lastError)
    {
    	erreur("Erreur à la lecture d'une fiche FseLibre.", standardError, lastError) ;
      close() ;
      return false ;
    }

    // condition d'arret
    if (string(_Donnees.numcompt) != sNumCompt)
    	break ;

    // on remplit le tableau
    paFseCCAM->push_back(new NSBlocFse16(this)) ;

    // ... on passe au composant suivant
    lastError = suivant(dbiWRITELOCK) ;
    if ((DBIERR_NONE != lastError) && (DBIERR_EOF != lastError))
    {
    	erreur("Erreur d'acces à une fiche FseLibre.", standardError, lastError) ;
      close() ;
      return false ;
    }
  } // fin du while (recherche des composants images)

  // on ferme la base FseCCAM
  lastError = close() ;
  if (DBIERR_NONE != lastError)
  {
   	erreur("Erreur de fermeture du fichier FseLibre.", standardError, lastError) ;
    return false ;
  }

	return true ;
}

//---------------------------------------------------------------------------//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSFseLibre&
NSFseLibre::operator=(const NSFseLibre& src)
{
	if (this == &src)
		return *this ;

	_Donnees = src._Donnees ;

	return *this ;
}

//---------------------------------------------------------------------------//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSFseLibre::operator == (const NSFseLibre& o)
{
	return (_Donnees == o._Donnees) ;
}
//---------------------------------------------------------------------------//  Fonction:		NSFseCCAMInfo::NSFseCCAMInfo()
//
//  Description:	Constructeur par défaut
//---------------------------------------------------------------------------
NSFseLibreInfo::NSFseLibreInfo()
{
}

//---------------------------------------------------------------------------//  Description:	Constructeur à partir d'un NSFseCCAM
//---------------------------------------------------------------------------
NSFseLibreInfo::NSFseLibreInfo(const NSFseLibre* pFseLibre)
{
  if (pFseLibre)
    _Donnees = pFseLibre->_Donnees ;
}

//---------------------------------------------------------------------------//  Constructeur copie
//---------------------------------------------------------------------------
NSFseLibreInfo::NSFseLibreInfo(const NSFseLibreInfo& rv)
{
  _Donnees = rv._Donnees ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSFseLibreInfo::~NSFseLibreInfo()
{
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSFseLibreInfo&
NSFseLibreInfo::operator=(const NSFseLibreInfo& src)
{
  if (this == &src)
  	return *this ;

  _Donnees = src._Donnees ;

	return *this ;
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSFseLibreInfo::operator==(const NSFseLibreInfo& o)
{
	 return (_Donnees == o._Donnees) ;
}
voidNSFseLibreInfo::initFromLibreInfo(const NSLibreInfo* pLibreInfo){  if ((NSLibreInfo*) NULL == pLibreInfo)    return ;	strcpy(_Donnees.code,      pLibreInfo->getCode().c_str()) ;	strcpy(_Donnees.montant_e, pLibreInfo->getPrixE().c_str()) ;  strcpy(_Donnees.montant_f, pLibreInfo->getPrixF().c_str()) ;}//***************************************************************************// Implémentation des méthodes NSFseMat
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NSFseMatData::NSFseMatData()
{
	//
	// Met les champs de données à zéro
	//
	metAZero() ;
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSFseMatData::NSFseMatData(const NSFseMatData& rv)
{
	strcpy(_numcompt,  rv._numcompt) ;
	strcpy(_numprest,  rv._numprest) ;
  strcpy(_code,      rv._code) ;
	strcpy(_montant_e, rv._montant_e) ;
  strcpy(_montant_f, rv._montant_f) ;
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
NSFseMatData::~NSFseMatData()
{
}

//---------------------------------------------------------------------------
//  Opérateur =
//---------------------------------------------------------------------------
NSFseMatData&
NSFseMatData::operator=(const NSFseMatData& src)
{
	if (this == &src)
		return *this ;

	strcpy(_numcompt,  src._numcompt) ;
	strcpy(_numprest,  src._numprest) ;
	strcpy(_code,      src._code) ;
	strcpy(_montant_f, src._montant_f) ;
  strcpy(_montant_e, src._montant_e) ;

	return *this ;}

//---------------------------------------------------------------------------//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSFseMatData::operator==(const NSFseMatData& o)
{
	if ((strcmp(_numcompt, o._numcompt) == 0))
		return 1;
	else
		return 0;
}

//---------------------------------------------------------------------------
//  Met à zéro les variables de la fiche
//---------------------------------------------------------------------------
void
NSFseMatData::metAZero()
{
	//
	// Met les champs de données à zéro
	//
	memset(_numcompt,  0, FSE_NUMCOMPT_LEN + 1) ;
	memset(_numprest,  0, FSE_NUMPREST_LEN + 1) ;
	memset(_code,      0, FSEMAT_CODE_LEN + 1) ;
	memset(_montant_f, 0, FSEMAT_MONTANT_LEN + 1) ;  memset(_montant_e, 0, FSEMAT_MONTANT_LEN + 1) ;}

//---------------------------------------------------------------------------//  Constructeur
//---------------------------------------------------------------------------
NSFseMat::NSFseMat(NSSuper* pSuper)
         :NSFiche(pSuper)
{
}

//---------------------------------------------------------------------------//  Constructeur copie
//---------------------------------------------------------------------------
NSFseMat::NSFseMat(const NSFseMat& rv)
         :NSFiche(rv._pSuper)
{
  _Donnees = rv._Donnees ;
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
NSFseMat::~NSFseMat()
{
}

//---------------------------------------------------------------------------// Description : Transfère le contenu de pRecBuff dans les variables de
//             	la fiche
//---------------------------------------------------------------------------
void
NSFseMat::alimenteFiche()
{
	alimenteChamp(_Donnees._numcompt,  FSEMAT_NUMCOMPT_FIELD,  FSE_NUMCOMPT_LEN) ;
	alimenteChamp(_Donnees._numprest,  FSEMAT_NUMPREST_FIELD,  FSE_NUMPREST_LEN) ;
	alimenteChamp(_Donnees._code,      FSEMAT_CODE_FIELD,      FSEMAT_CODE_LEN) ;
  alimenteChamp(_Donnees._montant_f, FSEMAT_MONTANT_F_FIELD, FSEMAT_MONTANT_LEN) ;
	alimenteChamp(_Donnees._montant_e, FSEMAT_MONTANT_E_FIELD, FSEMAT_MONTANT_LEN) ;
}

//---------------------------------------------------------------------------// Description : Transfère le contenu des valeurs de la fiche dans pRecBuff
//---------------------------------------------------------------------------
void
NSFseMat::videFiche()
{
	videChamp(_Donnees._numcompt,  FSEMAT_NUMCOMPT_FIELD,  FSE_NUMCOMPT_LEN) ;
	videChamp(_Donnees._numprest,  FSEMAT_NUMPREST_FIELD,  FSE_NUMPREST_LEN) ;
	videChamp(_Donnees._code,      FSEMAT_CODE_FIELD,      FSEMAT_CODE_LEN) ;
	videChamp(_Donnees._montant_f, FSEMAT_MONTANT_F_FIELD, FSEMAT_MONTANT_LEN) ;  videChamp(_Donnees._montant_e, FSEMAT_MONTANT_E_FIELD, FSEMAT_MONTANT_LEN) ;}

//---------------------------------------------------------------------------//  Description :	Ouvre la table primaire et les index secondaires
//
//  Returns :		PXERR_, s'il y a lieu
//---------------------------------------------------------------------------
DBIResult
NSFseMat::open()
{
	char tableName[] = "FSEMAT.DB" ;
	//
	// Appelle la fonction open() de la classe de base pour ouvrir
	// l'index primaire
	//
	lastError = NSFiche::open(tableName, NSF_COMPTA) ;
	return(lastError) ;
}

boolNSFseMat::Create()
{
	return true ;
}

boolNSFseMat::Modify()
{
	return true ;
}

bool
NSFseMat::initArrayForCompt(string sNumCompt, NSFse16Array *paFseCCAM)
{
  if ((string("") == sNumCompt) || ((NSFse16Array*) NULL == paFseCCAM))
    return false ;

  lastError = open() ;
  if (DBIERR_NONE != lastError)
  {
   	erreur("Erreur à l'ouverture de la base FseMat.", standardError, lastError) ;
    return false ;
  }

  lastError = chercheClef(&sNumCompt,
                          "",
                          0,
                          keySEARCHGEQ,
                          dbiWRITELOCK) ;

  if (DBIERR_BOF == lastError)	// cas fichier vide
  {
  	close() ;
    return true ;		// le tableau est vide
  }

  if ((DBIERR_NONE != lastError) && (DBIERR_EOF != lastError))
  {
  	erreur("Erreur à la recherche d'une fiche FseMat.", standardError, lastError) ;
    close() ;
    return false ;
  }

  while (DBIERR_EOF != lastError)
  {
  	lastError = getRecord() ;
    if (DBIERR_NONE != lastError)
    {
    	erreur("Erreur à la lecture d'une fiche FseMat.", standardError, lastError) ;
      close() ;
      return false ;
    }

    // condition d'arret
    if (string(_Donnees._numcompt) != sNumCompt)
    	break ;

    // on remplit le tableau
    paFseCCAM->push_back(new NSBlocFse16(this)) ;

    // ... on passe au composant suivant
    lastError = suivant(dbiWRITELOCK) ;
    if ((DBIERR_NONE != lastError) && (DBIERR_EOF != lastError))
    {
    	erreur("Erreur d'acces à une fiche FseMat.", standardError, lastError) ;
      close() ;
      return false ;
    }
  } // fin du while (recherche des composants images)

  // on ferme la base FseCCAM
  lastError = close() ;
  if (DBIERR_NONE != lastError)
  {
   	erreur("Erreur de fermeture du fichier FseMat.", standardError, lastError) ;
    return false ;
  }

	return true ;
}

//---------------------------------------------------------------------------//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSFseMat&
NSFseMat::operator=(const NSFseMat& src)
{
	if (this == &src)
		return *this ;

	_Donnees = src._Donnees ;

	return *this ;
}

//---------------------------------------------------------------------------//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSFseMat::operator == (const NSFseMat& o)
{
	return (_Donnees == o._Donnees) ;
}

//---------------------------------------------------------------------------//  Description:	Constructeur par défaut
//---------------------------------------------------------------------------
NSFseMatInfo::NSFseMatInfo()
{
}

//---------------------------------------------------------------------------//  Description:	Constructeur à partir d'un NSFseCCAM
//---------------------------------------------------------------------------
NSFseMatInfo::NSFseMatInfo(const NSFseMat* pFseMat)
{
  if (pFseMat)
    _Donnees = pFseMat->_Donnees ;
}

//---------------------------------------------------------------------------//  Constructeur copie
//---------------------------------------------------------------------------
NSFseMatInfo::NSFseMatInfo(const NSFseMatInfo& rv)
{
  _Donnees = rv._Donnees ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSFseMatInfo::~NSFseMatInfo()
{
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSFseMatInfo&
NSFseMatInfo::operator=(const NSFseMatInfo& src)
{
  if (this == &src)
  	return *this ;

  _Donnees = src._Donnees ;

	return *this ;
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSFseMatInfo::operator==(const NSFseMatInfo& o)
{
	 return (_Donnees == o._Donnees) ;
}
voidNSFseMatInfo::initFromProduitInfo(const NSLotInfo* pLotInfo){  if ((NSLotInfo*) NULL == pLotInfo)    return ;	strcpy(_Donnees._code,      pLotInfo->getCode().c_str()) ;	strcpy(_Donnees._montant_e, pLotInfo->getPrixVenteE().c_str()) ;  strcpy(_Donnees._montant_f, pLotInfo->getPrixVenteF().c_str()) ;}//***************************************************************************// Implémentation des méthodes NSCodPrest
//***************************************************************************

//---------------------------------------------------------------------------//  Constructeur
//---------------------------------------------------------------------------
NSCodPrestData::NSCodPrestData()
{
	//
	// Met les champs de données à zéro
	//
	metAZero();
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSCodPrestData::NSCodPrestData(const NSCodPrestData& rv)
{
	strcpy(code,		rv.code);
  strcpy(libelle,	rv.libelle);
  strcpy(fse16xx,	rv.fse16xx);
  strcpy(prix_f,	rv.prix_f);
  strcpy(prix_e,	rv.prix_e);
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
NSCodPrestData::~NSCodPrestData()
{
}

//---------------------------------------------------------------------------
//  Opérateur =
//---------------------------------------------------------------------------
NSCodPrestData&
NSCodPrestData::operator=(const NSCodPrestData& src)
{
  if (this == &src)
  	return *this ;

	strcpy(code,		src.code);  strcpy(libelle,	src.libelle);
  strcpy(fse16xx,	src.fse16xx);
  strcpy(prix_f,	src.prix_f);
  strcpy(prix_e,	src.prix_e);

	return *this ;
}

//---------------------------------------------------------------------------//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSCodPrestData::operator == ( const NSCodPrestData& o )
{
	if ((strcmp(code, o.code) == 0))
		return 1;
	else
		return 0;
}

//---------------------------------------------------------------------------//  Met à zéro les variables de la fiche
//---------------------------------------------------------------------------
void
NSCodPrestData::metAZero()
{
	//
	// Met les champs de données à zéro
	//
  memset(code,			0, CODPREST_CODE_LEN + 1);
  memset(libelle,		0, CODPREST_LIBELLE_LEN + 1);
  memset(fse16xx,		0, CODPREST_FSE16XX_LEN + 1);
  memset(prix_f,		0, CODPREST_PRIX_F_LEN + 1);
  memset(prix_e,		0, CODPREST_PRIX_E_LEN + 1);
}

//---------------------------------------------------------------------------//  Constructeur
//---------------------------------------------------------------------------
NSCodPrest::NSCodPrest(NSSuper* pSuper)
           :NSFiche(pSuper)
{
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSCodPrest::NSCodPrest(const NSCodPrest& rv)
           :NSFiche(rv._pSuper)
{
	_Donnees = rv._Donnees ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSCodPrest::~NSCodPrest()
{
}

//---------------------------------------------------------------------------
//  Fonction :  	NSCodPrest::alimenteFiche()
//
//  Description : Transfère le contenu de pRecBuff dans les variables de
//             	la fiche
//---------------------------------------------------------------------------
void
NSCodPrest::alimenteFiche()
{
  alimenteChamp(_Donnees.code,    CODPREST_CODE_FIELD,    CODPREST_CODE_LEN) ;
  alimenteChamp(_Donnees.libelle, CODPREST_LIBELLE_FIELD, CODPREST_LIBELLE_LEN) ;
  alimenteChamp(_Donnees.fse16xx, CODPREST_FSE16XX_FIELD, CODPREST_FSE16XX_LEN) ;
  alimenteChamp(_Donnees.prix_f,  CODPREST_PRIX_F_FIELD,  CODPREST_PRIX_F_LEN) ;
  alimenteChamp(_Donnees.prix_e,  CODPREST_PRIX_E_FIELD,  CODPREST_PRIX_E_LEN) ;
}

//---------------------------------------------------------------------------//  Fonction :   	NSCodPrest::videFiche()
//
//  Description : Transfère le contenu des valeurs de la fiche dans pRecBuff
//---------------------------------------------------------------------------
void
NSCodPrest::videFiche()
{
	videChamp(_Donnees.code,    CODPREST_CODE_FIELD,    CODPREST_CODE_LEN) ;
	videChamp(_Donnees.libelle, CODPREST_LIBELLE_FIELD, CODPREST_LIBELLE_LEN) ;
	videChamp(_Donnees.fse16xx, CODPREST_FSE16XX_FIELD, CODPREST_FSE16XX_LEN) ;
	videChamp(_Donnees.prix_f,  CODPREST_PRIX_F_FIELD,  CODPREST_PRIX_F_LEN) ;
	videChamp(_Donnees.prix_e,  CODPREST_PRIX_E_FIELD,  CODPREST_PRIX_E_LEN) ;
}

//---------------------------------------------------------------------------
//  Fonction :  	NSCodPrest::open()
//
//  Description :	Ouvre la table primaire et les index secondaires
//
//  Returns :		PXERR_, s'il y a lieu
//---------------------------------------------------------------------------
DBIResult
NSCodPrest::open()
{
	char tableName[] = "CODPREST.DB" ;
	//
	// Appelle la fonction open() de la classe de base pour ouvrir
	// l'index primaire
	//
	lastError = NSFiche::open(tableName, NSF_COMPTA) ;
	return(lastError) ;
}

//---------------------------------------------------------------------------
//  Function:  NSCodPrest::Create()
//---------------------------------------------------------------------------
bool
NSCodPrest::Create()
{
	return true ;
}

//---------------------------------------------------------------------------
//  Function:  NSCodPrest::Modify()
//---------------------------------------------------------------------------
bool
NSCodPrest::Modify()
{
	return true ;
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSCodPrest&
NSCodPrest::operator=(const NSCodPrest& src)
{
  if (this == &src)
  	return *this ;

  _Donnees = src._Donnees ;

	return *this ;
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSCodPrest::operator == (const NSCodPrest& o)
{
	return (_Donnees == o._Donnees) ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSCodPrestInfo::NSCodPrestInfo()
//
//  Description:	Constructeur par défaut
//---------------------------------------------------------------------------
NSCodPrestInfo::NSCodPrestInfo()
{
}

//---------------------------------------------------------------------------
//  Fonction:		NSCodPrestInfo::NSCodPrestInfo(NSCodPrest*)
//
//  Description:	Constructeur à partir d'un NSCodPrest
//---------------------------------------------------------------------------
NSCodPrestInfo::NSCodPrestInfo(const NSCodPrest* pCodPrest)
{
  if (pCodPrest)
    _Donnees = pCodPrest->_Donnees ;
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSCodPrestInfo::NSCodPrestInfo(const NSCodPrestInfo& rv)
{
	_Donnees = rv._Donnees ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSCodPrestInfo::~NSCodPrestInfo()
{
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSCodPrestInfo&
NSCodPrestInfo::operator=(const NSCodPrestInfo& src)
{
  if (this == &src)
  	return *this ;

	_Donnees = src._Donnees ;

	return *this ;
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSCodPrestInfo::operator == (const NSCodPrestInfo& o)
{
	return (_Donnees == o._Donnees) ;
}

#ifndef _ENTERPRISE_DLL
// **************************************************************************
//  Méthodes de LocalActsManagementDialog
// **************************************************************************
DEFINE_RESPONSE_TABLE1(NgapActsManagementDialog, TDialog)
	EV_CHILD_NOTIFY_AND_CODE(IDC_LOTS, LBN_DBLCLK, CmMaterielDblClk),
  EV_LVN_GETDISPINFO(IDC_LOTS, LvnGetDispInfo),
  EV_LVN_COLUMNCLICK(IDC_LOTS, LVNColumnclick),
  EV_WM_SIZE,
  EV_COMMAND(IDOK,   CmOk),
  EV_COMMAND(ID_NEW, CmNewAct),
END_RESPONSE_TABLE;

NgapActsManagementDialog::NgapActsManagementDialog(TWindow* pere, NSContexte* pCtx)
                         :TDialog(pere, "IDD_NGAP_MGMT", pNSResModule), NSRoot(pCtx)
{
try
{
	_pMatBox 	        = new NSMngtListeNgapActsWin(this, IDC_CIM10BOX) ;
  _bNaturallySorted = true ;
	_iSortedColumn    = 0 ;
}
catch (...)
{
	erreur("Exception ChoixLotsDialog ctor.", standardError, 0) ;
}
}

NgapActsManagementDialog::~NgapActsManagementDialog(){
	delete _pMatBox ;
}

//---------------------------------------------------------------------------//  Initialise la boite de liste des codes CCAM
//---------------------------------------------------------------------------
void
NgapActsManagementDialog::SetupWindow()
{
try
{
	TDialog::SetupWindow() ;

  InitListe() ;

  if ((false == InitActsArray()) || (_aActsArray.empty()))
		return ;

	AfficheListe() ;

  _pMatBox->SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED) ;
}
catch (...)
{
	erreur("Exception NgapActsManagementDialog::SetupWindow.", standardError, 0) ;
}
}

void
NgapActsManagementDialog::EvSize(uint sizeType, ClassLib::TSize& size)
{
	TDialog::EvSize(sizeType, size) ;

	if (sizeType != SIZE_MINIMIZED)
	{
  	int LEFT_OFFSET  = 8 ;
    int TOP_OFFSET   = 8 ;
    int WIDTH_OFFSET = 24 ;

		ClassLib::TRect rect      = GetWindowRect() ;
    ClassLib::TRect list_rect = _pMatBox->GetWindowRect() ;
		_pMatBox->SetWindowPos(0, LEFT_OFFSET, TOP_OFFSET,
                              rect.Width() - WIDTH_OFFSET, list_rect.Height(),
                              SWP_NOZORDER ) ;
	}
}

bool
NgapActsManagementDialog::InitActsArray()
{
try
{
	_aActsArray.vider() ;

	//
	// Remplissage de la liste avec les codes correspondant à l'examen
	//
	NSCodPrest Base(pContexte->getSuperviseur()) ;
	//
	// Ouverture du fichier
	//
	Base.lastError = Base.open() ;
	if (DBIERR_NONE != Base.lastError)
	{
		erreur("Erreur à l'ouverture du fichier des codes prestation.", standardError, Base.lastError) ;
		return false ;
	}
	//
	// Recherche des codes correspondant au module
	//
	Base.lastError = Base.debut(dbiWRITELOCK) ;

  // si la table est vide debut() renvoie DBIERR_EOF
  if (DBIERR_EOF == Base.lastError)
  {
    erreur("La table des prestations est vide.", standardError, Base.lastError) ;
		Base.close() ;
		return false ;
  }

  if (DBIERR_NONE != Base.lastError)
	{
		erreur("La table des prestations est endommagée.", standardError, Base.lastError) ;
		Base.close() ;
		return false ;
	}

	Base.lastError = Base.getRecord();
	if (DBIERR_NONE != Base.lastError)
	{
		erreur("Erreur à la lecture de la 1ère prestation.", standardError, Base.lastError) ;
		Base.close() ;
		return false ;
	}

	string sLibelleAffiche = string("") ;

	while (DBIERR_NONE == Base.lastError)
	{
		// pMatBox->AddString(sLibelleAffiche.c_str());
		_aActsArray.push_back(new NSCodPrestInfo(&Base)) ;

		//
		// Prise du code suivant
		//
		Base.lastError = Base.suivant(dbiWRITELOCK) ;
		if ((DBIERR_NONE != Base.lastError) && (DBIERR_EOF != Base.lastError))
			erreur("Erreur à l'accès à l'acte libre suivant.", standardError, Base.lastError) ;
		else if (DBIERR_NONE == Base.lastError)
		{
			Base.lastError = Base.getRecord() ;
			if (DBIERR_NONE != Base.lastError)
				erreur("Erreur à la lecture de l'acte libre.", standardError, Base.lastError) ;
		}
	}
	Base.close() ;

	return true ;
}
catch (...)
{
	erreur("Exception NgapActsManagementDialog::InitActsArray.", standardError, 0) ;
  return false ;
}
}

//---------------------------------------------------------------------------
//  Réponse au double click
//  Sélectionne un matériel et effectue CmOk
//---------------------------------------------------------------------------
void
NgapActsManagementDialog::CmMaterielDblClk(WPARAM /* Cmd */)
{
  int iCodeChoisi = _pMatBox->IndexItemSelect() ;
  if (iCodeChoisi < 0)
    return ;

  NSCodPrestInfo *pSelectedAct = _aActsArray[iCodeChoisi] ;

  EditNgapDialog actDlg(this, pContexte, pSelectedAct, this, false) ;
  if ((actDlg.Execute()) != IDOK)
   	return ;

  updateAct(pSelectedAct) ;

  AfficheListe() ;
}

void
NgapActsManagementDialog::CmOk()
{
	TDialog::CmOk() ;
}

void
NgapActsManagementDialog::CmNewAct()
{
  NSCodPrestInfo newActInfo ;

  EditNgapDialog actDlg(this, pContexte, &newActInfo, this, true) ;
  if ((actDlg.Execute()) != IDOK)
   	return ;

  if (false == createAct(&newActInfo))
    return ;

  _aActsArray.push_back(new NSCodPrestInfo(newActInfo)) ;
  AfficheListe() ;
}

void
NgapActsManagementDialog::CmNewActFrom()
{
  int iSel = _pMatBox->IndexItemSelect() ;
  if (iSel < 0)
    return ;

  NSCodPrestInfo newActInfo(*(_aActsArray[iSel])) ;
  newActInfo.setCode(string("")) ;

  EditNgapDialog actDlg(this, pContexte, &newActInfo, this, true) ;
  if ((actDlg.Execute()) != IDOK)
   	return ;

  if (false == createAct(&newActInfo))
    return ;

  _aActsArray.push_back(new NSCodPrestInfo(newActInfo)) ;
  AfficheListe() ;
}

bool
NgapActsManagementDialog::createAct(NSCodPrestInfo* pActInfo)
{
  if ((NSCodPrestInfo*) NULL == pActInfo)
    return false ;

  NSCodPrest Base(pContexte->getSuperviseur()) ;

  Base.lastError = Base.open() ;
	if (DBIERR_NONE != Base.lastError)
	{
		erreur("Erreur à l'ouverture de la base des codes prestation.", standardError, Base.lastError, pContexte->GetMainWindow()->GetHandle()) ;
		return false ;
	}

  pActInfo->initCodPrest(Base) ;

  Base.lastError = Base.appendRecord() ;
  if (DBIERR_NONE != Base.lastError)
  {
  	erreur("Erreur à l'ajout dans la base des codes prestation.", standardError, Base.lastError, pContexte->GetMainWindow()->GetHandle()) ;
		return false ;
	}

  Base.lastError = Base.close() ;
	if (DBIERR_NONE != Base.lastError)
		erreur("Erreur à la fermeture de la base des codes prestation.", standardError, Base.lastError, pContexte->GetMainWindow()->GetHandle()) ;

  return true ;
}

bool
NgapActsManagementDialog::updateAct(NSCodPrestInfo* pActInfo)
{
  if ((NSCodPrestInfo*) NULL == pActInfo)
    return false ;

  if (string("") == pActInfo->getCode())
    return false ;

  NSCodPrest Base(pContexte->getSuperviseur()) ;

  Base.lastError = Base.open() ;
	if (DBIERR_NONE != Base.lastError)
	{
		erreur("Erreur à l'ouverture de la base des prestations.", standardError, Base.lastError, pContexte->GetMainWindow()->GetHandle()) ;
		return false ;
	}

  string sNum = pActInfo->getCode() ;

	Base.lastError = Base.chercheClef(&sNum,
                                    "",
                                    0,
                                    keySEARCHEQ,
                                    dbiWRITELOCK) ;

	if (DBIERR_NONE != Base.lastError)
	{
		erreur("Erreur à la recherche de la fiche de prestation.", standardError, Base.lastError, pContexte->GetMainWindow()->GetHandle()) ;
		Base.close() ;
		return false ;
	}

	Base.lastError = Base.getRecord() ;
	if (DBIERR_NONE != Base.lastError)
	{
		erreur("Erreur à lecture du fichier des prestations", standardError, Base.lastError, pContexte->GetMainWindow()->GetHandle()) ;
		Base.close() ;
		return false ;
	}

  pActInfo->initCodPrest(Base) ;

	Base.lastError = Base.modifyRecord(TRUE) ;
	if (DBIERR_NONE != Base.lastError)
	{
		erreur("Erreur à la modification de la fiche de prestation.", standardError, Base.lastError, pContexte->GetMainWindow()->GetHandle()) ;
		Base.close() ;
		return false ;
	}

	Base.lastError = Base.close() ;
	if (DBIERR_NONE != Base.lastError)
		erreur("Erreur à la fermeture de la base des prestations.", standardError, Base.lastError, pContexte->GetMainWindow()->GetHandle()) ;

	return true ;
}

bool
NgapActsManagementDialog::existAct(string sCode)
{
  NSCodPrest Base(pContexte->getSuperviseur()) ;

  Base.lastError = Base.open() ;
	if (DBIERR_NONE != Base.lastError)
	{
		erreur("Erreur à l'ouverture de la base des prestations.", standardError, Base.lastError, pContexte->GetMainWindow()->GetHandle()) ;
		return false ;
	}

	Base.lastError = Base.chercheClef(&sCode,
                                    "",
                                    0,
                                    keySEARCHEQ,
                                    dbiWRITELOCK) ;

	if (DBIERR_NONE != Base.lastError)
	{
		Base.close() ;
		return false ;
	}

  Base.lastError = Base.getRecord() ;
	if (DBIERR_NONE != Base.lastError)
	{
		erreur("Erreur à lecture du fichier des prestations", standardError, Base.lastError, pContexte->GetMainWindow()->GetHandle()) ;
		Base.close() ;
		return false ;
	}

  Base.close() ;

  if (Base.getCode() == sCode)
    return true ;

  return false ;
}

void
NgapActsManagementDialog::InitListe()
{
  TListWindColumn colCode("Code", 60, TListWindColumn::Left, 0) ;
	_pMatBox->InsertColumn(0, colCode) ;
	TListWindColumn colLibelle("Libelle", 400, TListWindColumn::Left, 1) ;
	_pMatBox->InsertColumn(1, colLibelle) ;
	TListWindColumn colPrixE("Montant €", 90, TListWindColumn::Right, 3) ;
	_pMatBox->InsertColumn(3, colPrixE) ;
  string sSigle = string("Montant ") + pContexte->getSuperviseur()->getSigle() ;
  TListWindColumn colPrixF((char*) sSigle.c_str(), 90, TListWindColumn::Right, 4) ;
	_pMatBox->InsertColumn(4, colPrixF) ;
}

void
NgapActsManagementDialog::AfficheListe()
{
	_pMatBox->DeleteAllItems() ;

  if (_aActsArray.empty())
    return ;

	for (NSCodPrestReverseIter i = _aActsArray.rbegin() ; _aActsArray.rend() != i ; i++)
	{
    string sCode = (*i)->getCode() ;
    TListWindItem Item(sCode.c_str(), 0) ;
    _pMatBox->InsertItem(Item) ;
	}
}

void
NgapActsManagementDialog::LvnGetDispInfo(TLwDispInfoNotify& dispInfo)
{
	TListWindItem& dispInfoItem = *(TListWindItem*)&dispInfo.item ;
	int index = dispInfoItem.GetIndex() ;
  if (index < 0)
		return ;

  if ((size_t)index > _aActsArray.size())
  	return ;

  string sBuffer = string("") ;

  NSCodPrestInfo* pInfo = _aActsArray[index] ;
  if ((NSCodPrestInfo*) NULL == pInfo)
    return ;

	// Affiche les informations en fonction de la colonne

	switch (dispInfoItem.GetSubItem())
	{
		case 1 : // Label

    	sBuffer = pInfo->getLabel() ;
      break ;

		case 2 : // Montant Euros

    	sBuffer = NSBlocFse16::GetSumInUnits(pInfo->getPriceE()) ;
      break ;

    case 3 : // Montant seconde monnaie

    	sBuffer = NSBlocFse16::GetSumInUnits(pInfo->getPriceF()) ;
      break ;
	}

  static char buffer[1024] ;
  strcpy(buffer, sBuffer.c_str()) ;
  dispInfoItem.SetText(buffer) ;
}

bool
NgapSortByCodeInf(NSCodPrestInfo *pCCAM1, NSCodPrestInfo *pCCAM2)
{
	return (pCCAM1->getCode() < pCCAM2->getCode()) ;
}
bool
NgapSortByCodeSup(NSCodPrestInfo *pCCAM1, NSCodPrestInfo *pCCAM2)
{
	return (pCCAM1->getCode() > pCCAM2->getCode()) ;
}

bool
NgapSortByLibelleInf(NSCodPrestInfo *pCCAM1, NSCodPrestInfo *pCCAM2)
{
	return (pseumaj(pCCAM1->getLabel()) < pseumaj(pCCAM2->getLabel())) ;
}
bool
NgapSortByLibelleSup(NSCodPrestInfo *pCCAM1, NSCodPrestInfo *pCCAM2)
{
	return (pseumaj(pCCAM1->getLabel()) > pseumaj(pCCAM2->getLabel())) ;
}

void
NgapActsManagementDialog::LVNColumnclick(TLwNotify& lwn)
{
  switch (lwn.iSubItem)
  {
    case 0  : sortByCode() ;      break ;
    case 1  : sortByLibelle() ;   break ;
  }
}

void
NgapActsManagementDialog::swapSortOrder(int iColumn)
{
  if (_iSortedColumn == iColumn)
  {
    if (_bNaturallySorted)
      _bNaturallySorted = false ;
    else
      _bNaturallySorted = true ;
  }
  else
  {
    _iSortedColumn    = iColumn ;
    _bNaturallySorted = true ;
  }
}

void
NgapActsManagementDialog::sortByCode()
{
	swapSortOrder(0) ;

  if (_aActsArray.empty())
    return ;

  if (_bNaturallySorted)
    sort(_aActsArray.begin(), _aActsArray.end(), NgapSortByCodeInf) ;
  else
    sort(_aActsArray.begin(), _aActsArray.end(), NgapSortByCodeSup) ;

  AfficheListe() ;
}

void
NgapActsManagementDialog::sortByLibelle()
{
	swapSortOrder(1) ;

  if (_aActsArray.empty())
    return ;

  if (_bNaturallySorted)
    sort(_aActsArray.begin(), _aActsArray.end(), NgapSortByLibelleInf) ;
  else
    sort(_aActsArray.begin(), _aActsArray.end(), NgapSortByLibelleSup) ;

  AfficheListe() ;
}

#endif // #ifndef _ENTERPRISE_DLL

#ifndef _ENTERPRISE_DLL
// -----------------------------------------------------------------
//
//  Méthodes de NSListeMatWindow
//
// -----------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSMngtListeNgapActsWin, TListWindow)
	EV_WM_LBUTTONDBLCLK,
  EV_WM_RBUTTONDOWN,
  EV_COMMAND(CM_EDIT_ACT,     CmEditAct),
  EV_COMMAND(CM_NEW_ACT,      CmNewAct),
  EV_COMMAND(CM_NEW_ACT_FROM, CmNewActFrom),
END_RESPONSE_TABLE;

void
NSMngtListeNgapActsWin::SetupWindow()
{
  ListView_SetExtendedListViewStyle(this->HWindow, LVS_EX_FULLROWSELECT) ;
  TListWindow::SetupWindow() ;
}

//---------------------------------------------------------------------------
//  Description: Fonction de réponse au double-click
//---------------------------------------------------------------------------
void
NSMngtListeNgapActsWin::EvLButtonDblClk(uint /* modKeys */, NS_CLASSLIB::TPoint& point)
{
	TLwHitTestInfo info(point) ;

	HitTest(info) ;

  if (info.GetFlags() & LVHT_ONITEM)
  	pDlg->CmMaterielDblClk((WPARAM) 0) ;
}

void
NSMngtListeNgapActsWin::EvRButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point)
{
	TLwHitTestInfo info(point) ;

  // création d'un menu popup
  NS_CLASSLIB::TPoint lp = point ;

  OWL::TPopupMenu menu ;

  NSSuper* pSuper = pDlg->pContexte->getSuperviseur() ;

  string sNew = pSuper->getText("accountingLocalActs", "newLocalAct") ;
  menu.AppendMenu(MF_STRING, CM_NEW_ACT, sNew.c_str()) ;

  int indexItem = HitTest(info) ;
  if (info.GetFlags() & LVHT_ONITEM)  {  	SetItemState(indexItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED) ;    Invalidate() ;		string sEdit    = pSuper->getText("accountingLocalActs", "editThisLocalAct") ;		string sNewFrom = pSuper->getText("accountingLocalActs", "newLocalActFromThisOne") ;

    menu.AppendMenu(MF_STRING, CM_NEW_ACT_FROM, sNewFrom.c_str()) ;
    menu.AppendMenu(MF_SEPARATOR, 0, 0) ;
    menu.AppendMenu(MF_STRING, CM_EDIT_ACT, sEdit.c_str()) ;
  }

  ClientToScreen(lp) ;
  menu.TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, lp, 0, HWindow) ;
  TListWindow::EvRButtonDown(modkeys, point) ;
}

void
NSMngtListeNgapActsWin::CmNewAct()
{
  pDlg->CmNewAct() ;
}

void
NSMngtListeNgapActsWin::CmNewActFrom()
{
  pDlg->CmNewActFrom() ;
}

void
NSMngtListeNgapActsWin::CmEditAct()
{
  pDlg->CmMaterielDblClk((WPARAM) 0) ;
}

//---------------------------------------------------------------------------
//  Description: Retourne l'index du premier item sélectionné
//  Returns:     index si item est sélectionné, -1 sinon
//---------------------------------------------------------------------------
int
NSMngtListeNgapActsWin::IndexItemSelect()
{
	int count = GetItemCount() ;
	int index = -1 ;

	for (int i = 0; i < count; i++)
		if (GetItemState(i, LVIS_SELECTED))
    {
    	index = i ;
      break ;
    }

	return index ;
}
#endif // #ifndef _ENTERPRISE_DLL

// -----------------------------------------------------------------
//
//  Méthodes de EditLotDialog
//
// -----------------------------------------------------------------

// ---------------------------------------------------------------------------

#ifndef _ENTERPRISE_DLL
DEFINE_RESPONSE_TABLE1(EditNgapDialog, NSUtilDialog)	EV_COMMAND(IDOK, CmOk),
	EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE;

EditNgapDialog::EditNgapDialog(TWindow* pere, NSContexte* pCtx, NSCodPrestInfo* pActInfo, NgapActsManagementDialog* pListMgmtDlg, bool bIsNew)
               :NSUtilDialog(pere, pCtx, "IDD_NGAP_EDIT", pNSResModule)
{
	// Initialisation des donnees
  _pListMgmtDlg = pListMgmtDlg ;
  _pInfo        = pActInfo ;  _bIsNew       = bIsNew ;	// Création de tous les "objets de contrôle"
  _pCode      = new NSUtilEdit2(pContexte, this, IDC_NGAP_CODE, CLAM_CODE_LEN) ;
  _pLabel     = new NSUtilEdit2(pContexte, this, IDC_NGAP_LABEL, CLAM_LIBELLE_LEN) ;
  _pMontantE  = new NSUtilEditSomme(pContexte, this, IDC_NGAP_PRIX_E) ;
  _pMontantF  = new NSUtilEditSomme(pContexte, this, IDC_NGAP_PRIX_F) ;
  _pCategory  = new TComboBox(this, IDC_NGAP_CAT) ;
  _pSigle		  = new TStatic(this, IDC_LOT_SIGLE) ;
}
EditNgapDialog::~EditNgapDialog(){
  delete _pLabel ;
  delete _pCode ;
  delete _pMontantE	;
	delete _pMontantF ;
  delete _pCategory ;
  delete _pSigle ;
}
void
EditNgapDialog::SetupWindow()
{
	NSUtilDialog::SetupWindow() ;  _pCode->SetText(_pInfo->getCode()) ;  _pLabel->SetText(_pInfo->getLabel()) ;  string sSigle = pContexte->getSuperviseur()->getSigle() ;
	_pSigle->SetText(sSigle.c_str()) ;

  _pMontantE->setSomme(_pInfo->getPriceE()) ;
  _pMontantF->setSomme(_pInfo->getPriceF()) ;

  // Initialize categories
  //
  for (int i = 0 ; i < 3 ; i++)
    _pCategory->AddString(getCategoryFromIndex(i).c_str()) ;

  int iIndex = getIndexFromCategory(_pInfo->getFse16xx()) ;
  _pCategory->SetSelIndex(iIndex) ;
}

voidEditNgapDialog::CmOk(){  string sText ;  _pCode->GetText(sText) ;  if (_bIsNew && _pListMgmtDlg->existAct(sText))  {    string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName() ;    int ret = MessageBox("Ce code d'acte existe déjà dans la base.", sCaption.c_str(), MB_OK) ;    return ;  }	_pInfo->setCode(sText) ;  _pLabel->GetText(sText) ;	_pInfo->setLabel(sText) ;
	_pMontantE->getSomme(&sText) ;
	_pInfo->setPriceE(sText) ;
  _pMontantF->getSomme(&sText) ;
	_pInfo->setPriceF(sText) ;

  int indexCat = _pCategory->GetSelIndex() ;
  switch(indexCat)
  {
    case 0  : _pInfo->setFse16xx(string("1610")) ; break ;
    case 1  : _pInfo->setFse16xx(string("1620")) ; break ;
    case 2  : _pInfo->setFse16xx(string("1630")) ; break ;
    default : _pInfo->setFse16xx(string("1610")) ; break ;
  }

	NSUtilDialog::CmOk() ;
}
voidEditNgapDialog::CmCancel(){	NSUtilDialog::CmCancel() ;}intEditNgapDialog::getIndexFromCategory(const string sCategory) const{  if (string("1610") == sCategory)    return 0 ;  if (string("1620") == sCategory)    return 1 ;  if (string("1630") == sCategory)    return 2 ;  return 0 ;}stringEditNgapDialog::getCategoryFromIndex(int iIndex) const{  switch(iIndex)  {
    case 0  : return string("1610") ;
    case 1  : return string("1620") ;
    case 2  : return string("1630") ;
  }

  return string("1610") ;
}#endif // #ifndef _ENTERPRISE_DLL

//////////////////////////// fin du fichier NSFSE16.cpp

