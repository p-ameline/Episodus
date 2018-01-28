// nsagadlg.cpp : dialogues des AGA des fiches Fact// RS Sept 98
//////////////////////////////////////////////////////////

#include <stdio.h>
#include <classlib\date.h>
#include <classlib\time.h>

#include "partage\nsdivfct.h"
#include "nautilus\nssuper.h"
#include "nscompta\nsagavar.h"
#include "nsbb\tagNames.h"

//***************************************************************************
// 								Classe NSComboArray
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSComboArray::NSComboArray(const NSComboArray& rv) : NSComboBoxArray()
{
	if (false == rv.empty())
		for (NSComboConstIter i = rv.begin() ; rv.end() != i ; i++)
			push_back(new NSComboItem(*(*i))) ;
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
void
NSComboArray::vider()
{
	if (empty())
  	return ;

	for (NSComboIter i = begin() ; end() != i ; )
  {
  	delete *i ;
    erase(i) ;
  }
}

NSComboArray::~NSComboArray(){
	vider() ;
}

//-----------------------------------------------------------------------// fonction globale d'initialisation des combobox
//-----------------------------------------------------------------------
bool
InitComboBox(TComboBox* pCombo, NSComboArray* pComboArray, string sFichier)
{
  ifstream 	  inFile ;
  string 		  sData = "" ;
  string      line ;
  size_t 		  i = 0 ;
  NSComboItem tCombo ;

  inFile.open(sFichier.c_str());
	if (!inFile)
    return false ;

  while (!inFile.eof())
  {
    getline(inFile,line) ;
    if (line != "")
      sData += line + "\n" ;
  }
  inFile.close() ;

  while ((i < strlen(sData.c_str())) && (';' != sData[i]))
  {
    tCombo.sCode  = "" ;
    tCombo.sLabel = "" ;

    while ((i < strlen(sData.c_str())) && ('\n' != sData[i]) && (' ' != sData[i]))
      tCombo.sCode += sData[i++] ;
    i++ ;

    while ((i < strlen(sData.c_str())) && ('\n' != sData[i]))
      tCombo.sLabel += sData[i++] ;
    i++ ;

    pComboArray->push_back(new NSComboItem(tCombo)) ;
  }

  if (false == pComboArray->empty())
    for (NSComboIter j = pComboArray->begin() ; pComboArray->end() != j ; j++)
      pCombo->AddString(((*j)->sLabel).c_str()) ;

  return true ;
}

// -----------------------------------------------------------------//
//  Méthodes de NSVarCompta
//
// -----------------------------------------------------------------
NSVarCompta::NSVarCompta(NSContexte* pCtx)
{
	_parite        = pCtx->getSuperviseur()->getParite() ;
	_monnaieRef    = pCtx->getSuperviseur()->getMonnaieRef() ;
	_sigle         = pCtx->getSuperviseur()->getSigle() ;
	_indiceConsult = pCtx->getSuperviseur()->getIndiceConsult() ;

	_sDateC        = string("") ;
	_sHeureC       = string("") ;
	_bCreation     = true ;
	_bFact0        = true ;
}

NSVarCompta::NSVarCompta(const NSVarCompta& rv){
	_parite        = rv._parite ;
  _monnaieRef    = rv._monnaieRef ;
  _sigle         = rv._sigle ;
  _indiceConsult = rv._indiceConsult ;
  _sDateC        = rv._sDateC ;
  _sHeureC       = rv._sHeureC ;
  _bCreation     = rv._bCreation ;
  _bFact0        = rv._bFact0 ;
}

NSVarCompta&
NSVarCompta::operator=(const NSVarCompta& src)
{
	if (this == &src)
		return *this ;

	_parite        = src._parite ;
  _monnaieRef    = src._monnaieRef ;
  _sigle         = src._sigle ;
  _indiceConsult = src._indiceConsult ;
  _sDateC        = src._sDateC ;
  _sHeureC       = src._sHeureC ;
  _bCreation     = src._bCreation ;
  _bFact0        = src._bFact0 ;

	return *this ;
}

string
NSVarCompta::getMonnaieSigle()
{
  if (MONNAIE_LOCALE == _monnaieRef)
    return _sigle ;

  return string("Euros") ;
}

int
NSVarCompta::getEuroFromLocal(int iCents)
{
  if (0 == _parite)
    return 0 ;

  return dtoi(double(iCents) / double(_parite)) ;
}

int
NSVarCompta::getLocalFromEuro(int iCents)
{
  if (0 == _parite)
    return 0 ;

  return dtoi(double(iCents) * double(_parite)) ;
}

//***************************************************************************// Implémentation des méthodes NSAgaData
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NSAgaData::NSAgaData(NSContexte* pCtx)
          :patInfo(pCtx->getSuperviseur()), NSRoot(pCtx)
{
	//
	// Met les champs de données à zéro
	//
	metAZero() ;
}

//---------------------------------------------------------------------------//  Constructeur copie
//---------------------------------------------------------------------------
NSAgaData::NSAgaData(const NSAgaData& rv)
          :patInfo(rv.patInfo), NSRoot(rv.pContexte)
{
	strcpy(nomPatient, rv.nomPatient) ;
	strcpy(libelle,    rv.libelle) ;
	strcpy(numCompt,   rv.numCompt) ;
	strcpy(actes,	     rv.actes) ;
	strcpy(montant,    rv.montant) ;
	strcpy(unite, 	   rv.unite) ;
	strcpy(modePaie,   rv.modePaie) ;
	strcpy(datePaie,   rv.datePaie) ;
}
//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
NSAgaData::~NSAgaData()
{
}

//---------------------------------------------------------------------------//  Opérateur =
//---------------------------------------------------------------------------
NSAgaData&
NSAgaData::operator=(const NSAgaData& src)
{
	if (this == &src)
		return *this ;

	strcpy(nomPatient, src.nomPatient) ;
	strcpy(libelle,    src.libelle) ;	strcpy(numCompt, 	 src.numCompt) ;
	strcpy(actes,		   src.actes) ;
	strcpy(montant,  	 src.montant) ;
	strcpy(unite, 		 src.unite) ;
	strcpy(modePaie,	 src.modePaie) ;
	strcpy(datePaie,   src.datePaie) ;

	patInfo = src.patInfo ;

	return *this ;
}

//---------------------------------------------------------------------------//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSAgaData::operator==(const NSAgaData& o)
{
	if ((strcmp(nomPatient,	o.nomPatient)   == 0) &&
        (strcmp(libelle,    o.libelle)      == 0) &&
   	    (strcmp(numCompt, 	o.numCompt) 	== 0) &&
   	    (strcmp(actes, 		o.actes) 		== 0) &&
        (strcmp(montant,	o.montant)		== 0) &&
        (strcmp(unite,		o.unite)		== 0) &&
        (strcmp(modePaie,	o.modePaie)		== 0) &&
        (strcmp(datePaie,   o.datePaie)     == 0) &&        (patInfo == o.patInfo))
		return 1;
	else
		return 0;
}

//---------------------------------------------------------------------------//  Met à zéro les variables de la fiche
//---------------------------------------------------------------------------
void
NSAgaData::metAZero()
{
	//
	// Met les champs de données à zéro
	//
  memset(nomPatient,  0, PAT_NOM_LONG_LEN + 1);
  memset(libelle,     0, REC_LIBELLE_LEN + 1);	memset(numCompt,    0, FACT_NUMCOMPT_LEN + 1);
  memset(actes,       0, AGA_ACTES_LEN + 1);
	memset(montant,     0, FACT_MONTANT_LEN + 1);
	memset(unite,       0, FACT_UNITE_LEN + 1);
  memset(modePaie,    0, FACT_MODE_PAIE_LEN + 1);
  memset(datePaie,    0, AGA_DATE_PAIE_LEN + 1);

#ifndef _MUE
    patInfo.pDonnees->metAZero();#endif
}
//***************************************************************************// 					Implémentation des méthodes NSAgaArray
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSAgaArray::NSAgaArray(const NSAgaArray& rv)
           :NSFicheAgaArray()
{
	if (false == rv.empty())
		for (NSAgaConstIter i = rv.begin() ; rv.end() != i ; i++)
    	push_back(new NSAgaData(*(*i))) ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
void
NSAgaArray::vider()
{
	if (empty())
		return ;

	for (NSAgaIter i = begin() ; end() != i ; )
	{
  	delete *i ;
    erase(i) ;
	}
}

NSAgaArray::~NSAgaArray(){
	vider() ;
}

//***************************************************************************// 					Implémentation des méthodes NSCriteres
//***************************************************************************

NSCriteres::NSCriteres(){
	_sDateAga1   = string("") ;
	_sDateAga2   = string("") ;
	_bActesPerso = false ;
}

NSCriteres::NSCriteres(const NSCriteres& rv)
{
	_sDateAga1   = rv._sDateAga1 ;
	_sDateAga2   = rv._sDateAga2 ;
	_bActesPerso = rv._bActesPerso ;
}

NSCriteres&NSCriteres::operator=(const NSCriteres& src)
{
	if (this == &src)
		return *this ;

	_sDateAga1   = src._sDateAga1 ;	_sDateAga2   = src._sDateAga2 ;
	_bActesPerso = src._bActesPerso ;

	return *this ;
}

NSCriteres::~NSCriteres(){
}

//***************************************************************************
//
// Implémentation des méthodes NSAffDepensData//
//***************************************************************************
//---------------------------------------------------------------------------//  Constructeur
//---------------------------------------------------------------------------

NSAffDepensData::NSAffDepensData(){
	//
	// Met les champs de données à zéro
	//
	metAZero() ;
}
//---------------------------------------------------------------------------//  Constructeur copie
//---------------------------------------------------------------------------

NSAffDepensData::NSAffDepensData(const NSAffDepensData& rv){
	_depense  = rv._depense ;
	_ecriture = rv._ecriture ;
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------

NSAffDepensData::~NSAffDepensData(){
}

//---------------------------------------------------------------------------//  Opérateur =
//---------------------------------------------------------------------------

NSAffDepensData&NSAffDepensData::operator=(const NSAffDepensData& src)
{
	if (this == &src)
		return *this ;

	_depense  = src._depense ;
	_ecriture = src._ecriture ;
	return *this ;}

//---------------------------------------------------------------------------//  Opérateur de comparaison
//---------------------------------------------------------------------------

intNSAffDepensData::operator==(const NSAffDepensData& o)
{
	if ((_depense  == o._depense) &&
      (_ecriture == o._ecriture))
		return 1 ;
	else
		return 0 ;
}

//---------------------------------------------------------------------------//  Met à zéro les variables de la fiche
//---------------------------------------------------------------------------

voidNSAffDepensData::metAZero()
{
	_depense.metAZero() ;
	_ecriture.metAZero() ;
}
//***************************************************************************// 					Implémentation des méthodes NSAffDepensArray
//***************************************************************************

//---------------------------------------------------------------------------//  Constructeur copie
//---------------------------------------------------------------------------

NSAffDepensArray::NSAffDepensArray(const NSAffDepensArray& rv)                 :NSFicheAffDepensArray(){
	if (false == rv.empty())
  	for (NSAffDepensConstIter i = rv.begin() ; rv.end() != i ; i++)
    	push_back(new NSAffDepensData(*(*i))) ;
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------

voidNSAffDepensArray::vider()
{
  if (empty())
    return ;

  for (NSAffDepensIter i = begin() ; end() != i ; )  {
    delete *i ;
    erase(i) ;
  }
}

NSAffDepensArray::~NSAffDepensArray(){
	vider() ;
}

//***************************************************************************//
// Implémentation des méthodes NSImpData//
//***************************************************************************
//---------------------------------------------------------------------------//  Constructeur
//---------------------------------------------------------------------------
NSImpData::NSImpData(NSContexte* pCtx)
          :patInfo(pCtx->getSuperviseur()), NSRoot(pCtx)
{
	//
	// Met les champs de données à zéro
	//
	metAZero() ;
}

//---------------------------------------------------------------------------//  Constructeur copie
//---------------------------------------------------------------------------
NSImpData::NSImpData(const NSImpData& rv)
          :patInfo(rv.patInfo), NSRoot(rv.pContexte)
{
	strcpy(numCompt,	   rv.numCompt) ;
	strcpy(dateCompt,	   rv.dateCompt) ;
	// strcpy(nomPatient,	 rv.nomPatient) ;
	// strcpy(libExam, 	   rv.libExam) ;
  strcpy(codePatient,	 rv.codePatient) ;
  strcpy(sommeDueLoc,	 rv.sommeDueLoc) ;
  strcpy(sommeDueEuro, rv.sommeDueEuro) ;
  strcpy(impayeLoc,	   rv.impayeLoc) ;
  strcpy(impayeEuro,	 rv.impayeEuro) ;
  strcpy(libOrga,		   rv.libOrga) ;

  sNomPatient = rv.sNomPatient ;
  sLibExam    = rv.sLibExam ;

  bPatientLoaded = rv.bPatientLoaded ;
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
NSImpData::~NSImpData()
{
}

//---------------------------------------------------------------------------
//  Opérateur =
//---------------------------------------------------------------------------
NSImpData&
NSImpData::operator=(const NSImpData& src)
{
	if (this == &src)
		return *this ;

	strcpy(numCompt,		 src.numCompt) ;	strcpy(dateCompt,		 src.dateCompt) ;
	// strcpy(nomPatient,	 src.nomPatient) ;
	// strcpy(libExam, 	   src.libExam) ;
  strcpy(codePatient,	 src.codePatient) ;
  strcpy(sommeDueLoc,	 src.sommeDueLoc) ;
  strcpy(sommeDueEuro, src.sommeDueEuro) ;
  strcpy(impayeLoc,		 src.impayeLoc) ;
  strcpy(impayeEuro,	 src.impayeEuro) ;
  strcpy(libOrga,		   src.libOrga) ;

  sNomPatient = src.sNomPatient ;
  sLibExam    = src.sLibExam ;

  bPatientLoaded = src.bPatientLoaded ;

  patInfo = src.patInfo ;

	return *this ;
}

//---------------------------------------------------------------------------//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSImpData::operator==(const NSImpData& o)
{
	if ((strcmp(numCompt,		 o.numCompt)		 == 0) &&
   	 (strcmp(dateCompt,		 o.dateCompt)		 == 0) &&
   	 // (strcmp(nomPatient, 	 o.nomPatient) 	 == 0) &&
   	 // (strcmp(libExam, 		 o.libExam) 		 == 0) &&
     (strcmp(codePatient,  o.codePatient)  == 0) &&
     (strcmp(sommeDueLoc,	 o.sommeDueLoc)	 == 0) &&
     (strcmp(sommeDueEuro, o.sommeDueEuro) == 0) &&
     (strcmp(impayeLoc,		 o.impayeLoc)		 == 0) &&
     (strcmp(impayeEuro,	 o.impayeEuro)	 == 0) &&
     (strcmp(libOrga,			 o.libOrga)			 == 0) &&
     (sNomPatient	== o.sNomPatient) &&
     (sLibExam	  == o.sLibExam) &&
     (patInfo     == o.patInfo))
		return 1;
	else
		return 0;
}

//---------------------------------------------------------------------------//  Met à zéro les variables de la fiche
//---------------------------------------------------------------------------
void
NSImpData::metAZero()
{
	//
	// Met les champs de données à zéro
	//
  memset(numCompt,		 0, CPTA_NUMCOMPT_LEN + 1) ;
	memset(dateCompt,		 0, CPTA_DATE_LEN + 1) ;
  // memset(nomPatient,	 0, PAT_NOM_LONG_LEN + 1) ;
	// memset(libExam, 		 0, IMP_LIB_EXAM_LEN + 1) ;
  memset(codePatient,	 0, PAT_NSS_LEN + 1) ;
  memset(sommeDueLoc,	 0, CPTA_DUE_F_LEN + 1) ;
	memset(sommeDueEuro, 0, CPTA_DUE_E_LEN + 1) ;
	memset(impayeLoc, 	 0, TPAY_RESTE_DU_LEN + 1) ;
  memset(impayeEuro, 	 0, TPAY_RESTE_DU_LEN + 1) ;
  memset(libOrga,		   0, TPAY_LIBELLE_LEN + 1) ;

  sNomPatient = string("") ;
  sLibExam    = string("") ;

  bPatientLoaded = false ;

#ifndef _MUE
   patInfo.pDonnees->metAZero() ;#endif
}
bool
NSImpData::initPatientInfo()
{
  if ('\0' == codePatient[0])
    return false ;

  bPatientLoaded = true ;

  NSPersonsAttributesArray AttsList ;

  NSBasicAttributeArray AttrArray ;
  AttrArray.push_back(new NSBasicAttribute(PERSON, string(codePatient))) ;

  const char* serviceName = (NautilusPilot::SERV_PERSON_TRAITS_LIST).c_str() ;

	bool listOk = pContexte->getSuperviseur()->getPilot()->getUserProperties(serviceName, &AttsList, &AttrArray) ;
  if ((false == listOk) || AttsList.empty())
    return false ;

  NSPersonsAttributeIter iterPerson = AttsList.begin() ;

  NSBasicAttributeArray* pAttribute = *iterPerson ;
  if ((NULL == pAttribute) || pAttribute->empty())
    return false ;

  NSPersonInfo tempInfo(pContexte->getSuperviseur(), pAttribute) ;

  // Previous method, that loads whole admin graph
/*
	NSPersonInfo tempInfo(pContexte, string(codePatient), pidsPatient) ;
*/

	sNomPatient = tempInfo.getCivilite() ;
  if (string("") == sNomPatient)
    sNomPatient = tempInfo.getNomLong() ;

  patInfo     = NSPersonInfo(tempInfo) ;

	return true ;
}

//***************************************************************************// 					Implémentation des méthodes NSImpArray
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------NSImpArray::NSImpArray(const NSImpArray& rv)
           :NSFicheImpArray()
{
	if (false == rv.empty())
		for (NSImpConstIter i = rv.begin() ; rv.end() != i ; i++)
   		push_back(new NSImpData(*(*i))) ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
void
NSImpArray::vider()
{
	if (false == empty())
		for (NSImpIter i = begin() ; end() != i ; )
   	{
   		delete *i ;
      erase(i) ;
   	}
}

NSImpArray::~NSImpArray(){
	vider() ;
}

void
NSImpArray::initAllPatientInfo()
{
  if (empty())
    return ;

  for (NSImpIter i = begin() ; end() != i ; )
    if (false == (*i)->bPatientLoaded)
      (*i)->initPatientInfo() ;
}

//***************************************************************************// Implémentation des méthodes NSSomActData
//***************************************************************************

NSSomActData::NSSomActData()
{
  _iNbTotal           = 0 ;
  _iNbTotalAvecDepass = 0 ;

	_iTotalLoc          = 0 ;
  _iTotalEuro         = 0 ;
  depassLoc           = 0 ;
  depassEuro          = 0 ;
  paieLoc             = 0 ;
  paieEuro            = 0 ;
  espLoc              = 0 ;
  espEuro             = 0 ;
  chqLoc              = 0 ;
  chqEuro             = 0 ;
  virLoc              = 0 ;
  virEuro             = 0 ;
  cbLoc               = 0 ;
  cbEuro              = 0 ;
  impayeLoc           = 0 ;
  impayeEuro          = 0 ;
  impTPLoc            = 0 ;
  impTPEuro           = 0 ;
  impAutreLoc         = 0 ;
  impAutreEuro        = 0 ;

  _iTotalAvecDepassLoc  = 0 ;
  _iTotalAvecDepassEuro = 0 ;
}

NSSomActData::~NSSomActData(){
}

NSSomActData::NSSomActData(const NSSomActData& rv){
  _iNbTotal           = rv._iNbTotal ;
  _iNbTotalAvecDepass = rv._iNbTotalAvecDepass ;

	_iTotalLoc          = rv._iTotalLoc ;
  _iTotalEuro         = rv._iTotalEuro ;
  depassLoc           = rv.depassLoc ;
  depassEuro          = rv.depassEuro ;
  paieLoc             = rv.paieLoc ;
  paieEuro            = rv.paieEuro ;
  espLoc              = rv.espLoc ;
  espEuro             = rv.espEuro ;
  chqLoc              = rv.chqLoc ;
  chqEuro             = rv.chqEuro ;
  virLoc              = rv.virLoc ;
  virEuro             = rv.virEuro ;
  cbLoc               = rv.cbLoc ;
  cbEuro              = rv.cbEuro ;
  impayeLoc           = rv.impayeLoc ;
  impayeEuro          = rv.impayeEuro ;
  impTPLoc            = rv.impTPLoc ;
  impTPEuro           = rv.impTPEuro ;
  impAutreLoc         = rv.impAutreLoc ;
  impAutreEuro        = rv.impAutreEuro ;

  _iTotalAvecDepassLoc  = rv._iTotalAvecDepassLoc ;
  _iTotalAvecDepassEuro = rv._iTotalAvecDepassEuro ;
}
NSSomActData&NSSomActData::operator=(const NSSomActData& src)
{
	if (this == &src)
		return *this ;

	_iNbTotal           = src._iNbTotal ;
  _iNbTotalAvecDepass = src._iNbTotalAvecDepass ;

	_iTotalLoc          = src._iTotalLoc ;
  _iTotalEuro         = src._iTotalEuro ;
  depassLoc           = src.depassLoc ;
  depassEuro          = src.depassEuro ;
  paieLoc             = src.paieLoc ;
  paieEuro            = src.paieEuro ;
  espLoc              = src.espLoc ;
  espEuro             = src.espEuro ;
  chqLoc              = src.chqLoc ;
  chqEuro             = src.chqEuro ;
  virLoc              = src.virLoc ;
  virEuro             = src.virEuro ;
  cbLoc               = src.cbLoc ;
  cbEuro              = src.cbEuro ;
  impayeLoc           = src.impayeLoc ;
  impayeEuro          = src.impayeEuro ;
  impTPLoc            = src.impTPLoc ;
  impTPEuro           = src.impTPEuro ;
  impAutreLoc         = src.impAutreLoc ;
  impAutreEuro        = src.impAutreEuro ;

  _iTotalAvecDepassLoc  = src._iTotalAvecDepassLoc ;
  _iTotalAvecDepassEuro = src._iTotalAvecDepassEuro ;

	return *this ;
}

string
NSSomActData::GetCountsSentence()
{
  string sLabel = string("Nombre total d'actes : ") + IntToString(_iNbTotal) ;

  if (0 == _iNbTotal)
    return sLabel ;

  if (0 == _iNbTotalAvecDepass)
    sLabel += string(" (sans aucun dépassement)") ;
  else
  {
    sLabel += string(" dont ") + IntToString(_iNbTotalAvecDepass) + string(" dépassement") ;
    if (_iNbTotalAvecDepass > 1)
      sLabel += string("s") ;

		double dPourcent = double(_iNbTotalAvecDepass) / double(_iNbTotal) ;
		double chiffre   = 100 * dPourcent ;
		dPourcent = floor(chiffre) ;
		if (chiffre - dPourcent > 0.5)
			dPourcent++ ;

    sLabel += string(" (soit ") + DoubleToString(&dPourcent, 0, 0) + string("% d'actes avec dépassement") ;

    dPourcent = 100 - dPourcent ;

    sLabel += string(" et ") + DoubleToString(&dPourcent, 0, 0) + string("% d'actes sans dépassement)") ;
  }

  return sLabel ;
}

string
NSSomActData::GetDepassSentence()
{
  if ((0 == _iTotalAvecDepassEuro) && (0 == _iTotalAvecDepassLoc))
    return string("") ;

  int iTotal       = 0 ;
  int iTotalGlobal = 0 ;
  int iDepas       = 0 ;

  if ((_iTotalAvecDepassEuro > 0) && (depassEuro > 0))
  {
    iTotal       = _iTotalAvecDepassEuro ;
    iTotalGlobal = _iTotalEuro ;
    iDepas       = depassEuro ;
  }
  else if ((_iTotalAvecDepassLoc > 0) && (depassLoc > 0))
  {
    iTotal       = _iTotalAvecDepassLoc ;
    iTotalGlobal = _iTotalLoc ;
    iDepas       = depassLoc ;
  }

  if ((0 == iTotal) || (0 == iDepas))
    return string("") ;

  double dPourcent = double(iDepas) / double(iTotal) ;
  double chiffre   = 100 * dPourcent ;
  dPourcent = floor(chiffre) ;
  if (chiffre - dPourcent > 0.5)
    dPourcent++ ;

  string sLabel = string("Part moyenne du dépassement pour les actes avec dépassement : ") + DoubleToString(&dPourcent, 0, 0) + string("%") ;

  if (iTotalGlobal > iDepas)
  {
    dPourcent = double(iDepas) / double(iTotalGlobal - iDepas) ;
    chiffre   = 100 * dPourcent ;
    dPourcent = floor(chiffre) ;
    if (chiffre - dPourcent > 0.5)
      dPourcent++ ;

    sLabel += string(" (soit ") + DoubleToString(&dPourcent, 0, 0) + string("% par rapport au tarif officiel)") ;
  }

  return sLabel ;
}

//***************************************************************************// Implémentation des méthodes NSTotauxData
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NSTotauxData::NSTotauxData()
{
	//
	// Met les champs de données à zéro
	//
	sLibelle    = string("") ;
  montantLoc  = 0 ;
  montantEuro = 0 ;
}

//---------------------------------------------------------------------------//  Constructeur copie
//---------------------------------------------------------------------------
NSTotauxData::NSTotauxData(const NSTotauxData& rv)
{
  sLibelle    = rv.sLibelle ;
  montantLoc  = rv.montantLoc ;
  montantEuro = rv.montantEuro ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSTotauxData::~NSTotauxData()
{
}

//---------------------------------------------------------------------------//  Opérateur =
//---------------------------------------------------------------------------
NSTotauxData&
NSTotauxData::operator=(const NSTotauxData& src)
{
	if (this == &src)
		return *this ;

	sLibelle    = src.sLibelle ;
  montantLoc  = src.montantLoc ;
  montantEuro = src.montantEuro ;

	return *this ;
}

//***************************************************************************// 					Implémentation des méthodes NSTotauxArray
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSTotauxArray::NSTotauxArray(const NSTotauxArray& rv)
              :NSFicheTotauxArray()
{
  if (false == rv.empty())
		for (NSTotauxConstIter i = rv.begin() ; rv.end() != i ; i++)
   		push_back(new NSTotauxData(*(*i))) ;
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
void
NSTotauxArray::vider()
{
	if (false == empty())
		for (NSTotauxIter i = begin() ; end() != i ; )
   	{
   		delete *i ;
      erase(i) ;
   	}
}

NSTotauxArray::~NSTotauxArray(){
	vider() ;
}

//***************************************************************************// Implémentation des méthodes NSKCodeData
//***************************************************************************

//---------------------------------------------------------------------------//  Constructeur
//---------------------------------------------------------------------------
NSKCodeData::NSKCodeData()
{
	//
	// Met les champs de données à zéro
	//
	sKCode = string("") ;
  occur  = 0.0 ;
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSKCodeData::NSKCodeData(const NSKCodeData& rv)
{
	sKCode = rv.sKCode ;
  occur  = rv.occur ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSKCodeData::~NSKCodeData()
{
}

//---------------------------------------------------------------------------//  Opérateur =
//---------------------------------------------------------------------------
NSKCodeData&
NSKCodeData::operator=(const NSKCodeData& src)
{
  if (this == &src)
		return *this ;

	sKCode = src.sKCode ;
  occur  = src.occur ;

	return *this ;}

//---------------------------------------------------------------------------//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSKCodeData::operator==(const NSKCodeData& o)
{
	if ((sKCode == o.sKCode) &&
   	  (occur  == o.occur))
		return 1 ;
	else
		return 0 ;
}

//***************************************************************************// 					Implémentation des méthodes NSKCodeArray
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSKCodeArray::NSKCodeArray(const NSKCodeArray& rv)
             :NSFicheKCodeArray()
{
	if (false == rv.empty())
		for (NSKCodeConstIter i = rv.begin() ; rv.end() != i ; i++)
   		push_back(new NSKCodeData(*(*i))) ;
}

NSKCodeArray&NSKCodeArray::operator=(const NSKCodeArray& src)
{
  if (this == &src)
		return *this ;

	vider() ;

	if (false == src.empty())
		for (NSKCodeConstIter i = src.begin() ; src.end() != i ; i++)
   		push_back(new NSKCodeData(*(*i))) ;

   return *this ;
}

intNSKCodeArray::operator==(const NSKCodeArray& o)
{
	if (size() != o.size())
   	return 0 ;

  NSKCodeConstIter i = begin() ;
  NSKCodeConstIter j = o.begin() ;

	for ( ; end() != i ; i++, j++)
   	if (!((*(*i)) == (*(*j)))) return 0 ;

   return 1 ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
void
NSKCodeArray::vider()
{
	if (false == empty())
		for (NSKCodeIter i = begin() ; end() != i ; )
		{
   		delete *i ;
    	erase(i) ;
		}
}

NSKCodeArray::~NSKCodeArray(){
	vider() ;
}

//***************************************************************************
// Implémentation des méthodes NSCCAMCodeData
//***************************************************************************

//---------------------------------------------------------------------------//  Constructeur
//---------------------------------------------------------------------------
NSCCAMCodeData::NSCCAMCodeData()
{
	//
	// Met les champs de données à zéro
	//
	_sCCAMCode = string("") ;
  _sCCAMlib  = string("") ;
  _dNbre     = 0.0 ;
  _dSomTotal = 0.0 ;
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSCCAMCodeData::NSCCAMCodeData(const NSCCAMCodeData& rv)
{
	_sCCAMCode = rv._sCCAMCode ;
  _sCCAMlib  = rv._sCCAMlib ;
  _dNbre     = rv._dNbre ;
  _dSomTotal = rv._dSomTotal ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSCCAMCodeData::~NSCCAMCodeData()
{
}

//---------------------------------------------------------------------------//  Opérateur =
//---------------------------------------------------------------------------
NSCCAMCodeData&
NSCCAMCodeData::operator=(const NSCCAMCodeData& src)
{
  if (this == &src)
		return *this ;

	_sCCAMCode = src._sCCAMCode ;
  _sCCAMlib  = src._sCCAMlib ;
  _dNbre     = src._dNbre ;
  _dSomTotal = src._dSomTotal ;

	return *this ;}

//---------------------------------------------------------------------------//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSCCAMCodeData::operator==(const NSCCAMCodeData& o)
{
	if ((_sCCAMCode == o._sCCAMCode) &&
      (_sCCAMlib  == o._sCCAMlib)  &&
      (_dNbre     == o._dNbre)     &&
   	  (_dSomTotal == o._dSomTotal))
		return 1 ;
	else
		return 0 ;
}

//***************************************************************************// 					Implémentation des méthodes NSKCodeArray
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSCCAMCodeArray::NSCCAMCodeArray(const NSCCAMCodeArray& rv)
                :NSFicheCCAMCodeArray()
{
	if (false == rv.empty())
		for (NSCCAMCodeConstIter i = rv.begin(); rv.end() != i ; i++)
   		push_back(new NSCCAMCodeData(*(*i))) ;
}

NSCCAMCodeArray&NSCCAMCodeArray::operator=(const NSCCAMCodeArray& src)
{
	if (this == &src)
		return *this ;

	vider() ;

	if (false == src.empty())
		for (NSCCAMCodeConstIter i = src.begin() ; src.end() != i ; i++)
   		push_back(new NSCCAMCodeData(*(*i))) ;

	return *this ;
}

intNSCCAMCodeArray::operator==(const NSCCAMCodeArray& o)
{
	if (size() != o.size())
   	return 0 ;

  NSCCAMCodeConstIter i = begin() ;
  NSCCAMCodeConstIter j = o.begin() ;

	for ( ; end() != i ; i++, j++)
   	if (!((*(*i)) == (*(*j))))
      return 0 ;

   return 1 ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
void
NSCCAMCodeArray::vider()
{
	if (false == empty())
		for (NSCCAMCodeIter i = begin() ; end() != i ; )
		{
   		delete *i ;
    	erase(i) ;
		}
}

NSCCAMCodeArray::~NSCCAMCodeArray(){
	vider() ;
}

//***************************************************************************
// Implémentation des méthodes NSExamData
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NSExamData::NSExamData()
{
	//
	// Met les champs de données à zéro
	//
  sCodeExam = string("") ;
  nbExam    = 0 ;
}

//---------------------------------------------------------------------------//  Constructeur copie
//---------------------------------------------------------------------------
NSExamData::NSExamData(const NSExamData& rv)
{
  sCodeExam   = rv.sCodeExam ;
  nbExam      = rv.nbExam ;
  aKCodeArray = rv.aKCodeArray ;
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
NSExamData::~NSExamData()
{
}

//---------------------------------------------------------------------------//  Opérateur =
//---------------------------------------------------------------------------
NSExamData&
NSExamData::operator=(const NSExamData& src)
{
	if (this == &src)
		return *this ;

	sCodeExam 	= src.sCodeExam ;
	nbExam		  = src.nbExam ;
	aKCodeArray = src.aKCodeArray ;

	return *this ;
}

//---------------------------------------------------------------------------//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSExamData::operator==(const NSExamData& o)
{
	if ((sCodeExam 	 == o.sCodeExam) &&
   	  (nbExam  		 == o.nbExam)		 &&
      (aKCodeArray == o.aKCodeArray))
		return 1 ;
	else
		return 0 ;
}

//***************************************************************************
// 					Implémentation des méthodes NSExamArray
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSExamArray::NSExamArray(const NSExamArray& rv)
            :NSFicheExamArray()
{
	if (false == rv.empty())
		for (NSExamConstIter i = rv.begin() ; rv.end() != i ; i++)   		push_back(new NSExamData(*(*i))) ;
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
void
NSExamArray::vider()
{
	if (false == empty())
		for (NSExamIter i = begin() ; end() != i ; )   	{
   		delete *i ;
      erase(i) ;
   	}
}

NSExamArray::~NSExamArray()
{
	vider() ;
}

//***************************************************************************// Implémentation des méthodes NSListActData
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NSListActData::NSListActData(NSContexte* pCtx)
              :patInfo(pCtx->getSuperviseur()), NSRoot(pCtx)
{
	//
	// Met les champs de données à zéro
	//	metAZero() ;
}

//---------------------------------------------------------------------------//  Constructeur copie
//---------------------------------------------------------------------------
NSListActData::NSListActData(const NSListActData& rv)
              :patInfo(rv.patInfo), NSRoot(rv.pContexte)
{
	strcpy(numCompt,   rv.numCompt) ;
	strcpy(dateCompt,  rv.dateCompt) ;
	strcpy(nomPatient, rv.nomPatient) ;
	strcpy(libExam,    rv.libExam) ;
	strcpy(actes,	     rv.actes) ;
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
NSListActData::~NSListActData()
{
}

//---------------------------------------------------------------------------
//  Opérateur =
//---------------------------------------------------------------------------
NSListActData&
NSListActData::operator=(const NSListActData& src)
{
	if (this == &src)
		return *this ;

	strcpy(numCompt,	 src.numCompt) ;	strcpy(dateCompt,	 src.dateCompt) ;
	strcpy(nomPatient, src.nomPatient) ;
	strcpy(libExam, 	 src.libExam) ;
	strcpy(actes,			 src.actes) ;

	patInfo = src.patInfo ;

	return *this ;
}

//---------------------------------------------------------------------------//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSListActData::operator==(const NSListActData& o)
{
	if ((strcmp(numCompt,		o.numCompt)   == 0) &&
   	  (strcmp(dateCompt,  o.dateCompt)  == 0) &&
   	  (strcmp(nomPatient, o.nomPatient) == 0) &&
   	  (strcmp(libExam, 		o.libExam)    == 0) &&
      (strcmp(actes,			o.actes)      == 0) &&
      (patInfo 		== 		o.patInfo))
		return 1 ;
	else
		return 0 ;
}

//---------------------------------------------------------------------------
//  Met à zéro les variables de la fiche
//---------------------------------------------------------------------------
void
NSListActData::metAZero()
{
	//
	// Met les champs de données à zéro
	//
  memset(numCompt,   0, CPTA_NUMCOMPT_LEN + 1) ;
	memset(dateCompt,  0, CPTA_DATE_LEN + 1) ;
  memset(nomPatient, 0, PAT_NOM_LONG_LEN + 1) ;
	memset(libExam,    0, IMP_LIB_EXAM_LEN + 1) ;
  memset(actes,      0, AGA_ACTES_LEN + 1) ;

#ifndef _MUE
  patInfo.pDonnees->metAZero() ;
#endif
}
//***************************************************************************// 					Implémentation des méthodes NSListActArray
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSListActArray::NSListActArray(const NSListActArray& rv)
               :NSFicheListActArray()
{
	if (false == rv.empty())
		for (NSListActConstIter i = rv.begin() ; rv.end() != i ; i++)
   		push_back(new NSListActData(*(*i))) ;
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
void
NSListActArray::vider()
{
	if (false == empty())
		for (NSListActIter i = begin() ; end() != i ; )   	{
   		delete *i ;
      erase(i) ;
   	}
}

NSListActArray::~NSListActArray(){
	vider() ;
}

//***************************************************************************// Implémentation des méthodes NSEncaissData
//***************************************************************************

//---------------------------------------------------------------------------//  Constructeur
//---------------------------------------------------------------------------
NSEncaissData::NSEncaissData()
{
	//
	// Met les champs de données à zéro
	//
	paieLoc  = 0 ;
	paieEuro = 0 ;
  espLoc 	 = 0 ;
  espEuro  = 0 ;
  chqLoc 	 = 0 ;
  chqEuro  = 0 ;
  virLoc 	 = 0 ;
  virEuro  = 0 ;
  cbLoc 	 = 0 ;
  cbEuro 	 = 0 ;
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSEncaissData::NSEncaissData(const NSEncaissData& rv)
{
	paieLoc  = rv.paieLoc ;
	paieEuro = rv.paieEuro ;
  espLoc 	 = rv.espLoc ;
  espEuro  = rv.espEuro ;
  chqLoc 	 = rv.chqLoc ;
  chqEuro  = rv.chqEuro ;
  virLoc 	 = rv.virLoc ;
  virEuro  = rv.virEuro ;
  cbLoc 	 = rv.cbLoc ;
  cbEuro 	 = rv.cbEuro ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSEncaissData::~NSEncaissData()
{
}

//---------------------------------------------------------------------------
//  Opérateur =
//---------------------------------------------------------------------------
NSEncaissData&
NSEncaissData::operator=(const NSEncaissData& src)
{
	if (this == &src)
		return *this ;

	paieLoc  = src.paieLoc ;
  paieEuro = src.paieEuro ;
  espLoc 	 = src.espLoc ;
  espEuro  = src.espEuro ;
  chqLoc 	 = src.chqLoc ;
  chqEuro  = src.chqEuro ;
  virLoc 	 = src.virLoc ;
  virEuro  = src.virEuro ;
  cbLoc 	 = src.cbLoc ;
  cbEuro 	 = src.cbEuro ;

	return *this ;
}

//---------------------------------------------------------------------------//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSEncaissData::operator==(const NSEncaissData& o)
{
	if ((paieLoc	== o.paieLoc) 	&&
   	  (paieEuro == o.paieEuro)	&&
      (espLoc		== o.espLoc)	  &&
      (espEuro	== o.espEuro)	  &&
      (chqLoc		== o.chqLoc)	  &&
      (chqEuro	== o.chqEuro)	  &&
      (virLoc		== o.virLoc)	  &&
      (virEuro	== o.virEuro)	  &&
      (cbLoc		== o.cbLoc)		  &&
      (cbEuro		== o.cbEuro))
		return 1 ;
	else
		return 0 ;
}

//***************************************************************************// 					Implémentation des méthodes NSEncaissArray
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSEncaissArray::NSEncaissArray(const NSEncaissArray& rv)
               :NSFicheEncaissArray()
{
	if (false == rv.empty())
		for (NSEncaissConstIter i = rv.begin() ; rv.end() != i ; i++)   		push_back(new NSEncaissData(*(*i))) ;
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
void
NSEncaissArray::vider()
{
	if (false == empty())
		for (NSEncaissIter i = begin() ; end() != i ; )   	{
   		delete *i ;
      erase(i) ;
   	}
}

NSEncaissArray::~NSEncaissArray()
{
	vider() ;
}

//***************************************************************************// 					Implémentation des méthodes NSMultiCriteres
//***************************************************************************

NSMultiCriteres::NSMultiCriteres()
{
	bActesPerso			= false ;
	sDate1				  = string("") ;
	sDate2				  = string("") ;
	sCodeExamen 		= string("") ;
	sSynExamen			= string("") ;
	sCodePrescript 	= string("") ;
	sTitrePrescript	= string("") ;
	iImpayes				= 0 ;
	iContexte			  = 0 ;
	sCodeOrga			  = string("") ;
	sLibCourtOrga 	= string("") ;
	sLibLongOrga		= string("") ;

  bInterruptedProcess = false ;
}

NSMultiCriteres::NSMultiCriteres(const NSMultiCriteres& rv){
	bActesPerso			= rv.bActesPerso ;
	sDate1				  = rv.sDate1 ;
  sDate2				  = rv.sDate2 ;
  sCodeExamen 		= rv.sCodeExamen ;
  sSynExamen			= rv.sSynExamen ;
  sCodePrescript 	= rv.sCodePrescript ;
  sTitrePrescript	= rv.sTitrePrescript ;
  iImpayes				= rv.iImpayes ;
  iContexte			  = rv.iContexte ;
  sCodeOrga			  = rv.sCodeOrga ;
  sLibCourtOrga 	= rv.sLibCourtOrga ;
  sLibLongOrga		= rv.sLibLongOrga ;

  bInterruptedProcess = rv.bInterruptedProcess ;
}

NSMultiCriteres&NSMultiCriteres::operator=(const NSMultiCriteres& src)
{
	if (this == &src)
		return *this ;

	bActesPerso			= src.bActesPerso ;
	sDate1				  = src.sDate1 ;
  sDate2				  = src.sDate2 ;
  sCodeExamen 		= src.sCodeExamen ;
  sSynExamen			= src.sSynExamen ;
  sCodePrescript 	= src.sCodePrescript ;
  sTitrePrescript = src.sTitrePrescript ;
  iImpayes				= src.iImpayes ;
  iContexte			  = src.iContexte ;
  sCodeOrga			  = src.sCodeOrga ;
  sLibCourtOrga		= src.sLibCourtOrga ;
  sLibLongOrga		= src.sLibLongOrga ;

  bInterruptedProcess = src.bInterruptedProcess ;

	return *this ;
}

NSMultiCriteres::~NSMultiCriteres(){
}

string
NSMultiCriteres::getSummaryString(NSContexte* pCtx)
{
	string sLang = string("") ;
  if (pCtx)
  	sLang = pCtx->getUserLanguage() ;

	string sReturnString = string("") ;
  if (bActesPerso)
		sReturnString += "vos actes" ;
	else
  	sReturnString += "actes" ;

	if ((string("") != sCodeExamen) && (string("") != sSynExamen))
	{
  	string sCodeComplet = sCodeExamen + sSynExamen ;
    string sLibExam ;
    pCtx->getDico()->donneLibelle(sLang, &sCodeComplet, &sLibExam) ;

  	if (string("") != sReturnString)
    	sReturnString += string(" ") ;
  	sReturnString += string("(") + sLibExam + string(")") ;
	}

  if (iContexte != 0)
	{
  	if (string("") != sReturnString)
    	sReturnString += string(" ") ;
    if      (1 == iContexte)
    	sReturnString += string("de contexte non précisé") ;
    else if (2 == iContexte)
    	sReturnString += string("externes") ;
    else if (4 == iContexte)
    	sReturnString += string("hospitalisés") ;
    else if (8 == iContexte)
    	sReturnString += string("ambulatoires") ;
	}

  if (iImpayes != 0)
	{
  	if (string("") != sReturnString)
    	sReturnString += string(" ") ;
    if      (1 == iImpayes)
    	sReturnString += string("impayés") ;
    else if (2 == iImpayes)
    	sReturnString += string("payés") ;
	}

  if (string("") != sCodeOrga)
	{
  	if (string("") != sReturnString)
    	sReturnString += string(" ") ;
  	sReturnString += string("dûs par ") + sLibCourtOrga ;
	}

  if (string("") != sDate1)
  {
    if (string("") != sReturnString)
    	sReturnString += string(" ") ;
    sReturnString += string("du ") + donne_date(sDate1, sLang) ;
  }
  if ((string("") != sDate2) && (sDate2 != sDate1))
  {
    if (string("") != sReturnString)
    	sReturnString += string(" ") ;
    sReturnString += string("au ") + donne_date(sDate2, sLang) ;
  }

  if (string("") != sTitrePrescript)
	{
  	if (string("") != sReturnString)
    	sReturnString += string(" ") ;
  	sReturnString += string("prescrits par ") + sTitrePrescript ;
	}

  if (true == bInterruptedProcess)
    sReturnString += string(" (résultats partiels, processus interrompu en cours de recherche)") ;

  sReturnString[0] = pseumaj(sReturnString[0]) ;

  return sReturnString ;
}

// fin de nsagavar.cpp