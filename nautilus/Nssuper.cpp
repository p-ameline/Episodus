// -----------------------------------------------------------------------------// Nssuper.cpp
// -----------------------------------------------------------------------------
// Implémentation des méthodes du Superviseur.
// -----------------------------------------------------------------------------
// $Revision: 1.80 $
// $Author: pameline $
// $Date: 2017/07/31 12:42:35 $
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

#define __NSSUPER

#include <owl\window.h>
#include <owl\applicat.h>
#include <owl\owlpch.h>
#include <stdio.h>
#include <fstream>
#include <bde.hpp>
#include <sysutils.hpp>

#include "nssavoir\nsBdmDriver.h"
#include "nssavoir\nspathor.h"
#include "nautilus\nsrechdl.h"    // Pour ouvrir un nouveau patient
#include "nautilus\nscrvue.h"     // Document / Vues CN (Compte-Rendu Nautilus)
#include "nautilus\nsttx.h" 	    // Document / Vues TTX
#include "nautilus\nsAcroRead.h" 	// Document / Vues PDF
#include "nautilus\nscsdoc.h"     // Document / Vues CS Consultation
#include "nautilus\nscsvue.h"     // Document / Vues CS Consultation
#include "nautilus\nscqvue.h"     // Document / Vues CQ Formulaires
#include "nautilus\nsCoopChartDV.h"     
#include "nautilus\nsvisual.h"	  // Document / Vues Visualisation#include "nssavoir\nstransa.h"    // Transactions#include "nautilus\nsmodhtm.h"    // Pour le HtmlCS
#include "nautilus\nsepicap.h"    // Episodus
#include "nautilus\nscompub.h"
#include "nautilus\nsdocaga.h"    
#include "nssavoir\nsgraphe.h"    // Graphe + Gestion des archetypes
#include "nssavoir\nshealthteam.h"
#include "nsoutil\nsoutdlg.h"
#include "nsoutil\nsepiout.h"
#include "nsoutil\nsfilgui.h"
#include "nsoutil\nsHtmlToPdf.h"
#include "nsbb\ns_skins.h"
#include "nsbb\nsarc.h"
#include "nsbb\nsdefArch.h"
#include "nsbb\nsTipBrowser.h"
#include "nsbb\nsbbitem.h"
#include "nsbb\nsbb_dlg.h"
#include "nsbb\nsmediccycle.h"
#include "nsbb\nslistwind.h"
#include "ns_grab\nsgrabfc.h"
#include "nautilus\nsdocview.h"
#include "nautilus\nsbrowseSvce.h"
#include "nautilus\nshistdo.h"
#include "nautilus\nscaptview.h"
#include "nautilus\nscqdoc.h"
#include "nautilus\nsldvdoc.h"
#include "nautilus\nsmdiframe.h"
#include "nsldv\nsldvdrug.h"
#include "nautilus\nsldvvue.h"
#include "nsldv\nsldvgoal.h"
#include "nautilus\ns_html.h"
#include "dcodeur\nsdkd.h"
#include "dcodeur\nsphrobj.h"
#include "dcodeur\nsphrase.h"
#include "nsepisod\nsToDo.h"
#include "nsepisod\nsCsvParser.h"
#include "balise.h"

#include "nautilus\nsbackup.h"

#include "pilot\Pilot.hpp"
#include "pilot\NautilusPilot.hpp"
#include "pilot\JavaSystem.hpp"#ifndef __NSSUPER_H
# include "nautilus\nssuper.h"
#endif

#include "ns_ob1\nautilus-bbk.h"
#include "partage\nsdivfile.h"

#ifdef __EPIPUMP__
# include "EpiPump\IHEGlobal.h"
#endif

//#include "ns_bbk\TControler\Interface.h"

//***************************************************************************
// Implémentation des méthodes de NSSuper
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur.
//
//  Crée une instance d'objet de la classe NSSuper.
//	 Crée un moteur Paradox et une database.
//	 Initialise statut.
//
//---------------------------------------------------------------------------
NSSuper::NSSuper()
        :_statut(0), _hNSdb(NULL), _JniManager(this)
{
try
{
	sNumVersion         = string("5.14.0013") ;
  _numInstance        = 0 ;
	_pSuperContext      = (NSContexte*) 0 ;
  _bJavaOk            = false ;

  initInstanceCounters() ;

  pmExecArgs          = new std::map<string, string> ;

  _sHindex            = string("") ;
	_sHcorps            = string("") ;
	_pHelpTip           = (NSWebTipWindow*) 0 ;

  _paSkins            = new skinArray(this) ;
  _pFilGuide          = new NSFilGuide(this, GUIDE) ;
  _pFilDecode         = new NSFilGuide(this, DECODE) ;
  pBufDragDrop        = new NSCutPaste(this) ;
  pBufCopie           = new NSCutPaste(this) ;

  _iTraceConsole        = trNone ;
  _iTraceBBK            = trNone ;
  _iTrace               = trWarning  ;
  _iIndexManagementRule = umAuto /* imUndefined */ ;

  _pLocalStrings      = new NSLocalChapterArray ;
  _PathDB             = new NSLocalChapterArray ;
  _typeMime           = new NSLocalChapterArray ;
  _whereToFind        = new NSLocalChapterArray ;

  loadLocalisationString("") ;

  _pArcManager        = new NSArcManager(this) ;
	_pPersonArray       = new NSPersonArray(this) ;
  bDragAndDrop        = false ;
  offNode             = false ;
  DragDrop            = (OWL::TImageList*) 0 ;
  _pMainWindow        = (TWindow*) 0 ;
  _pDialogFont        = new TFont("MS Sans Serif", 8) ;

  _parite                = 0 ;
  _monnaieRef            = MONNAIE_EURO ;
  _sigle                 = string("") ;
  _bComptaAuto           = false ;
  _bNoCCAMforReport      = false ;
  _bNoCCAMforExam        = true ;
  _indiceConsult         = string("") ;
  _bCodageAuto           = false ;
  _sExport_dll           = string("") ;
  _sAccountingExport_dll = string("") ;
  _sStaysSynchro_dll     = string("") ;
  _sIppSite              = string("") ;
  _sOperationalSite      = string("") ;
  _sNomModule            = string("") ;
  _sDiffuseur            = string("") ;
  _sURLDiffuseur         = string("") ;
  _sDefaultFctUnitId     = string("") ;
  _sCarePlaceId          = string("") ;
  _sInPatientsFileDesc   = string("") ;
  _sOdbcPatientsFileDesc = string("") ;
  _bUseIeForPdf          = true ;
  _bMustKillPdfProcess   = false ;

  for (int i = 0 ; i < 9 ; i++)
    _sUserFctDll[i] = string("") ;

  _sDemographicArchetypeId              = NAME_ARC_ADMIN ;
	_sAddressArchetypeId                  = NAME_ARC_ADRESSE ;
	_sSimpleAddressArchetypeId            = NAME_ARC_ADRSIMPLE ;
	_sPatientAddressEditArchetypeId       = NAME_ARC_ADRPATEDIT ;
	_sProfessionnalAddressEditArchetypeId = NAME_ARC_ADRCOREDIT ;
	_sReferalProfessionnalArchetypeId     = NAME_ARC_CORRESP ;
	_sReferalProfessionnalEditArchetypeId = NAME_ARC_COREDIT ;
	_sTeamMemberArchetypeId               = NAME_ARC_NEWTEAM ;

  _bModeClientGroup    = false ;
	_bGroupInterfaceOnly = false ;
  _bSlaveMode          = false ;
  _sSlaveDllName       = string("") ;

  _bVerboseErrorMessages = true ;
  _pGrabModule         = (OWL::TModule*) 0 ;  _pMatrox             = (NSMatrox*) 0 ;
	_pNSApplication	     = (TMyApp*) 0 ;

	_bToDoLocked         = false ;

  _pHtmlToPdfConverter = (NSHtmlToPdfConverter*) 0 ;

//  NSSuper *pSup = pFilGuide->pSuperContext->getSuperviseur() ;

	_m_hStdOut					 = NULL ;
}
catch (...)
{
  erreur("Exception NSSuper ctor.", standardError, 0) ;
}
}

//---------------------------------------------------------------------------//  Constructeur copie
//---------------------------------------------------------------------------
NSSuper::NSSuper(const NSSuper& srcNSSuper)
        :_statut(srcNSSuper._statut),
         _hNSdb(srcNSSuper._hNSdb),
         _numInstance(srcNSSuper._numInstance),
         _pFilGuide(srcNSSuper._pFilGuide),
         _pMainWindow(srcNSSuper._pMainWindow),
         _bJavaOk(srcNSSuper._bJavaOk),
         _sExport_dll(srcNSSuper._sExport_dll),
         _sAccountingExport_dll(srcNSSuper._sAccountingExport_dll),
         _sStaysSynchro_dll(srcNSSuper._sStaysSynchro_dll),
         _sIppSite(srcNSSuper._sIppSite),
         _sOperationalSite(srcNSSuper._sOperationalSite),
         _sDemographicArchetypeId(srcNSSuper._sDemographicArchetypeId),
         _sAddressArchetypeId(srcNSSuper._sAddressArchetypeId),
         _sSimpleAddressArchetypeId(srcNSSuper._sSimpleAddressArchetypeId),
         _sPatientAddressEditArchetypeId(srcNSSuper._sPatientAddressEditArchetypeId),
         _sProfessionnalAddressEditArchetypeId(srcNSSuper._sProfessionnalAddressEditArchetypeId),
         _sReferalProfessionnalArchetypeId(srcNSSuper._sReferalProfessionnalArchetypeId),
         _sReferalProfessionnalEditArchetypeId(srcNSSuper._sReferalProfessionnalEditArchetypeId),
         _sTeamMemberArchetypeId(srcNSSuper._sTeamMemberArchetypeId),
         _pGrabModule(srcNSSuper._pGrabModule),
         _pMatrox(srcNSSuper._pMatrox),
         _aSupportsArray(srcNSSuper._aSupportsArray),
         _sNomModule(srcNSSuper._sNomModule),
         _sDiffuseur(srcNSSuper._sDiffuseur),
         _sURLDiffuseur(srcNSSuper._sURLDiffuseur),
         _sDefaultFctUnitId(srcNSSuper._sDefaultFctUnitId),
         _sCarePlaceId(srcNSSuper._sCarePlaceId),
         _bUseIeForPdf(srcNSSuper._bUseIeForPdf),
         _bMustKillPdfProcess(srcNSSuper._bMustKillPdfProcess),
         _sInPatientsFileDesc(srcNSSuper._sInPatientsFileDesc),
         _sOdbcPatientsFileDesc(srcNSSuper._sOdbcPatientsFileDesc),
         _JniManager(this)
{
  pmExecArgs = new std::map<string, string> ;
  *pmExecArgs = *(srcNSSuper.pmExecArgs) ;

  if (srcNSSuper._pDialogFont)
  	_pDialogFont = new TFont(*(srcNSSuper._pDialogFont));

  _bModeClientGroup      = srcNSSuper._bModeClientGroup ;
  _bGroupInterfaceOnly   = srcNSSuper._bGroupInterfaceOnly ;
  _bSlaveMode            = srcNSSuper._bSlaveMode ;
  _sSlaveDllName         = srcNSSuper._sSlaveDllName ;
  _bVerboseErrorMessages = srcNSSuper._bVerboseErrorMessages ;
  _iIndexManagementRule  = srcNSSuper._iIndexManagementRule ;

  for (int i = 0 ; i < 9 ; i++)
    _sUserFctDll[i] = srcNSSuper._sUserFctDll[i] ;

  _pHtmlToPdfConverter   = (NSHtmlToPdfConverter*) 0 ;

	_bToDoLocked    = false ;
}

//---------------------------------------------------------------------------//  Opérateur =
//---------------------------------------------------------------------------
NSSuper&
NSSuper::operator=(const NSSuper& srcNSSuper)
{
	if (this == &srcNSSuper)
		return *this ;

  *pmExecArgs       = *(srcNSSuper.pmExecArgs) ;

  _statut           = srcNSSuper._statut ;
  _hNSdb            = srcNSSuper._hNSdb ;
  _bJavaOk          = srcNSSuper._bJavaOk ;
  _numInstance      = srcNSSuper._numInstance ;
  _pFilGuide   	    = srcNSSuper._pFilGuide ;
  pBufDragDrop      = srcNSSuper.pBufDragDrop ;
  pBufCopie         = srcNSSuper.pBufCopie ;
  bDragAndDrop      = srcNSSuper.bDragAndDrop ;
  offNode 		      = srcNSSuper.offNode ;
  DragDrop 		      = srcNSSuper.DragDrop ;
  _pMainWindow      = srcNSSuper._pMainWindow ;

  _pDialogFont           = new TFont(*(srcNSSuper._pDialogFont)) ;
  _sExport_dll           = srcNSSuper._sExport_dll ;
  _sAccountingExport_dll = srcNSSuper._sAccountingExport_dll ;
  _sStaysSynchro_dll     = srcNSSuper._sStaysSynchro_dll ;
  _sIppSite              = srcNSSuper._sIppSite ;
	_sOperationalSite      = srcNSSuper._sOperationalSite ;
  _iTraceConsole         = srcNSSuper._iTraceConsole ;
  _iTraceBBK             = srcNSSuper._iTraceBBK ;
  _iTrace                = srcNSSuper._iTrace ;

  _sDemographicArchetypeId              = srcNSSuper._sDemographicArchetypeId ;
	_sAddressArchetypeId                  = srcNSSuper._sAddressArchetypeId ;
	_sSimpleAddressArchetypeId            = srcNSSuper._sSimpleAddressArchetypeId ;
	_sPatientAddressEditArchetypeId       = srcNSSuper._sPatientAddressEditArchetypeId ;
	_sProfessionnalAddressEditArchetypeId = srcNSSuper._sProfessionnalAddressEditArchetypeId ;
	_sReferalProfessionnalArchetypeId     = srcNSSuper._sReferalProfessionnalArchetypeId ;
	_sReferalProfessionnalEditArchetypeId = srcNSSuper._sReferalProfessionnalEditArchetypeId ;
  _sTeamMemberArchetypeId               = srcNSSuper._sTeamMemberArchetypeId ;

  _bModeClientGroup      = srcNSSuper._bModeClientGroup ;
  _bGroupInterfaceOnly   = srcNSSuper._bGroupInterfaceOnly ;
  _bSlaveMode            = srcNSSuper._bSlaveMode ;
  _sSlaveDllName         = srcNSSuper._sSlaveDllName ;
  _bVerboseErrorMessages = srcNSSuper._bVerboseErrorMessages ;
  _iIndexManagementRule  = srcNSSuper._iIndexManagementRule ;
  _pGrabModule           = srcNSSuper._pGrabModule ;  _pMatrox               = srcNSSuper._pMatrox ;  _aSupportsArray        = srcNSSuper._aSupportsArray ;  _sNomModule            = srcNSSuper._sNomModule ;  _sDiffuseur            = srcNSSuper._sDiffuseur ;
  _sURLDiffuseur         = srcNSSuper._sURLDiffuseur ;
  _sDefaultFctUnitId     = srcNSSuper._sDefaultFctUnitId ;
  _sCarePlaceId          = srcNSSuper._sCarePlaceId ;
  _bUseIeForPdf          = srcNSSuper._bUseIeForPdf ;
  _bMustKillPdfProcess   = srcNSSuper._bMustKillPdfProcess ;
  _sInPatientsFileDesc   = srcNSSuper._sInPatientsFileDesc ;  _sOdbcPatientsFileDesc = srcNSSuper._sOdbcPatientsFileDesc ;
  for (int i = 0 ; i < 9 ; i++)
    _sUserFctDll[i] = srcNSSuper._sUserFctDll[i] ;

  _pHtmlToPdfConverter  = (NSHtmlToPdfConverter*) 0 ;

	_bToDoLocked          = false ;

  return (*this) ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSSuper::~NSSuper()
{
  delete pmExecArgs ;

  if (_pFilGuide)
    delete _pFilGuide ;

  if (_m_hStdOut)
  {
    FreeConsole() ;
    CloseHandle(_m_hStdOut) ;
  }

#ifndef __EPIPUMP__
# ifndef __EPIBRAIN__
	if (_pHelpTip)
		delete _pHelpTip ;
# endif // !__EPIBRAIN__
#endif // !__EPIPUMP__

  delete pBufDragDrop ;
  delete pBufCopie ;

  // delete pGraphe ;
  delete _pArcManager ;  delete _pPersonArray ;  delete _pDialogFont ;  delete _paSkins ;
  if (_pGrabModule)  {    delete _pGrabModule ;    _pGrabModule = (OWL::TModule*) 0 ;  }
  delete _pLocalStrings ;
  delete _PathDB ;
  delete _typeMime ;
  delete _whereToFind ;

  _aToDo.vider() ;

  if (_pHtmlToPdfConverter)
    delete _pHtmlToPdfConverter ;

  traceInstanceCounters() ;
}
void
NSSuper::registerContexte(NSContexte* pContexte)
{
  // There is currently only on context
  //
  _pSuperContext = pContexte ;
}

// -----------------------------------------------------------------------------// Fonction servant à initialiser les chemins et tles types
// -----------------------------------------------------------------------------
bool
NSSuper::InitialiseFileDatabase()
{
  string ps = string("Entering NSSuper::InitialiseFileDatabase") ;
  trace(&ps, 1, trSteps) ;

  // Récupération du répertoire perso
 	string sPathPerso = PathBase("BCHE") ;

  // Chargement des chemins
  //
  string sPathCDB = sPathPerso + "chemindb.dat" ;
  
  ifstream CheminDB ;
  CheminDB.open(sPathCDB.c_str(), ofstream::in) ;
  if (CheminDB.is_open())
  {
    string sErrorMsg = string("") ;
  	_PathDB->init(&CheminDB, &sErrorMsg) ;
    CheminDB.close() ;
  }
  else
  {
    ps = string("Cannot open file ") + sPathCDB ;
    trace(&ps, 1, trError) ;
    string error = "Le fichier " + sPathCDB + " n'existe pas" ;
    erreur(error.c_str(), standardError, 0) ;
  }

  // chargement des types mimes
  //
  string sPathMime = sPathPerso + "TypeMime.dat" ;

  ifstream Mime ;
  Mime.open(sPathMime.c_str(), ofstream::in) ;
  if (Mime.is_open())
  {
    string sErrorMsg = string("") ;
  	_typeMime->init(&Mime, &sErrorMsg) ;
    Mime.close() ;
  }
  else
  {
    ps = string("Cannot open file ") + sPathMime ;
    trace(&ps, 1, trError) ;
  	string error = "Le fichier " + sPathMime + " n'existe pas" ;
    erreur(error.c_str(), standardError, 0) ;
  }

  // chargement des types whereToFind
  //
  string sPathFind = sPathPerso + "whereToFind.dat" ;

  ifstream WhereTo ;
  WhereTo.open(sPathFind.c_str(), ofstream::in) ;
  if (WhereTo.is_open())
  {
    string sErrorMsg = string("") ;
  	_whereToFind->init(&WhereTo, &sErrorMsg) ;
    WhereTo.close() ;
  }
  else
  {
    ps = string("Cannot open file ") + sPathFind ;
    trace(&ps, 1, trError) ;
  	string error = "Le fichier " + sPathFind + " n'existe pas" ;
    erreur(error.c_str(), standardError, 0) ;
  }

  return true ;
}

bool
NSSuper::chercheTypeMimeInfo(string sFichExt, NSTypeMimeInfo* pTypeMimeInfo)
{
	string sLowerFichExt = string("") ;
  for (size_t i = 0 ; i < strlen(sFichExt.c_str()) ; i++)
		sLowerFichExt += string(1, (char) tolower(sFichExt[i])) ;

	string sTyp = _typeMime->getLocalText(sLowerFichExt, string("$TYPE")) ;
  if (string("") == sTyp)
		return false ;

	string sLib  = _typeMime->getLocalText(sLowerFichExt, string("$LIBELLE")) ;
  string sMime = _typeMime->getLocalText(sLowerFichExt, string("$MIME")) ;

	if (pTypeMimeInfo)
	{
  	pTypeMimeInfo->setExtension(sLowerFichExt) ;
  	pTypeMimeInfo->setLibelle(sLib) ;
  	pTypeMimeInfo->setMime(sMime) ;
  	pTypeMimeInfo->setType(sTyp) ;
	}

  return true ;
}

// -----------------------------------------------------------------------------
// Fonction    : NSSuper::InitialiseDatabase()
// Description : Initialise un moteur IDAPI et une database. Initialise statut.
// Retour      : True : OK, False : Echec
// -----------------------------------------------------------------------------
bool
NSSuper::InitialiseDatabase(NSContexte* pCtx)
{
  if (_pNSApplication)
  {
	  _pNSApplication->sendMessageToSplash("Init database") ;
    _pNSApplication->setSplashPercentDone(5) ;
  }

#ifdef __EPIPUMP__
# ifndef __EPIBRAIN__
  IHEGlobal::PrintReport("Database init - begin") ;
# endif
#endif
  string ps = string("Database init - begin") ;
  trace(&ps, 1, trSteps) ;

  // Ouverture de IDAPI
  // Initialise IDAPI

  bool bUseEnvironnement = false ;

  DBIEnv Environnement ;
  memset(&Environnement, 0, sizeof(DBIEnv));
  Environnement.bForceLocalInit = FALSE ;     // default value when env is NULL

  string sBdeParamsFileName = string(".\\bdeParams.dat") ;

  // This function requires VCL
  //
  // if (FileExists(AnsiString(sBdeParamsFileName.c_str())))
  // {
    ifstream inFile ;
	  // on ouvre le fichier de configuration
    inFile.open(sBdeParamsFileName.c_str()) ;

/*
    if (!inFile)
    {
  	  string sErrorMsg = getText("fileErrors", "errorOpeningInputFile") ;
      sErrorMsg += string(" ") + sBdeParamsFileName ;
      erreur(sErrorMsg.c_str(), warningError, 0) ;
    }
*/

    if (inFile)
    {
      string sData = string("") ;
	    while (!inFile.eof())
	    {
        string sLine = string("") ;
		    getline(inFile, sLine) ;
        sData += sLine + "\n" ;
	    }

      inFile.close() ;

      size_t i = 0 ;

	    while (i < strlen(sData.c_str()))
	    {
		    string sNomAttrib = string("") ;
		    string sValAttrib = string("") ;

		    while ((i < strlen(sData.c_str())) && (' ' != sData[i]) && ('\t' != sData[i]))
			    sNomAttrib += pseumaj(sData[i++]) ;
		    while ((i < strlen(sData.c_str())) && ((' ' == sData[i]) || ('\t' == sData[i])))
			    i++ ;

		    while ((i < strlen(sData.c_str())) && ('\n' != sData[i]))
			    sValAttrib += sData[i++] ;

		    i++ ;

        // Example c:\\Episodus\\bases
        //
		    if      ((sNomAttrib == "WORKDIR") && (sValAttrib != ""))
          strcpy(Environnement.szWorkDir, sValAttrib.c_str()) ;
        //
        // Example c:\\myidapi\\idapi32.cfg
        //
        else if ((sNomAttrib == "INIFILE") && (sValAttrib != ""))
          strcpy(Environnement.szIniFile, sValAttrib.c_str()) ;
        else if ((sNomAttrib == "LANG") && (sValAttrib != ""))
          strcpy(Environnement.szLang, sValAttrib.c_str()) ;
        else if ((sNomAttrib == "CLIENTNAME") && (sValAttrib != ""))
          strcpy(Environnement.szClientName, sValAttrib.c_str()) ;
        else if ((sNomAttrib == "FORCELOCALINIT") && (sValAttrib != ""))
		    {
				  if (IsYes(sValAttrib))
            Environnement.bForceLocalInit = TRUE ;
          else
            Environnement.bForceLocalInit = FALSE ;
        }
      }
      bUseEnvironnement = true ;
    }
//	}

#ifdef __EPIPUMP__
# ifndef __EPIBRAIN__
  IHEGlobal::PrintReport("DBI Init") ;
# endif
#endif

  DBIResult Err_DBI ;

  if (bUseEnvironnement)
    Err_DBI = DbiInit(&Environnement) ;
  else
    Err_DBI = DbiInit(NULL) ;

  if (DBIERR_NONE != Err_DBI)
  {
    _statut = 1 ;
#ifdef __EPIPUMP__
# ifndef __EPIBRAIN__
    IHEGlobal::PrintError("bdeError, cannotInitEngine") ;
# endif
#endif

    string sErreur = getText("bdeError", "cannotInitEngine") ;

    if (DBIERR_MULTIPLEINIT	== Err_DBI)
    {
      sErreur += string(": Illegal attempt to initialize BDE more than once.") ;
      Err_DBI = 0 ;
    }
    else if (DBIERR_NOCONFIGFILE	== Err_DBI)
    {
      sErreur += string(": No config file.") ;
      Err_DBI = 0 ;
    }

    trace(&sErreur, 1, NSSuper::trError) ;
    erreur(sErreur.c_str(), fatalError, Err_DBI) ;
    return false ;
  }

  // Utilisation du debugger
  DbiDebugLayerOptions(DEBUGON | OUTPUTTOFILE | FLUSHEVERYOP, "trace.inf");

#ifdef __EPIPUMP__
# ifndef __EPIBRAIN__
  IHEGlobal::PrintReport("DBI OpenDatabase") ;
# endif
#endif

  // IDAPI est initialisé. Ouverture d'une database standard
  Err_DBI = DbiOpenDatabase(NULL, NULL, dbiREADWRITE, dbiOPENSHARED, NULL, 0, NULL, NULL, _hNSdb) ;
  if (DBIERR_NONE != Err_DBI)
  {
    _statut = 1 ;
#ifdef __EPIPUMP__
# ifndef __EPIBRAIN__
    IHEGlobal::PrintError("bdeError, cannotInitDatabase") ;
# endif
#endif
    string sErreur = getText("bdeError", "cannotInitDatabase") ;

    // DBIERR_UNKNOWNDB     The specified database or database type is invalid.
    // DBIERR_NOCONFIGFILE	The configuration file was not found.
    // DBIERR_INVALIDDBSPEC When using an alias from the configuration file, the specification is invalid.
    // DBIERR_DBLIMIT       The maximum number of databases have been opened.

    trace(&sErreur, 1, NSSuper::trError) ;
    erreur(sErreur.c_str(), fatalError, Err_DBI) ;
    return false ;
  }
  _statut = 0 ;

#ifdef __EPIPUMP__
# ifndef __EPIBRAIN__
  IHEGlobal::PrintReport("InitPathBase") ;
# endif
#endif
  // On récupère les chemins d'accès aux bases
  if (!InitPathBase(pCtx))
  {
#ifdef __EPIPUMP__
# ifndef __EPIBRAIN__
  	IHEGlobal::PrintError("InitPathBase failed") ;
# endif
#endif
    return false ;
  }

#ifdef __EPIPUMP__
# ifndef __EPIBRAIN__
  IHEGlobal::PrintReport("InitParite") ;
# endif
#endif
  // parite Franc / Euro
  if (false == InitParite())
  {
#ifdef __EPIPUMP__
# ifndef __EPIBRAIN__
  	IHEGlobal::PrintError("InitParite failed") ;
# endif
#endif
    return false ;
  }

#ifdef __EPIPUMP__
# ifndef __EPIBRAIN__
  IHEGlobal::PrintReport("Database init - done") ;
# endif
#endif
  ps = string("Database init - done") ;
  trace(&ps, 1, trSteps) ;

  if (string("") != _sInPatientsFileDesc)
  {
    ps = string("In-patient file description is ") + _sInPatientsFileDesc ;
    trace(&ps, 1, trSubSteps) ;
  }

  return true ;
}
// -----------------------------------------------------------------------------
// Function    : NSSuper::AppelUtilisateur()
// Description : Crée l'utilisateur principal.
// Retour      : Aucun
// -----------------------------------------------------------------------------
void
NSSuper::AppelUtilisateur()
{
try
{
  string ps = string("Entering AppelUtilisateur") ;
	trace(&ps, 1, trSteps) ;

  NSUtilisateurChoisi* pNewUser = GetNewUser() ;

  if ((NSUtilisateurChoisi*) NULL == pNewUser)
    return ;

  // Closing patient asynchonously
  //
  NSPatientChoisi*     pOpenedPatient = _pSuperContext->getPatient() ;
  NSUtilisateurChoisi* pPreviousUser  = _pSuperContext->getUtilisateur() ;
  if (pPreviousUser && pOpenedPatient)
  {
    if (false == pPreviousUser->CanClosePatient())
      return ;

    NSToDoTask *pToDo = new NSToDoTask ;
    pToDo->setWhatToDo(string("SettleUser")) ;
    pToDo->setPointer1((void*) pNewUser, false) ;

    pPreviousUser->setToDoAfterPatClose(pToDo) ;
    pPreviousUser->ClosePatientAsynchronous(NSUtilisateurChoisi::cpnExecToDo) ;

    return ;
  }

  SettleNewUser(pNewUser) ;
}
catch (...)
{
	erreur("Exception NSSuper::AppelUtilisateur.", standardError, 0) ;
}
}

NSUtilisateurChoisi*
NSSuper::GetNewUser()
{
try
{
  string sPreviousUserLanguage = string("") ;

  // Enregistrement des paramètres de l'utilisateur précédent
  NSUtilisateurChoisi* pPreviousUser = _pSuperContext->getUtilisateur() ;
  if (pPreviousUser)
  {
  	sPreviousUserLanguage = pPreviousUser->donneLang() ;

#ifndef __EPIPUMP__
    NSEpisodus* pEpisodus = _pSuperContext->getEpisodus() ;
    if (pEpisodus && pEpisodus->bAutoSave)
    {
      NSPrometheFile PrometCfg(_pSuperContext) ;
      PrometCfg.EcrireFichier(pPreviousUser->getID()) ;
    }
#endif // !__EPIPUMP__
	}

  // new NSDataGraph
  // sLogin
  // sPassword
  //initilisation du dataGraph et login+password pour un nouveau utilisateur

  // Mise en place du nouvel utilisateur
  // NSDataGraph*     pDataGraph = new NSDataGraph(this, graphPerson) ;
  // NSUtilisateurChoisi* nsUtil = new NSUtilisateurChoisi(_pSuperContext, pDataGraph) ;

  NSUtilisateurChoisi* pNewUser = new NSUtilisateurChoisi(_pSuperContext) ;

  if (false == _bSlaveMode)
  {
    string ps = string("GetNewUser, not in slave mode: opening login dialog") ;
    trace(&ps, 1, trSubDetails) ;

    int retour = UserLoginNTiersDialog(donneMainWindow(), _pSuperContext, pNewUser).Execute() ;

    // cas du changement de mot de passe :
    bool bPwdOk = true ;
    if (pNewUser->pwdNeedChange())
      bPwdOk = pNewUser->changePassword() ;
    if (false == bPwdOk)
      retour = IDCANCEL ;

    // Choix OK dans la boite de dialogue
    if (IDOK != retour)
    {
      delete pNewUser ;
      // delete pDataGraph ;

      ps = string("AppelUtilisateur, login dialog cancelled") ;
      trace(&ps, 1, trSubDetails) ;

      return (NSUtilisateurChoisi*) 0 ;
    }

    return pNewUser ;
  }

  if (string("") != _sSlaveDllName)
  {
    string ps = string("GetNewUser, in slave mode") ;
    trace(&ps, 1, trSubDetails) ;

    TModule* pSlaveModule = new TModule(_sSlaveDllName.c_str(), TRUE) ;
		if (NULL == pSlaveModule)
		{
      string sErreur = getText("modulesManagement", "cannotLoadDll") + string(" ") + _sSlaveDllName ;
      trace(&sErreur, 1, NSSuper::trError) ;
			erreur(sErreur.c_str(), standardError, 0) ;

			delete pNewUser ;
      // delete pDataGraph ;
			return (NSUtilisateurChoisi*) 0 ;
		}

    NSBasicAttributeArray ResultArray ;

    bool (FAR *pAdrFctOpenUser) (NSContexte far *, NSBasicAttributeArray far *) ;
    (FARPROC) pAdrFctOpenUser = pSlaveModule->GetProcAddress(MAKEINTRESOURCE(1)) ;
		if (pAdrFctOpenUser)
		{
			bool bSuccess = ((*pAdrFctOpenUser)((NSContexte far *) _pSuperContext,
                                          (NSBasicAttributeArray far *) &ResultArray)) ;
      if (false == bSuccess)
      {
        string sErreur = string("GetNewUser, in slave mode, calling ") + string(" ") + _sSlaveDllName + string(" returned false; cancelling.") ;
        trace(&sErreur, 1, NSSuper::trError) ;

      	delete pNewUser ;
        // delete pDataGraph ;
        delete pSlaveModule ;
			  return (NSUtilisateurChoisi*) 0 ;
      }

      pNewUser->SetUtilisateurChoisi(&ResultArray) ;
    }

		delete pSlaveModule ;

    return pNewUser ;
  }

  delete pNewUser ;
  // delete pDataGraph ;

  return (NSUtilisateurChoisi*) 0 ;
}
catch(TWindow::TXWindow& e)
{
	string sErrorMsg = string("Exception NSSuper::GetNewUser : ") + e.why() ;
  trace(&sErrorMsg, 1, trError) ;
	erreur(sErrorMsg.c_str(), standardError, 0) ;
  return (NSUtilisateurChoisi*) 0 ;
}
catch (...)
{
  string sErrorMsg = string("Exception NSSuper::GetNewUser") ;
  trace(&sErrorMsg, 1, trError) ;
	erreur(sErrorMsg.c_str(), standardError, 0) ;
  return (NSUtilisateurChoisi*) 0 ;
}
}

void
NSSuper::SettleNewUser(NSUtilisateurChoisi* pNewUser)
{
try
{
  if ((NSUtilisateurChoisi*) NULL == pNewUser)
    return ;

  string sPreviousUserLanguage = string("") ;

  // Destruction de l'utilisateur précédent
  // la transaction est fermée automatiquement
  //
  // Putting out previous user
  // Current transaction is automatically closed
  //
  if (_pSuperContext->getUtilisateur())
  {
    sPreviousUserLanguage = _pSuperContext->getUtilisateur()->donneLang() ;

    delete _pSuperContext->getUtilisateur() ;
    _pSuperContext->setUtilisateur(0) ;
  }

  //             Mise en place du nouvel utilisateur
  // **************** Putting new user in place ****************
  //
  string ps = string("Setting new user") ;
  trace(&ps, 1, trDetails) ;

  _pSuperContext->setUtilisateur(pNewUser) ;
  _pSuperContext->getDico()->UserChanged() ;

  ps = string("Getting localization strings for new user") ;
  trace(&ps, 1, trDetails) ;

  string sNewUserLanguage = _pSuperContext->getUserLanguage() ;
  loadLocalisationString(sNewUserLanguage) ;

  // Set main caption
  //
  _pSuperContext->setMainCaption() ;

  ps = string("Initializing mail information for new user") ;
  trace(&ps, 1, trDetails) ;

  _pSuperContext->getUtilisateur()->initMail() ;

  if (sPreviousUserLanguage != sNewUserLanguage)
  {
    HACCEL hAccelerator ;
    getApplication()->setMenu(string(""), &hAccelerator) ;
  }

  ps = string("Setting up tool bar for new user") ;
  trace(&ps, 1, trDetails) ;

  _pSuperContext->getUtilisateur()->setupToolbar() ;
  // bool bServiceMode = true ;

#ifndef __EPIPUMP__
  NSEpisodus* pEpisodus = _pSuperContext->getEpisodus() ;
  if (pEpisodus)
  {
    ps = string("UserChanged for Episodus") ;
    trace(&ps, 1, trDetails) ;

    pEpisodus->UserChanged() ;
  }

#ifndef __EPIBRAIN__
  if (_pSuperContext->getPredi())
  {
    ps = string("UserChanged for PreDI") ;
    trace(&ps, 1, trDetails) ;

    _pSuperContext->getPredi()->UserChanged() ;
  }
#endif // !__EPIBRAIN__

  if (string("") != pEpisodus->sServiceUrl)
  {
    ps = string("Creating WebServices : ") + pEpisodus->sServiceUrl ;
    trace(&ps, 1, trDetails) ;

    _pSuperContext->_pWebServiceWindow = new NSWebServiceWindow(donneMainWindow(), pEpisodus->sServiceUrl, _pSuperContext) ;
    _pSuperContext->_pWebServiceChild  = new NSWebServiceChild(_pSuperContext, *(getApplication()->prendClient()), pEpisodus->sServiceTitle.c_str(), _pSuperContext->_pWebServiceWindow) ;
    _pSuperContext->_pWebServiceChild->Create() ;
  }
#endif // !__EPIPUMP__

  if (_bInitInstance)
  {
    ps = string("Unlocking the console") ;
    trace(&ps, 1, trDetails) ;

    _pSuperContext->getUtilisateur()->debloqueConsole() ;
    _bInitInstance = false ;
  }

  /*
  if(pSuperContext->getUtilisateur()->sPasswordType == "T")
  {
    string sErrorText = pSuperContext->getSuperviseur()->getText("N_TIERS", "passwordMustBeChanged") ;
    if (sErrorText == "")
      sErrorText = "Il faut changer votre mot de passe" ;
    erreur(sErrorText.c_str(), 0, 0, 0) ;

    if (!pSuperContext->getUtilisateur()->Modifier(pSuperContext->GetMainWindow()))
      erreur("Erreur à la modification d'un nouvel utilisateur", 0, 0) ;
  }
  */

  // Open In-patients list
  //
  bool bMustCallPatient = true ;

  string sInPatientsFileDesc = getInPatientsFileDesc() ;
  if ((string("") != sInPatientsFileDesc) && _pNSApplication)
  {
    NSMDIFrame* pFrame = _pNSApplication->getFrame() ;
    if (pFrame)
    {
      ps = string("Opening in-patients list (from params file \"") + sInPatientsFileDesc + string("\")") ;
      trace(&ps, 1, trDetails) ;

      pFrame->openWhoIsThereWindow(sInPatientsFileDesc) ;
      bMustCallPatient = false ;
    }
  }

  // Appel du patient
  if (bMustCallPatient && _pSuperContext->getUtilisateur()->isUser())
  {
    ps = string("Calling for a patient") ;
    trace(&ps, 1, trDetails) ;

    if (false == _bSlaveMode)
      _pSuperContext->getUtilisateur()->AppelPatient() ;
    else
      _pSuperContext->getUtilisateur()->CallPatientInSlaveMode() ;
  }
}
catch (...)
{
  string sErrorMsg = string("Exception NSSuper::SettleNewUser") ;
  trace(&sErrorMsg, 1, trError) ;
	erreur(sErrorMsg.c_str(), standardError, 0) ;
}
}

// -----------------------------------------------------------------------------
// Function     : NSSuper::FermeDatabase()
// -----------------------------------------------------------------------------
// Description  : Ferme le moteur IDAPI.
// -----------------------------------------------------------------------------
// Retour       : Aucun
// -----------------------------------------------------------------------------
void
NSSuper::FermeDatabase()
{
  string ps = string("Closing BDE database and engine") ;
  trace(&ps, 1, trSteps) ;

  // Closing BDE database
  //
  DBIResult Err_DBI = DbiCloseDatabase(_hNSdb) ;

  if (DBIERR_NONE != Err_DBI)
  {
    string sError = getText("bdeError", "cannotCloseDatabase") ;
    trace(&sError, 1, trError) ;
    erreur(sError.c_str(), fatalError, Err_DBI) ;
  }

  DbiDebugLayerOptions(NULL, NULL) ;

  // Closing IDAPI engine
  //
  Err_DBI = DbiExit() ;

  if (DBIERR_NONE != Err_DBI)
  {
    string sError = getText("bdeError", "cannotCloseEngine") ;
    trace(&sError, 1, trError) ;
    erreur(sError.c_str(), fatalError, Err_DBI) ;
  }

  ps = string("BDE closed") ;
  trace(&ps, 1, trSteps) ;
}

void
NSSuper::DeleteLocalDatabaseObjects()
{
  if (_pFilGuide)
  {
    delete _pFilGuide ;
    _pFilGuide = (NSFilGuide*) 0 ;
  }
  if (_pFilDecode)
  {
    delete _pFilDecode ;
    _pFilDecode = (NSFilGuide*) 0 ;
  }
}

void
NSSuper::traceInstanceCounters()
{
  // CheckZeroObjectCount(string("NSSuperRoot"),                 NSSuperRoot::getSRNbInstance()) ;
  // CheckZeroObjectCount(string("NSRoot"),                      NSRoot::getRootNbInstance()) ;

  CheckZeroObjectCount(string("NSFiche"),                     NSFiche::getNbInstance()) ;

  CheckZeroObjectCount(string("NSPatInfo"),                   NSPatInfo::getNbInstance()) ;
  CheckZeroObjectCount(string("NSCorrespondantInfo"),         NSCorrespondantInfo::getNbInstance()) ;

  CheckZeroObjectCount(string("NSBasicAttribute"),            NSBasicAttribute::getNbInstance()) ;
  CheckZeroObjectCount(string("NSBasicAttributeArray"),       NSBasicAttributeArray::getNbInstance()) ;
  CheckZeroObjectCount(string("NSPersonsAttributesArray"),    NSPersonsAttributesArray::getNbInstance()) ;

  CheckZeroObjectCount(string("NSDataTree"),                  NSDataTree::getNbInstance()) ;
  CheckZeroObjectCount(string("NSDataTreeArray"),             NSDataTreeArray::getNbInstance()) ;
  CheckZeroObjectCount(string("NSArcLink"),                   NSArcLink::getNbInstance()) ;
  CheckZeroObjectCount(string("NSArcLinkArray"),              NSArcLinkArray::getNbInstance()) ;
  CheckZeroObjectCount(string("NSNodeRight"),                 NSNodeRight::getNbInstance()) ;
  CheckZeroObjectCount(string("NSNodeRightArray"),            NSNodeRightArray::getNbInstance()) ;
  CheckZeroObjectCount(string("NSDataGraph"),                 NSDataGraph::getNbInstance()) ;
  CheckZeroObjectCount(string("NSObjectGraphManager"),        NSObjectGraphManager::getNbInstance()) ;
  CheckZeroObjectCount(string("NSPersonGraphManager"),        NSPersonGraphManager::getNbInstance()) ;

  CheckZeroObjectCount(string("NSPatPathoInfo"),              NSPatPathoInfo::getNbInstance()) ;
  CheckZeroObjectCount(string("NSPatPathoArray"),             NSPatPathoArray::getNbInstance()) ;
  CheckZeroObjectCount(string("NSVectPatPathoArray"),         NSVectPatPathoArray::getNbInstance()) ;
  CheckZeroObjectCount(string("NSFatheredPatPathoArray"),     NSFatheredPatPathoArray::getNbInstance()) ;
  CheckZeroObjectCount(string("NSVectFatheredPatPathoArray"), NSVectFatheredPatPathoArray::getNbInstance()) ;

  CheckZeroObjectCount(string("NSTreeNode"),                  NSTreeNode::getNbInstance()) ;

  CheckZeroObjectCount(string("NVLdVTemps"),                  NVLdVTemps::getNbInstance()) ;
  CheckZeroObjectCount(string("NVLdVPoint"),                  NVLdVPoint::getNbInstance()) ;
  CheckZeroObjectCount(string("NVLdVRect"),                   NVLdVRect::getNbInstance()) ;
  CheckZeroObjectCount(string("NSPixel"),                     NSPixel::getNbInstance()) ;

  CheckZeroObjectCount(string("NSDocumentInfo"),              NSDocumentInfo::getNbInstance()) ;
  CheckZeroObjectCount(string("NSRefDocument"),               NSRefDocument::getNbInstance()) ;
  CheckZeroObjectCount(string("NSNoyauDocument"),             NSNoyauDocument::getNbInstance()) ;
  CheckZeroObjectCount(string("NSDocumentHisto"),             NSDocumentHisto::getNbInstance()) ;
  CheckZeroObjectCount(string("NSDocHistoArray"),             NSDocHistoArray::getNbInstance()) ;
  CheckZeroObjectCount(string("NSDocumentsTank"),             NSDocumentsTank::getNbInstance()) ;
  CheckZeroObjectCount(string("NSHISTODocument"),             NSHISTODocument::getNbInstance()) ;

  CheckZeroObjectCount(string("NSModHtml"),                   NSModHtml::getNbInstance()) ;
  CheckZeroObjectCount(string("NSBlocHtml"),                  NSBlocHtml::getNbInstance()) ;
  CheckZeroObjectCount(string("NSHtml"),                      NSHtml::getNbInstance()) ;

  CheckZeroObjectCount(string("BBFilsItem"),                  BBFilsItem::getNbInstance()) ;
  CheckZeroObjectCount(string("BBFilsArray"),                 BBFilsArray::getNbInstance()) ;
  CheckZeroObjectCount(string("BBItem"),                      BBItem::getNbInstance()) ;
  CheckZeroObjectCount(string("NSTransferInfo"),              NSTransferInfo::getNbInstance()) ;
  CheckZeroObjectCount(string("Message"),                     Message::getNbInstance()) ;
  CheckZeroObjectCount(string("NSControle"),                  NSControle::getNbInstance()) ;

  CheckZeroObjectCount(string("NSHealthTeam"),                NSHealthTeam::getNbInstance()) ;

  CheckZeroObjectCount(string("Cbalise"),                     Cbalise::getNbInstance()) ;
  CheckZeroObjectCount(string("CAttributs"),                  CAttributs::getNbInstance()) ;
  CheckZeroObjectCount(string("CValeur"),                     CValeur::getNbInstance()) ;

  CheckZeroObjectCount(string("NSHistoryValueManagement"),    NSHistoryValueManagement::getNbInstance()) ;

  CheckZeroObjectCount(string("NSConcernModifier"),           NSConcernModifier::getNbInstance()) ;
  CheckZeroObjectCount(string("NSConcern"),                   NSConcern::getNbInstance()) ;
  CheckZeroObjectCount(string("NSLdvSousObjet"),              NSLdvSousObjet::getNbInstance()) ;
  CheckZeroObjectCount(string("NSLdvObjet"),                  NSLdvObjet::getNbInstance()) ;
  CheckZeroObjectCount(string("NSLdvDrugTake"),               NSLdvDrugTake::getNbInstance()) ;
  CheckZeroObjectCount(string("NSLdvDrugCycle"),              NSLdvDrugCycle::getNbInstance()) ;
  CheckZeroObjectCount(string("NSLdvDrugPhase"),              NSLdvDrugPhase::getNbInstance()) ;
  CheckZeroObjectCount(string("NSLdvDrug"),                   NSLdvDrug::getNbInstance()) ;
  CheckZeroObjectCount(string("NSFrameInformation"),          NSFrameInformation::getNbInstance()) ;
  CheckZeroObjectCount(string("NSLdvDocument"),               NSLdvDocument::getNbInstance()) ;

  CheckZeroObjectCount(string("NSLdvGoalInfo"),               NSLdvGoalInfo::getNbInstance()) ;
  CheckZeroObjectCount(string("NSDelayZone"),                 NSDelayZone::getNbInstance()) ;
  CheckZeroObjectCount(string("NSDateZone"),                  NSDateZone::getNbInstance()) ;
  CheckZeroObjectCount(string("NSValueZone"),                 NSValueZone::getNbInstance()) ;
  CheckZeroObjectCount(string("NSLdvGoal"),                   NSLdvGoal::getNbInstance()) ;

  CheckZeroObjectCount(string("NSLdvTimeToon"),               NSLdvTimeToon::getNbInstance()) ;
  CheckZeroObjectCount(string("NSPbModifView"),               NSPbModifView::getNbInstance()) ;
  CheckZeroObjectCount(string("NSLigneView"),                 NSLigneView::getNbInstance()) ;
  CheckZeroObjectCount(string("NSConcernView"),               NSConcernView::getNbInstance()) ;
  CheckZeroObjectCount(string("NSPreoccupView"),              NSPreoccupView::getNbInstance()) ;
  CheckZeroObjectCount(string("NSEventView"),                 NSEventView::getNbInstance()) ;
  CheckZeroObjectCount(string("NSDrugLineView"),              NSDrugLineView::getNbInstance()) ;
  CheckZeroObjectCount(string("NSGoalLineView"),              NSGoalLineView::getNbInstance()) ;
  CheckZeroObjectCount(string("NSBaseLineView"),              NSBaseLineView::getNbInstance()) ;
  CheckZeroObjectCount(string("NSSsObjView"),                 NSSsObjView::getNbInstance()) ;
  CheckZeroObjectCount(string("NSLdvCurvePoint"),             NSLdvCurvePoint::getNbInstance()) ;
  CheckZeroObjectCount(string("NSLdvObjetView"),              NSLdvObjetView::getNbInstance()) ;
  CheckZeroObjectCount(string("NSQualityOfLifeChartView"),    NSQualityOfLifeChartView::getNbInstance()) ;
  CheckZeroObjectCount(string("NSLdvGoalCycleView"),          NSLdvGoalCycleView::getNbInstance()) ;
  CheckZeroObjectCount(string("NSLdvTankView"),               NSLdvTankView::getNbInstance()) ;
  CheckZeroObjectCount(string("NSTimeZoomLevel"),             NSTimeZoomLevel::getNbInstance()) ;
  CheckZeroObjectCount(string("NSLdvView"),                   NSLdvView::getNbInstance()) ;
  CheckZeroObjectCount(string("NSLdvViewArea"),               NSLdvViewArea::getNbInstance()) ;
  CheckZeroObjectCount(string("NSLdvViewAreaWindow"),         NSLdvViewAreaWindow::getNbInstance()) ;

  CheckZeroObjectCount(string("NSLdvToon"),                   NSLdvToon::getNbInstance()) ;

  CheckZeroObjectCount(string("NSLdvWindowToon"),             NSLdvWindowToon::getNbInstance()) ;
  CheckZeroObjectCount(string("NSLdvCommandPannel"),          NSLdvCommandPannel::getNbInstance()) ;
  CheckZeroObjectCount(string("NSLdvCommandPannelInterface"), NSLdvCommandPannelInterface::getNbInstance()) ;
  CheckZeroObjectCount(string("NSLdvChronoLine"),             NSLdvChronoLine::getNbInstance()) ;
  CheckZeroObjectCount(string("NSLdvChronoLineInterface"),    NSLdvChronoLineInterface::getNbInstance()) ;
  CheckZeroObjectCount(string("NSLdvCurveYAxis"),             NSLdvCurveYAxis::getNbInstance()) ;
  CheckZeroObjectCount(string("NSLdvCurveYAxisInterface"),    NSLdvCurveYAxisInterface::getNbInstance()) ;
  CheckZeroObjectCount(string("NSLdvCurveBackgroundQuadri"),  NSLdvCurveBackgroundQuadri::getNbInstance()) ;
  CheckZeroObjectCount(string("NSLdvCurveBackground"),        NSLdvCurveBackground::getNbInstance()) ;
  CheckZeroObjectCount(string("NSCurvesProperty"),            NSCurvesProperty::getNbInstance()) ;
  CheckZeroObjectCount(string("NSCurvesPropertyWindow"),      NSCurvesPropertyWindow::getNbInstance()) ;
  CheckZeroObjectCount(string("NSLdvCurve"),                  NSLdvCurve::getNbInstance()) ;
  CheckZeroObjectCount(string("NSLdvCurvesManagementPannel"), NSLdvCurvesManagementPannel::getNbInstance()) ;
  CheckZeroObjectCount(string("NSLdvCurvesManagementPannelInterface"), NSLdvCurvesManagementPannelInterface::getNbInstance()) ;
  CheckZeroObjectCount(string("NsObjectIcon"),                NsObjectIcon::getNbInstance()) ;
  CheckZeroObjectCount(string("NSLdvViewIconsShowWindow"),    NSLdvViewIconsShowWindow::getNbInstance()) ;

  CheckZeroObjectCount(string("NSCoopCardButton"),            NSCoopCardButton::getNbInstance()) ;
  CheckZeroObjectCount(string("NSCoopCard"),                  NSCoopCard::getNbInstance()) ;
  CheckZeroObjectCount(string("NSCoopLine"),                  NSCoopLine::getNbInstance()) ;
  CheckZeroObjectCount(string("NSQualityOfLifeInformation"),  NSQualityOfLifeInformation::getNbInstance()) ;
  CheckZeroObjectCount(string("NSQualityOfLifeChart"),        NSQualityOfLifeChart::getNbInstance()) ;
  CheckZeroObjectCount(string("NSCoopChart"),                 NSCoopChart::getNbInstance()) ;

  CheckZeroObjectCount(string("NSphaseMedic"),                NSphaseMedic::getNbInstance()) ;
  CheckZeroObjectCount(string("NSMedicCycleGlobal"),          NSMedicCycleGlobal::getNbInstance()) ;
  CheckZeroObjectCount(string("NSCircadien"),                 NSCircadien::getNbInstance()) ;

  CheckZeroObjectCount(string("dkdNoyau"),                    dkdNoyau::getNbInstance()) ;
  CheckZeroObjectCount(string("gereDate"),                    gereDate::getNbInstance()) ;
  CheckZeroObjectCount(string("numStorage"),                  numStorage::getNbInstance()) ;
  CheckZeroObjectCount(string("gereNum"),                     gereNum::getNbInstance()) ;
  CheckZeroObjectCount(string("gereHeure"),                   gereHeure::getNbInstance()) ;
  CheckZeroObjectCount(string("gereCode"),                    gereCode::getNbInstance()) ;
  CheckZeroObjectCount(string("NSDkdPhrase"),                 NSDkdPhrase::getNbInstance()) ;
  CheckZeroObjectCount(string("decodageBase"),                decodageBase::getNbInstance()) ;
  CheckZeroObjectCount(string("NSGenerateur"),                NSGenerateur::getNbInstance()) ;
  CheckZeroObjectCount(string("NSGenComplement"),             NSGenComplement::getNbInstance()) ;

  CheckZeroObjectCount(string("NSChoixPubli"),                NSChoixPubli::getNbInstance()) ;
  CheckZeroObjectCount(string("NSMapiCtrl"),                  NSMapiCtrl::getNbInstance()) ;
  CheckZeroObjectCount(string("NSLdvPubli"),                  NSLdvPubli::getNbInstance()) ;
  CheckZeroObjectCount(string("NSPublication"),               NSPublication::getNbInstance()) ;

  CheckZeroObjectCount(string("NSDocCompta"),                 NSDocCompta::getNbInstance()) ;

  CheckZeroObjectCount(string("NSPhraseObjet"),               NSPhraseObjet::getNbInstance()) ;
  CheckZeroObjectCount(string("NSPhraseMot"),                 NSPhraseMot::getNbInstance()) ;
  CheckZeroObjectCount(string("NSPhraseur"),                  NSPhraseur::getNbInstance()) ;
  CheckZeroObjectCount(string("NsProposition"),               NsProposition::getNbInstance()) ;

  CheckZeroObjectCount(string("MappingNSSearchResult"),       MappingNSSearchResult::getNbInstance()) ;
  CheckZeroObjectCount(string("NSSearchStruct"),              NSSearchStruct::getNbInstance()) ;
  CheckZeroObjectCount(string("NSRequestResult"),             NSRequestResult::getNbInstance()) ;

  CheckZeroObjectCount(string("NSLocalisedChapter"),          NSLocalisedChapter::getNbInstance()) ;

  CheckZeroObjectCount(string("nsGenericParseur"),            nsGenericParseur::getGenericNbInstance()) ;
  CheckZeroObjectCount(string("nsarcParseur"),                nsarcParseur::getNbInstance()) ;
  CheckZeroObjectCount(string("nsrefParseur"),                nsrefParseur::getNbInstance()) ;
  CheckZeroObjectCount(string("nsGuidelineParseur"),          nsGuidelineParseur::getNbInstance()) ;
  CheckZeroObjectCount(string("NSValidateur"),                NSValidateur::getNbInstance()) ;
  CheckZeroObjectCount(string("NSDrufRefFileManager"),        NSDrufRefFileManager::getNbInstance()) ;

  CheckZeroObjectCount(string("NSCsvField"),                  NSCsvField::getNbInstance()) ;
  CheckZeroObjectCount(string("NSCsvRecord"),                 NSCsvRecord::getNbInstance()) ;
  CheckZeroObjectCount(string("NSCsvParser"),                 NSCsvParser::getNbInstance()) ;

  CheckZeroObjectCount(string("NSCapture"),                   NSCapture::getNbInstance()) ;
  CheckZeroObjectCount(string("NSCaptureArray"),              NSCaptureArray::getNbInstance()) ;
  CheckZeroObjectCount(string("analysedCapture"),             analysedCapture::getNbInstance()) ;
}

// -----------------------------------------------------------------------------
// Fonction     : NSSuper::estEgal(string *pChaine, string *pModele)
// -----------------------------------------------------------------------------
// Description  : Teste l'équivalence des deux chaines.
// -----------------------------------------------------------------------------
// Retour       :	true si les chaines sont équivalentes, false sinon
// -----------------------------------------------------------------------------
bool
NSSuper::estEgal(string* pChaine, string* pModele)
{
  // Cas simple : égalité stricte
  if (*pChaine == *pModele)
    return true ;

  // On compare les deux chaines de gauche à droite, jusqu'à trouver
  // un élément du modèle qui ne se ramène pas à un élément de la chaine
  // par une relation "est un"
  string Chaine = *pChaine ;
  string Modele = *pModele ;

  // On "simplifie" les chaines en supprimant les redondances
  // (par exemple "ETT/Valve cardiaque/Mitrale" devient "ETT/Mitrale")
	return false ;
}

// -----------------------------------------------------------------------------
// Function     : NSSuper::InitParite()
// -----------------------------------------------------------------------------
// Arguments    : Aucun
// -----------------------------------------------------------------------------
// Description  : Initialise la parite Franc / Euro d'apres Euro.dat
// -----------------------------------------------------------------------------
// Returns      : True : Succès, False : Echec
// -----------------------------------------------------------------------------
bool
NSSuper::InitParite()
{
	// Attention : ne pas passer le Handle de la MainWindow aux fonctions erreur
	// dans InitParite car à ce stade, la MainWindow n'est pas encore instanciée

  string sMsg = string("Opening startup parameters (euro.dat)") ;
  trace(&sMsg, 1, NSSuper::trSteps) ;

  // Opening file
  //
  ifstream inFile ;
	inFile.open("euro.dat");
	if (!inFile)
	{
    sMsg = string("Cannot open euro.dat") ;
    trace(&sMsg, 1, NSSuper::trError) ;

		erreur("Erreur d'ouverture du fichier euro.dat.", standardError, 0) ;
    return false ;
	}

  // Loading data
  //
  string sData = string("") ;
	while (!inFile.eof())
	{
    string line = string("") ;
  	getline(inFile, line) ;
    if (string("") != line)
    	sData += line + string("\n") ;
	}

	inFile.close() ;

  size_t iDataLen = strlen(sData.c_str()) ;

  size_t i = 0 ;
	// boucle de chargement des attributs de compta
  //
	while (i < iDataLen)
	{
  	string sNomAttrib = string("") ;
    string sValAttrib = string("") ;

    while ((i < iDataLen) && (' ' != sData[i]) && ('\t' != sData[i]))
    	sNomAttrib += pseumaj(sData[i++]) ;

    while ((i < iDataLen) && ((' ' == sData[i]) || ('\t' == sData[i])))
    	i++ ;

    while ((i < iDataLen) && ('\n' != sData[i]))
    	sValAttrib += sData[i++] ;

    i++ ;

    if 		((string("PARITE") == sNomAttrib) && (string("") != sValAttrib))
    {
    	// Parité monnaie locale / euros
      _parite = atof(sValAttrib.c_str()) ;

      if (0 == _parite)
      {
      	erreur("La parité Franc / Euro n'est pas initialisée par euro.dat", standardError, 0) ;
        _parite = 6.5596 ;
      }
    }
    else if ((string("MONNAIE") == sNomAttrib) && (string("") != sValAttrib))
    {
    	if (sValAttrib == "EURO")
      	_monnaieRef = MONNAIE_EURO ;
      else
      	_monnaieRef = MONNAIE_LOCALE ;
    }
    else if ((string("SIGLE") == sNomAttrib) && (string("") != sValAttrib))
    {
    	// sigle de la monnaie locale
      _sigle = sValAttrib ;
    }
    else if ((string("AUTO") == sNomAttrib) && (string("") != sValAttrib))
    {
    	// compta automatique
      if (IsYes(sValAttrib))
      	_bComptaAuto = true ;
      else
      	_bComptaAuto = false ;
    }
    else if ((string("NOCCAMCR") == sNomAttrib) && (string("") != sValAttrib))
    {
    	// pas de CCAM pour les comptes rendus
      if (IsYes(sValAttrib))
      	_bNoCCAMforReport = true ;
      else
      	_bNoCCAMforReport = false ;
    }
    else if ((string("NOCCAMCS") == sNomAttrib) && (string("") != sValAttrib))
    {
    	// pas de CCAM pour les consultations
      if (IsYes(sValAttrib))
      	_bNoCCAMforExam = true ;
      else
      	_bNoCCAMforExam = false ;
    }
    else if ((string("INDICE") == sNomAttrib) && (string("") != sValAttrib))
    {
    	// indice de consultation
      _indiceConsult = sValAttrib ;
    }
    else if ((string("EXPORT") == sNomAttrib) && (string("") != sValAttrib))
    {
    	// dll d'exportation automatique à la sauvegarde
      _sExport_dll = sValAttrib ;

      sMsg = string("Export dll: ") + _sExport_dll ;
      trace(&sMsg, 1, NSSuper::trSubSteps) ;
    }
    else if ((string("EXPORT_CPTA") == sNomAttrib) && (string("") != sValAttrib))
    {
    	// dll d'exportation automatique à la sauvegarde
      _sAccountingExport_dll = sValAttrib ;

      sMsg = string("Export dll for accounting: ") + _sAccountingExport_dll ;
      trace(&sMsg, 1, NSSuper::trSubSteps) ;
    }
    else if ((string("STAYS_SYNCHRO") == sNomAttrib) && (string("") != sValAttrib))
    {
      _sStaysSynchro_dll = sValAttrib ;
    }
    else if ((string("CODEAUTO") == sNomAttrib) && (string("") != sValAttrib))
    {
    	// compta automatique
      if (IsYes(sValAttrib))
      	_bCodageAuto = true ;
      else
      	_bCodageAuto = false ;
    }
    else if ((string("USEIEFORPDF") == sNomAttrib) && (string("") != sValAttrib))
    {
      if (IsNo(sValAttrib))
      	_bUseIeForPdf = false ;
      else
      	_bUseIeForPdf = true ;
    }
    else if ((string("MUSTKILLPDF") == sNomAttrib) && (string("") != sValAttrib))
    {
      if (IsNo(sValAttrib))
        _bMustKillPdfProcess = false ;
      else
        _bMustKillPdfProcess = true ;
    }
    else if ((string("NOM_MODULE") == sNomAttrib) && (string("") != sValAttrib))
    {
    	// nom officiel du module
      _sNomModule = sValAttrib ;
    }
    else if ((string("DIFFUSEUR") == sNomAttrib) && (string("") != sValAttrib))
    {
    	// nom officiel du module
      _sDiffuseur = sValAttrib ;
    }
    else if ((string("URL_DIFFUSEUR") == sNomAttrib) && (string("") != sValAttrib))
    {
    	// nom officiel du module
      _sURLDiffuseur = sValAttrib ;
    }
    else if ((string("IPP_SITE") == sNomAttrib) && (string("") != sValAttrib))
    {
    	// reference site for Patient's permanent identifier
      _sIppSite = sValAttrib ;

      sMsg = string("Refered site for IPP: ") + _sIppSite ;
      trace(&sMsg, 1, NSSuper::trSubSteps) ;
    }
    else if ((string("OPERATION_SITE") == sNomAttrib) && (string("") != sValAttrib))
    {
    	// operational site code
      _sOperationalSite = sValAttrib ;

      sMsg = string("Operational site code: ") + _sOperationalSite ;
      trace(&sMsg, 1, NSSuper::trSubSteps) ;
    }
    else if ((string("DEMOGRAPHIC") == sNomAttrib) && (string("") != sValAttrib))
    {
    	// demographic archetype
      _sDemographicArchetypeId = sValAttrib ;

      sMsg = string("Demographic archetype: ") + _sDemographicArchetypeId ;
      trace(&sMsg, 1, NSSuper::trSubSteps) ;
    }
    else if ((string("FCT_UNIT") == sNomAttrib) && (string("") != sValAttrib))
    {
    	// Fonctionnal unit
      _sDefaultFctUnitId = sValAttrib ;

      sMsg = string("Functional unit for this unit: ") + _sDefaultFctUnitId ;
      trace(&sMsg, 1, NSSuper::trSubSteps) ;
    }
    else if ((string("CAREPLACE_ID") == sNomAttrib) && (string("") != sValAttrib))
    {
    	// reference site for Patien's permanent identifier
      _sCarePlaceId = sValAttrib ;

      sMsg = string("Care place ID: ") + _sCarePlaceId ;
      trace(&sMsg, 1, NSSuper::trSubSteps) ;
    }
    else if ((string("INDEX_MGMT") == sNomAttrib) && (string("") != sValAttrib))
    {
      sValAttrib = pseumaj(sValAttrib) ;
    	if      (string("SINGLETREE") == sValAttrib)
      	_iIndexManagementRule = umSingleTree ;
      else if ((string("AUTO") == sValAttrib) || (string("AUTOMATIC") == sValAttrib))
      	_iIndexManagementRule = umAuto ;
      else if (string("MULTITREES") == sValAttrib)
      	_iIndexManagementRule = umMultiTrees ;
    }
    else if ((string("IN_PATIENTS") == sNomAttrib) && (string("") != sValAttrib))
    {
    	// complete file name for In Patien's file description
      _sInPatientsFileDesc = sValAttrib ;

      sMsg = string("In Patients file: ") + _sInPatientsFileDesc ;
      trace(&sMsg, 1, NSSuper::trSubSteps) ;
    }
    //
    // User functions modules
    //
    else if ((string("USER_MODULE_1") == sNomAttrib) && (string("") != sValAttrib))
      _sUserFctDll[0] = sValAttrib ;
    else if ((string("USER_MODULE_2") == sNomAttrib) && (string("") != sValAttrib))
      _sUserFctDll[1] = sValAttrib ;
    else if ((string("USER_MODULE_3") == sNomAttrib) && (string("") != sValAttrib))
      _sUserFctDll[2] = sValAttrib ;
    else if ((string("USER_MODULE_4") == sNomAttrib) && (string("") != sValAttrib))
      _sUserFctDll[3] = sValAttrib ;
    else if ((string("USER_MODULE_5") == sNomAttrib) && (string("") != sValAttrib))
      _sUserFctDll[4] = sValAttrib ;
    else if ((string("USER_MODULE_6") == sNomAttrib) && (string("") != sValAttrib))
      _sUserFctDll[5] = sValAttrib ;
    else if ((string("USER_MODULE_7") == sNomAttrib) && (string("") != sValAttrib))
      _sUserFctDll[6] = sValAttrib ;
    else if ((string("USER_MODULE_8") == sNomAttrib) && (string("") != sValAttrib))
      _sUserFctDll[7] = sValAttrib ;
    else if ((string("USER_MODULE_9") == sNomAttrib) && (string("") != sValAttrib))
      _sUserFctDll[8] = sValAttrib ;
	}
	return true ;
}

//---------------------------------------------------------------------------------------//  Function: NSSuper::InitPathBase()
//  Arguments:	  Aucun
//  Description: Initialise le tableau sPathBase avec les chemins d'accès aux bases
//  Returns:     True : Succès, False : Echec
//---------------------------------------------------------------------------------------
bool
NSSuper::InitPathBase(NSContexte* pCtx)
{
try
{
  //
  // First, make certain that error/warning messages won't be hidden by the
  // splash screen
  //
  HWND hWnd = 0 ;
  if (_pNSApplication)
  {
	  TSplashWindow* pSplashWin = _pNSApplication->getSplashWindow() ;
	  if (pSplashWin)
  	  hWnd = pSplashWin->GetHandle() ;
	  _pNSApplication->sendMessageToSplash("Reading chemins.dat") ;    _pNSApplication->setSplashPercentDone(10) ;  }

	// Setting default values
  //
  _iTrace        = trWarning ;
  _iTraceConsole = trNone ;
  _iTraceBBK     = trNone ;

	// Attention : ne pas passer le Handle de la MainWindow aux fonctions erreur
	// dans InitPathBase car à ce stade, la MainWindow n'est pas encore instanciée

	// On initalise à vide sPathBase
	for (int k = 0 ; k < NB_CHEMINS_BASES ; k++)		_sPathBase[k] = string("") ;

	// On récupère d'abord le chemin de la base des chemins
	// NOTE : chemins.dat est obligatoirement à la racine
	// --- on ne doit pas préciser de chemin pour ce fichier ---
  //
  string sCheminsFile = string("chemins.dat") ;

  if (false == NsFileExists(sCheminsFile))
  {
    string sErreur = getText("fileErrors", "missingFile") + string(" ") + sCheminsFile ;
    trace(&sErreur, 1, NSSuper::trError) ;
    erreur(sErreur.c_str(), standardError, 0, hWnd) ;
    return false ;
  }

  ifstream inFile ;
  inFile.open(sCheminsFile.c_str()) ;
  if (!inFile)
  {
    string sErreur = getText("fileErrors", "errorOpeningInputFile") ;
    sErreur += string(" ") + sCheminsFile ;
    trace(&sErreur, 1, NSSuper::trError) ;
    erreur(sErreur.c_str(), standardError, 0, hWnd) ;
    return false ;
	}

  string sFichierDat = string("") ;

	while (!inFile.eof())
	{
    string sLine = string("") ;

  	getline(inFile, sLine) ;

    if (string("") != sLine)
    	sFichierDat += sLine + "\n" ;
	}
	inFile.close() ;

  size_t i ;

	// First line: numéro de console
  //
  string sConsole = string("") ;
	for (i = 0 ; (i < strlen(sFichierDat.c_str())) && ('\n' != sFichierDat[i]) ; i++)
		sConsole += sFichierDat[i] ;

	if (strlen(sConsole.c_str()) == 2)
	{
  	strcpy(_noConsole, sConsole.c_str()) ;
    //
    // Numéro de console flottant
    //
    if ('?' == sConsole[0])
    {
    	char chConsole = _noConsole[1] ;
      chConsole++ ;
      if (chConsole < '0')
      	chConsole = '0' ;
      else if ((chConsole > '9') && (chConsole < 'A'))
      	chConsole = 'A' ;
      else if (chConsole > 'Z')
      	chConsole = '0' ;

      _noConsole[1] = chConsole ;

      ofstream outFile ;
      outFile.open("chemins.dat", ios::out) ;
      if (!outFile)
      {
        string sErreur = getText("fileErrors", "errorOpeningOutputFile") + string(" ") + sCheminsFile ;
        trace(&sErreur, 1, NSSuper::trError) ;
        erreur(sErreur.c_str(), standardError, 0, hWnd) ;
      }
      else
      {
        sFichierDat[1] = chConsole ;
      	outFile.write(sFichierDat.c_str(), strlen(sFichierDat.c_str())) ;
        outFile.close() ;
      }

      string sMsg = string("Dynamic console ID set to ") + string(_noConsole) ;
      trace(&sMsg, 1, trWarning) ;
    }
    else
    {
      string sMsg = string("Static console ID: ") + string(_noConsole) ;
      trace(&sMsg, 1, trWarning) ;
    }
  }
  // Incremental console id with centralized file
  //
  else if ((string(sConsole, 0, 2) == string("??")) && (strlen(sConsole.c_str()) > 3))
  {
    string sIdFileName = string(sConsole, 3, strlen(sConsole.c_str()) - 3) ;

    if (false == NsFileExists(sIdFileName))
		{
      string sErreur = getText("fileErrors", "missingFile") + string(" ") + sIdFileName ;
      trace(&sErreur, 1, NSSuper::trError) ;
      erreur(sErreur.c_str(), standardError, 0, hWnd) ;
    }

    bool   bLookingForId  = true ;
    size_t iAttemptNumber = 0 ;

    NVLdVTemps tLimit ;
    tLimit.takeTime() ;
    tLimit.ajouteSecondes(2) ;

    while (bLookingForId)
    {
      ifstream externalIdFile(sIdFileName.c_str(), ofstream::in) ;
      if (!externalIdFile)
      {
        iAttemptNumber++ ;

        NVLdVTemps tNow ;
        tNow.takeTime() ;

        if (tNow > tLimit)
        {
          string sErreur = getText("fileErrors", "errorOpeningFile") ;
          sErreur += string(" ") + sIdFileName ;
          trace(&sErreur, 1, NSSuper::trError) ;
          erreur(sErreur.c_str(), standardError, 0, hWnd) ;
          return false ;
        }

        if (_pNSApplication)
          _pNSApplication->PumpWaitingMessages() ;
      }
      else
      {
        bLookingForId = false ;

        string sIdLine = string("") ;

        getline(externalIdFile, sIdLine) ;

        externalIdFile.close() ;

        if (strlen(sIdLine.c_str()) != 2)
        {
          string sErreur = getText("paramsManagement", "badConsoleIdentifier") ;
          trace(&sErreur, 1, NSSuper::trError) ;
          erreur(sErreur.c_str(), standardError, 0, hWnd) ;
          return false ;
        }

        strcpy(_noConsole, sIdLine.c_str()) ;

        char chConsole = _noConsole[1] ;
        chConsole++ ;
        if (chConsole < '0')
      	  chConsole = '0' ;
        else if ((chConsole > '9') && (chConsole < 'A'))
      	  chConsole = 'A' ;
        else if (chConsole > 'Z')
        {
      	  chConsole = '0' ;

          char chConsoTen = _noConsole[0] ;
          chConsoTen++ ;
          if (chConsoTen < '0')
      	    chConsoTen = '0' ;
          else if ((chConsoTen > '9') && (chConsoTen < 'A'))
      	    chConsoTen = 'A' ;
          else if (chConsoTen > 'Z')
            chConsoTen = '0' ;

          _noConsole[0] = chConsoTen ;
        }

        _noConsole[1] = chConsole ;

        sIdLine = string(_noConsole) ;

        ofstream outFile ;
        outFile.open(sIdFileName.c_str(), ios::out) ;
        if (!outFile)
        {
          string sErreur = getText("fileErrors", "errorOpeningOutputFile") ;
          sErreur += string(" ") + sIdFileName ;
          trace(&sErreur, 1, NSSuper::trError) ;
          erreur(sErreur.c_str(), standardError, 0, hWnd) ;
        }
        else
        {
      	  outFile.write(sIdLine.c_str(), strlen(sIdLine.c_str())) ;
          outFile.close() ;
        }

        string sMsg = string("Dynamic console ID set to ") + string(_noConsole) + string(" from file ") + sIdFileName ;
        trace(&sMsg, 1, trWarning) ;
      }
    }
  }
  else
  {
    string sErreur = getText("paramsManagement", "badConsoleIdentifier") ;
    trace(&sErreur, 1, NSSuper::trError) ;
    erreur(sErreur.c_str(), standardError, 0, hWnd) ;
    return false ;
	}

	// chemin de la base des chemins
	for (i = i+1 ; (i < strlen(sFichierDat.c_str())) && ('\n' != sFichierDat[i]) ; i++)
  	_sPathBase[0] += sFichierDat[i] ;

	if (strlen(_sPathBase[0].c_str()) == 0)
  {
    string sErreur = getText("paramsManagement", "pathIsMissingInChemins.dat") ;
    trace(&sErreur, 1, NSSuper::trError) ;
    erreur(sErreur.c_str(), standardError, 0, hWnd) ;
    return false ;
	}

  string sMsg = string("Path for params set to ") + _sPathBase[0] ;
  trace(&sMsg, 1, trWarning) ;

  // Other parameters, defined by their first letter
  //
  bool bOtherParametersExist = true ;
  while (bOtherParametersExist)
  {
    string sParamerString = string("") ;
    for (i = i + 1 ; (i < strlen(sFichierDat.c_str())) && ('\n' != sFichierDat[i]); i++)
  	  sParamerString += sFichierDat[i] ;

    if (i >= strlen(sFichierDat.c_str()))
      bOtherParametersExist = false ;

    if (string("") != sParamerString)
    {
      size_t iParamsLength = strlen(sParamerString.c_str()) ;

      // Trace level
      //
      if (('T' == sParamerString[0]) || ('t' == sParamerString[0]))
	    {
        if ((iParamsLength > 1) && (isdigit(sParamerString[iParamsLength-1])))
        {
          string sTraceLevel = string(1, sParamerString[iParamsLength-1]) ;
          _iTrace = getTraceLevelFromString(sTraceLevel) ;
        }

        sMsg = string("Trace level: ") + getTraceLevelLabel(_iTrace) ;
        trace(&sMsg, 1, _iTrace) ;
      }
      // Console level
      //
      else if (('C' == sParamerString[0]) || ('c' == sParamerString[0]))
	    {
        if ((iParamsLength > 1) && (isdigit(sParamerString[iParamsLength-1])))
        {
          string sTraceLevel = string(1, sParamerString[iParamsLength-1]) ;
          _iTraceConsole = getTraceLevelFromString(sTraceLevel) ;

          if (trNone != _iTraceConsole)
  	        StartDebugConsole(80, 20, "") ;
        }

        sMsg = string("Console level: ") + getTraceLevelLabel(_iTraceConsole) ;
        trace(&sMsg, 1, _iTrace) ;
      }
      // BBK trace level
      //
      else if (('B' == sParamerString[0]) || ('b' == sParamerString[0]))
	    {
        if ((iParamsLength > 1) && (isdigit(sParamerString[iParamsLength-1])))
        {
          string sTraceLevel = string(1, sParamerString[iParamsLength-1]) ;
          _iTraceBBK = getTraceLevelFromString(sTraceLevel) ;
        }

        sMsg = string("BBK trace level: ") + getTraceLevelLabel(_iTraceBBK) ;
        trace(&sMsg, 1, _iTrace) ;
      }
      // Group mode
      //
      else if (('G' == sParamerString[0]) || ('I' == sParamerString[0]))
	    {
  	    _bModeClientGroup    = true ;
        if (sParamerString[0] == 'I')
    	    _bGroupInterfaceOnly = true ;
        else
    	    _bGroupInterfaceOnly = false ;
	    }
      // Slave mode: automatic login and patient creation/opening (and refreshening)
      //
      else if (('S' == sParamerString[0]) || ('s' == sParamerString[0]))
	    {
  	    _bSlaveMode = true ;

        if (strlen(sParamerString.c_str()) > 2)
          _sSlaveDllName = string(sParamerString, 2, strlen(sParamerString.c_str()) - 2) ;

        string sMsg = string("Switching to slave mode with dll ") + _sSlaveDllName ;
        trace(&sMsg, 1, trWarning) ;
	    }
    }
  }

  string sCodeChemin[NB_CHEMINS_BASES - 1] = {"BGLO","BPAR","BPER","BCPT","BGUI"} ;
  InitialiseFileDatabase() ;
  for (int j = 1; j < NB_CHEMINS_BASES; j++)
  {
  	std::string key     = std::string(sCodeChemin[j-1]) ;
    std::string support = _PathDB->getLocalText(key, std::string("$SUPPORT")) ;
    std::string chemin  = _PathDB->getLocalText(key, std::string("$CHEMIN")) ;
    if (string("") != support)
    {
    	NSSupportsInfo SuppInfo ;
      if (chercheSupportsInfo(support, &SuppInfo))
      	_sPathBase[j] = SuppInfo.getRootPath() ;
      _sPathBase[j] += chemin ;
    }
  }

	_paSkins->init(pCtx->PathName("FSKN", 0, false /* verbose */)) ;

	return true ;
}
catch (...)
{
  erreur("Exception NSSuper::InitPathBase.", standardError, 0) ;
  return NULL ;
}
}

NSSuper::TRACETYPE
NSSuper::getTraceLevelFromString(string sTraceLevel)
{
  strip(sTraceLevel, stripBoth) ;

  if (string("") == sTraceLevel)
    return trNone ;

  int iLevel = atoi(sTraceLevel.c_str()) ;
  switch(iLevel)
  {
    case 0  : return trError ;
    case 1  : return trWarning ;
    case 2  : return trSteps ;
    case 3  : return trSubSteps ;
    case 4  : return trDetails ;
    case 5  : return trSubDetails ;
    case 9  : return trDebug ;
  }

  if (iLevel > 5)
    return trSubDetails ;

  return trNone ;
}

string
NSSuper::getTraceLevelLabel(TRACETYPE iTraceLevel)
{
  switch(iTraceLevel)
  {
    case trError      : return string("Errors only") ;
    case trWarning    : return string("Errors and Warnings only") ;
    case trSteps      : return string("Main steps") ;
    case trSubSteps   : return string("Sub-steps") ;
    case trDetails    : return string("Main details") ;
    case trSubDetails : return string("Sub-details") ;
    case trDebug      : return string("Debug") ;
  }

  return string("undefined") ;
}

//---------------------------------------------------------------------------------------
//  Function: NSSuper::PathNameType(string sTypeFichier, string& sLocalis, string& sUnite, string& sServeur)
//  Arguments:	  sTypeFichier : Type de fichier dont on veut trouver le chemin
//  Description: Retrouve le chemin correspondant au type + les infos de support
//  Returns:     Une string contenant le chemin RELATIF
//---------------------------------------------------------------------------------------
string
NSSuper::PathNameType(string sTypeFichier, string& sLocalis, string& sUnite, string& sServeur, NSContexte* pCtx)
{
try
{
	if (_PathDB->empty() )
  	return string("") ;

	for (NSLocalChapterArrayIter iter = _PathDB->begin() ; _PathDB->end() != iter ; iter++)
  	if ((*iter).second->getLocalText("$TYPE_DOC") == sTypeFichier)
    {
    	std::string support =  (*iter).second->getLocalText("$SUPPORT") ;
      NSSupportsInfo SuppInfo ;
      if (false == chercheSupportsInfo(support, &SuppInfo))
      {
      	erreur("echec de la fonction PathNameType", standardError, 0) ;
        return string("") ;
      }
      sServeur = SuppInfo.getRootPath() ;
      sUnite	 = string("") ;
      std::string sPath = (*iter).second->getLocalText("$CHEMIN") ;
      sLocalis =(*iter).first ;
      return sPath ;
    }

	return string("") ;
}
catch (...)
{
	erreur("Exception NSSuper::PathNameType.", standardError, 0) ;
	return string("") ;
}
}

//---------------------------------------------------------------------------------------
//  Function: NSSuper::ReferenceTousArchetypes()
//  Description: Reference automatiquement les archetypes et les referentiels
//  Returns:     void
//---------------------------------------------------------------------------------------
void
NSSuper::ReferenceTousArchetypes()
{
try
{
	string ps = string("New Archetypes management - begin") ;
	trace(&ps, 1, trDetails) ;	string sSysImportPath = _pSuperContext->PathName("INEW") ;
  if (string("") == sSysImportPath)
  {
  	ps = string("Can't find SysImport Path - exiting new Archetypes management") ;
		trace(&ps, 1, trWarning) ;
    return ;
  }

  string sPathName = _pSuperContext->PathName("IXML") ;
  if (string("") == sPathName)
  {
  	ps = string("Can't find Archetypes Path - exiting new Archetypes management") ;
		trace(&ps, 1, trWarning) ;
    return ;
  }

  string sStatusMsg = getText("archetypesManagement", "automaticReferencing") ;  afficheStatusMessage((char*) sStatusMsg.c_str()) ;  string sStatusBase = getText("archetypesManagement", "referencingOf") ;  string sStatusString = "" ;  // Recherche des fichier .xml dans SysImport  // Looking for xml files in SysImport directory  char szMask[1024] ;  strcpy(szMask, sSysImportPath.c_str()) ;  strcat(szMask, "*.xml") ;
  WIN32_FIND_DATA FileData ;
  HANDLE hSearch = FindFirstFile(szMask, &FileData) ;
  if (INVALID_HANDLE_VALUE == hSearch)  {    sStatusMsg = getText("archetypesManagement", "noNewArchetype") ;    afficheStatusMessage((char*) sStatusMsg.c_str()) ;    ps = string("No new Archetype - exiting") ;		trace(&ps, 1, trSubDetails) ;    return ;  }
  _pSuperContext->GetMainWindow()->SetCursor(0, IDC_WAIT) ;

  bool bFinish = false ;
  while (false == bFinish)
  {    DWORD dwAttr = FileData.dwFileAttributes;
    if (!(dwAttr & FILE_ATTRIBUTE_DIRECTORY))
    {      string                sFileName = string(FileData.cFileName) ;      string                sXML = sSysImportPath + sFileName ;      string                sNomArc ;      NSArcManager::TYPEARC	iTypeArc ;      ps = string("File : ") + sFileName ;      trace(&ps, 1, trSubDetails) ;      if (false == _pArcManager->ExisteFichierArchetype(sFileName, sPathName, sNomArc, iTypeArc))      {      	nsarcParseur ArcParseur(_pSuperContext->getSuperviseur(), false) ;        if (ArcParseur.open(sXML))        {
          sStatusString = sStatusBase + sXML ;
          afficheStatusMessage((char*) sStatusString.c_str()) ;

          ps = string("- Referencing as Archetype with ID : ") + ArcParseur.getArchetype()->getName() ;
      		trace(&ps, 1, trSubDetails) ;

          _pArcManager->AjouteArchetype(_pSuperContext, NSArcManager::archetype, ArcParseur.getArchetype()->getName(), sFileName, sPathName) ;

          ps = string("- Moving ") + sXML + string(" to ") + sPathName + sFileName ;
      		trace(&ps, 1, trSubDetails) ;

          if (0 == ::MoveFile(sXML.c_str(), (sPathName + sFileName).c_str()))
          {
            string sErrorText = getText("fileErrors", "errorMovingFile") ;
            sErrorText += string(" ") + sXML + string(" -> ") + sPathName + sFileName ;

            string sSystemLastErrorMessage = getSystemLastErrorMessage() ;
			      if (string("") != sSystemLastErrorMessage)
      	      sErrorText += string(" (") + sSystemLastErrorMessage + string(")") ;

			      trace(&sErrorText, 1, NSSuper::trError) ;
			      erreur(sErrorText.c_str(), warningError, 0, 0) ;
          }
        }
        else
        {
        	nsrefParseur RefParseur(_pSuperContext->getSuperviseur(), false) ;
         	if ((true == RefParseur.open(sXML)) && (NULL != RefParseur.getReferentiel()))
        	{
          	sStatusString = sStatusBase + sXML ;
          	afficheStatusMessage((char*) sStatusString.c_str()) ;

            string sTypeName   = string("") ;
            string sDomainName = string("") ;
            string sFrame      = string("") ;
            Creferences* pRefRef = RefParseur.getReferentiel()->getReference() ;
            if (pRefRef)
            {
            	Cconcern *pConcern = pRefRef->getFirstCconcern() ;
              if (pConcern)
              {
              	sTypeName   = pConcern->getCode() ;
                sDomainName = pConcern->getCategory() ;
                sFrame      = pConcern->getFrame() ;
              }
            }

            string sTitle = RefParseur.getReferentiel()->getTitle() ;

            ps = string("- Referencing as Referential with ID : ") + RefParseur.getReferentiel()->getName() ;
      			trace(&ps, 1, trSubDetails) ;

          	_pArcManager->AjouteArchetype(_pSuperContext, NSArcManager::referentiel, RefParseur.getReferentiel()->getName(), sFileName, sPathName, sTypeName, sDomainName, sTitle, sFrame) ;
          	if (0 == MoveFile(sXML.c_str(), (sPathName + sFileName).c_str()))
            {
              string sErrorText = getText("fileErrors", "errorMovingFile") ;
              sErrorText += string(" ") + sXML + string(" -> ") + sPathName + sFileName ;

              string sSystemLastErrorMessage = getSystemLastErrorMessage() ;
			        if (string("") != sSystemLastErrorMessage)
      	        sErrorText += string(" (") + sSystemLastErrorMessage + string(")") ;

			        trace(&sErrorText, 1, NSSuper::trError) ;
			        erreur(sErrorText.c_str(), warningError, 0, 0) ;
            }
          }
          else
          {
            nsGuidelineParseur guidelineParseur(_pSuperContext->getSuperviseur(), false) ;
         	  if ((true == guidelineParseur.open(sXML)) && (NULL != guidelineParseur.getGuideline()))
        	  {
          	  sStatusString = sStatusBase + sXML ;
          	  afficheStatusMessage((char*) sStatusString.c_str()) ;

              string sTypeName   = string("") ;
              string sDomainName = string("") ;
              string sFrame      = string("") ;
              Creferences* pRefRef = guidelineParseur.getGuideline()->getReference() ;
              if (pRefRef)
              {
            	  Cconcern *pConcern = pRefRef->getFirstCconcern() ;
                if (pConcern)
                {
              	  sTypeName   = pConcern->getCode() ;
                  sDomainName = pConcern->getCategory() ;
                  sFrame      = pConcern->getFrame() ;
                }
              }

              string sTitle = guidelineParseur.getGuideline()->getTitle() ;

              ps = string("- Referencing as Guideline with ID : ") + guidelineParseur.getGuideline()->getName() ;
      			  trace(&ps, 1, trSubDetails) ;

          	  _pArcManager->AjouteArchetype(_pSuperContext, NSArcManager::decisionTree, guidelineParseur.getGuideline()->getName(), sFileName, sPathName, sTypeName, sDomainName, sTitle, sFrame) ;
          	  if (0 == MoveFile(sXML.c_str(), (sPathName + sFileName).c_str()))
              {
                string sErrorText = getText("fileErrors", "errorMovingFile") ;
                sErrorText += string(" ") + sXML + string(" -> ") + sPathName + sFileName ;

                string sSystemLastErrorMessage = getSystemLastErrorMessage() ;
			          if (string("") != sSystemLastErrorMessage)
      	          sErrorText += string(" (") + sSystemLastErrorMessage + string(")") ;

			          trace(&sErrorText, 1, NSSuper::trError) ;
			          erreur(sErrorText.c_str(), warningError, 0, 0) ;
              }
            }
            else
            {
          	  ps = string("- Neither an Archetype, a Referential or a Guideline... or one that doesn't parse") ;
      			  trace(&ps, 1, trSubDetails) ;
            }
          }
        }      }      else      {      	ps = string("- Already there : ignored (you might take it out)") ;        trace(&ps, 1, trSubDetails) ;      }    }
    if (!FindNextFile(hSearch, &FileData))    {      if (GetLastError() != ERROR_NO_MORE_FILES)      {        string sSystemLastErrorMessage = getSystemLastErrorMessage() ;
      	ps = string("FindNextFile error - exiting") ;				trace(&ps, 1, trError) ;        sStatusMsg = getText("fileErrors", "cantGetNextFile") + string(" ") + sSysImportPath ;        if (string("") != sSystemLastErrorMessage)          sStatusMsg += string(" (") + sSystemLastErrorMessage + string(")") ;        trace(&sStatusMsg, 1, NSSuper::trError) ;        erreur(sStatusMsg.c_str(), standardError, 0) ;        return ;      }      bFinish = true ;    }  }

  ps = string("New Archetypes management - end") ;
	trace(&ps, 1, trDetails) ;
  afficheStatusMessage("") ;
  _pSuperContext->GetMainWindow()->SetCursor(0, IDC_ARROW);
}
catch (...)
{
  erreur("Exception NSSuper::ReferenceTousArchetypes.", standardError, 0) ;
  return ;
}
}

//---------------------------------------------------------------------------------------
//  Function: NSSuper::ReferenceNewFilsGuides()
//  Description: Reference automatiquement les nouveaux fils guides
//  Returns:     void
//---------------------------------------------------------------------------------------
void
NSSuper::ReferenceNewFilsGuides()
{
try
{
	string ps = string("New Fils Guides management - begin") ;
	trace(&ps, 1, trDetails) ;	string sSysImportPath = _pSuperContext->PathName("INEW") ;
  if (sSysImportPath == "")
  {
  	ps = string("Can't find SysImport Path - exiting new Fils Guides management") ;
		trace(&ps, 1, trWarning) ;
    return ;
  }

  string sStatusMsg = getText("filsGuidesManagement", "automaticReferencing") ;
  afficheStatusMessage((char*) sStatusMsg.c_str()) ;  string sStatusBase = getText("filsGuidesManagement", "referencingOf") ;  string sStatusString = "" ;  // Recherche des fichier .xml dans SysImport  // Looking for xml files in SysImport directory  char szMask[1024] ;  strcpy(szMask, sSysImportPath.c_str()) ;  strcat(szMask, "*.nsg") ;
  WIN32_FIND_DATA FileData ;
  HANDLE hSearch = FindFirstFile(szMask, &FileData) ;
  if (hSearch == INVALID_HANDLE_VALUE)  {    sStatusMsg = getText("filsGuidesManagement", "noNewFilsGuidesFile") ;    afficheStatusMessage((char*) sStatusMsg.c_str()) ;    ps = string("No new Fils guides file - exiting") ;		trace(&ps, 1, trSubDetails) ;    return ;  }
  _pSuperContext->GetMainWindow()->SetCursor(0, IDC_WAIT) ;

  bool bFinish = false ;
  while (!bFinish)
  {    DWORD dwAttr = FileData.dwFileAttributes;
    if (!(dwAttr & FILE_ATTRIBUTE_DIRECTORY))
    {      string	sFileName = string(FileData.cFileName);      string	sNSG = sSysImportPath + sFileName;      ps = string("File : ") + sFileName ;      trace(&ps, 1, trSubDetails) ;      NSGuidesManager guidesManager(_pSuperContext) ;      guidesManager.setImportFile(sNSG) ;      if (false == guidesManager.isHeaderValid())      {				ps = string("- Bad header, skipping") ;        trace(&ps, 1, trSubDetails) ;      }      if (false == guidesManager.isSystemGroup())      {				ps = string("- This function only works for system groups, skipping") ;        trace(&ps, 1, trSubDetails) ;      }      else      {      	string sGroup = guidesManager.getGroup() ;        string sGroupDate ;        bool   bError ;        if (guidesManager.doesGroupExist(sGroup, bError, sGroupDate))        	guidesManager.deleteGroup(sGroup) ;      	string sFilGuideLabel = guidesManager.getLabel() ;        sStatusString = sStatusBase + string(" ") + sFilGuideLabel ;        afficheStatusMessage((char*) sStatusString.c_str()) ;      	bool bSuccess = guidesManager.importGroup() ;        if (false == bSuccess)        	ps = string("- Import failed for ") + sFilGuideLabel ;        else        {        	bSuccess = guidesManager.referenceGroup() ;          if (false == bSuccess)        		ps = string("- Import succeeded but referencing failed for ") + sFilGuideLabel ;          else        		ps = string("- Import succeeded for ") + sFilGuideLabel ;          NsDeleteTemporaryFile(this, sNSG) ;        }
        trace(&ps, 1, trSubDetails) ;      }    }
    if (!FindNextFile(hSearch, &FileData))    {      if (GetLastError() != ERROR_NO_MORE_FILES)      {      	ps = string("FindNextFile error - exiting") ;				trace(&ps, 1, trError) ;        sStatusMsg = getText("fileErrors", "cantGetNextFile") + string(" ") + sSysImportPath ;        trace(&sStatusMsg, 1, NSSuper::trError) ;        erreur(sStatusMsg.c_str(), standardError, 0) ;        return ;      }      bFinish = true ;    }  }

  ps = string("New Fils guides management - end") ;
	trace(&ps, 1, trDetails) ;
  afficheStatusMessage("") ;
  _pSuperContext->GetMainWindow()->SetCursor(0, IDC_ARROW) ;
}
catch (...)
{
  erreur("Exception NSSuper::ReferenceNewFilsGuides.", standardError, 0) ;
  return ;
}
}

// -----------------------------------------------------------------------------
// Function    : NSSuper::PurgeImpression()
// Description : détruit automatiquement les fichiers temporaires d'impression
// Returns     : void
// -----------------------------------------------------------------------------
void
NSSuper::PurgeImpression()
{
try
{
	string sUtil = _pSuperContext->getUtilisateur()->getNss() ;
	string sTemp = _pSuperContext->PathName("FPER") + string("tempo.dat") ;
	string sRes ;

	if (!lireParam(sUtil, sTemp, "KillTmp", sRes))
		return ;

  if (false == IsNo(sRes))
    return ;

  WIN32_FIND_DATA FileData;
  HANDLE          hSearch;  char            szMask[255];  bool            bFinish = false;  size_t          pos;  string          sPathName, sFileName, sExtension;  string          sHTML;  DWORD           dwAttr;  afficheStatusMessage("Purge du répertoire d'impression.");  _pSuperContext->GetMainWindow()->SetCursor(0, IDC_WAIT);  sPathName = _pSuperContext->PathName("SHTM");  strcpy(szMask, sPathName.c_str());  strcat(szMask, "*.*");
  hSearch = FindFirstFile(szMask, &FileData);
  if (hSearch == INVALID_HANDLE_VALUE)    return ;  while (false == bFinish)  {    dwAttr = FileData.dwFileAttributes ;

    if (!(dwAttr & FILE_ATTRIBUTE_DIRECTORY))    {      sFileName = string(FileData.cFileName);      // Récupération de l'extension      pos = sFileName.find(".");    }
    if ((!(dwAttr & FILE_ATTRIBUTE_DIRECTORY)) && (NPOS != pos))    {      sExtension = string(sFileName, pos+1, strlen(sFileName.c_str())-pos-1) ;
      if (string("HTM") == pseumaj(sExtension))      {        sHTML = sPathName + sFileName ;        if (false == NsDeleteTemporaryFile(this, sHTML))          erreur("Pb de destruction du fichier temporaire d'impression.", standardError, 0) ;
      }    }
    if (!FindNextFile(hSearch, &FileData))    {      if (GetLastError() == ERROR_NO_MORE_FILES)        bFinish = true ;      else      {        erreur("Impossible de trouver le fichier suivant dans le répertoire d'impression.", standardError, 0) ;        return ;      }    }  }

  afficheStatusMessage("") ;
  _pSuperContext->GetMainWindow()->SetCursor(0, IDC_ARROW) ;
}
catch (...)
{
  erreur("Exception NSSuper::PurgeImpression.", standardError, 0) ;
  return ;
}
}

// -----------------------------------------------------------------------------// Function     : NSSuper::OuvreDocument(NSDocumentInfo& Document)// 				        appelee par ChoixChemiseDialog (NSRechdl)
// -----------------------------------------------------------------------------
// Arguments    :	Document à ouvrir
// -----------------------------------------------------------------------------
// Description  : Attache le document au modèle Document/Visualisation
// -----------------------------------------------------------------------------
// Returns      : pDocNoy si on peut ouvrir le document, 0 sinon
// -----------------------------------------------------------------------------
NSNoyauDocument
*NSSuper::OuvreDocument(NSDocumentInfo& Document, NSContexte *pCtx, bool bReadOnly)
{
  NSDocumentInfo  *pDocument = &Document ;
  NSDocumentInfo  *pDocHtml  = 0 ;
  NSNoyauDocument *pDocNoy   = 0 ;

	string sTypeDoc = Document.getTypeSem() ;

  if (typeDocument(sTypeDoc, NSSuper::isTree))
  {
  	if      (sTypeDoc == "ZCN00")
    	pDocNoy = OuvreDocumentCR(pDocument, pDocHtml, pCtx, bReadOnly) ;
    else if (sTypeDoc == "ZCS00")
    	pDocNoy = OuvreDocumentCS(pDocument, pDocHtml, pCtx, bReadOnly) ;
    else if (sTypeDoc == "ZCQ00")
    	pDocNoy = OuvreDocumentCQ(pDocument, pDocHtml, pCtx, bReadOnly) ;
    else if (sTypeDoc == "ZQQOL")
    	pDocNoy = OuvreDocumentQoL(pDocument, pDocHtml, pCtx, bReadOnly) ;
  }
  else if (typeDocument(sTypeDoc, NSSuper::isText))
  {
  	if     ((sTypeDoc == "ZTRTF") || (sTypeDoc == "ZTHTM"))
    	pDocNoy = OuvreDocumentRTF(pDocument,pDocHtml, pCtx, bReadOnly) ;
    else if (sTypeDoc == "ZTTXT")
    	pDocNoy = OuvreDocumentTXT(pDocument,pDocHtml, pCtx, bReadOnly) ;
    else if (sTypeDoc == "ZTPDF")
    	pDocNoy = OuvreDocumentPDF(pDocument,pDocHtml, pCtx, bReadOnly) ;
  }
  else if (typeDocument(sTypeDoc, NSSuper::isHTML)) // cas des html statiques
  {
  	if ((sTypeDoc == "ZSHTM") || (sTypeDoc == "ZIHTM"))
    	pDocNoy = OuvreDocumentHTML(pDocument, pDocHtml, pCtx, bReadOnly) ;
  }
  else if (typeDocument(sTypeDoc, NSSuper::isImage))
  {
  	// cas des images : générique pour tous les types d'image
    // y compris les images animées (vidéos)
    pDocNoy = OuvreDocumentIMG(pDocument, pDocHtml, pCtx, bReadOnly) ;
  }

  return pDocNoy ;
}


// -----------------------------------------------------------------------------
// Function     : NSSuper::OuvreDocumentCR(NSDocumentInfo Document)
// -----------------------------------------------------------------------------
// Arguments    :	Document à ouvrir
// -----------------------------------------------------------------------------
// Description  : Attache le document au modèle Document/Visualisation
// -----------------------------------------------------------------------------
// Returns      : true si on peut ouvrir le document, false sinon
// -----------------------------------------------------------------------------
NSNoyauDocument*
NSSuper::OuvreDocumentCR(NSDocumentInfo *pDocument, NSDocumentInfo *pDocHtml, NSContexte *pCtx, bool bReadOnly)
{
try
{
	if ((NSDocumentInfo*) NULL == pDocument)
		return 0 ;

	NSCRDocument *DocVisuDoc = new NSCRDocument(0, pDocument, pDocHtml, pCtx, "", bReadOnly) ;
	if (false == DocVisuDoc->isValidDoc())
	{
  	string sNomDoc  = pDocument->getDocName() ;
    string sCodeDoc = pDocument->getID() ;
    char   msg[255] ;

    sprintf(msg, "Le document %s (code = %s) n'est pas un document valide.", sNomDoc.c_str(), sCodeDoc.c_str()) ;
    erreur(msg, standardError, 0, _pSuperContext->GetMainWindow()->GetHandle()) ;
    return 0 ;
	}

  // Avant de pouvoir visualiser un CR, on doit s'assurer qu'il
  // est bien décodé dans un fichier de décodage (pour créer le Html)
  //
  if (!DocVisuDoc->IsDecode())
  {
  	if ((DocVisuDoc->decodeCR(&(DocVisuDoc->_aCRPhrases))) != 0)
    {
    	erreur("Impossible de décoder le compte-rendu.", standardError, 0, _pSuperContext->GetMainWindow()->GetHandle()) ;
      return 0 ;
    }
	}

  DocVisuDoc->Visualiser() ;
  return DocVisuDoc ;
}
catch (...)
{
  erreur("Exception NSSuper::OuvreDocumentCR.", standardError, 0) ;
  return NULL ;
}
}


// -----------------------------------------------------------------------------
// Function     : NSSuper::OuvreDocumentCS(NSDocumentInfo *pDocument, NSDocumentInfo *pDocHtml)
// -----------------------------------------------------------------------------
// Arguments    : Document à ouvrir
// -----------------------------------------------------------------------------
// Description  : Attache le document au modèle Document/Visualisation
// -----------------------------------------------------------------------------
// Returns      : true si on peut ouvrir le document, false sinon
// -----------------------------------------------------------------------------
NSNoyauDocument
*NSSuper::OuvreDocumentCS(NSDocumentInfo *pDocument, NSDocumentInfo *pDocHtml, NSContexte *pCtx, bool bReadOnly)
{
try
{
	if ((NSDocumentInfo*) NULL == pDocument)
		return 0 ;

	NSCSDocument *DocVisuDoc = new NSCSDocument(0, pDocument, pDocHtml, pCtx, "", bReadOnly) ;
  if (false == DocVisuDoc->isValidDoc())
  {
    string sNomDoc  = pDocument->getDocName() ;
    string sCodeDoc = pDocument->getID() ;
    char   msg[255] ;

    sprintf(msg, "Le document %s (code = %s) n'est pas un document valide.", sNomDoc.c_str(), sCodeDoc.c_str()) ;
    erreur(msg, standardError, 0, _pSuperContext->GetMainWindow()->GetHandle()) ;
    return 0 ;
  }

  // Avant de pouvoir visualiser le document CS
  // on regarde s'il existe une vue d'édition.
  // sinon, on en crée une fictive et on génère le NodeArray pour
  // pouvoir créer le html.
  NSCsVue *pEditView ;

  TView *pView = _pSuperContext->getPatient()->getDocHis()->ActiveFenetre(DocVisuDoc->_pDocInfo, "NSCsVue") ;
  if (pView)
  {
    // en fait, ce cas ne se produit pas en théorie car si le document est édité
    // on ne doit pas pouvoir l'ouvrir par NSSuper::OuvreDocument, on passe
    // directement par Visualiser...
    pEditView = dynamic_cast<NSCsVue *>(pView) ;
  }
  else
  {
    // on crée dans ce cas une vue artificielle (invisible)
    // pour créer le pNodeArray qui permet d'obtenir le pHtmlCS
    pEditView = new NSCsVue(*DocVisuDoc, _pSuperContext->GetMainWindow()) ;
    pEditView->Create() ;
    pEditView->Show(SW_HIDE) ;
  }

  // DocVisuDoc est ici un nouveau document. On se sert de la vue
  // pour lui créer un pHtmlCS et on lance Visualiser()
  DocVisuDoc->_pHtmlCS = new NSHtml(tCS) ;
	pEditView->InscrireHtml(DocVisuDoc->_pHtmlCS) ;
  DocVisuDoc->Visualiser() ;

  // si la vue d'édition vient d'être créée, on doit la détruire ici
  // sinon elle empêchera la destruction du document car il
  // existera toujours une vue en cours.
  // ATTENTION : il faut obligatoirement avoir au préalable
  // lancé DocVisuDoc->Visualiser() pour créer avant une VisualView
  // sinon le document serait détruit dans la foulée (...)
  if (!pView)
    delete pEditView ;
  return DocVisuDoc ;
}
catch (...)
{
  erreur("Exception NSSuper::OuvreDocumentCS.", standardError, 0) ;
  return NULL ;
}
}

// -----------------------------------------------------------------------------
// Function     : NSSuper::OuvreDocumentCQ(NSDocumentInfo *pDocument, NSDocumentInfo *pDocHtml)
// -----------------------------------------------------------------------------
// Arguments    : Document à ouvrir
// -----------------------------------------------------------------------------
// Description  : Attache le document au modèle Document/Visualisation
// -----------------------------------------------------------------------------
// Returns      : true si on peut ouvrir le document, false sinon
// -----------------------------------------------------------------------------
NSNoyauDocument*
NSSuper::OuvreDocumentCQ(NSDocumentInfo* pDocument, NSDocumentInfo* pDocHtml, NSContexte* pCtx, bool bReadOnly)
{
try
{
	if ((NSDocumentInfo*) NULL == pDocument)
		return 0 ;

	NSCQDocument *DocVisuDoc = new NSCQDocument(0, pDocument, pDocHtml, pCtx, bReadOnly) ;

	// on regarde si on a un fichier valide pour ce document
	if (false == DocVisuDoc->isValidDoc())
	{
  	string sNomDoc  = pDocument->getDocName() ;
    string sCodeDoc = pDocument->getID() ;
    char   msg[255] ;

    sprintf(msg, "Le document %s (code = %s) n'est pas un document valide.", sNomDoc.c_str(), sCodeDoc.c_str()) ;
    erreur(msg, standardError, 0, _pSuperContext->GetMainWindow()->GetHandle()) ;
    return 0 ;
	}

	DocVisuDoc->Visualiser() ;
	return DocVisuDoc ;
}
catch (...)
{
    erreur("Exception NSSuper::OuvreDocumentCQ.", standardError, 0) ;
    return NULL ;
}
}

NSNoyauDocument*
NSSuper::OuvreDocumentQoL(NSDocumentInfo* pDocument, NSDocumentInfo* pDocHtml, NSContexte* pCtx, bool bReadOnly)
{
try
{
	if ((NSDocumentInfo*) NULL == pDocument)
		return (NSNoyauDocument*) 0 ;

	NSCoopChartDocument *DocVisuDoc = new NSCoopChartDocument(0, pDocument, pDocHtml, pCtx, bReadOnly) ;

	// on regarde si on a un fichier valide pour ce document
	if (false == DocVisuDoc->isValidDoc())
	{
  	string sNomDoc  = pDocument->getDocName() ;
    string sCodeDoc = pDocument->getID() ;
    char   msg[255] ;

    sprintf(msg, "Le document %s (code = %s) n'est pas un document valide.", sNomDoc.c_str(), sCodeDoc.c_str()) ;
    erreur(msg, standardError, 0, _pSuperContext->GetMainWindow()->GetHandle()) ;
    return 0 ;
	}

	DocVisuDoc->Visualiser() ;
	return DocVisuDoc ;
}
catch (...)
{
  erreur("Exception NSSuper::OuvreDocumentQoL.", standardError, 0) ;
  return (NSNoyauDocument*) 0 ;
}
}

// -----------------------------------------------------------------------------
// Function     : NSSuper::OuvreDocumentRTF(NSDocumentInfo Document)
// -----------------------------------------------------------------------------
// Arguments    : Document à ouvrir
// -----------------------------------------------------------------------------
// Description  : Attache le document au modèle Document/Visualisation
// -----------------------------------------------------------------------------
// Returns      : true si on peut ouvrir le document, false sinon
// -----------------------------------------------------------------------------
NSNoyauDocument
*NSSuper::OuvreDocumentRTF(NSDocumentInfo *pDocument, NSDocumentInfo *pDocHtml, NSContexte *pCtx, bool bReadOnly)
{
try
{
	if ((NSDocumentInfo*) NULL == pDocument)
		return 0 ;

  // on traite ici le cas des documents word
  // pour l'ouverture, on les associe à une VisualView (pour les visualiser en html)
  // pour l'édition, on utilisera une AutoWordView (définie dans nsttx)
  NSTtxDocument *DocVisuDoc = new NSTtxDocument(0, bReadOnly, pDocument, pDocHtml, pCtx) ;

  // on regarde si on a un fichier valide pour ce document
  // (ValideFichier est lancée par Open du DocTtx)
  if (false == DocVisuDoc->isValidDoc())
  {
    string sNomDoc  = pDocument->getDocName() ;
    string sCodeDoc = pDocument->getID() ;
    char   msg[255] ;

    sprintf(msg, "Le document %s (code = %s) n'est pas un document valide.", sNomDoc.c_str(), sCodeDoc.c_str()) ;
    erreur(msg, standardError, 0, _pSuperContext->GetMainWindow()->GetHandle()) ;
    return 0 ;
  }

  DocVisuDoc->Visualiser() ;
  return DocVisuDoc ;
}
catch (...)
{
  erreur("Exception NSSuper::OuvreDocumentRTF.", standardError, 0) ;
  return NULL ;
}
}


// -----------------------------------------------------------------------------
// Function     : NSSuper::OuvreDocumentHTML(NSDocumentInfo* pDocument, NSDocumentInfo* pDocHtml)
// -----------------------------------------------------------------------------
// Arguments    : Document à ouvrir
// -----------------------------------------------------------------------------
// Description  : Attache le document au modèle Document/Visualisation (cf nsvisual.h)
// -----------------------------------------------------------------------------
// Returns      : true si on peut ouvrir le document, false sinon
// -----------------------------------------------------------------------------
NSNoyauDocument
*NSSuper::OuvreDocumentHTML(NSDocumentInfo *pDocument, NSDocumentInfo *pDocHtml, NSContexte *pCtx, bool bReadOnly)
{
try
{
	NSRefDocument *pDocVisu = new NSRefDocument(0, pDocument, pDocHtml, pCtx, bReadOnly) ;

#ifndef __EPIPUMP__
  // on regarde si on a un fichier valide pour ce document
  pDocVisu->ValideFichier() ;
  if (false == pDocVisu->isValidDoc())
  {
    string sNomDoc  = pDocument->getDocName() ;
    string sCodeDoc = pDocument->getID() ;
    char   msg[255] ;

    sprintf(msg, "Le document %s (code = %s) n'est pas un document valide.", sNomDoc.c_str(), sCodeDoc.c_str()) ;
    erreur(msg, standardError, 0, _pSuperContext->GetMainWindow()->GetHandle()) ;
    return 0 ;
  }

  string sLocalis = pDocument->getLocalis() ;
  if (sLocalis != "")
  {
    // cas des html statiques : la template associée au fichier est le fichier lui-meme
    pDocVisu->_sTemplate = _pSuperContext->PathName(sLocalis) ;

    pDocVisu->_sTemplate += pDocument->getFichier() ;
  }
  else // cas des html externes
    pDocVisu->_sTemplate = string("") ;

	NSDocViewManager dvManager(_pSuperContext) ;
	dvManager.createView(pDocVisu, "Visual Format") ;

#endif // !__EPIPUMP__
  return pDocVisu ;
}
catch (...)
{
  erreur("Exception NSSuper::OuvreDocumentHTML.", standardError, 0) ;
  return NULL ;
}
}

//---------------------------------------------------------------------------
//  Function: NSSuper::OuvreDocumentIMG(NSDocumentInfo* pDocument, NSDocumentInfo* pDocHtml)
//  Arguments:	  Document à ouvrir
//  Description: Attache le document au modèle Document/Visualisation
//  Returns:     true si on peut ouvrir le document, false sinon
//---------------------------------------------------------------------------
NSNoyauDocument*
NSSuper::OuvreDocumentIMG(NSDocumentInfo* pDocument, NSDocumentInfo* pDocHtml, NSContexte* pCtx, bool bReadOnly)
{
try
{
	NSRefDocument*	pDocVisu = new NSRefDocument(0, pDocument, pDocHtml, pCtx, bReadOnly);

#ifndef __EPIPUMP__
	// on regarde si on a un fichier valide pour ce document
	pDocVisu->ValideFichier();

	if (false == pDocVisu->isValidDoc())
	{
		string sNomDoc  = pDocument->getDocName() ;
		string sCodeDoc = pDocument->getID() ;
		char   msg[1024];

		sprintf(msg, "Le document %s (code = %s) n'est pas un document valide.", sNomDoc.c_str(), sCodeDoc.c_str()) ;
		erreur(msg, standardError, 0, _pSuperContext->GetMainWindow()->GetHandle()) ;
		return 0;
	}

	// cas des documents images : on passe pour le moment la template en dur
	pDocVisu->_sTemplate = _pSuperContext->PathName("NTPL") + string("docimage.htm");

	NSDocViewManager dvManager(_pSuperContext) ;
	dvManager.createView(pDocVisu, "Visual Format") ;

#endif // !__EPIPUMP__
	return pDocVisu;
}
catch (...)
{
	erreur("Exception NSSuper::OuvreDocumentIMG.", standardError, 0) ;
	return NULL;
}
}

//---------------------------------------------------------------------------
//  Function: NSSuper::OuvreDocumentTXT(NSDocumentInfo* pDocument, NSDocumentInfo* pDocHtml)
//  Arguments:	  Document à ouvrir
//  Description: Attache le document au modèle Document/Visualisation
//  Returns:     true si on peut ouvrir le document, false sinon
//---------------------------------------------------------------------------
NSNoyauDocument*
NSSuper::OuvreDocumentTXT(NSDocumentInfo* pDocument, NSDocumentInfo* pDocHtml, NSContexte* pCtx, bool bReadOnly)
{
try
{
	NSSimpleTxtDocument *DocVisuDoc = new NSSimpleTxtDocument(0, bReadOnly, pDocument, pDocHtml, pCtx) ;

	// on regarde si on a un fichier valide pour ce document
	// (ValideFichier est lancée par Open du DocTtx)
	if (false == DocVisuDoc->isValidDoc())
	{
		string sNomDoc  = pDocument->getDocName() ;
		string sCodeDoc = pDocument->getID() ;
		char   msg[1024] ;

		sprintf(msg, "Le document %s (code = %s) n'est pas un document valide.", sNomDoc.c_str(), sCodeDoc.c_str()) ;
		erreur(msg, standardError, 0, _pSuperContext->GetMainWindow()->GetHandle()) ;
		return 0 ;
	}

	DocVisuDoc->Visualiser() ;
	return DocVisuDoc ;
}
catch (...)
{
	erreur("Exception NSSuper::OuvreDocumentTXT", standardError, 0) ;
	return NULL;
}
}

//---------------------------------------------------------------------------
//  Function: NSSuper::OuvreDocumentTXT(NSDocumentInfo* pDocument, NSDocumentInfo* pDocHtml)
//  Arguments:	  Document à ouvrir
//  Description: Attache le document au modèle Document/Visualisation
//  Returns:     true si on peut ouvrir le document, false sinon
//---------------------------------------------------------------------------
NSNoyauDocument*
NSSuper::OuvreDocumentPDF(NSDocumentInfo* pDocument, NSDocumentInfo* pDocHtml, NSContexte* pCtx, bool bReadOnly)
{
try
{
	NSAcrobatDocument *DocVisuDoc = new NSAcrobatDocument(0, bReadOnly, pDocument, pDocHtml, pCtx) ;

	// on regarde si on a un fichier valide pour ce document
	// (ValideFichier est lancée par Open du DocTtx)
	if (false == DocVisuDoc->isValidDoc())
	{
		string sNomDoc  = pDocument->getDocName() ;
		string sCodeDoc = pDocument->getID() ;
		char   msg[1024] ;

		sprintf(msg, "Le document %s (code = %s) n'est pas un document valide.", sNomDoc.c_str(), sCodeDoc.c_str()) ;
		erreur(msg, standardError, 0, _pSuperContext->GetMainWindow()->GetHandle()) ;
		return 0 ;
	}

	DocVisuDoc->Visualiser() ;
	return DocVisuDoc ;
}
catch (...)
{
	erreur("Exception NSSuper::OuvreDocumentPDF", standardError, 0) ;
	return NULL;
}
}

string
NSSuper::GetDocAsHtml(NSDocumentInfo& Document, NSContexte* pCtx, bool bInsertMeta, bool bInsertTree)
{
  NSDocumentInfo  *pDocument = &Document ;
  // NSDocumentInfo  *pDocHtml  = 0 ;
  // NSNoyauDocument *pDocNoy   = 0 ;

	string sTypeDoc = Document.getTypeSem() ;

  if (typeDocument(sTypeDoc, NSSuper::isTree))
  {
  	if      (sTypeDoc == "ZCN00")
      return GetDocAsHtmlForCR(pDocument, pCtx, bInsertMeta, bInsertTree) ;
    else if (sTypeDoc == "ZCS00")
    	return GetDocAsHtmlForCS(pDocument, pCtx, bInsertMeta, bInsertTree) ;
    else if (sTypeDoc == "ZCQ00")
    	return GetDocAsHtmlForCQ(pDocument, pCtx, bInsertMeta, bInsertTree) ;
    else if (sTypeDoc == "ZQQOL")
    	return GetDocAsHtmlForQoL(pDocument, pCtx, bInsertMeta, bInsertTree) ;
  }
  else if (typeDocument(sTypeDoc, NSSuper::isText))
  {
  	if     ((sTypeDoc == "ZTRTF") || (sTypeDoc == "ZTHTM"))
    	return GetDocAsHtmlForRTF(pDocument, pCtx, bInsertMeta) ;
    else if (sTypeDoc == "ZTTXT")
    	return GetDocAsHtmlForTXT(pDocument, pCtx, bInsertMeta) ;
    else if (sTypeDoc == "ZTPDF")
    	return GetDocAsHtmlForPDF(pDocument, pCtx, bInsertMeta) ;
  }
  else if (typeDocument(sTypeDoc, NSSuper::isHTML)) // cas des html statiques
  {
  	if ((sTypeDoc == "ZSHTM") || (sTypeDoc == "ZIHTM"))
    	return GetDocAsHtmlForHTML(pDocument, pCtx, bInsertMeta) ;
  }
  else if (typeDocument(sTypeDoc, NSSuper::isImage))
  {
  	// cas des images : générique pour tous les types d'image
    // y compris les images animées (vidéos)
    return GetDocAsHtmlForIMG(pDocument, pCtx, bInsertMeta) ;
  }

  return string("") ;
}

string
NSSuper::GetDocAsHtmlForCR(NSDocumentInfo* pDocument, NSContexte* pCtx, bool bInsertMeta, bool bInsertTree)
{
  if (NULL == pDocument)
		return string("") ;

  NSDocumentInfo *pDocHtml = (NSDocumentInfo *) 0 ;

	NSCRDocument DocVisuDoc((TDocument*) 0, pDocument, pDocHtml, pCtx, "", true) ;
  DocVisuDoc.setCloneStatus(true) ;
	if (false == DocVisuDoc.isValidDoc())
	{
    string sErrorMsg = string("Document ") + pDocument->getDocName() + string(" (ID= ") + pDocument->getID() + string(") is not a valid document.") ;
    trace(&sErrorMsg, 1, trError) ;
    return string("") ;
	}

  string sOut = string("") ;

  if (false == DocVisuDoc.GenereHtmlText(sOut))
	{
    string sErrorMsg = string("Document ") + pDocument->getDocName() + string(" (ID= ") + pDocument->getID() + string(") cannot be generated as HTML.") ;
    trace(&sErrorMsg, 1, trError) ;
    return string("") ;
	}

  return sOut ;
}

string
NSSuper::GetDocAsHtmlForCS(NSDocumentInfo* pDocument, NSContexte* pCtx, bool bInsertMeta, bool bInsertTree)
{
  if ((NSDocumentInfo*) NULL == pDocument)
    return string("") ;

  NSDocumentInfo *pDocHtml = (NSDocumentInfo *) 0 ;

	NSCSDocument DocVisuDoc((TDocument*) 0, pDocument, pDocHtml, pCtx, "", true) ;
  DocVisuDoc.setCloneStatus(true) ;
	if (false == DocVisuDoc.isValidDoc())
	{
    string sErrorMsg = string("Document ") + pDocument->getDocName() + string(" (ID= ") + pDocument->getID() + string(") is not a valid document.") ;
    trace(&sErrorMsg, 1, trError) ;
    return string("") ;
	}

  string sOut = string("") ;
  if (false == DocVisuDoc.GenereHtmlText(sOut))
	{
    string sErrorMsg = string("Document ") + pDocument->getDocName() + string(" (ID= ") + pDocument->getID() + string(") cannot be generated as HTML.") ;
    trace(&sErrorMsg, 1, trError) ;
    return string("") ;
	}
  return sOut ;
}

string
NSSuper::GetDocAsHtmlForCQ(NSDocumentInfo* pDocument, NSContexte* pCtx, bool bInsertMeta, bool bInsertTree)
{
  if ((NSDocumentInfo*) NULL == pDocument)
		return string("") ;

  NSDocumentInfo *pDocHtml = (NSDocumentInfo *) 0 ;

	NSCQDocument DocVisuDoc((TDocument*) 0, pDocument, pDocHtml, pCtx, true) ;
  DocVisuDoc.setCloneStatus(true) ;
	if (false == DocVisuDoc.isValidDoc())
	{
    string sErrorMsg = string("Document ") + pDocument->getDocName() + string(" (ID= ") + pDocument->getID() + string(") is not a valid document.") ;
    trace(&sErrorMsg, 1, trError) ;
    return string("") ;
	}

  string sOut = string("") ;
  if (false == DocVisuDoc.GenereHtmlText(sOut))
	{
    string sErrorMsg = string("Document ") + pDocument->getDocName() + string(" (ID= ") + pDocument->getID() + string(") cannot be generated as HTML.") ;
    trace(&sErrorMsg, 1, trError) ;
    return string("") ;
	}
  return sOut ;
}

string
NSSuper::GetDocAsHtmlForQoL(NSDocumentInfo* pDocument, NSContexte* pCtx, bool bInsertMeta, bool bInsertTree)
{
  if ((NSDocumentInfo*) NULL == pDocument)
		return string("") ;

  NSDocumentInfo *pQolHtml = (NSDocumentInfo *) 0 ;

	NSCoopChartDocument DocVisuDoc((TDocument*) 0, pDocument, pQolHtml, pCtx, true) ;
  DocVisuDoc.setCloneStatus(true) ;
	if (false == DocVisuDoc.isValidDoc())
	{
    string sErrorMsg = string("Document ") + pDocument->getDocName() + string(" (ID= ") + pDocument->getID() + string(") is not a valid document.") ;
    trace(&sErrorMsg, 1, trError) ;
    return string("") ;
	}

  string sOut = string("") ;
  if (false == DocVisuDoc.GenereHtmlText(sOut))
	{
    string sErrorMsg = string("Document ") + pDocument->getDocName() + string(" (ID= ") + pDocument->getID() + string(") cannot be generated as HTML.") ;
    trace(&sErrorMsg, 1, trError) ;
    return string("") ;
	}
  return sOut ;
}

string
NSSuper::GetDocAsHtmlForRTF(NSDocumentInfo* pDocument, NSContexte* pCtx, bool bInsertMeta)
{
  // if (NULL == pDocument)
		return string("") ;
}

string
NSSuper::GetDocAsHtmlForTXT(NSDocumentInfo* pDocument, NSContexte* pCtx, bool bInsertMeta)
{
  // if (NULL == pDocument)
		return string("") ;
}

string
NSSuper::GetDocAsHtmlForPDF(NSDocumentInfo* pDocument, NSContexte* pCtx, bool bInsertMeta)
{
  // if (NULL == pDocument)
		return string("") ;
}

string
NSSuper::GetDocAsHtmlForHTML(NSDocumentInfo* pDocument, NSContexte* pCtx, bool bInsertMeta)
{
  // if (NULL == pDocument)
		return string("") ;
}

string
NSSuper::GetDocAsHtmlForIMG(NSDocumentInfo* pDocument, NSContexte* pCtx, bool bInsertMeta)
{
  // if (NULL == pDocument)
		return string("") ;
}

bool
NSSuper::ExportDocAsHtml(NSDocumentInfo* pDocument, NSContexte* pCtx, string sDocName)
{
  if ((NSDocumentInfo*) NULL == pDocument)
		return false ;

	string sTypeDoc = pDocument->getTypeSem() ;

  if (typeDocument(sTypeDoc, NSSuper::isTree))
  {
  	if      (sTypeDoc == "ZCN00")
      return ExportDocAsHtmlForCR(pDocument, pCtx, sDocName) ;
    else if (sTypeDoc == "ZCS00")
    	return ExportDocAsHtmlForCS(pDocument, pCtx, sDocName) ;
    else if (sTypeDoc == "ZCQ00")
    	return ExportDocAsHtmlForCQ(pDocument, pCtx, sDocName) ;
    else if (sTypeDoc == "ZQQOL")
    	return ExportDocAsHtmlForQoL(pDocument, pCtx, sDocName) ;
  }
  else if (typeDocument(sTypeDoc, NSSuper::isText))
  {
  	if     ((sTypeDoc == "ZTRTF") || (sTypeDoc == "ZTHTM"))
    	return ExportDocAsHtmlForRTF(pDocument, pCtx, sDocName) ;
    else if (sTypeDoc == "ZTTXT")
    	return ExportDocAsHtmlForTXT(pDocument, pCtx, sDocName) ;
    else if (sTypeDoc == "ZTPDF")
    	return ExportDocAsHtmlForPDF(pDocument, pCtx, sDocName) ;
  }
  else if (typeDocument(sTypeDoc, NSSuper::isHTML)) // cas des html statiques
  {
  	if ((sTypeDoc == "ZSHTM") || (sTypeDoc == "ZIHTM"))
    	return ExportDocAsHtmlForHTML(pDocument, pCtx, sDocName) ;
  }
  else if (typeDocument(sTypeDoc, NSSuper::isImage))
  {
  	// cas des images : générique pour tous les types d'image
    // y compris les images animées (vidéos)
    return ExportDocAsHtmlForIMG(pDocument, pCtx, sDocName) ;
  }

  return false ;
}

bool
NSSuper::ExportDocAsHtmlForCR(NSDocumentInfo* pDocument, NSContexte* pCtx, string sDocName)
{
  if ((NSDocumentInfo*) NULL == pDocument)
		return false ;

  NSDocumentInfo *pDocHtml = (NSDocumentInfo *) 0 ;

	NSCRDocument DocVisuDoc((TDocument*) 0, pDocument, pDocHtml, pCtx, "", true) ;
  DocVisuDoc.setCloneStatus(true) ;
	if (false == DocVisuDoc.isValidDoc())
	{
    string sErrorMsg = string("Document ") + pDocument->getDocName() + string(" (ID= ") + pDocument->getID() + string(") is not a valid document.") ;
    trace(&sErrorMsg, 1, trError) ;
    return false ;
	}

  if (false == DocVisuDoc.IsDecode())
  {
    if (DocVisuDoc.decodeCR(&(DocVisuDoc._aCRPhrases)) != 0)
      return false ;
  }

  return DocVisuDoc.exportFile(sDocName, string("HTM")) ;
}

bool
NSSuper::ExportDocAsHtmlForCS(NSDocumentInfo* pDocument, NSContexte* pCtx, string sDocName)
{
  if ((NSDocumentInfo*) NULL == pDocument)
		return false ;

  NSDocumentInfo *pDocHtml = (NSDocumentInfo *) 0 ;

	NSCSDocument DocVisuDoc((TDocument*) 0, pDocument, pDocHtml, pCtx, "", true) ;
  DocVisuDoc.setCloneStatus(true) ;
	if (false == DocVisuDoc.isValidDoc())
	{
    string sErrorMsg = string("Document ") + pDocument->getDocName() + string(" (ID= ") + pDocument->getID() + string(") is not a valid document.") ;
    trace(&sErrorMsg, 1, trError) ;
    return false ;
	}

  NSCsVue EditView(DocVisuDoc, _pSuperContext->GetMainWindow()) ;
  EditView.Create() ;
  EditView.Show(SW_HIDE) ;

  DocVisuDoc._pHtmlCS = new NSHtml(tCS) ;
	EditView.InscrireHtml(DocVisuDoc._pHtmlCS) ;
  // DocVisuDoc.Visualiser() ;

  return DocVisuDoc.exportFile(sDocName, string("HTM")) ;
}

bool
NSSuper::ExportDocAsHtmlForCQ(NSDocumentInfo* pDocument, NSContexte* pCtx, string sDocName)
{
  if ((NSDocumentInfo*) NULL == pDocument)
		return false ;

  NSDocumentInfo *pDocHtml = (NSDocumentInfo *) 0 ;

	NSCQDocument DocVisuDoc((TDocument*) 0, pDocument, pDocHtml, pCtx, true) ;
  DocVisuDoc.setCloneStatus(true) ;
	if (false == DocVisuDoc.isValidDoc())
	{
    string sErrorMsg = string("Document ") + pDocument->getDocName() + string(" (ID= ") + pDocument->getID() + string(") is not a valid document.") ;
    trace(&sErrorMsg, 1, trError) ;
    return false ;
	}

  return DocVisuDoc.exportFile(sDocName, string("HTM")) ;
}

bool
NSSuper::ExportDocAsHtmlForQoL(NSDocumentInfo* pDocument, NSContexte* pCtx, string sDocName)
{
  if ((NSDocumentInfo*) NULL == pDocument)
		return false ;

  NSDocumentInfo *pDocHtml = (NSDocumentInfo *) 0 ;

	NSCoopChartDocument DocVisuDoc((TDocument*) 0, pDocument, pDocHtml, pCtx, true) ;
  DocVisuDoc.setCloneStatus(true) ;
	if (false == DocVisuDoc.isValidDoc())
	{
    string sErrorMsg = string("Document ") + pDocument->getDocName() + string(" (ID= ") + pDocument->getID() + string(") is not a valid document.") ;
    trace(&sErrorMsg, 1, trError) ;
    return false ;
	}

  return DocVisuDoc.exportFile(sDocName, string("HTM")) ;
}

bool
NSSuper::ExportDocAsHtmlForRTF(NSDocumentInfo* pDocument, NSContexte* pCtx, string sDocName)
{
  if ((NSDocumentInfo*) NULL == pDocument)
		return false ;

  NSDocumentInfo *pDocHtml = (NSDocumentInfo *) 0 ;

	NSTtxDocument DocVisuDoc((TDocument*) 0, true, pDocument, pDocHtml, pCtx) ;
  DocVisuDoc.setCloneStatus(true) ;
	if (false == DocVisuDoc.isValidDoc())
	{
    string sErrorMsg = string("Document ") + pDocument->getDocName() + string(" (ID= ") + pDocument->getID() + string(") is not a valid document.") ;
    trace(&sErrorMsg, 1, trError) ;
    return false ;
	}

  return DocVisuDoc.exportFile(sDocName, string("HTM")) ;
}

bool
NSSuper::ExportDocAsHtmlForTXT(NSDocumentInfo* pDocument, NSContexte* pCtx, string sDocName)
{
  if ((NSDocumentInfo*) NULL == pDocument)
		return false ;

  NSDocumentInfo *pDocHtml = (NSDocumentInfo *) 0 ;

	NSSimpleTxtDocument DocVisuDoc((TDocument*) 0, true, pDocument, pDocHtml, pCtx) ;
  DocVisuDoc.setCloneStatus(true) ;
	if (false == DocVisuDoc.isValidDoc())
	{
    string sErrorMsg = string("Document ") + pDocument->getDocName() + string(" (ID= ") + pDocument->getID() + string(") is not a valid document.") ;
    trace(&sErrorMsg, 1, trError) ;
    return false ;
	}

  return DocVisuDoc.exportFile(sDocName, string("HTM")) ;
}

bool
NSSuper::ExportDocAsHtmlForPDF(NSDocumentInfo* pDocument, NSContexte* pCtx, string sDocName)
{
  if ((NSDocumentInfo*) NULL == pDocument)
		return false ;

  NSDocumentInfo *pDocHtml = (NSDocumentInfo *) 0 ;

	NSAcrobatDocument DocVisuDoc((TDocument*) 0, true, pDocument, pDocHtml, pCtx) ;
  DocVisuDoc.setCloneStatus(true) ;
	if (false == DocVisuDoc.isValidDoc())
	{
    string sErrorMsg = string("Document ") + pDocument->getDocName() + string(" (ID= ") + pDocument->getID() + string(") is not a valid document.") ;
    trace(&sErrorMsg, 1, trError) ;
    return false ;
	}

  return DocVisuDoc.exportFile(sDocName, string("HTM")) ;
}

bool
NSSuper::ExportDocAsHtmlForHTML(NSDocumentInfo* pDocument, NSContexte* pCtx, string sDocName)
{
  if ((NSDocumentInfo*) NULL == pDocument)
		return false ;

  NSDocumentInfo *pDocHtml = (NSDocumentInfo *) 0 ;

	NSRefDocument DocVisuDoc((TDocument*) 0, pDocument, pDocHtml, pCtx, true) ;
  DocVisuDoc.setCloneStatus(true) ;
	if (false == DocVisuDoc.isValidDoc())
	{
    string sErrorMsg = string("Document ") + pDocument->getDocName() + string(" (ID= ") + pDocument->getID() + string(") is not a valid document.") ;
    trace(&sErrorMsg, 1, trError) ;
    return false ;
	}

  return DocVisuDoc.exportFile(sDocName, string("HTM")) ;
}

bool
NSSuper::ExportDocAsHtmlForIMG(NSDocumentInfo* pDocument, NSContexte* pCtx, string sDocName)
{
  if ((NSDocumentInfo*) NULL == pDocument)
		return false ;

  NSDocumentInfo *pDocHtml = (NSDocumentInfo *) 0 ;

	NSRefDocument DocVisuDoc((TDocument*) 0, pDocument, pDocHtml, pCtx, true) ;
  DocVisuDoc.setCloneStatus(true) ;
	if (false == DocVisuDoc.isValidDoc())
	{
    string sErrorMsg = string("Document ") + pDocument->getDocName() + string(" (ID= ") + pDocument->getID() + string(") is not a valid document.") ;
    trace(&sErrorMsg, 1, trError) ;
    return false ;
	}

  return DocVisuDoc.exportFile(sDocName, string("HTM")) ;
}

NSHtmlToPdfConverter*
NSSuper::getHtmlToPdfConverter()
{
  if ((NSHtmlToPdfConverter*) NULL == _pHtmlToPdfConverter)
    _pHtmlToPdfConverter = new NSHtmlToPdfConverter(this) ;

  return _pHtmlToPdfConverter ;
}

voidNSSuper::NavigationEncyclopedie(string sUrl, NSContexte* pCtx)
{
try
{
#ifndef __EPIPUMP__

	NSDocumentInfo Document(pCtx) ;
  Document.setFichier(sUrl) ;
  NSRefDocument *pDocVisu = new NSRefDocument(0, &Document, 0, pCtx, true) ;

	NSDocViewManager dvManager(_pSuperContext) ;
	dvManager.createView(pDocVisu, "Visual Format") ;

#endif // !__EPIPUMP__
}
catch (...)
{
	erreur("Exception NavigationEncyclopedie.", standardError, 0) ;
}
}

void
NSSuper::NavigationCertificat(string sUrl, NSContexte* pCtx)
{
try
{
#ifndef __EPIPUMP__

	NSDocumentInfo Document(pCtx) ;
  Document.setFichier(sUrl) ;
  NSRefDocument *pDocVisu = new NSRefDocument(0, &Document, 0, pCtx, true) ;

	NSDocViewManager dvManager(_pSuperContext) ;
	dvManager.createView(pDocVisu, "Visual Format") ;

#endif // !__EPIPUMP__
}
catch (...)
{
	erreur("Exception NavigationEncyclopedie.", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------
// ATTENTION : Mettre ces fonctions inline pose des problèmes dans les DLL
// -----------------------------------------------------------------------------
short
NSSuper::donneStatut()
{
  return _statut ;
}


hDBIDb
NSSuper::PrendPDatabase()
{
	return _hNSdb ;
}


/*
#if defined(_INCLUS)
void
NSSuper::EnableBWCC(bool enable, uint language)
{
  if (enable)
  {
    if (!BWCCModule)
    {
      try
      {
        BWCCModule = new TBwccDll() ;
        BWCCModule->IntlInit(language) ;
        TModule *pModule = pMainWindow->GetModule() ;
        BWCCModule->Register(pModule->GetInstance()) ;

        WARNX(OwlApp, BWCCModule->GetVersion() < BWCCVERSION, 0,  "Old version of BWCC DLL found") ;
      }
      catch (...)
      {
        TRACEX(OwlApp, 0, "Unable to create instance of TBwccDll") ;
        return ;
      }
    }
  }
  BWCCOn = enable ;
}
#endif
*/

/*
vector<string>
NSSuper::getLocalTeams()
{
  string serviceName = NautilusPilot::SERV_SEARCH_OBJECT_HAVING_TRAITS ;
  vector<string> vRes ;

  NSPersonsAttributesArray ResultList ;
  NSBasicAttributeArray		 AttrArray ;
  AttrArray.push_back(new NSBasicAttribute("trait", LOCALTEAM)) ;

  bool listOk = _pPilot->objectList(serviceName.c_str(), &ResultList, &AttrArray) ;

  if ((false == listOk) || (ResultList.empty()))
		return vRes ;

  for (NSPersonsAttributeIter personAttrIter = ResultList.begin() ; personAttrIter != ResultList.end() ; personAttrIter++)
  {
    if (false == (*personAttrIter)->empty())
    {
    	for (NSBasicAttributeIter basicAttrIter = (*personAttrIter)->begin() ; basicAttrIter != (*personAttrIter)->end() ; basicAttrIter++)
    	{
    		if ((*basicAttrIter)->getBalise() == OIDS)
      	{
      		string sObjectID = (*basicAttrIter)->getText() ;
        	vRes.push_back(sObjectID) ;
        }
      }
    }
  }

  return vRes ;
}
*/

void
NSSuper::initInstanceCounters()
{
  NSSuperRoot::initSRNbInstance() ;
  NSRoot::initRootNbInstance() ;

  NSFiche::initNbInstance() ;

  // Counters from nsperson.h
  NSPatInfo::initNbInstance() ;
	NSCorrespondantInfo::initNbInstance() ;

	// Counters from NautilusPilot.hpp
	NSBasicAttribute::initNbInstance() ;
	NSBasicAttributeArray::initNbInstance() ;
	NSPersonsAttributesArray::initNbInstance() ;

  // counters from nsmanager.h
	NSDataTree::initNbInstance() ;
	NSDataTreeArray::initNbInstance() ;
  NSArcLink::initNbInstance() ;
	NSArcLinkArray::initNbInstance() ;
  NSNodeRight::initNbInstance() ;
	NSNodeRightArray::initNbInstance() ;
	NSDataGraph::initNbInstance() ;
	NSObjectGraphManager::initNbInstance() ;
	NSPersonGraphManager::initNbInstance() ;

  // counters from nsldvuti.h
	NVLdVTemps::initNbInstance() ;
  NVLdVPoint::initNbInstance() ;
  NVLdVRect::initNbInstance() ;
  NSPixel::initNbInstance() ;

  // counters from nspatpat.h
	NSPatPathoInfo::initNbInstance() ;
	NSPatPathoArray::initNbInstance() ;
	NSVectPatPathoArray::initNbInstance() ;
  NSFatheredPatPathoArray::initNbInstance() ;
  NSVectFatheredPatPathoArray::initNbInstance() ;

  NSTreeNode::initNbInstance() ;

  NSDocumentInfo::initNbInstance() ;
  NSRefDocument::initNbInstance() ;
  NSNoyauDocument::initNbInstance() ;
  NSDocumentHisto::initNbInstance() ;
  NSDocHistoArray::initNbInstance() ;
  NSDocumentsTank::initNbInstance() ;
  NSHISTODocument::initNbInstance() ;

  // counters from nsmodhtm.h
	NSModHtml::initNbInstance() ;
  NSBlocHtml::initNbInstance() ;
  NSHtml::initNbInstance() ;

  BBFilsItem::initNbInstance() ;
  BBFilsArray::initNbInstance() ;
  BBItem::initNbInstance() ;
  NSTransferInfo::initNbInstance() ;
  Message::initNbInstance() ;
  NSControle::initNbInstance() ;

  NSHealthTeam::initNbInstance() ;

  Cbalise::initNbInstance() ;
  CAttributs::initNbInstance() ;
  CValeur::initNbInstance() ;

  NSHistoryValueManagement::initNbInstance() ;

  // counters from nsldvdoc.h and nsldgoal.h
  NSConcernModifier::initNbInstance() ;
  NSConcern::initNbInstance() ;
  NSLdvSousObjet::initNbInstance() ;
  NSLdvObjet::initNbInstance() ;
  NSLdvDrugTake::initNbInstance() ;
  NSLdvDrugCycle::initNbInstance() ;
  NSLdvDrugPhase::initNbInstance() ;
  NSLdvDrug::initNbInstance() ;
  NSFrameInformation::initNbInstance() ;
  NSLdvDocument::initNbInstance() ;

  NSLdvGoalInfo::initNbInstance() ;
  NSDelayZone::initNbInstance() ;
  NSDateZone::initNbInstance() ;
  NSValueZone::initNbInstance() ;
  NSLdvGoal::initNbInstance() ;

  NSLdvTimeToon::initNbInstance() ;
  NSPbModifView::initNbInstance() ;
  NSLigneView::initNbInstance() ;
  NSConcernView::initNbInstance() ;
  NSPreoccupView::initNbInstance() ;
  NSEventView::initNbInstance() ;
  NSDrugLineView::initNbInstance() ;
  NSGoalLineView::initNbInstance() ;
  NSBaseLineView::initNbInstance() ;
  NSSsObjView::initNbInstance() ;
  NSLdvCurvePoint::initNbInstance() ;
  NSLdvObjetView::initNbInstance() ;
  NSQualityOfLifeChartView::initNbInstance() ;
  NSLdvGoalCycleView::initNbInstance() ;
  NSLdvTankView::initNbInstance() ;
  NSTimeZoomLevel::initNbInstance() ;
  NSLdvView::initNbInstance() ;
  NSLdvViewArea::initNbInstance() ;
  NSLdvViewAreaWindow::initNbInstance() ;

  NSLdvToon::initNbInstance() ;

  NSLdvWindowToon::initNbInstance() ;
  NSLdvCommandPannel::initNbInstance() ;
  NSLdvCommandPannelInterface::initNbInstance() ;
  NSLdvChronoLine::initNbInstance() ;
  NSLdvChronoLineInterface::initNbInstance() ;
  NSLdvCurveYAxis::initNbInstance() ;
  NSLdvCurveYAxisInterface::initNbInstance() ;
  NSLdvCurveBackgroundQuadri::initNbInstance() ;
  NSLdvCurveBackground::initNbInstance() ;
  NSCurvesProperty::initNbInstance() ;
  NSCurvesPropertyWindow::initNbInstance() ;
  NSLdvCurve::initNbInstance() ;
  NSLdvCurvesManagementPannel::initNbInstance() ;
  NSLdvCurvesManagementPannelInterface::initNbInstance() ;
  NsObjectIcon::initNbInstance() ;
  NSLdvViewIconsShowWindow::initNbInstance() ;

  NSphaseMedic::initNbInstance() ;

  // counters from nscoopchart.h
  NSCoopCardButton::initNbInstance() ;
  NSCoopCard::initNbInstance() ;
  NSCoopLine::initNbInstance() ;
  NSQualityOfLifeInformation::initNbInstance() ;
  NSQualityOfLifeChart::initNbInstance() ;
  NSCoopChart::initNbInstance() ;

  // counters from nsdkd.h
  dkdNoyau::initNbInstance() ;
  gereDate::initNbInstance() ;
  numStorage::initNbInstance() ;
  gereNum::initNbInstance() ;
  gereHeure::initNbInstance() ;
  gereCode::initNbInstance() ;
  NSDkdPhrase::initNbInstance() ;
  decodageBase::initNbInstance() ;
  NSGenerateur::initNbInstance() ;
  NSGenComplement::initNbInstance() ;

  // counters from nscompub.h
  NSChoixPubli::initNbInstance() ;
  NSMapiCtrl::initNbInstance() ;
  NSLdvPubli::initNbInstance() ;
  NSPublication::initNbInstance() ;

  NSDocCompta::initNbInstance() ;

  // counters from nsphrase.h and nsphrobj.h
  NSPhraseObjet::initNbInstance() ;
  NSPhraseMot::initNbInstance() ;
  NSPhraseur::initNbInstance() ;
  NsProposition::initNbInstance() ;

  // counters from ns_search.h
  MappingNSSearchResult::initNbInstance() ;
  NSSearchStruct::initNbInstance() ;
  NSRequestResult::initNbInstance() ;

  // counters from nslocalise.h
  NSLocalisedChapter::initNbInstance() ;

  // counters from nsarc.h
  nsGenericParseur::iniGenerictNbInstance() ;
  nsarcParseur::initNbInstance() ;
  nsrefParseur::initNbInstance() ;
  nsGuidelineParseur::initNbInstance() ;
  NSValidateur::initNbInstance() ;
  NSDrufRefFileManager::initNbInstance() ;

  // counters from nsCsvParser.h
  NSCsvField::initNbInstance() ;
  NSCsvRecord::initNbInstance() ;
  NSCsvParser::initNbInstance() ;

  NSCapture::initNbInstance() ;
  NSCaptureArray::initNbInstance() ;
  analysedCapture::initNbInstance() ;
}

NSContexte::NSContexte(NSSuper* pSuper)
           :NSSuperRoot(pSuper),
            _pPatient((NSPatientChoisi*) 0),
            _pUtilisateur((NSUtilisateurChoisi*) 0)
{
  _sTransaction        = string("") ;
  _sTransactionSys     = string("") ;
  _sBaseDirectory      = string("") ;
  _pGraphPerson        = (NSPersonGraphManager*) 0 ;
  _pAllTraits          = (NSBasicAttributeArray*) 0 ;
  _pWebServiceWindow   = (NSWebServiceWindow*) 0 ;
  _pWebServiceChild    = (NSWebServiceChild*) 0 ;
  _pMailBoxWindow      = (NSMailServiceWindow*) 0 ;
  _pMailBoxChild       = (NSMailServiceChild*) 0 ;
  _pAlertBoxWindow     = (NSAlertServiceWindow*) 0 ;
  _pAlertBoxChild      = (NSAlertServiceChild*) 0 ;
  _bBBKToDoLocked      = false ;
  _blackboardInterface = (BB1BBInterface*) 0 ;
  _bb                  = (BB1BB*) 0 ;
  _pDico               = new NSDico(this) ;
  _pBdmDriver          = (NSBdmDriver*) 0 ;

  _pEpisodus           = (NSEpisodus*) 0 ;
  _pPredi              = (NSPredi*) 0 ;

#ifndef __EPIPUMP__
  _pEpisodus           = new NSEpisodus(this) ;
# ifndef __EPIBRAIN__
  _pPredi              = new NSPredi(this) ;

  _iBamType            = btNone ;
  _iDrugSort           = dsName ;
  _bDisplayEaccess     = true ;
  _sBamApplicationID   = string("") ;
  _sBamApplicationKey  = string("") ;

/*
  _sAppName = string("Episodus") ;
  if (_pPredi && (string("") != _pPredi->_sNomModule))
  {
    _sAppName = _pPredi->_sNomModule ;
    if (_pPredi->_sOEMModule != "")
      _sAppName += string("/") + _pPredi->_sOEMModule ;
  }
*/

# endif // !__EPIBRAIN__
#endif // !__EPIPUMP__

  _pSuper->registerContexte(this) ;
}

NSContexte::NSContexte(NSContexte& rv)
           :NSSuperRoot(rv._pSuper),
            _pPatient(rv._pPatient),
            _pUtilisateur(rv._pUtilisateur)
{
  _sTransaction        = rv._sTransaction ;
  _sTransactionSys     = rv._sTransactionSys ;
  _sBaseDirectory      = rv._sBaseDirectory ;
  _pGraphPerson        = rv._pGraphPerson ;
  _pAllTraits          = NULL ;
  _pWebServiceWindow   = rv._pWebServiceWindow ;
  _pWebServiceChild    = rv._pWebServiceChild ;
  _pMailBoxWindow      = rv._pMailBoxWindow ;
  _pMailBoxChild       = rv._pMailBoxChild ;
  _pAlertBoxWindow     = rv._pAlertBoxWindow ;
  _pAlertBoxChild      = rv._pAlertBoxChild ;
  _bBBKToDoLocked      = false ;
  _blackboardInterface = rv._blackboardInterface ;
  _bb                  = rv._bb ;
  _pDico               = new NSDico(*(rv._pDico)) ;

  _pBdmDriver          = (NSBdmDriver*) 0 ;
  _pEpisodus           = (NSEpisodus*) 0 ;
  _pPredi              = (NSPredi*) 0 ;

  _iBamType            = rv._iBamType ;
  _iDrugSort           = rv._iDrugSort ;
  _bDisplayEaccess     = rv._bDisplayEaccess ;
  _sBamApplicationID   = rv._sBamApplicationID ;
  _sBamApplicationKey  = rv._sBamApplicationKey ;

#ifndef __EPIPUMP__
  _pEpisodus           = rv._pEpisodus ;
# ifndef __EPIBRAIN__
  _pPredi              = rv._pPredi ;
# endif // !__EPIBRAIN__
#endif // !__EPIPUMP__
}

NSContexte::~NSContexte()
{
  if (_pDico)
    delete _pDico ;
  if (_pBdmDriver)
    delete _pBdmDriver ;

#ifndef __EPIPUMP__
  if (_pEpisodus)
    delete _pEpisodus ;
# ifndef __EPIBRAIN__
  delete _pPredi ;
# endif // !__EPIBRAIN__
#endif // !__EPIPUMP__
}

void
NSContexte::setMainCaption()
{
	if (NULL == _pSuper)
		return ;

	TWindow* pMainWindow = _pSuper->donneMainWindow() ;
	if ((NULL == pMainWindow) || (!(pMainWindow->IsWindow())))
		return ;

	string sCaption = _pSuper->_sAppName ;

  sCaption += string(" (") + _pSuper->getInstanceString() + string(")") ;

	if (_pUtilisateur)
		sCaption += string(" ") + _pUtilisateur->donneSignature(this) ;

  string sLang = getUserLanguage() ;

	// si on a un patient ouvert, on ajoute la caption patient
	// sans créer de nouvelle contribution
	if (_pPatient)
	{
    // on fabrique le nom long
    _pPatient->fabriqueNomLong() ;

    // On ajoute le nom et l'age du patient au titre de l'application
    sCaption += string(" - ") + _pPatient->getNomLong() ;

    string sPatientAge = _pPatient->fabriqueAgeLabel(sLang) ;
    if (string("") != sPatientAge)
    	sCaption += string(" (") + sPatientAge + string(" - ") + _pPatient->donneDateNaiss(sLang) + string(")") ;

    string sPatientCode = _pPatient->getCode() ;
    if (string("") != sPatientCode)
    	sCaption += string(" ") + sPatientCode ;

    string sPatientIpp = _pPatient->getIpp() ;
    if (string("") != sPatientIpp)
    	sCaption += string(" ") + sPatientIpp ;
	}

	pMainWindow->SetCaption(sCaption.c_str()) ;
}

void
NSContexte::resetBdmDriver()
{
  if (_pBdmDriver)
    delete _pBdmDriver ;

  if (NSContexte::btNone == _iBamType)
  {
    _pBdmDriver = (NSBdmDriver*) 0 ;
    return ;
  }

  _pBdmDriver = new NSBdmDriver(this) ;

  // Test the connexion by asking its version number
  //
  string sBamVersionId = _pBdmDriver->getBamVersionId() ;

  CURLcode lastRestError = _pBdmDriver->getLastRestError() ;

  if ((string("") != sBamVersionId) && (CURLE_OK == lastRestError))
  {
    string sMsg = string("Connected to drug database ") + sBamVersionId ;
    _pSuper->trace(&sMsg, 1, NSSuper::trSteps) ;
    return ;
  }

  // Error message
  //
  string sMsg = string("Failed to connect to drug database") ;
  string sErr = _pSuper->getText("drugEngineManagement", "FailedToConnect") ;

  switch(lastRestError)
  {
    case  2 : sMsg += string(" (the communication engine could not initialize).") ;
              sErr += string(" (the communication engine could not initialize).") ;
              break ;
    case  3 : sMsg += string(" (URL d'appel malformée).") ;
              sErr += string(" (URL d'appel malformée).") ;
              break ;
    case  5 : sMsg += string(" (could not resolve proxy).") ;
              sErr += string(" (") + _pSuper->getText("drugEngineManagement", "ErrorCouldNotResolveProxy") + string(").") ;
              break ;
    case  6 : sMsg += string(" (could not resolve host - check web connection).") ;
              sErr += string(" (") + _pSuper->getText("drugEngineManagement", "ErrorCouldNotResolveHost") + string(").") ;
              break ;
    case  7 : sMsg += string(" (could not connect - is the service down?).") ;
              sErr += string(" (") + _pSuper->getText("drugEngineManagement", "ErrorCouldNotConnect") + string(").") ;
              break ;
    default : sMsg += string(" (error ") + IntToString(lastRestError) + string(").") ;
              sErr += string(".") ;
  }

  _pSuper->trace(&sMsg, 1, NSSuper::trError) ;
  erreur(sErr.c_str(), standardError, 0) ;
}

boolNSContexte::captureData(NSCaptureArray *pCapture)
{
  if (((NSCaptureArray*) NULL == pCapture) || pCapture->empty())
		return false ;

try
{
  if (((NSUtilisateurChoisi*) NULL == _pUtilisateur) || (false == _pUtilisateur->isUser()))
		return false ;

  string sMsg = string("Looking for a patient from captured information") ;
  _pSuper->trace(&sMsg, 1, NSSuper::trSteps) ;

  // Did we get an IPP? If yes, try to open patient with such IPP
  //
  string sIPP = string("") ;
  pCapture->trouveChemin(string("0LIBI1/LSITE1/LIPP01"), &sIPP) ;

  if (string("") != sIPP)
  {
    sMsg = string("Looking for a patient with IPP=") + sIPP ;
    _pSuper->trace(&sMsg, 1, NSSuper::trSubSteps) ;

    NSPatInfo* pPatWithIpp = _pUtilisateur->getPatientForIpp(sIPP) ;
    if (pPatWithIpp)
    {
      sMsg = string("Patient found, leaving") ;
      _pSuper->trace(&sMsg, 1, NSSuper::trSubSteps) ;

      _pEpisodus->PutNewCaptureInTank(true /*bEmptyTankFirst*/, true /*bool bResetNew*/) ;

      _pUtilisateur->AutomaticOpenOrCreate(pPatWithIpp) ;
      delete pPatWithIpp ;
      return true ;
    }

    sMsg = string("No patient found with IPP=") + sIPP ;
    _pSuper->trace(&sMsg, 1, NSSuper::trSubSteps) ;
  }

	// Si on n'a pas capturé le nom, on ajoute simplement les nouvelles données
	// capturées à l'array générale
	//
	string sNom       = string("") ;
  string sPrenom    = string("") ;

  pCapture->getIdentity(sNom, sPrenom) ;

	if (string("") == sNom)
	{
    sMsg = string("No name captured, leaving") ;
    _pSuper->trace(&sMsg, 1, NSSuper::trSubSteps) ;

#ifndef __EPIPUMP__
		if ((NSEpisodus*) NULL == _pEpisodus)
			return false ;

		// NSCaptureArray* pCaptureArray = &(pSuper->pEpisodus->CaptureArray) ;
		// pCaptureArray->append(pCapture) ;
		// pCapture->vider() ;

    _pEpisodus->PutNewCaptureInTank(false /*bEmptyTankFirst*/, true /*bool bResetNew*/) ;
    _pEpisodus->getCaptureDoc()->openView() ;

		return true ;
#else
		return false ;
#endif // !__EPIPUMP__
	}

	// Si on a au moins capturé le nom, il est possible d'identifier/créer le patient
	//
	string sSexe  = string("") ;	string sNaiss = string("") ;
	// On cherche les autres éléments administratifs : prénom, sexe, date de naissance	pCapture->trouveChemin(string("LSEXE1"), &sSexe) ;
	pCapture->trouveChemin(string("KNAIS1"), &sNaiss) ;

	// on enlève les blancs terminaux des nom et prénom
	strip(sNom, stripRight) ;
	strip(sPrenom, stripRight) ;

	char nomPat[PIDS_NOM_LEN + 1] ;
	char prenomPat[PIDS_PRENOM_LEN + 1] ;
	strcpy(nomPat, sNom.c_str()) ;
	strcpy(prenomPat, sPrenom.c_str()) ;

  if ((NSPatInfo*) NULL == _pUtilisateur->getPatRech())
    _pUtilisateur->setPatRech(new NSPatInfo(this)) ;
  NSPatInfo* pPatEnCours = _pUtilisateur->getPatRech() ;

	char szInstance[3] ;
	int iInstance = getSuperviseur()->_numInstance ;
	itoa(iInstance, szInstance, 10) ;

	// SetCursor(0, IDC_WAIT) ;
	string user = getUtilisateurID() ;

  bool bUseBirthdateAsTrait = true ;

	NSBasicAttributeArray AttrArray  ;

  sMsg = string("Looking for a patient with name=\"") + string(nomPat) + string("\", firstname=\"") + string(prenomPat) + string("\" and birthdate=\"") + sNaiss + string("\"") ;
  _pSuper->trace(&sMsg, 1, NSSuper::trSubSteps) ;

	AttrArray.push_back(new NSBasicAttribute(LAST_NAME,  string(nomPat))) ;
  AttrArray.push_back(new NSBasicAttribute(FIRST_NAME, string(prenomPat))) ;
  AttrArray.push_back(new NSBasicAttribute(BIRTHDATE,  sNaiss)) ;

  NSPersonsAttributesArray PatiensList ;

  bool bListOk = _pSuper->getPilot()->personList((NautilusPilot::SERV_PERSON_LIST_FROM_TRAITS).c_str(), &PatiensList, &AttrArray) ;
	if (false == bListOk)
	{
		std::string tempMessage = _pSuper->getPilot()->getWarningMessage() ;
		std::string tempError   = _pSuper->getPilot()->getErrorMessage() ;
		if (tempMessage != "")
    {
      _pSuper->trace(&tempMessage, 1, NSSuper::trWarning) ;
			::MessageBox(GetMainWindow()->GetHandle(), tempMessage.c_str(), "Message Nautilus", MB_OK) ;
    }
		if (tempError != "")
    {
      _pSuper->trace(&tempError, 1, NSSuper::trError) ;
			::MessageBox(GetMainWindow()->GetHandle(), tempError.c_str(), "Message Nautilus", MB_OK) ;
    }
		return false ;
	}

  if (PatiensList.size() > 1)
	{
    sMsg = string("Several patients found with traits including birthdate, leaving") ;
    _pSuper->trace(&sMsg, 1, NSSuper::trSubSteps) ;

  	_pUtilisateur->AppelPatient() ;
    return true ;
	}

  // Nobody found... searching without birthdate as a trait
  //
	if (true == PatiensList.empty())
	{
    // New algorithm : if no patient with same 1st name, 2nd names and birthday
    //                 then always ask user
    //
    sMsg = string("No patient found with traits including birthdate, leaving") ;
    _pSuper->trace(&sMsg, 1, NSSuper::trSubSteps) ;

    _pUtilisateur->AppelPatient() ;
    return true ;

    sMsg = string("No patient found with traits including birthdate, searching with only name and firstname") ;
    _pSuper->trace(&sMsg, 1, NSSuper::trSubSteps) ;

  	AttrArray.vider() ;
  	AttrArray.push_back(new NSBasicAttribute(LAST_NAME,  string(nomPat))) ;
  	AttrArray.push_back(new NSBasicAttribute(FIRST_NAME, string(prenomPat))) ;

  	NSPersonsAttributesArray PatiensList ;

  	bool bListOk = _pSuper->getPilot()->personList((NautilusPilot::SERV_PERSON_LIST_FROM_TRAITS).c_str(), &PatiensList, &AttrArray) ;
		if (false == bListOk)
		{
			std::string tempMessage = _pSuper->getPilot()->getWarningMessage() ;
			std::string tempError   = _pSuper->getPilot()->getErrorMessage() ;
			if (string("") != tempMessage)
      {
        _pSuper->trace(&tempMessage, 1, NSSuper::trWarning) ;
				::MessageBox(GetMainWindow()->GetHandle(), tempMessage.c_str(), "Message Nautilus", MB_OK) ;
      }
			if (string("") != tempError)
      {
        _pSuper->trace(&tempError, 1, NSSuper::trError) ;
				::MessageBox(GetMainWindow()->GetHandle(), tempError.c_str(), "Message Nautilus", MB_OK) ;
      }
			return false ;
		}

    // Nobody or several persons found: let the user digg
    //
    if ((true == PatiensList.empty()) || (PatiensList.size() > 1))
		{
      if (true == PatiensList.empty())
        sMsg = string("No patient found without birthdate, leaving") ;
      else
        sMsg = string("Several patients found without birthdate, leaving") ;
      _pSuper->trace(&sMsg, 1, NSSuper::trSubSteps) ;

  		_pUtilisateur->AppelPatient() ;
    	return true ;
		}

    bUseBirthdateAsTrait = false ;
	}

  if (getPatient())
  {
    _pUtilisateur->ClosePatientAsynchronous(NSUtilisateurChoisi::cpnOpenFromCapture) ;
    return true ;

		// if (false == _pUtilisateur->fermePatEnCours())
    //  return false ;
  }
  else
    _pEpisodus->PutNewCaptureInTank(true /*bEmptyTankFirst*/, true /*bool bResetNew*/) ;

	// This Service must only be called when there is only one person that fits
  // provided traits - or the Pilot will crash!
  //
  sMsg = string("A single patient was found, we can open it") ;
  _pSuper->trace(&sMsg, 1, NSSuper::trSubSteps) ;

  AttrArray.vider() ;
  AttrArray.push_back(new NSBasicAttribute(LAST_NAME,  string(nomPat))) ;
  AttrArray.push_back(new NSBasicAttribute(FIRST_NAME, string(prenomPat))) ;
  if (bUseBirthdateAsTrait)
  	AttrArray.push_back(new NSBasicAttribute(BIRTHDATE, sNaiss)) ;
	AttrArray.push_back(new NSBasicAttribute(OPERATOR,   user)) ;
	AttrArray.push_back(new NSBasicAttribute(CONSOLE,    string(getSuperviseur()->_noConsole))) ;
	AttrArray.push_back(new NSBasicAttribute(INSTANCE,   string(szInstance))) ;
  NSPersonsAttributesArray List ;
  bool res = _pSuper->getPilot()->searchPatient(NautilusPilot::SERV_SEARCH_PATIENT_FROM_TRAITS.c_str(),
                                    pPatEnCours->getGraphPerson()->getDataGraph(), &List, &AttrArray) ;
  if (false == res)
  {
  	string tempMessage = _pSuper->getPilot()->getWarningMessage() ;
    ::MessageBox(GetMainWindow()->GetHandle(), tempMessage.c_str(), "Message Nautilus", MB_OK) ;
    return false ;
	}

  if (((NSDataGraph*) NULL == pPatEnCours->getGraphPerson()->getDataGraph()) ||
  		              (false == pPatEnCours->getGraphPerson()->graphPrepare()))
  	return false ;

  pPatEnCours->getGraphPerson()->setNeedUnlock(false) ;
  pPatEnCours->getGraphPerson()->setReadOnly(true) ;

  //
  // Personne ne correspond aux traits, ou plusieurs personnes correspondent
  // Nobody - or several persons - share these traits
  //
  if (List.empty() || (pPatEnCours->getGraphPerson()->getDataGraph()->getGraphID() == ""))
  {
  	_pUtilisateur->AppelPatient() ;
    return true ;
	}

  string sIsLocked = List.getAttributeValue("locked") ;
  if (sIsLocked == "ok")
  	pPatEnCours->getGraphPerson()->setNeedUnlock(true) ;
  string sOperationType	= List.getAttributeValue("operationType") ;
  if (sOperationType == "readWrite")
  	pPatEnCours->getGraphPerson()->setReadOnly(false) ;

  AttrArray.push_back(new NSBasicAttribute(PERSON , pPatEnCours->getGraphPerson()->getPersonID())) ;
  pPatEnCours->getGraphPerson()->setInfoPids(&AttrArray) ;
  pPatEnCours->getGraphPerson()->getDataGraph()->setLastTree() ;

	string sCodePat = pPatEnCours->getGraphPerson()->getRootTree() ;
	// NSDataGraph* pGraph = pPatEnCours->pGraphPerson->pDataGraph ;

	// SetCursor(0, IDC_ARROW) ;

	_pUtilisateur->ChangePatientChoisi() ;
	_pUtilisateur->setPatRech((NSPatInfo*) 0) ;
	_pPatient->InitialisationAsynchronous(NSPatientChoisi::opnUpdateFromCapture) ;
	return true ;
}
catch (...)
{
  erreur("Exception NSContexte::captureData.", standardError, 0) ;
  return false ;
}
}

void
NSSuper::creePilot()
{
try
{
  if (_pNSApplication)
  {
	  _pNSApplication->sendMessageToSplash("Create Pilot") ;
    _pNSApplication->setSplashPercentDone(15) ;
  }

#ifndef __EPIPUMP__
	string sVerNum = sNumVersion ;
	string ps = string("Starting Pilot") ;
  trace(&ps, 1) ;
#endif

  _pPilot = new NautilusPilot(this) ;

#ifndef __EPIPUMP__
  _pPilot->setEpisodusVersion(sVerNum) ;
  //pPilot->markIncorrectAgents() ;
  _pNSApplication->sendMessageToSplash("Init agent list") ;
  _pNSApplication->setSplashPercentDone(17) ;

  _pPilot->initAgentList() ;

  _pNSApplication->sendMessageToSplash("Set services list") ;
  _pNSApplication->setSplashPercentDone(20) ;

  _pPilot->setServiceList() ;
#endif
}
catch (...)
{
  erreur("Exception NSSuper::creePilot.", standardError, 0) ;
  return ;
}
}

void
NSSuper::parseExecArgs(int argc, char* argv[])
{
  if ((0 == argc) || (NULL == argv) || (NULL == pmExecArgs))
    return ;

  // By convention, first parameter is the executable path
  //
  (*pmExecArgs)[string("ExecString")] = argv[0] ;

  // Getting other params in the form field=value
  //
  for (int i = 1 ; i < argc ; i++)
  {
    string sArgument = string(argv[i]) ;

    strip(sArgument, stripBoth) ;
    if (string("") != sArgument)
    {
      size_t iCursor  = 0 ;
      size_t iArgSize = strlen(sArgument.c_str()) ;

      // if it starts by " then look for = after closing "
      //
      if ('\"' == sArgument[0])
      {
        iCursor = sArgument.find('\"', 1) ;
        if (NPOS == iCursor)
          iCursor = 0 ;
      }
      iCursor = sArgument.find('=', iCursor) ;
      if (NPOS != iCursor)
      {
        string sFields[2] = { "" , "" } ;

        sFields[0] = string(sArgument, 0, iCursor) ;
        if (iCursor < iArgSize - 1)
          sFields[1] = string(sArgument, iCursor + 1, iArgSize - iCursor - 1) ;

        for (size_t j = 0 ; j < 2 ; j++)
        {
          strip(sFields[j], stripBoth) ;
          size_t iFieldSize = strlen(sFields[j].c_str()) ;
          if ((iFieldSize >= 2) && ('\"' == sFields[j][0]) && ('\"' == sFields[j][iFieldSize - 1]))
          {
            if (iFieldSize == 2)
              sFields[j] = string("") ;
            else
              sFields[j] = string(sFields[j], 1, iFieldSize - 2) ;
          }
        }

        if ((string("") != sFields[0]) && (string("") != sFields[1]))
        {
          (*pmExecArgs)[sFields[0]] = sFields[1] ;

          string sMsg = string("Exec param: ") + sFields[0] + string(" -> ") + sFields[1] ;
          trace(&sMsg, 1, trWarning) ;
        }
      }
    }
  }
}

void
NSContexte::setUtilisateur(NSUtilisateurChoisi* pUti)
{
	_pUtilisateur = pUti ;

#ifndef __EPIPUMP__
	// reset captured informations
  if (_pEpisodus)
	{
  	_pEpisodus->deleteCaptureDoc() ;

    if (pUti)
    	_pEpisodus->createCaptureDoc() ;
	}
#endif
}

