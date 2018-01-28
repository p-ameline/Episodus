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

  if (_data != NULL)
		_data->Load() ;
}

void
NSRythme::ReinitDialog(NSPosologieBlock* win)
{
	if (NULL == win)
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
	if (_data != NULL)
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
	int tab = _parent->GetWindow()->tabPoso->GetSel();
	switch(tab)
  {
    case 3:                        // Rythme journalier regulier
   //save_func_for_circadien<CircBaseRegular>(&_CircadienData,_Type,RegularCircadien ,_parent);
      _Type  =  RegularCircadien ;
   		if (NULL != _CircadienData)
   		{
      	CircBaseRegular* temp = dynamic_cast<CircBaseRegular *>(_CircadienData) ;
      	if (temp != NULL)
        	temp->save() ;
      	else
        {
        	delete (_CircadienData) ;
          _CircadienData = new CircBaseRegular(_parent) ;
        }
      }
    	else
      	_CircadienData = new CircBaseRegular(_parent) ;
    	_CircadienData->save() ;
    	break ;

    case 4:                             // Rythme regulier libre
    //save_func_for_circadien<CircBaseFree>(&_CircadienData,_Type,FreeCircadien ,_parent);
			_Type = FreeCircadien ;
			if (NULL != _CircadienData)
			{
      	CircBaseFree* temp = dynamic_cast<CircBaseFree *>(_CircadienData) ;
      	if (temp != NULL)
        	temp->save() ;
      	else
        {
          delete (_CircadienData) ;
          _CircadienData = new CircBaseFree(_parent) ;
        }
      }
    else
      _CircadienData = new  CircBaseFree(_parent);
    _CircadienData->save();
    break;

    case 2:
    _Type  =  HourCircadien;                                // Rythme par heure
     if (NULL != _CircadienData)
    {
      CircBaseHeures* temp = dynamic_cast<CircBaseHeures *>(_CircadienData);
      if (temp != NULL)
        temp->save();
        else
        {
          delete (_CircadienData);
          _CircadienData = new  CircBaseHeures(_parent);
        }
      }
    else
      _CircadienData = new  CircBaseHeures(_parent);
    _CircadienData->Load();
    _CircadienData->save();
    break;

    case 1:
    // save_func_for_circadien<CircBaseMMS>(&_CircadienData,_Type,MMS ,_parent);
    _Type  =  MMS;                                          // Matin Midi Soir
    if (NULL != _CircadienData)
    {
      CircBaseMMS* temp = dynamic_cast<CircBaseMMS *>(_CircadienData);
      if (temp != NULL)
        temp->save();
      else
        {
          delete (_CircadienData);
          _CircadienData = new  CircBaseMMS(_parent);
        }
      }
    else
      _CircadienData = new  CircBaseMMS(_parent);
    _CircadienData->save();
    break;

    case 0:
     //save_func_for_circadien<CircBasePeriode>(&_CircadienData,_Type,Period ,_parent);
     _Type  =  Period;
    if (NULL != _CircadienData)
    {
      CircBasePeriode* temp = dynamic_cast<CircBasePeriode *>(_CircadienData);
      if (temp != NULL)
        temp->save();
      else
        {
          delete (_CircadienData);
          _CircadienData = new  CircBasePeriode(_parent);
        }
      }
    else
      _CircadienData = new  CircBasePeriode(_parent);
    _CircadienData->save();                                        // Par periode
    break;

    case 5:
   // save_func_for_circadien<CircBaseText>(&_CircadienData,_Type,FreeTxt ,_parent);
    _Type  =  FreeTxt;
    if (NULL != _CircadienData)
    {
      CircBaseText* temp = dynamic_cast<CircBaseText *>(_CircadienData);
      if (temp != NULL)
        temp->save();
      else
        {
          delete (_CircadienData);
          _CircadienData = new  CircBaseText(_parent);
        }
      }
    else
      _CircadienData = new  CircBaseText(_parent);
    _CircadienData->save();
    break;
    case 6:
     save_func_for_circadien<CircUnique>(&_CircadienData,_Type,PriseUnique ,_parent);
    _Type  =  PriseUnique;
    if (NULL != _CircadienData)
    {
      CircUnique* temp = dynamic_cast<CircUnique *>(_CircadienData);
      if (temp != NULL)
        temp->save();
      else
        {
          delete (_CircadienData);
          _CircadienData = new  CircUnique(_parent);
        }
      }
    else
      _CircadienData = new  CircUnique(_parent);
    _CircadienData->save();
    	break;
    default:
    	break;
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
	if (NULL == win)
		return ;

	win->pPriseMatin->getEditNum()->setText("0") ;
  win->pPriseMidi->getEditNum()->setText("0") ;
  win->pPriseSoir->getEditNum()->setText("0") ;
  win->pPriseCoucher->getEditNum()->setText("0") ;
}

void  CircBaseMMS::Load()
{
  _parent->GetWindow()->pPriseMatin->getEditNum()->setText(_matin);
  _parent->GetWindow()->pPriseMidi->getEditNum()->setText(_midi);
  _parent->GetWindow()->pPriseSoir->getEditNum()->setText(_soir);
  _parent->GetWindow()->pPriseCoucher->getEditNum()->setText(_coucher);
}

void  CircBaseMMS::save()
{
  std::string temp = "";
  _matin = _parent->GetWindow()->pPriseMatin->getEditNum()->getText();
  if (temp != "" ) _matin = temp;
  temp =_parent->GetWindow()->pPriseMidi->getEditNum()->getText();
  if (temp != "" ) _midi = temp;
  temp = _parent->GetWindow()->pPriseSoir->getEditNum()->getText();
  if (temp != "" ) _soir = temp;
  temp = _parent->GetWindow()->pPriseCoucher->getEditNum()->getText();
  if (temp != "" ) _coucher = temp;
}

NSPatPathoArray* CircBaseMMS::CreateTree()
{
	NSPatPathoArray* result = new NSPatPathoArray(_parent->pContexte->getSuperviseur());
  result->ajoutePatho("KRYTH1", 0) ;

	if (isValidValue(&_matin))
		func_create_patho_quant("KMATI1", _matin, result, 1);

	if (isValidValue(&_midi))
		func_create_patho_quant("KMIDI1", _midi, result, 1) ;

	if (isValidValue(&_soir))
		func_create_patho_quant("KSOIR1", _soir, result, 1) ;

	if (isValidValue(&_coucher))
		func_create_patho_quant("KCOUC1", _coucher, result, 1) ;

	return result ;
}

CircUnique::CircUnique(NSMedicCycleGlobal*  parent):BaseCirc(parent)
{
  _quantity = "0" ;
}

std::string  CircUnique::IsValid()
{
  if ((_quantity == "") ||  (_quantity == "0"))
    return "Cycle circadien unique incomplet" ;
  return "" ;
}

NSPatPathoArray* CircUnique::CreateTree()
{
  NSPatPathoArray* result = new NSPatPathoArray(_parent->pContexte->getSuperviseur()) ;

  result->ajoutePatho("KRYTH1", 0) ;

	Message CodeMsg ;
  result->ajoutePatho("VNBDO1", 1) ;
  CodeMsg.SetUnit("200001") ;
  CodeMsg.SetComplement(_quantity) ;
  result->ajoutePatho("£N0;03", &CodeMsg, 2) ;

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
  _parent->GetWindow()->priseUnique->getEditNum()->setText(_quantity) ;
}

void  CircUnique::save()
{
  std::string temp = _parent->GetWindow()->priseUnique->getEditNum()->getText() ;
  if (temp != "")
    _quantity = temp ;
}

void CircUnique::ReinitDialog(NSPosologieBlock* win)
{
	if (NULL == win)
		return ;

	win->priseUnique->getEditNum()->setText("0") ;
}

void CircBasePeriode::ReinitDialog(NSPosologieBlock* win)
{
	if (NULL == win)
		return ;

  win->pPriseReveil->getEditNum()->setText("0") ;         // reveil
  win->pPriseMatin2->getEditNum()->setText("0") ;         // matin
  win->pPriseMidi2->getEditNum()->setText("0") ;          // midi
  win->pPriseGouter->getEditNum()->setText("0") ;         // gouter
  win->pPriseCoucher2->getEditNum()->setText("0") ;       // soir
  win->pPriseSoir2->getEditNum()->setText("0") ;          // coucher
  win->pPriseNuit->getEditNum()->setText("0") ;           // nuit
}

void CircBasePeriode::Load()
{
  _parent->GetWindow()->pPriseReveil->getEditNum()->setText(_reveil) ; // reveil
  _parent->GetWindow()->pPriseMatin2->getEditNum()->setText(_matin) ;  // matin
  _parent->GetWindow()->pPriseMidi2->getEditNum()->setText(_midi) ;    // midi
  _parent->GetWindow()->pPriseGouter->getEditNum()->setText(_gouter) ; // gouter
  _parent->GetWindow()->pPriseCoucher2->getEditNum()->setText(_soir) ; // soir
  _parent->GetWindow()->pPriseSoir2->getEditNum()->setText(_coucher) ; // coucher
  _parent->GetWindow()->pPriseNuit->getEditNum()->setText(_nuit) ;
}

void CircBasePeriode::save()
{
  std::string temp = "" ;

  temp = _parent->GetWindow()->pPriseReveil->getEditNum()->getText() ; // reveil
  if (temp != "")
  	_reveil = temp ;

  temp = _parent->GetWindow()->pPriseMatin2->getEditNum()->getText() ;
	if (temp != "")
  	_matin = temp ;

  temp = _parent->GetWindow()->pPriseMidi2->getEditNum()->getText() ; // midi
  if (temp != "")
  	_midi = temp ;

  temp = _parent->GetWindow()->pPriseGouter->getEditNum()->getText() ; // gouter
  if (temp != "")
  	_gouter = temp ;

  temp =   _parent->GetWindow()->pPriseCoucher2->getEditNum()->getText() ; // soir
  if (temp != "")
  	_coucher = temp ;

  temp= _parent->GetWindow()->pPriseSoir2->getEditNum()->getText() ;  // coucher
  if (temp != "")
  	_soir = temp ;

  temp = _parent->GetWindow()->pPriseNuit->getEditNum()->getText() ;
  if (temp != "")
  	_nuit = temp ;
}

std::string CircBasePeriode::IsValid()
{
  bool temp = false;
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
  if (temp == false)
    return "Cycle circadien non remplie\n" ;
  return "" ;
}

CircBaseHeures::CircBaseHeures(NSMedicCycleGlobal*  parent):BaseCirc(parent)
{
	_prises = new std::vector<PriseHeure*>() ;
}

std::string CircBaseHeures::IsValid()
{
  if (_prises->size() > 0)
    return "" ;
  return "Cycle circadien non rempli\n" ;
}

void CircBaseHeures::ReinitDialog(NSPosologieBlock* win)
{
	if (NULL == win)
		return ;

  win->ViewPriseHeure->SetAssociatedData(NULL, false) ;
  win->ViewPriseHeure->Clear() ;
  win->ViewPriseHeure->PrintListAsNow() ;
}

CircBaseHeures::~CircBaseHeures()
{
  for (size_t i = 0; i < _prises->size() ; i++)
    delete( (*_prises)[i]) ;
  _prises->clear() ;
  delete(_prises) ;
}

std::string CreateLabelForHour(std::string quant, std::string heure)
{
  std::string transform = "" ;
  for (int i = 0;i <2; i++)
    transform += heure[i] ;
  transform +=':' ;
	for (int i = 2;i <4; i++)
    transform +=heure[i] ;
  std::string toAffiche = quant + " à " + transform ;
  return  toAffiche ;
}

void  CircBaseHeures::Load()
{
  _parent->GetWindow()->ViewPriseHeure->Clear() ;
  _parent->GetWindow()->ViewPriseHeure->SetAssociatedData(_prises, false) ;
	for (size_t i = 0; i < _prises->size(); i++ )
  	_parent->GetWindow()->ViewPriseHeure->AddLabel ((char*)CreateLabelForHour((*_prises)[i]->getQuantity(), (*_prises)[i]->getHeure()).c_str() ) ;
	_parent->GetWindow()->ViewPriseHeure->PrintListAsNow() ;
}

void  CircBaseHeures::save()
{
}

NSPatPathoArray* CircBaseHeures::CreateTree()
{
	NSPatPathoArray* result = new NSPatPathoArray(_parent->pContexte->getSuperviseur()) ;

 	result->ajoutePatho("KRYTH1", 0) ;
 	for (size_t i = 0; i < _prises->size(); i++)
 	{
    NSPatPathoArray* temp = (*_prises)[i]->CreateTree(_parent->pContexte) ;
    result->InserePatPatho(result->end(), temp, 1) ;
   // delete (temp);
 	}

 	return result ;
}

PriseHeure *LoadPriseHeure(PatPathoIter& pptIter, PatPathoIter& pptend)
{
  std::string heure = "";
  std::string quantity = "";
  Avance(pptIter,pptend); // mange KHHM1
  if (pptIter !=  pptend)
  {
    std::string temp = (*pptIter)->getLexique();
    if  (temp == "£H0;04")
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

  if ((heure != "") && (quantity != ""))
    return new PriseHeure(quantity, heure) ;
  return NULL ;
}

bool CircBaseHeures::Load(PatPathoIter& pptIter, PatPathoIter& pptend )
{
	PriseHeure* temp = NULL ;
  do
  {
    temp = LoadPriseHeure(pptIter, pptend) ;
    if (temp != NULL)
    	_prises->push_back(temp) ;
  }
  while (temp != NULL) ;

  return true ;
}

CircBaseFree::CircBaseFree(NSMedicCycleGlobal*  parent)  : BaseCirc(parent)
{
  _quantity  = "0" ;
  _fois      = "0" ;
  _frequence = "0" ;
  _codeFreq  = "" ;
}

std::string  CircBaseFree::IsValid()
{
  if (_codeFreq =="")
    return "Cycle circadien libre incomplet" ;
  if ((_fois == "")&& (_fois == "0"))
    return "Cycle circadien libre incomplet" ;
  if ((_frequence == "")&& (_frequence == "0"))
    return "Cycle circadien libre incomplet" ;
  if ((_quantity == "")&& (_quantity == "0"))
    return "Cycle circadien libre incomplet" ;
  return "" ;
}

void CircBaseFree::Load()
{
	_parent->GetWindow()->_quantFCycle->getEditNum()->setText(_quantity) ;
	_parent->GetWindow()->_quantFreqFCycle->getEditNum()->setText(_fois) ;
	_parent->GetWindow()->_quantFoisCycle->getEditNum()->setText(_frequence) ;
	_parent->GetWindow()->_FCycleComboF->setCode(_codeFreq) ;
}

void CircBaseFree::save()
{
  std::string temp = "" ;

  temp = _parent->GetWindow()->_quantFCycle->getEditNum()->getText() ;
  if (temp != "")
  	_frequence = temp ;

  temp = _parent->GetWindow()->_quantFreqFCycle->getEditNum()->getText() ;
  if (temp != "")
  	_quantity = temp ;

  temp = _parent->GetWindow()->_quantFoisCycle->getEditNum()->getText() ;
  if (temp != "")
  	_fois = temp ;

  temp = _parent->GetWindow()->_FCycleComboF->getSelCode() ;
  if (temp != "")
  	_codeFreq = temp ;
}

void CircBaseFree::ReinitDialog(NSPosologieBlock* win)
{
	if (NULL == win)
		return ;

	win->_quantFCycle->getEditNum()->setText("0") ;
	win->_quantFreqFCycle->getEditNum()->setText("0") ;
	win->_quantFoisCycle->getEditNum()->setText("0") ;
	win->_FCycleComboF->setCode("") ;
}

bool isCircBaseFree(std::string& temp)
{
  return ( (temp == std::string("KDURC1")) || (temp == std::string("VAINC1")) || (temp == std::string("VNBDO1")));
}

bool
CircBaseFree::Load(PatPathoIter& pptIter, PatPathoIter& pptend)
{
  int col ;
  std::string temp = "" ;
  if (pptIter != pptend)
    temp = (*pptIter)->getLexique() ;
  if (pptIter != pptend)
  	Avance(pptIter, pptend) ;
  if (pptIter != pptend)
  	Avance(pptIter, pptend) ;

  if (pptIter != pptend)
  {
   col  = (*pptIter)->getColonne() ;
   temp = (*pptIter)->getLexique() ;
  }
  else
  	return true ;

  int tempCol = col ;
  while (col == tempCol)
  {
  	if (isCircBaseFree(temp) == false)
    	return false;
    if (std::string("KDURC1") == temp)
    {
    	Avance(pptIter, pptend) ;
      if (pptIter != pptend)
      {
      	_codeFreq =  (*pptIter)->getUnit();
        _frequence = (*pptIter)->getComplement();
        Avance(pptIter, pptend) ;
      }
    }
    if (std::string("VAINC1") == temp)
    {
    	Avance(pptIter,pptend);
      if (pptIter != pptend)
      {
      	_fois = (*pptIter)->getComplement() ;
      	Avance(pptIter, pptend) ;
      }
    }
    if (std::string("VNBDO1") == temp)
    {
    	Avance(pptIter, pptend) ;
      if (pptIter != pptend)
      {
      	_quantity = (*pptIter)->getComplement() ;
        Avance(pptIter, pptend) ;
      }
    }

    if (pptIter !=  pptend)
    	temp = (*pptIter)->getLexique() ;
  }
  return true ;
}

NSPatPathoArray* CircBaseFree::CreateTree()
{
	NSPatPathoArray* result = new NSPatPathoArray(_parent->pContexte->getSuperviseur()) ;
	result->ajoutePatho("KRYTH1", 0) ;
	result->ajoutePatho("KCYCI1", 1) ;
	result->ajoutePatho("KRYLI1", 2) ;

	Message CodeMsg ;

	if (isValidValue(&_frequence))
	{
		result->ajoutePatho("KDURC1", 3) ;
		CodeMsg.SetUnit(_codeFreq) ;
		CodeMsg.SetComplement(_frequence) ;
		result->ajoutePatho("£N0;03", &CodeMsg, 4) ;
	}

	if (isValidValue(&_fois))
	{
		result->ajoutePatho("VAINC1", 3) ;
    CodeMsg.Reset() ;
		CodeMsg.SetUnit("2FOIS1") ;
		CodeMsg.SetComplement(_fois) ;
		result->ajoutePatho("£N0;03", &CodeMsg, 4) ;
	}

	if (isValidValue(&_quantity))
	{
		result->ajoutePatho("VNBDO1", 2) ;
    CodeMsg.Reset() ;
		CodeMsg.SetUnit("200001") ;
		CodeMsg.SetComplement(_quantity) ;
		result->ajoutePatho("£N0;03", &CodeMsg, 3) ;
	}

	return result ;
}

CircBaseRegular::CircBaseRegular(NSMedicCycleGlobal* parent) : BaseCirc(parent)
{
  _quantity  = "0";
  _freq      = "0";
  _codeFreq  = "";
}

std::string  CircBaseRegular::IsValid()
{
  if ((_freq =="") || (_freq =="0"))
    return "Cycle circadien incomplet";
  if (_codeFreq == "")
    return "Cycle circadien incomplet";
  if ((_quantity =="") || (_quantity =="0"))
    return "Cycle circadien incomplet";
  return "";
}

void CircBaseRegular::Load()
{
  if (_quantity == "")  _quantity = "0";
  _parent->GetWindow()->_quantRCycle->getEditNum()->setText(_quantity);
  if (_freq == "")  _freq = "0";
  _parent->GetWindow()->_freqRCycle->getEditNum()->setText(_freq);
  if (_codeFreq == "")  _codeFreq = "0";
  _parent->GetWindow()->_RCycleComboF->setCode(_codeFreq);
}

void CircBaseRegular::save()
{
  std::string temp = "";
  temp  = _parent->GetWindow()->_quantRCycle->getEditNum()->getText() ;
  if (temp != "") _quantity = temp;
  temp      = _parent->GetWindow()->_freqRCycle->getEditNum()->getText();
  if (temp != "") _freq = temp;
  temp  = _parent->GetWindow()->_RCycleComboF->getSelCode();
  if (temp != "") _codeFreq = temp;
}

bool
CircBaseRegular::Load(PatPathoIter& pptIter, PatPathoIter& pptend)
{
  std::string temp = (*pptIter)->getLexique() ;
  Avance(pptIter, pptend) ;
  if (pptIter == pptend)
		return true ;

  int col = (*pptIter)->getColonne() ;
  Avance(pptIter, pptend) ;
  if (pptIter == pptend)
		return true ;

  int tempCol = col ;
  temp = (*pptIter)->getLexique() ;
  while (col <= tempCol)
  {
  	if (std::string("KDURC1") == temp)
    {
    	Avance(pptIter, pptend) ;
      if (pptIter != pptend)
      {
      	_codeFreq =  (*pptIter)->getUnit() ;
        _freq = (*pptIter)->getComplement() ;
        Avance( pptIter,pptend) ;
      }
    }
    if (std::string("KDURA1") == temp)
    {
    	Avance(pptIter,pptend) ;
      if (pptIter != pptend)
      	Avance(pptIter,pptend) ;
    }
    if (std::string("VNBDO1") == temp)
    {
    	Avance(pptIter, pptend) ;
      if (pptIter != pptend)
      {
      	_quantity =  (*pptIter)->getComplement() ;
      	Avance(pptIter,pptend) ;
      }
    }

    if (pptIter != pptend)
    {
    	temp    = (*pptIter)->getLexique() ;
    	tempCol = (*pptIter)->getColonne() ;
    }
    else
    	break ;
  }
  return true ;
}

NSPatPathoArray* CircBaseRegular::CreateTree()
{
	NSPatPathoArray* result = new NSPatPathoArray(_parent->pContexte->getSuperviseur()) ;

	result->ajoutePatho("KRYTH1", 0) ;
	result->ajoutePatho("KCYCI1", 1) ;
	result->ajoutePatho("KRYRE1", 2) ;

	Message CodeMsg  ;

  result->ajoutePatho("KDURC1", 3) ;    // duree de la cure premier edit
	CodeMsg.SetUnit(_codeFreq) ;
	CodeMsg.SetComplement(_freq) ;
	result->ajoutePatho("£N0;03", &CodeMsg, 4) ;

	result->ajoutePatho("KDURA1", 3) ;
  CodeMsg.Reset() ;
	CodeMsg.SetUnit("2FOIS1") ;
	CodeMsg.SetComplement("1") ;
	result->ajoutePatho("£N0;03", &CodeMsg, 4) ;

	result->ajoutePatho("VNBDO1", 2) ;
  CodeMsg.Reset() ;
	CodeMsg.SetUnit("200001") ;
	CodeMsg.SetComplement(_quantity) ;
	result->ajoutePatho("£N0;03", &CodeMsg, 3) ;

  return result;
}


void CircBaseRegular::ReinitDialog(NSPosologieBlock* win)
{
	if (NULL == win)
		return ;

  win->_quantRCycle->getEditNum()->setText("0") ;
  win->_freqRCycle->getEditNum()->setText("0") ;
  win->_RCycleComboF->setCode("") ;
}

CircBaseText::CircBaseText(NSMedicCycleGlobal* parent) :BaseCirc(parent)
{
  _text = "" ;
}

std::string  CircBaseText::IsValid()
{
  if (_text =="")
    return "Cycle circadien : text libre imcomple\n";
  return "";
}

void CircBaseText::Load()
{
  _parent->GetWindow()->pEditTextLibre->setText(_text);
}

void CircBaseText::save()
{
  _text = _parent->GetWindow()->pEditTextLibre->getTexte();
}

void CircBaseText::ReinitDialog(NSPosologieBlock* win)
{
	if (NULL == win)
		return ;

  win->pEditTextLibre->SetText("") ;
}

NSPatPathoArray* CircBaseText::CreateTree()
{
	NSPatPathoArray* result = new NSPatPathoArray(_parent->pContexte->getSuperviseur()) ;

	result->ajoutePatho("KRYTH1", 0) ;
	Message CodeMsg ;
	CodeMsg.SetTexteLibre(_text) ;
	result->ajoutePatho("£?????", &CodeMsg, 1) ;

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
  if (_parent->GetWindow()->tabCycle->IsWindowEnabled() == false)
  {
  	_type = RythmSimple ;
    if (NULL != _data)
    {
    	RythmeSimple* temp =  dynamic_cast<RythmeSimple *>(_data) ;
      if (temp != NULL)
      	temp->save() ;
      else
      {
      	delete(_data) ;
        _data = new RythmeSimple(_parent) ;
      }
    }
    else
    	_data = new   RythmeSimple(_parent) ;
    _data->save() ;
  }
  else
  {
    int tab = _parent->GetWindow()->tabCycle->GetSel() ;
    switch(tab)
    {
    	case 0 :
      	_type = RegularRythm ;
        if (NULL != _data)
        {
        	RythmeRegulier* temp = dynamic_cast<RythmeRegulier*>(_data) ;
          if (temp != NULL)
          	temp->save() ;
          else
          {
          	delete(_data) ;
            _data = new RythmeRegulier(_parent) ;
          }
        }
        else
        	_data = new  RythmeRegulier(_parent) ;
        _data->save() ;
        break ;

      case 1 :
      	_type = FreeRythm ;
        if (NULL != _data)
        {
        	RythmeFree* temp = dynamic_cast<RythmeFree*>(_data) ;
          if (temp != NULL)
          	temp->save() ;
          else
          {
          	delete(_data) ;
            _data = new RythmeFree(_parent) ;
          }
        }
        else
        	_data = new  RythmeFree(_parent) ;
        _data->save() ;
        break ;

      case 2:
      	_type = DayRythm ;
        if (NULL != _data)
        {
        	DayRythme* temp = dynamic_cast<DayRythme *>(_data) ;
          if (temp != NULL)
          	temp->save() ;
          else
          {
          	delete(_data) ;
            _data = new DayRythme(_parent) ;
          }
        }
        else
        	_data = new  DayRythme(_parent) ;
        _data->save() ;
        break ;

      case 3:
      	_type = Day1Day2Day3 ;
        if (NULL != _data)
        {
        	Day1Day2Day3Rythme* temp = dynamic_cast<Day1Day2Day3Rythme *>(_data) ;
          if (temp != NULL)
          	temp->save() ;
          else
          {
          	delete(_data) ;
            _data = new Day1Day2Day3Rythme(_parent) ;
          }
        }
        else
        	_data = new Day1Day2Day3Rythme(_parent) ;
        _data->save() ;
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
 _parent->GetWindow()->_extendtedCycle = !_parent->GetWindow()->_extendtedCycle ;
 _parent->GetWindow()->DrawCycleMode() ;
 _parent->GetWindow()->CycleSimple->SetSelIndex(_SelectedIndex) ;
}

void
RythmeSimple::save()
{
	_SelectedIndex = _parent->GetWindow()->CycleSimple->GetSelIndex() ;
}

std::string
RythmeSimple::IsValid()
{
  return "" ; // FIXME
}

bool
RythmeSimple::Equal(RythmeBase& /* temp */)
{
  return true ; // FIXME
}

NSPatPathoArray*
RythmeSimple::SimplifiedTreeForInterpretation()
{
  return (*_parent->GetWindow()->CycleSimple)[_SelectedIndex]->getElement() ;
}

//
// ---------------------- DayRythme ----------------------
//

DayRythme::DayRythme(NSMedicCycleGlobal* temp) : RythmeBase(temp)
{
	for (int i = 0; i < 7; i++)
  	_week[i] = false ;
}

NSPatPathoArray*
DayRythme::SimplifiedTreeForInterpretation()
{
  char* temp[] = {"KJLUN1", "KJMAR1", "KJMER1", "KJJEU1", "KJVEN1", "KJSAM1", "KJDIM1"} ;
  NSPatPathoArray* result = new NSPatPathoArray(_parent->pContexte->getSuperviseur()) ;
  result->ajoutePatho("KRYTP1", 0) ; //Ajoue du noeud principal

  for (int i = 0; i < 7; i++)
    if (_week[i] == true)
      result->ajoutePatho(temp[i], 1) ;

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
    return "Rythme journalier non remplie" ;
  return "" ;
}

bool
DayRythme::Equal(RythmeBase& /* temp */)
{
  return true ; // FIXME
}

void
DayRythme::ReinitDialog(NSPosologieBlock* win)
{
	if (NULL == win)
		return ;

  win->pLundi->setValue(BF_UNCHECKED );
  win->pMardi->setValue(BF_UNCHECKED );
  win->pMercredi->setValue(BF_UNCHECKED );
  win->pJeudi->setValue(BF_UNCHECKED );
  win->pVendredi->setValue(BF_UNCHECKED );
  win->pSamedi->setValue(BF_UNCHECKED );
  win->pDimanche->setValue(BF_UNCHECKED );
}

void
DayRythme::save()
{
  _week[0] = (bool)_parent->GetWindow()->pLundi->getValue() ;
  _week[1] = (bool)_parent->GetWindow()->pMardi->getValue() ;
  _week[2] = (bool)_parent->GetWindow()->pMercredi->getValue() ;
  _week[3] = (bool)_parent->GetWindow()->pJeudi->getValue() ;
  _week[4] = (bool)_parent->GetWindow()->pVendredi->getValue() ;
  _week[5] = (bool)_parent->GetWindow()->pSamedi->getValue() ;
  _week[6] = (bool)_parent->GetWindow()->pDimanche->getValue() ;
}

void
DayRythme::Load()
{
  _parent->GetWindow()->pLundi->setValue( (_week[0] == true) ? BF_CHECKED : BF_UNCHECKED ) ;
  _parent->GetWindow()->pMardi->setValue( (_week[1] == true) ? BF_CHECKED : BF_UNCHECKED ) ;
  _parent->GetWindow()->pMercredi->setValue( (_week[2] == true) ? BF_CHECKED : BF_UNCHECKED ) ;
  _parent->GetWindow()->pJeudi->setValue( (_week[3] == true) ? BF_CHECKED : BF_UNCHECKED ) ;
  _parent->GetWindow()->pVendredi->setValue( (_week[4] == true) ? BF_CHECKED : BF_UNCHECKED ) ;
  _parent->GetWindow()->pSamedi->setValue( (_week[5] == true) ? BF_CHECKED : BF_UNCHECKED ) ;
  _parent->GetWindow()->pDimanche->setValue( (_week[6] == true) ? BF_CHECKED : BF_UNCHECKED ) ;
}

//
// ---------------------- Day1Day2Day3Rythme ----------------------
//

Day1Day2Day3Rythme::Day1Day2Day3Rythme(NSMedicCycleGlobal* temp) : RythmeBase(temp)
{
	bVoid   = false ;
	iIndice = 0 ;     // 0 means not initialized
}

NSPatPathoArray*
Day1Day2Day3Rythme::SimplifiedTreeForInterpretation()
{
  NSPatPathoArray* result = new NSPatPathoArray(_parent->pContexte->getSuperviseur()) ;
  result->ajoutePatho("KRYTP1", 0) ; //Ajoue du noeud principal

	result->ajoutePatho("2DAT01", 1) ;
  if (iIndice > 0)
  {
		result->ajoutePatho("VNUMT1", 2) ;

		Message CodeMsg  ;
		CodeMsg.SetUnit("200001") ;
		CodeMsg.SetComplement(IntToString(iIndice)) ;
		result->ajoutePatho("£N0;03", &CodeMsg, 3) ;
  }

  if (bVoid)
  	result->ajoutePatho("9VOID1", 1) ;
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
  return "" ;
}

bool
Day1Day2Day3Rythme::Equal(RythmeBase& /* temp */)
{
  return true ; // FIXME
}

void
Day1Day2Day3Rythme::ReinitDialog(NSPosologieBlock* win)
{
	if (NULL == win)
		return ;

  win->pJour1->setValue(BF_CHECKED) ;
  win->pJour2->setValue(BF_UNCHECKED) ;
}

void
Day1Day2Day3Rythme::save()
{
	std::string temp = "" ;
  temp = _parent->GetWindow()->_numJour->getEditNum()->getText() ;
  if (temp != std::string(""))
  	iIndice = atoi((char*) temp.c_str()) ;

  bVoid = (bool)_parent->GetWindow()->pJour2->getValue() ;
}

void
Day1Day2Day3Rythme::Load()
{
	_parent->GetWindow()->_numJour->getEditNum()->setText(IntToString(iIndice));

  _parent->GetWindow()->pJour1->setValue( (bVoid == false) ? BF_CHECKED : BF_UNCHECKED ) ;
  _parent->GetWindow()->pJour2->setValue( (bVoid == true) ? BF_CHECKED : BF_UNCHECKED ) ;
}

//
// ---------------------- RythmeRegulier ----------------------
//

void
RythmeRegulier::ReinitDialog(NSPosologieBlock* win)
{
	if (NULL == win)
		return ;

  win->pDureeCure->getEditNum()->setText("0") ;
  win->psymDureeCure->setCode("2DAT01") ;
  win->pDureeCycleR->getEditNum()->setText("0") ;
  win->psymDureeCycleR->setCode("2DAT01") ;
}

std::string
RythmeRegulier::IsValid()
{
	if (_symDureeCure == "")
  	return "Rythme journalier imcomplet" ;
	if (_SymDureeCycle == "")
  	return "Rythme journalier imcomplet" ;
	if ((_dureeCycle == "") || (_dureeCycle == "0"))
  	return "Rythme journalier imcomplet" ;
	if ((_dureeCure == "") || (_dureeCure == "0"))
  	return "Rythme journalier imcomplet" ;
	return "" ;
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
  return ((temp ==  std::string("KDURA1")) || (std::string("KDURC1") == temp));
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

  result->ajoutePatho("KRYTP1", 0) ;
  result->ajoutePatho("KRYRE1", 1) ;

  Message CodeMsg  ;

  result->ajoutePatho("KDURA1", 2) ;    // duree de la cure premier edit
	CodeMsg.SetUnit(symcure) ;
	CodeMsg.SetComplement(dure_cure) ;
	result->ajoutePatho("£N0;03", &CodeMsg, 3) ;

	result->ajoutePatho("KDURC1", 2) ;     //duree du cycle
  CodeMsg.Reset() ;
	CodeMsg.SetUnit(SymCycle) ;
	CodeMsg.SetComplement(duree_cycle) ;
	result->ajoutePatho("£N0;03", &CodeMsg, 3) ;

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
	if (NULL == win)
		return ;

  win->pDureeCureF->getEditNum()->setText("0");
  win->psymDureeCureF->setCode("");
  win->pDureeCureFTime->getEditNum()->setText("0");
  win->pDureeCycleFreqF->getEditNum()->setText("0");
  win->psymDureeCycleFreqF->setCode("");
}

bool IsAllowweForFreeRythme(std::string& temp)
{
  return ((temp ==  std::string("KDURA1")) || (std::string("KDURC1") == temp)|| (std::string("VAINC1") == temp));
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
  NSPatPathoArray* result = new NSPatPathoArray(_parent->pContexte->getSuperviseur());
  result->ajoutePatho("KRYTP1", 0);
  result->ajoutePatho("KRYLI1", 1) ;

  Message CodeMsg ;

	if (isValidValue(&_pDureeCureF))
	{
		result->ajoutePatho("KDURA1", 2) ;    // duree de la cure premier edit
    CodeMsg.SetUnit(_psymDureeCureF) ;
    CodeMsg.SetComplement(_pDureeCureF) ;
    result->ajoutePatho("£N0;03", &CodeMsg, 3) ;
	}

	if (isValidValue(&_pDureeCycleFreqF))
	{
    result->ajoutePatho("KDURC1", 2) ;     //duree du cycle
    CodeMsg.Reset() ;
    CodeMsg.SetUnit(_psymDureeCycleFreqF) ;
    CodeMsg.SetComplement(_pDureeCycleFreqF) ;
    result->ajoutePatho("£N0;03", &CodeMsg, 3) ;
	}

	if (isValidValue(&_pDureeCureFTime))
	{
		result->ajoutePatho("VAINC1", 2) ;
    CodeMsg.Reset() ;
		CodeMsg.SetUnit("2FOIS1") ;
		CodeMsg.SetComplement(_pDureeCureFTime) ;
		result->ajoutePatho("£N0;03", &CodeMsg, 3) ;
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
  return true; // FIXME
}

RythmeFree::RythmeFree(NSMedicCycleGlobal* temp)
           :RythmeBase(temp)
{
  _pDureeCureF = "0";
  _psymDureeCureF = "";
  _pDureeCureFTime = "0";
  _pDureeCycleFreqF = "0";
  _psymDureeCycleFreqF = "";
}

void RythmeFree::Load()
{
  _parent->GetWindow()->pDureeCureF->getEditNum()->setText(_pDureeCureF);
  _parent->GetWindow()->psymDureeCureF->setCode(_psymDureeCureF);
  _parent->GetWindow()->pDureeCureFTime->getEditNum()->setText(_pDureeCureFTime);
  _parent->GetWindow()->pDureeCycleFreqF->getEditNum()->setText(_pDureeCycleFreqF);
  _parent->GetWindow()->psymDureeCycleFreqF->setCode(_psymDureeCycleFreqF);
}

void RythmeFree::save()
{
  std::string temp = "" ;
  temp = _parent->GetWindow()->pDureeCureF->getEditNum()->getText() ;
  if (temp != "" )
		_pDureeCureF = temp ;

  temp = _parent->GetWindow()->psymDureeCureF->getSelCode() ;
  if (temp != "" )
		_psymDureeCureF = temp ;

  temp = _parent->GetWindow()->pDureeCureFTime->getEditNum()->getText() ;
  if (temp != "" )
		_pDureeCureFTime = temp ;

  temp = _parent->GetWindow()->pDureeCycleFreqF->getEditNum()->getText() ;
  if (temp != "" )
		_pDureeCycleFreqF = temp ;

  temp = _parent->GetWindow()->psymDureeCycleFreqF->getSelCode() ;
  if (temp != "" )
		_psymDureeCycleFreqF = temp ;
}


RythmeRegulier::RythmeRegulier(NSMedicCycleGlobal* temp): RythmeBase(temp)
{
    _dureeCure = "0";
    _symDureeCure = ""; // Combo
    _dureeCycle = "0";
    _SymDureeCycle = "";
}

void RythmeRegulier::Load()
{
  _parent->GetWindow()->pDureeCure->getEditNum()->setText(_dureeCure);
  _parent->GetWindow()->psymDureeCure->setCode(_symDureeCure);
  _parent->GetWindow()->pDureeCycleR->getEditNum()->setText(_dureeCycle);
  _parent->GetWindow()->psymDureeCycleR->setCode(_SymDureeCycle);
}

void RythmeRegulier::save()
{
  _dureeCure = _parent->GetWindow()->pDureeCure->getEditNum()->getText();
  _symDureeCure = _parent->GetWindow()->psymDureeCure->getSelCode();
  _dureeCycle = _parent->GetWindow()->pDureeCycleR->getEditNum()->getText();
 _SymDureeCycle =  _parent->GetWindow()->psymDureeCycleR->getSelCode();
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
	if (NULL == win)
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
	result->ajoutePatho("KCYTR1", 0) ;                    // code lexique d'un cycle

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
  	// delete pNewPPT2 ;
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
	if ((NULL == pPPT) || (string("") == sCode) || (string("") == sVal))
		return ;

  Message	CodeMsg ;
  CodeMsg.SetUnit(sUnit) ;
  CodeMsg.SetComplement(sVal) ;
  pPPT->ajoutePatho(sCode, &CodeMsg, iColonne++) ;
}

void createNodeComplement(NSPatPathoArray *pPPT, string sCode, string sUnit, float fVal, int iColonne)
{
	if ((NULL == pPPT) || (string("") == sCode))
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
	if ((NULL == pPPT) || (string("") == sCode))
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
  string sVal = "" ;

  if ((NULL == pPPT) || (NULL == pptIter) || (string("") == sCode) || (string("") == sUnit))
		return sVal ;

  if (((*pptIter) != pPPT->end()) && ((**pptIter)->getColonne() > iColonneBase))
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

