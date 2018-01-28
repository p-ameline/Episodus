/*
** Copyright Nautilus, (10/9/2004)
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
#include "ns_ob1\Transfer.h"
#include "ns_ob1\BB1Object.h"

Transfert::Transfert()
{
	_transfer_class = string("") ;
}

Transfert::Transfert(const Transfert& temp)
{
	_transfer_class = temp._transfer_class ;

  if (false == temp._data.empty())
  	for (std::vector< TransfertClass* >::iterator iter = temp.getData()->begin() ; temp.getData()->end() != iter ; iter++)
    {
    	TransfertClass* temp = new TransfertClass((*iter)->Key(), (*iter)->SearchStruct(), (*iter)->OBject(), (*iter)->getPersistenceHandle()) ;
    	_data.push_back(temp) ;
    }
}

Transfert::~Transfert()
{
	vider() ;
}

void
Transfert::insertItem(std::string& key, NSSearchStruct* pSearchStruct, BB1Object* val, long int iPersistenceHandle)
{
  if (exist(key, pSearchStruct) == false)
	{
  	TransfertClass* temp = new TransfertClass(key, pSearchStruct, val, iPersistenceHandle) ;
    _data.push_back(temp) ;
  }
}

/*
BB1Object*
Transfert::operator[] (std::string& key)
{
	if (_data.empty())
  	return (BB1Object *) 0 ;

  for (std::vector<TransfertClass*>::iterator iter = _data.begin() ; _data.end() != iter ; iter++)
  	if ((*iter)->Key() == key)
      return ((*iter)->OBject()) ;

  return (BB1Object *) 0 ;
}
*/

BB1Object*
Transfert::getObject(std::string& sKey, std::string& sSearchParams)
{
  if (_data.empty())
  	return (BB1Object *) 0 ;

  NSSearchStruct searchStruct ;
  searchStruct.init(sSearchParams) ;

  for (std::vector<TransfertClass*>::iterator iter = _data.begin() ; _data.end() != iter ; iter++)
  	if (((*iter)->Key() == sKey) && areSameSearchStruct(&searchStruct, (*iter)->SearchStruct()))
      return ((*iter)->OBject()) ;

  return (BB1Object *) 0 ;
}

bool
Transfert::exist(std::string& key, std::string& sSearchParams)
{
  if (_data.empty())
  	return false ;

  NSSearchStruct searchStruct ;
  searchStruct.init(sSearchParams) ;

  return exist(key, &searchStruct) ;
}

bool
Transfert::exist(std::string& key, NSSearchStruct* pSearchStruct)
{
	if (_data.empty())
  	return false ;

  for (std::vector<TransfertClass*>::iterator iter = _data.begin() ; _data.end() != iter ; iter++)
  	if (((*iter)->Key() == key) && areSameSearchStruct(pSearchStruct, (*iter)->SearchStruct()))
      return true ;

  return false ;
}

void
Transfert::setAskedQuestion(BB1Object* pQuest, long int iPersistenceHandle)
{
	insertItem(string("Question"), (NSSearchStruct*) 0, pQuest, iPersistenceHandle) ;
}

BB1Object*
Transfert::getAskedQuestion()
{
	string sQuest  = string("Question") ;
  string sSearch = string("") ;
  return getObject(sQuest, sSearch) ;
	// return (*this)[sQuest] ;
}

void
Transfert::vider()
{
	if (_data.empty())
  	return ;

	for (std::vector<TransfertClass*>::iterator iter = _data.begin() ; _data.end() != iter ; )
	{
		if (*iter)
			delete(*iter) ;
		_data.erase(iter) ;
	}
}

Transfert&
Transfert::operator=(const Transfert& src)
{
	if (&src == this)
  	return *this ;

	vider() ;

	_transfer_class = src._transfer_class ;

  if (false == src._data.empty())
  	for (std::vector< TransfertClass* >::iterator iter = src.getData()->begin() ; src.getData()->end() != iter ; iter++)
    {
    	TransfertClass* temp = new TransfertClass((*iter)->Key(), (*iter)->SearchStruct(), (*iter)->OBject(), (*iter)->getPersistenceHandle()) ;
    	_data.push_back(temp) ;
    }

	return *this ;
}

//  ------------ TransfertClass ------------
//

TransfertClass::~TransfertClass()
{
  if ((_object) && (-1 != _iPersistenceHandle))
    _object->BB().unsuscribePersistenceToQuestion(_object, _iPersistenceHandle) ;
}

