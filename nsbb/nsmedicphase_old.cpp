// -----------------------------------------------------------------------------
// nsmedicphase.cpp
// -----------------------------------------------------------------------------
// $Revision: 1.2 $
// $Author: pameline $
// $Date: 2009/07/04 09:15:50 $
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// FLP  - novembre 2003
// -----------------------------------------------------------------------------


#include "nsbb\nsmedicphase.h"
#include "nsbb\nsmedicdlg.h"
#include "nsbb\nsbb.h"

#include "nautilus\nssuper.h"
#include "dcodeur\nsphrobj.h"


// -----------------------------------------------------------------------------
// constructeur par défaut
// -----------------------------------------------------------------------------
NSphaseMedic::NSphaseMedic(NSMedicamentDlg *pDlg, bool auto_cycle)
{
	pDialog            = pDlg ;
  sLabel             = "" ;
  iDureeCycle        = 0 ;
  sUnitDureeCycle    = "" ;
  iNbRenouvellement  = 0 ;
  _phasepreviewIndex = -1 ;

  _Cycles = new std::vector<NSMedicCycleGlobal * >() ;
  if (auto_cycle == true)
  	_Cycles->push_back(new NSMedicCycleGlobal(this)) ;       // Creation d'un cycle par default
  _currentCycle = 0 ;
  _Monocycle    = true ;
}


// -----------------------------------------------------------------------------
// constructeur copie
// -----------------------------------------------------------------------------
NSphaseMedic::NSphaseMedic(NSphaseMedic& src)
 // : NSbaseObjMedic()
{
	pDialog            = src.pDialog ;
	sLabel             = src.sLabel ;
	iDureeCycle        = src.iDureeCycle ;
	sUnitDureeCycle    = src.sUnitDureeCycle ;
	iNbRenouvellement  = src.iNbRenouvellement ;
  _phasepreviewIndex = src._phasepreviewIndex ;

  _Cycles = new std::vector<NSMedicCycleGlobal * >() ;
  if (!(src._Cycles->empty()))
  {
  	std::vector<NSMedicCycleGlobal*>::iterator iter = _Cycles->begin() ;
  	for ( ; iter != _Cycles->end(); iter++)
    	_Cycles->push_back(new NSMedicCycleGlobal(**iter)) ;
  }
}


// -----------------------------------------------------------------------------
// destructeur
// -----------------------------------------------------------------------------
NSphaseMedic::~NSphaseMedic()
{
	if (!(_Cycles->empty()))
	{
		std::vector<NSMedicCycleGlobal*>::iterator iter = _Cycles->begin() ;
  	for ( ; iter != _Cycles->end(); )
  	{
    	delete *iter ;
      _Cycles->erase(iter) ;
    }
  }
	delete _Cycles ;
}


// -----------------------------------------------------------------------------
// operateur =
// -----------------------------------------------------------------------------
NSphaseMedic&
NSphaseMedic::operator=(NSphaseMedic src)
{
	if (this == &src)
		return *this ;

	pDialog            = src.pDialog ;
	sLabel             = src.sLabel ;
	iDureeCycle        = src.iDureeCycle ;
	sUnitDureeCycle    = src.sUnitDureeCycle ;
	iNbRenouvellement  = src.iNbRenouvellement ;
  _phasepreviewIndex = src._phasepreviewIndex ;

  if (!(_Cycles->empty()))
	{
		std::vector<NSMedicCycleGlobal*>::iterator iter = _Cycles->begin() ;
  	for ( ; iter != _Cycles->end(); )
  	{
    	delete *iter ;
      _Cycles->erase(iter) ;
    }
  }

  if (!(src._Cycles->empty()))
  {
  	std::vector<NSMedicCycleGlobal*>::iterator iter = _Cycles->begin() ;
  	for ( ; iter != _Cycles->end(); iter++)
    	_Cycles->push_back(new NSMedicCycleGlobal(**iter)) ;
  }

	return (*this) ;
}


// -----------------------------------------------------------------------------
// operateur ==
// -----------------------------------------------------------------------------
int
NSphaseMedic::operator==(NSphaseMedic& src)
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
	if (   ((pDialog->pDureePhase->getValue() == 0) && (pDialog->pCBDureePhase->getSelCode() == ""))
      || ((pDialog->pDureePhase->getValue() > 0 ) && (pDialog->pCBDureePhase->getSelCode() == ""))
      || ((pDialog->pDureePhase->getValue() == 0 )&& (pDialog->pCBDureePhase->getSelCode() != "")))
		return false ;
	else
		return true ;
}

/*
** Renvoie le cycle courrant
*/
NSMedicCycleGlobal*
NSphaseMedic::operator[](int index)
{
  if ((index >= 0) && (index < (int)_Cycles->size()))
    return (*_Cycles)[index] ;
  return NULL ;
}

void
NSphaseMedic::SetCurrentCycle(int index)
{
  _currentCycle = index;

  if (_Cycles->size() > 1)
    (*_Cycles)[_currentCycle]->Load(false);
  else
    (*_Cycles)[_currentCycle]->Load(true);
   pDialog->pDimanche->setChangeResponse(new MemFunctor<NSphaseMedic>( (NSphaseMedic*)this, &NSphaseMedic::test ) );
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
NSphaseMedic::save()
{
  iDureeCycle       =  pDialog->pDureePhase->getValue() ;
  sUnitDureeCycle   =  pDialog->pCBDureePhase->getSelCode() ;
  iNbRenouvellement =  pDialog->pRenouvellement->getValue() ;
  SaveCurrentCycle() ;
  Decode() ;
}

void
NSphaseMedic::Load()
{
	pDialog->pDureePhase->initControle(iDureeCycle) ;
	pDialog->pCBDureePhase->setCode(sUnitDureeCycle) ;
  pDialog->pRenouvellement->initControle(iNbRenouvellement);
  _currentCycle = 0 ;
  LinkToDialog() ;
  bool test_if_simple ;
  if (_Cycles->size() > 1) // Il faut passer en mode etendu
  {
  	pDialog->_Cycle = true ;
    test_if_simple = true ;
	}
	else
	{
  	pDialog->_Cycle = false ;
		test_if_simple = false ;
	}
	for (int i = 0; i < (int)_Cycles->size(); i++)
		pDialog->viewCycle->ChangeLabel((*_Cycles)[i]->GetRythme()->Decode(), i) ;
	(*_Cycles)[_currentCycle]->Load(test_if_simple) ;
	pDialog->viewCycle->PrintListAsNow() ;
	pDialog->viewCycle->SetSelIndex(_currentCycle) ;
}

int
NSphaseMedic::CycleNumber()
{
	return _Cycles->size() ;
}

std::string
NSphaseMedic::CheckIntegrity()
{
  std::string result= "" ;
  for (size_t i = 0; i < _Cycles->size(); i++)
  	result += (*_Cycles)[i]->IsValid() ;

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
  	std::string temp = (*pptIter)->getLexiqueSens(pDialog->pContexte) ;
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
NSphaseMedic::Load(PatPathoIter& pptIter, PatPathoIter& pptIEnd)
{
	Avance(pptIter, pptIEnd) ;  // On avance et on mange KPAT
  if (pptIter == pptIEnd)
  	return false ;

  std::string temp = (*pptIter)->getLexiqueSens(pDialog->pContexte) ;
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
    	NSMedicCycleGlobal* cycle = new NSMedicCycleGlobal(this) ;
      cycle->Load(pptIter,pptIEnd) ;
      _Cycles->push_back(cycle) ;
    }

    if (pptIter != pptIEnd)
    {
      temp = (*pptIter)->getLexiqueSens(pDialog->pContexte) ;
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

	string sFormat = (*pptIter)->getLexiqueSens(pDialog->pContexte) ;
	string sValeur = (*pptIter)->getComplement() ;
	string sUnite  = (*pptIter)->getUnitSens(pDialog->pContexte) ;

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
  (*_Cycles)[_currentCycle]->save();
}

/*
**  Ajoute un cycle a la phase
*/
void
NSphaseMedic::AddCycle()
{
	std::string CycleName2 = "" ;
	int val = _Cycles->size() ;
	char re = char('a') + char(val) ;
	CycleName2 = std::string("Cycle ") + string(1, re) ;

	pDialog->viewCycle->addItem(CycleName2.c_str(), new NSMedicCycleGlobal(this)) ;
	pDialog->viewCycle->SetSelIndex(_Cycles->size()-1) ;
	_currentCycle = _Cycles->size()-1 ;
	_Monocycle = false ;
}

void
NSphaseMedic::LinkToDialog()
{
  pDialog->viewCycle->Clear() ;
  pDialog->viewCycle->SetAssociatedData(_Cycles) ;
  for (int i = 0; i < (int)_Cycles->size(); i++)
  {
    std::string CycleName ;
    char re = char('a') + char(i) ;
    CycleName = std::string("Cycle ") + string(1, re) ;
    pDialog->viewCycle->AddLabel((char*)CycleName.c_str()) ;
  }
  pDialog->viewCycle->PrintListAsNow() ;
  pDialog->viewCycle->SetSelIndex(_Cycles->size()-1) ;
}

NSPatPathoArray*
NSphaseMedic::SimplifiedTreeForInterpretation()
{
  int iColonne = 0 ;
  int iColBasePhase = iColonne ;
  NSPatPathoArray* tree = new NSPatPathoArray(pDialog->pContexte) ;
  tree->ajoutePatho("KPHAT1", iColonne, 0) ;
  // traitement des paramètres de la phase
  // pendant
  iColonne = iColBasePhase + 1 ;
  if (iDureeCycle != 0)
  {
  	tree->ajoutePatho("VDURE2", iColonne++, 1) ;              // code pour pendant
    createNodeComplement(tree, "£N0;03", sUnitDureeCycle, iDureeCycle, iColonne) ;
	}

  iColonne = iColBasePhase + 1 ;
  if (iNbRenouvellement != 0)
  {
  	tree->ajoutePatho("VRENO1", iColonne++, 1) ;            // code pour à renouveler
    createNodeComplement(tree, "£N0;03", "200001", iNbRenouvellement, iColonne) ;
	}
	return tree ;
}

NSPatPathoArray*
NSphaseMedic::CreateTree()
{
  NSPatPathoArray* result = SimplifiedTreeForInterpretation() ;
  std::vector<NSMedicCycleGlobal * >::iterator iter, endC ;
  endC = _Cycles->end() ;
  for (iter= _Cycles->begin(); iter != endC; iter++)
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
	if (!pPpt)
		return std::string("") ;
	if (pPpt->empty())
	{
  	delete pPpt ;
    return std::string("") ;
	}

	std::string sLang = "" ;
	if (pDialog->pContexte->getUtilisateur())
		sLang = pDialog->pContexte->getUtilisateur()->donneLang() ;

	NSPhrasePhase Cycle(pDialog->pContexte, dcTiret, sLang) ;
	Cycle.setPPtArray(pPpt) ;
	Cycle.initialiseIterateurs() ;
	Cycle.Avance() ;
	Cycle.decode(0) ;
	Cycle.metPhrase() ;
	string*	pResultat =	Cycle.sDcodeur() ;
	std::string result = *pResultat ;

	delete pPpt ;

	return result ;
}

NSphaseMedicArray::NSphaseMedicArray()
  : NSphaseMedicVector()
{
}

NSphaseMedicArray::NSphaseMedicArray(NSphaseMedicArray& src)
                  :NSphaseMedicVector()
{
	if (!(src.empty()))
		for (NSphaseMedicIter iter = src.begin() ; iter != src.end() ; iter++)
			push_back(new NSphaseMedic(**iter)) ;
}


NSphaseMedicArray::~NSphaseMedicArray()
{
	vider() ;
}


NSphaseMedicArray&
NSphaseMedicArray::operator=(NSphaseMedicArray src)
{
	if (this == &src)
		return (*this) ;

	if (!(src.empty()))
		for (NSphaseMedicIter iter = src.begin() ; iter != src.end() ; iter++)
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

	for (NSphaseMedicIter iter = begin() ; iter != end() ; )
	{
		delete (*iter) ;
    erase(iter) ;
	}
}

