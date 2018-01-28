// -----------------------------------------------------------------------------
// csvParser.h
// -----------------------------------------------------------------------------
// Parseur de fichier CSV
// CSV file parser
// -----------------------------------------------------------------------------
// $Revision: 1.4 $
// $Author: pameline $
// $Date: 2010/02/16 14:01:18 $
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


#ifndef __CSVPARSER_H__
# define __CSVPARSER_H__

# include "nsutil/attvalelem.h"
# include "partage/NTArray.h"


//! \brief CSVRecord is a simple class that has an Attribute-Value list.
//!        This class was developped for integration of Nautilus software in Amiens' CHU.
//!
//! This class is used by CSVparser, to store a record from a CSV-formatted file.
class CSVRecord
{
 public:
 	//! default constructor.
  CSVRecord() { pAVList = new AttributeValueList() ; }

  //! constructor from an attribute-value list.
  CSVRecord(const AttributeValueList& src) ;

  //! copy constructor.
  CSVRecord(const CSVRecord& src) ;

  //! destructor
  ~CSVRecord() ;

  //! operator=
  CSVRecord& operator=(const CSVRecord& src) ;

  //! operator==
  int operator==(const CSVRecord& src) ;

  //! get the object's value which has attribute
  string getValWithAttr(const string attr)  { return (pAVList->getAVwithAttr(attr)->getValue()) ; }

 protected:
  AttributeValueList  *pAVList ;                         //!< the Attribute-Value list.
} ;


typedef vector<CSVRecord *>                CSVRecordVector ;
typedef CSVRecordVector::iterator          CSVRecordIter ;
typedef CSVRecordVector::reverse_iterator  CSVRecordRIter ;
typedef NTArray<CSVRecord>                 CSVRecordArray ;


//! \brief CSVStructure is a class that describes a structure.
//!        This class was developped for integration of Nautilus software in Amiens' CHU.
class CSVStructure
{
 public:
  //! \brief default constructor.
  //! \param code is the code affected to this structure.
  //! \param label is the structure's label.
  //! \param linkAt is a pointer to the father structure.
  //! \param type is the Functionnal Unit type.
	CSVStructure(string code, string label, CSVStructure *linkAt, string type = "") ;

  //! copy constructor
  CSVStructure(const CSVStructure& src) ;

  //! destructor
  ~CSVStructure() ;

  //! operator=
  CSVStructure& operator=(const CSVStructure& src) ;

  //! operator==
  int operator==(const CSVStructure& src) ;

  //! \brief  method that create the object in database that correspods to the
  //!         structure.
  //! \param  pCtx is Nautilus' context.
  //! \param  sOType is the structure type.
  //! \return if the creation is succesfull : true, else : false.
  bool createObjectPPT(NSSuper* pSuper, string sUserId, string sOType) ;

 protected:
 	string							sObjectID ;             //!< object ID from database.
  string              sCode ;                 //!< code affected to the structure from CSV-formatted file.
  string              sLabel ;                //!< label of the structure from CSV-formatted file.
  string              sFUType ;               //!< Functionnal Unit type of the structure from CSV-formatted file. available only for Functional Unit else equal to "".
  CSVStructure        *pLinkAt ;              //!< link to father structure.
} ;


typedef vector<CSVStructure *>                CSVStructureVector ;
typedef CSVStructureVector::iterator          CSVStructureIter ;
typedef CSVStructureVector::const_iterator    CSVStructureCIter ;
typedef CSVStructureVector::reverse_iterator  CSVStructureRIter ;


//! \brief list of structures.
//!        This class was developped for integration of Nautilus software in Amiens' CHU.
class CSVStructureArray : public CSVStructureVector
{
 public:
  //! default constructor
 	CSVStructureArray() ;

  //! copy constructor
  CSVStructureArray(const CSVStructureArray& src) ;

  //! destructor
  ~CSVStructureArray() ;

  //! \brief  get a structure from parameters.
  //! \param  sCode code affected to the structure
  //! \param  sLabel label of the structure
  //! \param  pLinkAt father structure
  //! \param  sType structure's type
  //! \return If the structure is found, the structure, else NULL.
	CSVStructure * searchStructure(string sCode, string sLabel, CSVStructure *pLinkAt = NULL, string sType = "") ;

  //! \brief  adds a structure, if it is not already in the list.
  //! \param  sCode code affected to the structure
  //! \param  sLabel label of the structure
  //! \param  pLinkAt father structure
  //! \param  sType structure's type
  //! \return If the structure was before in the list, return its, else if it has been added, return its, else NULL.
  CSVStructure * addStructure(string sCode, string sLabel, CSVStructure *plinkAt = NULL, string sType = "") ;

  //! \brief  create all objects which are in the list.
  //!
  //! You have to be careffuly by using this function while all objects are created with the same type.
  //! \param  pCtx is Nautilus' context
  //! \param  sOType is the structure type of all objects.
  //! \return if the operation is successfull true, else false.
	bool createObjects(NSSuper* pSuper, string sUserId, string sOType) ;

 protected:
  //! \brief  erase all structures in the list.
  //! \return true if all is ok, else false.
	bool vider() ;
} ;

//! \brief Object manager for pilot with specific functions for structures.
//!        This class was developped for integration of Nautilus software in Amiens' CHU.
class _CLASSUTIL NSStructureGraphManager : public NSObjectGraphManager
{
 public:
 	//! \brief default constructor
  //! \param pCtx is the Nautilus' context
 	NSStructureGraphManager(NSSuper* pSuper) : NSObjectGraphManager(pSuper) {}

  //! copy constructor
  NSStructureGraphManager(NSTeamGraphManager& rv) : NSObjectGraphManager(rv) {}

  //! destructor
	~NSStructureGraphManager() {}

  //! \brief  get Structure from an object ID.
  //! \param  sObjID is the object ID.
  //! \param  pPPT is the PatPathoArray object that describes the object.
  //! \return if the structure is found true, else false.
  bool getStructureGraph(string sObjID, NSPatPathoArray *pPPT = 0) ;

  //! \brief  save structure in the database.
  //! \param  pPPT is the PatPathoArray object that describes the object.
  //! \param  sOType is the structure type.
  //! \param  sFatherID is the ID of father structure.
  //! \return if the structure is found true, else false.
  bool setStructureGraph(string sUserId, NSPatPathoArray *pPPT, string sOType, string sFatherID) ;

  //! \brief  get Structure name from a PatPathoArray that describes the object.
  //! \param  pPPT is the PatPathoArray object that describes the object.
  //! \return the object name if it exists else "".
  string getStructureName(NSPatPathoArray *pPPT) ;

  //! \brief  get Structure ID from a PatPathoArray that describes the object.
  //! \param  pPPT is the PatPathoArray object that describes the object.
  //! \return the object ID if it exists else "".
  string getStructureID(NSPatPathoArray *pPPT) ;
} ;

//! \brief  CSVParser is the class that manages the CSV-formatted file
//!         importation that defines structures.
//!         This class was developped for integration of Nautilus software in Amiens' CHU.
class _CLASSUTIL CSVParser
{
 public:
 	//! \brief default constructor.
  //! \param file is the file where datas to import are.
  //! \param cField is the character that separates two fields.
  //! \param cItem is the character that separates two items.
  CSVParser(string file, char cField, char cItem) ;

  //! destructor
  ~CSVParser() ;

  //! \brief  import datas from file.
  //! \return true if it is ok, else false.
  bool importFromFile() ;

  //! \brief  run the creation of objects that are defined in a CSV-formatted file.
  //! \return true if it is ok, else false.
  bool run(NSSuper* pSuper, string sUserId) ;

 protected:
  string          sFile ;             //!< CSV-formatted file to import datas.
  char            cFieldSeparator ;   //!< character that separates two fields.
  char            cItemSeparator ;    //!< character that separates two items.

  CSVRecordArray  *csvRecords ;       //!< list of records imported from CSV-formatted file.
} ;

#endif // !__CSVPARSER_H__

