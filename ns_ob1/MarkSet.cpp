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

#include "ns_ob1\MarkSet.h"  /* Header class of the method's after */

long MarkSet::_lObjectCount  = 0 ;

//------------------------------------------------------------------------------
/*
** Markset Class
** Default constructeur
*/
MarkSet::MarkSet()
{
  _lObjectCount++ ;
}

MarkSet::MarkSet(const MarkSet& rv)
        :_Marks(rv._Marks)
{
  _lObjectCount++ ;
}

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/*
** MarkSet Class
** Destructor
*/
MarkSet::~MarkSet()
{
  _lObjectCount-- ;
}
//------------------------------------------------------------------------------

void
MarkSet::reset()
{
  _Marks.vider() ;
}

MarkSet&
MarkSet::operator=(const MarkSet& src)
{
  if (&src == this)
		return *this ;

  _Marks = src._Marks ;

  return *this ;
}

//------------------------------------------------------------------------------
/*
** MarkSet Class
** Set the value at the element which has the key key
** Condition :  The element of Marker object must
**              have the same dynamic type of the
**              element which has the key key
**              The Element must exit.
*/
void
MarkSet::set(std::string& key, MarkerObject* temp)
{
	if (_Marks.empty())
  {
    delete temp ;
    return ;
  }

  for (std::vector<Valeur*>::iterator iter = _Marks.begin() ; _Marks.end() != iter ; iter++)
    if ((*iter)->Key() == key)
    {
      (*iter)->putValue(temp) ;
      return ;
    }

  delete temp ;
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/*
** MarkSet Class
** Say if the element which has key id is present ?
** The function return True if it is the case
** and else False
*/
inline
bool MarkSet::Exist(std::string& sKey)
{
	return _Marks.Exist(sKey) ;
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/*
** MarkSet Class
** insert a new bucket if the key keys don't exist
** Else the function do nothing
*/
void MarkSet::insert(std::string& keys, MarkerObject* val)
{
  if ((MarkerObject*) NULL == val)
    return ;

  if (Exist(keys))
  {
    delete val ;
    return ;
  }

  Valeur* tempo = new Valeur(keys, val) ;
  _Marks.push_back(tempo) ;
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/*
** MarkSet Class
** Return the marks at the i element
*/
Valeur*
MarkSet::operator[] (unsigned int i) const
{
  if (i < _Marks.size())
    return (_Marks[i]) ;

  return ((Valeur*) 0) ;
}
//------------------------------------------------------------------------------

void
MarkSet::Remove(std::string& sKey)
{
  _Marks.Remove(sKey) ;
}

bool
MarkSet::hasStringAttributes(std::string sName, std::string sVal)
{
  return _Marks.hasStringAttributes(sName, sVal) ;
}

bool
MarkSet::hasSubStringAttributes(std::string sName, std::string sVal)
{
	return _Marks.hasSubStringAttributes(sName, sVal) ;
}

//------------------------------------------------------------------------------
/*
** MarkSet Class
** return the value of the key key_to_find
*/
MarkerObject*
MarkSet::get(std::string& key_to_find)
{
  return _Marks.get(key_to_find) ;
}
//------------------------------------------------------------------------------

std::ostream&	operator<< (std::ostream& ostr, const MarkSet& m)
{
  ostr << "SET MARK{" << std::endl ;
  ostr << "Taille :" << m.DecorateurSize() << std::endl ;
  for (int i = 0; i < m.DecorateurSize(); i++)
  {
    ostr << "Index[" << i << "] (" << std::endl ;
    ostr <<  "  key: " << (m[i])->Key() << std::endl ;
    ostr <<  "  " << (m[i])->Mark() ;
    ostr <<  "  )" << std::endl << std::endl ;
  }

  ostr << "}" ;
  return (ostr) ;
}

