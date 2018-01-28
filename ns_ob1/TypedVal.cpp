/*
** Copyright Nautilus, (10/9/2004)
** fabrice.le_perru@nautilus-info.com
** david.giocanti@nautilus-info.com

** Ce logiciel est un programme informatique servant à [rappeler les
** caractéristiques techniques de votre logiciel].

** Ce logiciel est régi par la licence CeCILL soumise au droit français et
** respectant les principes de diffusion des logiciels libres. Vous pouvez
** utiliser, modifier et/ou redistribuer ce programme sous les conditions
** de la licence CeCILL telle que diffusée par le CEA, le CNRS et l'INRIA
** sur le site "http://www.cecill.info".

** En contrepartie de l'accessibilité au code source et des droits de copie,
** de modification et de redistribution accordés par cette licence, il n'est
** offert aux utilisateurs qu'une garantie limitée.  Pour les mêmes raisons,
** seule une responsabilité restreinte pèse sur l'auteur du programme,  le
** titulaire des droits patrimoniaux et les concédants successifs.

** A cet égard  l'attention de l'utilisateur est attirée sur les risques
** associés au chargement,  à l'utilisation,  à la modification et/ou au
** développement et à la reproduction du logiciel par l'utilisateur étant
** donné sa spécificité de logiciel libre, qui peut le rendre complexe à
** manipuler et qui le réserve donc à des développeurs et des professionnels
** avertis possédant  des  connaissances  informatiques approfondies.  Les
** utilisateurs sont donc invités à charger  et  tester  l'adéquation  du
** logiciel à leurs besoins dans des conditions permettant d'assurer la
** sécurité de leurs systèmes et ou de leurs données et, plus généralement,
** à l'utiliser et l'exploiter dans les mêmes conditions de sécurité.

** Le fait que vous puissiez accéder à cet en-tête signifie que vous avez
** pris connaissance de la licence CeCILL, et que vous en avez accepté les
** termes.
*/

#ifdef __GNUC__
# pragma implementation
#endif

#include <iostream.h>   // for operator<<(...)

#ifndef FOR_WINDOWS
# include "TypedVal.h"
#else
# include "ns_ob1\TypedVal.h"
#endif

long TypedVal::_lTVCount = 0 ;

// -----------------------------------------------------------------------------
// Overload the << operator to write a TypedVal to an output stream.
// -----------------------------------------------------------------------------
std::ostream& operator <<(std::ostream& os, const TypedVal& tv)
{
  os << "[ " << tv.typeName() << " " ;
  tv.putTypedVal(os) ;
  os << " ]" ;
  return (os) ;
}

// -----------------------------------------------------------------------------
// This is the write accessor
// -----------------------------------------------------------------------------
TypedVal&
TypedVal::operator=(const TypedVal& v)
{
	if (this == &v)
		return *this ;

	// First, emptying current object
  switch (_discr)
  {
    case OB1QEST :
      if (NULLLNAUTQUEST != _squest)
      {
      	delete _squest ;
      	_squest = NULLLNAUTQUEST ;
      }
      break ;
    case OB1ANSWERSTATUS :
      if (NULLLANSWERSTATUS != _pAnswerStatus)
      {
      	delete _pAnswerStatus ;
      	_pAnswerStatus = NULLLANSWERSTATUS ;
      }
      break ;
    case BB1PATPATHO	:
      if (true != _extPtr)
      {
        delete _patpathoVal ;
      	_patpathoVal = NULLPATHO ;
      }
    	break ;
    case BB1NAUTEVENT	:
      if (true != _extPtr)
      {
        delete _nautEvent ;
      	_nautEvent = NULLNAUTILUSEVENT ;
      }
    	break ;
    case BB1SEARCHSTRUCT	:
      delete _pSearchStruct ;
      _pSearchStruct = NULLSEARCHSTRUCT ;
    	break ;
    case BB1HANDLES :
      {
        if (false == _aHandles.empty())
          for (vector<HANDLE>::iterator it = (HANDLE*)v._aHandles.begin() ; v._aHandles.end() != it ; )
            _aHandles.erase(it) ;
      }
      break ;
#ifdef __OB1__UNOPTIM__
		case BB1PATHS :
      if (pathsVal != NULL)
      {
      	delete pathsVal ;
      	pathsVal = NULL ;
      }
      break ;
#endif
  }

  InitWithZeros() ;

  // assign the discriminant
  _discr = v._discr ;

  // assign the value
  switch (_discr)
  {
    case BB1DOUBLE		:	doubleVal 	= v.doubleVal ; break ;
    case BB1LONG			:	longVal   	= v.longVal ;		break ;
    case BB1STRING		:	_StringVal 	= v._StringVal ;	break ;
    case OB1QEST      :
      if ((NULL != v._squest) && (NULLLNAUTQUEST != v._squest))
      	_squest = new NautilusQuestion(v._squest) ;
   		break ;
    case OB1ANSWERSTATUS :
      if ((NULL != v._pAnswerStatus) && (NULLLANSWERSTATUS != v._pAnswerStatus))
      	_pAnswerStatus = new AnswerStatus(v._pAnswerStatus) ;
   		break ;
    case BB1POINTER		:	ptrVal    	= v.ptrVal ;		break ;
    case BB1NAUTEVENT :
    	if (true == v._extPtr)
      {
        _extPtr = true;
        _nautEvent = v._nautEvent ;
      }
      else
      {
        _extPtr = false;
        _nautEvent	= new NautilusEvent(*(v._nautEvent)) ;
      }
      break ;
    case BB1SEARCHSTRUCT :
      if (v._pSearchStruct)
        _pSearchStruct	= new NSSearchStruct(*(v._pSearchStruct)) ;
      break ;
    case BB1PATPATHO	:
      if (true == v._extPtr)
      {
        _extPtr = true ;
        _patpathoVal = v._patpathoVal ;
      }
      else
      {
        _extPtr = false;
        _patpathoVal = new NSPatPathoArray(*(v._patpathoVal)) ;
      }
    	break ;
    case BB1PATPATHOITER	:
      _papeCathoIter = new PatPathoIter(*(v._papeCathoIter)) ;
      break ;
    case BB1HANDLES :
      {
        if (false == v._aHandles.empty())
          for (vector<HANDLE>::iterator it = (HANDLE*)v._aHandles.begin() ; v._aHandles.end() != it ; it++)
            _aHandles.push_back(*it) ;
      }
      break ;

    case BB1OBJ				: _objectVal	= v._objectVal ; break ;
#ifdef __OB1__UNOPTIM__
    case BB1INT				:	intVal    	= v.intVal ;		 break ;
    case BB1FLOAT			:	floatVal  	= v.floatVal ;	 break ;
    case BB1UCHAR			:	uCharVal  	= v.uCharVal ;	 break ;
    case BB1UINT			:	uIntVal   	= v.uIntVal ;		 break ;
    case BB1ULONG			:	uLongVal  	= v.uLongVal ;	 break ;
    case BB1USHORT		:	uShortVal 	= v.uShortVal ;	 break ;
    case BB1SHORT			:	shortVal  	= v.shortVal ;	 break ;
    case BB1CHAR			:	charVal   	= v.charVal ;		 break ;
    case BB1PATHS			:	pathsVal		= new PathsList(*(v.pathsVal)) ; break ;
#endif

    default:		{ /* unknown type */ }
  }

  // return the recipient of the message
  return (*this) ;
}

void
TypedVal::swapPatho(NSPatPathoArray *patho, bool bExt)
{
	if (BB1PATPATHO != _discr)
		return ;

	if (_patpathoVal && (false == _extPtr))
  	delete _patpathoVal ;
  _patpathoVal = NULLPATHO ;

  if (NULL == patho)
		return ;

  _extPtr = bExt ;
  if (true == bExt)
  	_patpathoVal = patho ;
	else
  	_patpathoVal = new NSPatPathoArray(*patho) ;
}

void
TypedVal::addHandle(HANDLE hHandle)
{
  if (BB1HANDLES != _discr)
    return ;

  if (false == isThisHandleHere(hHandle))
    _aHandles.push_back(hHandle) ;
}

// Since counter is not linked with handle, this function must only get used
// for BBK interface
//
void
TypedVal::addHandleWithCounter(HANDLE hHandle)
{
  if (BB1HANDLES != _discr)
    return ;

  if (false == isThisHandleHere(hHandle))
  {
    _aHandles.push_back(hHandle) ;
    intVal = 1 ;
  }
  else
    intVal++ ;
}

void
TypedVal::removeHandleWithCounter(HANDLE hHandle)
{
  if ((BB1HANDLES != _discr) || (_aHandles.empty()))
    return ;

  vector<HANDLE>::iterator it = _aHandles.begin() ;
  for ( ; _aHandles.end() != it ; )
  {
    if (*it == hHandle)
    {
      intVal-- ;
      if (intVal <= 0)
        _aHandles.erase(it) ;
      else
        it++ ;
    }
    else
      it++ ;
  }
}

void
TypedVal::removeHandle(HANDLE hHandle)
{
  if ((BB1HANDLES != _discr) || (_aHandles.empty()))
    return ;

  vector<HANDLE>::iterator it = _aHandles.begin() ;
  for ( ; _aHandles.end() != it ; )
  {
    if (*it == hHandle)
      _aHandles.erase(it) ;
    else
      it++ ;
  }
}

bool
TypedVal::isThisHandleHere(HANDLE hHandle)
{
  if ((BB1HANDLES != _discr) || (_aHandles.empty()))
    return false ;

  vector<HANDLE>::iterator it = _aHandles.begin() ;
  for ( ; _aHandles.end() != it ; it++)
    if (*it == hHandle)
      return true ;

  return false ;
}

/*
bool TypedVal::operator==(const TypedVal& v) const
{
  if (this == &v)
		return (true) ;

	if (discr != v.discr)
		return (false) ;
	else
  {
    switch (discr)
    {
			case BB1DOUBLE		:	return (doubleVal 	== v.doubleVal) ;
			case BB1LONG			:	return (longVal   	== v.longVal) ;
			case BB1STRING		:	return (StringVal 	== v.StringVal) ;
			case BB1POINTER		:	return (ptrVal    	== v.ptrVal) ;
			case BB1PATPATHO	: return (patpathoVal	== v.patpathoVal) ;
			case BB1OBJ				: return (objectVal		== v.objectVal) ;
      case OB1QEST      :        return (*squest      == *(v.squest) );
      #ifdef __OB1__UNOPTIM__
      case BB1SHORT			:	return (shortVal  	== v.shortVal) ;
      case BB1INT				:	return (intVal    	== v.intVal) ;
      case BB1FLOAT			:	return (floatVal  	== v.floatVal) ;
      case BB1CHAR			:	return (charVal   	== v.charVal) ;
      case BB1PATHS			:	return (pathsVal 		== v.pathsVal) ;
      case BB1UCHAR			:	return (uCharVal  	== v.uCharVal) ;
			case BB1UINT			:	return (uIntVal   	== v.uIntVal) ;
			case BB1ULONG			:	return (uLongVal  	== v.uLongVal) ;
			case BB1USHORT		:	return (uShortVal 	== v.uShortVal) ;
      #endif
			default           :	return (false) ;
    }
  }
}
*/

// -----------------------------------------------------------------------------
// Convert an AttType to a char *.
// -----------------------------------------------------------------------------
char*
TypedVal::typeName() const
{
  switch (_discr)
  {
		case BB1DOUBLE		   : return("double") ;
		case BB1LONG			   : return("long") ;
		case BB1STRING		   : return("string") ;
		case BB1POINTER		   : return("void *") ;
		case BB1PATPATHO	   : return("NSPatPathoArray *") ;
    case BB1PATPATHOITER : return("PatPathoIter *") ;
    case OB1QEST         : return("OB1QEST*") ;
    case OB1ANSWERSTATUS : return("OB1ANSWERSTATUS*") ;
		case BB1OBJ				   : return("BB1Object *") ;
    case BB1NAUTEVENT    : return("NautilusEvent *") ;
    case BB1SEARCHSTRUCT : return("NSSearchStruct *") ;

    #ifdef __OB1__UNOPTIM__
    case BB1SHORT			:	return("short") ;
    case BB1FLOAT			:	return("float") ;
    case BB1INT				:	return("int") ;
    case BB1UCHAR			:	return("unsigned char") ;
		case BB1UINT			:	return("unsigned int") ;
		case BB1ULONG			:	return("unsigned long") ;
		case BB1USHORT		:	return("unsigned short") ;
    case BB1CHAR			:	return("char") ;
    case BB1PATHS			: return("PathsList *") ;
    #endif
    default						:	return("unknown type") ;
	}
}

// -----------------------------------------------------------------------------
// Since there is no way to return values of different types, this method has to
// do the output operation.
// -----------------------------------------------------------------------------
void
TypedVal::putTypedVal(std::ostream& os) const
{
  switch (_discr)
  {
    #ifdef __OB1__UNOPTIM__
		case BB1CHAR			:	os << getChar() ;			break ;
		case BB1DOUBLE		:	os << getDouble() ;		break ;
		case BB1FLOAT			:	os << getFloat() ;		break ;
		case BB1INT				:	os << getInt() ;			break ;
		case BB1LONG			:	os << getLong() ;			break ;
		case BB1SHORT			:	os << getShort() ;		break ;
		case BB1STRING		:	os << getString() ;		break ;
		case BB1UCHAR			:	os << getUChar() ;		break ;
		case BB1UINT			:	os << getUInt() ;			break ;
		case BB1ULONG			:	os << getULong() ;		break ;
		case BB1USHORT		:	os << getShort() ;		break ;
		case BB1POINTER		:	os << getPtr() ;			break ;
    #endif
//		case BB1PATHS			: os << getPaths() ;		break ;
//		case BB1PATPATHO	: os << getPatPatho() ;	break ;
//		case BB1OBJ				: os << getObj() ;			break ;
    default						:	os << "" ;
  }
}

std::string
TypedVal::toString() const
{
  switch (_discr)
  {
		case BB1DOUBLE		   : return "TypedVal : Double" ;
    case BB1LONG			   : return "TypedVal : Long" ;
		case BB1STRING		   : return (getString()) ;
    case OB1QEST         : return (getNautQuest()->Question()) ;
		case BB1POINTER		   : return "TypedVal : pointer" ;
		case BB1PATPATHO	   : return "TypedVal : PATPATHO" ;
  	case BB1OBJ				   : return "TypedVal : OBJ" ;
    case BB1NAUTEVENT    : return "TypedVal : NautilusEvent" ;
    case BB1SEARCHSTRUCT : return "TypedVal : NSSearchStruct" ;
    #ifdef __OB1__UNOPTIM__
    case BB1INT				: return "TypedVal : Int";
    case BB1FLOAT			: return "TypedVal : Float";
    case BB1INT				: return "TypedVal : Int";
    case BB1CHAR			: return "TypedVal : char";
    case BB1PATHS			: return "TypedVal : PATH";
    case BB1UCHAR			:	return "TypedVal : UChar";
		case BB1UINT			:	return "TypedVal : UINT";
		case BB1ULONG			:	return "TypedVal : ULONG";
		case BB1USHORT		:	return "TypedVal : USSHORT";
    case BB1SHORT			:	return "TypedVal : Short";
    #endif
    default						:	return "Undefined TypedVal" ;
  }
}

std::string
ComputeSortingId(TypedVal& temp)
{
  std::string result = "" ;
  switch (temp._discr)
  {
		case BB1DOUBLE		:	result = "TypedVal : Double";
    break;
		case BB1STRING		:	result = "string" + (temp.getString()) ;
    break;
    case OB1QEST      : result = "ob1Qest" + (temp.getNautQuest()->Question());
    break;
		case BB1POINTER		:	result = "TypedVal : pointer";
    break;
		case BB1PATPATHO	: result = "TypedVal : PATPATHO";
    break;
  	case BB1OBJ				: result = "TypedVal : OBJ";
    break;
    case BB1LONG			: result = "TypedVal : Long";
    break;
    case BB1NAUTEVENT : result = "TypedVal : NautilusEvent" ;
    break;
    case BB1SEARCHSTRUCT : result = "TypedVal : NSSearchStruct" ;
    break;
    #ifdef __OB1__UNOPTIM__
    case BB1INT				: result = "TypedVal : Int";
    break;
    case BB1FLOAT			: result = "TypedVal : Float";
    break;
    case BB1CHAR			: result = "TypedVal : char";
    break;
    case BB1SHORT			:	result = "TypedVal : Short";
    break;
    case BB1PATHS			: result = "TypedVal : PATH";
    break;
    case BB1UCHAR			:	result = "TypedVal : UChar";
    break;
		case BB1UINT			:	result = "TypedVal : UINT";
    break;
		case BB1ULONG			:	result = "TypedVal : ULONG";
    break;
		case BB1USHORT		:	result = "TypedVal : USSHORT";
    break;
    #endif
    default						:	result = "Undefined TypedVal" ;
    break;
  }
  return result ;
}

std::string GetStringFromSortingId(std::string sortingID)
{
	size_t iStrLen = strlen(sortingID.c_str()) ;

	if (iStrLen < 7)
		return std::string("") ;
  if (std::string(sortingID, 0, 6) != std::string("string"))
  	return std::string("") ;

  return std::string(sortingID, 6, iStrLen - 6) ;
}

