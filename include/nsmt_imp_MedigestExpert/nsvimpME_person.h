//---------------------------------------------------------------------------
//     NSIMPMEPERSON.H
//
//  Contient les définitions des objets PATIENT et UTILISATEUR
//---------------------------------------------------------------------------
#ifndef __NSIMPME_PERSON_H
#define __NSIMPME_PERSON_H

# include <cstring.h>
# include <owl\window.h>

//---------------------------------------------------------------------------
//  Classe NSRecupPatientData
//---------------------------------------------------------------------------
//
class NSRecupPatientData : public NSRoot
{
 public:
	//
	// Variables de stockage des valeurs
	//
	string sID ;
  string sGender ;
  string sNom ;
  string sPrenom ;
  string sMaidenName ;
  string sAdresse ;
  string sZip ;
  string sCity ;
  string sPhone ;
  string sBirthDate ;
  string sCorresp ;

  string sUnknown10 ;
  string sUnknown11 ;
  string sUnknown15 ;
  string sUnknown16 ;
  string sUnknown17 ;

  string sMaritalStatus ;
  string sJob ;
  string sNumSecu ;

	NSRecupPatientData(NSContexte* pCtx) ;
	NSRecupPatientData(const NSRecupPatientData& rv) ;

	NSRecupPatientData& operator=(const NSRecupPatientData& src) ;
	int                 operator==(const NSRecupPatientData& o) ;

	void metAZero();
	bool estMasculin() { return ('1' == sGender[0]) ; }
	bool estFeminin()  { return ('2' == sGender[0]) ; }
	void metMasculin() { sGender = string("1") ; }
	void metFeminin()  { sGender = string("2") ; }

  string getBirthDate() ;
  string getCivility() ;
} ;

//---------------------------------------------------------------------------
//  Classe NSRecupCorrespData
//---------------------------------------------------------------------------
//
class NSRecupCorrespData : public NSRoot
{
 public:
	//
	// Variables de stockage des valeurs
	//
	string sID ;
  string sNom ;
  string sPrenom ;
  string sTitre ;
  string sAdresse ;
  string sZip ;
  string sCity ;
  string sPhone ;
  string sPhone2 ;
  string sSpeciality ;
  string sMail ;
  string sPolitesse ;

  string sComplmt1 ;
  string sComplmt2 ;

	NSRecupCorrespData(NSContexte* pCtx) ;
	NSRecupCorrespData(const NSRecupCorrespData& rv) ;

	NSRecupCorrespData& operator=(const NSRecupCorrespData& src) ;
	int                 operator==(const NSRecupCorrespData& o) ;

	void metAZero() ;

  string getCivility() ;
  string getSpecility() ;
} ;

//---------------------------------------------------------------------------
//  Classe NSRecupDocumentData
//---------------------------------------------------------------------------
//
class NSRecupDocumentData : public NSRoot
{
 public:
	//
	// Variables de stockage des valeurs
	//
	string sID ;
  string sPatientID ;
  string sLabel ;
  string sCreationDate ;
  string sAuthorId ;
  string sType ;
  string sFileName ;

	NSRecupDocumentData(NSContexte* pCtx) ;
	NSRecupDocumentData(const NSRecupDocumentData& rv) ;

	NSRecupDocumentData& operator=(const NSRecupDocumentData& src) ;
	int                  operator==(const NSRecupDocumentData& o) ;

	void metAZero() ;
} ;

//---------------------------------------------------------------------------
#endif    // __NSIMPME_PERSON_H

