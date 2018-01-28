// -----------------------------------------------------------------------------
// nsmedicphase.cpp
// -----------------------------------------------------------------------------
// $Revision: 1.7 $
// $Author: pameline $
// $Date: 2015/06/19 14:20:05 $
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// FLP  - novembre 2003
// -----------------------------------------------------------------------------

#include "nsbb\nsmedicphase.h"
#include "nsbb\nsmedicdlg.h"
#include "nsbb\nsbb.h"
#include "nsbb\nsbbtran.h"

#include "nautilus\nssuper.h"
#include "dcodeur\nsphrobj.h"

long NSphaseMedic::lObjectCount = 0 ;

// -----------------------------------------------------------------------------
// constructeurs
// -----------------------------------------------------------------------------
NSphaseMedic::NSphaseMedic(NSContexte* pCtx)
             :NSRoot(pCtx)
{
	pDialog = (NSPosoIncludeDlg*) 0 ;

	initialize() ;
}

NSphaseMedic::NSphaseMedic(NSPosoIncludeDlg *pDlg, bool auto_cycle)
             :NSRoot(pDlg->pContexte)
{
	pDialog = pDlg ;

	initialize() ;

	if (true == auto_cycle)
  	// _Cycles->push_back(new NSMedicCycleGlobal(this)) ;       // Creation d'un cycle par default
    _Cycles.push_back(new NSMedicCycleGlobal(pContexte, pDialog->_pPosoBlock)) ;
}

// -----------------------------------------------------------------------------
// constructeur copie
// -----------------------------------------------------------------------------
NSphaseMedic::NSphaseMedic(const NSphaseMedic& src)
             :NSRoot(src.pContexte)
{
	pDialog            = src.pDialog ;
	sLabel             = src.sLabel ;
	iDureeCycle        = src.iDureeCycle ;
	sUnitDureeCycle    = src.sUnitDureeCycle ;
	iNbRenouvellement  = src.iNbRenouvellement ;
  _phasepreviewIndex = src._phasepreviewIndex ;
  _tStartingDate     = src._tStartingDate ;
  _tClosingDate      = src._tClosingDate ;

  if (false == src._Cycles.empty())
  {
  	std::vector<NSMedicCycleGlobal*>::iterator iter = _Cycles.begin() ;
  	for ( ; _Cycles.end() != iter ; iter++)
    	_Cycles.push_back(new NSMedicCycleGlobal(**iter)) ;
  }
}

// -----------------------------------------------------------------------------
// destructeur
// -----------------------------------------------------------------------------
NSphaseMedic::~NSphaseMedic()
{
	EmptyCycles() ;
}

void
NSphaseMedic::initialize()
{
  sLabel             = string("") ;
  iDureeCycle        = 0 ;
  sUnitDureeCycle    = string("") ;
  iNbRenouvellement  = 0 ;
  _phasepreviewIndex = -1 ;

  _tStartingDate.init() ;
  _tClosingDate.init() ;

  _currentCycle = 0 ;
  _Monocycle    = true ;
}

// -----------------------------------------------------------------------------
// operateur =
// -----------------------------------------------------------------------------
NSphaseMedic&
NSphaseMedic::operator=(const NSphaseMedic& src)
{
	if (this == &src)
		return *this ;

	pDialog            = src.pDialog ;
	sLabel             = src.sLabel ;
	iDureeCycle        = src.iDureeCycle ;
	sUnitDureeCycle    = src.sUnitDureeCycle ;
	iNbRenouvellement  = src.iNbRenouvellement ;
  _phasepreviewIndex = src._phasepreviewIndex ;
  _tStartingDate     = src._tStartingDate ;
  _tClosingDate      = src._tClosingDate ;

  EmptyCycles() ;

  if (false == src._Cycles.empty())
  {
  	std::vector<NSMedicCycleGlobal*>::const_iterator iter = _Cycles.begin() ;
  	for ( ; _Cycles.end() != iter ; iter++)
    	_Cycles.push_back(new NSMedicCycleGlobal(**iter)) ;
  }

	return (*this) ;
}

void
NSphaseMedic::EmptyCycles()
{
  if (_Cycles.empty())
    return ;

  std::vector<NSMedicCycleGlobal*>::iterator iter = _Cycles.begin() ;
  for ( ; _Cycles.end() != iter ; )
  {
    delete *iter ;
    _Cycles.erase(iter) ;
  }
}

// -----------------------------------------------------------------------------
// operateur ==
// -----------------------------------------------------------------------------
int
NSphaseMedic::operator==(const NSphaseMedic& src)
{
  if ((pDialog            == src.pDialog)         &&
      (sLabel             == src.sLabel)          &&
      (iDureeCycle        == src.iDureeCycle)     &&
      (sUnitDureeCycle    == src.sUnitDureeCycle) &&
      (iNbRenouvellement  == src.iNbRenouvellement))
    return 1 ;
  else
    return 0 ;
}

bool
NSphaseMedic::isValid()
{
/*
	if (NULL == pDialog)
		return true ;

	if (   ((pDialog->pDureePhase->getValue() == 0) && (pDialog->pCBDureePhase->getSelCode() == ""))
      || ((pDialog->pDureePhase->getValue() > 0 ) && (pDialog->pCBDureePhase->getSelCode() == ""))
      || ((pDialog->pDureePhase->getValue() == 0 )&& (pDialog->pCBDureePhase->getSelCode() != "")))
		return false ;
	else
*/
		return true ;
}

bool
NSphaseMedic::isEmpty()
{
	return ((_tStartingDate.estVide()) && (_tClosingDate.estVide()) &&
          (string("") == sLabel) && (0 == iDureeCycle) && (string("") == sUnitDureeCycle) &&
          (0 == iNbRenouvellement) &&
          _Cycles.empty()) ;
}

/*
** Renvoie le cycle courrant
*/
NSMedicCycleGlobal*
NSphaseMedic::operator[](int index)
{
  if ((index >= 0) && (index < (int)_Cycles.size()))
    return _Cycles[index] ;
  return NULL ;
}

void
NSphaseMedic::SetCurrentCycle(int index)
{
  _currentCycle = index ;

  if (_Cycles.size() > 1)
    (_Cycles[_currentCycle])->Load(false) ;
  else
    (_Cycles[_currentCycle])->Load(true) ;
   // pDialog->pDimanche->setChangeResponse(new MemFunctor<NSphaseMedic>( (NSphaseMedic*)this, &NSphaseMedic::test ) );
}

void
NSphaseMedic::test()
{
  erreur("test", standardError, 0) ;
}

/*
** Sauvegarde la phase
** Recupere les valeurs présentes dans la boite de dialogue
*/
void
NSphaseMedic::save(int iPhaseLength, string sPhaseLengthUnit, int iRenewNb, string sDateDeb, string sDateFin)
{
	iDureeCycle       = iPhaseLength ;
	sUnitDureeCycle   = sPhaseLengthUnit ;
	iNbRenouvellement = iRenewNb ;

  _tStartingDate.initFromDate(sDateDeb) ;
  _tClosingDate.initFromDate(sDateFin) ;

  SaveCurrentCycle() ;
  Decode() ;
}

/*
void
NSphaseMedic::Load()
{
	if (NULL == pDialog)
		return ;

  pDialog->pDureePhase->initControle(iDureeCycle) ;
  pDialog->pCBDureePhase->setCode(sUnitDureeCycle) ;
  pDialog->pRenouvellement->initControle(iNbRenouvellement) ;

  _currentCycle = 0 ;
  LinkToDialog(pDialog->_pPosoBlock) ;
  bool test_if_simple ;
  if (_Cycles->size() > 1) // Il faut passer en mode etendu
  {
  	pDialog->setMultiCycleMode(true) ;
    test_if_simple = true ;
	}
	else
	{
  	pDialog->setMultiCycleMode(false) ;
		test_if_simple = false ;
	}

	for (int i = 0; i < (int)_Cycles->size(); i++)
		pDialog->getViewCycle()->ChangeLabel((*_Cycles)[i]->GetRythme()->Decode(), i) ;
	(*_Cycles)[_currentCycle]->Load(test_if_simple) ;
	pDialog->getViewCycle()->PrintListAsNow() ;
	pDialog->getViewCycle()->SetSelIndex(_currentCycle) ;
}
*/

int
NSphaseMedic::CycleNumber()
{
	return _Cycles.size() ;
}

std::string
NSphaseMedic::CheckIntegrity()
{
  std::string result= "" ;
  for (size_t i = 0 ; i < _Cycles.size() ; i++)
  	result += (_Cycles[i])->IsValid() ;

  if (iDureeCycle  == 0)
  	result += "Une des phases à une durée de 0" ;

  if (sUnitDureeCycle == "")
  	result += "Une des phases n'a pas pas de choix" ;

  return result ;
}

/*
** Teste si les noeuds sont auhtorizés comme fils d'un KPATH
*/
bool TestIfPathoOkForPhase(std::string &temp)
{
  return ((std::string("VDURE") == temp )
        || (std::string("VRENO") == temp )
        || (std::string("KOUVR") == temp )
        || (std::string("KFERM") == temp )
        || (std::string("KCYTR") == temp )) ;
}

/*
** Tente de recuperer un enregistrement pour une patPahto de pahse
** iterateur de debut
** iterateur de fin
** colonne
*/
bool
NSphaseMedic::recupereDataForPhase(PatPathoIter& pptIter, PatPathoIter& pptIEnd, int Col)
{
	while (pptIter != pptIEnd)
  {
  	std::string temp = (*pptIter)->getLexiqueSens() ;
    if ((TestIfPathoOkForPhase(temp) == true) && ((*pptIter)->getColonne() <= Col))
    {
      pptIter-- ;
      return true ;
    }
    pptIter++ ;
  }
  return false ;
}

void Avance(PatPathoIter& pptIter, PatPathoIter& pptEnd)
{
	if (pptIter != pptEnd)
		pptIter++ ;
}

/*
** Ajoute un cycle globale
*/
bool
NSphaseMedic::Load(PatPathoIter& pptIter, PatPathoIter& pptIEnd, NSPosologieBlock* pPosoBlock)
{
	Avance(pptIter, pptIEnd) ;  // On avance et on mange KPAT
  if (pptIter == pptIEnd)
  	return false ;

  std::string temp = (*pptIter)->getLexiqueSens() ;
  int KAPATCol = (*pptIter)->getColonne() ;

  int col = KAPATCol;
  while ((temp != "KPHAT") && (pptIter != pptIEnd) && ( KAPATCol <= col))
  {
    if  (TestIfPathoOkForPhase(temp) == false)
      if (recupereDataForPhase(pptIter, pptIEnd, KAPATCol) == false)
        return false ;

		if ((temp == "KOUVR") && (LoadDate(pptIter, pptIEnd, &_tStartingDate) == false))
      if (recupereDataForPhase(pptIter, pptIEnd, KAPATCol) == false)
        return false ;

		if ((temp == "KFERM") && (LoadDate(pptIter, pptIEnd, &_tClosingDate) == false))
      if (recupereDataForPhase(pptIter, pptIEnd, KAPATCol) == false)
        return false ;

    if ((temp == "VDURE") && (LoadDuree(pptIter, pptIEnd) == false))
      if (recupereDataForPhase(pptIter, pptIEnd, KAPATCol) == false)
        return false ;

    if ((temp == "VRENO") && (LoadRenouvellement(pptIter, pptIEnd) == false))
    	if (recupereDataForPhase(pptIter, pptIEnd, KAPATCol) == false)
        return false ;

    if (temp == "KCYTR")
    {
    	// NSMedicCycleGlobal* cycle = new NSMedicCycleGlobal(this) ;
      NSPosologieBlock* pPosologyBlock = pPosoBlock ;
      if ((NULL == pPosologyBlock) && (NULL != pDialog))
      	pPosologyBlock = pDialog->_pPosoBlock ;

      if (NULL != pPosologyBlock)
      {
      	NSMedicCycleGlobal* cycle = new NSMedicCycleGlobal(pContexte, pPosologyBlock) ;
      	cycle->Load(pptIter,pptIEnd) ;
      	_Cycles.push_back(cycle) ;
      }
      else
      {
      	Avance(pptIter, pptIEnd) ;
        KAPATCol = (*pptIter)->getColonne() ;
        while ((pptIter != pptIEnd) && ( KAPATCol > col))
				{
        	Avance(pptIter, pptIEnd) ;
        	KAPATCol = (*pptIter)->getColonne() ;
        }
      }
    }

    if (pptIter != pptIEnd)
    {
      temp = (*pptIter)->getLexiqueSens() ;
      col = (*pptIter)->getColonne() ;
    }
  }
  return true;
}

bool
NSphaseMedic::LoadDate(PatPathoIter& pptIter, PatPathoIter& pptIEnd, NVLdVTemps* pDate)
{
	if (pptIter == pptIEnd)
		return false ;

	pptIter++ ;

  if (pptIter == pptIEnd)
		return false ;

	string sFormat = (*pptIter)->getLexiqueSens() ;
	string sValeur = (*pptIter)->getComplement() ;
	string sUnite  = (*pptIter)->getUnitSens() ;

	if (string("") == sValeur)
		return false ;

  if (string("") == sFormat)
		return false ;
	if (('D' != sFormat[1]) && ('T' != sFormat[1]))
		return false ;

	if (("2DA01" != sUnite) && ("2DA02" != sUnite))
		return false ;

  pDate->initFromDate(sValeur) ;

  Avance(pptIter, pptIEnd) ;
  return true ;
}

/*
** Charge la duree de la phase
*/
bool
NSphaseMedic::LoadDuree(PatPathoIter& pptIter, PatPathoIter& pptIEnd)
{
	if (pptIter == pptIEnd)
		return false ;

	pptIter++ ;

  if (pptIter == pptIEnd)
		return false ;

  sUnitDureeCycle = (*pptIter)->getUnit() ;
  iDureeCycle     = atoi(((*pptIter)->getComplement()).c_str()) ;

  Avance(pptIter, pptIEnd) ;
  return true ;
}


bool
NSphaseMedic::LoadRenouvellement(PatPathoIter& pptIter, PatPathoIter& pptIEnd)
{
	if (pptIter == pptIEnd)
		return false ;

	pptIter++ ;

  if (pptIter == pptIEnd)
		return false ;

	std::string complement = (*pptIter)->getComplement() ;
	iNbRenouvellement = atoi(complement.c_str()) ;

	Avance(pptIter, pptIEnd) ;
	return true ;
}

void
NSphaseMedic::SaveCurrentCycle()
{
	if (pDialog && pDialog->_pPosoBlock)
		_currentCycle = pDialog->_pPosoBlock->getCurrentCycle() ;

  (_Cycles[_currentCycle])->save() ;
}

/*
**  Ajoute un cycle a la phase
*/
void
NSphaseMedic::AddCycle(NSPosologieBlock* pPosoBlock)
{
	std::string CycleName2 = "" ;
	size_t val = _Cycles.size() ;
	char re = char('a' + val) ;
	CycleName2 = std::string("Cycle ") + string(1, re) ;

	// pDialog->getViewCycle()->addItem(CycleName2.c_str(), new NSMedicCycleGlobal(this)) ;

  NSPosologieBlock* pPosologyBlock = pPosoBlock ;
	if ((NULL == pPosologyBlock) && (NULL != pDialog))
		pPosologyBlock = pDialog->_pPosoBlock ;

	if (NULL == pPosologyBlock)
		return ;

	pPosologyBlock->getViewCycle()->addItem(CycleName2.c_str(), new NSMedicCycleGlobal(pContexte, pPosologyBlock)) ;
	pPosologyBlock->getViewCycle()->SetSelIndex(_Cycles.size() - 1) ;
	_currentCycle = _Cycles.size() - 1 ;
	_Monocycle = false ;
}

void
NSphaseMedic::LinkToDialog(NSPosologieBlock* pPosoBlock)
{
	NSPosologieBlock* pPosologyBlock = pPosoBlock ;
	if ((NULL == pPosologyBlock) && (NULL != pDialog))
		pPosologyBlock = pDialog->_pPosoBlock ;

	if (NULL == pPosologyBlock)
		return ;

  pPosologyBlock->getViewCycle()->Clear() ;
  pPosologyBlock->getViewCycle()->SetAssociatedData(&_Cycles, false) ;
  for (size_t i = 0 ; i < _Cycles.size() ; i++)
  {
    std::string CycleName ;
    char re = char('a' + i) ;
    CycleName = std::string("Cycle ") + string(1, re) ;
    pPosologyBlock->getViewCycle()->AddLabel((char*)CycleName.c_str()) ;
  }
  pPosologyBlock->getViewCycle()->PrintListAsNow() ;
  pPosologyBlock->getViewCycle()->SetSelIndex(_Cycles.size() - 1) ;
}

NSPatPathoArray*
NSphaseMedic::SimplifiedTreeForInterpretation()
{
  int iColonne = 0 ;
  int iColBasePhase = iColonne ;
  NSPatPathoArray* tree = new NSPatPathoArray(pContexte->getSuperviseur()) ;
  tree->ajoutePatho("KPHAT1", iColonne) ;

  // traitement des paramètres de la phase
  //
  if (false == _tStartingDate.estVide())
  {
  	tree->ajoutePatho("KOUVR1", iColonne++) ;
  	Message CodeMsg ;
		CodeMsg.SetUnit("2DA021") ;
		CodeMsg.SetComplement(_tStartingDate.donneDateHeure()) ;
		tree->ajoutePatho("£D0;19", &CodeMsg, iColonne++) ;
	}

  if (false == _tClosingDate.estVide())
  {
  	tree->ajoutePatho("KFERM1", iColonne++) ;
  	Message CodeMsg ;
		CodeMsg.SetUnit("2DA021") ;
		CodeMsg.SetComplement(_tClosingDate.donneDateHeure()) ;
		tree->ajoutePatho("£D0;19", &CodeMsg, iColonne++) ;
	}

  // pendant
  iColonne = iColBasePhase + 1 ;
  if (iDureeCycle != 0)
  {
  	tree->ajoutePatho("VDURE2", iColonne++) ;              // code pour pendant
    createNodeComplement(tree, "£N0;03", sUnitDureeCycle, iDureeCycle, iColonne) ;
	}

  iColonne = iColBasePhase + 1 ;
  if (iNbRenouvellement != 0)
  {
  	tree->ajoutePatho("VRENO1", iColonne++) ;            // code pour à renouveler
    createNodeComplement(tree, "£N0;03", "200001", iNbRenouvellement, iColonne) ;
	}
	return tree ;
}

NSPatPathoArray*
NSphaseMedic::CreateTree()
{
  NSPatPathoArray* result = SimplifiedTreeForInterpretation() ;

	if (_Cycles.empty())
  	return result ;

  std::vector<NSMedicCycleGlobal*>::iterator iter, endC ;
  endC = _Cycles.end() ;
  for (iter = _Cycles.begin() ; iter != endC ; iter++)
  {
  	NSPatPathoArray* temp = (*iter)->CreateTree() ;
    if (temp)
    {
  		result->InserePatPatho(result->end(), temp, 1) ;
      delete temp ;
    }
  }
  return result ;
}

std::string
NSphaseMedic::Decode()
{
	NSPatPathoArray* pPpt = SimplifiedTreeForInterpretation() ;
	if ((NSPatPathoArray*) NULL == pPpt)
		return std::string("") ;

	if (pPpt->empty())
	{
  	delete pPpt ;
    return std::string("") ;
	}

	std::string sLang = pContexte->getUserLanguage() ;

	NSPhrasePhase Cycle(pContexte, dcTiret, sLang) ;
	Cycle.setPPtArray(pPpt) ;
	Cycle.initialiseIterateurs() ;
	Cycle.Avance() ;
	Cycle.decode(0) ;
	Cycle.metPhrase() ;

	std::string result = Cycle.getDcodeur() ;

	delete pPpt ;

	return result ;
}

NSphaseMedicArray::NSphaseMedicArray()
                  :NSphaseMedicVector()
{
}

NSphaseMedicArray::NSphaseMedicArray(const NSphaseMedicArray& src)
                  :NSphaseMedicVector()
{
	if (false == src.empty())
		for (NSphaseMedicConstIter iter = src.begin() ; src.end() != iter ; iter++)
			push_back(new NSphaseMedic(**iter)) ;
}

NSphaseMedicArray::~NSphaseMedicArray()
{
	vider() ;
}

NSphaseMedicArray&
NSphaseMedicArray::operator=(const NSphaseMedicArray& src)
{
	if (this == &src)
		return (*this) ;

	if (false == src.empty())
		for (NSphaseMedicConstIter iter = src.begin() ; src.end() != iter ; iter++)
			push_back(new NSphaseMedic(**iter)) ;

	return (*this) ;
}

int
NSphaseMedicArray::operator==(const NSphaseMedicArray& src)
{
	if      (src.empty() && empty())
		return 1 ;
	else if (src.empty())
		return 0 ;
	else if (empty())
		return 0 ;

	NSphaseMedicConstIter srcIter ;
	NSphaseMedicIter      thisIter ;

	for (srcIter = src.begin(), thisIter = begin() ; (srcIter != src.end()) && (thisIter != end()) ; srcIter++, thisIter++)
		if (!((**srcIter) == (**thisIter)))
			return 0 ;

	if ((srcIter == src.end()) && (thisIter == end()))
		return 1 ;
	else
		return 0 ;
}

void
NSphaseMedicArray::vider()
{
	if (empty())
  	return ;

	for (NSphaseMedicIter iter = begin() ; end() != iter ; )
	{
		delete (*iter) ;
    erase(iter) ;
	}
}

