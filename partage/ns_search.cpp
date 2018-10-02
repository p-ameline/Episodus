// -----------------------------------------------------------------------------// ns_search.cpp
// -----------------------------------------------------------------------------
// Research methods
// -----------------------------------------------------------------------------
// $Revision: 1.6 $
// $Author: pameline $
// $Date: 2014/01/29 20:44:39 $
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Copyright Nautilus, 2004
// http://www.nautilus-info.com
// -----------------------------------------------------------------------------
// Ce logiciel est un programme informatique servant à gérer et traiter les
// informations de santé d'une personne.
//
// Ce logiciel est régi par la licence CeCILL soumise au droit français et
// respectant les principes de diffusion des logiciels libres. Vous pouvez
// utiliser, modifier et/ou redistribuer ce programme sous les conditions de la
// licence CeCILL telle que diffusée par le CEA, le CNRS et l'INRIA sur le site
// "http://www.cecill.info".
//
// En contrepartie de l'accessibilité au code source et des droits de copie, de
// modification et de redistribution accordés par cette licence, il n'est offert
// aux utilisateurs qu'une garantie limitée. Pour les mêmes raisons, seule une
// responsabilité restreinte pèse sur l'auteur du programme, le titulaire des
// droits patrimoniaux et les concédants successifs.
//
// A cet égard  l'attention de l'utilisateur est attirée sur les risques
// associés au chargement, à l'utilisation, à la modification et/ou au
// développement et à la reproduction du logiciel par l'utilisateur étant donné
// sa spécificité de logiciel libre, qui peut le rendre complexe à manipuler et
// qui le réserve donc à des développeurs et des professionnels avertis
// possédant des connaissances informatiques approfondies. Les utilisateurs sont
// donc invités à charger et tester l'adéquation du logiciel à leurs besoins
// dans des conditions permettant d'assurer la sécurité de leurs systèmes et ou
// de leurs données et, plus généralement, à l'utiliser et l'exploiter dans les
// mêmes conditions de sécurité.
//
// Le fait que vous puissiez accéder à cet en-tête signifie que vous avez pris
// connaissance de la licence CeCILL, et que vous en avez accepté les termes.
// -----------------------------------------------------------------------------
// This software is a computer program whose purpose is to manage and process
// a person's health data.
//
// This software is governed by the CeCILL  license under French law and abiding
// by the rules of distribution of free software. You can use, modify and/ or
// redistribute the software under the terms of the CeCILL license as circulated
// by CEA, CNRS and INRIA at the following URL "http://www.cecill.info".
//
// As a counterpart to the access to the source code and  rights to copy, modify
// and redistribute granted by the license, users are provided only with a
// limited warranty and the software's author, the holder of the economic
// rights, and the successive licensors have only limited liability.
//
// In this respect, the user's attention is drawn to the risks associated with
// loading, using, modifying and/or developing or reproducing the software by
// the user in light of its specific status of free software, that may mean that
// it is complicated to manipulate, and that also therefore means that it is
// reserved for developers and experienced professionals having in-depth
// computer knowledge. Users are therefore encouraged to load and test the
// software's suitability as regards their requirements in conditions enabling
// the security of their systems and/or data to be ensured and, more generally,
// to use and operate it in the same conditions as regards security.
//
// The fact that you are presently reading this means that you have had
// knowledge of the CeCILL license and that you accept its terms.
// -----------------------------------------------------------------------------
#ifdef _ENTERPRISE_DLL  #include "enterpriseLus/superLus.h"
  #include "enterpriseLus/nsdivfctForCgi.h"
#else
  #include "nautilus\nssuper.h"
  #include "partage\nsdivfct.h"
#endif#include "partage\ns_search.h"#include "nsldv\nsldvuti.h"long MappingNSSearchResult::lObjectCount = 0 ;long NSSearchStruct::lObjectCount = 0 ;//***************************************************************************//                      MappingNSSearchResult
//***************************************************************************MappingNSSearchResult::MappingNSSearchResult(const MappingNSSearchResult& rv){  if (rv._data.empty())    return ;  _data = rv._data ;}MappingNSSearchResult&MappingNSSearchResult::operator=(const MappingNSSearchResult& src){  if (this == &src)		return *this ;  reinit() ;  _data = src._data ;  return *this ;}voidMappingNSSearchResult::reinit(){  _data.clear() ;}intMappingNSSearchResult::getNunberOfResultBeforeDate(std::string dat){  if (_data.empty())    return 0 ;  int result = 0 ;  MMapRevIt end = _data.rend() ;  for ( MMapRevIt it = _data.rbegin() ; end != it ; it++)  {    std::string date = it->first ;    if (date > dat)      result++  ;  }  return result ;}voidMappingNSSearchResult::insert(std::string date, std::string node){  _data.insert(make_pair(date, node)) ;}boolMappingNSSearchResult::empty(){  return _data.empty() ;}intMappingNSSearchResult::size(){  return _data.size() ;}MappingNSSearchResult::MMapRevItMappingNSSearchResult::rbegin(){  return (_data.rbegin()) ;}MappingNSSearchResult::MMapRevItMappingNSSearchResult::rend(){  return (_data.rend()) ;}MappingNSSearchResult::MMapItMappingNSSearchResult::begin(){  return (_data.begin()) ;}MappingNSSearchResult::MMapIt
MappingNSSearchResult::end()
{
  return (_data.end()) ;
}
voidMappingNSSearchResult::fullRData(MMapIt it, std::string& date, std::string& node){  date = it->first ;  node = it->second ;}voidMappingNSSearchResult::fullData(MMapRevIt it, std::string& date, std::string& node){  date = it->first ;  node = it->second ;}//***************************************************************************//                               NSSearchStruct
//***************************************************************************boolareSameSearchStruct(NSSearchStruct *pStruct1, NSSearchStruct *pStruct2){  // Both are null or default: Ok  //  if (((NULL == pStruct1) || pStruct1->isDefault()) &&      ((NULL == pStruct2) || pStruct2->isDefault()))    return true ;  // One is null: Bad  //  if ((NULL == pStruct1) || (NULL == pStruct2))    return false ;  return (pStruct1->isSameAs(*pStruct2)) ;}voidNSSearchStruct::reinit()  /* reinit the search place */{  if (false == _aFoundNodes.empty())    _aFoundNodes.reinit() ;  _iStatus = NSSearchStruct::foundNothing ;}boolNSSearchStruct::accept_date(std::string sDate){  if ((std::string("") == _sStartingDate) && (std::string("") == _sEndingDate))    return true ;  bool result = true ;  if ((std::string("") != _sStartingDate) && (sDate < _sStartingDate))    result = false ;  if ((std::string("") != _sEndingDate) && (sDate > _sEndingDate))    result = false ;  return (result) ;}boolNSSearchStruct::accept_episode(std::string sOpeningDate, std::string sClosingDate){  if (episodeUndefined == _iEpisodeStatus)    return true ;  // No opening date -> cannot be opened or in the future  //  if ((std::string("") == sOpeningDate) && ((episodeOpened == _iEpisodeStatus) ||                                            (episodeFuture == _iEpisodeStatus)))    return false ;  // No closing date -> cannot be closed (but may be opened)  //  if ((std::string("") == sClosingDate) && (episodeClosed == _iEpisodeStatus))    return false ;  // get current date  //  NVLdVTemps tpsNow ;
  tpsNow.takeTime() ;  if (std::string("") != sOpeningDate)  {    NVLdVTemps tpsOpening ;    tpsOpening.initFromDateHeure(sOpeningDate) ;    if ((tpsOpening > tpsNow) && ((episodeOpened == _iEpisodeStatus) ||                                  (episodeClosed == _iEpisodeStatus)))      return false ;    if ((tpsOpening < tpsNow) && (episodeFuture == _iEpisodeStatus))      return false ;  }  if (std::string("") != sClosingDate)  {    NVLdVTemps tpsClosing ;    tpsClosing.initFromDateHeure(sClosingDate) ;    if ((tpsClosing > tpsNow) && (episodeClosed == _iEpisodeStatus))      return false ;    if ((tpsClosing < tpsNow) && ((episodeOpened == _iEpisodeStatus) ||                                  (episodeFuture == _iEpisodeStatus)))      return false ;  }  return true ;}voidNSSearchStruct::create_result(){  if (0 != _iNbNodes)  {    if      (_aFoundNodes.size() == _iNbNodes)      _iStatus = NSSearchStruct::foundFull ;    else if (_aFoundNodes.size() > 0)      _iStatus = NSSearchStruct::foundPartial ;    else      _iStatus = NSSearchStruct::foundNothing ;  }  else  {    if (_aFoundNodes.size() > 0)      _iStatus = NSSearchStruct::foundFull ;    else      _iStatus = NSSearchStruct::foundNothing ;  }}NSSearchStruct::NSSearchStruct(SEARCHMODE    imode,                               SEARCHTYPE    itype,                               SEARCHPOSIT   inodesposit,                               int           inbnodes,                               std::string   startingdatePattern,                               std::string   startingdate,                               std::string   endingdatePattern,                               std::string   endingdate,                               std::string*  psstartingnode,                               EPISODESTATUS episodeStatus,                               std::string   mandatoryPath,                               std::string   archetypeId,                               bool          bmandatoryesorno,                               SEARCHPOSIT   mandatoryposit){  _iMode                = imode ;  _iNbNodes             = inbnodes ;  _iType                = itype ;  _iNodesPosit          = inodesposit ;  _sStartingDatePattern = startingdatePattern ;  _sStartingDate        = startingdate ;  _sEndingDatePattern   = endingdatePattern ;  _sEndingDate          = endingdate ;  _psStartingNode       = psstartingnode ;  _psStopNode           = (std::string*) 0 ;  _sUnit                = std::string("") ;  _iEpisodeStatus       = episodeStatus ;  _sMandatoryPath       = mandatoryPath ;  _bMandatoryForYesOrNo = bmandatoryesorno ;  _iMandatoryPosit      = mandatoryposit ;  _sMandatoryArchetype  = archetypeId ;  reinit() ;}NSSearchStruct::NSSearchStruct(const NSSearchStruct& rv){  _iMode           = rv._iMode ;  _iType           = rv._iType ;  _iNodesPosit     = rv._iNodesPosit ;  _iStatus         = rv._iStatus ;  _iNbNodes             = rv._iNbNodes ;  _sStartingDatePattern = rv._sStartingDatePattern ;  _sStartingDate        = rv._sStartingDate ;  _sEndingDatePattern   = rv._sEndingDatePattern ;  _sEndingDate          = rv._sEndingDate ;  _psStartingNode       = rv._psStartingNode ;  _psStopNode           = rv._psStopNode ;  _iEpisodeStatus       = rv._iEpisodeStatus ;  _sMandatoryPath       = rv._sMandatoryPath ;  _bMandatoryForYesOrNo = rv._bMandatoryForYesOrNo ;  _iMandatoryPosit      = rv._iMandatoryPosit ;  _sMandatoryArchetype  = rv._sMandatoryArchetype ;  _aFoundNodes          = rv._aFoundNodes ;  _sUnit                = rv._sUnit ;}voidNSSearchStruct::init(string sParams){  if (string("") == sParams)		return ;

  string sFil = sParams ;

  bool bProgress = true ;
  do
  {
    string sField ;
    splitString(sFil, &sField, &sFil, '|') ;

  	if (sField == sFil)
      bProgress = false ;

    strip(sField, stripBoth) ;

    string sValue = string("") ;
    splitString(sField, &sField, &sValue, '=') ;
    strip(sValue, stripBoth) ;

    if (string("") != sValue)
    {
      // F R and C are used for controls automatic filling
      //
      if      ('D' == sField[0])
      {
      	_sStartingDatePattern = sValue ;

        NVLdVTemps tpsObj ;
        tpsObj.takeTime() ;

        string sAjoutAns  = string(_sStartingDatePattern, 0, 2) ;
        string sAjoutMois = string(_sStartingDatePattern, 2, 2) ;
        string sAjoutJour = string(_sStartingDatePattern, 4, 2) ;

        if (sAjoutAns != "00")
  	      tpsObj.ajouteAns((-1) * atoi(sAjoutAns.c_str()), false /* don't normalize */) ;
        if (sAjoutMois != "00")
  	      tpsObj.ajouteMois((-1) * atoi(sAjoutMois.c_str()), false /* don't normalize */) ;
        if (sAjoutJour != "00")
  	      tpsObj.ajouteJours((-1) * atoi(sAjoutJour.c_str())) ;

        _sStartingDate = tpsObj.donneDateHeure() ;
      }
      else if ('M' == sField[0])
      {
      	_sMandatoryPath = sValue ;
        _bMandatoryForYesOrNo = false ;
      }
      else if ('N' == sField[0])
      {
      	_sMandatoryPath = sValue ;
        _bMandatoryForYesOrNo = true ;
      }
      else if ('E' == sField[0])
      {
        if      ('C' == sValue[0])
          _iEpisodeStatus = episodeClosed ;
        else if ('O' == sValue[0])
          _iEpisodeStatus = episodeOpened ;
        else if ('F' == sValue[0])
          _iEpisodeStatus = episodeFuture ;
      }
      else if ('A' == sField[0])
      {
      	_sMandatoryArchetype = sValue ;
      }
    }
  }
  while(bProgress) ;}NSSearchStruct&NSSearchStruct::operator=(const NSSearchStruct& src){  if (this == &src)		return *this ;  _iMode                = src._iMode ;  _iType                = src._iType ;  _iNodesPosit          = src._iNodesPosit ;  _iStatus              = src._iStatus ;  _iNbNodes             = src._iNbNodes ;  _sStartingDatePattern = src._sStartingDatePattern ;  _sStartingDate        = src._sStartingDate ;  _sEndingDatePattern   = src._sEndingDatePattern ;  _sEndingDate          = src._sEndingDate ;  _psStartingNode       = src._psStartingNode ;  _psStopNode           = src._psStopNode ;  _iEpisodeStatus       = src._iEpisodeStatus ;  _sMandatoryPath       = src._sMandatoryPath ;  _bMandatoryForYesOrNo = src._bMandatoryForYesOrNo ;  _iMandatoryPosit      = src._iMandatoryPosit ;  _sMandatoryArchetype  = src._sMandatoryArchetype ;  _aFoundNodes          = src._aFoundNodes ;  _sUnit                = src._sUnit ;  return *this ;}intNSSearchStruct::operator==(const NSSearchStruct& src){  if ((_iMode                == src._iMode)           &&      (_iType                == src._iType)           &&      (_iNodesPosit          == src._iNodesPosit)     &&      (_iNbNodes             == src._iNbNodes)        &&      (_sStartingDate        == src._sStartingDate)   &&      (_sEndingDate          == src._sEndingDate)     &&      (_psStartingNode       == src._psStartingNode)  &&      (_psStopNode           == src._psStopNode)      &&      (_iEpisodeStatus       == src._iEpisodeStatus)  &&      (_sMandatoryPath       == src._sMandatoryPath)  &&      (_bMandatoryForYesOrNo == src._bMandatoryForYesOrNo) &&      (_iMandatoryPosit      == src._iMandatoryPosit) &&      (_sMandatoryArchetype  == src._sMandatoryArchetype) &&      (_sUnit                == src._sUnit))    return 1 ;  return 0 ;}// The problem with == operator is that, if two "identical" strcutures have// been instantiated at different moment, their _sStartingDate and// _sEndingDate will not be the same, event if the pattern is common//// This function compares patterns instead of instantiated dates//boolNSSearchStruct::isSameAs(const NSSearchStruct& src){  if ((_iMode                == src._iMode)           &&      (_iType                == src._iType)           &&      (_iNodesPosit          == src._iNodesPosit)     &&      (_iNbNodes             == src._iNbNodes)        &&      (_sStartingDatePattern == src._sStartingDatePattern)   &&      (_sEndingDatePattern   == src._sEndingDatePattern)     &&      (_psStartingNode       == src._psStartingNode)  &&      (_psStopNode           == src._psStopNode)      &&      (_iEpisodeStatus       == src._iEpisodeStatus)  &&      (_sMandatoryPath       == src._sMandatoryPath)  &&      (_bMandatoryForYesOrNo == src._bMandatoryForYesOrNo) &&      (_iMandatoryPosit      == src._iMandatoryPosit) &&      (_sMandatoryArchetype  == src._sMandatoryArchetype) &&      (_sUnit                == src._sUnit))    return true ;  return false ;}// Is this search structure initialized with default values// that is "like null"//boolNSSearchStruct::isDefault(){  if ((modeAsIs         == _iMode) &&      (typePersoAnt     == _iType) &&      (positFirstInTime == _iNodesPosit) &&      (std::string("")  == _sStartingDatePattern) &&      (std::string("")  == _sEndingDatePattern) &&      (episodeUndefined == _iEpisodeStatus) &&      (std::string("")  == _sMandatoryPath) &&      (std::string("")  == _sMandatoryArchetype) &&      (positFirstInTime == _iMandatoryPosit))    return true ;  return false ;}