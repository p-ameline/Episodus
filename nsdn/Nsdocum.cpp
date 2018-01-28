//---------------------------------------------------------------------------//    NSDOCUM.CPP
//    PA   juillet 94
//  Implémentation des objets document de NAUTILUS
//---------------------------------------------------------------------------

//
// Exigé par la version 5.01 du compilateur
//
// Dans BC5/INCLUDE/OCF/AUTODEFS.H
// #if !defined(_OLECTL_H_)
//		# if defined(__BIVBX_H) && !defined(NO_VBX_PICTURES)
//			#   error BIVBX.H's LPPICTURE is incompatible with OLECTL.H - define NO_VBX_PICTURES
//
#define  NO_VBX_PICTURES

#include <windows.h>
#include <mem.h>

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/superLus.h"
  #include "enterpriseLus/nsdivfctForCgi.h"
#else
  #include "nautilus\nssuper.h"
  #include "partage\nsdivfct.h"
#endif

// #include "nsdn\nsdocdlg.h"

#include "nsdn\nsdocum.h"
#include "nsbb\nspatpat.h"
#include "nsbb\nsbbtran.h"
#include "nssavoir\nscapture.h"

long NSChemiseInfo::lObjectCount = 0 ;
long NSSejourInfo::lObjectCount  = 0 ;
long NSChemDocInfo::lObjectCount = 0 ;
long NSDocStatus::lObjectCount   = 0 ;

//***************************************************************************
// Implémentation des méthodes NSDocument
//
// Les méthodes de NSDocumentData et NSDocumentInfo sont dans NSDOCINF.CPP
//***************************************************************************


//***************************************************************************// Implémentation des méthodes NSChemise
//***************************************************************************

//---------------------------------------------------------------------------
//  Function:		NSChemiseData::metAZero()
//  Description:	Initialise les champs de données à zéro.
//  Retour:			Aucun
//---------------------------------------------------------------------------
void NSChemiseData::metAZero()
{
  _sCode        = string("") ;
  _sNom         = string("") ;
  _sCreation    = string("") ;
  _sAcces       = string("") ;
  _sCreateur    = string("") ;
  _sCommentaire = string("") ;
}

//---------------------------------------------------------------------------
//  Fonction:  	NSChemiseData::donneIntitule(char *intitule)
//  Description:	Renvoie dans une chaine l'intitulé de la chemise
//  Retour:			Rien
//---------------------------------------------------------------------------
string
NSChemiseData::donneIntitule(string sLang)
{
	string sIntitule = _sNom ;
  strip(sIntitule, stripBoth) ;

  if (string("") != _sCreation)
		sIntitule += string(" du ") + donne_date(_sCreation, sLang) ;

  return sIntitule ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSChemiseData::NSChemiseData(NSChemiseData& rv)
//  Description:	Constructeur copie
//  Retour:			Rien
//---------------------------------------------------------------------------
NSChemiseData::NSChemiseData(const NSChemiseData& rv)
{  _sCode        = rv._sCode ;  _sNom         = rv._sNom ;
  _sCreation    = rv._sCreation ;
  _sAcces       = rv._sAcces ;
  _sCreateur    = rv._sCreateur ;
  _sCommentaire = rv._sCommentaire ;}
//---------------------------------------------------------------------------
//  Fonction:		NSChemiseData::operator=(NSChemiseData src)
//  Description:	Opérateur d'affectation
//  Retour:			Rien
//---------------------------------------------------------------------------
NSChemiseData& NSChemiseData::operator=(const NSChemiseData& src)
{
  if (&src == this)
		return *this ;

	_sCode        = src._sCode ;
  _sNom         = src._sNom ;
  _sCreation    = src._sCreation ;
  _sAcces       = src._sAcces ;
  _sCreateur    = src._sCreateur ;
  _sCommentaire = src._sCommentaire ;

	return *this ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSChemiseData::operator==(const NSChemiseData& o)
//  Description:	Opérateur de comparaison
//  Retour:			Rien
//---------------------------------------------------------------------------
int NSChemiseData::operator==(const NSChemiseData& o) const
{
  if ((_sCode        == o._sCode)     &&
      (_sNom         == o._sNom)      &&
      (_sCreation    == o._sCreation) &&
      (_sAcces       == o._sAcces)    &&
      (_sCreateur    == o._sCreateur) &&
      (_sCommentaire == o._sCommentaire))
    return 1;
  else
    return 0;
}

//---------------------------------------------------------------------------//  Fonction:		NSChemiseInfo::NSChemiseInfo()
//  Description:	Constructeur par défaut
//  Retour:			Rien
//---------------------------------------------------------------------------
NSChemiseInfo::NSChemiseInfo()
{
	_sNodeChemise = string("") ;

  lObjectCount++ ;
}

//---------------------------------------------------------------------------
//  Function:		NSChemiseInfo::~NSChemiseInfo()
//  Description:	Destructeur.
//  Retour:			Aucun
//---------------------------------------------------------------------------
NSChemiseInfo::~NSChemiseInfo()
{
  lObjectCount-- ;
}

//---------------------------------------------------------------------------
//  Function:		NSChemiseInfo::donneIntitule(char *intitule)
//  Description:	Donne l'intitulé de la chemise.
//  Retour:			Aucun
//---------------------------------------------------------------------------
string
NSChemiseInfo::donneIntitule(string sLang)
{
	return _Donnees.donneIntitule(sLang) ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSChemiseInfo::NSChemiseInfo(NSChemiseInfo& rv)
//  Description:	Constructeur copie
//  Retour:			Rien
//---------------------------------------------------------------------------
NSChemiseInfo::NSChemiseInfo(const NSChemiseInfo& rv)
{
	//
	// Initialise les données à partir de celles de la source
	//
	_Donnees      = rv._Donnees ;
	_sNodeChemise = rv._sNodeChemise ;

  lObjectCount++ ;
}

//---------------------------------------------------------------------------//  Fonction:		NSChemiseInfo::operator=(NSChemiseInfo src)
//  Description:	Constructeur copie
//  Retour:			Rien
//---------------------------------------------------------------------------
NSChemiseInfo& NSChemiseInfo::operator=(const NSChemiseInfo& src)
{
	if (this == &src)
		return *this ;

	_Donnees      = src._Donnees ;
	_sNodeChemise = src._sNodeChemise ;

	return *this ;
}

int NSChemiseInfo::operator==(const NSChemiseInfo& o) const
{
	 return (_Donnees == o._Donnees) ;
}

bool
NSChemiseArray::initFromPatpatho(NSPatPathoArray *pFoldersPpt)
{
  if ((NSPatPathoArray*) NULL == pFoldersPpt)
    return false ;

  if (pFoldersPpt->empty())
    return true ;

  PatPathoIter iter = pFoldersPpt->begin() ;
  int iColBase = (*iter)->getColonne() ;
  iter++ ;

  while ((pFoldersPpt->end() != iter) && ((*iter)->getColonne() > iColBase))
  {
    string sSens = (*iter)->getLexiqueSens() ;

    if (string("0CHEM") == sSens)
    {
      string sNodeChem = (*iter)->getNode() ;
      string sNom      = string("") ;
      string sDate     = string("") ;

      iter++ ;

      // on charge les données de la chemise
      while ((pFoldersPpt->end() != iter) &&
                          ((*iter)->getColonne() > iColBase + 1))
      {
        sSens = (*iter)->getLexiqueSens() ;

        // Folder name
        //
        if (string("0INTI") == sSens)
        {
          iter++ ;

          while ((pFoldersPpt->end() != iter) &&
                          ((*iter)->getColonne() > iColBase + 2))
          {
            // must be a free text
            string sElemLex = (*iter)->getLexique() ;

            if (string("£?????") == sElemLex)
              sNom = (*iter)->getTexteLibre() ;

            iter++ ;
          }
        }
        // Dates
        else if (string("KOUVR") == sSens)
        {
          iter++ ;
          int iLigneBase = (*iter)->getLigne() ;

          string sUnite  = string("") ;
          string sFormat = string("") ;
          string sValeur = string("") ;

          while ((pFoldersPpt->end() != iter) &&
                         ((*iter)->getLigne() == iLigneBase))
          {
            string sLexique = (*iter)->getLexique() ;
            if ((string("") != sLexique) && ('£' == sLexique[0]))
            {
              sFormat = (*iter)->getLexiqueSens() ;
              sValeur = (*iter)->getComplement() ;
              sUnite  = (*iter)->getUnitSens() ;
              iter++ ;
              break ;
            }

            iter++ ;
          }

          // sFormat est du type £D0;03
          if ((string("") != sFormat) && ('T' == sFormat[1]) &&
              (string("") != sValeur))
          {
            if (string("2DA02") == sUnite)
              sDate = string(sValeur, 0, 8) ;
          }
        }
        else
          iter++ ;
      }

      if (string("") != sNom)
      {
        NSChemiseInfo *pNewChemInfo = new NSChemiseInfo() ;
        pNewChemInfo->_sNodeChemise = sNodeChem ;
        pNewChemInfo->setNom(sNom) ;
        pNewChemInfo->setCreation(sDate) ;

        push_back(pNewChemInfo) ;
      }
    }
    else
      iter++ ;
  }

  return true ;
}

//***************************************************************************
// Implémentation des méthodes NSSejour
//***************************************************************************

//---------------------------------------------------------------------------
//  Initialise les champs de données à zéro.
//---------------------------------------------------------------------------
void NSSejourData::metAZero()
{
  sTreeID    = string("") ;
  sNumero    = string("") ;
  sDateDeb   = string("") ;
  sDateFin   = string("") ;
  sUnit      = string("") ;
  sCarePlace = string("") ;
}

//---------------------------------------------------------------------------
//  Renvoie dans une chaine l'intitulé de la chemise
//---------------------------------------------------------------------------
void NSSejourData::donneIntitule(char * /* intitule */)
{
/*
	char dateAffiche[20];	strcpy(intitule, nom);
	ote_blancs(intitule);
	if (strcmp(creation, "        ") != 0)
	{
		strcat(intitule, " du ");
		donne_date(creation, dateAffiche, sLang);
		strcat(intitule, dateAffiche);
	} */
}

//---------------------------------------------------------------------------//  Constructeur copie
//---------------------------------------------------------------------------
NSSejourData::NSSejourData(const NSSejourData& rv)
{
	sTreeID    = rv.sTreeID ;
	sNumero    = rv.sNumero ;
	sDateDeb   = rv.sDateDeb ;
	sDateFin   = rv.sDateFin ;
	sUnit      = rv.sUnit ;
  sCarePlace = rv.sCarePlace ;
}

//---------------------------------------------------------------------------//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSSejourData& NSSejourData::operator=(const NSSejourData& src)
{
	if (this == &src)
		return *this ;

  sTreeID    = src.sTreeID ;
  sNumero    = src.sNumero ;
  sDateDeb   = src.sDateDeb ;
  sDateFin   = src.sDateFin ;
  sUnit      = src.sUnit ;
  sCarePlace = src.sCarePlace ;

  return *this ;
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------

int
NSSejourData::hasSameContent(const NSSejourData& o) const
{
  if ((sNumero    == o.sNumero)  &&
      (sDateDeb   == o.sDateDeb) &&
      (sDateFin   == o.sDateFin) &&
      (sUnit      == o.sUnit)    &&
      (sCarePlace == o.sCarePlace))
		return 1 ;

  return 0 ;
}

int
NSSejourData::operator==(const NSSejourData& o) const
{
	if ((sTreeID == o.sTreeID) && hasSameContent(o))
		return 1 ;

  return 0 ;
}

//---------------------------------------------------------------------------
//  Constructeur par défaut
//---------------------------------------------------------------------------
NSSejourInfo::NSSejourInfo()
{
  lObjectCount++ ;
}

//---------------------------------------------------------------------------
//  Destructeur.
//---------------------------------------------------------------------------
NSSejourInfo::~NSSejourInfo()
{
  lObjectCount-- ;
}

//---------------------------------------------------------------------------
//  Donne l'intitulé du séjour.
//---------------------------------------------------------------------------
void
NSSejourInfo::donneIntitule(char *intitule)
{
	_Donnees.donneIntitule(intitule);
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSSejourInfo::NSSejourInfo(const NSSejourInfo& rv)
{
	//
	// Initialise les données à partir de celles de la source
	//
	_Donnees = rv._Donnees ;

  lObjectCount++ ;
}

bool
NSSejourInfo::initFromPatPatho(NSPatPathoArray *pPPT)
{
  if (((NSPatPathoArray*) NULL == pPPT) || (pPPT->empty()))
    return false ;

  PatPathoIter iter = pPPT->begin() ;
  int iColBase = (*iter)->getColonne() ;

  setTreeID((*iter)->getDoc()) ;

  iter++ ;

  while ((pPPT->end() != iter) && ((*iter)->getColonne() > iColBase))
  {
    string sElemSens = (*iter)->getLexiqueSens() ;

    // numéro de séjour
    if (string("LNUSE") == sElemSens)
      _Donnees.sNumero = pPPT->getFreeTextValue(&iter) ;

    // unité fonctionnelle
    else if (string("LUNIF") == sElemSens)
    {
      iter++ ;
      while ((pPPT->end() != iter) && ((*iter)->getColonne() > iColBase + 1))
      {
        string sSubElemSens = (*iter)->getLexiqueSens() ;
        if (string("£OB") == sSubElemSens)
        {
          _Donnees.sUnit = (*iter)->getComplement() ;
          iter++ ;
        }
        else if (string("0CODE") == sSubElemSens)
          _Donnees.sUnit = pPPT->getFreeTextValue(&iter) ;
        else
          iter++ ;
      }
    }
    // care place
    else if (string("UETAB") == sElemSens)
    {
      iter++ ;
      while ((pPPT->end() != iter) && ((*iter)->getColonne() > iColBase + 1))
      {
        string sSubElemSens = (*iter)->getLexiqueSens() ;
        if (string("£OB") == sSubElemSens)
        {
          _Donnees.sUnit = (*iter)->getComplement() ;
          iter++ ;
        }
        else if (string("LFINE") == sSubElemSens)
          _Donnees.sUnit = pPPT->getFreeTextValue(&iter) ;
        else
          iter++ ;
      }
    }
    // date d'ouverture
    else if (string("KOUVR") == sElemSens)
    {
      iter++ ;

      string sUnite  = (*iter)->getUnitSens() ;
      string sFormat = (*iter)->getLexiqueSens() ;
      string sValeur = (*iter)->getComplement() ;

      if (string("2DA02") == sUnite)
        _Donnees.sDateDeb = sValeur ;

      iter++ ;
    }
    // date de fermeture
    else if (string("KFERM") == sElemSens)
    {
      iter++ ;

      string sUnite  = (*iter)->getUnitSens() ;
      string sFormat = (*iter)->getLexiqueSens() ;
      string sValeur = (*iter)->getComplement() ;

      if (string("2DA02") == sUnite)
        _Donnees.sDateFin = sValeur ;

      iter++ ;
    }
    else
      iter++ ;
  }

  return true ;
}

bool
NSSejourInfo::initFromCapture(NSCaptureArray* pCapt)
{
  if (((NSCaptureArray*) NULL == pCapt) || pCapt->empty())
    return false ;

  string sVenueId        = string("") ;
  string sFonctionalUnit = string("") ;
  string sBeginDate      = string("") ;
  string sEndDate        = string("") ;

  pCapt->trouveChemin(string("LSEJO1/LNUSE1"),        &sVenueId) ;
  pCapt->trouveChemin(string("LSEJO1/LUNIF1/0CODE1"), &sFonctionalUnit) ;
  pCapt->trouveChemin(string("LSEJO1/KOUVR1"),        &sBeginDate) ;
  pCapt->trouveChemin(string("LSEJO1/KFERM1"),        &sEndDate) ;

  if ((string("") == sVenueId)   && (string("") == sFonctionalUnit) &&
      (string("") == sBeginDate) && (string("") == sEndDate))
    return false ;

  setNumero(sVenueId) ;
  setUnit(sFonctionalUnit) ;
  setBeginDate(sBeginDate) ;
  setEndDate(sEndDate) ;

  return true ;
}

bool
NSSejourInfo::updateFromCapture(NSCaptureArray* pCapt)
{
  if (((NSCaptureArray*) NULL == pCapt) || pCapt->empty())
    return false ;

  string sVenueId        = string("") ;
  string sFonctionalUnit = string("") ;
  string sBeginDate      = string("") ;
  string sEndDate        = string("") ;

  pCapt->trouveChemin(string("LSEJO1/LNUSE1"),        &sVenueId) ;
  pCapt->trouveChemin(string("LSEJO1/LUNIF1/0CODE1"), &sFonctionalUnit) ;
  pCapt->trouveChemin(string("LSEJO1/KOUVR1"),        &sBeginDate) ;
  pCapt->trouveChemin(string("LSEJO1/KFERM1"),        &sEndDate) ;

  if ((string("") == sVenueId)   && (string("") == sFonctionalUnit) &&
      (string("") == sBeginDate) && (string("") == sEndDate))
    return false ;

  if (string("") != sVenueId)
    setNumero(sVenueId) ;

  if (string("") != sFonctionalUnit)
    setUnit(sFonctionalUnit) ;

  if (string("") != sBeginDate)
    setBeginDate(sBeginDate) ;

  if (string("") != sEndDate)
    setEndDate(sEndDate) ;

  return true ;
}

bool
NSSejourInfo::buildPatPatho(NSPatPathoArray *pPPT)
{
  if ((NSPatPathoArray*) NULL == pPPT)
    return false ;

  pPPT->vider() ;

  pPPT->ajoutePatho("LSEJO1", 0) ;

  if (string("") != getNumero())
  {
    pPPT->ajoutePatho("LNUSE1", 1) ;

    Message Msg ;
    Msg.SetTexteLibre(getNumero()) ;
    pPPT->ajoutePatho("£CL000", &Msg, 2) ;
  }

  if (string("") != getCarePlace())
  {
    pPPT->ajoutePatho("UETAB1", 1) ;
    pPPT->ajoutePatho("LFINE1", 2) ;

    Message Msg ;
    Msg.SetTexteLibre(getCarePlace()) ;
    pPPT->ajoutePatho("£CL000", &Msg, 3) ;
  }

  if (string("") != getUnit())
  {
    pPPT->ajoutePatho("LUNIF1", 1) ;
    pPPT->ajoutePatho("0CODE1", 2) ;

    Message Msg ;
    Msg.SetTexteLibre(getUnit()) ;
    pPPT->ajoutePatho("£CL000", &Msg, 3) ;
  }

  if (string("") != getBeginDate())
  {
    pPPT->ajoutePatho("KOUVR1", 1) ;

    Message Msg ;
    Msg.SetComplement(getBeginDate()) ;
    Msg.SetUnit("2DA021") ;
    pPPT->ajoutePatho("£T0;19", &Msg, 2) ;
  }

  if (string("") != getEndDate())
  {
    pPPT->ajoutePatho("KFERM1", 1) ;

    Message Msg ;
    Msg.SetComplement(getEndDate()) ;
    Msg.SetUnit("2DA021") ;
    pPPT->ajoutePatho("£T0;19", &Msg, 2) ;
  }

  return true ;
}

bool
NSSejourInfo::updatePatPatho(NSPatPathoArray *pPPT)
{
  if ((NSPatPathoArray*) NULL == pPPT)
    return false ;

  if (pPPT->empty())
    return buildPatPatho(pPPT) ;

  PatPathoIter iter = pPPT->begin() ;
  int iColBase = (*iter)->getColonne() ;

  iter++ ;

  while ((pPPT->end() != iter) && ((*iter)->getColonne() > iColBase))
  {
    string sElemSens = (*iter)->getLexiqueSens() ;

    // numéro de séjour
    if (string("LNUSE") == sElemSens)
    {
      iter++ ;
      while ((pPPT->end() != iter) && ((*iter)->getColonne() > iColBase + 1))
      {
        // mise à jour d'un texte libre
        string sSubElemSens = (*iter)->getLexiqueSens() ;
        if (string("£CL") == sSubElemSens)
        {
          string sPreviousVenueNumber = (*iter)->getTexteLibre() ;
          if (sPreviousVenueNumber != _Donnees.sNumero)
            (*iter)->setTexteLibre(_Donnees.sNumero) ;
        }

        iter++ ;
      }
    }
    // unité fonctionnelle
    else if (string("LUNIF") == sElemSens)
    {
      iter++ ;
      while ((pPPT->end() != iter) && ((*iter)->getColonne() > iColBase + 1))
      {
        string sSubElemSens = (*iter)->getLexiqueSens() ;
        if (string("£OB") == sSubElemSens)
        {
          string sPreviousFunctionnalUnit = (*iter)->getComplement() ;
          if (sPreviousFunctionnalUnit != _Donnees.sUnit)
            (*iter)->setComplement(_Donnees.sUnit) ;
          iter++ ;
        }
        else if (string("0CODE") == sSubElemSens)
        {
          iter++ ;
          while ((pPPT->end() != iter) && ((*iter)->getColonne() > iColBase + 2))
          {
            // on cherche ici un texte libre
            string sSubElemSens = (*iter)->getLexiqueSens() ;
            if (string("£CL") == sSubElemSens)
            {
              string sPreviousCode = (*iter)->getTexteLibre() ;
              if (sPreviousCode != _Donnees.sUnit)
                (*iter)->setTexteLibre(_Donnees.sUnit) ;
            }

            iter++ ;
          }
        }
        else
          iter++ ;
      }
    }
    // care place
    else if (string("UETAB") == sElemSens)
    {
      iter++ ;
      while ((pPPT->end() != iter) && ((*iter)->getColonne() > iColBase + 1))
      {
        string sSubElemSens = (*iter)->getLexiqueSens() ;
        if (string("£OB") == sSubElemSens)
        {
          string sPreviousFunctionnalUnit = (*iter)->getComplement() ;
          if (sPreviousFunctionnalUnit != _Donnees.sUnit)
            (*iter)->setComplement(_Donnees.sUnit) ;
          iter++ ;
        }
        else if (string("LFINE") == sSubElemSens)
        {
          iter++ ;
          while ((pPPT->end() != iter) && ((*iter)->getColonne() > iColBase + 2))
          {
            // on cherche ici un texte libre
            string sSubElemSens = (*iter)->getLexiqueSens() ;
            if (string("£CL") == sSubElemSens)
            {
              string sPreviousCode = (*iter)->getTexteLibre() ;
              if (sPreviousCode != _Donnees.sUnit)
                (*iter)->setTexteLibre(_Donnees.sUnit) ;
            }

            iter++ ;
          }
        }
        else
          iter++ ;
      }
    }
    // date d'ouverture
    else if (string("KOUVR") == sElemSens)
    {
      iter++ ;

      string sUnite  = (*iter)->getUnitSens() ;
      string sFormat = (*iter)->getLexiqueSens() ;
      string sValeur = (*iter)->getComplement() ;

      if (string("2DA02") == sUnite)
      {
        if (sValeur != _Donnees.sDateDeb)
          (*iter)->setComplement(_Donnees.sDateDeb) ;
      }

      iter++ ;
    }
    // date de fermeture
    else if (string("KFERM") == sElemSens)
    {
      iter++ ;

      string sUnite  = (*iter)->getUnitSens() ;
      string sFormat = (*iter)->getLexiqueSens() ;
      string sValeur = (*iter)->getComplement() ;

      if (string("2DA02") == sUnite)
      {
        if (sValeur != _Donnees.sDateFin)
          (*iter)->setComplement(_Donnees.sDateFin) ;
      }

      iter++ ;
    }
    else
      iter++ ;
  }

  return true ;
}

//---------------------------------------------------------------------------//  Constructeur copie
//---------------------------------------------------------------------------
NSSejourInfo&
NSSejourInfo::operator=(const NSSejourInfo& src)
{
  if (this == &src)
		return *this ;

	_Donnees = src._Donnees ;

	return *this ;
}

intNSSejourInfo::operator == (const NSSejourInfo& o) const{
	return (_Donnees == o._Donnees) ;
}

int
NSSejourInfo::hasSameContent(const NSSejourInfo& o) const{
	return _Donnees.hasSameContent(o._Donnees) ;
}

//***************************************************************************// Implémentation des méthodes NSChemDoc
//***************************************************************************

//---------------------------------------------------------------------------
//  Function:		NSChemDocData::NSChemDocData()
//  Description:	Constructeur.
//  Retour:			Aucun
//---------------------------------------------------------------------------
NSChemDocData::NSChemDocData()
{
  // Met les champs de données à zéro
  memset(_chemise,  0, CDO_CHEMISE_LEN + 1) ;
  memset(_ordre,    0, CDO_ORDRE_LEN + 1) ;
  memset(_document, 0, CDO_DOCUMENT_LEN + 1) ;
}

NSChemDocData::NSChemDocData(const NSChemDocData& rv)
{
	strcpy(_chemise,  rv._chemise) ;
	strcpy(_ordre,    rv._ordre) ;
	strcpy(_document, rv._document) ;
}

NSChemDocData& NSChemDocData::operator=(const NSChemDocData& src)
{
  if (this == &src)
		return *this ;

	strcpy(_chemise,  src._chemise) ;
	strcpy(_ordre,    src._ordre) ;
	strcpy(_document, src._document) ;

	return *this ;
}

//---------------------------------------------------------------------------
//  Description:	Opérateur de comparaison
//  Retour:			Rien
//---------------------------------------------------------------------------
int NSChemDocData::operator==(const NSChemDocData& o)
{
  if ((strcmp(_chemise,  o._chemise)  == 0) &&
		  (strcmp(_ordre,    o._ordre) 	  == 0) &&
		  (strcmp(_document, o._document) == 0))
    return 1 ;
  else
    return 0 ;
}

//---------------------------------------------------------------------------
//  Constructeur par défaut
//---------------------------------------------------------------------------
NSChemDocInfo::NSChemDocInfo()
{
  lObjectCount++ ;
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSChemDocInfo::NSChemDocInfo(const NSChemDocInfo& rv)
{
	_Donnees = rv._Donnees ;

  lObjectCount++ ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSChemDocInfo::~NSChemDocInfo()
{
  lObjectCount-- ;
}

//---------------------------------------------------------------------------
//  Opérateur =
//---------------------------------------------------------------------------
NSChemDocInfo& NSChemDocInfo::operator=(const NSChemDocInfo& src)
{
	if (this == &src)
		return *this ;

	_Donnees = src._Donnees ;

	return *this;
}

int NSChemDocInfo::operator==(const NSChemDocInfo& o)
{
	return (_Donnees == o._Donnees) ;
}

// --------------------------------------------------------------------------
//                 Implémentation des méthodes NSDocStatus
// --------------------------------------------------------------------------

NSDocStatus::NSDocStatus()
{
	_iStatus     = StatusUndefined ;
  _iCptaStatus = StatusUndefined ;

  lObjectCount++ ;
}

NSDocStatus::NSDocStatus(const NSDocStatus& rv)
{
	_iStatus     = rv._iStatus ;
  _iCptaStatus = rv._iCptaStatus ;

  lObjectCount++ ;
}

NSDocStatus::~NSDocStatus()
{
  lObjectCount-- ;
}

NSDocStatus& NSDocStatus::operator=(const NSDocStatus& src)
{
	if (this == &src)
		return *this ;

  _iStatus     = src._iStatus ;
  _iCptaStatus = src._iCptaStatus ;

	return *this ;
}

