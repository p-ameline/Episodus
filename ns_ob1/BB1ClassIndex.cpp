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
#include "ns_ob1\OB1ClassIndex.h"

BB1ClassIndex::BB1ClassIndex()
{
  _max = 1 ;
}

BB1ClassIndex::~BB1ClassIndex()
{
}

void BB1ClassIndex::insertItem(std::string& key)
{
  _index[key] = _max ;
  _max *= 2 ;
}

int
BB1ClassIndex::operator[] (std::string& clas) const
{
  std::map<std::string, int>::const_iterator ite = _index.find(clas) ;
  if (_index.end() != ite)
    return (*ite).second ;
  return -42 ;
}

std::string BB1ClassIndex::toHTML()
{
  std::string temp = "[Dictionnaire des classes <br /><ul>\n" ;

  std::map<std::string, int>::iterator ite ;
  if (false == _index.empty())
    for (ite = _index.begin() ; _index.end() != ite ; ite++)
    {
      temp += "<li>(" + (*ite).first +"," ;
      char ty[10] ;
      itoa((*ite).second , ty, 10) ;
      temp.append(ty) ;
      temp += ")</li>" ;
    }
  temp += "</ul>" ;
  return temp ;
}

