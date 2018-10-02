// -----------------------------------------------------------------------------// nsepicap.h
// -----------------------------------------------------------------------------
// Document/Vues Capture Données Utilisateur
// -----------------------------------------------------------------------------
// $Revision: 1.17 $
// $Author: pameline $
// $Date: 2015/12/14 08:20:01 $
// -----------------------------------------------------------------------------
// FLP - 08/2001
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

#ifndef __NSEPICAP_H__
# define __NSEPICAP_H__

class NSEpisodView ;
class NSSOAPView ;
class NSSOAPTankView ;
class classifExpert ;
class SOAPBasketArray ;
class PrinciplesArray ;
class SOAPObject ;
class NSCaptDocument ;
class EpisodusData ;
class ArrayEpisodusModelData ;
class EpisodusModel ;
class NSLogChild ;
class NSLogWindow ;
class NSUtilisateurChoisi ;
class NSPrediDlg ;

#ifndef __EPIBRAIN__
# include <owl\dialog.h>
# include <owl\groupbox.h>
# include <owl\button.h>
#endif // !__EPIBRAIN__

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsglobalLus.h"
#else
  #include "partage/nsglobal.h"
#endif

# include "nautilus\nssuper.h"
      // # include "partage\NTArray.h"
# include "nssavoir\nscaptur.h"
# include "nsldv\nsldvuti.h"
# include "nsepisod\nsPapyrus.h"
# include "nsepisod\nsPrediDlg.h"
      // # include "nautilus\nsbrowse.h"
      // # include "nautilus\nscaptview.h"
# include "nsbb\tagnames.h"
      // #include "nsepisod\nssoapdiv.h"

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsdivfctForCgi.h"
#endif

# ifdef _EXT_CAPTURE
#  include "ns_capture\nscaptengine.h"
# endif // _EXT_CAPTURE

# ifndef _EXT_CAPTURE

// -----------------------------------------------------------------------------
//
// Classe MylParam
// Sert pour l'envoi à la fonction d'énumération
//
// -----------------------------------------------------------------------------

class MylParam
{
  public:

    TWindow::THandle AppWindow ;
    TWindow::THandle theMainWindow ;
    NSContexte       *pContexte ;

    // Constructeurs - Destructeur
    MylParam() ;
    MylParam(TWindow::THandle hWnd, TWindow::THandle mainWindow, NSContexte * pCtx) { AppWindow = hWnd ; theMainWindow = mainWindow ; pContexte = pCtx ; }
    ~MylParam() {}
} ;

# endif // !_EXT_CAPTURE

// -----------------------------------------------------------------------------//// Classe NSEpisodus
// Classe de gestion des fonctions Episodus
//
// -----------------------------------------------------------------------------
#ifndef _ENTERPRISE_DLL
class NSEpisodus : public NSRoot
{
  protected:

    // objets de capture
    NSCaptDocument*   pCaptureDoc ;         //!

 public:

# ifndef _EXT_CAPTURE
    EpisodusData *    pEpisodusData ;               //! list of catured datas
    int               iDefaultAnalyse ;             //!
# else
    NSCaptureEngine * pCaptureEngine ;              //! pointer on capture engine
# endif // _EXT_CAPTURE

    NSEpisodView *    pViewData ;                   //! pointer on episodus datas view

    NSSOAPView *      pSOAPView ;                   //! pointer on SOAP datas view
    NSSOAPTankView *  pSOAPTank ;                   //! pointer on SOAP tank datas view
    PrinciplesArray * pPrincipes ;                  //!
    SOAPBasketArray * pBasketList ;                 //!

    bool              bAutoSave ;                   //!
    string            sPosSOAP ;                    //!
    string            sPosTank ;                    //!
    string            sPosCapture ;                 //!

    string            sPosLdV ;                     //!
    string            sOpenModeLdV ;                //!
    bool              bAutoOpenLdV ;                //!
    bool              bAllowFreeTextLdV ;           //!

    //! enumeration of Classification process states (none, soaping, episodes, message, finished)
    enum EPISODUSSTATE { state0 = 1, stateSOAPing, stateEpisod, stateMessage, stateFinished } ;

    int               iEpisoState ;                 //! state of classfication process -- must be an EPISODUSSTATE !!

    // tableau de modèles
    ArrayEpisodusModelData *  pModels ;             //! pointer on models list
    ArrayEpisodusModelData *  pSubModels ;          //! pointer on partial models list

# ifndef _EXT_CAPTURE
    EpisodusModel *   pCurrentModel ;       //! pointer on current model
# endif // !_EXT_CAPTURE

    // objets de capture
    NSCaptureArray    CaptureArray ;        //!
    NSCaptureArray    newCaptureArray ;     //!

    classifExpert *   pClassifExpert ;      //!

    // Paramètres spécifiques de l'utilisateur --  User parameters
    bool                      bActiveBabylon ;      //! does babylon capture functionalities active ?
    bool                      bActiveGlobal ;       //! does global capture fuonctionalities active ?

    bool                      bAutoFlocule ;        //!
    bool                      bSendNow ;            //!    string                    sAsymetricHash ;      //!
    string                    sPromethePrincipe ;   //!
    string                    sPrometheMail ;       //! Prometheus Mail
    string                    sPrometheUserId ;     //! Prometheus ID
    string                    sPrometheURL ;        //! Prometheus URL
    string                    sPrometheUserLogin ;  //! Prometheus Login
    string                    sPrometheUserPaswd ;  //! Prometheus Password
    string                    sBabylonKey ;         //! babylon capture key
    string                    sCaptureKey ;         //! capture key
    string                    sServiceUrl ;
    string                    sServiceTitle ;

    int                       _iAutoSearchDelay ;
    int                       _iAutoSearchMinChar ;

    // Constructeur - Destructeur
    NSEpisodus(NSContexte * pCtx) ;
    ~NSEpisodus() ;

    // fonctions de capture
    void    Capture(HWND HWnd) ;             // fonction de capture standard
    bool    captureSubModels(EpisodusData* pCapturedData, NSCaptureArray * pCaptureArray) ;
    bool    captureSubModel(EpisodusData* pCapturedData, EpisodusData* pCompareData, NSCaptureArray* pCaptureArray) ;
    void    donneCaptureElemts(NSCaptureArray* pCaptArray, EpisodusData* pData) ;
    void    CaptureBabylon(HWND HWnd) ;      // fonction de capture du texte sélectionné
    void    OCRcapture() ;
    void    ScreenCapture(HWND HWnd) ;       // fonction de capture d'écran
    void    CaptureOld(HWND HWnd) ;
    bool    CaptureHookOK(HWND HWnd) ;       // fonction permettant de savoir si une capture de données est possible
    void    PutNewCaptureInTank(bool bEmptyTankFirst = false, bool bResetNew = true) ;
    void    EmptyCaptureTank() ;

    void    creeCapture(EpisodusElemData * pCapture, EpisodusElemData * pModel, NSCaptureArray * pCaptArray) ;

    HWND    hOldWindowCaptured ;                    //!

    // Hook de la souris
    void    lanceHook() ;        // lancement du hook
    void    fermeHook() ;        // arrêt du hook

    // récupération de la position de la souris
    POINT * getPMousePoint() { return &MousePoint ; }

    // taches à effectuer en cas de changement d'utilisateur
    void    UserChanged() ;

    // taches à effectuer en cas de changement de patient
    void    PatChanged() ;

    // fonction d'importation - exportation de modèles
    bool    ImportModels(bool verbose = true) ;
    bool    ExportModels() ;

    bool    loadParams() ;
    bool    saveParams() ;
    bool    saveParams(string sUserId) ;

    bool    loadEpiParams() ;
    string  getParam(string sChapter, string sCode) ;

    void    chargerPrincipes() ;

    // fonction de log
    void    logSpyMessage(string sSpyMessage) ;

# ifndef _EXT_CAPTURE
    // fonctions d'énumération
    static bool FAR PASCAL _export NSEpisodus::SearchChildWindow(HWND hWnd, LPARAM lParam) ;
    static bool FAR PASCAL _export NSEpisodus::SearchChildWindowRecur(HWND hWnd, LPARAM lParam) ;
# endif // !_EXT_CAPTURE

    void createViewData(EpisodusModel * pModel) ;
    void createSOAPView() ;                           // Vue SOAP
    void addToSOAP(NSCapture * pNewCap) ;
    void addToSOAP(SOAPObject * pNewObj) ;
    void endingSOAP() ;

    // Fonctions liées à Prometheus
    // Prometheus related methods
    void appendPrometheFile() ;
    bool sendPrometheMessage() ;

    void reveilleToi() ;

    // fonctions pour l'état du module Episodus
    bool    GetStateOn()                            { return bStateOn ; }
    void    SetStateOn(bool bStateChange)           { bStateOn = bStateChange ; }

    int     GetEpisoState()                         { return iEpisoState ; }
    void    SetEpisoState(int iNewState)            { iEpisoState = iNewState ; }

    // fonctions controllant l'état de la capture à partir du hook de la souris
    bool    GetMouseHookState()                     { return bMouseHookState ; }
    void    SetMouseHookState(bool bStateChange)    { bMouseHookState = bStateChange ; }

    // fonctions
    bool    GetModelFound()                         { return bModelFound ; }
    void    SetModelFound(bool bStateChange)        { bModelFound = bStateChange ; }

    bool    GetLearning()                           { return bLearning ; }
    void    SetLearning(bool bLearn)                { bLearning = bLearn ; }

    void    SetPhareOn() ;
    void    SetPhareOff() ;

    bool    addVirtualDrug()                        { return _bDrugAddVirtualDrug ; }

# ifndef _EXT_CAPTURE
    void    resetCaptureArray()                     { CaptureArray.vider() ; }
# else
    void    resetCaptureArray()                     { pCaptureEngine->resetCaptureArray() ; }
# endif // _EXT_CAPTURE

    NSCaptDocument* getCaptureDoc()                 { return pCaptureDoc ; }
    void            createCaptureDoc() ;
    void            deleteCaptureDoc() ;

    NSBitmap *  getScreenMap()                      { return &screenMap ; }

    // void    formatElement(string * sText, string sFormat, bool bForced = true) ;
    // void    preCompare(string * psText, string * psModel) ;
    // bool    smartCompare(string * psText, string * psModel) ;

  protected:

    int      CM_MOUSE_HOOK ;     //!
    POINT    MousePoint ;        //! mouse "on screen" position
    bool     bStateOn ;          //! is Episodus component active ?
    bool     bMouseHookState ;   //! état du hook
    bool     bModelFound ;       //! was a model be found ?
    bool     bLearning ;         //! is learning mode active ?
    NSBitmap screenMap ;         //!

    bool     _bDrugAddVirtualDrug ;

    NSLocalChapterArray aEpiParams ;
} ;

inline string
NSEpisodus::getParam(string sChapter, string sCode)
{
  return aEpiParams.getLocalText(sChapter, sCode, true) ;
}

inline void
NSEpisodus::EmptyCaptureTank()
{
#ifndef __EPIBRAIN__
  CaptureArray.vider() ;
#endif
}
#endif

#ifndef __EPIBRAIN__
// -----------------------------------------------------------------------------
//
// Classe NSPredi
// Classe de gestion des fonctions PréDI
//
// -----------------------------------------------------------------------------
class NSPredi : public NSRoot
{
  public:

    bool            _bPrediActif ;
    bool            _bMinimalInterface ;
    bool            _bReturnCloses ;

    bool            _bRVDBactif ;

    // ajout du parametre gérant le precochage dans DPI0 -- fab BBpowered
    bool						_bPrediPreCoche ;

    // Etat du processus PréDI (rien, questionnaires, message, terminé)
    enum PREDISTATE { state0 = 1, stateQuestions, stateMessage, stateFinished } ;

    PREDISTATE      _iPrediState ;

    NSLogChild *    _pLogMDIWnd ;
    NSLogWindow *   _pLogWnd ;

    bool            _bSendPage ;
    string          _sSendPage ;
    string          _sMailAdress ;

    string          _sNomModule ;
    string          _sOEMModule ;
    string          _sOEMWeb ;

    int             _iPosX ;
    int             _iPosY ;
    int             _iDeltaPosX ;
    int             _iDeltaPosY ;

    int             _iCurrentX ;
    int             _iCurrentY ;

    NSPrediDlg*     _pPrediDlg ;

    // Constructeur - Destructeur
    NSPredi(NSContexte * pCtx) ;    ~NSPredi() ;

    // taches à effectuer en cas de changement d'utilisateur
    void   UserChanged() ;

    // taches à effectuer en cas de changement de patient
    void   PatOpening() ;
    void   PatChanged() ;
    void   ReinitLogFile() ;
    void   AppendLogFile() ;
    void   addToLogPage(string sAdd, string sBefore = string(""), string sAfter = string("")) ;

    void   LogDocument(NSDocumentInfo* pNSDocumentInfo, const NSPatPathoArray* pNSPatPathoArray, bool bIsNew) ;

    void   AppendSendFile() ;
    void   sendFile() ;
    bool   sendMessage() ;

    bool   loadParams() ;
    bool   saveParams() ;

    bool   referenceFormulaire(TWindow* pDialog, string sTitle) ;
    void   fermeFormulaire(void *pMDIpointer) ;

    bool   referenceDecisionTree(TWindow* pDialog, string sArchetype) ;
    void   fermeDecisionTree(void *pMDIpointer) ;

    TWindow* isDecisionTreeOpen(string sArchetype) ;

    bool   mustLogPage() { return _bLogPage ; }
    string getLogPage()  { return _Papyrus.getLogPage() ; }

  protected:

    bool            _bLogPage ;
    NSPapyrus       _Papyrus ;
    string          _logDocumentFileName ;

    ArrayPrediData  _aOpenDecisionTrees ;
} ;

# ifndef _EXT_CAPTURE
#  ifndef _ENTERPRISE_DLL
class NSModelDlg : public TDialog, public NSRoot
{
  public:

    OWL::TEdit *    pNewLib ;
    OWL::TButton *  pCreer ;

    OWL::TListBox * pLibelles ;
    OWL::TButton *  pModifier ;
    OWL::TButton *  pSupprimer ;

    OWL::TListBox * pSubLibelles ;
    OWL::TButton *  pSubSupprimer ;

    OWL::TButton *  pBtnImport ;

    EpisodusModel * pModel ;

    ArrayEpisodusModelData * pNewModels ;        // pointeur sur la liste de modèles
    ArrayEpisodusModelData * pNewSubModels ;     // pointeur sur la liste de modèles partiels

    NSModelDlg(TWindow * pere, NSContexte * pCtx, EpisodusModel * pModel) ;
    ~NSModelDlg() ;

    void CmOk() ;
    void CmCancel();
    void SetupWindow() ;

    void creer() ;
    void modifier() ;
    void supprimer() ;
    void sub_supprimer() ;
    void importer() ;

    void rempliListe() ;

    void importModel(ArrayEpisodusModelData * pModelImport) ;
    void importSubModels(ArrayEpisodusModelData * pModelImport) ;

  DECLARE_RESPONSE_TABLE(NSModelDlg) ;
} ;

class NSModelImportDlg : public TDialog, public NSRoot{
  public:

    OWL::TEdit *    pNewLib ;

    OWL::TListBox * pLibelles ;
    OWL::TListBox * pSubLibelles ;

    OWL::TButton *  pBtnImport ;
    OWL::TButton *  pBtnImportAll ;

    NSModelDlg *    pModelPere ;

    ArrayEpisodusModelData * pModels ;        // pointeur sur la liste de modèles
    ArrayEpisodusModelData * pSubModels ;     // pointeur sur la liste de modèles partiels

    string sImportedFile ;

    NSModelImportDlg(NSModelDlg * pere, NSContexte * pCtx, string sImportFile) ;
    ~NSModelImportDlg() ;

    void CmOk() ;
    // void CmCancel();
    void SetupWindow() ;

    void openFile() ;
    void rempliListe() ;

    void importer() ;
    void importAll() ;

  DECLARE_RESPONSE_TABLE(NSModelImportDlg) ;
} ;
#   endif  // !_ENTERPRISE_DLL
# endif // !_EXT_CAPTURE#endif // !__EPIBRAIN__# ifndef __EPIBRAIN__voidNSPredi::addToLogPage(string sAdd, string sBefore, string sAfter)
{
  if ((string("") == sAdd) && (string("") == sBefore) && (string("") == sAfter))
    return ;

  _Papyrus.addToLogPage(sAdd, sBefore, sAfter) ;
}

#ifndef _ENTERPRISE_DLL// -----------------------------------------------------------------------------// Description : Enregistre les paramètres// -----------------------------------------------------------------------------
bool
NSPredi::saveParams()
{
  NSUtilisateurChoisi *pUtilisat = pContexte->getUtilisateur() ;
  if (NULL == pUtilisat)
    return false ;

	string sFichier = pContexte->PathName("FGLO") + string("episo") + pUtilisat->getID() + string(".dat") ;

  ofstream outFile ;
  string sLine ;
  outFile.open(sFichier.c_str(), ios::out) ;
  if (!outFile)
  {
    string sErreur = "Erreur d'ouverture du fichier " + string(sFichier) ;
    erreur(sErreur.c_str(), standardError, 0) ;
    return false ;
  }

  if (_bPrediActif)
    outFile << (string("ACTIF              ") + string("Oui") + string("\n")) ;
  else
    outFile << (string("ACTIF              ") + string("Non") + string("\n")) ;

  if (_bMinimalInterface)
    outFile << (string("INTERFACE_MINIMALE ") + string("Oui") + string("\n")) ;
  else
    outFile << (string("INTERFACE_MINIMALE ") + string("Non") + string("\n")) ;

  if (_bLogPage)
    outFile << (string("LOG_WINDOW         ") + string("Oui") + string("\n")) ;
  else
    outFile << (string("LOG_WINDOW         ") + string("Non") + string("\n")) ;

  outFile.close() ;

  return true ;
}

TWindow*
NSPredi::isDecisionTreeOpen(string sArchetype)
{
  if (_aOpenDecisionTrees.empty())
    return (TWindow*) 0 ;

  for (ArrayPrediDataIter it = _aOpenDecisionTrees.begin() ; _aOpenDecisionTrees.end() != it ; it++)
    if ((*it)->getTitle() == sArchetype)
      return (*it)->getWindow() ;

  return (TWindow*) 0 ;
}

#endif // #ifndef _ENTERPRISE_DLL

# endif // !__EPIBRAIN__
#endif // !__NSEPICAP_H__
