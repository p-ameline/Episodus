// --------------------------------------------------------------------------// nscapmodels.h
// --------------------------------------------------------------------------
// Document/Vues Capture Données Utilisateur
// --------------------------------------------------------------------------
// $Revision: 1.3 $
// $Author: fabrice $
// $Date: 2005/05/24 17:41:04 $
// -----------------------------------------------------------------------------
// FLP - Aout 2001
// --------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Copyright Nautilus, 2005
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


#ifndef __NSCAPMODELS_H__
# define __NSCAPMODELS_H__

# include <owl\dialog.h>
# include <vector>
# include <string>

//#include "partage\nsglobal.h"
//#include "partage\NTArray.h"
# include "ns_capture\nscaptexport.h"
# include "ns_capture\nscapelements.h"

// -----------------------------------------------------------------------------
//
// Classe EpisodusElemData
//
// -----------------------------------------------------------------------------

//class ChildEpisodusData
class _NSCAPTURECLASSE EpisodusElemData
{
 public:
  unsigned long   AppWindow ;
  unsigned long   AppWndChild ;
  string          sClassChild ;
  string          sTextChild ;
  string          sCorrespondence ;
  string          sClassifier ;
  string          sFormat ;
  string          sUnit ;
  ClassLib::TRect Placement ;

  enum TYPECAPTURE    { captureSystem = 1, captureOCR } ;  enum TYPEANALYSIS   { analyseDefault = 1, analysePlacement, analyseOrder, analysePlacementAndOrder, analyseLibelle, analyseRelativeOrder, analyseRelativePlacement, analyseNever } ;  int	            iModeCapture ;  int	            iModeAnalyse ;  int             iOrder ;  // Constructeurs - Destructeur
  EpisodusElemData();
  EpisodusElemData(const EpisodusElemData& rv) ;
  EpisodusElemData(EpisodusElemData *pNewEpisodusElemData) ;
  EpisodusElemData(string sXMLBaliseContenu, int iGivenOrder) ;
  EpisodusElemData(THandle aWindow, THandle aWndChild, string aClassChild, string aTextChild, ClassLib::TRect aPlacement, int iOrd, string aCorrespondence = "", int iModCapt = captureSystem, int iModAnalys = analyseDefault, string sClassifier = "", string sFormatage = "", string sUnite = "") ;
  ~EpisodusElemData() ;

  bool    ImportModelXML(string sXMLBaliseContenu) ;

  // fonction de comparaison
  bool    CompareWithElementModel(EpisodusElemData *, int iDefaultAnalyse)  ;
  bool    CompareWithRelativeElementModel(EpisodusElemData * pModel, EpisodusElemData * pRepereCapture, EpisodusElemData * pRepereModel) ;
  EpisodusElemData& operator=(const EpisodusElemData& src) ;
} ;


// définition d'un array représentant la fenêtre de capture
typedef vector<EpisodusElemData *>        ArrayEpisodusData ;
typedef ArrayEpisodusData::iterator       ArrayEpisodusDataIter ;
typedef ArrayEpisodusData::const_iterator ArrayEpisodusCIter ;


class _NSCAPTURECLASSE EpisodusData : public ArrayEpisodusData
{
 public:
  enum TYPEANALYSE { byPlacement = 1, byOrder, byPlacementAndOrder } ;
  int     iModeAnalyse ;

  // Constructeurs - Destructeur
  EpisodusData() ;
  EpisodusData(const EpisodusData& rv) ;
  EpisodusData(EpisodusData *pNewEpisodusData) ;
  EpisodusData(string sXMLBaliseContenu) ;
  ~EpisodusData() ;

  void    Reorder() ;

  // Fonctions de Correspondance  bool    SetElement(size_t index, string sLabel) ;  bool    SetClassifElement(size_t index, string sLabel) ;
  bool    SetFormatElement(size_t index, string sLabel) ;
  bool    SetUnitElement(size_t index, string sLabel) ;
  bool    SetAnalyseTypeElement(size_t index, int iAnalyse) ;

  EpisodusElemData * getElement(size_t index) ;

  // fonction d'importation exportation
  bool    ExportModel(ofstream * pOutFile) ;
  bool    ImportModel() ;
  bool    ImportModel(string sXMLBaliseContenu) ;
  bool    Model2View(EpisodusData * pEpisodDataView) ;

  bool    SaveModel() ;

  // fonctions de comparaison
  bool    CompareWithModel(EpisodusData *, bool bExact) ;
  bool    captureSubModel(EpisodusData * pCompareData, NSCaptureArray * pCaptureArray) ;

  void    vider();

  EpisodusData& operator=(const EpisodusData& src) ;
} ;


typedef EpisodusData::iterator        EpisodusDataIter ;
typedef EpisodusData::const_iterator  EpisodusDataCIter ;


class _NSCAPTURECLASSE EpisodusModel : public EpisodusData
{
 public:
  string      sNom ;
  bool        bModified ;
  EpisodusModel() ;  EpisodusModel(const EpisodusModel& rv) ;  EpisodusModel(EpisodusData *pNewEpisodusData) : EpisodusData(pNewEpisodusData) {}
  EpisodusModel(string sXMLBaliseContenu, string sXMLBaliseVariables) ;
  ~EpisodusModel() ;

  void donneCaptureElemts(NSCaptureArray * pCaptArray, EpisodusData * pData) ;
  bool ExportModel(ofstream * pOutFile, string sType) ;
  bool useOCR() ;

  EpisodusModel& operator=(const EpisodusModel& src) ;
} ;


// définition d'un array de modèles
typedef vector<EpisodusModel *>                   ArrayEpisodusModelesData ;
typedef ArrayEpisodusModelesData::iterator        ArrayEpisodusModelIter ;
typedef ArrayEpisodusModelesData::const_iterator  ArrayEpisodusModelCIter ;


class _NSCAPTURECLASSE ArrayEpisodusModelData : public ArrayEpisodusModelesData
{
 public:
  // Constructeurs - Destructeur
  ArrayEpisodusModelData() : ArrayEpisodusModelesData() {}
  ArrayEpisodusModelData(const ArrayEpisodusModelData& rv) ;
  ~ArrayEpisodusModelData() ;

  EpisodusModel * trouveModele(EpisodusData * pEpisodusData) ;
  bool            captureSubModels(EpisodusData * pCapturedData, NSCaptureArray * pCaptureArray) ;

  bool    sauvegarder(ofstream * pOutFile, string sType) ;
  void    vider() ;  ArrayEpisodusModelData& operator=(const ArrayEpisodusModelData& src) ;
} ;


class EpisodusModelXML
{
 public:
  EpisodusModelXML() {}
  ~EpisodusModelXML() {}
  bool ImportModelXML(string input, size_t pos = 0) ;

  vector<string>  sArrayBalise ;
  vector<string>  sArrayBaliseVariables ;
  vector<string>  sArrayBaliseContenu ;
} ;

#endif // !__NSCAPMODELS_H__

