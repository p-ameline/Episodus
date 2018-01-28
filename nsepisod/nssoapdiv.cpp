// --------------------------------------------------------------------------// NSSOAPVIEW.CPP		Document/Vues SOAP
// --------------------------------------------------------------------------
// Fabrice LE PERRU - Novembre/Décembre 2001
// source : Fabrice LE PERRU -- nsepisodview.cpp - Aout 2001
//          -- source : Rémi SPAAK - Mai 1997
// --------------------------------------------------------------------------
// #include <owl/uihelper.h>

// #include <owl\owlpch.h>
// #include <owl\validate.h>
// #include <owl\inputdia.h>
// #include <fstream.h>

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/superLus.h"
  #include "enterpriseLus/nsglobalLus.h"
  #include "enterpriseLus/nsdivfctForCgi.h"
#else
  #include "nautilus\nssuper.h"
  #include "partage\nsglobal.h"
  #include "partage\nsdivfct.h"
#endif

#include "nsepisod\nsSOAPdiv.h"

// #include "nautilus\ns_html.h"
// #include "nautilus\nscompub.h"		// pour les formules
// #include "nautilus\nsresour.h"      // pour AutoWordDialog
// #include "nautilus\nsvisual.h"
// #include "nautilus\nshistdo.h"
// #include "nautilus\nscsdoc.h"

#include "nssavoir\nscaptur.h"
#include "nssavoir\nsfilgd.h"

// #include "nssavoir\nspatho.h"
// #include "nssavoir\nspathor.h"
// #include "nssavoir\nsgraphe.h"
// #include "nautilus\nsldvdoc.h"

// --------------------------------------------------------------------------
//
//                          Class SOAPObject
//
// --------------------------------------------------------------------------

SOAPObject::SOAPObject()
{
  sTexteInitial   = "";
  sCode           = "";
  sClassification = "";
  iConfidence     = 0;

  sCase           = "";
  iImageIndex     = -1;

  iOldListParam   = -1;
  iListParam      = -1;

  pCaptElemnt     = 0;
  sNoeud          = "";
  pFilsItem       = 0;

  bFloculable     = true;
}

SOAPObject::SOAPObject(NSCapture* pCapt)
{
  sTexteInitial   = pCapt->getLibelle() ;
  sClassification = pCapt->getClassifier() ;

  initFromCapture(pCapt);

  sCase           = "";
  iImageIndex     = -1;

  iOldListParam   = -1;
  iListParam      = -1;

  pCaptElemnt     = pCapt;
  sNoeud          = "";
  pFilsItem       = 0;

  bFloculable     = true;
}

SOAPObject::SOAPObject(string sTI, string sCd, string sCl, int iCf, string sNd)
{
  sTexteInitial   = sTI;
  sCode           = sCd;
  sClassification = sCl;
  iConfidence     = iCf;

  sCase           = "";
  iImageIndex     = -1;

  iOldListParam   = -1;
  iListParam      = -1;

  pCaptElemnt     = 0;
  sNoeud          = sNd;
  pFilsItem       = 0;

  bFloculable     = true;
}

SOAPObject::SOAPObject(string sTI, string sCd, string sCl, int iCf, BBFilsItem* pItem)
{  sTexteInitial   = sTI;
  sCode           = sCd;
  sClassification = sCl;
  iConfidence     = iCf;

  sCase           = "";
  iImageIndex     = -1;

  iOldListParam   = -1;
  iListParam      = -1;

  pCaptElemnt     = 0;
  sNoeud          = "";
  pFilsItem       = pItem;

  bFloculable     = true;
}
SOAPObject::SOAPObject(const SOAPObject &rv){
  sTexteInitial   = rv.sTexteInitial;
  sCode           = rv.sCode;
  sClassification = rv.sClassification;
  iConfidence     = rv.iConfidence;
  sCase           = rv.sCase;
  iImageIndex     = rv.iImageIndex;
  pCaptElemnt     = rv.pCaptElemnt;
  sNoeud          = rv.sNoeud;
  pFilsItem       = rv.pFilsItem;
  bFloculable     = rv.bFloculable;

  iOldListParam   = -1;
  iListParam      = -1;
}

SOAPObject::SOAPObject(SOAPObject *pNewSOAPObject){
  if (pNewSOAPObject)
  {
    sTexteInitial   = pNewSOAPObject->sTexteInitial;
    sCode           = pNewSOAPObject->sCode;
    sClassification = pNewSOAPObject->sClassification;
    iConfidence     = pNewSOAPObject->iConfidence;
    sCase           = pNewSOAPObject->sCase;
    iImageIndex     = pNewSOAPObject->iImageIndex;
    pCaptElemnt     = pNewSOAPObject->pCaptElemnt;
    sNoeud          = pNewSOAPObject->sNoeud;
    pFilsItem       = pNewSOAPObject->pFilsItem;
    bFloculable     = pNewSOAPObject->bFloculable;
  }
  else
  {
    sTexteInitial   = "";
    sCode           = "";
    sClassification = "";
    iConfidence     = 0;
    sCase           = "";
    iImageIndex     = -1;
    pCaptElemnt     = 0;
    sNoeud          = "";
    pFilsItem       = 0;
    bFloculable     = true;
  }
  iOldListParam   = -1;
  iListParam      = -1;
}

SOAPObject::~SOAPObject(){
}

void
SOAPObject::initFromCapture(NSCapture* pCapt)
{
  //
  // Code et degré de confiance
  //
  if (pCapt->getClassifResultP() != "")
  {
    size_t pos  = pCapt->getClassifResultP().find(";") ;
    size_t pos2 = pCapt->getClassifResultP().find("-") ;
    if ((pos == NPOS) && (pos2 == NPOS))
    {
      sCode       = pCapt->getClassifResultP() ;
      iConfidence = 100 ;
    }
    else
    {
      sCode       = string("") ;
      // sCode       = string(pCapt->sClassifResultP, 0, pos);
      iConfidence = 50 ;
    }
  }
  else
  {
    sCode       = string("") ;
    iConfidence = 0 ;
  }
}

string
SOAPObject::donneLibelle()
{
  string sLibelle = sTexteInitial;

  if (sCode != "")
  {
      sLibelle += string(" (");
      if      (sClassification == "6CISP")
          sLibelle += string("cisp");
      else if (sClassification == "6CIMA")
          sLibelle += string("cim");
      sLibelle += sCode;
      sLibelle += string(")");
  }

  return sLibelle;
}

string
SOAPObject::donneChapitre()
{
  if (sCode == "")
  	return string("") ;

  return string(sCode, 0, 1) ;
}

SOAPObject&
SOAPObject::operator=(const SOAPObject& src)
{
	if (this == &src)
  	return *this ;

  sTexteInitial   = src.sTexteInitial;
  sCode           = src.sCode;
  sClassification = src.sClassification;
  iConfidence     = src.iConfidence;
  sCase           = src.sCase;
  iImageIndex     = src.iImageIndex;
  pCaptElemnt     = src.pCaptElemnt;
  sNoeud          = src.sNoeud;
  pFilsItem       = src.pFilsItem;

  iOldListParam   = src.iOldListParam;
  iListParam      = src.iListParam;

  return *this;
}

// --------------------------------------------------------------------------
//
//                          Class SOAPObjectArray
//
// --------------------------------------------------------------------------

SOAPObjectArray::SOAPObjectArray(ClassificationPrinciple* pPrinc, bool bOwnPointers)
                :ArraySOAPObject()
{
    pPrincipe   = pPrinc;
    bOwner      = bOwnPointers;
}

SOAPObject*
SOAPObjectArray::donneObjet(int iListParam)
{
    if (empty())
        return 0;

    for (ArraySOAPObjectIter i = begin(); i != end(); i++)
        if ((*i)->iListParam == iListParam)
            return *i;

    return 0;
}

int
SOAPObjectArray::donneNextListParam()
{
    if (empty())
        return 0;

    int iMaxParam = 0;
    for (ArraySOAPObjectIter i = begin(); i != end(); i++)
        if ((*i)->iListParam > iMaxParam)
            iMaxParam = (*i)->iListParam;

    return iMaxParam + 1;
}

SOAPObjectArray::SOAPObjectArray(const SOAPObjectArray &rv)
                :ArraySOAPObject()
{
try
{
  pPrincipe = rv.pPrincipe;
  bOwner    = rv.bOwner;

  if (rv.empty())
  	return ;

  for (ArraySOAPObjectConstIter SOAPIter = rv.begin(); rv.end() != SOAPIter ; SOAPIter++)
  {
    if (bOwner)
      push_back(new SOAPObject(*(*SOAPIter))) ;
    else
      push_back(*SOAPIter) ;
  }
}
catch (...)
{
	erreur("Exception SOAPObjectArray copy ctor.", standardError);
}
}

SOAPObjectArray::SOAPObjectArray(SOAPObjectArray *pNewSOAPObjectArray)
                :ArraySOAPObject()
{
try
{
  pPrincipe = pNewSOAPObjectArray->pPrincipe ;
  bOwner    = pNewSOAPObjectArray->bOwner ;

  if ((NULL == pNewSOAPObjectArray) || (pNewSOAPObjectArray->empty()))
  	return ;

  for (ArraySOAPObjectIter SOAPIter = pNewSOAPObjectArray->begin(); SOAPIter != pNewSOAPObjectArray->end() ; SOAPIter++)
  {
  	if (bOwner)
    	push_back(new SOAPObject(*(*SOAPIter))) ;
    else
    	push_back(*SOAPIter) ;
  }
}
catch (...)
{
	erreur("Exception SOAPObjectArray(SOAPObjectArray*).", standardError) ;
}
}

SOAPObjectArray::~SOAPObjectArray(){
  vider() ;
}

SOAPObjectArray&
SOAPObjectArray::operator=(const SOAPObjectArray& src)
{
	if (this == &src)
  	return *this ;

try
{
  vider() ;

  pPrincipe = src.pPrincipe;
  bOwner    = src.bOwner;

  if (src.empty())
    return *this ;

  for (ArraySOAPObjectConstIter SOAPIter = src.begin() ; src.end() != SOAPIter ; SOAPIter++)
  {
    if (bOwner)
      push_back(new SOAPObject(*(*SOAPIter))) ;
    else
      push_back(*SOAPIter) ;
  }

  return *this ;
}
catch (...)
{
	erreur("Exception SOAPObjectArray (op =).", standardError) ;
  return *this ;
}
}

void
SOAPObjectArray::vider()
{
  if (empty())
    return ;

  for (ArraySOAPObjectIter i = begin() ; end() != i ; )
  {
    if (bOwner)
      delete *i ;
    erase(i) ;
  }
}

// --------------------------------------------------------------------------
//
//                          Class SOAPBasket
//
// --------------------------------------------------------------------------

SOAPBasket::SOAPBasket(PrinciplesArray* pPrincips, SOAPObject* /* pInitObject */)
           :ArraySOAPObjectArray()
{
try
{
	pPrincipes = pPrincips ;

	if ((!pPrincipes) || (pPrincipes->empty()))
		return ;

	PrinciplesIter iPrincipes = pPrincipes->begin() ;

	for (; iPrincipes != pPrincipes->end(); iPrincipes++)
  	push_back(new SOAPObjectArray(*iPrincipes)) ;
}
catch (...)
{
	erreur("Exception SOAPBasket ctor.", standardError);
}
}

SOAPObjectArray*
SOAPBasket::trouveCase(ClassificationPrinciple* pPrincipe)
{
    if (empty())
        return 0;

    for (SOAPBasketIter i = begin(); i != end(); i++)
        if ((*i)->pPrincipe == pPrincipe)
            return (*i);

    return 0;
}

SOAPObject*
SOAPBasket::donneDiagObject()
{
    ClassificationPrinciple* pPrA = pPrincipes->trouvePrincipe("0SOA41");
    if (!pPrA)
        return NULL;

    SOAPObjectArray* pSlotA = trouveCase(pPrA);
    if ((!pSlotA) || (pSlotA->empty()))
        return NULL;

    SOAPObject* pObjectA = *(pSlotA->begin());

    return pObjectA ;
}

string
SOAPBasket::donneChapitre()
{
    //
    // Le chapitre est constitué de la première lettre du code 'A'
    //
    if (empty())
        return string("");

    //
    // Trouver le principe qui correspond au diagnostic
    //
    ClassificationPrinciple* pPrincA = pPrincipes->trouvePrincipe(string("0SOA41"));

    if (!pPrincA)
        return string("");

    //
    // Trouver la case du panier qui correspond à ce principe
    //
    SOAPObjectArray* pCaseA = trouveCase(pPrincA);
    if ((!pCaseA) || (pCaseA->empty()))        return string("");
    ArraySOAPObjectIter i = pCaseA->begin();
    return (*i)->donneChapitre();
}

boolSOAPBasket::isEmpty()
{
    if (empty())
        return true;

    for (SOAPBasketIter i = begin(); i != end() ; i++)
        if (!((*i)->empty()))
            return false;

    return true;
}

SOAPBasket::SOAPBasket(const SOAPBasket &rv)
           :ArraySOAPObjectArray()
{
try
{
  if (rv.empty())
    return ;

  for (SOAPBasketConstIter iBastket = rv.begin() ; rv.end() != iBastket ; iBastket++)
    push_back(new SOAPObjectArray(*(*iBastket))) ;
}
catch (...)
{
  erreur("Exception SOAPBasket copy ctor.", standardError) ;
}
}

SOAPBasket::~SOAPBasket(){
  vider() ;
}

SOAPBasket&
SOAPBasket::operator=(const SOAPBasket& src)
{
  if (this == &src)
  	return *this ;

try
{
  vider() ;

  if (false == src.empty())
    for (SOAPBasketConstIter iBastket = src.begin() ; src.end() != iBastket ; iBastket++)
      push_back(new SOAPObjectArray(*(*iBastket))) ;

  return *this ;
}
catch (...)
{
    erreur("Exception SOAPBasket =.", standardError);
    return *this;
}
}

void
SOAPBasket::vider()
{
    if (empty())
        return;

    for (SOAPBasketIter i = begin(); i != end(); )
    {
        delete *i;
        erase(i);
    }
}

// --------------------------------------------------------------------------
//
//                          Class SOAPBasketArray
//
// --------------------------------------------------------------------------

SOAPBasketArray::SOAPBasketArray()
                :ArraySOAPBasket()
{
}

SOAPBasketArray::SOAPBasketArray(const SOAPBasketArray &rv)
                :ArraySOAPBasket()
{
try
{
  if (rv.empty())
    return ;

  for (SOAPBasketArrayConstIter i = rv.begin(); rv.end() != i ; i++)    push_back(new SOAPBasket(*(*i))) ;}
catch (...)
{
  erreur("Exception SOAPBasketArray copy ctor.", standardError);
}
}

SOAPBasketArray::~SOAPBasketArray()
{
  vider() ;
}

SOAPBasketArray&
SOAPBasketArray::operator=(const SOAPBasketArray& src)
{
  if (this == &src)
    return *this ;

try
{
  vider();

  if (false == src.empty())
    for (SOAPBasketArrayConstIter i = src.begin(); src.end() != i ; i++)
      push_back(new SOAPBasket(*(*i))) ;

  return *this ;
}
catch (...)
{  erreur("Exception SOAPBasketArray =.", standardError) ;  return *this ;
}
}

void
SOAPBasketArray::vider()
{
    if (empty())
        return;

    for (SOAPBasketArrayIter i = begin(); i != end(); )
    {
        delete *i;
        erase(i);
    }
}

// --------------------------------------------------------------------------
//
//                          Class ClassificationPrinciple
//
// --------------------------------------------------------------------------

ClassificationPrinciple::ClassificationPrinciple(string sType, string sClassif, bool bUnik, string sLibel){
  sCase           = sType;
  sClassification = sClassif;
  bUnique         = bUnik;
  sLibelle        = sLibel;
}

ClassificationPrinciple::ClassificationPrinciple(const ClassificationPrinciple &rv){
  sCase           = rv.sCase;
  sClassification = rv.sClassification;
  bUnique         = rv.bUnique;
  sLibelle        = rv.sLibelle;
}

ClassificationPrinciple::~ClassificationPrinciple(){
}

ClassificationPrinciple&ClassificationPrinciple::operator=(const ClassificationPrinciple& src)
{
	if (this == &src)
  	return *this ;

  sCase           = src.sCase;  sClassification = src.sClassification;
  bUnique         = src.bUnique;
  sLibelle        = src.sLibelle;

  return *this;
}

boolClassificationPrinciple::operator==(const ClassificationPrinciple& src)
{
  if ((sCase           == src.sCase) &&
      (sClassification == src.sClassification) &&
      (bUnique         == src.bUnique))
  	return true ;
  else
  	return false ;
}

// --------------------------------------------------------------------------//
//                          Class PrinciplesArray
//
// --------------------------------------------------------------------------
PrinciplesArray::PrinciplesArray(const PrinciplesArray &rv)
                :ArrayPrinciples(), NSSuperRoot(rv._pSuper)
{
try
{
  if (rv.empty())
  	return ;

  for (PrinciplesConstIter Iter = rv.begin() ; rv.end() != Iter ; Iter++)
  	push_back(new ClassificationPrinciple(*(*Iter))) ;
}
catch (...)
{
	erreur("Exception PrinciplesArray copy ctor.", standardError) ;}
}

//
//
ClassificationPrinciple*
PrinciplesArray::trouvePrincipe(string sCase)
{
  if (empty())
  	return (ClassificationPrinciple*) 0 ;

  string sCaseSens ;
  NSDico::donneCodeSens(&sCase, &sCaseSens) ;
  size_t iSensLen = strlen(sCaseSens.c_str()) ;

  string sSensPrincipe;
  for (PrinciplesIter i = begin() ; end() != i ; i++)
  	if ((strlen((*i)->sCase.c_str()) >= iSensLen) &&
        (string((*i)->sCase, 0, iSensLen) == sCaseSens))
    	return (*i) ;

  return (ClassificationPrinciple*) 0 ;
}

// Find a classification path inside sPath, return in sExcesPath what is behind
//
ClassificationPrinciple*
PrinciplesArray::trouvePrincipe(string sPath, string sExcesPath)
{
	if (empty() || (sPath == string("")))
  	return (ClassificationPrinciple*) 0 ;

  sExcesPath = string("") ;

  string sPathSens ;
  NSDico::donneCodeSens(&sPath, &sPathSens) ;

  for (PrinciplesIter i = begin() ; end() != i ; i++)
  {
  	string sCaseSens ;
  	string sCase = (*i)->sCase ;
    NSDico::donneCodeSens(&sCase, &sCaseSens) ;

    size_t pos = sPathSens.find(sCaseSens) ;
    if (pos != NPOS)
    {
    	size_t pos2 = sPathSens.find("|", pos + 1) ;
      if (pos2 != NPOS)
      	sExcesPath = string(sPathSens, pos2+1, strlen(sPathSens.c_str())-pos2-1) ;

      return (*i) ;
    }
  }

  return (ClassificationPrinciple*) 0 ;
}

PrinciplesArray::~PrinciplesArray()
{
  this->vider() ;
}

PrinciplesArray&
PrinciplesArray::operator=(const PrinciplesArray& src)
{
	if (this == &src)
  	return *this ;

try
{
	vider() ;

  if (false == src.empty())
    for (PrinciplesConstIter Iter = src.begin() ; src.end() != Iter ; Iter++)
  	  push_back(new ClassificationPrinciple(*(*Iter))) ;

  return *this ;
}
catch (...)
{
	erreur("Exception PrinciplesArray::operator=", standardError) ;
  return *this ;
}
}

void
PrinciplesArray::vider()
{
  if (empty())
  	return ;

  for (PrinciplesIter i = begin() ; end() != i ; )
  {
  	delete *i ;
    erase(i) ;
  }
}

