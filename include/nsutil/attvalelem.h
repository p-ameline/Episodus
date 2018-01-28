// -----------------------------------------------------------------------------
// attvalelem.h
// -----------------------------------------------------------------------------
// Objet Attribut Valeur avec Fonctionnalités
// -----------------------------------------------------------------------------
// $Revision: 1.2 $
// $Author: pameline $
// $Date: 2009/07/23 17:08:10 $
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


#ifndef __ATTVALELEM_H__
# define __ATTVALELEM_H__


# ifdef _NSUTILDLL
#  define _CLASSUTIL __export
# else
#  define _CLASSUTIL __import
# endif // _NSUTILDLL

# include "partage\ns_vector.h"

// # include <vector>
// # include <string>
// using namespace std ;


// -----------------------------------------------------------------------------
// Important note about Doxygen tag
// -----------------------------------------------------------------------------
// All comments begining with "//!" or "//!<" must not be deleted, they concerns
// Doxygen doc generator.
// If you modify some classes and/or methods and/or variables please modify also
// the doxygen available informations.
// -----------------------------------------------------------------------------


//! \brief AttributeValueElem is a simple class that has a value and an
//!        attribute.
//!        This class was developped for integration of Nautilus software in Amiens' CHU.
class	_CLASSUTIL AttributeValueElem
{
 public:
  //! \brief default constructor.
  //! \param attr is the attribute.
  //! \param val is the value.
 	AttributeValueElem(string attr, string val)   { sAttribute = attr ; sValue = val ; }

  //! destructor
  ~AttributeValueElem()                         {}

  //! operator=
	AttributeValueElem& operator=(const AttributeValueElem& src)  { sAttribute = src.sAttribute ; sValue = src.sValue ; return (*this) ; }

  //! operator==
  int operator==(const AttributeValueElem& src) { if ((sAttribute == src.sAttribute) && (sValue == src.sValue)) return 1 ; return 0 ; }

  //! get the object's attribute
  const string getAttribute()            				{ return sAttribute ; }

	//! get the object's value
  const string getValue()                				{ return sValue ; }

 protected:
  string              sAttribute ;              //!< object's attribute
  string              sValue ;                  //!< object's value
} ;


typedef vector<AttributeValueElem *>            AttributeValueVector ;
typedef AttributeValueVector::iterator          AttributeValueIter ;
typedef AttributeValueVector::const_iterator    AttributeValueCIter ;
typedef AttributeValueVector::reverse_iterator  AttributeValueRIter ;


//! \brief AttributeValueList is a simple class that is a list of
//!        AttributeValueElem with some functionnalities.
//!        This class was developped for integration of Nautilus software in Amiens' CHU.
class _CLASSUTIL AttributeValueList : public AttributeValueVector
{
 public:
 	//! default constructor
 	AttributeValueList() ;

  //! copy constructor
  AttributeValueList(const AttributeValueList& src) ;

  //! destructor
	~AttributeValueList() ;

  //! operator=
  AttributeValueList& operator=(const AttributeValueList& src) ;

  //! operator==
  int operator==(const AttributeValueList& src) ;

  //! \brief  Adds an elem in the list. This method also check if the elem's
  //!         attribute does not exist in the list before adding.
  //! \param  attr is the new elem attribute.
  //! \param  val is the new elem value.
  //! \return if the new elem has been added : true, else : false.
  bool addElem(string attr, string val) ;

  //! \brief  Check if an attribute exist in the list.
  //! \param  attr is the attribute.
  //! \return if the attribute is present : true, else : false.
  bool isAttrPresent(string attr) ;

  //! \brief  Get the elem that has an attribute.
  //! \param  attr is the attribute.
  //! \return elem which has the attribute.
  AttributeValueElem * getAVwithAttr(const string attr) const ;

 private:
 	//! clear all contents of the list.
  void clearContents() ;
} ;

#endif // !__ATTVALELEM_H__
