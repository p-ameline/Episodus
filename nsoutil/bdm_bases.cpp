// -----------------------------------------------------------------------------// bdm_bases.cpp// Base de donnees medicamenteuse
// -----------------------------------------------------------------------------
// $Revision: 1.4 $
// $Author: fabrice $
// $Date: 2003/07/09 15:40:00 $
// -----------------------------------------------------------------------------
// FLP - mai/juin/juillet 2003
// PA - février 2003
// -----------------------------------------------------------------------------

#include <utility.h>#include <mem.h>
#include <string.h>
#include <cstring.h>

#include "partage\nsdivfct.h"
#include "nautilus\nssuper.h"
#include "nsoutil\bdm_bases.h"

//***************************************************************************
// Implémentation des méthodes NSBdmData
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NSBdmData::NSBdmData()
{
  metAZero() ;
}


//---------------------------------------------------------------------------//  Met les champs de données à zéro
//---------------------------------------------------------------------------
void
NSBdmData::metAZero()
{
	memset(id,					0, BDM_ID_LEN           + 1) ;
	memset(codeCIP,     0, BDM_CIP_LEN          + 1) ;
	memset(extension,   0, BDM_EXT_LEN          + 1) ;
	memset(leximed,     0, BDM_LEXIMED_LEN      + 1) ;
	memset(codeVidal,   0, BDM_VIDAL_LEN        + 1) ;
	memset(leximedOld,	0, BDM_LEXOLD_LEN       + 1) ;
  memset(generiqGrpe,	0, BDM_GRP_GENERIQ_LEN  + 1) ;
  memset(generiqType,	0, BDM_TYPE_GENERIQ_LEN + 1) ;
}

//---------------------------------------------------------------------------//  Constructeur copie
//---------------------------------------------------------------------------
NSBdmData::NSBdmData(const NSBdmData& rv)
{
	strcpy(id,          rv.id) ;
	strcpy(codeCIP,     rv.codeCIP) ;
	strcpy(extension,   rv.extension) ;
	strcpy(leximed,     rv.leximed) ;
	strcpy(codeVidal,   rv.codeVidal) ;
	strcpy(leximedOld,  rv.leximedOld) ;
  strcpy(generiqGrpe, rv.generiqGrpe) ;
  strcpy(generiqType, rv.generiqType) ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSBdmData::~NSBdmData()
{
}

//---------------------------------------------------------------------------//  Opérateur =
//---------------------------------------------------------------------------
NSBdmData&
NSBdmData::operator=(const NSBdmData& rv)
{
  if (this == &rv)
  	return *this ;

	strcpy(id,          rv.id) ;
	strcpy(codeCIP,     rv.codeCIP) ;
	strcpy(extension,   rv.extension) ;
	strcpy(leximed,     rv.leximed) ;
	strcpy(codeVidal,   rv.codeVidal) ;
	strcpy(leximedOld,  rv.leximedOld) ;
  strcpy(generiqGrpe, rv.generiqGrpe) ;
  strcpy(generiqType, rv.generiqType) ;

	return *this ;
}

//---------------------------------------------------------------------------//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSBdmData::operator==(const NSBdmData& o)
{
	if ((strcmp(id,					 o.id)					== 0) &&
			(strcmp(codeCIP,     o.codeCIP)     == 0) &&
			(strcmp(extension,   o.extension)   == 0) &&
			(strcmp(leximed,     o.leximed)     == 0) &&
			(strcmp(codeVidal,   o.codeVidal)   == 0) &&
      (strcmp(leximedOld,  o.leximedOld)  == 0) &&
      (strcmp(generiqGrpe, o.generiqGrpe) == 0) &&
			(strcmp(generiqType, o.generiqType) == 0))
		return 1 ;
	else
		return 0 ;
}

//***************************************************************************// Implémentation des méthodes NSBdm
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NSBdm::NSBdm(NSContexte* pCtx)
      :NSBdmInfo(),
       NSFiche(pCtx->getSuperviseur())
{
}

//---------------------------------------------------------------------------
//  Destructeur.
//---------------------------------------------------------------------------
NSBdm::~NSBdm()
{
}

//---------------------------------------------------------------------------//  Transfère le contenu de pRecBuff dans les variables de la fiche
//---------------------------------------------------------------------------
void
NSBdm::alimenteFiche()
{
	alimenteChamp(Donnees.id,					 BDM_ID_FIELD,			     BDM_ID_LEN) ;
	alimenteChamp(Donnees.codeCIP,     BDM_CIP_FIELD, 	       BDM_CIP_LEN) ;
	alimenteChamp(Donnees.extension,   BDM_EXT_FIELD, 		     BDM_EXT_LEN) ;
	alimenteChamp(Donnees.leximed,     BDM_LEXIMED_FIELD,      BDM_LEXIMED_LEN) ;
	alimenteChamp(Donnees.codeVidal,   BDM_VIDAL_FIELD, 	     BDM_VIDAL_LEN) ;
	alimenteChamp(Donnees.leximedOld,  BDM_LEXOLD_FIELD, 	     BDM_LEXOLD_LEN) ;
  alimenteChamp(Donnees.generiqGrpe, BDM_GRP_GENERIQ_FIELD,  BDM_GRP_GENERIQ_LEN) ;
  alimenteChamp(Donnees.generiqType, BDM_TYPE_GENERIQ_FIELD, BDM_TYPE_GENERIQ_LEN) ;
}

//---------------------------------------------------------------------------//  Transfère le contenu des valeurs de la fiche dans pRecBuff
//---------------------------------------------------------------------------
void
NSBdm::videFiche()
{
	videChamp(Donnees.id,					 BDM_ID_FIELD,           BDM_ID_LEN) ;
	videChamp(Donnees.codeCIP,     BDM_CIP_FIELD,          BDM_CIP_LEN) ;
	videChamp(Donnees.extension,   BDM_EXT_FIELD,          BDM_EXT_LEN) ;
	videChamp(Donnees.leximed,     BDM_LEXIMED_FIELD,      BDM_LEXIMED_LEN) ;
	videChamp(Donnees.codeVidal,   BDM_VIDAL_FIELD,        BDM_VIDAL_LEN) ;
	videChamp(Donnees.leximedOld,  BDM_LEXOLD_FIELD,       BDM_LEXOLD_LEN) ;
  videChamp(Donnees.generiqGrpe, BDM_GRP_GENERIQ_FIELD,  BDM_GRP_GENERIQ_LEN) ;
  videChamp(Donnees.generiqType, BDM_TYPE_GENERIQ_FIELD, BDM_TYPE_GENERIQ_LEN) ;
}

//---------------------------------------------------------------------------//  Ouvre la table
//---------------------------------------------------------------------------
DBIResult
NSBdm::open()
{
	char tableName[] = "BDM.DB" ;

	// Appelle la fonction open() de la classe de base
	lastError = NSFiche::open(tableName, NSF_PARTAGE_GLOBAL) ;
	return (lastError) ;
}

//***************************************************************************
// Implémentation des méthodes NSBdmInfo
//***************************************************************************

//---------------------------------------------------------------------------//  Constructeur par défaut
//---------------------------------------------------------------------------
NSBdmInfo::NSBdmInfo()
{
}

//---------------------------------------------------------------------------
//  Constructeur à partir d'une NSChemise
//---------------------------------------------------------------------------
NSBdmInfo::NSBdmInfo(NSBdm* pBdm)
{
	// Initialise les données à partir de celles de la source
	Donnees	= pBdm->Donnees ;
}

//---------------------------------------------------------------------------//  Destructeur.
//---------------------------------------------------------------------------
NSBdmInfo::~NSBdmInfo()
{
}

//---------------------------------------------------------------------------//  Constructeur copie
//---------------------------------------------------------------------------
NSBdmInfo::NSBdmInfo(const NSBdmInfo& rv)
{
	// Initialise les données à partir de celles de la source
	Donnees = rv.Donnees ;
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSBdmInfo&
NSBdmInfo::operator=(const NSBdmInfo& src)
{
  if (this == &src)
  	return *this ;

  Donnees = src.Donnees ;

	return *this ;
}

int
NSBdmInfo::operator==(const NSBdmInfo& o)
{
	return (Donnees == o.Donnees) ;
}

// --------------------------------------------------------------------------// -------------------------- METHODES DE NSBdmArray ------------------------
// --------------------------------------------------------------------------
NSBdmArray::NSBdmArray(const NSBdmArray& rv)
           :NSBMArray()
{
try
{
  if (false == rv.empty())
    for (NSBdmConstIter it = rv.begin() ; rv.end() != it ; it++)
      push_back(new NSBdmInfo(**it)) ;
}
catch (...)
{
  erreur("Exception NSBdmArray copy ctor.", standardError, 0) ;
}
}

voidNSBdmArray::vider()
{
	if (empty())
		return ;

	for (NSBdmIter it = begin() ; end() != it ; )	{
		delete *it ;
		erase(it) ;
	}
}

NSBdmArray::~NSBdmArray(){
	vider() ;
}

NSBdmArray&
NSBdmArray::operator=(const NSBdmArray& src)
{
  if (this == &src)
    return *this ;

try
{
  vider() ;

  if (false == src.empty())
    for (NSBdmConstIter it = src.begin() ; src.end() != it ; it++)
      push_back(new NSBdmInfo(**it)) ;
}
catch (...)
{
  erreur("Exception NSBdmArray (op =).", standardError, 0) ;
}
  return (*this) ;
}

