// -----------------------------------------------------------------------------// nscaptengine.cpp
// -----------------------------------------------------------------------------
// Document/Vues Capture Données Utilisateur
// -----------------------------------------------------------------------------
// $Revision: 1.4 $
// $Author: fabrice $
// $Date: 2005/06/01 18:14:12 $
// -----------------------------------------------------------------------------
// FLP - 08/2001
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------// Copyright Nautilus, 2005
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


#ifndef __NSCAPTENGINE_H__
# define __NSCAPTENGINE_H__

# include <owl\dialog.h>
# include <vector>
# include <string>

//# include "partage\nsglobal.h"
//# include "partage\NTArray.h"
# include "ns_capture\nscaptexport.h"
# include "ns_capture\nscapmodels.h"
# include "ns_capture\nscapelements.h"

// -----------------------------------------------------------------------------
//
// Classe NSCaptureEngine
// Moteur de capture de données écran
// Screen data capture engine
//
// -----------------------------------------------------------------------------
class _NSCAPTURECLASSE NSCaptureEngine
{
 public:
  enum CAPTURE_RESULT { captureOk = 1, modelNotFound, captureFailed, captureCancelled } ;

  EpisodusData *            pEpisodusData ;                                     //! pointer on Episodus captured datas


  // tableau de modèles
  ArrayEpisodusModelData *  pModels ;                                           //! pointer on models list
  ArrayEpisodusModelData *  pSubModels ;                                        //! pointer on partial models list
  EpisodusModel *           pCurrentModel ;                                     //! pointer on current model

  NSCaptureArray            bufferCaptureArray ;

  // Constructeur - Destructeur
  NSCaptureEngine() ;
  ~NSCaptureEngine() ;

  // fonctions de capture
  CAPTURE_RESULT            Capture(HWND HWnd) ;                                // fonction de capture standard
  void                      CaptureBabylon(HWND HWnd) ;                         // fonction de capture du texte sélectionné
  void                      OCRcapture() ;
  void                      ScreenCapture(HWND HWnd) ;                          // fonction de capture d'écran
  // void                     CaptureOld(HWND HWnd) ;
  // bool                     CaptureHookOK(HWND HWnd) ;                        // fonction permettant de savoir si une capture de données est possible

  void                      getCapturedData(NSCaptureArray * pCaptured) ;
  EpisodusModel *           getCurrentModel() { return pCurrentModel ; }        // renvoie un pointeur sur le modèle courant
  void                      clearCurrentModel() { delete pCurrentModel ; pCurrentModel = 0 ; }

  void    creeCapture(EpisodusElemData * pCapture, EpisodusElemData * pModel, NSCaptureArray * pCaptArray) ;

  HWND    hOldWindowCaptured ;

  // récupération de la position de la souris
  POINT * getPMousePoint() { return &MousePoint ; }

  // fonction d'importation - exportation de modèles
  bool    ImportModels(string sFichier, bool verbose = true) ;
  bool    ExportModels(string sFichier) ;

  void    chargerPrincipes() ;

  // fonctions d'énumération
  static bool FAR PASCAL _export NSCaptureEngine::SearchChildWindow(HWND hWnd, LPARAM lParam) ;
  static bool FAR PASCAL _export NSCaptureEngine::SearchChildWindowRecur(HWND hWnd, LPARAM lParam) ;

  void      resetCaptureArray()   { bufferCaptureArray.vider() ; }

  NSBitmap * getScreenMap()       { return &screenMap ; }

  // void      formatElement(string *sText, string sFormat, bool bForced = true) ;
  void      preCompare(string * psText, string * psModel) ;
  bool      smartCompare(string * psText, string * psModel) ;

  // Accessors

  EpisodusData::TYPEANALYSE  getDefaultAnalyse()         { return iDefaultAnalyse ; }
  void setDefaultAnalyse(EpisodusData::TYPEANALYSE iDef) { iDefaultAnalyse = iDef ; }

 protected:
  POINT       MousePoint ;                                                      //! mouse screen position
  NSBitmap    screenMap ;

  EpisodusData::TYPEANALYSE iDefaultAnalyse ;
} ;


// -----------------------------------------------------------------------------
//
// Classe MylParam
// Sert pour l'envoi à la fonction d'énumération
//
// -----------------------------------------------------------------------------

class MylParam
{
 public:
  THandle          AppWindow ;
  THandle          theMainWindow ;
  NSCaptureEngine * pEngine ;

  // Constructeurs - Destructeur
  MylParam() ;
  MylParam(THandle hWnd, THandle mainWindow, NSCaptureEngine * pEng)  { AppWindow = hWnd ; theMainWindow = mainWindow ; pEngine = pEng ; }
  ~MylParam() { ; }
} ;

#endif // !__NSCAPTENGINE_H__
