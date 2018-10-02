// -----------------------------------------------------------------------------
// nsmedicdlg.cpp
// -----------------------------------------------------------------------------
// $Revision: 1.7 $
// $Author: pameline $
// $Date: 2015/06/19 14:20:05 $
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// FLP  - novembre 2003
// -----------------------------------------------------------------------------

#include "nssavoir/nsBdmDriver.h"
#include "nsbb\nsmediccycle.h"
#include "nsbb\nsmedicdlg.h"
#include "nsbb\nsbb.h"
#include "nsbb\nsbbtran.h"
#include "nautilus\nssuper.h"
#include "dcodeur\nsphrobj.h"

long NSMedicCycleGlobal::lObjectCount = 0 ;
long NSCircadien::lObjectCount = 0 ;

// Accepted values are integer, or doubles or fractions of integers
//
bool isValidValue(string* psValue)
{
	if (!psValue)
  	return false ;

  strip(*psValue, stripBoth, ' ') ;
  strip(*psValue, stripLeft, '0') ;

	if (*psValue == "")
  	return false ;

  // Handling decimal value of the kind .5 -> 0.5
  if ((*psValue)[0] == '.')
  	*psValue = string(1, '0') + *psValue ;
	// Handling decimal value of the kind /2 -> 1/2
  else if ((*psValue)[0] == '/')
  	*psValue = string(1, '1') + *psValue ;

  size_t iLen = strlen(psValue->c_str()) ;
  if (iLen > BASE_COMPLEMENT_LEN)
  	return false ;

  bool bAlreadySeenADot   = false ;
  bool bAlreadySeenASlash = false ;
  for (size_t i = 0 ; i < iLen ; i++)
  {
  	if ((*psValue)[i] == '.')
    {
    	if (bAlreadySeenASlash || bAlreadySeenADot)
      	return false ;
      bAlreadySeenADot = true ;
    }
    else if ((*psValue)[i] == '/')
    {
    	if (bAlreadySeenASlash || bAlreadySeenADot)
      	return false ;
      bAlreadySeenASlash = true ;
    }
  	else if (!(isdigit((*psValue)[i])))
    	return false ;
  }

  return true ;
}

/*
** Fonction avalant une patpatho
*/
void Avance(PatPathoIter& pptIter,PatPathoIter& pptEnd)
{
	if (pptIter != pptEnd)
  	pptIter++ ;
}

void Recule(PatPathoIter& pptIter)
{
	pptIter-- ;
}

NSRythme::NSRythme(NSMedicCycleGlobal* parent) :  _parent(parent)
{
  _data = NULL ;
}

bool NSRythme::IsEqual(NSRythme& Rythme)
{
  if (Rythme.Type() != _type)
    return false;
  //else
	return true ;
}


/*
** Fonction servant a charger un rythme particulier
**  parent : Pointeur permettant d'acceder a la fenetre
**  cycle  : La fenetre doit'elle etre petite ou grande
**  index  : Index de l'onglet a activer
*/
void
NSRythme::load_cyle_func(bool cycle, int index)
{
	_parent->GetWindow()->_Cycle = cycle ;
	_parent->GetWindow()->updateCycle(index) ;
}

/*
** Fonction sert à charger un rythme sur la fenêtre en fonction de son type
*/
void
NSRythme::Load(bool /* test_if_simple */)
{
  DayRythme::ReinitDialog(_parent->GetWindow()) ;
  Day1Day2Day3Rythme::ReinitDialog(_parent->GetWindow()) ;
  RythmeRegulier::ReinitDialog(_parent->GetWindow()) ;
  RythmeFree::ReinitDialog(_parent->GetWindow()) ;

  switch(_type)
  {
  	case DayRythm :
      load_cyle_func(true, 2) ;
    	break ;

    case Day1Day2Day3 :
      load_cyle_func(true, 3) ;
    	break ;

    case RegularRythm :
      load_cyle_func(true, 0) ;
    	break ;

    case FreeRythm :
    	load_cyle_func(true, 1) ;
    	break ;

    case RythmSimple :
      load_cyle_func(false, 4) ;
    	break ;
  }

  if (_data)
		_data->Load() ;
}

void
NSRythme::ReinitDialog(NSPosologieBlock* win)
{
	if ((NSPosologieBlock*) NULL == win)
		return ;

  DayRythme::ReinitDialog(win) ;
  Day1Day2Day3Rythme::ReinitDialog(win) ;
  RythmeRegulier::ReinitDialog(win) ;
  RythmeFree::ReinitDialog(win) ;
  win->updateCycle(4) ;
}

NSPatPathoArray*
NSRythme::SimplifiedTreeForInterpretation()
{
	if (_data)
  	return _data->SimplifiedTreeForInterpretation() ;
	return (NSPatPathoArray*) 0 ;
}

std::string
NSRythme::Decode()
{
	NSPatPathoArray* pPpt = SimplifiedTreeForInterpretation() ;
	if (((NSPatPathoArray*) NULL == pPpt) || pPpt->empty())
		return std::string("") ;

	std::string sLang = _parent->pContexte->getUserLanguage() ;

	NSPhraseCycle Cycle(_parent->pContexte, dcTiret, sLang) ;

	Cycle.setPPtArray(new NSPatPathoArray(*pPpt)) ;
	Cycle.initialiseIterateurs() ;
	Cycle.decode(-1) ;
	Cycle.metPhrase() ;

	std::string result = Cycle.getDcodeur() ;

	return result ;
}

//
// ----------------- NSCircadien -----------------
//

NSCircadien::NSCircadien(NSMedicCycleGlobal* parent)
{
  _parent        = parent ;
  _CircadienData = (BaseCirc*) 0 ;
  _Type          = UndefinedCircadien ;

  lObjectCount++ ;
}

NSCircadien::NSCircadien(const NSCircadien& rv)
{
  _parent        = rv._parent ;
  _CircadienData = rv._CircadienData ;
  _Type          = rv._Type ;

  lObjectCount++ ;
}

NSCircadien::~NSCircadien()
{
  lObjectCount-- ;
}

NSCircadien&
NSCircadien::operator=(const NSCircadien& src)
{
  if (this == &src)
		return *this ;

  _parent        = src._parent ;
  _CircadienData = src._CircadienData ;
  _Type          = src._Type ;

  return *this ;
}

void NSCircadien::ReinitDialog(NSPosologieBlock* win)
{
	if (NULL == win)
		return ;

  CircBaseMMS::ReinitDialog(win) ;
  CircBasePeriode::ReinitDialog(win) ;
  CircBaseText::ReinitDialog(win) ;
  CircBaseFree::ReinitDialog(win) ;
  CircBaseRegular::ReinitDialog(win) ;
  CircBaseHeures::ReinitDialog(win) ;
  //win->updatePosologie(1);
}

void
NSCircadien::Load()
{
  CircBaseMMS::ReinitDialog(_parent->GetWindow());
  CircBasePeriode::ReinitDialog(_parent->GetWindow());
  CircBaseText::ReinitDialog(_parent->GetWindow());
  CircBaseFree::ReinitDialog(_parent->GetWindow());
  CircBaseRegular::ReinitDialog(_parent->GetWindow());
  CircBaseHeures::ReinitDialog(_parent->GetWindow());
  switch(_Type)
  {
    case RegularCircadien:                        // Rythme journalier regulier
      _parent->GetWindow()->updatePosologie(3);
    break;
    case FreeCircadien:                                                      // Rythme regulier libre
       _parent->GetWindow()->updatePosologie(4);
    break;
    case HourCircadien:                                                        // Rythme par heure
       _parent->GetWindow()->updatePosologie(2);
    break;
    case MMS:                                           // Matin Midi Soir
      _parent->GetWindow()->updatePosologie(1);
    break;
    case Period:                                          // Par periode
      _parent->GetWindow()->updatePosologie(0);
    break;
    case FreeTxt:
      _parent->GetWindow()->updatePosologie(5);
    break;
    case PriseUnique:
      _parent->GetWindow()->updatePosologie(6);
    break;
    default:
      _parent->GetWindow()->updatePosologie(1); // ouverture par default  sur MMS
    break;
  };
  if (_CircadienData != NULL)
		_CircadienData->Load() ;
}


template <class CastingType>
void save_func_for_circadien(BaseCirc** CircadienData,CircadienCycle& typeR ,CircadienCycle typeTo, NSMedicCycleGlobal* parent)
{
	typeR = typeTo ;
	if (NULL != CircadienData)
	{
  	CastingType* temp = dynamic_cast<CastingType *>(*CircadienData);
    if (temp != NULL)
    	temp->save() ;
    else
    {
    	delete (*CircadienData) ;
      *CircadienData = new  CastingType(parent) ;
    }
  }
  else
		*CircadienData = new  CircBaseRegular(parent) ;

	(*CircadienData)->save() ;
}

void
NSCircadien::save()
{
	int tab = _parent->GetWindow()->_tabPoso->GetSel() ;

	switch(tab)
  {
    case 3:                        // Rythme journalier regulier
   //save_func_for_circadien<CircBaseRegular>(&_CircadienData,_Type,RegularCircadien ,_parent);

      _Type = RegularCircadien ;

   		if (_CircadienData)
   		{
      	CircBaseRegular* temp = dynamic_cast<CircBaseRegular *>(_CircadienData) ;
      	if (temp)
        	temp->save() ;
      	else
        {
        	delete (_CircadienData) ;
          _CircadienData = new CircBaseRegular(_parent) ;
          _CircadienData->save() ;
        }
      }
    	else
      {
      	_CircadienData = new CircBaseRegular(_parent) ;
    	  _CircadienData->save() ;
      }

    	break ;

    case 4:                             // Rythme regulier libre
    //save_func_for_circadien<CircBaseFree>(&_CircadienData,_Type,FreeCircadien ,_parent);

			_Type = FreeCircadien ;

			if (_CircadienData)
			{
      	CircBaseFree* temp = dynamic_cast<CircBaseFree *>(_CircadienData) ;
      	if (temp)
        	temp->save() ;
      	else
        {
          delete (_CircadienData) ;
          _CircadienData = new CircBaseFree(_parent) ;
          _CircadienData->save() ;
        }
      }
      else
      {
        _CircadienData = new CircBaseFree(_parent) ;
        _CircadienData->save() ;
      }

      break;

    case 2:

      _Type = HourCircadien ;               // Rythme par heure

      if (_CircadienData)
      {
        CircBaseHeures* temp = dynamic_cast<CircBaseHeures *>(_CircadienData) ;
        if (temp)
          temp->save() ;
        else
        {
          delete _CircadienData ;
          _CircadienData = new CircBaseHeures(_parent) ;
          _CircadienData->Load() ;
          _CircadienData->save() ;
        }
      }
      else
      {
        _CircadienData = new CircBaseHeures(_parent) ;
        _CircadienData->Load() ;
        _CircadienData->save() ;
      }

      break ;

    case 1:

      // save_func_for_circadien<CircBaseMMS>(&_CircadienData,_Type,MMS ,_parent);

      _Type = MMS ;
                                                // Matin Midi Soir
      if (_CircadienData)
      {
        CircBaseMMS* temp = dynamic_cast<CircBaseMMS *>(_CircadienData) ;
        if (temp)
          temp->save() ;
        else
        {
          delete (_CircadienData) ;
          _CircadienData = new CircBaseMMS(_parent) ;
          _CircadienData->save() ;
        }
      }
      else
      {
        _CircadienData = new  CircBaseMMS(_parent) ;
        _CircadienData->save() ;
      }

      break ;

    case 0:

      //save_func_for_circadien<CircBasePeriode>(&_CircadienData,_Type,Period ,_parent);

      _Type = Period ;

      if (_CircadienData)
      {
        CircBasePeriode* temp = dynamic_cast<CircBasePeriode *>(_CircadienData) ;
        if (temp)
          temp->save() ;
        else
        {
          delete (_CircadienData) ;
          _CircadienData = new CircBasePeriode(_parent) ;
          _CircadienData->save() ;
        }
      }
      else
      {
        _CircadienData = new CircBasePeriode(_parent) ;
        _CircadienData->save() ;                           // Par periode
      }

      break ;

    case 5:

      // save_func_for_circadien<CircBaseText>(&_CircadienData,_Type,FreeTxt ,_parent);

      _Type = FreeTxt ;

      if (_CircadienData)
      {
        CircBaseText* temp = dynamic_cast<CircBaseText *>(_CircadienData) ;
        if (temp)
          temp->save() ;
        else
        {
          delete (_CircadienData) ;
          _CircadienData = new CircBaseText(_parent) ;
          _CircadienData->save() ;
        }
      }
      else
      {
        _CircadienData = new CircBaseText(_parent) ;
        _CircadienData->save() ;
      }

      break ;

    case 6:

      save_func_for_circadien<CircUnique>(&_CircadienData, _Type, PriseUnique, _parent) ;

      _Type = PriseUnique ;

      if (_CircadienData)
      {
        CircUnique* temp = dynamic_cast<CircUnique *>(_CircadienData) ;
        if (temp)
          temp->save() ;
        else
        {
          delete (_CircadienData) ;
          _CircadienData = new  CircUnique(_parent) ;
          _CircadienData->save() ;
        }
      }
      else
      {
        _CircadienData = new CircUnique(_parent) ;
        _CircadienData->save() ;
      }

    	break ;

    default:
    	break ;
  };
}

NSPatPathoArray*
NSCircadien::CreateTree()
{
	if ((BaseCirc*) NULL == _CircadienData)
		return (NSPatPathoArray*) 0 ;

  return _CircadienData->CreateTree() ;
}

bool IsAllowedForCicadien(std::string& temp)
{
  return ((temp == std::string("VNBDO"))
          || (std::string("KCYCI") == temp)
          || (std::string("KMATI") == temp)
          || (std::string("KMIDI") == temp)
          || (std::string("KSOIR") == temp)
          || (std::string("KCOUC") == temp)
          || (std::string("KREVE") == temp)
          || (std::string("KGOUT") == temp)
          || (std::string("KNOCT") == temp)
          || (std::string("£?????") == temp)
          || (std::string("KHHMM") == temp)) ;
}

CircadienCycle
NSCircadien::FindWichPanelToOpen(PatPathoIter& pptIter, PatPathoIter& pptend, int /* Co1l */)
{
  PatPathoIter iter = pptIter ;

  std::string temp = (*iter)->getLexiqueSens() ;

  // bool MMSorPeriod = false ;
  int Col = (*iter)->getColonne() ;
  int tempCol = Col ;

  if (temp == std::string("VNBDO"))
    return PriseUnique ;

  if (temp == std::string("KHHMM"))
    return HourCircadien ;

  if (temp == std::string("£?????"))
    return FreeTxt ;

  if (temp == std::string("KCYCI"))
  {
    if (iter != pptend)
    {
    	iter++ ;
      if (iter != pptend)
      {
      	temp = (*iter)->getLexiqueSens() ;
        if (temp == std::string("KRYLI"))
        	return  FreeCircadien ;
        if (temp == std::string("KRYRE"))
        	return  RegularCircadien ;
      }
    }
  }

  if ((temp == std::string("KREVE")) || (temp == std::string("KMATI")) || (temp == std::string("KMIDI"))
  ||  (temp == std::string("KGOUT")) || (temp == std::string("KSOIR")) || (temp == std::string("KCOUC"))
  ||  (temp == std::string("KNOCT")))

  while (Col == tempCol)
  {
    if (std::string("KREVE") == temp)
    	return Period ;
    if (std::string("KGOUT") == temp)
    	return Period ;
    if (std::string("KNOCT") == temp)
    	return Period ;
    Avance(iter, pptend) ;
    if (iter != pptend)
    	Avance(iter, pptend) ;
    if (iter != pptend)
    	Avance(iter, pptend) ;

    if (iter != pptend)
    {
      temp =    (*iter)->getLexiqueSens() ;
      tempCol = (*iter)->getColonne() ;
    }
    else
    	break ;

  }
  return MMS ;
}

bool NSCircadien::Load(PatPathoIter& pptIter, PatPathoIter& pptend)
{
	if (pptIter == pptend)
  	return false ;

	Avance(pptIter, pptend) ;

  std::string temp = "" ;
  int Col ;
  if (pptIter != pptend)
  {
  	temp = (*pptIter)->getLexiqueSens() ;
    Col  = (*pptIter)->getColonne() ;
  }
  if (IsAllowedForCicadien(temp) == false)
  	return false ;

  CircadienCycle panel = FindWichPanelToOpen(pptIter, pptend, Col) ;

  switch(panel)
  {
  	case Period:
    	_CircadienData = new CircBasePeriode(_parent) ;
      _Type = Period ;
      break;
    case MMS :
    	_CircadienData = new CircBaseMMS(_parent) ;
      _Type = MMS ;
      break;
    case PriseUnique :
    	_CircadienData = new CircUnique(_parent) ;
      _Type = PriseUnique ;
      break;
    case  FreeTxt:
    	_CircadienData = new CircBaseText(_parent) ;
      _Type = FreeTxt ;
      break;
    case FreeCircadien:
    	_CircadienData = new CircBaseFree(_parent) ;
      _Type = FreeCircadien ;
      break;
    case RegularCircadien :
    	_CircadienData = new CircBaseRegular(_parent) ;
      _Type = RegularCircadien ;
      break ;
    case HourCircadien:
    	_CircadienData = new CircBaseHeures(_parent) ;
      _Type = HourCircadien ;
      break ;
  }
	return _CircadienData->Load(pptIter, pptend) ;
}

/*
** Fonction servant a chercher les valeurs
*/
void FullData(std::string& temp, PatPathoIter& pptIter, PatPathoIter& pptend)
{
	Avance(pptIter, pptend) ;
  // temp =  (*pptIter)->getComplement();
  if (pptIter != pptend)
  	Avance(pptIter, pptend) ;
  if (pptIter != pptend)
  	temp = (*pptIter)->getComplement() ;
  Avance(pptIter, pptend) ;
}

bool
CircBaseMMS::Load(PatPathoIter& pptIter, PatPathoIter& pptend)
{
  int col  = (*pptIter)->getColonne() ;
  int colN = (*pptIter)->getColonne() ;

  std::string temp = "" ;
  while (colN >= col)
  {
    if (pptIter != pptend)
    {
      temp = (*pptIter)->getLexiqueSens() ;
      colN = (*pptIter)->getColonne() ;
    }
    else
    	break;
    if (temp == std::string("KMATI"))
      FullData(_matin,pptIter, pptend) ;
    if (temp == std::string("KMIDI"))
      FullData(_midi, pptIter, pptend) ;
    if (temp == std::string("KSOIR"))
      FullData(_soir, pptIter, pptend) ;
    if (temp == std::string("KCOUC"))
      FullData(_coucher,pptIter, pptend) ;
  }
  return true ;
}

bool CircBasePeriode::Load(PatPathoIter& pptIter, PatPathoIter& pptend)
{
  int col = (*pptIter)->getColonne() ;
  int colN = (*pptIter)->getColonne() ;
  std::string temp = "" ;
  while (colN >= col)
  {
    if (pptIter != pptend)
    {
      temp = (*pptIter)->getLexiqueSens() ;
      colN = (*pptIter)->getColonne() ;
    }
    else
    	break ;

    if (temp == std::string("KMATI"))
      FullData(_matin,pptIter, pptend) ;

    if (temp == std::string("KMIDI"))
      FullData(_midi, pptIter, pptend) ;

    if (temp == std::string("KSOIR"))
      FullData(_soir, pptIter, pptend) ;

    if (temp == std::string("KCOUC"))
      FullData(_coucher,pptIter, pptend) ;

     if (temp == std::string("KREVE"))
      FullData(_reveil, pptIter, pptend) ;

    if (temp == std::string("KGOUT"))
      FullData(_gouter, pptIter, pptend) ;

    if (temp == std::string("KNOCT"))
      FullData(_nuit,pptIter, pptend) ;
  }
  return true ;
}

bool IsAllowedForRythme(std::string& temp)
{
  return (( std::string("KRYRE") == temp)
      ||  ( std::string("KRYLI") == temp)
      ||  ( std::string("KJLUN") == temp)
      ||  ( std::string("KJMAR") == temp)
      ||  ( std::string("KJMER") == temp)
      ||  ( std::string("KJJEU") == temp)
      ||  ( std::string("KJVEN") == temp)
      ||  ( std::string("KJSAM") == temp)
      ||  ( std::string("KJDIM") == temp)
      ||  ( std::string("2DAT0") == temp)) ;
}

bool NSRythme::Load(PatPathoIter& pptIter, PatPathoIter& pptend)
{
	std::string temp = "" ;
  Avance(pptIter, pptend) ;
  if (pptIter != pptend)
  	temp = (*pptIter)->getLexiqueSens() ;

  if (IsAllowedForRythme(temp) == false)
  	return false ;

  if (temp == "KRYRE")
  {
    _data = new RythmeRegulier(_parent) ;
    _type = RegularRythm ;
  }

  else if (temp == "KRYLI")
  {
    _data = new RythmeFree(_parent) ;
    _type = FreeRythm ;
  }

  else if ( (temp == std::string("KJLUN")) || (std::string("KJMAR") == temp)||(std::string("KJMER") == temp)
      ||(std::string("KJJEU") == temp) || (std::string("KJVEN") == temp)
      ||(std::string("KJSAM") == temp) || (std::string("KJDIM") == temp))
  {
  	_data = new DayRythme(_parent) ;
    _type = DayRythm ;
  }

  else if (temp == "2DAT0")
  {
    _data = new Day1Day2Day3Rythme(_parent) ;
    _type = Day1Day2Day3 ;
  }

	if (NULL == _data)
		return false ;

  if (_data->Load(pptIter, pptend) == false)
  	return false ;

	return true ;
}

CircBasePeriode::CircBasePeriode(NSMedicCycleGlobal*  parent) :BaseCirc(parent)
{
	_reveil = "0" ;
  _matin  = "0" ;
  _midi   = "0" ;
  _gouter = "0" ;
  _soir   = "0" ;
  _coucher= "0" ;
  _nuit   = "0" ;
}

void func_create_patho_quant(std::string sroot, std::string quant, NSPatPathoArray* pat, int col)
{
	if (NULL == pat)
		return ;

  Message CodeMsg ;

	if (sroot != "VNBDO1")
	{
  	pat->ajoutePatho(sroot, col) ;
    pat->ajoutePatho("VNBDO1", col+1) ;
		CodeMsg.SetUnit("200001") ;
    CodeMsg.SetComplement(quant) ;
    pat->ajoutePatho("£N0;03", &CodeMsg, col+2) ;
	}
  else
  {
  	pat->ajoutePatho("VNBDO1", col) ;
		CodeMsg.SetUnit("200001") ;
    CodeMsg.SetComplement(quant) ;
    pat->ajoutePatho("£N0;03", &CodeMsg, col+3) ;
	}
}

NSPatPathoArray*
CircBasePeriode::CreateTree()
{
  NSPatPathoArray* result = new NSPatPathoArray(_parent->pContexte->getSuperviseur());
  result->ajoutePatho("KRYTH1", 0) ;

	if (isValidValue(&_reveil))
		func_create_patho_quant("KREVE1", _reveil, result, 1) ;

	if (isValidValue(&_matin))
		func_create_patho_quant("KMATI1", _matin, result, 1) ;

	if (isValidValue(&_midi))
		func_create_patho_quant("KMIDI1", _midi, result, 1) ;

	if (isValidValue(&_gouter))
		func_create_patho_quant("KGOUT1", _gouter, result, 1) ;

	if (isValidValue(&_soir))
		func_create_patho_quant("KSOIR1", _soir, result, 1) ;

	if (isValidValue(&_coucher))
		func_create_patho_quant("KCOUC1", _coucher, result, 1) ;

	if (isValidValue(&_nuit))
		func_create_patho_quant("KNOCT3", _nuit, result, 1) ;

  return result ;
}

CircBaseMMS::CircBaseMMS(NSMedicCycleGlobal*  parent) :BaseCirc(parent)
{
	_matin   = "0" ;
  _midi    = "0" ;
  _soir    = "0" ;
  _coucher = "0" ;
}

std::string CircBaseMMS::IsValid()
{
  bool result = false ;
  if ((_midi == "") || (_midi == "0"))
  	result = true ;
  if ((_matin == "") || (_matin == "0"))
  	result = true ;
  if ((_soir == "") || (_soir == "0"))
  	result = true ;
  if ((_coucher == "") || (_coucher == "0"))
  	result = true ;

	if (result == false)
  	return "Cycle circadien Matin-Midi-Soir incomplet" ;
	return "" ;
}

void CircBaseMMS::ReinitDialog(NSPosologieBlock* win)
{
	if ((NSPosologieBlock*) NULL == win)
		return ;

	win->_pPriseMatin->getEditNum()->setText("0") ;
  win->_pPriseMidi->getEditNum()->setText("0") ;
  win->_pPriseSoir->getEditNum()->setText("0") ;
  win->_pPriseCoucher->getEditNum()->setText("0") ;
}

void  CircBaseMMS::Load()
{
  NSPosologieBlock* pPosoBlock = _parent->GetWindow() ;
  if ((NSPosologieBlock*) NULL == pPosoBlock)
    return ;

  pPosoBlock->_pPriseMatin->getEditNum()->setText(_matin) ;
  pPosoBlock->_pPriseMidi->getEditNum()->setText(_midi) ;
  pPosoBlock->_pPriseSoir->getEditNum()->setText(_soir) ;
  pPosoBlock->_pPriseCoucher->getEditNum()->setText(_coucher) ;
}

void  CircBaseMMS::save()
{
  NSPosologieBlock* pPosoBlock = _parent->GetWindow() ;
  if ((NSPosologieBlock*) NULL == pPosoBlock)
    return ;

  string sTemp = string("") ;
  sTemp = pPosoBlock->_pPriseMatin->getEditNum()->getText() ;
  if (string("") != sTemp)
    _matin = sTemp ;

  sTemp = pPosoBlock->_pPriseMidi->getEditNum()->getText() ;
  if (string("") != sTemp)
    _midi = sTemp ;

  sTemp = pPosoBlock->_pPriseSoir->getEditNum()->getText() ;
  if (string("") != sTemp)
    _soir = sTemp ;

  sTemp = pPosoBlock->_pPriseCoucher->getEditNum()->getText() ;
  if (string("") != sTemp)
    _coucher = sTemp ;
}

NSPatPathoArray* CircBaseMMS::CreateTree()
{
	NSPatPathoArray* result = new NSPatPathoArray(_parent->pContexte->getSuperviseur());
  result->ajoutePatho(string("KRYTH1"), 0) ;

	if (isValidValue(&_matin))
		func_create_patho_quant(string("KMATI1"), _matin, result, 1);

	if (isValidValue(&_midi))
		func_create_patho_quant(string("KMIDI1"), _midi, result, 1) ;

	if (isValidValue(&_soir))
		func_create_patho_quant(string("KSOIR1"), _soir, result, 1) ;

	if (isValidValue(&_coucher))
		func_create_patho_quant(string("KCOUC1"), _coucher, result, 1) ;

	return result ;
}

CircUnique::CircUnique(NSMedicCycleGlobal*  parent):BaseCirc(parent)
{
  _quantity = string("0") ;
}

std::string  CircUnique::IsValid()
{
  if ((string("") == _quantity) || (string("0") == _quantity))
    return string("Cycle circadien unique incomplet") ;
  return string("") ;
}

NSPatPathoArray* CircUnique::CreateTree()
{
  NSPatPathoArray* result = new NSPatPathoArray(_parent->pContexte->getSuperviseur()) ;

  result->ajoutePatho(string("KRYTH1"), 0) ;

	Message CodeMsg ;
  result->ajoutePatho(string("VNBDO1"), 1) ;
  CodeMsg.SetUnit(string("200001")) ;
  CodeMsg.SetComplement(_quantity) ;
  result->ajoutePatho(string("£N0;03"), &CodeMsg, 2) ;

  return result ;
}

bool CircUnique::Load(PatPathoIter& pptIter, PatPathoIter& pptend)
{
  Avance(pptIter, pptend) ;
  if (pptIter != pptend)
    _quantity = (*pptIter)->getComplement() ;
  Avance(pptIter, pptend) ;
  return true ;
}

void  CircUnique::Load()
{
  NSPosologieBlock* pPosoBlock = _parent->GetWindow() ;
  if ((NSPosologieBlock*) NULL == pPosoBlock)
    return ;

  pPosoBlock->_priseUnique->getEditNum()->setText(_quantity) ;
}

void  CircUnique::save()
{
  NSPosologieBlock* pPosoBlock = _parent->GetWindow() ;
  if ((NSPosologieBlock*) NULL == pPosoBlock)
    return ;

  std::string sTemp = pPosoBlock->_priseUnique->getEditNum()->getText() ;
  if (string("") != sTemp)
    _quantity = sTemp ;
}

void CircUnique::ReinitDialog(NSPosologieBlock* pPosoBlock)
{
	if ((NSPosologieBlock*) NULL == pPosoBlock)
		return ;

	pPosoBlock->_priseUnique->getEditNum()->setText("0") ;
}

void CircBasePeriode::ReinitDialog(NSPosologieBlock* pPosoBlock)
{
	if ((NSPosologieBlock*) NULL == pPosoBlock)
		return ;

  pPosoBlock->_pPriseReveil->getEditNum()->setText("0") ;         // reveil
  pPosoBlock->_pPriseMatin2->getEditNum()->setText("0") ;         // matin
  pPosoBlock->_pPriseMidi2->getEditNum()->setText("0") ;          // midi
  pPosoBlock->_pPriseGouter->getEditNum()->setText("0") ;         // gouter
  pPosoBlock->_pPriseCoucher2->getEditNum()->setText("0") ;       // soir
  pPosoBlock->_pPriseSoir2->getEditNum()->setText("0") ;          // coucher
  pPosoBlock->_pPriseNuit->getEditNum()->setText("0") ;           // nuit
}

void CircBasePeriode::Load()
{
  NSPosologieBlock* pPosoBlock = _parent->GetWindow() ;
  if ((NSPosologieBlock*) NULL == pPosoBlock)
    return ;

  pPosoBlock->_pPriseReveil->getEditNum()->setText(_reveil) ; // reveil
  pPosoBlock->_pPriseMatin2->getEditNum()->setText(_matin) ;  // matin
  pPosoBlock->_pPriseMidi2->getEditNum()->setText(_midi) ;    // midi
  pPosoBlock->_pPriseGouter->getEditNum()->setText(_gouter) ; // gouter
  pPosoBlock->_pPriseCoucher2->getEditNum()->setText(_soir) ; // soir
  pPosoBlock->_pPriseSoir2->getEditNum()->setText(_coucher) ; // coucher
  pPosoBlock->_pPriseNuit->getEditNum()->setText(_nuit) ;
}

void CircBasePeriode::save()
{
  NSPosologieBlock* pPosoBlock = _parent->GetWindow() ;
  if ((NSPosologieBlock*) NULL == pPosoBlock)
    return ;

  std::string sTemp = string("") ;

  sTemp = pPosoBlock->_pPriseReveil->getEditNum()->getText() ; // reveil
  if (string("") != sTemp)
  	_reveil = sTemp ;

  sTemp = pPosoBlock->_pPriseMatin2->getEditNum()->getText() ;
	if (string("") != sTemp)
  	_matin = sTemp ;

  sTemp = pPosoBlock->_pPriseMidi2->getEditNum()->getText() ; // midi
  if (string("") != sTemp)
  	_midi = sTemp ;

  sTemp = pPosoBlock->_pPriseGouter->getEditNum()->getText() ; // gouter
  if (string("") != sTemp)
  	_gouter = sTemp ;

  sTemp = pPosoBlock->_pPriseCoucher2->getEditNum()->getText() ; // soir
  if (string("") != sTemp)
  	_coucher = sTemp ;

  sTemp = pPosoBlock->_pPriseSoir2->getEditNum()->getText() ;  // coucher
  if (string("") != sTemp)
  	_soir = sTemp ;

  sTemp = pPosoBlock->_pPriseNuit->getEditNum()->getText() ;
  if (string("") != sTemp)
  	_nuit = sTemp ;
}

std::string
CircBasePeriode::IsValid()
{
  bool temp = false ;
  if ((_reveil != "") || (_reveil != "0"))
    temp = true ;
  if ((_matin != "") || (_matin != "0"))
    temp = true ;
  if ((_midi != "") || (_midi != "0"))
    temp = true ;
  if ((_gouter != "") || (_gouter != "0"))
    temp = true ;
  if ((_coucher != "") || (_coucher != "0"))
    temp = true ;
  if ((_soir != "") || (_soir != "0"))
    temp = true ;
  if ((_nuit != "") || (_nuit != "0"))
    temp = true ;

  if (false == temp)
    return string("Cycle circadien non renseigné\n") ;

  return string("") ;
}

CircBaseHeures::CircBaseHeures(NSMedicCycleGlobal* parent)
               :BaseCirc(parent)
{
	_prises = new std::vector<PriseHeure*>() ;
}

std::string CircBaseHeures::IsValid()
{
  if (_prises->size() > 0)
    return string("") ;
  return string("Cycle circadien non renseigné\n") ;
}

void CircBaseHeures::ReinitDialog(NSPosologieBlock* win)
{
	if ((NSPosologieBlock*) NULL == win)
		return ;

  win->_ViewPriseHeure->SetAssociatedData(NULL, false) ;
  win->_ViewPriseHeure->Clear() ;
  win->_ViewPriseHeure->PrintListAsNow() ;
}

CircBaseHeures::~CircBaseHeures()
{
  for (size_t i = 0; i < _prises->size() ; i++)
    delete((*_prises)[i]) ;

  _prises->clear() ;
  delete(_prises) ;
}

std::string CreateLabelForHour(std::string quant, std::string heure)
{
  std::string transform = string("") ;

  for (int i = 0 ; i < 2 ; i++)
    transform += heure[i] ;
  transform += string(":") ;

	for (int i = 2 ; i < 4 ; i++)
    transform += heure[i] ;

  std::string toAffiche = quant + string(" à ") + transform ;
  return toAffiche ;
}

void
CircBaseHeures::Load()
{
  NSPosologieBlock* pPosoBlock = _parent->GetWindow() ;
  if ((NSPosologieBlock*) NULL == pPosoBlock)
    return ;

  pPosoBlock->_ViewPriseHeure->Clear() ;
  pPosoBlock->_ViewPriseHeure->SetAssociatedData(_prises, false) ;

	for (size_t i = 0 ; i < _prises->size() ; i++)
  	pPosoBlock->_ViewPriseHeure->AddLabel((char*)CreateLabelForHour((*_prises)[i]->getQuantity(), (*_prises)[i]->getHeure()).c_str() ) ;

	pPosoBlock->_ViewPriseHeure->PrintListAsNow() ;
}

void  CircBaseHeures::save()
{
}

NSPatPathoArray* CircBaseHeures::CreateTree()
{
	NSPatPathoArray* result = new NSPatPathoArray(_parent->pContexte->getSuperviseur()) ;

 	result->ajoutePatho(string("KRYTH1"), 0) ;
 	for (size_t i = 0 ; i < _prises->size() ; i++)
 	{
    NSPatPathoArray* temp = (*_prises)[i]->CreateTree(_parent->pContexte) ;
    result->InserePatPatho(result->end(), temp, 1) ;
    delete temp ;
 	}

 	return result ;
}

PriseHeure *LoadPriseHeure(PatPathoIter& pptIter, PatPathoIter& pptend)
{
  std::string heure    = string("") ;
  std::string quantity = string("") ;

  Avance(pptIter, pptend) ; // mange KHHM1
  if (pptIter !=  pptend)
  {
    std::string temp = (*pptIter)->getLexique() ;
    if (string("£H0;04") == temp)
    {
    	heure = (*pptIter)->getComplement() ;
      Avance(pptIter, pptend) ;
      if (pptIter !=  pptend)
      {
      	Avance(pptIter, pptend) ; // Mange le VBNOI
        if (pptIter != pptend)
        {
        	quantity = (*pptIter)->getComplement() ;
        	Avance(pptIter, pptend) ;
        }
      }
    }
  }

  if ((string("") != heure) && (string("") != quantity))
    return new PriseHeure(quantity, heure) ;

  return (PriseHeure*) 0 ;
}

bool CircBaseHeures::Load(PatPathoIter& pptIter, PatPathoIter& pptend )
{
	PriseHeure* temp = (PriseHeure*) 0 ;
  do
  {
    temp = LoadPriseHeure(pptIter, pptend) ;
    if (temp)
    	_prises->push_back(temp) ;
  }
  while (temp) ;

  return true ;
}

CircBaseFree::CircBaseFree(NSMedicCycleGlobal*  parent)
             :BaseCirc(parent)
{
  _quantity  = string("0") ;
  _fois      = string("0") ;
  _frequence = string("0") ;
  _codeFreq  = string("") ;
}

std::string  CircBaseFree::IsValid()
{
  if (string("") == _codeFreq)
    return string("Cycle circadien libre incomplet") ;
  if ((string("") == _fois) || (string("0") == _fois))
    return string("Cycle circadien libre incomplet") ;
  if ((string("") == _frequence) || (string("0") == _frequence))
    return string("Cycle circadien libre incomplet") ;
  if ((string("") == _quantity) || (string("0") == _quantity))
    return string("Cycle circadien libre incomplet") ;

  return string("") ;
}

void CircBaseFree::Load()
{
  NSPosologieBlock* pPosoBlock = _parent->GetWindow() ;
  if ((NSPosologieBlock*) NULL == pPosoBlock)
    return ;

	pPosoBlock->_quantFCycle->getEditNum()->setText(_quantity) ;
	pPosoBlock->_quantFreqFCycle->getEditNum()->setText(_fois) ;
	pPosoBlock->_quantFoisCycle->getEditNum()->setText(_frequence) ;
	pPosoBlock->_FCycleComboF->setCode(_codeFreq) ;
}

void CircBaseFree::save()
{
  NSPosologieBlock* pPosoBlock = _parent->GetWindow() ;
  if ((NSPosologieBlock*) NULL == pPosoBlock)
    return ;

  std::string sTemp = string("") ;

  sTemp = pPosoBlock->_quantFCycle->getEditNum()->getText() ;
  if (string("") != sTemp)
  	_frequence = sTemp ;

  sTemp = pPosoBlock->_quantFreqFCycle->getEditNum()->getText() ;
  if (string("") != sTemp)
  	_quantity = sTemp ;

  sTemp = pPosoBlock->_quantFoisCycle->getEditNum()->getText() ;
  if (string("") != sTemp)
  	_fois = sTemp ;

  sTemp = pPosoBlock->_FCycleComboF->getSelCode() ;
  if (string("") != sTemp)
  	_codeFreq = sTemp ;
}

void CircBaseFree::ReinitDialog(NSPosologieBlock* win)
{
	if ((NSPosologieBlock*) NULL == win)
		return ;

	win->_quantFCycle->getEditNum()->setText("0") ;
	win->_quantFreqFCycle->getEditNum()->setText("0") ;
	win->_quantFoisCycle->getEditNum()->setText("0") ;
	win->_FCycleComboF->setCode("") ;
}

bool isCircBaseFree(std::string& temp)
{
  return ((temp == std::string("KDURC1")) || (temp == std::string("VAINC1")) ||
          (temp == std::string("VNBDO1"))) ;
}

bool
CircBaseFree::Load(PatPathoIter& pptIter, PatPathoIter& pptend)
{
  if (((PatPathoIter) NULL == *pptIter) || (pptIter == pptend))
		return false ;

  string sSens = (*pptIter)->getLexiqueSens() ;
  if (string("KRYLI") != sSens)
  	return false ;

  Avance(pptIter, pptend) ;
  if (pptIter == pptend)
		return true ;

  return ParseTree(pptIter, pptend) ;
}

bool
CircBaseFree::ParseTree(PatPathoIter& pptIter, PatPathoIter& pptend)
{
  if (((PatPathoIter) NULL == *pptIter) || (pptIter == pptend))
		return false ;

  string sSens = (*pptIter)->getLexiqueSens() ;
  if (string("KRYLI") != sSens)
  	return false ;

  int refCol = (*pptIter)->getColonne() ;

  Avance(pptIter, pptend) ;
  if (pptIter == pptend)
		return false ;

  while ((pptIter != pptend) && ((*pptIter)->getColonne() > refCol))
  {
    sSens = (*pptIter)->getLexiqueSens() ;

  	if      (std::string("KDURC") == sSens)
    {
    	Avance(pptIter, pptend) ;
      if (pptIter != pptend)
      {
      	_codeFreq  = (*pptIter)->getUnit() ;
        _frequence = (*pptIter)->getComplement() ;
        Avance( pptIter,pptend) ;
      }
    }
    else if (std::string("VAINC") == sSens)
    {
    	Avance(pptIter,pptend);
      if (pptIter != pptend)
      {
      	_fois = (*pptIter)->getComplement() ;
      	Avance(pptIter, pptend) ;
      }
    }
    else if (std::string("VNBDO") == sSens)
    {
    	Avance(pptIter, pptend) ;
      if (pptIter != pptend)
      {
      	_quantity =  (*pptIter)->getComplement() ;
      	Avance(pptIter,pptend) ;
      }
    }
    else
      return false ;
  }

  return true ;
}

NSPatPathoArray*
CircBaseFree::CreateTree()
{
	NSPatPathoArray* result = new NSPatPathoArray(_parent->pContexte->getSuperviseur()) ;
	result->ajoutePatho(string("KRYTH1"), 0) ;
	result->ajoutePatho(string("KCYCI1"), 1) ;
	result->ajoutePatho(string("KRYLI1"), 2) ;

	Message CodeMsg ;

	if (isValidValue(&_frequence))
	{
		result->ajoutePatho(string("KDURC1"), 3) ;
		CodeMsg.SetUnit(_codeFreq) ;
		CodeMsg.SetComplement(_frequence) ;
		result->ajoutePatho(string("£N0;03"), &CodeMsg, 4) ;
	}

	if (isValidValue(&_fois))
	{
		result->ajoutePatho(string("VAINC1"), 3) ;
    CodeMsg.Reset() ;
		CodeMsg.SetUnit(string("2FOIS1")) ;
		CodeMsg.SetComplement(_fois) ;
		result->ajoutePatho(string("£N0;03"), &CodeMsg, 4) ;
	}

	if (isValidValue(&_quantity))
	{
		result->ajoutePatho(string("VNBDO1"), 3) ;
    CodeMsg.Reset() ;
		CodeMsg.SetUnit(string("200001")) ;
		CodeMsg.SetComplement(_quantity) ;
		result->ajoutePatho(string("£N0;03"), &CodeMsg, 4) ;
	}

	return result ;
}

double
CircBaseFree::getTakeCountPerDay(NSContexte *pContexte)
{
  double dDosePerTAke = StringToDouble(_quantity) ;
  if (0 == dDosePerTAke)
    return 0 ;

  double dTakeFrequency = StringToDouble(_frequence) ;
  if (0 == dTakeFrequency)
    return 0 ;

  double dTakeCount = StringToDouble(_fois) ;
  if (0 == dTakeCount)
    return 0 ;

  double dUnitCountPerDay = 0 ;

  string sSens ;
	pContexte->getDico()->donneCodeSens(&_codeFreq, &sSens) ;

  if      (string("2HEUR") == sSens)
    dUnitCountPerDay = 24 ;
  else if (string("2MINU") == sSens)
    dUnitCountPerDay = 1440 ;
  else if (string("2SEC0") == sSens)
    dUnitCountPerDay = 86400 ;

  if (0 == dUnitCountPerDay)
    return 0 ;

  return (dUnitCountPerDay / dTakeFrequency) * dDosePerTAke * dTakeCount ;
}

/**
 *  CircBaseRegular take every X duration units
 */
CircBaseRegular::CircBaseRegular(NSMedicCycleGlobal* parent)
                :BaseCirc(parent)
{
  _quantity  = string("0") ;
  _freq      = string("0") ;
  _codeFreq  = string("") ;
}

std::string  CircBaseRegular::IsValid()
{
  if ((string("") == _freq) || (string("0") == _freq))
    return string("Cycle circadien incomplet") ;
  if (string("") == _codeFreq)
    return string("Cycle circadien incomplet") ;
  if ((string("") == _quantity) || (string("0") == _quantity))
    return string("Cycle circadien incomplet") ;

  return string("") ;
}

void CircBaseRegular::Load()
{
  NSPosologieBlock* pPosoBlock = _parent->GetWindow() ;
  if ((NSPosologieBlock*) NULL == pPosoBlock)
    return ;

  if (string("") == _quantity)
    _quantity = string("0") ;
  pPosoBlock->_quantRCycle->getEditNum()->setText(_quantity) ;

  if (string("") == _freq)
    _freq = string("0") ;
  pPosoBlock->_freqRCycle->getEditNum()->setText(_freq) ;

  if (string("") == _codeFreq)
    _codeFreq = string("0") ;
  pPosoBlock->_RCycleComboF->setCode(_codeFreq) ;
}

void CircBaseRegular::save()
{
  NSPosologieBlock* pPosoBlock = _parent->GetWindow() ;
  if ((NSPosologieBlock*) NULL == pPosoBlock)
    return ;

  std::string sTemp = string("") ;

  sTemp = pPosoBlock->_quantRCycle->getEditNum()->getText() ;
  if (string("") != sTemp)
    _quantity = sTemp ;

  sTemp = pPosoBlock->_freqRCycle->getEditNum()->getText() ;
  if (string("") != sTemp)
    _freq = sTemp ;

  sTemp = pPosoBlock->_RCycleComboF->getSelCode() ;
  if (string("") != sTemp)
    _codeFreq = sTemp ;
}

bool
CircBaseRegular::Load(PatPathoIter& pptIter, PatPathoIter& pptend)
{
  if (((PatPathoIter) NULL == *pptIter) || (pptIter == pptend))
		return false ;

  string sSens = (*pptIter)->getLexiqueSens() ;
  if (string("KRYTP") != sSens)
  	return false ;

  Avance(pptIter, pptend) ;
  if (pptIter == pptend)
		return true ;

  return ParseTree(pptIter, pptend) ;
}

bool
CircBaseRegular::ParseTree(PatPathoIter& pptIter, PatPathoIter& pptend)
{
  if (((PatPathoIter) NULL == *pptIter) || (pptIter == pptend))
		return false ;

  string sSens = (*pptIter)->getLexiqueSens() ;
  if (string("KRYRE") != sSens)
  	return false ;

  int refCol = (*pptIter)->getColonne() ;

  Avance(pptIter, pptend) ;
  if (pptIter == pptend)
		return false ;

  while ((pptIter != pptend) && ((*pptIter)->getColonne() > refCol))
  {
    sSens = (*pptIter)->getLexiqueSens() ;

  	if      (std::string("KDURC") == sSens)
    {
    	Avance(pptIter, pptend) ;
      if (pptIter != pptend)
      {
      	_codeFreq =  (*pptIter)->getUnit() ;
        _freq = (*pptIter)->getComplement() ;
        Avance( pptIter,pptend) ;
      }
    }
    else if (std::string("KDURA") == sSens)
    {
    	Avance(pptIter,pptend) ;
      if (pptIter != pptend)       // always 1 time
      	Avance(pptIter,pptend) ;
    }
    else if (std::string("VNBDO") == sSens)
    {
    	Avance(pptIter, pptend) ;
      if (pptIter != pptend)
      {
      	_quantity =  (*pptIter)->getComplement() ;
      	Avance(pptIter,pptend) ;
      }
    }
    else
      return false ;
  }

  return true ;
}

double
CircBaseRegular::getTakeCountPerDay(NSContexte *pContexte)
{
  double dDosePerTAke = StringToDouble(_quantity) ;
  if (0 == dDosePerTAke)
    return 0 ;

  double dTakeFrequency = StringToDouble(_freq) ;
  if (0 == dTakeFrequency)
    return 0 ;

  double dUnitCountPerDay = 0 ;

  string sSens ;
	pContexte->getDico()->donneCodeSens(&_codeFreq, &sSens) ;

  if      (string("2HEUR") == sSens)
    dUnitCountPerDay = 24 ;
  else if (string("2MINU") == sSens)
    dUnitCountPerDay = 1440 ;
  else if (string("2SEC0") == sSens)
    dUnitCountPerDay = 86400 ;

  if (0 == dUnitCountPerDay)
    return 0 ;

  return (dUnitCountPerDay / dTakeFrequency) * dDosePerTAke ;
}

NSPatPathoArray*
CircBaseRegular::CreateTree()
{
	NSPatPathoArray* result = new NSPatPathoArray(_parent->pContexte->getSuperviseur()) ;

	result->ajoutePatho(string("KRYTH1"), 0) ;
	result->ajoutePatho(string("KCYCI1"), 1) ;
	result->ajoutePatho(string("KRYRE1"), 2) ;

	Message CodeMsg  ;

  result->ajoutePatho(string("KDURC1"), 3) ;    // duree de la cure premier edit
	CodeMsg.SetUnit(_codeFreq) ;
	CodeMsg.SetComplement(_freq) ;
	result->ajoutePatho(string("£N0;03"), &CodeMsg, 4) ;

	result->ajoutePatho(string("KDURA1"), 3) ;
  CodeMsg.Reset() ;
	CodeMsg.SetUnit(string("2FOIS1")) ;
	CodeMsg.SetComplement("1") ;
	result->ajoutePatho(string("£N0;03"), &CodeMsg, 4) ;

	result->ajoutePatho(string("VNBDO1"), 3) ;
  CodeMsg.Reset() ;
	CodeMsg.SetUnit(string("200001")) ;
	CodeMsg.SetComplement(_quantity) ;
	result->ajoutePatho(string("£N0;03"), &CodeMsg, 4) ;

  return result;
}


void CircBaseRegular::ReinitDialog(NSPosologieBlock* win)
{
	if ((NSPosologieBlock*) NULL == win)
		return ;

  win->_quantRCycle->getEditNum()->setText("0") ;
  win->_freqRCycle->getEditNum()->setText("0") ;
  win->_RCycleComboF->setCode("") ;
}

CircBaseText::CircBaseText(NSMedicCycleGlobal* parent)
             :BaseCirc(parent)
{
  _text = string("") ;
}

std::string  CircBaseText::IsValid()
{
  if (string("") == _text)
    return string("Cycle circadien : text libre imcomple\n") ;

  return string("") ;
}

void CircBaseText::Load()
{
  NSPosologieBlock* pPosoBlock = _parent->GetWindow() ;
  if ((NSPosologieBlock*) NULL == pPosoBlock)
    return ;

  pPosoBlock->_pEditTextLibre->setText(_text) ;
}

void CircBaseText::save()
{
  NSPosologieBlock* pPosoBlock = _parent->GetWindow() ;
  if ((NSPosologieBlock*) NULL == pPosoBlock)
    return ;

  _text = pPosoBlock->_pEditTextLibre->getTexte() ;
}

void CircBaseText::ReinitDialog(NSPosologieBlock* win)
{
	if ((NSPosologieBlock*) NULL == win)
		return ;

  win->_pEditTextLibre->SetText("") ;
}

NSPatPathoArray* CircBaseText::CreateTree()
{
	NSPatPathoArray* result = new NSPatPathoArray(_parent->pContexte->getSuperviseur()) ;

	result->ajoutePatho(string("KRYTH1"), 0) ;
	Message CodeMsg ;
	CodeMsg.SetTexteLibre(_text) ;
	result->ajoutePatho(string("£?????"), &CodeMsg, 1) ;

	return result ;
}

bool CircBaseText::Load(PatPathoIter& pptIter, PatPathoIter& pptend)
{
	_text = (*pptIter)->getTexteLibre() ;
  Avance(pptIter, pptend) ;
  return true ;
}

void
NSRythme::save()
{
  NSPosologieBlock* pPosoBlock = _parent->GetWindow() ;
  if ((NSPosologieBlock*) NULL == pPosoBlock)
    return ;

  if (pPosoBlock->_tabCycle->IsWindowEnabled() == false)
  {
  	_type = RythmSimple ;
    if (_data)
    {
    	RythmeSimple* temp = dynamic_cast<RythmeSimple *>(_data) ;
      if (temp)
      	temp->save() ;
      else
      {
      	delete(_data) ;
        _data = new RythmeSimple(_parent) ;
        _data->save() ;
      }
    }
    else
    {
    	_data = new RythmeSimple(_parent) ;
      _data->save() ;
    }
  }
  else
  {
    int tab = _parent->GetWindow()->_tabCycle->GetSel() ;
    switch(tab)
    {
    	case 0 :
      	_type = RegularRythm ;
        if (_data)
        {
        	RythmeRegulier* temp = dynamic_cast<RythmeRegulier*>(_data) ;
          if (temp)
          	temp->save() ;
          else
          {
          	delete(_data) ;
            _data = new RythmeRegulier(_parent) ;
            _data->save() ;
          }
        }
        else
        {
        	_data = new RythmeRegulier(_parent) ;
          _data->save() ;
        }
        break ;

      case 1 :
      	_type = FreeRythm ;
        if (_data)
        {
        	RythmeFree* temp = dynamic_cast<RythmeFree*>(_data) ;
          if (temp)
          	temp->save() ;
          else
          {
          	delete(_data) ;
            _data = new RythmeFree(_parent) ;
            _data->save() ;
          }
        }
        else
        {
        	_data = new  RythmeFree(_parent) ;
          _data->save() ;
        }
        break ;

      case 2:
      	_type = DayRythm ;
        if (_data)
        {
        	DayRythme* temp = dynamic_cast<DayRythme *>(_data) ;
          if (temp)
          	temp->save() ;
          else
          {
          	delete(_data) ;
            _data = new DayRythme(_parent) ;
            _data->save() ;
          }
        }
        else
        {
        	_data = new  DayRythme(_parent) ;
          _data->save() ;
        }
        break ;

      case 3:
      	_type = Day1Day2Day3 ;
        if (_data)
        {
        	Day1Day2Day3Rythme* temp = dynamic_cast<Day1Day2Day3Rythme *>(_data) ;
          if (temp)
          	temp->save() ;
          else
          {
          	delete(_data) ;
            _data = new Day1Day2Day3Rythme(_parent) ;
            _data->save() ;
          }
        }
        else
        {
        	_data = new Day1Day2Day3Rythme(_parent) ;
          _data->save() ;
        }
        break ;

      default:
      	break ;
    }
  }
}

//
// ---------------------- RythmeSimple ----------------------
//

void
RythmeSimple::Load()
{
  NSPosologieBlock* pPosoBlock = _parent->GetWindow() ;
  if ((NSPosologieBlock*) NULL == pPosoBlock)
    return ;

  pPosoBlock->_extendtedCycle = !_parent->GetWindow()->_extendtedCycle ;
  pPosoBlock->DrawCycleMode() ;
  pPosoBlock->_CycleSimple->SetSelIndex(_SelectedIndex) ;
}

void
RythmeSimple::save()
{
  NSPosologieBlock* pPosoBlock = _parent->GetWindow() ;
  if ((NSPosologieBlock*) NULL == pPosoBlock)
    return ;

	_SelectedIndex = pPosoBlock->_CycleSimple->GetSelIndex() ;
}

std::string
RythmeSimple::IsValid()
{
  return string("") ; // FIXME
}

bool
RythmeSimple::Equal(RythmeBase& /* temp */)
{
  return true ; // FIXME
}

NSPatPathoArray*
RythmeSimple::SimplifiedTreeForInterpretation()
{
  NSPosologieBlock* pPosoBlock = _parent->GetWindow() ;
  if ((NSPosologieBlock*) NULL == pPosoBlock)
    return (NSPatPathoArray*) 0 ;

  NSPatPathoArray* local = (*pPosoBlock->_CycleSimple)[_SelectedIndex]->getElement() ;
  if ((NSPatPathoArray*) NULL == local)
    return (NSPatPathoArray*) 0 ;

  return new NSPatPathoArray(*local) ;
}

//
// ---------------------- DayRythme ----------------------
//

DayRythme::DayRythme(NSMedicCycleGlobal* temp)
          :RythmeBase(temp)
{
	for (int i = 0 ; i < 7 ; i++)
  	_week[i] = false ;
}

NSPatPathoArray*
DayRythme::SimplifiedTreeForInterpretation()
{
  char* temp[] = {"KJLUN1", "KJMAR1", "KJMER1", "KJJEU1", "KJVEN1", "KJSAM1", "KJDIM1"} ;
  NSPatPathoArray* result = new NSPatPathoArray(_parent->pContexte->getSuperviseur()) ;
  result->ajoutePatho(string("KRYTP1"), 0) ; //Ajoue du noeud principal

  for (int i = 0; i < 7; i++)
    if (_week[i] == true)
      result->ajoutePatho(string(temp[i]), 1) ;

  return result ;
}

bool IsDayRythme(std::string &inp)
{
	std::string temp[] = { std::string("KJLUN1"),
                         std::string("KJMAR1"),
                         std::string("KJMER1"),
                         std::string("KJJEU1"),
                         std::string("KJVEN1"),
                         std::string("KJSAM1"),
                         std::string("KJDIM1") } ;
 	for (int i = 0; i < 7; i++)
  	if (inp == temp[i] )
    	return true ;

 	return false ;
}

bool DayRythme::Load(PatPathoIter& pptIter, PatPathoIter& pptend)
{
  int Col = 0 ;
  if (pptIter != pptend)
    Col = (*pptIter)->getColonne() ;
  int ColTemp = Col ;
  while ((pptIter != pptend) && (ColTemp >= Col))
  {
    std::string temp = (*pptIter)->getLexique() ;
    if (IsDayRythme(temp) == false)
      return false ;

    if (temp == std::string("KJLUN1"))
    {
       _week[0] = true ;
       Avance(pptIter, pptend) ;
    }
    if (temp == std::string("KJMAR1"))
    {
      _week[1] = true ;
      Avance(pptIter, pptend) ;
    }
    if (temp == std::string("KJMER1"))
    {
      _week[2] = true ;
      Avance(pptIter, pptend) ;
    }
    if (temp == std::string("KJJEU1"))
    {
      _week[3] = true ;
      Avance(pptIter, pptend) ;
    }
    if (temp == std::string("KJVEN1"))
    {
       _week[4] = true ;
       Avance(pptIter, pptend) ;
    }
    if (temp == std::string("KJSAM1"))
    {
      _week[5] = true ;
       Avance(pptIter, pptend) ;
    }
    if (temp == std::string("KJDIM1"))
    {
      _week[6] = true ;
      Avance(pptIter, pptend) ;
    }
    if (pptIter != pptend)
      ColTemp = (*pptIter)->getColonne() ;
  }
  return true ;
}

std::string
DayRythme::IsValid()
{
  bool result = false ;
  for (int i =0; i < 7; i++)
    if (_week[i] == true)
      result = true ;

  if (result == false)
    return string("Rythme journalier non renseigné") ;
  return string("") ;
}

bool
DayRythme::Equal(RythmeBase& /* temp */)
{
  return true ; // FIXME
}

void
DayRythme::ReinitDialog(NSPosologieBlock* win)
{
	if ((NSPosologieBlock*) NULL == win)
		return ;

  win->_pLundi->setValue(BF_UNCHECKED) ;
  win->_pMardi->setValue(BF_UNCHECKED) ;
  win->_pMercredi->setValue(BF_UNCHECKED) ;
  win->_pJeudi->setValue(BF_UNCHECKED) ;
  win->_pVendredi->setValue(BF_UNCHECKED) ;
  win->_pSamedi->setValue(BF_UNCHECKED) ;
  win->_pDimanche->setValue(BF_UNCHECKED) ;
}

void
DayRythme::save()
{
  NSPosologieBlock* pPosoBlock = _parent->GetWindow() ;
  if ((NSPosologieBlock*) NULL == pPosoBlock)
    return ;

  _week[0] = (bool) pPosoBlock->_pLundi->getValue() ;
  _week[1] = (bool) pPosoBlock->_pMardi->getValue() ;
  _week[2] = (bool) pPosoBlock->_pMercredi->getValue() ;
  _week[3] = (bool) pPosoBlock->_pJeudi->getValue() ;
  _week[4] = (bool) pPosoBlock->_pVendredi->getValue() ;
  _week[5] = (bool) pPosoBlock->_pSamedi->getValue() ;
  _week[6] = (bool) pPosoBlock->_pDimanche->getValue() ;
}

void
DayRythme::Load()
{
  NSPosologieBlock* pPosoBlock = _parent->GetWindow() ;
  if ((NSPosologieBlock*) NULL == pPosoBlock)
    return ;

  pPosoBlock->_pLundi->setValue( (_week[0] == true) ? BF_CHECKED : BF_UNCHECKED ) ;
  pPosoBlock->_pMardi->setValue( (_week[1] == true) ? BF_CHECKED : BF_UNCHECKED ) ;
  pPosoBlock->_pMercredi->setValue( (_week[2] == true) ? BF_CHECKED : BF_UNCHECKED ) ;
  pPosoBlock->_pJeudi->setValue( (_week[3] == true) ? BF_CHECKED : BF_UNCHECKED ) ;
  pPosoBlock->_pVendredi->setValue( (_week[4] == true) ? BF_CHECKED : BF_UNCHECKED ) ;
  pPosoBlock->_pSamedi->setValue( (_week[5] == true) ? BF_CHECKED : BF_UNCHECKED ) ;
  pPosoBlock->_pDimanche->setValue( (_week[6] == true) ? BF_CHECKED : BF_UNCHECKED ) ;
}

//
// ---------------------- Day1Day2Day3Rythme ----------------------
//

Day1Day2Day3Rythme::Day1Day2Day3Rythme(NSMedicCycleGlobal* temp)
                   :RythmeBase(temp)
{
	bVoid   = false ;
	iIndice = 0 ;     // 0 means not initialized
}

NSPatPathoArray*
Day1Day2Day3Rythme::SimplifiedTreeForInterpretation()
{
  NSPatPathoArray* result = new NSPatPathoArray(_parent->pContexte->getSuperviseur()) ;
  result->ajoutePatho(string("KRYTP1"), 0) ; //Ajoue du noeud principal

	result->ajoutePatho(string("2DAT01"), 1) ;
  if (iIndice > 0)
  {
		result->ajoutePatho(string("VNUMT1"), 2) ;

		Message CodeMsg  ;
		CodeMsg.SetUnit(string("200001")) ;
		CodeMsg.SetComplement(IntToString(iIndice)) ;
		result->ajoutePatho(string("£N0;03"), &CodeMsg, 3) ;
  }

  if (bVoid)
  	result->ajoutePatho(string("9VOID1"), 1) ;

  return result ;
}

bool IsDay1Day2Day3Rythme(std::string &inp)
{
	if (inp == std::string("2DAT01"))
		return true ;

 	return false ;
}

bool
Day1Day2Day3Rythme::Load(PatPathoIter& pptIter, PatPathoIter& pptend)
{
  int Col;
  if (pptIter != pptend)
    Col = (*pptIter)->getColonne();
  int ColTemp = Col;

  while ((pptIter != pptend) && (ColTemp >=  Col))
  {
    std::string temp = (*pptIter)->getLexique();
    if (IsDay1Day2Day3Rythme(temp) == false)
      return false;

    if (temp == std::string("2DAT01"))
    {
    	Avance(pptIter,pptend) ;
      if ((pptIter != pptend) && ((*pptIter)->getLexique() == std::string("VNUMT1")))
    	{
      	Avance(pptIter, pptend) ;
        if (pptIter != pptend)
        {
        	iIndice = atoi((char*) (*pptIter)->getComplement().c_str()) ;
      		Avance(pptIter, pptend) ;
        }
    	}
    }
    else if (temp == std::string("9VOID1"))
    	bVoid = true ;

    if (pptIter != pptend)
      ColTemp = (*pptIter)->getColonne();
  }
  return true ;
}

std::string
Day1Day2Day3Rythme::IsValid()
{
  return string("") ;
}

bool
Day1Day2Day3Rythme::Equal(RythmeBase& /* temp */)
{
  return true ; // FIXME
}

void
Day1Day2Day3Rythme::ReinitDialog(NSPosologieBlock* win)
{
	if ((NSPosologieBlock*) NULL == win)
		return ;

  win->_pJour1->setValue(BF_CHECKED) ;
  win->_pJour2->setValue(BF_UNCHECKED) ;
}

void
Day1Day2Day3Rythme::save()
{
  NSPosologieBlock* pPosoBlock = _parent->GetWindow() ;
  if ((NSPosologieBlock*) NULL == pPosoBlock)
    return ;

	std::string temp = "" ;
  temp = pPosoBlock->_numJour->getEditNum()->getText() ;
  if (temp != std::string(""))
  	iIndice = atoi((char*) temp.c_str()) ;

  bVoid = (bool) pPosoBlock->_pJour2->getValue() ;
}

void
Day1Day2Day3Rythme::Load()
{
  NSPosologieBlock* pPosoBlock = _parent->GetWindow() ;
  if ((NSPosologieBlock*) NULL == pPosoBlock)
    return ;

	pPosoBlock->_numJour->getEditNum()->setText(IntToString(iIndice));

  pPosoBlock->_pJour1->setValue((bVoid == false) ? BF_CHECKED : BF_UNCHECKED ) ;
  pPosoBlock->_pJour2->setValue((bVoid == true) ? BF_CHECKED : BF_UNCHECKED ) ;
}

//
// ---------------------- RythmeRegulier ----------------------
//

void
RythmeRegulier::ReinitDialog(NSPosologieBlock* win)
{
	if ((NSPosologieBlock*) NULL == win)
		return ;

  win->_pDureeCure->getEditNum()->setText(string("0")) ;
  win->_psymDureeCure->setCode(string("2DAT01")) ;
  win->_pDureeCycleR->getEditNum()->setText(string("0")) ;
  win->_psymDureeCycleR->setCode(string("2DAT01")) ;
}

std::string
RythmeRegulier::IsValid()
{
	if (string("") == _symDureeCure)
  	return string("Rythme journalier imcomplet") ;
	if (string("") == _SymDureeCycle)
  	return string("Rythme journalier imcomplet") ;
	if ((string("") == _dureeCycle) || (string("0") == _dureeCycle))
  	return string("Rythme journalier imcomplet") ;
	if ((string("") == _dureeCure) || (string("0") == _dureeCure))
  	return string("Rythme journalier imcomplet") ;

	return string("") ;
}

bool
RythmeRegulier::Equal(RythmeBase& temp2)
{
	RythmeRegulier& temp = dynamic_cast<RythmeRegulier&>(temp2) ;
	return ((_dureeCure == temp._dureeCure)
       && (_symDureeCure == temp._symDureeCure)
       && (_dureeCycle == temp._dureeCycle)
       && (_SymDureeCycle == temp._SymDureeCycle)) ;
}

bool IsAllowweForREgularRythme(std::string& temp)
{
  return ((std::string("KDURA1") == temp) || (std::string("KDURC1") == temp)) ;
}

bool
RythmeRegulier::Load(PatPathoIter& pptIter, PatPathoIter& pptend)
{
  if (pptIter == pptend)
		return false ;

  int Col = (*pptIter)->getColonne() ;

  pptIter++ ;
  if (pptIter == pptend)
		return false ;

  int tempCol = (*pptIter)->getColonne() ;

  while ((pptIter != pptend) && (Col < tempCol))
  {
  	if (pptIter == pptend)
    	return false ;

    std::string tempBoucle = (*pptIter)->getLexique() ;

    if (IsAllowweForREgularRythme(tempBoucle) == false)
    	return false ;

    if (tempBoucle == std::string("KDURA1"))
    {
    	Avance(pptIter, pptend) ;
      if (pptIter != pptend)
      {
      	_symDureeCure = (*pptIter)->getUnit() ;
      	_dureeCure = (*pptIter)->getComplement() ;
      	Avance(pptIter, pptend) ;
      }
    }
    if (tempBoucle == std::string("KDURC1"))
    {
    	Avance(pptIter, pptend) ;
      if (pptIter != pptend)
      {
      	_SymDureeCycle = (*pptIter)->getUnit() ;
      	_dureeCycle = (*pptIter)->getComplement() ;
      	Avance(pptIter, pptend) ;
      }
    }
    if (pptIter != pptend)
    	tempCol = (*pptIter)->getColonne() ;
  }
	return true ;
}

NSPatPathoArray*
CreateRegularRythme(std::string dure_cure, std::string symcure, std::string duree_cycle, std::string SymCycle, NSContexte* cont)
{
  NSPatPathoArray* result = new NSPatPathoArray(cont->getSuperviseur()) ;

  result->ajoutePatho(string("KRYTP1"), 0) ;
  result->ajoutePatho(string("KRYRE1"), 1) ;

  Message CodeMsg  ;

  result->ajoutePatho(string("KDURA1"), 2) ;    // duree de la cure premier edit
	CodeMsg.SetUnit(symcure) ;
	CodeMsg.SetComplement(dure_cure) ;
	result->ajoutePatho(string("£N0;03"), &CodeMsg, 3) ;

	result->ajoutePatho(string("KDURC1"), 2) ;     //duree du cycle
  CodeMsg.Reset() ;
	CodeMsg.SetUnit(SymCycle) ;
	CodeMsg.SetComplement(duree_cycle) ;
	result->ajoutePatho(string("£N0;03"), &CodeMsg, 3) ;

  return result ;
}

NSPatPathoArray*
RythmeRegulier::SimplifiedTreeForInterpretation()
{
  NSPatPathoArray* result = CreateRegularRythme(_dureeCure, _symDureeCure, _dureeCycle,_SymDureeCycle, _parent->pContexte);
  return result;
}

//
// ---------------------- RythmeFree ----------------------
//

void
RythmeFree::ReinitDialog(NSPosologieBlock* win)
{
	if ((NSPosologieBlock*) NULL == win)
		return ;

  win->_pDureeCureF->getEditNum()->setText("0") ;
  win->_psymDureeCureF->setCode("") ;
  win->_pDureeCureFTime->getEditNum()->setText("0") ;
  win->_pDureeCycleFreqF->getEditNum()->setText("0") ;
  win->_psymDureeCycleFreqF->setCode("") ;
}

bool IsAllowweForFreeRythme(std::string& temp)
{
  return ((std::string("KDURA1") == temp) || (std::string("KDURC1") == temp) ||
          (std::string("VAINC1") == temp)) ;
}

bool
RythmeFree::Load(PatPathoIter& pptIter, PatPathoIter& pptend)
{
	if (pptIter == pptend)
		return true ;

	int Col = (*pptIter)->getColonne() ;

  Avance(pptIter, pptend) ;

  if (pptIter == pptend)
		return true ;

  int tempCol = (*pptIter)->getColonne() ;

  std::string tempBoucle = (*pptIter)->getLexique() ;
  while ((pptIter != pptend) && (Col <= tempCol))
  {
  	if (IsAllowweForFreeRythme(tempBoucle) == false)
    	return false ;

    if (tempBoucle == std::string("KDURA1"))
    {
    	Avance(pptIter, pptend) ;
      if (pptIter != pptend)
      {
      	_psymDureeCureF = (*pptIter)->getUnit() ;
        _pDureeCureF = (*pptIter)->getComplement() ;
        Avance(pptIter, pptend) ;
      }
    }

    if (tempBoucle == std::string("KDURC1"))
    {
    	Avance(pptIter, pptend) ;
      if (pptIter != pptend)
      {
      	_psymDureeCycleFreqF = (*pptIter)->getUnit() ;
        _pDureeCycleFreqF = (*pptIter)->getComplement() ;
        Avance(pptIter, pptend) ;
      }
    }

    if (tempBoucle == std::string("VAINC1"))
    {
    	Avance(pptIter, pptend) ;
      if (pptIter != pptend)
      {
      	_pDureeCureFTime = (*pptIter)->getComplement() ;
        Avance(pptIter, pptend) ;
      }
    }

    if (pptIter != pptend)
    {
    	tempCol    = (*pptIter)->getColonne() ;
      tempBoucle = (*pptIter)->getLexique() ;
    }
  }
  return true;
}

NSPatPathoArray* RythmeFree::SimplifiedTreeForInterpretation()
{
  NSPatPathoArray* result = new NSPatPathoArray(_parent->pContexte->getSuperviseur()) ;
  result->ajoutePatho(string("KRYTP1"), 0) ;
  result->ajoutePatho(string("KRYLI1"), 1) ;

  Message CodeMsg ;

	if (isValidValue(&_pDureeCureF))
	{
		result->ajoutePatho(string("KDURA1"), 2) ; // duree de la cure premier edit
    CodeMsg.SetUnit(_psymDureeCureF) ;
    CodeMsg.SetComplement(_pDureeCureF) ;
    result->ajoutePatho(string("£N0;03"), &CodeMsg, 3) ;
	}

	if (isValidValue(&_pDureeCycleFreqF))
	{
    result->ajoutePatho(string("KDURC1"), 2) ;     //duree du cycle
    CodeMsg.Reset() ;
    CodeMsg.SetUnit(_psymDureeCycleFreqF) ;
    CodeMsg.SetComplement(_pDureeCycleFreqF) ;
    result->ajoutePatho(string("£N0;03"), &CodeMsg, 3) ;
	}

	if (isValidValue(&_pDureeCureFTime))
	{
		result->ajoutePatho(string("VAINC1"), 2) ;
    CodeMsg.Reset() ;
		CodeMsg.SetUnit(string("2FOIS1")) ;
		CodeMsg.SetComplement(_pDureeCureFTime) ;
		result->ajoutePatho(string("£N0;03"), &CodeMsg, 3) ;
	}

  return result ;
}

std::string RythmeFree::IsValid()
{
  if ((_pDureeCureF == "0") || (_pDureeCureF == ""))
   return "Rythme libre incomplet";
  if (_psymDureeCureF == "")
    return "Rythme libre incomplet";
  if ((_pDureeCureFTime == "0") || (_pDureeCureFTime == ""))
   return "Rythme libre incomplet";
  if ((_pDureeCycleFreqF == "0") || (_pDureeCycleFreqF == ""))
   return "Rythme libre incomplet";
  if (_psymDureeCycleFreqF == "")
    return "Rythme libre incomplet";

  return "" ;
}

bool
RythmeFree::Equal(RythmeBase& /* temp */)
{
  return true ; // FIXME
}

RythmeFree::RythmeFree(NSMedicCycleGlobal* temp)
           :RythmeBase(temp)
{
  _pDureeCureF         = string("0") ;
  _psymDureeCureF      = string("") ;
  _pDureeCureFTime     = string("0") ;
  _pDureeCycleFreqF    = string("0") ;
  _psymDureeCycleFreqF = string("") ;
}

void RythmeFree::Load()
{
  NSPosologieBlock* pPosoBlock = _parent->GetWindow() ;
  if ((NSPosologieBlock*) NULL == pPosoBlock)
    return ;

  pPosoBlock->_pDureeCureF->getEditNum()->setText(_pDureeCureF) ;
  pPosoBlock->_psymDureeCureF->setCode(_psymDureeCureF) ;
  pPosoBlock->_pDureeCureFTime->getEditNum()->setText(_pDureeCureFTime) ;
  pPosoBlock->_pDureeCycleFreqF->getEditNum()->setText(_pDureeCycleFreqF) ;
  pPosoBlock->_psymDureeCycleFreqF->setCode(_psymDureeCycleFreqF) ;
}

void RythmeFree::save()
{
  NSPosologieBlock* pPosoBlock = _parent->GetWindow() ;
  if ((NSPosologieBlock*) NULL == pPosoBlock)
    return ;

  std::string temp = "" ;
  temp = pPosoBlock->_pDureeCureF->getEditNum()->getText() ;
  if (temp != "" )
		_pDureeCureF = temp ;

  temp = pPosoBlock->_psymDureeCureF->getSelCode() ;
  if (temp != "" )
		_psymDureeCureF = temp ;

  temp = pPosoBlock->_pDureeCureFTime->getEditNum()->getText() ;
  if (temp != "" )
		_pDureeCureFTime = temp ;

  temp = pPosoBlock->_pDureeCycleFreqF->getEditNum()->getText() ;
  if (temp != "" )
		_pDureeCycleFreqF = temp ;

  temp = pPosoBlock->_psymDureeCycleFreqF->getSelCode() ;
  if (temp != "" )
		_psymDureeCycleFreqF = temp ;
}


RythmeRegulier::RythmeRegulier(NSMedicCycleGlobal* temp)
               :RythmeBase(temp)
{
  _dureeCure     = string("0") ;
  _symDureeCure  = string("") ; // Combo
  _dureeCycle    = string("0") ;
  _SymDureeCycle = string("") ;
}

void RythmeRegulier::Load()
{
  NSPosologieBlock* pPosoBlock = _parent->GetWindow() ;
  if ((NSPosologieBlock*) NULL == pPosoBlock)
    return ;

  pPosoBlock->_pDureeCure->getEditNum()->setText(_dureeCure) ;
  pPosoBlock->_psymDureeCure->setCode(_symDureeCure) ;
  pPosoBlock->_pDureeCycleR->getEditNum()->setText(_dureeCycle) ;
  pPosoBlock->_psymDureeCycleR->setCode(_SymDureeCycle) ;
}

void RythmeRegulier::save()
{
  NSPosologieBlock* pPosoBlock = _parent->GetWindow() ;
  if ((NSPosologieBlock*) NULL == pPosoBlock)
    return ;

  _dureeCure     = pPosoBlock->_pDureeCure->getEditNum()->getText() ;
  _symDureeCure  = pPosoBlock->_psymDureeCure->getSelCode() ;
  _dureeCycle    = pPosoBlock->_pDureeCycleR->getEditNum()->getText() ;
  _SymDureeCycle = pPosoBlock->_psymDureeCycleR->getSelCode() ;
}

//
// ------------------- NSMedicCycleGlobal -------------------
//

NSMedicCycleGlobal::NSMedicCycleGlobal(NSContexte* pCtx, NSphaseMedic* Phase)
                   :NSRoot(pCtx), _parentPhase(Phase), _cycleCircadien(this),
                                  _cycleRythme(this)
{
  _pControlsBlock = (NSPosologieBlock*) 0 ;

  lObjectCount++ ;
}

NSMedicCycleGlobal::NSMedicCycleGlobal(NSContexte* pCtx, NSPosologieBlock* pCtrlsBlock)
                   :NSRoot(pCtx), _pControlsBlock(pCtrlsBlock), _cycleCircadien(this),
                                  _cycleRythme(this)
{
  _parentPhase = (NSphaseMedic*) 0 ;

  lObjectCount++ ;
}

NSMedicCycleGlobal::NSMedicCycleGlobal(const NSMedicCycleGlobal& rv)
                   :NSRoot(rv.pContexte), _cycleCircadien(rv._cycleCircadien),
                                          _cycleRythme(rv._cycleRythme)
{
  _pControlsBlock = rv._pControlsBlock ;
  _parentPhase    = rv._parentPhase ;

  lObjectCount++ ;
}

NSMedicCycleGlobal&
NSMedicCycleGlobal::operator=(const NSMedicCycleGlobal& src)
{
  if (this == &src)
  	return *this ;

  _pControlsBlock = src._pControlsBlock ;
  _parentPhase    = src._parentPhase ;

  _cycleCircadien = src._cycleCircadien ;
  _cycleRythme    = src._cycleRythme ;

  return *this ;
}

NSPosologieBlock*
NSRythme::GetWindow()
{
  return _parent->GetWindow() ;
}

NSPosologieBlock*
NSCircadien::GetWindow()
{
  return _parent->GetWindow() ;
}

void NSMedicCycleGlobal::ReinitDialog(NSPosologieBlock* win)
{
	if ((NSPosologieBlock*) NULL == win)
		return ;

  NSRythme::ReinitDialog(win) ;
  NSCircadien::ReinitDialog(win) ;
}

NSPosologieBlock*
NSMedicCycleGlobal::GetWindow()
{
  return _pControlsBlock ;
}

void
NSMedicCycleGlobal::save()
{
  _cycleCircadien.save() ;
  _cycleRythme.save() ;
  std::string decode = _cycleRythme.Decode() ;
}

NSMedicCycleGlobal::~NSMedicCycleGlobal()
{
  lObjectCount-- ;
}

NSPatPathoArray*
NSMedicCycleGlobal::CreateTree()
{
	NSPatPathoArray* result = new NSPatPathoArray(pContexte->getSuperviseur()) ;
	result->ajoutePatho(string("KCYTR1"), 0) ;                    // code lexique d'un cycle

  NSPatPathoArray *pNewPPT = _cycleCircadien.CreateTree() ;
  if (pNewPPT)
  {
  	result->InserePatPatho(result->end(), pNewPPT, 1) ;
    delete pNewPPT ;
	}

  NSPatPathoArray *pNewPPT2 = _cycleRythme.SimplifiedTreeForInterpretation() ;
  if (pNewPPT2)
  {
  	result->InserePatPatho(result->end(), pNewPPT2, 1) ;
  	delete pNewPPT2 ;
  }

  return result ;
}


/*
** Fonction indiquant les codes fils authorisés pour  NSMedicCycleGlobal
*/
bool ISAllowedForGlovCycle(std::string temp)
{
	return ((std::string("KRYTP") == temp )
       || (std::string("KRYTH") == temp )) ;
}

bool
RecupereCycleGlob(PatPathoIter& pptIter, PatPathoIter& pptend, int Col, NSContexte* pContexte)
{
	if (pptIter == pptend)
		return false ;

  int col = (*pptIter)->getColonne() ;

  while ((pptIter != pptend) || (col <= Col))
  {
    std::string temp = (*pptIter)->getLexiqueSens() ;
    col = (*pptIter)->getColonne() ;
  	if (ISAllowedForGlovCycle(temp) == true)
    	return true ;
    Avance(pptIter, pptend) ;
  }
  return false ;
}

bool NSMedicCycleGlobal::Load(PatPathoIter& pptIter, PatPathoIter& pptend)
{
	if (pptIter == pptend)
  	return true ;

  int ColKCytr = (*pptIter)->getColonne() ;
  std::string temp2 = (*pptIter)->getLexique() ;

  Avance(pptIter, pptend) ;

  if (pptIter == pptend)
  	return true ;

  int col = (*pptIter)->getColonne() ;

  string temp ;
  while ((pptIter != pptend) && (col > ColKCytr))
  {
    temp = (*pptIter)->getLexiqueSens() ;

    if (ISAllowedForGlovCycle(temp) == false)
    	if (RecupereCycleGlob(pptIter, pptend, ColKCytr, pContexte) == false)
      	return false ;

    if (temp == "KRYTP")   // Rythme
    {
      if (_cycleRythme.Load(pptIter, pptend) == false)
      	if (RecupereCycleGlob(pptIter, pptend, ColKCytr, pContexte) == false)
        	return false ;
        //Avance(pptIter,pptend);
    }
    if (temp == "KRYTH")   // Cycle circadien
    {
      _cycleCircadien.Load(pptIter, pptend) ;
    }

    if (pptIter != pptend)
      col = (*pptIter)->getColonne() ;
  }
  return true ;
}

void NSMedicCycleGlobal::Load(bool test_if_simple)
{
  _cycleCircadien.Load() ;
  _cycleRythme.Load(test_if_simple) ;
}

std::string
NSMedicCycleGlobal::IsValid()
{
  std::string temp = std::string("") ;
  temp += _cycleCircadien.IsValid();
  temp += _cycleRythme.IsValid();

  return temp ;
}

/*
NSContexte*
NSMedicCycleGlobal::getContext()
{
	if (NULL != _parentPhase)
		return _parentPhase->GetWindow()->pContexte ;

  if (NULL != _pControlsBlock)
		return _pControlsBlock->pContexte ;

	return NULL ;
}
*/

void createNodeComplement(NSPatPathoArray *pPPT, string sCode, string sUnit, string  sVal, int iColonne)
{
	if (((NSPatPathoArray*) NULL == pPPT) || (string("") == sCode) || (string("") == sVal))
		return ;

  Message	CodeMsg ;
  CodeMsg.SetUnit(sUnit) ;
  CodeMsg.SetComplement(sVal) ;
  pPPT->ajoutePatho(sCode, &CodeMsg, iColonne++) ;
}

void createNodeComplement(NSPatPathoArray *pPPT, string sCode, string sUnit, float fVal, int iColonne)
{
	if (((NSPatPathoArray*) NULL == pPPT) || (string("") == sCode))
		return ;

  double dVal = fVal ;
  string sVal = DoubleToString(&dVal, 1, 4) ;

  Message	CodeMsg ;
  CodeMsg.SetUnit(sUnit) ;
  CodeMsg.SetComplement(sVal) ;
  pPPT->ajoutePatho(sCode, &CodeMsg, iColonne++) ;
}

void createNodeComplement(NSPatPathoArray *pPPT, string sCode, string sUnit, int iVal, int iColonne)
{
	if (((NSPatPathoArray*) NULL == pPPT) || (string("") == sCode))
		return ;

  char        pcVal[23] ;
  itoa(iVal, pcVal, 10) ;

  Message	CodeMsg ;
  CodeMsg.SetUnit(sUnit) ;
  CodeMsg.SetComplement(pcVal) ;
  pPPT->ajoutePatho(sCode, &CodeMsg, iColonne++) ;
}

std::string getValNodeComplement(NSPatPathoArray *pPPT, PatPathoIter *pptIter, int iColonneBase, string sCode, string sUnit)
{
  string sVal = string("") ;

  if (((NSPatPathoArray*) NULL == pPPT) || ((PatPathoIter*) NULL == pptIter) ||
      (string("") == sCode) || (string("") == sUnit))
		return sVal ;

  if ((pPPT->end() != *pptIter) && ((**pptIter)->getColonne() > iColonneBase))
  {
    if (((**pptIter)->getLexique() == sCode) && ((**pptIter)->getUnit() == sUnit))
    {
      sVal = (**pptIter)->getComplement() ;
      (*pptIter)++ ;
      return sVal ;
    }
  }

  return sVal ;
}

