// -----------------------------------------------------------------------------
// nschoisi.cpp
// -----------------------------------------------------------------------------
// $Revision: 1.53 $
// $Author: pameline $
// $Date: 2017/04/10 21:06:59 $
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Copyright Nautilus, 2001-2005
// http://www.nautilus-info.com
// -----------------------------------------------------------------------------
// Ce logiciel est un programme informatique servant à [rappeler les
// caractéristiques techniques de votre logiciel].
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
// This software is a computer program whose purpose is to [describe
// functionalities and technical features of your software].
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

#include <mem.h>
#include <owl\olemdifr.h>

#include <owl\opensave.h>

#include "nssavoir\nsBdmDriver.h"
#include "nautilus\nssuper.h"
#include "nautilus\nsrechdl.h"
#include "nautilus\nsrechd2.h"
#include "partage\nsdivfct.h"
#include "partage\nsdivfile.h"
#include "nautilus\nsadmiwd.h"
#include "nsbb\nsbb.h"
#include "nsdn\nsintrad.h"
#include "nsdn\nsframedocs.h"
#include "nautilus\nsttx.h"
#include "nautilus\nsacroread.h"
#include "nautilus\nscrvue.h"
#include "nautilus\nscsdoc.h"
#include "nautilus\nscsvue.h"
#include "nautilus\nshistor.h"
#include "nautilus\nsresour.h"  // pour le dialogue de la fiche admin
#include "nssavoir\nstransa.h"
#include "nautilus\nscompub.h"

#include "nssavoir\nsbloque.h"
#include "nautilus\nspatdlg.h"
#include "nautilus\nsmodhtm.h"
#include "nautilus\nsbrowse.h"
#include "nautilus\nsGrabObjects.h"
#include "nautilus\nsdocaga.h"	// pour les documents de compta et pDocRefHisto
#include "nautilus\nschoisi.h"
#include "nautilus\nsepicap.h"
#include "nautilus\nsdocview.h"
#include "nautilus\nsldvvue.h"
#include "nautilus\nsdrugview.h"
#include "nautilus\nsprocessview.h"
#include "nautilus\nsFollowUpView.h"
#include "nautilus\nsgoalview.h"
#include "nautilus\nsrcview.h"
#include "nautilus\nscqdoc.h"
#include "nautilus\nscqvue.h"
#include "nautilus\nsMailSvce.h"
#include "nautilus\ns_html.h"
#include "ns_grab\nsgrabfc.h"
#include "nsbb\nsarc.h"
#include "nsbb\nsarcCtrls.h"
#include "nsbb\nsfltbtnga.h"
#include "nsbb\nsbbitem.h"
#include "nsbb\nsbbtran.h"
//#include "nsoutil\nssmedit.h"
#include "nsoutil\nsarcedit.h"
// #include "nsoutil\nsInterfaces.h"
#include "nautilus\nsInterface.h"
#include "nsldv\nsldvuti.h"
#include "nssavoir\nsgraphe.h"
#include "dcodeur\decoder.h"
#include "nsbb\nsdefArch.h"
#include "nautilus\nsTeamDocView.h"
#include "nautilus\nsVenueDocView.h"
#include "nautilus\nsCoopChartDV.h"
#include "nautilus\nsVisual.h"
#include "nautilus\nsmdiframe.h"
#include "nsepisod\nsPilotProxy.h"
#include "nsepisod\nsToDo.h"

# include "ns_ob1\nautilus-bbk.h"

#include "nsbb\nsmpids.h"
#include "nsbb\nsmanager.h"
#include "nsbb\logpass.h"

# include "pilot\NautilusPilot.hpp"

# include "nsbb\tagNames.h"
# include "nsbb\nsattvaltools.h"

#include "docking\stripeDock.h"

//***************************************************************************
// Implémentation des mèthodes NSPatientChoisi
//***************************************************************************

//---------------------------------------------------------------------------
//  Function :  	NSPatientChoisi::~NSPatientChoisi()
//
//  Arguments :
//
//  Description :	Destructeur
//
//  Returns :		Rien
//---------------------------------------------------------------------------
NSPatientChoisi::~NSPatientChoisi()
{
  string sMsg = string("Entering NSPatientChoisi destructor") ;
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;

	if (getADebloquer())
		debloquer() ;

  // Fermeture des objets de la Ligne de vie
#ifndef __EPIPUMP__
#ifndef __EPIBRAIN__

  // Should be done in ~NSPatInfo() but not possible yet due to linking dependencies
  //
/*  Documents are automatically deleted when their last vue closes
  if (_pDocHis)
    delete _pDocHis ;

	if (_pDocLdv)
		delete _pDocLdv ;

  if (_pHealthDoc)
    delete _pHealthDoc ;

  if (_pVenuesDoc)
    delete _pVenuesDoc ;

	if (_pSynthese)
		delete _pSynthese ;
*/
#endif // !__EPIBRAIN__
#endif // !__EPIPUMP__
}

void NSPatientChoisi::CmWebExport()
{
  if ((NSHISTODocument*) NULL == _pDocHis)
    return ;

  NSDocHistoArray* pDocuments = _pDocHis->getVectDocument() ;

  if (((NSDocHistoArray*) NULL == pDocuments) || pDocuments->empty())
	{
    string sErrorText = pContexte->getSuperviseur()->getText("documentManagementErrors", "emptyHistoryManager") ;
    erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
		return ;
	}

  string sLang = pContexte->getUserLanguage() ;

  string sPathHtml = pContexte->PathName("EHTM") ;

  NSHtmlHistoIndex htmlIndex(toExporter, pContexte) ;

  DocumentIter iterDoc = pDocuments->begin() ;
  for ( ; pDocuments->end() != iterDoc ; iterDoc++)
  {
    string sDocLabel   = (*iterDoc)->getDocTitleWithDate() ;
    string sAdrCorresp = string("") ;

    string sCreateDate   = (*iterDoc)->getDateDoc() ;
    string sFormatedTime("") ;
    if (string("") != sCreateDate)
    {
      string sTimeFormat = pContexte->getSuperviseur()->getText("0localInformation", "timeFormat") ;
      sFormatedTime = getFormatedTime(sCreateDate, sLang, sTimeFormat, "") ;
    }

    // Pdf files are exported "as is"
    //
    if ((*iterDoc)->getTypeSem() == string("ZTPDF"))
    {
      string sOriginalFilPath = pContexte->PathName((*iterDoc)->getLocalis()) ;
      string sOriginalFileName = sOriginalFilPath + (*iterDoc)->getFichier() ;

      string sPathExport = pContexte->PathName("EHTM") ;
      string sFileName   = (*iterDoc)->getID() + string(".pdf") ;
      string sTargetName = sPathExport + sFileName ;

      if (FALSE == CopyFile(sOriginalFileName.c_str(), sTargetName.c_str(), FALSE))
      {
			  string sErrorText = pContexte->getSuperviseur()->getText("fileErrors", "errorCopyingFile") ;
        sErrorText += string(" ") + sOriginalFileName + string(" -> ") + sTargetName ;

        string sSystemLastErrorMessage = getSystemLastErrorMessage() ;
			  if (string("") != sSystemLastErrorMessage)
          sErrorText += string(" (") + sSystemLastErrorMessage + string(")") ;

			  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
			  erreur(sErrorText.c_str(), warningError, 0, 0) ;
      }
      else
        htmlIndex.ajouteLigne(sDocLabel, sFileName, (*iterDoc)->getID(), sFormatedTime, (*iterDoc)->getDocName()) ;
    }

    // Other documents are exported as HTML files
    //
    else if ((*iterDoc)->getSemCont() != string("ZSYNT")) // Hack for AG Synthèse non exportée
    {
      string sBasicHtmlName = (*iterDoc)->getID() + string(".htm") ;
      string sHtmlName      = sPathHtml + sBasicHtmlName ;

      if (false == pContexte->getSuperviseur()->ExportDocAsHtml(*iterDoc, pContexte, sHtmlName))
      {
        string sOriginalPath = (*iterDoc)->getLocalis() ;
        string sOriginalFile = (*iterDoc)->getFichier() ;

        if ((string("") == sOriginalFile) || (string("") == sOriginalPath))
        {
          string sErrorText = pContexte->getSuperviseur()->getText("publishingManagement", "cannotCreateHtmlExportFile") ;
          sErrorText += string(" (") + sDocLabel + string(")") ;
          pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
          erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
        }
        else
        {
          string sRoot ;
          string sExtension ;
          NsParseExtension(sOriginalFile, sRoot, sExtension) ;

          string sOriginalFileName = pContexte->PathName(sOriginalPath) + sOriginalFile ;

          string sPathExport = pContexte->PathName("EHTM") ;
          string sFileName   = (*iterDoc)->getID() + string(".") + sExtension ;
          string sTargetName = sPathExport + sFileName ;

          if (FALSE == CopyFile(sOriginalFileName.c_str(), sTargetName.c_str(), FALSE))
          {
            string sErrorText = pContexte->getSuperviseur()->getText("fileErrors", "errorCopyingFile") ;
            sErrorText += string(" ") + sOriginalFileName + string(" -> ") + sTargetName ;

            string sSystemLastErrorMessage = getSystemLastErrorMessage() ;
            if (string("") != sSystemLastErrorMessage)
              sErrorText += string(" (") + sSystemLastErrorMessage + string(")") ;

            pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
            erreur(sErrorText.c_str(), warningError, 0, 0) ;
          }
          else
            htmlIndex.ajouteLigne(sDocLabel, sFileName, (*iterDoc)->getID(), sFormatedTime, (*iterDoc)->getDocName()) ;
        }
      }
      else
        htmlIndex.ajouteLigne(sDocLabel, sBasicHtmlName, (*iterDoc)->getID(), sFormatedTime, (*iterDoc)->getDocName()) ;
    }
  }

  string sIndexFileName = sPathHtml + getNss() + string("_index.htm") ;

  string sBaseImg = string("") ;
  htmlIndex.genereHtml(sIndexFileName, sBaseImg, string("dfmodpatindex.htm"), string("")) ;
}

//---------------------------------------------------------------------------
//  Function :  	NSPatientChoisi::ChercheChemise()
//
//  Description :	Choix des documents à visualiser
//---------------------------------------------------------------------------

void NSPatientChoisi::CmChemise()
{
try
{
	ChoixChemiseDialog* ChoixChemiseDl = new ChoixChemiseDialog(pContexte->GetMainWindow(), pContexte);
	ChoixChemiseDl->Execute();
	delete ChoixChemiseDl;
}
catch (...)
{
	erreur("Exception NSPatientChoisi::CmChemise.", standardError, 0) ;
}
}

/**
* New text
*/
void NSPatientChoisi::CmNewTTxt()
{
try
{
#ifndef __EPIPUMP__

	if (getReadOnly())
	{
		string sErrorText = pContexte->getSuperviseur()->getText("privilegeManagement", "patientFileIsReadOnly") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
		return ;
	}

	NSTtxDocument* pNewDocTtx = new NSTtxDocument(0, pContexte) ;
	NSDocViewManager dvManager(pContexte) ;
	dvManager.createView(pNewDocTtx, "Rtf Format") ;

#endif // !__EPIPUMP__
}
catch (...)
{
  erreur("Exception NSPatientChoisi::CmNewTTxt.", standardError, 0) ;
}
}

/**
*	Crée un document de type Word
* Create a Word document
*/
void NSPatientChoisi::CmNewTmpl()
{
try
{
#ifndef __EPIPUMP__

	if (getReadOnly())
	{
		string sErrorText = pContexte->getSuperviseur()->getText("privilegeManagement", "patientFileIsReadOnly") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
		return ;
	}

	NSTtxDocument*   pNewDocTtx = new NSTtxDocument(0, pContexte) ;
  NSDocViewManager dvManager(pContexte) ;
	dvManager.createView(pNewDocTtx, "Word Format") ;

#endif // !__EPIPUMP__
}
catch (...)
{
	erreur("Exception NSPatientChoisi::CmNewTmpl().", standardError, 0) ;
}
}

void NSPatientChoisi::CmOpenTTxt()
{
try
{
#ifndef __EPIPUMP__

	if (getReadOnly())
	{
		string sErrorText = pContexte->getSuperviseur()->getText("privilegeManagement", "patientFileIsReadOnly") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
		return ;
	}

	// on installe ici le point d'entrée de la routine d'importation (temporaire)

	// on choisi d'abord le répertoire par défaut d'importation (IHTM)
	string sPath = pContexte->PathName("NLTR") ;

	TFileOpenDialog::TData initData(OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,
																	"Fichiers texte (*.rtf)|*.rtf;|",
																	0,
																	(char*) sPath.c_str(),
																	"*.rtf");

	int iUserChoice = TFileOpenDialog(pContexte->GetMainWindow(), initData).Execute() ;

	pContexte->reallocateBaseDirectory() ;

	if (IDOK != iUserChoice)
		return ;

	string sFileName = string(initData.FileName);

	NSTtxDocument* pNewDocTtx = new NSTtxDocument(0, pContexte) ;
	pNewDocTtx->SetFichierExterne(sFileName) ;
	NSDocViewManager dvManager(pContexte) ;
	dvManager.createView(pNewDocTtx, "Rtf Format") ;

#endif // !__EPIPUMP__
}
catch (...)
{
  erreur("Exception NSPatientChoisi::CmOpenTTxt.", standardError, 0) ;
}
}

void NSPatientChoisi::CmOpenPdf()
{
try
{
#ifndef __EPIPUMP__

	if (getReadOnly())
	{
		string sErrorText = pContexte->getSuperviseur()->getText("privilegeManagement", "patientFileIsReadOnly") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
		return ;
	}

	// on installe ici le point d'entrée de la routine d'importation (temporaire)

	// on choisi d'abord le répertoire par défaut d'importation (NLTR)
	string sPath = pContexte->PathName("NLTR") ;

	TFileOpenDialog::TData initData(OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,
																	"Adobe acrobat (*.pdf)|*.pdf;|",
																	0,
																	(char*) sPath.c_str(),
																	"*.pdf") ;

	int iResult = TFileOpenDialog(pContexte->GetMainWindow(), initData).Execute() ;

	pContexte->reallocateBaseDirectory() ;

	if (IDOK != iResult)
		return ;

	string sFileName = string(initData.FileName) ;

	NSAcrobatDocument* pNewDocPdf = new NSAcrobatDocument(0, pContexte) ;
	pNewDocPdf->SetFichierExterne(sFileName) ;
	NSDocViewManager dvManager(pContexte) ;
	dvManager.createView(pNewDocPdf, "Pdf Format") ;

#endif // !__EPIPUMP__
}
catch (...)
{
  erreur("Exception NSPatientChoisi::CmOpenPdf.", standardError, 0) ;
}
}

void NSPatientChoisi::CmOpenHtml()
{
try
{
#ifndef __EPIPUMP__

	if (getReadOnly())
	{
		string sErrorText = pContexte->getSuperviseur()->getText("privilegeManagement", "patientFileIsReadOnly") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
		return ;
	}

	// on installe ici le point d'entrée de la routine d'importation (temporaire)

	// on choisi d'abord le répertoire par défaut d'importation (NLTR)
	string sPath = pContexte->PathName("NLTR") ;

	TFileOpenDialog::TData initData(OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,
																	"HTML (*.htm)|*.htm;|",
																	0,
																	(char*) sPath.c_str(),
																	"*.htm") ;

	int iResult = TFileOpenDialog(pContexte->GetMainWindow(), initData).Execute() ;

	pContexte->reallocateBaseDirectory() ;

	if (IDOK != iResult)
		return ;

	string sFileName = string(initData.FileName) ;

	NSHtmlModelDocument* pNewDocHtml = new NSHtmlModelDocument(0, pContexte) ;
  pNewDocHtml->SetModelFile(sFileName) ;
	NSDocViewManager dvManager(pContexte) ;
	dvManager.createView(pNewDocHtml, "Html Format") ;

#endif // !__EPIPUMP__
}
catch (...)
{
  erreur("Exception NSPatientChoisi::CmOpenHtml.", standardError, 0) ;
}
}

void NSPatientChoisi::CmOpenTmpl()
{
try
{
#ifndef __EPIPUMP__

	if (getReadOnly())
	{
		string sErrorText = pContexte->getSuperviseur()->getText("privilegeManagement", "patientFileIsReadOnly") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
		return ;
	}

	// on installe ici le point d'entrée de la routine d'importation (temporaire)

	// on choisi d'abord le répertoire par défaut d'importation (IHTM)
	string sPath = pContexte->PathName("NLTR") ;

	TFileOpenDialog::TData initData(OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,
																	"Fichiers Word (*.doc)|*.doc;*.docx|",
																	0,
																	(char*) sPath.c_str(),
																	"*.doc") ;

	int iUserChoice = TFileOpenDialog(pContexte->GetMainWindow(), initData).Execute() ;

  pContexte->reallocateBaseDirectory() ;

	if (IDOK != iUserChoice)
		return ;

	string sFileName = string(initData.FileName) ;

	NSTtxDocument* pNewDocTtx = new NSTtxDocument(0, pContexte) ;
  pNewDocTtx->SetFichierExterne(sFileName) ;
  NSDocViewManager dvManager(pContexte) ;
	dvManager.createView(pNewDocTtx, "Word Format") ;

#endif // !__EPIPUMP__
}
catch (...)
{
  erreur("Exception NSPatientChoisi::CmOpenTmpl.", standardError, 0) ;
}
}

void
NSPatientChoisi::CmAssistCapture()
{
#ifndef __EPIPUMP__
try
{
	string sRes ;
	bool bOk = lireParam(pContexte->getUtilisateur()->getNss(), "matrox.dat", "Assist", sRes) ;
	if (bOk)
	{
		if (sRes == string("oui"))
		{
			AssistCaptureDialog* pAssist = new AssistCaptureDialog(pContexte->GetMainWindow(), pContexte) ;
			pAssist->Create() ;
			// delete pAssist;
		}
    else if (sRes == string("bmp"))
    {
    	string sFileName = string("Image51.bmp") ;

      pContexte->getSuperviseur()->initGrabModule("ns_grab.dll") ;

      if (!pContexte->getSuperviseur()->getGrabModule())
      {
        erreur("Impossible d'ouvrir la DLL ns_grab.dll.", standardError, -1, pContexte->GetMainWindow()->GetHandle()) ;
        pContexte->getSuperviseur()->afficheStatusMessage("") ;
        return ;
      }

      // Initialisation du contexte Matrox
      bool (FAR *pAdrConvert) (string far *, NSContexte far *) ;
      // Récupération du pointeur sur la fonction ConvertBmpToJpg
      (FARPROC) pAdrConvert = pContexte->getSuperviseur()->getGrabModule()->GetProcAddress(MAKEINTRESOURCE(M_CONVERTBMP)) ;

      if (pAdrConvert == NULL)
      {
        erreur("Impossible de récupérer la fonction ConvertBmpToJpg()", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
        TModule* pGrabMod = pContexte->getSuperviseur()->getGrabModule() ;
        delete pGrabMod ;
        pContexte->getSuperviseur()->setGrabModule(0) ;
        return ;
      }

      bool bOk = ((*pAdrConvert)((string far *)(&sFileName), (NSContexte far *)pContexte)) ;
      if (!bOk)
      {
        erreur("Erreur à la conversion d'une image bmp.", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
        TModule* pGrabMod = pContexte->getSuperviseur()->getGrabModule() ;
        delete pGrabMod ;
        pContexte->getSuperviseur()->setGrabModule(0) ;
        return ;
      }

      ::MessageBox(0, "Conversion terminée...", "Message Nautilus", MB_OK) ;
    }
		else
		{
			CmCapture() ;
		}
	}
}
catch (...)
{
	erreur("Exception NSPatientChoisi::CmAssistCapture.", standardError, 0) ;
}
#endif // #ifndef __EPIPUMP__
}

void
NSPatientChoisi::CmTwainCapture()
{
#ifndef __EPIPUMP__
try
{
	NSTwainWindow*    pTwainWindow = new NSTwainWindow(pContexte->GetMainWindow(), pContexte) ;
	NSTwainGrabChild* pTwainChild  = new NSTwainGrabChild(pContexte, *(pContexte->getSuperviseur()->getApplication()->prendClient()), "Scan", pTwainWindow) ;
	pTwainChild->Create() ;
}
catch (...)
{
	erreur("Exception NSPatientChoisi::CmTwainCapture.", standardError, 0) ;
}
#endif // #ifndef __EPIPUMP__
}

void
NSPatientChoisi::CmCapture(AssistCaptureDialog* pAssist)
{
#ifndef __EPIPUMP__
try
{
	// on regarde si la capture est déjà lancée
	if (pContexte->getSuperviseur()->getMatrox())
	{
		erreur("La capture d'images est en cours...", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
		pContexte->getSuperviseur()->afficheStatusMessage("") ;
    return ;
	}

	char title[1024] ;
	// Pour empêcher la TMDIChild de redéfinir le titre de la MainWindow
	pContexte->GetMainWindow()->GetWindowText(title, 1024) ;

	TModule* pGrabMod = pContexte->getSuperviseur()->getGrabModule() ;

	if (pGrabMod)
		delete pGrabMod ;

	// Initialisation du module de la dll de capture
	pGrabMod = new TModule("ns_grab.dll", TRUE);

	if (NULL == pGrabMod)
	{
  	erreur("Impossible d'ouvrir la DLL ns_grab.dll.", standardError, -1, pContexte->GetMainWindow()->GetHandle()) ;
    pContexte->getSuperviseur()->afficheStatusMessage("") ;
    pContexte->getSuperviseur()->setGrabModule(NULL) ;
    return ;
	}
	pContexte->getSuperviseur()->setGrabModule(pGrabMod) ;

	// Initialisation du contexte Matrox
	bool (FAR *pAdrGrabInit) (NSMatrox far **, NSContexte far *) ;
	// Récupération du pointeur sur la fonction GrabInit
	(FARPROC) pAdrGrabInit = pContexte->getSuperviseur()->getGrabModule()->GetProcAddress(MAKEINTRESOURCE(M_GRABINIT)) ;

	if (NULL == pAdrGrabInit)
	{
		erreur("Impossible de récupérer la fonction GrabInit()", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
		delete pGrabMod ;
    pContexte->getSuperviseur()->setGrabModule(NULL) ;
    return ;
	}

	NSMatrox* pMatroxPtr = NULL ;
	bool bOk = ((*pAdrGrabInit)((NSMatrox far **) &pMatroxPtr, (NSContexte far *) pContexte)) ;
	if (false == bOk)
	{
		erreur("Erreur à l'initialisation du contexte Matrox", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
		delete pGrabMod ;
		pContexte->getSuperviseur()->setGrabModule(NULL) ;
    pContexte->getSuperviseur()->initMatrox(NULL) ;
    return ;
	}
	pContexte->getSuperviseur()->initMatrox(pMatroxPtr) ;

	// on crée maintenant la fenetre de capture, qui récupère le contexte
	NSMilGrabWindow* pGrabWindow = new NSMilGrabWindow(pContexte->GetMainWindow(), pContexte, pAssist) ;
	NSMilGrabChild*  pGrabChild  = new NSMilGrabChild(pContexte, *(pContexte->getSuperviseur()->getApplication()->prendClient()), "Capture", pGrabWindow) ;
	pContexte->GetMainWindow()->SetCaption(title);

	// nsMenuIniter menuIter(pContexte) ;
	// OWL::TMenuDescr menuBar ;
	// menuIter.initMenuDescr(&menuBar, "menubar") ;
	// TMyApp* pMyApp = pContexte->getSuperviseur()->getApplication() ;
	// pMyApp->GetMainWindow()->SetMenuDescr(menuBar) ;
	// pMyApp->setMenu(string("menubar")) ;
	pGrabChild->Create() ;
}
catch (...)
{
	erreur("Exception NSPatientChoisi::CmCapture.", standardError, 0) ;
}
#endif // #ifndef __EPIPUMP__
}

/**
* Importation
*/
void NSPatientChoisi::CmImporter()
{
try
{
#ifndef __EPIPUMP__
	if (getReadOnly())
	{
		string sErrorText = pContexte->getSuperviseur()->getText("privilegeManagement", "patientFileIsReadOnly") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
		return ;
	}

	// on installe ici le point d'entrée de la routine d'importation (temporaire)

	// on choisi d'abord le répertoire par défaut d'importation (IHTM)
	string sPath = pContexte->PathName("IHTM") ;

	TFileOpenDialog::TData initData(OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,
																	"Tous fichiers (*.*)|*.*|",
																	0,
																	(char*) sPath.c_str(),
																	"*") ;

	int iUserChoice = TFileOpenDialog(pContexte->GetMainWindow(), initData).Execute() ;

	pContexte->reallocateBaseDirectory() ;

	if (IDOK != iUserChoice)
		return ;

	string sFileName = string(initData.FileName) ;

	// Recherche préalable de l'extension pour
	// traiter le cas des documents Word
	string sExtension = "" ;

	// Récupération du path
	size_t pos1 = sFileName.find_last_of("\\") ;

	if (pos1 == NPOS)
	{
		/* on doit normalement récupérer au moins C:\
		// => on génère une erreur s'il n'y a pas de '\\'  */
		erreur("Aucun PathName ne peut etre associé à ce fichier.", standardError, 0) ;
		return ;
	}

	// sPathName = string(sFileName, 0, pos1+1);
	// Récupération de l'extension
	size_t pos2 = sFileName.find(".") ;

	if (pos2 == NPOS)
	{
		//
		// On vérifie d'abord si ce n'est pas un fichier texte de l'ancien
		// Nautilus : gastroda
		//
		string sNomSeul = string(sFileName, pos1+1, strlen(sFileName.c_str())-pos1-1) ;
		//sNomSeul = pseumaj(sNomSeul.c_str());
		if (sNomSeul == "GASTRODA")
		{
			// cas de l'ancien Nautilus (traité plus tard dans l'importation)
			// pour le moment, on se contente de mettre l'extension à vide
			sExtension = "" ;
		}
		else
		{
			erreur("Ce fichier n'a pas d'extension.", standardError, 0) ;
			return ;
		}
	}
	else
	{
		sExtension = string(sFileName, pos2+1, strlen(sFileName.c_str())-pos2-1) ;
		pseumaj(&sExtension) ;
	}

	if ((string("DOC") == sExtension) || (string("DOCX") == sExtension)) // cas des documents Word
	{
		::MessageBox(0, "Votre document est de type Word. Pour ce type de fichier, Nautilus va ouvrir votre document dans une session Word. Pour l'importer, vous devez alors revenir sur la session Nautilus (par la barre des tâches) et cliquer sur <Archiver> dans la fenêtre <Document Word> de Nautilus.", "Message Nautilus", MB_OK);

		// on ouvre ici le document Word en automation OLE
		// cf code identique à la fonction CmOpenTpml()

    NSTtxDocument* pNewDocTtx = new NSTtxDocument(0, pContexte) ;
    pNewDocTtx->SetFichierExterne(sFileName) ;
		NSDocViewManager dvManager(pContexte) ;
		dvManager.createView(pNewDocTtx, "Word Format") ;
	}
	else
		Importer(sFileName) ;

#endif // !__EPIPUMP__
}
catch (...)
{
  erreur("Exception NSPatientChoisi::CmImporter.", standardError, 0) ;
}
}

void
NSPatientChoisi::CmGenericReferentiel(string sRefId, string sDocument, string sConcern)
{
try
{
#ifndef __EPIPUMP__
  ValIter ival, ivalp ;

  if (getReadOnly())
	{
		string sErrorText = pContexte->getSuperviseur()->getText("privilegeManagement", "patientFileIsReadOnly") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
		return ;
	}

  NSSuper* pSuper = pContexte->getSuperviseur() ;

  string ps = string("Entering CmGenericReferentiel, ID=") + sRefId ;
  pSuper->trace(&ps, 1) ;

  string sRefFile = pSuper->getArcManager()->DonneFichierArchetypeParNom(NSArcManager::referentiel, sRefId) ;
  if (string("") == sRefFile)
  {
    string sErrorText = string("Cannot find file name for referential ") + sRefId + string(". Leaving.");
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
    return ;
  }

  ps = string("Parsing file ") + sRefFile ;
  pSuper->trace(&ps, 1) ;

  nsrefParseur Parseur(pContexte->getSuperviseur()) ;
  if (false == Parseur.open(sRefFile))
    return ;

  if ((NULL == Parseur.getReferentiel()) || (NULL == Parseur.getReferentiel()->getValArray()))
  {
  	string sErrorText = pContexte->getSuperviseur()->getText("referentialManagement", "invalidReferential") + string(" ") + sRefId ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
    erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
    return ;
  }

  Valeur_array* pRefArray = Parseur.getReferentiel()->getValArray() ;
  if ((NULL == pRefArray) || pRefArray->empty())
  {
    string sErrorText = pContexte->getSuperviseur()->getText("referentialManagement", "emptyReferential") + string(" ") + sRefId ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
    erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
    return ;
  }

  bool	bSimple = true ;
  nsrefDialog* pDial = new nsrefDialog(pContexte->GetMainWindow(), pContexte, &Parseur, bSimple) ;
  int idRet = pDial->Execute() ;
  delete pDial ;

  ps = string("Réferentiel, lancement dialogue") ;
  pSuper->trace(&ps, 1);

  while ((IDOK != idRet) && (IDCANCEL != idRet))
  {
/*
    if (bSimple == true)
      bSimple = false ;
    else
      bSimple = true ;
*/

    if      (IDC_OTHER == idRet)
      bSimple = false ;
    else if (IDC_BBK_EVENT == idRet)
    {
      if (Parseur.getReferentiel())
      {
        Cglobalvars *pGVars = Parseur.getReferentiel()->getGVars() ;
        if (pGVars)
          pGVars->resetProcessState() ;
      }
    }

    pDial = new nsrefDialog(pContexte->GetMainWindow(), pContexte, &Parseur, bSimple) ;
    idRet = pDial->Execute() ;
    delete pDial ;
  }
  if (IDOK != idRet)
    return ;

	bool bDrugsAdded = false ;
	bool bGoalsAdded = false ;

	// Browsing all the proposals
  // on parcourt l'ensemble des propositions
  //
  for (ival = pRefArray->begin(); ival != pRefArray->end(); ival++)
  {    if (LABEL_PROPOSITION == (*ival)->getLabel())    {      // si la proposition a été retenue (check == true)      Cproposition* pCprop = dynamic_cast<Cproposition*>((*ival)->getObject()) ;
      if (pCprop && pCprop->getCheck())
      {
        //
        string sPropName = pCprop->getStringAttribute(ATTRIBUT_CONTR_NOM) ;
        if (string("") != sPropName)
        {
          ps = string("Réferentiel, traitement proposition ") + sPropName ;
          pSuper->trace(&ps, 1) ;
        }
        else
        {
          ps = string("Réferentiel, proposition sans nom") ;
          pSuper->trace(&ps, 1) ;
        }

        ps = string("Réferentiel, proposition, création d'objectifs") ;
        pSuper->trace(&ps, 1) ;

        // Browsing every proposal's trees
        //
        for (ivalp = pCprop->getValArray()->begin() ; ivalp != pCprop->getValArray()->end() ; ivalp++)
        {
          if (LABEL_TREE == (*ivalp)->getLabel())
          {
            // Creating the tree - On crée l'arbre
            //
            Ctree* pCtree = dynamic_cast<Ctree *>((*ivalp)->getObject()) ;
            if (pCtree)
            {
              string sDestinationPath = pCtree->getLocalisation() ;

              if      (string("ZPOMR/N0000") == sDestinationPath)
            	  bDrugsAdded = true ;
              else if (string("ZPOMR/0OBJE") == sDestinationPath)
            	  bGoalsAdded = true ;

              ps = string("Réferentiel, proposition, création d'un objet") ;
              pSuper->trace(&ps, 1) ;
              if (!CreeElementFromRef(sDestinationPath, pCtree->pPatPathoArray, sDocument, sRefId, pCprop->getLocalPropIDFromPropID(), pCprop->pReasonTree, sConcern))
            	  // cas d'erreur : on interrompt le processus
                return ;
            }
          }
        }
      }
    }
  }

	if (_pDocLdv)
	{
		if (true == bDrugsAdded)
    {
      NSFrameInformationArray *pFrames = _pDocLdv->getFrames() ;
      if (pFrames)
      {
        for (int i = 0 ; i < FRAMECOUNT ; i++)
        {
          NSFrameInformation *pFrameInfo = pFrames->getFrameInformation(getFrameForIndex(i)) ;
          if (pFrameInfo)
          {
            ArrayLdvDrugs *pDrugs = pFrameInfo->getTreatmentsArray() ;
            if (pDrugs)
      	      pDrugs->reinit() ;
          }
        }
      }
    }
    if (true == bGoalsAdded)
    	_pDocLdv->reinitAllGoals() ;

  	_pDocLdv->invalidateViews(string("MISCELLANOUS")) ;
	}

  ps = string("Leaving CmGenericReferentiel") ;
  pSuper->trace(&ps, 1) ;

#endif // !__EPIPUMP__
}
catch (...)
{
  erreur("Exception NSPatientChoisi::CmGenericReferentiel.", standardError, 0) ;
}
}

#define SAVE_GOALS_AS_BATCH

//
// Open a referential as a new window
//
// sRefId    : referential identifier
// sDocument : document id of the document from which this function is called
// iFrame    : frame to store elements into
//
void
NSPatientChoisi::CmReferentiel(string sRefId, string sDocument, LDVFRAME iFrame)
{
try
{
#ifndef __EPIPUMP__
  ValIter ival, ivalp ;

  if (getReadOnly())
	{
		string sErrorText = pContexte->getSuperviseur()->getText("privilegeManagement", "patientFileIsReadOnly") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
		return ;
	}

  NSSuper* pSuper = pContexte->getSuperviseur() ;

  string ps = string("Début réferentiel, ID=") + sRefId ;
  pSuper->trace(&ps, 1) ;

  // Get referential's file
  //
  string sRefFile = pSuper->getArcManager()->DonneFichierArchetypeParNom(NSArcManager::referentiel, sRefId) ;
  if (string("") == sRefFile)
    return ;

  ps = string("Réferentiel, parsing du fichier ") + sRefFile ;
  pSuper->trace(&ps, 1) ;

  // Open a parser and parse referential's file
  //
  nsrefParseur Parseur(pContexte->getSuperviseur()) ;
  if (false == Parseur.open(sRefFile))
    return ;

  if ((NULL == Parseur.getReferentiel()) || (NULL == Parseur.getReferentiel()->getValArray()))
  {
    string sErrorText = pContexte->getSuperviseur()->getText("referentialErrors", "incorrectReferential") ;
    sErrorText += string(" (") + sRefId + string(")") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
    return ;
  }

  Valeur_array* pRefArray = Parseur.getReferentiel()->getValArray() ;
  if (pRefArray->empty())
  {
    string sErrorText = pContexte->getSuperviseur()->getText("referentialErrors", "emptyReferential") ;
    sErrorText += string(" (") + sRefId + string(")") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
    return ;
  }

  // Open dialog
  //
  bool	bSimple = true ;
  nsrefDialog* pDial = new nsrefDialog(pContexte->GetMainWindow(), pContexte, &Parseur, bSimple) ;
  int idRet = pDial->Execute() ;
  delete pDial ;

  ps = string("Réferentiel, lancement dialogue") ;
  pSuper->trace(&ps, 1);

  // While dialog return is different from Ok or Cancel, the show must go on
  //
  while ((IDOK != idRet) && (IDCANCEL != idRet))
  {
/*
    if (bSimple == true)
      bSimple = false ;
    else
      bSimple = true ;
*/

    if      (IDC_OTHER == idRet)
      bSimple = false ;
    else if (IDC_BBK_EVENT == idRet)
    {
      if (Parseur.getReferentiel())
      {
        Cglobalvars *pGVars = Parseur.getReferentiel()->getGVars() ;
        if (pGVars)
          pGVars->resetProcessState() ;
      }
    }

    pDial = new nsrefDialog(pContexte->GetMainWindow(), pContexte, &Parseur, bSimple) ;
    idRet = pDial->Execute() ;
    delete pDial ;
  }
  if (IDOK != idRet)
    return ;

  string sLogString = string("") ;

#ifndef __EPIBRAIN__
	// Publication des contraintes du référentiel
  // We publish referential's constraints
	//
  Cglobalvars* pGlobVars = Parseur.getReferentiel()->getGVars() ;
	if (pGlobVars && pContexte->getPredi() && pContexte->getPredi()->mustLogPage() && pGlobVars->existPublishingString())
	{
		pContexte->getPredi()->addToLogPage(string(""), string("<br>\r\n")) ;
		pContexte->getPredi()->addToLogPage(string("Facteurs de risque retenus :"), string("<b>"), string("</b><br>\r\n")) ;

		sLogString = pGlobVars->getPublishingString(pContexte) ;

    //if (sLogString != "")
    //	pSuper->getDPIO()->addToLogPage(sLogString) ;
	}
#endif // !__EPIBRAIN__

  ArrayGoals PreviousGoals(_pDocLdv) ;
  ValArray   OldTrees ;
  ValArray   ClosedTrees ;

  // Récupération de la liste des objectifs issus d'un document de même type
  // Getting the list of goals created from a document of the same kind
  if (false == _pDocHis->getVectDocument()->empty())
  {
    // Récupération du précédent document du même type
    // Getting the previous doc of the same kind
    DocumentIter itCurrentDoc = _pDocHis->TrouveDocHisto(sDocument) ;
    if (NULL != itCurrentDoc)
    {
      NSPatPathoArray CurrentDocPpt(pContexte->getSuperviseur()) ;
      (*itCurrentDoc)->initFromPatPatho(&CurrentDocPpt) ;

      PatPathoIter iterRootPat = CurrentDocPpt.begin() ;
      string sTypeDoc = (*iterRootPat)->getLexique() ;
      
      NSPatPathoArray PreviousPatPatho(pContexte->getSuperviseur()) ;
      DocumentIter itPreviousDoc = _pDocHis->DonnePrevPatPathoDocument(sTypeDoc, &PreviousPatPatho, itCurrentDoc) ;

      // Il existe un document plus ancien : on stocke dans pPreviousGoals
      // des pointeurs sur ses objectifs
      if ((NULL != itPreviousDoc) && (_pDocHis->getVectDocument()->end() != itPreviousDoc))
      {
        string sCodePrevDoc = (*itPreviousDoc)->getID() ;
        NSLinkManager* pGraphe = _GraphPerson.getLinkManager() ;
        VecteurString GoalsID ;
        pGraphe->TousLesVrais(sCodePrevDoc, NSRootLink::refCreator, &GoalsID) ;

        if (false == GoalsID.empty())
        {
          NSFrameInformationArray *pFrames = _pDocLdv->getFrames() ;
          if (pFrames)
          {
            for (int i = 0 ; i < FRAMECOUNT ; i++)
            {
              NSFrameInformation* pFrameInfo = pFrames->getFrameInformation(getFrameForIndex(i)) ;
              if (pFrameInfo)
              {
                ArrayGoals* pLDVgoals = pFrameInfo->getGoalsArray() ;
                if ((pLDVgoals) && (false == pLDVgoals->empty()))
                {
                  EquiItemIter iGoals = GoalsID.begin() ;
                  for ( ; GoalsID.end() != iGoals ; iGoals++)
                  {
                    NSLdvGoal* pGoal = pLDVgoals->getGoal(**iGoals) ;
                    if (pGoal)
                      PreviousGoals.push_back(pGoal) ;
                  }
                }
              }
            }
          }
        }
      }
    }
  }

#ifndef __EPIBRAIN__
  if (pContexte->getPredi() && pContexte->getPredi()->mustLogPage())
  {
		pContexte->getPredi()->addToLogPage(string(""), string("<br>\r\n")) ;
		pContexte->getPredi()->addToLogPage(string("Objectifs de suivi :"), string("<b>"), string("</b><br>\r\n")) ;
  }
#endif // !__EPIBRAIN__

  // Fermeture des objectifs qui n'ont pas été à nouveau choisis et prolongation
  // des objectifs prorogés
  // Il est important d'effectuer cette étape avant l'insertion des nouveaux objectifs
  // --
  // Closing all the goals that were not selected again, and continuating already
  // existing ones
  // This step must be achieved before we insert new goals
  //
  if (false == PreviousGoals.empty())
  {
		// On marque tous les anciens objectifs comme "à détruire" ; seuls ceux
    // qui sont retrouvés dans la nouvelle liste seront sortis de cette liste
    //
		// Each and every new goal is flaged "to delete"; only those who are
    // found in the new selection are moved away from this list
    ArrayGoals ToDelete(PreviousGoals) ;

    // On ne garde au sein de pPreviousGoals que ceux qui n'ont pas à nouveau été choisis
    // on parcourt l'ensemble des propositions
    for (ival = pRefArray->begin() ; pRefArray->end() != ival ; ival++)
    {
      if (LABEL_PROPOSITION == (*ival)->getLabel())
      {        // si la proposition a été retenue (check == true)        Cproposition* pCprop = dynamic_cast<Cproposition*>((*ival)->getObject()) ;
        if (pCprop && pCprop->getCheck())
        {
          for (ivalp = pCprop->getValArray()->begin() ; pCprop->getValArray()->end() != ivalp ; ivalp++)
          {
            if (LABEL_TREE == (*ivalp)->getLabel())
            {
              // on crée l'objectif de santé pour chaque arbre
              Ctree* pCtree = dynamic_cast<Ctree*>((*ivalp)->getObject()) ;
              if (pCtree)
              {
                NSPatPathoArray PptObjectif(pContexte->getSuperviseur()) ;
                if (LocalPatho(pCtree->pPatPathoArray, &PptObjectif))
                {
                  NSLdvGoal Goal(pContexte, _pDocLdv, iFrame) ;
                  PatPathoIter itPpt = PptObjectif.begin() ;
                  Goal.initFromPatho(&PptObjectif, &itPpt) ;

                  for (ArrayGoalIter itGoals = ToDelete.begin(); ToDelete.end() != itGoals ; )
                  {
                    if (Goal.estIdentique(*itGoals))
                    {
										  NSLinkManager* pGraphe = _GraphPerson.getLinkManager() ;
                      pGraphe->etablirLien(sDocument, NSRootLink::refCreator, (*itGoals)->getNode()) ;

                      OldTrees.push_back(*ivalp) ;
                      ToDelete.erase(itGoals) ;
                    }
                    else
                      itGoals++ ;
                  }
                }
              }
            }
          }
        }

        // Proposition refusée avec motif : ouverture et fermeture instantanée
        else if (pCprop->bInitialyChecked && pCprop->pReasonTree &&
                                       (false == pCprop->pReasonTree->empty()))
        {
          // on parcours l'ensemble des arbres d'objectif de la proposition
          for (ivalp = pCprop->getValArray()->begin(); pCprop->getValArray()->end() != ivalp ; ivalp++)
          {
            if (LABEL_TREE == (*ivalp)->getLabel())
            {
              // on crée l'objectif de santé pour chaque arbre
              Ctree* pCtree = dynamic_cast<Ctree*>((*ivalp)->getObject()) ;
              if (pCtree)
              {
                NSPatPathoArray PptObjectif(pContexte->getSuperviseur()) ;
                if (LocalPatho(pCtree->pPatPathoArray, &PptObjectif))
                {
                  NSLdvGoal Goal(pContexte, _pDocLdv, iFrame) ;
                  PatPathoIter itPpt = PptObjectif.begin() ;
                  Goal.initFromPatho(&PptObjectif, &itPpt) ;

                  for (ArrayGoalIter itGoals = ToDelete.begin() ; ToDelete.end() != itGoals ; )
                  {
                    if (Goal.estIdentique(*itGoals))
                    {
                      _pDocLdv->closeGoal(*itGoals, pCprop->pReasonTree) ;
										  NSLinkManager* pGraphe = _GraphPerson.getLinkManager() ;

                      pGraphe->etablirLien(sDocument, NSRootLink::goalCloser, (*itGoals)->getNode()) ;

                      ClosedTrees.push_back(*ivalp) ;
                      ToDelete.erase(itGoals) ;
                    }
                    else
                      itGoals++ ;
                  }
                }
              }
            }
          }
        }
      }
    }
    if (false == ToDelete.empty())
    {
      for (ArrayGoalIter itGoals = ToDelete.begin(); ToDelete.end() != itGoals  ; )
      {
        _pDocLdv->closeGoal(*itGoals, NULL) ;
        ToDelete.erase(itGoals) ;
      }
    }

    for (ArrayGoalIter itGoals = PreviousGoals.begin() ; PreviousGoals.end() != itGoals ; )
      PreviousGoals.erase(itGoals) ;
  }

  // on parcourt l'ensemble des propositions
  for (ival = pRefArray->begin() ; pRefArray->end() != ival ; ival++)
  {    if (LABEL_PROPOSITION == (*ival)->getLabel())    {      // si la proposition a été retenue (check == true)      Cproposition* pCprop = dynamic_cast<Cproposition*>((*ival)->getObject()) ;
      if (pCprop && pCprop->getCheck())
      {
        //
        string sPropName = pCprop->getStringAttribute(ATTRIBUT_CONTR_NOM) ;
        if (sPropName != "")
        {
          ps = string("Referential, processing proposal ") + sPropName ;
          pSuper->trace(&ps, 1) ;
#ifndef __EPIBRAIN__
          pContexte->getPredi()->addToLogPage(sPropName, string("<li>"), string("</li>\r\n")) ;
#endif // !__EPIBRAIN__
        }
        else
        {
          ps = string("Referential, processing a nameless proposal") ;
          pSuper->trace(&ps, 1) ;
        }

        ps = string("Referential, processing proposal: creating goals") ;
        pSuper->trace(&ps, 1) ;
        // on parcours l'ensemble des arbres d'objectif de la proposition
        for (ivalp = pCprop->getValArray()->begin() ; pCprop->getValArray()->end() != ivalp ; ivalp++)
        {
          if (LABEL_TREE == (*ivalp)->getLabel())
          {
            // Cet objectif existait-il déjà ?
            // Was this goal already there ?
            //
            bool bAlreadyThere = false ;
            if (false == OldTrees.empty())
            {
              ValIter iOltTrees = OldTrees.begin() ;
              for ( ; (OldTrees.end() != iOltTrees) && ((*iOltTrees)->getObject() != (*ivalp)->getObject()) ; iOltTrees++)
                ;
              if (OldTrees.end() != iOltTrees)
                bAlreadyThere = true ;
            }
            if (false == bAlreadyThere)
            {
              // on crée l'objectif de santé pour chaque arbre
              Ctree* pCtree = dynamic_cast<Ctree *>((*ivalp)->getObject()) ;
              if (pCtree)
              {
                ps = string("Referential, processing proposal: creating a goal") ;
                pSuper->trace(&ps, 1) ;
#ifdef SAVE_GOALS_AS_BATCH
                if (false == CreeObjectif(pCtree->pPatPathoArray, sDocument, sRefId, pCprop->getLocalPropIDFromPropID(), pCprop->pReasonTree, iFrame, false))
#else
                if (false == CreeObjectif(pCtree->pPatPathoArray, sDocument, sRefId, pCprop->getLocalPropIDFromPropID(), pCprop->pReasonTree, iFrame, true))
#endif
                  // cas d'erreur : on interrompt le processus
                  return ;
              }
            }
          }
        }
      }
      // Proposition refusée avec motif : ouverture et fermeture instantanée
      else if (pCprop->bInitialyChecked && pCprop->pReasonTree && (false == pCprop->pReasonTree->empty()))
      {
        string sPropName = pCprop->getStringAttribute(ATTRIBUT_CONTR_NOM) ;
#ifndef __EPIBRAIN__
        if (sPropName != "")
        	pContexte->getPredi()->addToLogPage(string("Refus de ") + sPropName, string("<li>"), string("</li>\r\n")) ;
#endif // !__EPIBRAIN__

        // on parcours l'ensemble des arbres d'objectif de la proposition
        for (ivalp = pCprop->getValArray()->begin() ; pCprop->getValArray()->end() != ivalp ; ivalp++)
        {
          if (LABEL_TREE == (*ivalp)->getLabel())
          {
            // Cet objectif a-t-il déjà été fermé ?
            // Has this goal already been closed ?
            //
            bool bAlreadyClosed = false ;
            if (false == ClosedTrees.empty())
            {
              ValIter iOltTrees = ClosedTrees.begin() ;
              for ( ; (ClosedTrees.end() != iOltTrees) && ((*iOltTrees)->getObject() != (*ivalp)->getObject()) ; iOltTrees++)
                ;
              if (ClosedTrees.end() != iOltTrees)
                bAlreadyClosed = true ;
            }
            if (false == bAlreadyClosed)
            {
              // on crée l'objectif de santé pour chaque arbre
              Ctree* pCtree = dynamic_cast<Ctree *>((*ivalp)->getObject()) ;
              if (pCtree)
              {
                if (false == RefuseObjectif(pCtree->pPatPathoArray, sDocument, sRefId, pCprop->pReasonTree, iFrame))
                  // cas d'erreur : on interrompt le processus
                  return ;
              }
            }
          }
        }
      }
    }
  }

#ifdef SAVE_GOALS_AS_BATCH
  SaveGoalsBatch(iFrame) ;
#endif

  ps = string("Réferentiel, inscription du log DPIO") ;
  pSuper->trace(&ps, 1) ;

#endif // !__EPIPUMP__
}
catch (...)
{
  erreur("Exception NSPatientChoisi::CmReferentiel.", standardError, 0) ;
}
}

void
NSPatientChoisi::Importer(string sFileName, bool bAutomaticMode)
{
try
{
	char title[1024] ;

	// Pour empêcher la TMDIChild de redéfinir le titre de la MainWindow
	pContexte->GetMainWindow()->GetWindowText(title, 1024) ;

	// on crée la fenetre d'importation
	NSImportWindow* pImportWindow = new NSImportWindow(pContexte->GetMainWindow(), sFileName, pContexte, bAutomaticMode) ;

  if (false == pImportWindow->isNavOk())
  {
    // Here is an exception for the fu.... Word files that we treat like if
    // it has been selected by "open / file"
    //
    string sMajExtension = pseumaj(pImportWindow->getExtension()) ;
    delete pImportWindow ;

    if ((string("DOC") == sMajExtension) || (string("DOCX") == sMajExtension))
    {
      NSTtxDocument* pNewDocTtx = new NSTtxDocument(0, pContexte) ;
      pNewDocTtx->SetFichierExterne(sFileName) ;
		  NSDocViewManager dvManager(pContexte) ;
		  dvManager.createView(pNewDocTtx, "Word Format") ;
      return ;
    }

    if (string("") == pImportWindow->getNautilusType())
    {
      string sErrorText = pContexte->getSuperviseur()->getText("fileErrors", "unreferencedExtension") ;
      pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  	  erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
      return ;
	  }

    return ;
  }

	NSImportChild* pImportChild = new NSImportChild(pContexte, *(pContexte->getSuperviseur()->getApplication()->prendClient()), "Importation", pImportWindow) ;
	pContexte->GetMainWindow()->SetCaption(title) ;
	pImportChild->Create() ;
}
catch (TWindow::TXWindow& e)
{
  string sErr = string("Exception NSPatientChoisi::Importer: ") + e.why() ;
	erreur(sErr.c_str(), standardError, 0) ;
}
catch (...)
{
	erreur("Exception NSPatientChoisi::Importer.", standardError, 0) ;
}
}

void
NSPatientChoisi::CmImportImg(AssistCaptureDialog* pAssist)
{
try
{
	if (getReadOnly())
	{
		string sErrorText = pContexte->getSuperviseur()->getText("privilegeManagement", "patientFileIsReadOnly") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
		return ;
	}

	// on crée la fenetre d'importation des images
  NSImpImgWindow* pImportWindow = new NSImpImgWindow(pContexte->GetMainWindow(), pContexte, pAssist) ;

  if (false == pImportWindow->bNavOk)
	{
    delete pImportWindow ;
    return ;
  }

	NSImpImgChild* pImportChild = new NSImpImgChild(pContexte, *(pContexte->getSuperviseur()->getApplication()->prendClient()), "Importation des images", pImportWindow) ;

	pImportChild->Create() ;
}
catch (...)
{
	erreur("Exception NSPatientChoisi::CmImportImg.", standardError, 0) ;
}
}

void
NSPatientChoisi::CmGenerePannel()
{
try
{
	string sNomFichier, sLocFichier ;
	string sNomSource, sNomDest, sNomTemplate, sNomEnTeteBrut ;

	// char   nomModele[TMPL_FICHIER_LEN + 1];
	// char   nomEnTete[TMPL_EN_TETE_LEN + 1];

  string sNomModele = string("") ;
  string sNomEntete = string("") ;

	bool   bExisteImages = false ;
	// NSBaseImages*  pBaseCompo;

	// Dialogue de choix du template
	// qui sera le fichier source du nouveau HIHTM
	ChoixTemplateDialog* pChoixTemplateDlg =
		new ChoixTemplateDialog(pContexte->GetMainWindow(), pContexte, "ZIHTM", "") ;

	if (pChoixTemplateDlg->Execute() == IDCANCEL)
	{
		delete pChoixTemplateDlg ;
		return ;
	}

	int numModele = pChoixTemplateDlg->TemplateChoisi ;
	if (numModele > 0)
	{
		// strcpy(nomModele, (*(pChoixTemplateDlg->pTemplateArray))[numModele-1]->pDonnees->fichier) ;
		// strcpy(nomEnTete, (*(pChoixTemplateDlg->pTemplateArray))[numModele-1]->pDonnees->en_tete) ;

    sNomModele = (*(pChoixTemplateDlg->pTemplateArray))[numModele-1]->getFichier() ;
  	sNomEntete = (*(pChoixTemplateDlg->pTemplateArray))[numModele-1]->getEnTete() ;
	}
	else
	{
    string sErrorText = pContexte->getSuperviseur()->getText("imagesPannelErrors", "aTemplateMustBeSelected") ;
		erreur(sErrorText.c_str(), warningError, 0, pContexte->GetMainWindow()->GetHandle()) ;
    delete pChoixTemplateDlg ;
		return ;
	}

	delete pChoixTemplateDlg ;

	// on prend comme convention que le fichier template sera copié dans le fichier html dest
	// => le nom template ne figure pas dans le HDHTM (il y figure par contre l'en-tete)
  // car un fichier HIHTM est lui-même sa propre template
	sNomTemplate = pContexte->PathName("NTPL") + sNomModele ;
  sNomEnTeteBrut = sNomEntete ;

  NSRefDocument NewDoc(0, pContexte) ;

	// On trouve un nouveau nom pour le fichier à référencer
	if (!NewDoc.TrouveNomFichier("ZIHTM", "htm", sNomFichier, sLocFichier))
	{
    string sErrorText = pContexte->getSuperviseur()->getText("imagesPannelErrors", "errorWhenSettingAFileNameForThisPannel") ;
		erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
		return;
	}

	// on le copie depuis le repertoire source sous son nouveau nom
	sNomSource = sNomTemplate ;
	sNomDest = pContexte->PathName(sLocFichier) + sNomFichier ;

	if (!CopyFile(sNomSource.c_str(), sNomDest.c_str(), false))
	{
    string sErrorText = pContexte->getSuperviseur()->getText("fileErrors", "errorCopyingFile") ;
    sErrorText += string(" ") + sNomSource + string(" -> ") + sNomDest ;
		erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
		return;
	}

  string sLangue  = pContexte->getUserLanguage() ;
  string sCodeLex = string("ZIHTM1") ;
  string sLibelle ;
  pContexte->getDico()->donneLibelle(sLangue, &sCodeLex, &sLibelle) ;

	if (false == NewDoc.Referencer("ZIHTM", sLibelle, sNomFichier, sLocFichier, true,
                                true, "", NSRootLink::personDocument, 0, "_User_",
                                "", sNomEnTeteBrut))
	{
    string sErrorText = pContexte->getSuperviseur()->getText("documentManagementErrors", "failedToSave") ;
		erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
		return ;
	}

	// test préalable : la chemise contient-elle des images ?
	if (!NewDoc.ChemiseAvecImages(bExisteImages))
	{
    string sErrorText = pContexte->getSuperviseur()->getText("imagesPannelErrors", "itIsNotPossibleToDetermineIfThisPannel'sFolderContainsImages") ;
		erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
		return ;
	}

	if (false == bExisteImages)
	{
		string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str() ;
    string sErrorText = pContexte->getSuperviseur()->getText("imagesPannelErrors", "thisPannel'sFolderDoesNotContainAnyImages") ;
    sErrorText += string(" ") + pContexte->getSuperviseur()->getText("imagesPannelErrors", "doYouWantToKeepOn") ;
		int idRet = MessageBox(0, sErrorText.c_str(), sCaption.c_str(), MB_YESNO) ;
		if (IDNO == idRet)
			return ;
	}

	// Le pannel étant maintenant un nouveau document HIHTM
	// on charge automatiquement sa base de composition pour
	// pouvoir lui donner comme composants les images de la
	// chemise en cours.
	NSModHtml html(toComposer, &NewDoc, pContexte) ;

	if (!html.chargeBaseImages(pContexte->PathName("SHTM"), "", true))
	{
    string sErrorText = pContexte->getSuperviseur()->getText("imagesManagementErrors", "cannotCreateImagesList") ;
		erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
		return ;
	}

	if (string("") != html.sBaseCompo)
		NewDoc._sBaseCompo = html.sBaseCompo ;

  string sDroits ;

  // écriture des composants image
  //
  NSPatPathoArray PptPres(pContexte->getSuperviseur()) ;
  NewDoc._pDocInfo->initFromMeta(&PptPres) ;

  NewDoc.EcrireComposants(&PptPres) ;

  // enregistrement des composants et mise à jour de pDocInfo
  //
  _GraphPerson.setTree(&PptPres, "", NewDoc._pDocInfo->getCodeDocPres()) ;
  _GraphPerson.commitGraphTree(pContexte, NewDoc._pDocInfo->getCodeDocPres()) ;
  _GraphPerson.getTree(NewDoc._pDocInfo->getCodeDocPres(), &PptPres, &sDroits) ;

  NewDoc._pDocInfo->setPres(&PptPres) ;

	//
	// Ouverture effective du document : appel de pSuper
	//

	// NSDocumentInfo Docum(pContexte) ;
	// *(Docum.getData()) = *(NewDoc.pDocInfo->getData()) ;

  NSDocumentInfo Docum(*(NewDoc._pDocInfo)) ;

	NSNoyauDocument* pDocNoy = pContexte->getSuperviseur()->OuvreDocument(Docum, pContexte, false) ;
	if (pDocNoy)
	{
		// on reprend les donnees pour éviter l'effet de bord de OuvreDocument
		// (switch entre document HD et document brut CN)
    //
    NSDocumentData InfoData ;
    NewDoc._pDocInfo->initFromData(&InfoData) ;
    Docum.setData(&InfoData) ;

		// on range le pHtmlInfo dans le cas d'un HD
		// (permet de savoir que le document est composé)
		_pDocHis->RangeDocumentOuvert(&Docum, pDocNoy) ;
	}
}
catch (...)
{
	erreur("Exception NSPatientChoisi::CmGenerePannel.", standardError, 0) ;
}
}

void NSPatientChoisi::LdV_init()
{
try
{
  // Initializes frames information structures
  //
  loadFramesDocuments() ;

#ifndef __EPIPUMP__
	_pDocLdv = new NSLdvDocument(0, pContexte) ;
#endif // !__EPIPUMP__
}
catch (...)
{
	erreur("Exception NSPatientChoisi::LdV_init.", standardError, 0) ;
}
}

void
NSPatientChoisi::loadFramesDocuments()
{
try
{
  string sTraceText = string("NSPatientChoisi loading frames documents.") ;
  pContexte->getSuperviseur()->trace(&sTraceText, 1, NSSuper::trDetails) ;

  if (NULL == _pFramesDocuments)
    _pFramesDocuments = new NSFrameDocumentsArray(_pDocHis->getDocumentTank()) ;

  // First step: initialize frame indexes structures
  //
  for (int i = 0 ; i < FRAMECOUNT ; i++)
  {
    LDVFRAME iFrame = getFrameForIndex(i) ;
    _pFramesDocuments->openFrameInformation(iFrame) ;
  }

  // Second step: check if all indexes were found
  //
  int iNbIndex = 0 ;

  int i = 0 ;
  for ( ; i < FRAMECOUNT ; i++)
  {
    LDVFRAME iFrame = getFrameForIndex(i) ;

    NSFrameDocuments* pFrameDocuments = _pFramesDocuments->getFrameDocuments(iFrame) ;
    if (pFrameDocuments)
    {
      NSDocumentHisto* pDoc = pFrameDocuments->getIndexDocument() ;
      if (pDoc)
      {
        NSPatPathoArray DocPpt(pContexte->getSuperviseur()) ;
        pDoc->initFromPatPatho(&DocPpt) ;

        if (false == DocPpt.empty())
        {
          iNbIndex++ ;
          bool bNeedFixing = false ;

          // "Concerns" branch
          PatPathoIter iter = DocPpt.ChercherItem("0PRO11") ;
          if ((NULL == iter) || (DocPpt.end() == iter))
          {
            bNeedFixing = true ;
            DocPpt.ajoutePatho("0PRO11", 1) ;
          }
          // "Goals" branch
          iter = DocPpt.ChercherItem("0OBJE1") ;
          if ((NULL == iter) || (DocPpt.end() == iter))
          {
            bNeedFixing = true ;
            DocPpt.ajoutePatho("0OBJE1", 1) ;
          }
          // "Concerns" branch
          iter = DocPpt.ChercherItem("N00001") ;
          if ((NULL == iter) || (DocPpt.end() == iter))
          {
            bNeedFixing = true ;
            DocPpt.ajoutePatho("N00001", 1) ;
          }
          if (bNeedFixing)
          {
            pDoc->setPatPatho(&DocPpt) ;
            updateIndexDocument(pDoc, false) ;
          }
        }
      }
    }
  }

  // If Ok, lets quit
  //
  if (FRAMECOUNT == iNbIndex)
  {
    sTraceText = string("NSPatientChoisi frames documents loaded.") ;
    pContexte->getSuperviseur()->trace(&sTraceText, 1, NSSuper::trDetails) ;

    return ;
  }

/*
  string sQuestionText = pContexte->getSuperviseur()->getText("LigneDeVieErrors", "doYouWantToCreateMissingIndexes?") ;
	int iAnswer = MessageBox(0, sQuestionText.c_str(), 0, MB_YESNO) ;

  if (ID_NO == iAnswer)
    return ;
*/

  // Create missing indexes
  //
  NSPersonGraphManager* pGraphPerson = pContexte->getPatient()->getGraphPerson() ;

  for (int i = 0 ; i < FRAMECOUNT ; i++)
  {
    LDVFRAME iFrame = getFrameForIndex(i) ;

    if (NULL == getFrameIndex(iFrame, false))
    {
      NSRefDocument	PbIndex(0, 0, 0, pContexte, false) ;
      PbIndex.createFrame(pGraphPerson, iFrame) ;

      string sMetaID = PbIndex.getCodeDocMeta() ;
      pGraphPerson->commitGraphTree(pContexte, sMetaID, pidsPatient) ;

      string sTreeID = PbIndex.DonneDocInfo()->getID() ;
      pGraphPerson->commitGraphTree(pContexte, sTreeID, pidsPatient) ;

      PbIndex.updateAfterSave(sTreeID, sMetaID) ;

      NSPatPathoArray PptIndex(pContexte->getSuperviseur()) ;
      PbIndex.initFromPatPatho(&PptIndex) ;
      pContexte->getPatient()->getDocHis()->Rafraichir(PbIndex._pDocInfo, &PptIndex, &PbIndex) ;
    }
  }

  // Iterate
  //
  loadFramesDocuments() ;
}
catch (...)
{
	erreur("Exception NSPatientChoisi::loadFramesDocuments.", standardError, 0) ;
}
}

bool
NSPatientChoisi::updateIndexDocument(NSDocumentHisto* pDocument, bool bUpdateDisplay)
{
  if ((NSDocumentHisto*) NULL == pDocument)
    return false ;

  if ((NSHISTODocument*) NULL == _pDocHis)
	{
    pContexte->getSuperviseur()->traceAndDisplay("documentManagementErrors", "cannotFindHistoryManager") ;
		return false ;
	}

	NSNoyauDocument Noyau((TDocument*) 0, pDocument, (NSDocumentInfo*) 0, pContexte, false, true) ;

  NSPatPathoArray DocPpt(pContexte->getSuperviseur()) ;
  pDocument->initFromPatPatho(&DocPpt) ;
	Noyau.setPatPatho(&DocPpt) ;

	// Saving updated document
  //
	Noyau.enregistrePatPatho() ;

	bool bReload = Noyau.chargePatPatho() ;
	if (false == bReload)
		return false ;

  NSPatPathoArray PptNoyau(pContexte->getSuperviseur()) ;
  Noyau.initFromPatPatho(&PptNoyau) ;

  _pDocHis->Rafraichir(pDocument, &PptNoyau) ;

  return true ;
}

void
NSPatientChoisi::LdV_show(string sConcern)
{
try
{
#ifndef __EPIPUMP__

	if (NULL == _pDocLdv)
  {
    string sErrorText = string("NSPatientChoisi::LdV_show, no document, leaving.") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
    erreur(sErrorText.c_str(), standardError, 0) ;
		return ;
  }

  string sTraceText = string("NSPatientChoisi::LdV_show : checking if a view already exists.") ;
  pContexte->getSuperviseur()->trace(&sTraceText, 1, NSSuper::trDetails) ;

	TView* pView = _pDocLdv->GetViewList() ;
  if (pView)
  {
		do
		{
			NSLdvView* pLdVView = TYPESAFE_DOWNCAST(pView, NSLdvView) ;
			if (pLdVView /*&& (pLdVView->getConcern() == sConcern)*/)
			{
        string sTraceText = string("NSPatientChoisi::LdV_show : view already exists, setting focus to it.") ;
        pContexte->getSuperviseur()->trace(&sTraceText, 1, NSSuper::trDetails) ;

        pLdVView->SetFocus() ;
    		// ::SetFocus(pLdVView->GetHandle()) ;
        // TWindow* pWnd = (TWindow*) pLdVView->GetParent() ;
      	// if (pWnd)
        //   ::SetFocus(pWnd->GetHandle()) ;
				return ;
			}

			pView = _pDocLdv->NextView(pView) ;
		}
		while (pView) ;
  }

  sTraceText = string("NSPatientChoisi::LdV_show : no existing view, creating a new one.") ;
  pContexte->getSuperviseur()->trace(&sTraceText, 1, NSSuper::trDetails) ;

	NSDocViewManager dvManager(pContexte) ;
	dvManager.createView(_pDocLdv) ;

  sTraceText = string("NSPatientChoisi::LdV_show : new view creation ordered.") ;
  pContexte->getSuperviseur()->trace(&sTraceText, 1, NSSuper::trDetails) ;

#endif
}
catch (...)
{
  string sErrorText = string("Exception NSPatientChoisi::LdV_show.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0) ;
}
}

void
NSPatientChoisi::drugs_show(string sConcern)
{
try
{
#ifndef __EPIPUMP__
	if ((NSLdvDocument*) NULL == _pDocLdv)
		return ;

	TView* pView = _pDocLdv->GetViewList() ;
  if (pView)
  {
		do
		{
			NSDrugView* pDrugView = TYPESAFE_DOWNCAST(pView, NSDrugView) ;
			if (pDrugView && (pDrugView->getConcern() == sConcern))
			{
      	pDrugView->activate() ;
				return ;
			}

			pView = _pDocLdv->NextView(pView) ;
		}
		while (pView) ;
  }

	if (!(pContexte->userHasPrivilege(NSContexte::viewDrug, -1, -1, string(""), string(""), NULL, NULL)))
	{
		string sErrorText = pContexte->getSuperviseur()->getText("privilegeManagement", "insufficientRights") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), warningError, 0) ;
		return ;
	}

	NSDocViewManager dvManager(pContexte) ;
	if (string("") == sConcern)
		dvManager.createView(_pDocLdv, "Drug Management") ;
	else
	{
		NSDrugView* pDrugView = new NSDrugView(*_pDocLdv, sConcern) ;
		dvManager.createView(_pDocLdv, "Drug Management", pDrugView) ;
	}

#endif // !__EPIPUMP__
}
catch (...)
{
	erreur("Exception NSPatientChoisi::drugs_show.", standardError, 0) ;
}
}

void
NSPatientChoisi::goals_show(string sConcern)
{
try
{
#ifndef __EPIPUMP__
	if ((NSLdvDocument*) NULL == _pDocLdv)
		return ;
	//
	// On vérifie que la fenêtre n'est pas déjà ouverte
	// Checking if the same window is not already open
	//
	TView* pView = _pDocLdv->GetViewList() ;
  if (pView)
  {
		do
		{
			NSGoalView* pGoalView = TYPESAFE_DOWNCAST(pView, NSGoalView) ;
			if (pGoalView && (pGoalView->getConcern() == sConcern))
			{
				pGoalView->activate() ;
				return ;
			}

			pView = _pDocLdv->NextView(pView) ;
		}
		while (pView) ;
  }

	if (!(pContexte->userHasPrivilege(NSContexte::viewGoal, -1, -1, string(""), string(""), NULL, NULL)))
	{
		string sErrorText = pContexte->getSuperviseur()->getText("privilegeManagement", "insufficientRights") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), warningError, 0) ;
		return ;
	}

	NSDocViewManager dvManager(pContexte) ;
	if (sConcern == "")
		dvManager.createView(_pDocLdv, "Goal Management") ;
	else
	{
		NSGoalView* pGoalView = new NSGoalView(*_pDocLdv, sConcern) ;
		dvManager.createView(_pDocLdv, "Goal Management", pGoalView) ;
	}

#endif // !__EPIPUMP__
}
catch (...)
{
    erreur("Exception NSPatientChoisi::goals_show.", standardError, 0) ;
}
}

void
NSPatientChoisi::process_show(string sConcern)
{
try
{
#ifndef __EPIPUMP__
	if ((NSLdvDocument*) NULL == _pDocLdv)
		return ;
	// On vérifie que la fenêtre n'est pas déjà ouverte
	// Checking if the same window is not already open
	TView *pView = _pDocLdv->GetViewList() ;
  if (pView)
  {
		do
		{
			NSProcessView* pProcessView = TYPESAFE_DOWNCAST(pView, NSProcessView) ;
			if (pProcessView && (pProcessView->getConcern() == sConcern))
			{
				pProcessView->activate() ;
				return ;
			}

			pView = _pDocLdv->NextView(pView) ;
		}
		while (pView) ;
  }

	if (!(pContexte->userHasPrivilege(NSContexte::viewProcess, -1, -1, string(""), string(""), NULL, NULL)))
	{
		string sErrorText = pContexte->getSuperviseur()->getText("privilegeManagement", "insufficientRights") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), warningError, 0) ;
		return ;
	}

	NSDocViewManager dvManager(pContexte) ;
	if (sConcern == "")
		dvManager.createView(_pDocLdv, "Process Management") ;
	else
	{
		NSProcessView* pProcessView = new NSProcessView(*_pDocLdv, sConcern) ;
		dvManager.createView(_pDocLdv, "Process Management", pProcessView) ;
	}

#endif // !__EPIPUMP__
}
catch (...)
{
	erreur("Exception NSPatientChoisi::process_show.", standardError, 0) ;
}
}

void
NSPatientChoisi::followUp_show(string sConcern)
{
try
{
#ifndef __EPIPUMP__
	if ((NSLdvDocument*) NULL == _pDocLdv)
		return ;

	// On vérifie que la fenêtre n'est pas déjà ouverte
	// Checking if the same window is not already open
  //
	TView *pView = _pDocLdv->GetViewList() ;
  if (pView)
  {
		do
		{
			NSFollowUpView* pFUView = TYPESAFE_DOWNCAST(pView, NSFollowUpView) ;
			if (pFUView && (pFUView->getConcern() == sConcern))
			{
				pFUView->activate() ;
				return ;
			}

			pView = _pDocLdv->NextView(pView) ;
		}
		while (pView) ;
  }

	if (!(pContexte->userHasPrivilege(NSContexte::viewProcess, -1, -1, string(""), string(""), NULL, NULL)))
	{
		string sErrorText = pContexte->getSuperviseur()->getText("privilegeManagement", "insufficientRights") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), warningError, 0) ;
		return ;
	}

	NSDocViewManager dvManager(pContexte) ;
	if (string("") == sConcern)
		dvManager.createView(_pDocLdv, string("FollowUp Management")) ;
	else
	{
		NSFollowUpView* pFUView = new NSFollowUpView(*_pDocLdv, sConcern) ;
		dvManager.createView(_pDocLdv, string("FollowUp Management"), pFUView) ;
	}

#endif // !__EPIPUMP__
}
catch (...)
{
	erreur("Exception NSPatientChoisi::followUp_show.", standardError, 0) ;
}
}

void
NSPatientChoisi::curves_show(string sConcern)
{
try
{
#ifndef __EPIPUMP__
	if ((NSLdvDocument*) NULL == _pDocLdv)
		return ;

  if (string("") == sConcern)
    return ;

	// On vérifie que la fenêtre n'est pas déjà ouverte
	// Checking if the same window is not already open
	TView *pView = _pDocLdv->GetViewList() ;
  if (pView)
  {
		do
		{
			NSLdvView* pLDVView = TYPESAFE_DOWNCAST(pView, NSLdvView) ;
			if (pLDVView)
			{
				pLDVView->SetFocus() ;
        pLDVView->displayCurvesForBiometricalGoals(sConcern) ;
				return ;
			}

			pView = _pDocLdv->NextView(pView) ;
		}
		while (pView) ;
  }

#endif // !__EPIPUMP__
}
catch (...)
{
	erreur("Exception NSPatientChoisi::curves_show.", standardError, 0) ;
}
}

void
NSPatientChoisi::heathTeamRosaceShow()
{
try
{
#ifndef __EPIPUMP__
#ifndef __EPIBRAIN__

	if ((NSHealthTeamDocument*) NULL == _pHealthDoc)
  	_pHealthDoc = new NSHealthTeamDocument(0, pContexte, _pHealthTeam) ;

	TView* pView = _pHealthDoc->GetViewList() ;
  if (pView)
  {
		do
		{
			NSHealthTeamRosaceView* pRosaceView = TYPESAFE_DOWNCAST(pView, NSHealthTeamRosaceView) ;
			if (pRosaceView)
			{
        pRosaceView->activate() ;
				return ;
			}

			pView = _pHealthDoc->NextView(pView) ;
		}
		while (pView) ;
  }

	NSDocViewManager dvManager(pContexte) ;
	dvManager.createView(_pHealthDoc, "HealthTeam rosace") ;

#endif // !__EPIPUMP__
#endif // !__EPIBRAIN__
}
catch (...)
{
	erreur("Exception NSPatientChoisi::heathTeamRosaceShow.", standardError, 0) ;
}
}

void
NSPatientChoisi::heathTeamListShow()
{
try
{
#ifndef __EPIPUMP__
#ifndef __EPIBRAIN__

	if ((NSHealthTeamDocument*) NULL == _pHealthDoc)
  	_pHealthDoc = new NSHealthTeamDocument(0, pContexte, _pHealthTeam) ;

	TView* pView = _pHealthDoc->GetViewList() ;
  if (pView)
  {
		do
		{
			NSHealthTeamListView* pListView = TYPESAFE_DOWNCAST(pView, NSHealthTeamListView) ;
			if (pListView)
			{
        pListView->activate() ;
				return ;
			}

			pView = _pHealthDoc->NextView(pView) ;
		}
		while (pView) ;
  }

	NSDocViewManager dvManager(pContexte) ;
	dvManager.createView(_pHealthDoc, "HealthTeam list") ;

#endif
#endif
}
catch (...)
{
	erreur("Exception NSPatientChoisi::heathTeamRosaceShow.", standardError, 0) ;
}
}

void
NSPatientChoisi::venuesListShow()
{
try
{
#ifndef __EPIPUMP__
#ifndef __EPIBRAIN__

	if ((NSVenuesDocument*) NULL == _pVenuesDoc)
  	_pVenuesDoc = new NSVenuesDocument(0, pContexte) ;

	TView* pView = _pVenuesDoc->GetViewList() ;
  if (pView)
  {
		do
		{
			NSVenuesView* pListView = TYPESAFE_DOWNCAST(pView, NSVenuesView) ;
			if (pListView)
			{
        pListView->activate() ;
				return ;
			}

			pView = _pVenuesDoc->NextView(pView) ;
		}
		while (pView) ;
  }

	NSDocViewManager dvManager(pContexte) ;
	dvManager.createView(_pVenuesDoc, "Venues list") ;

#endif
#endif
}
catch (...)
{
	erreur("Exception NSPatientChoisi::venuesListShow.", standardError, 0) ;
}
}

void
NSPatientChoisi::admin_show()
{
  if (_bOpeningAdm)
    return ;

try
{
  _bOpeningAdm = true ;

	NSLinkManager* pGraphe = _GraphPerson.getLinkManager() ;
  string sCodeDocRoot = _GraphPerson.getRootTree() ;
	string sCodeDocAdmin, sCodeDocAdminData ;
  //
  // Meta document
  //
	VecteurString VecteurString ;
	pGraphe->TousLesVrais(sCodeDocRoot, NSRootLink::personAdminData, &VecteurString) ;

	if (VecteurString.empty())
	{
  	string sError = pContexte->getSuperviseur()->getText("patientOpening", "unableToFindAdminDocument") ;
    pContexte->getSuperviseur()->trace(&sError, 1, NSSuper::trError) ;
    erreur(sError.c_str(), standardError, 0) ;
    _bOpeningAdm = false ;
		return ;
	}
  //
  // Data document
  //
	sCodeDocAdmin = *(*(VecteurString.begin())) ;
	VecteurString.vider() ;
	pGraphe->TousLesVrais(sCodeDocAdmin, NSRootLink::docData, &VecteurString) ;
	if (VecteurString.empty())
	{
		erreur("Impossible de retrouver les données de la fiche administrative.", standardError, 0) ;
    _bOpeningAdm = false ;
		return ;
	}
	sCodeDocAdminData = *(*(VecteurString.begin())) ;

	string sCodePat = string(sCodeDocAdminData, 0, PAT_NSS_LEN) ;
	string sCodeDoc = string(sCodeDocAdminData, PAT_NSS_LEN, DOC_CODE_DOCUM_LEN) ;

	NSDocumentInfo NSDocumentInfo(pContexte) ;
	NSDocumentInfo.setPatient(sCodePat) ;
	NSDocumentInfo.setDocument(sCodeDoc) ;
	NSDocumentInfo.setType(string("CQ030")) ;

  // Already open ?
  //
	TView* pView = _pDocHis->ActiveFenetre(&NSDocumentInfo, "NSCQWindowsView");
	if (pView)
  {
    NSCQWindowsView* pCQView = TYPESAFE_DOWNCAST(pView, NSCQWindowsView) ;
    if (pCQView)
    {
      NSPaneSplitter* pPaneSplit = pCQView->getPaneSplitter() ;
      if (pPaneSplit)
        SetFocus(pPaneSplit->Parent->Parent->GetHandle()) ;
    }
    _bOpeningAdm = false ;
		return ;
  }

	NSPatPathoArray PatPathoAdmin(pContexte->getSuperviseur()) ;

	// _pDocHis->DonnePatPathoDocument("ZADMI1", &PatPathoAdmin) ; Doing this would scan documents
  _pDocHis->initFromPatPathoDocument(sCodeDocAdminData, &PatPathoAdmin) ;

	ChargeDonneesPatient(&PatPathoAdmin) ;
	initCorrespArray(&PatPathoAdmin) ;
	PostInitialisation() ;

	// On doit lancer postinitialisation AVANT d'ouvrir la fiche administrative
	// (fabrication du nom long, calcul de l'age du patient, etc.)
	_pDocHis->AutoriserEdition(&NSDocumentInfo) ;

  _bOpeningAdm = false ;
}
catch (...)
{
	erreur("Exception NSPatientChoisi::admin_show.", standardError, 0) ;
}
}

void
NSPatientChoisi::histo_show()
{
}

void NSPatientChoisi::CmFeuilleSecu()
{
	// on ne doit pas deleter pDocSecu car cela est fait
	// par la VisualView en fin de visualisation
	_pDocSecu = new NSSecuRefDocument(pContexte);

	if (_pDocSecu->bInitOk)
		_pDocSecu->Visualiser();
	else
		erreur("Impossible d'éditer la feuille de soins.", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
}

void NSPatientChoisi::CmNewCr()
{
try
{
#ifndef __EPIPUMP__
#ifndef __EPIBRAIN__
  /***********************
	if (getReadOnly())
	{
		erreur("Le dossier de ce patient est en lecture seule.", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
		return ;
	}
  ***********************/

	// choix du compte rendu
	string sChoixCR   = string("") ;
	string sImportDll = string("") ;

	ChoixCRDialog* pChoixCRDl = new ChoixCRDialog(pContexte->GetMainWindow(), &sChoixCR, &sImportDll, pContexte) ;
	if (pChoixCRDl->Execute() == IDCANCEL)
		return ;

	// Importation automatique
	if (string("") != sImportDll)
	{
		// Chargement de la dll d'importation / Loading the import dll
		TModule* pDCModule = new TModule(sImportDll.c_str(), TRUE) ;

		if ((TModule*) NULL == pDCModule)
		{
			string sErrMess = "Impossible d'ouvrir la DLL d'importation : " + sImportDll ;
      pContexte->getSuperviseur()->trace(&sErrMess, 1, NSSuper::trError) ;
			erreur(sErrMess.c_str(), standardError, -1, pContexte->GetMainWindow()->GetHandle()) ;
		}
		else
		{
			bool (FAR *pAdresseFct) (NSContexte far *) ;
			// Récupération du pointeur sur la fonction // Getting function's pointer
			(FARPROC) pAdresseFct = pDCModule->GetProcAddress(MAKEINTRESOURCE(1)) ;
			if (pAdresseFct)
			{
				bool bReussi = ((*pAdresseFct)((NSContexte far *) pContexte)) ;
				if (false == bReussi)
					erreur("Echec à l'importation", standardError, -1, pContexte->GetMainWindow()->GetHandle()) ;
			}
			delete pDCModule ;
		}
	}

	// Lancement du module
	NSCRDocument* pDocNewCr = new NSCRDocument((TDocument*) 0, pContexte, sChoixCR) ;
  NSDocViewManager dvManager(pContexte) ;
	dvManager.createView(pDocNewCr, "CN Format") ;

#endif // !__EPIPUMP__
#endif // !__EPIBRAIN__
}
catch (...)
{
	erreur("Exception NSPatientChoisi::CmNewCr.", standardError, 0) ;
}
}

void NSPatientChoisi::CmNewCoopChart()
{
try
{
#ifndef __EPIPUMP__
#ifndef __EPIBRAIN__

	// Lancement du module
	NSCoopChartDocument* pDocNewCoopChart = new NSCoopChartDocument(0, pContexte) ;
  NSDocViewManager dvManager(pContexte) ;
	dvManager.createView(pDocNewCoopChart, "Coop Chart") ;

#endif // !__EPIPUMP__
#endif // !__EPIBRAIN__
}
catch (...)
{
	erreur("Exception NSPatientChoisi::CmNewCr.", standardError, 0) ;
}
}

void NSPatientChoisi::CmNewArchetype()
{
try
{
	if (getReadOnly())
	{
		string sErrorText = pContexte->getSuperviseur()->getText("privilegeManagement", "patientFileIsReadOnly") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
		return ;
	}

	//
	// choix du compte rendu
	//
	string sChoixCR ;

	ChoixArchetypeDialog* pChoixCRDl = new ChoixArchetypeDialog(pContexte->GetMainWindow(),
                                                                &sChoixCR,
                                                                pContexte);
	if (pChoixCRDl->Execute() == IDCANCEL)
		return ;
  if (string("") == sChoixCR)
    return ;

	//
	// Lancement de l'achetype
	//
  NSToDoTask *pTask = new NSToDoTask() ;
  pTask->BbkAskUser(pContexte->getSuperviseur(), sChoixCR, NSCQDocument::standard) ;
	// pContexte->BbkAskUser(sChoixCR, NSCQDocument::standard) ;
}
catch (...)
{
	erreur("Exception NSPatientChoisi::CmNewArchetype.", standardError, 0) ;
}
}

void NSPatientChoisi::CmDeviceInterface()
{
try
{
  NSInterfaceDocument *pInterfaceDoc = new NSInterfaceDocument(0, pContexte) ;

  if (false == pInterfaceDoc->IsOpen())
  {
    delete pInterfaceDoc ;
    return ;
  }

  NSDocViewManager dvManager(pContexte) ;
	dvManager.createView(pInterfaceDoc, "Interface") ;

/*
  string sModuleName = string("") ;

	NsInterfacesDialog* pChoiceDlg = new NsInterfacesDialog(pContexte->GetMainWindow(),
                                                          pContexte,
                                                          &sModuleName) ;
	if (pChoiceDlg->Execute() != IDOK)
		return ;
  if (string("") == sModuleName)
    return ;

	// Launching the interface
	//
	TModule* pDCModule = new TModule(sModuleName.c_str(), TRUE) ;

  if (NULL == pDCModule)
  {
  	string sErrorText = pContexte->getSuperviseur()->getText("modulesManagement", "cannotLoadDll") + string(" ") + sModuleName ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
    return ;
  }

  void (FAR *pAdresseFct) (TWindow far *, NSContexte far *) ;

  // Récupération du pointeur sur la fonction // Getting function's pointer
  // (FARPROC) pAdresseFct = pDCModule->GetProcAddress(MAKEINTRESOURCE(2)) ;
  (FARPROC) pAdresseFct = pDCModule->GetProcAddress("@nsmToolsAction$qp11OWL@TWindowp10NSContexte") ;
  if (NULL == pAdresseFct)
  {
    string sErrorText = pContexte->getSuperviseur()->getText("modulesManagement", "cannotAccessMethodInDll") + string(" ") + sModuleName + string(" -> nsmToolsAction");
    pContexte->getSuperviseur()->trace(&sErrorText, 1) ;
    erreur(sErrorText.c_str(), standardError, -1, pContexte->GetMainWindow()->GetHandle()) ;
  	delete pDCModule ;
    return ;
  }

  ((*pAdresseFct)((TWindow far *) pContexte->GetMainWindow(), (NSContexte far *) pContexte)) ;
*/
}
catch (...)
{
	erreur("Exception NSPatientChoisi::CmDeviceInterface.", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------
// Function    : NSPatientChoisi::CmNewMagic()
// Arguments   :
// Description : Crée un document de type Magic
// Returns     : Rien
// -----------------------------------------------------------------------------
void NSPatientChoisi::CmNewConsult(bool bIsDocCS)
{
try
{
#ifndef __EPIPUMP__
#ifndef __EPIBRAIN__
	if (getReadOnly())
	{
		string sErrorText = pContexte->getSuperviseur()->getText("privilegeManagement", "patientFileIsReadOnly") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
		return ;
	}

  if (bIsDocCS && _pConsultEnCours)
  {
    _pConsultEnCours->activate() ;
    return ;
  }

	// choix du type document
	string sTypeDocument ;

  if (!bIsDocCS)
  {
    NSTypeDocument* pNSTypeDocument = new NSTypeDocument(pContexte->GetMainWindow(), pContexte, &sTypeDocument) ;
    if (pNSTypeDocument->Execute() == IDCANCEL)
    {
      delete pNSTypeDocument ;
      return ;
    }
    delete pNSTypeDocument ;
  }
  else
  	sTypeDocument = string("GCONS1") ;

  if ((sTypeDocument == "GCONS1") && _pConsultEnCours)
  {
    _pConsultEnCours->activate() ;
    return ;
  }

	NSCSDocument* pNewDocCS = new NSCSDocument(0, pContexte, sTypeDocument) ;

	if (sTypeDocument == "GCONS1")
		_pConsultEnCours = pNewDocCS ;

  NSDocViewManager dvManager(pContexte) ;
	dvManager.createView(pNewDocCS, "CS Format") ;

#endif // !__EPIPUMP__
#endif // !__EPIBRAIN__
}
catch (...)
{
	erreur("Exception NSPatientChoisi::CmNewConsult.", standardError, 0) ;
}
}

void
NSPatientChoisi::InitialisationSynchronous()
{
	string ps ;

#ifndef __EPIPUMP__
#ifndef __EPIBRAIN__
  // NSEnregPatientDlg	*pProgressDlg ;

  NSProgressAndMessageDlg progressDlg(pContexte->GetMainWindow(), pContexte) ;

try
{
	// pProgressDlg = new NSEnregPatientDlg(pContexte->GetMainWindow(), pContexte) ;
	progressDlg.Create() ;	progressDlg.SetCaption("Ouverture du patient") ;
  progressDlg.Show(SW_SHOW) ;
}
catch (...)
{
  string sErrorText = string("Exception NSPatientChoisi::Initialisation while opening progress dialog.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
}

try
{
  progressDlg.setProgressAndText(2, string("Blocage du patient")) ;

	bloquer() ;
}
catch (...)
{
  string sErrorText = string("Exception NSPatientChoisi::Initialisation while blocking patient.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
}

	NSSuper *pSuper = pContexte->getSuperviseur() ;

	// NSLinkManager	*pGraphe 	= pGraphPerson->pLinkManager ;

	// on vide le nom et le prénom de l'array de capture
  //
	if ((pContexte->getEpisodus()) && (false == pContexte->getEpisodus()->CaptureArray.empty()))
	{
		NSCaptureArray* pCapt = &(pContexte->getEpisodus()->CaptureArray) ;

		for (CaptureIter captIter = pCapt->begin() ; pCapt->end() != captIter ; )
		{
			if      ((*captIter)->getChemin() == "ZADMI1/LIDET1/LNOM01")
			{
				delete (*captIter) ;
				pCapt->erase(captIter) ;
			}
			else if ((*captIter)->getChemin() == "ZADMI1/LIDET1/LNOM21")
			{
				delete (*captIter) ;
				pCapt->erase(captIter) ;
			}
			else if ((*captIter)->getChemin() == "ZADMI1/LIDET1/LCODO1")
			{
				delete (*captIter) ;
				pCapt->erase(captIter) ;
			}
			else
				captIter++ ;
		}
	}

try
{
  // Initialisation du blackboard

  string ps = string("Initializing Blackboard") ;
  pSuper->trace(&ps, 1);

  progressDlg.setProgressAndText(5, string("Initialisation du Blackboard")) ;

	reinitBlackboard(pContexte) ;

  ps = string("Sending init event to Blackboard") ;
	pSuper->trace(&ps, 1) ;
}
catch (...)
{
  string sErrorText = string("Exception NSPatientChoisi::Initialisation while reiniting Blackboard.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
}

  addInitEvent(pContexte) ;

  //
  // Documents will be referenced in History inside NSHISTODocument ctor
  // We must create the contribution before, in case some documents need fixing
  //
  if (getContribution() == string(""))
  {
try
{
    ps = string("Creating a contribution for patient opening") ;
    pSuper->trace(&ps, 1);

    progressDlg.setProgressAndText(10, string("Création d'une contribution")) ;

    CreeContribution() ;

    ps = string("Contribution created") ;
    pSuper->trace(&ps, 1);
}
catch (...)
{
  string sErrorText = string("Exception NSPatientChoisi::Initialisation while creating contribution.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
}
  }

  ps = string("Initializing documents tank") ;
  pSuper->trace(&ps, 1);

  progressDlg.setProgressAndText(12, string("Création de l'historique")) ;

  _pDocHis = new NSHISTODocument(0, pContexte, this) ;

	// ---------------------------------------------------------------------------
	// Initialisation de l'Equipe de Santé du Patient.
	// Initialization of Patient's HealthTeam.
try
{
  ps = string("Loading health team") ;
  pSuper->trace(&ps, 1);

  progressDlg.setProgressAndText(15, string("Ouverture de l'équipe de santé")) ;

	NSPatPathoArray HTPatPatho(pContexte->getSuperviseur()) ;
	_pDocHis->DonnePatPathoDocument("LEQUI1", &HTPatPatho) ;
	_pHealthTeam	= new NSHealthTeam(&HTPatPatho) ;

  pContexte->getRelationship()->init(pContexte) ;
}
catch (...)
{
  string sErrorText = string("Exception NSPatientChoisi::Initialisation while loading health team.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
}

	// ---------------------------------------------------------------------------
	// création de la fiche historique
  //
  NSDocViewManager dvManager(pContexte) ;

try
{
  ps = pContexte->getSuperviseur()->getText("patientOpening", "openingHistory") + string("\n") ;
	pSuper->trace(&ps, 1) ;
	// pProgressDlg->CheckHisto() ;

  progressDlg.setProgressAndText(20, string("Affichage de l'historique")) ;

	dvManager.createView(_pDocHis, "HS Format") ;

	// fixer la position de la fiche historique selon les valeurs dans la base des registres
	// TView	*pTView = pDocHis->GetViewList() ;
	// NsHistorique *pNsHistorique = static_cast<NsHistorique *>(pTView) ;
}
catch (...)
{
  string sErrorText = string("Exception NSPatientChoisi::Initialisation while opening history.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
}

  NSDocumentInfo* pNSDocumentInfo = (NSDocumentInfo*) 0 ;

	// ouverture de la fiche de synthèse
  // opening synthesis document
try
{
	NSWindowProperty *pWinProp = pContexte->getUtilisateur()->getWindowProperty("Synthesis") ;
  if (pWinProp && (pWinProp->getActivity() == NSWindowProperty::openedWithPatient))
	{
  	ps = pContexte->getSuperviseur()->getText("patientOpening", "openingSynthesis") ;
		pSuper->trace(&ps, 1) ;
		// pProgressDlg->CheckSynth() ;
    progressDlg.setProgressAndText(25, string("Ouverture de la fiche de synthèse")) ;

		pNSDocumentInfo = (NSDocumentInfo*) 0 ;

		string sCodeDocRoot = _GraphPerson.getRootTree() ;

    VecteurString aVecteurString ;
    NSLinkManager* pGraphe = _GraphPerson.getLinkManager() ;
    pGraphe->TousLesVrais(sCodeDocRoot, NSRootLink::personSynthesis, &aVecteurString) ;

    if (false == aVecteurString.empty())
    {
      string sCodeDocSynth = *(*(aVecteurString.begin())) ;
      aVecteurString.vider() ;
      pGraphe->TousLesVrais(sCodeDocSynth, NSRootLink::docData, &aVecteurString) ;

      if (false == aVecteurString.empty())
      {
        string sCodeDocSynthData = *(*(aVecteurString.begin())) ;

        DocumentIter synthIter = _pDocHis->TrouveDocHisto(sCodeDocSynthData) ;
        if (synthIter)
        	pNSDocumentInfo = *synthIter ;
      }
      else
      {
      	string sError = pContexte->getSuperviseur()->getText("patientOpening", "unableToFindSynthesisData") ;
        pContexte->getSuperviseur()->trace(&sError, 1, NSSuper::trError) ;
        erreur(sError.c_str(), standardError, 0) ;
        // pProgressDlg->Destroy() ;
        // delete pProgressDlg ;
        // return ;
      }
    }
    else
    {
    	string sError = pContexte->getSuperviseur()->getText("patientOpening", "unableToFindSynthesisDocument") ;
      pContexte->getSuperviseur()->trace(&sError, 1, NSSuper::trError) ;
      erreur(sError.c_str(), standardError, 0) ;
      // pProgressDlg->Destroy() ;
      // delete pProgressDlg ;
      // return ;
    }

		// la synthèse est ouverte par l'historique

    if (pNSDocumentInfo)
    	_pDocHis->AutoriserEdition(pNSDocumentInfo) ;
	}
}
catch (...)
{
  string sErrorText = string("Exception NSPatientChoisi::Initialisation while opening synthesis.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
}

	// ---------------------------------------------------------------------------
  // Ouverture de la fiche administrative à partir de l'historique
  // Opening demographic information
  NSPatPathoArray PatPathoAdmin(pContexte->getSuperviseur()) ;

try
{
  // pProgressDlg->CheckAdmin() ;
  progressDlg.setProgressAndText(40, string("Chargement des données administratives")) ;
  pNSDocumentInfo = (NSDocumentInfo*) 0 ;

	_pDocHis->DonnePatPathoDocument("ZADMI1", &PatPathoAdmin) ;
	ChargeDonneesPatient(&PatPathoAdmin) ;
}
catch (...)
{
  string sErrorText = string("Exception NSPatientChoisi::Initialisation while opening demographic information.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
}

try
{
  string sStatusText = pContexte->getSuperviseur()->getText("HealthTeamManagement", "loadingCorrespondants") ;
  pContexte->getSuperviseur()->afficheStatusMessage(sStatusText) ;
  ps = string("Initializing windows position") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

  progressDlg.setProgressAndText(50, string("Initialisation des correspondants")) ;

	initCorrespArray(&PatPathoAdmin) ;

  ps = string("Initializing windows position") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;
  sStatusText = pContexte->getSuperviseur()->getText("HealthTeamManagement", "correspondantsLoaded") ;
  pContexte->getSuperviseur()->afficheStatusMessage(sStatusText) ;

  // pProgressDlg->CheckCorrs() ;
}
catch (...)
{
  string sErrorText = string("Exception NSPatientChoisi::Initialisation while opening correspondants.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
}

try
{
  progressDlg.setProgressAndText(60, string("Opérations post-initialisation")) ;

	PostInitialisation() ;
}
catch (...)
{
  string sErrorText = string("Exception NSPatientChoisi::Initialisation during post-initialization.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
}

	// On doit lancer postinitialisation AVANT d'ouvrir la fiche administrative
	// (fabrication du nom long, calcul de l'age du patient, etc.)
	if (pNSDocumentInfo)
  {
    progressDlg.setProgressAndText(65, string("Ouverture de la fiche administrative")) ;
		_pDocHis->AutoriserEdition(pNSDocumentInfo) ;
  }

	// ---------------------------------------------------------------------------
	// Initialisation de la ligne de vie
try
{
  ps = pContexte->getSuperviseur()->getText("patientOpening", "initializingLigneDeVie") + string("\n") ;
	pSuper->trace(&ps, 1) ;
  pContexte->getSuperviseur()->afficheStatusMessage(ps) ;

	// pProgressDlg->CheckLDV() ;
  progressDlg.setProgressAndText(80, string("Initialisation de la Ligne de vie")) ;

	LdV_init() ;

  ps = pContexte->getSuperviseur()->getText("patientOpening", "LigneDeVieInitialized") + string("\n") ;
	pSuper->trace(&ps, 1) ;
  pContexte->getSuperviseur()->afficheStatusMessage(ps) ;
}
catch (...)
{
  string sErrorText = string("Exception NSPatientChoisi::Initialisation while opening Ligne de vie.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
}

try
{
	if (false == pContexte->getUtilisateur()->getWindowPropertyArray()->empty())
	{
		ArrayWPIter iter = pContexte->getUtilisateur()->getWindowPropertyArray()->begin() ;
  	for ( ; pContexte->getUtilisateur()->getWindowPropertyArray()->end() != iter ; iter++)
    {
    	if (NSWindowProperty::openedWithPatient == (*iter)->getActivity())
      {
      	if      (string("HealthTeamList")   == (*iter)->getFunction())
      		heathTeamListShow() ;
        else if (string("HealthTeamRosace") == (*iter)->getFunction())
      		heathTeamRosaceShow() ;
        else if (string("DrugManagement")   == (*iter)->getFunction())
      		drugs_show() ;
        else if (string("GoalManagement")   == (*iter)->getFunction())
      		goals_show() ;
        else if (string("AdminData")        == (*iter)->getFunction())
      		admin_show() ;
        else if (string("GoalsFollowUp")    == (*iter)->getFunction())
      		followUp_show() ;
        else if (string("VenuesList")       == (*iter)->getFunction())
      		venuesListShow() ;
      }
    }
	}
}
catch (...)
{
  string sErrorText = string("Exception NSPatientChoisi::Initialisation while showing various windows.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
}

  // Mode Ouverture automatique de la Ligne de vie
	// if ((pSuper->getEpisodus()) && (pSuper->getEpisodus()->bAutoOpenLdV))
try
{
  ps = pContexte->getSuperviseur()->getText("patientOpening", "openingLigneDeVie") + string("\n") ;
	pSuper->trace(&ps, 1) ;
  pContexte->getSuperviseur()->afficheStatusMessage(ps) ;

  progressDlg.setProgressAndText(90, string("Ouverture de la Ligne de vie")) ;

  LdV_show() ;

  ps = pContexte->getSuperviseur()->getText("patientOpening", "LigneDeVieopened") + string("\n") ;
	pSuper->trace(&ps, 1) ;
  pContexte->getSuperviseur()->afficheStatusMessage(ps) ;
}
catch (...)
{
  string sErrorText = string("Exception NSPatientChoisi::Initialisation while showing Ligne de vie.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
}

	// Ouverture du gestionnaire de RC
#ifndef __EPIBRAIN__
try
{
	if (pContexte->getPredi() && (pContexte->getPredi()->_sNomModule.find("SCMG") != string::npos))
  	dvManager.createView(_pDocLdv, "RC view Format") ;
}
catch (...)
{
  string sErrorText = string("Exception NSPatientChoisi::Initialisation while opening RC window.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
}
#endif // !__EPIBRAIN__

  // blackboard initialization is now done much earlier
	// reinitBlackboard(pContexte) ;

	// pProgressDlg->Destroy() ;
	// delete pProgressDlg ;

	// bool  bDPIO = false ;
	// bool  bRVDB = false ;

	// Lancement de DPIO
#ifndef __EPIBRAIN__
try
{
	if ((pContexte->getPredi()) && (pContexte->getPredi()->_bPrediActif))
	{
    progressDlg.setProgressAndText(95, string("Ouverture de PreDi")) ;

		pContexte->getPredi()->PatOpening() ;
		//    DPIO(pContexte) ;
		// bDPIO = true ;
	}
}
catch (...)
{
  string sErrorText = string("Exception NSPatientChoisi::Initialisation during opening signal for Predi.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
}
#endif // !__EPIBRAIN__

	// lancement de RVDB
	/*
	if ((pSuper->getDPIO()) && (pSuper->getDPIO()->bRVDBactif))
	{
		//    RVDB(pContexte) ;
		bRVDB = true ;
	}
	*/

	if (pContexte->_pMailBoxWindow)
  	pContexte->_pMailBoxWindow->SetFocus() ;

try
{
  if (string("") != pSuper->getStaysSynchroDll())
  {
    string sStaysSynchroDll = pSuper->getStaysSynchroDll() ;

    string ps = string("Synchronizing venues using dll ") + sStaysSynchroDll ;
    pSuper->trace(&ps, 1) ;

    TModule* pDCModule = new TModule(sStaysSynchroDll.c_str(), TRUE) ;
    if (NULL == pDCModule)
    {
      string sErrorText = pContexte->getSuperviseur()->getText("modulesManagement", "cannotLoadDll") + string(" ") + sStaysSynchroDll ;
      pSuper->trace(&sErrorText, 1) ;
    }
    else
    {
      bool (FAR *pAdresseFct) (NSContexte far *) ;
      (FARPROC) pAdresseFct = pDCModule->GetProcAddress(MAKEINTRESOURCE(1)) ;
      if (NULL == pAdresseFct)
      {
        string sErrorText = pContexte->getSuperviseur()->getText("modulesManagement", "cannotAccessMethodInDll") + string(" ") + sStaysSynchroDll + string(" -> exporter") ;
        pSuper->trace(&sErrorText, 1) ;
        erreur(sErrorText.c_str(), standardError, -1, pContexte->GetMainWindow()->GetHandle()) ;
      }
      else
        /* bool bReussi = */ ((*pAdresseFct)((NSContexte far *) pContexte)) ;

      delete pDCModule ;
    }
  }
}
catch (...)
{
  string sErrorText = string("Exception NSPatientChoisi::Initialisation during synchronization process.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
}

  progressDlg.setProgressAndText(100, string("Patient ouvert")) ;

  ps = pContexte->getSuperviseur()->getText("patientOpening", "patientOpened") + string("\n") ;
	pSuper->trace(&ps, 1) ;
  pContexte->getSuperviseur()->afficheStatusMessage(ps) ;

#endif // !__EPIBRAIN__
#endif // !__EPIPUMP__
}

void
NSPatientChoisi::InitialisationAsynchronous(OPENPATNEXT iWhatToDoNext)
{
  _iWhatToDoAfterOpening = iWhatToDoNext ;
  _iCurrentOpeningStep   = ipsUndefined ;

  InitialisationContinue() ;
}

void
NSPatientChoisi::InitialisationNextStep()
{
  switch(_iCurrentOpeningStep)
  {
    case ipsUndefined :
      InitialisationStart() ;               // Open progress dialog
      break ;
    case ipsStart :
      InitialisationBlock() ;               // Block patient
      break ;
    case ipsBlockPat :
      InitialisationCreateContribution() ;  // Create contribution
      break ;
    case ipsContrib :
      InitialisationUpdateFromCapture() ;   // Update from captured data
      break ;
    case ipsUpdateFromCapture :
      InitialisationPurgeCapture() ;        // Erase captured data
      break ;
    case ipsPurgeCapt :
      InitialisationReinitBbk() ;           // Reinit BBK
      break ;
    case ipsReinitBbk :
      InitialisationAddInitEvent() ;        // Send BBK starting notifications
      break ;
    case ipsInitEvent :
      InitialisationCreateHistory() ;
      break ;
    case ipsHistory :
      InitialisationCreateHealthTeam() ;
      break ;
    case ipsHeathTeam :
      InitialisationOpenHistoryVue() ;
      break ;
    case ipsHistoryView :
      InitialisationOpenSynthesys() ;
      break ;
    case ipsSynthesys :
      InitialisationInitDemographics() ;
      break ;
    case ipsDemographics :
      InitialisationInitCorresps() ;
      break ;
    case ipsCorreps :
      InitialisationPostInitialisation() ;
      break ;
    case ipsPostInit :
      InitialisationCreateLdVDocument() ;
      break ;
    case ipsLDV :
      InitialisationOpenOtherVues() ;
      break ;
    case ipsOtherViews :
      InitialisationOpenLdVVue() ;
      break ;
    case ipsOpenLDV :
      InitialisationRCVue() ;
      break ;
    case ipsRC :
      InitialisationPredi() ;
      break ;
    case ipsPredi :
      InitialisationAdaptMailBox() ;
      break ;
    case ipsMailbox :
      InitialisationEnd() ;
      break ;
  }
}

void
NSPatientChoisi::InitialisationContinue()
{
  NSToDoTask *pToDoTask = new NSToDoTask ;
  pToDoTask->setWhatToDo(string("PatientInitNextStep")) ;
  pToDoTask->sendMe(pContexte->getSuperviseur()) ;
}

void
NSPatientChoisi::InitialisationStart()
{
  _iCurrentOpeningStep = ipsStart ;

  if (_progressDialog)
    delete _progressDialog ;

  _progressDialog = new NSProgressAndMessageDlg(pContexte->GetMainWindow(), pContexte) ;
  _progressDialog->Create() ;
  _progressDialog->SetCaption("Ouverture du patient") ;
  _progressDialog->ShowWindow(SW_SHOW) ;

  InitialisationContinue() ;
}

void
NSPatientChoisi::InitialisationBlock()
{
  _iCurrentOpeningStep = ipsBlockPat ;

  string sTrace = string("Blocking patient") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubSteps) ;

try
{
  _progressDialog->setProgressAndText(2, string("Blocage du patient")) ;

	bloquer() ;

  InitialisationContinue() ;
}
catch (...)
{
  string sErrorText = string("Exception NSPatientChoisi::InitialisationBlock while blocking patient.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;

  InitialisationContinue() ;
}
}

void
NSPatientChoisi::InitialisationPurgeCapture()
{
  _iCurrentOpeningStep = ipsPurgeCapt ;

  // on vide le nom et le prénom de l'array de capture
  //
	if ((pContexte->getEpisodus()) &&
      (false == pContexte->getEpisodus()->CaptureArray.empty()))
	{
		NSCaptureArray* pCapt = &(pContexte->getEpisodus()->CaptureArray) ;

		for (CaptureIter captIter = pCapt->begin() ; pCapt->end() != captIter ; )
		{
			if      ((*captIter)->getChemin() == "ZADMI1/LIDET1/LNOM01")
			{
				delete (*captIter) ;
				pCapt->erase(captIter) ;
			}
			else if ((*captIter)->getChemin() == "ZADMI1/LIDET1/LNOM21")
			{
				delete (*captIter) ;
				pCapt->erase(captIter) ;
			}
			else if ((*captIter)->getChemin() == "ZADMI1/LIDET1/LCODO1")
			{
				delete (*captIter) ;
				pCapt->erase(captIter) ;
			}
			else
				captIter++ ;
		}
	}

  InitialisationContinue() ;
}

void
NSPatientChoisi::InitialisationReinitBbk()
{
  _iCurrentOpeningStep = ipsReinitBbk ;

try
{
  // Initialisation du blackboard

  string ps = string("Initializing Blackboard") ;
  pContexte->getSuperviseur()->trace(&ps, 1);

  _progressDialog->setProgressAndText(5, string("Initialisation du Blackboard")) ;

	reinitBlackboard(pContexte) ;
}
catch (...)
{
  string sErrorText = string("Exception NSPatientChoisi::Initialisation while reiniting Blackboard.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
}

  InitialisationContinue() ;
}

void
NSPatientChoisi::InitialisationAddInitEvent()
{
  _iCurrentOpeningStep = ipsInitEvent ;

  string ps = string("Sending init event to Blackboard") ;
	pContexte->getSuperviseur()->trace(&ps, 1) ;

  addInitEvent(pContexte) ;

  InitialisationContinue() ;
}

void
NSPatientChoisi::InitialisationCreateContribution()
{
  _iCurrentOpeningStep = ipsContrib ;

  //
  // Documents will be referenced in History inside NSHISTODocument ctor
  // We must create the contribution before, in case some documents need fixing
  //
  if (getContribution() == string(""))
  {
try
{
    string ps = string("Creating a contribution for patient opening") ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

    _progressDialog->setProgressAndText(10, string("Création d'une contribution")) ;

    CreeContribution() ;

    ps = string("Contribution created") ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;
}
catch (...)
{
  string sErrorText = string("Exception NSPatientChoisi::Initialisation while creating contribution.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
}
  }

  InitialisationContinue() ;
}

void
NSPatientChoisi::InitialisationCreateHistory()
{
  _iCurrentOpeningStep = ipsHistory ;

  string ps = string("Initializing documents tank") ;
  pContexte->getSuperviseur()->trace(&ps, 1);

  _progressDialog->setProgressAndText(12, string("Création de l'historique (initialisation des documents)")) ;

  _pDocHis = new NSHISTODocument(0, pContexte, this) ;

  InitialisationContinue() ;
}

void
NSPatientChoisi::InitialisationCreateHealthTeam()
{
  _iCurrentOpeningStep = ipsHeathTeam ;

	// ---------------------------------------------------------------------------
	// Initialisation de l'Equipe de Santé du Patient.
	// Initialization of Patient's HealthTeam.
try
{
  string ps = string("Loading health team") ;
  pContexte->getSuperviseur()->trace(&ps, 1);

  _progressDialog->setProgressAndText(15, string("Ouverture de l'équipe de santé")) ;

	NSPatPathoArray HTPatPatho(pContexte->getSuperviseur()) ;
	_pDocHis->DonnePatPathoDocument("LEQUI1", &HTPatPatho) ;
	_pHealthTeam	= new NSHealthTeam(&HTPatPatho) ;

  pContexte->getRelationship()->init(pContexte) ;
}
catch (...)
{
  string sErrorText = string("Exception NSPatientChoisi::Initialisation while loading health team.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
}

  InitialisationContinue() ;
}

void
NSPatientChoisi::InitialisationOpenHistoryVue()
{
  _iCurrentOpeningStep = ipsHistoryView ;

  // ---------------------------------------------------------------------------
	// création de la fiche historique
  //
  NSDocViewManager dvManager(pContexte) ;

try
{
  string ps = pContexte->getSuperviseur()->getText("patientOpening", "openingHistory") + string("\n") ;
	pContexte->getSuperviseur()->trace(&ps, 1) ;
	// pProgressDlg->CheckHisto() ;

  _progressDialog->setProgressAndText(20, string("Affichage de l'historique")) ;

	dvManager.createView(_pDocHis, "HS Format") ;

	// fixer la position de la fiche historique selon les valeurs dans la base des registres
	// TView	*pTView = pDocHis->GetViewList() ;
	// NsHistorique *pNsHistorique = static_cast<NsHistorique *>(pTView) ;
}
catch (...)
{
  string sErrorText = string("Exception NSPatientChoisi::Initialisation while opening history.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
}

  InitialisationContinue() ;
}

void
NSPatientChoisi::InitialisationOpenSynthesys()
{
  _iCurrentOpeningStep = ipsSynthesys ;

	// ouverture de la fiche de synthèse
  // opening synthesis document
try
{
	NSWindowProperty *pWinProp = pContexte->getUtilisateur()->getWindowProperty("Synthesis") ;
  if (pWinProp && (pWinProp->getActivity() == NSWindowProperty::openedWithPatient))
	{
    NSSuper *pSuper = pContexte->getSuperviseur() ;

  	string ps = pSuper->getText("patientOpening", "openingSynthesis") ;
		pSuper->trace(&ps, 1) ;
		// pProgressDlg->CheckSynth() ;
    _progressDialog->setProgressAndText(25, string("Ouverture de la fiche de synthèse")) ;

		NSDocumentInfo* pNSDocumentInfo = (NSDocumentInfo*) 0 ;

		string sCodeDocRoot = _GraphPerson.getRootTree() ;

    VecteurString aVecteurString ;
    NSLinkManager* pGraphe = _GraphPerson.getLinkManager() ;
    pGraphe->TousLesVrais(sCodeDocRoot, NSRootLink::personSynthesis, &aVecteurString) ;

    if (false == aVecteurString.empty())
    {
      string sCodeDocSynth = *(*(aVecteurString.begin())) ;
      aVecteurString.vider() ;
      pGraphe->TousLesVrais(sCodeDocSynth, NSRootLink::docData, &aVecteurString) ;

      if (false == aVecteurString.empty())
      {
        string sCodeDocSynthData = *(*(aVecteurString.begin())) ;

        DocumentIter synthIter = _pDocHis->TrouveDocHisto(sCodeDocSynthData) ;
        if (synthIter)
        	pNSDocumentInfo = *synthIter ;
      }
      else
      {
      	string sError = pSuper->getText("patientOpening", "unableToFindSynthesisData") ;
        pSuper->trace(&sError, 1, NSSuper::trError) ;
        erreur(sError.c_str(), standardError, 0) ;
      }
    }
    else
    {
    	string sError = pContexte->getSuperviseur()->getText("patientOpening", "unableToFindSynthesisDocument") ;
      pContexte->getSuperviseur()->trace(&sError, 1, NSSuper::trError) ;
      erreur(sError.c_str(), standardError, 0) ;
    }

		// la synthèse est ouverte par l'historique

    if (pNSDocumentInfo)
    	_pDocHis->AutoriserEdition(pNSDocumentInfo) ;
	}
}
catch (...)
{
  string sErrorText = string("Exception NSPatientChoisi::Initialisation while opening synthesis.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
}

  InitialisationContinue() ;
}

//---------------------------------------------------------------------------
//  Update patient information from captured data
//---------------------------------------------------------------------------
void
NSPatientChoisi::InitialisationUpdateFromCapture()
{
  _iCurrentOpeningStep = ipsUpdateFromCapture ;

  // If we are not opening patients from the "in patients" list and we are not
  // opening the patient from captured information... then go to next step
  //
  string sInPat = pContexte->getSuperviseur()->getInPatientsFileDesc() ;

  if ((opnUpdateFromCapture != _iWhatToDoAfterOpening) && (string("") == sInPat))
  {
    InitialisationContinue() ;
    return ;
  }

  NSCaptureArray* pCapt = &(pContexte->getEpisodus()->CaptureArray) ;
  if (pCapt && (false == pCapt->empty()))
  {
    if (opnUpdateFromCapture == _iWhatToDoAfterOpening)
      updateDemographicsFromCapture(pCapt) ;
    updateVenuesFromCapture(pCapt) ;
  }

  InitialisationContinue() ;
}

void
NSPatientChoisi::InitialisationInitDemographics()
{
  _iCurrentOpeningStep = ipsDemographics ;

  string ps = string("Loading demographic informations") ;
  pContexte->getSuperviseur()->trace(&ps, 1);

  // ---------------------------------------------------------------------------
  // Ouverture de la fiche administrative à partir de l'historique
  // Opening demographic information
  NSPatPathoArray PatPathoAdmin(pContexte->getSuperviseur()) ;

try
{
  // pProgressDlg->CheckAdmin() ;
  _progressDialog->setProgressAndText(40, string("Chargement des données administratives")) ;

  if (_pDocHis)
  {
	  _pDocHis->DonnePatPathoDocument("ZADMI1", &PatPathoAdmin) ;
	  ChargeDonneesPatient(&PatPathoAdmin) ;
  }
}
catch (...)
{
  string sErrorText = string("Exception NSPatientChoisi::Initialisation while opening demographic information.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
}

try
{
  string sStatusText = pContexte->getSuperviseur()->getText("HealthTeamManagement", "loadingCorrespondants") ;
  pContexte->getSuperviseur()->afficheStatusMessage(sStatusText) ;
  string ps = string("Initializing windows position") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

  _progressDialog->setProgressAndText(50, string("Initialisation des correspondants")) ;

	initCorrespArray(&PatPathoAdmin) ;

  ps = string("Initializing windows position") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;
  sStatusText = pContexte->getSuperviseur()->getText("HealthTeamManagement", "correspondantsLoaded") ;
  pContexte->getSuperviseur()->afficheStatusMessage(sStatusText) ;

  // pProgressDlg->CheckCorrs() ;
}
catch (...)
{
  string sErrorText = string("Exception NSPatientChoisi::Initialisation while opening correspondants.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
}

  InitialisationContinue() ;
}

void
NSPatientChoisi::InitialisationInitCorresps()
{
  _iCurrentOpeningStep = ipsCorreps ;

  InitialisationContinue() ;
}

void
NSPatientChoisi::InitialisationPostInitialisation()
{
  _iCurrentOpeningStep = ipsPostInit ;

try
{
  _progressDialog->setProgressAndText(60, string("Opérations post-initialisation")) ;

	PostInitialisation() ;
}
catch (...)
{
  string sErrorText = string("Exception NSPatientChoisi::Initialisation during post-initialization.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
}

  InitialisationContinue() ;
}

void
NSPatientChoisi::InitialisationCreateLdVDocument()
{
  _iCurrentOpeningStep = ipsLDV ;

  // ---------------------------------------------------------------------------
	// Initialisation de la ligne de vie
try
{
  string ps = pContexte->getSuperviseur()->getText("patientOpening", "initializingLigneDeVie") + string("\n") ;
	pContexte->getSuperviseur()->trace(&ps, 1) ;
  pContexte->getSuperviseur()->afficheStatusMessage(ps) ;

	// pProgressDlg->CheckLDV() ;
  _progressDialog->setProgressAndText(80, string("Initialisation de la Ligne de vie")) ;

	LdV_init() ;

  ps = pContexte->getSuperviseur()->getText("patientOpening", "LigneDeVieInitialized") + string("\n") ;
	pContexte->getSuperviseur()->trace(&ps, 1) ;
  pContexte->getSuperviseur()->afficheStatusMessage(ps) ;
}
catch (...)
{
  string sErrorText = string("Exception NSPatientChoisi::Initialisation while opening Ligne de vie.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
}

  InitialisationContinue() ;
}

void
NSPatientChoisi::InitialisationOpenOtherVues()
{
  _iCurrentOpeningStep = ipsOtherViews ;

try
{
  ArrayWinProp* pWinProps = pContexte->getUtilisateur()->getWindowPropertyArray() ;
  if (((ArrayWinProp*) NULL == pWinProps) || pWinProps->empty())
    InitialisationContinue() ;

  for (ArrayWPIter iter = pWinProps->begin() ; pWinProps->end() != iter ; iter++)
  {
    if (NSWindowProperty::openedWithPatient == (*iter)->getActivity())
    {
      if      (string("HealthTeamList")   == (*iter)->getFunction())
        heathTeamListShow() ;
      else if (string("HealthTeamRosace") == (*iter)->getFunction())
        heathTeamRosaceShow() ;
      else if (string("DrugManagement")   == (*iter)->getFunction())
        drugs_show() ;
      else if (string("GoalManagement")   == (*iter)->getFunction())
        goals_show() ;
      else if (string("AdminData")        == (*iter)->getFunction())
        admin_show() ;
      else if (string("GoalsFollowUp")    == (*iter)->getFunction())
        followUp_show() ;
      else if (string("VenuesList")       == (*iter)->getFunction())
        venuesListShow() ;
    }
  }
}
catch (...)
{
  string sErrorText = string("Exception NSPatientChoisi::Initialisation while showing various windows.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
}

  InitialisationContinue() ;
}

void
NSPatientChoisi::InitialisationOpenLdVVue()
{
  _iCurrentOpeningStep = ipsOpenLDV ;

  // Mode Ouverture automatique de la Ligne de vie
	// if ((pSuper->getEpisodus()) && (pSuper->getEpisodus()->bAutoOpenLdV))

  string ps = pContexte->getSuperviseur()->getText("patientOpening", "openingLigneDeVie") + string("\n") ;
	pContexte->getSuperviseur()->trace(&ps, 1) ;

  _progressDialog->setProgressAndText(90, string("Ouverture de la Ligne de vie")) ;

try
{
  LdV_show() ;
}
catch (...)
{
  string sErrorText = string("Exception NSPatientChoisi::Initialisation while showing Ligne de vie.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
}

  ps = pContexte->getSuperviseur()->getText("patientOpening", "LigneDeVieopened") + string("\n") ;
	pContexte->getSuperviseur()->trace(&ps, 1) ;

  InitialisationContinue() ;
}

void
NSPatientChoisi::InitialisationRCVue()
{
  _iCurrentOpeningStep = ipsRC ;

  // Ouverture du gestionnaire de RC
#ifndef __EPIBRAIN__
try
{
	if (pContexte->getPredi() &&
      (pContexte->getPredi()->_sNomModule.find("SCMG") != string::npos))
  {
    NSDocViewManager dvManager(pContexte) ;
  	dvManager.createView(_pDocLdv, "RC view Format") ;
  }
}
catch (...)
{
  string sErrorText = string("Exception NSPatientChoisi::Initialisation while opening RC window.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
}
#endif // !__EPIBRAIN__


  InitialisationContinue() ;
}

void
NSPatientChoisi::InitialisationPredi()
{
  _iCurrentOpeningStep = ipsPredi ;

  #ifndef __EPIBRAIN__
try
{
	if ((pContexte->getPredi()) && (pContexte->getPredi()->_bPrediActif))
	{
    _progressDialog->setProgressAndText(95, string("Ouverture de PreDi")) ;

		pContexte->getPredi()->PatOpening() ;
		//    DPIO(pContexte) ;
		// bDPIO = true ;
	}
}
catch (...)
{
  string sErrorText = string("Exception NSPatientChoisi::Initialisation during opening signal for Predi.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
}
#endif // !__EPIBRAIN__

  InitialisationContinue() ;
}

void
NSPatientChoisi::InitialisationAdaptMailBox()
{
  _iCurrentOpeningStep = ipsMailbox ;

  if (pContexte->_pMailBoxWindow)
  	pContexte->_pMailBoxWindow->SetFocus() ;

  InitialisationContinue() ;
}

void
NSPatientChoisi::InitialisationEnd()
{
  _iCurrentOpeningStep = ipsFinal ;

try
{
  NSSuper *pSuper = pContexte->getSuperviseur() ;

  if (string("") != pSuper->getStaysSynchroDll())
  {
    _progressDialog->setProgressAndText(95, string("Synchronisation du séjour")) ;

    string sStaysSynchroDll = pSuper->getStaysSynchroDll() ;

    string ps = string("Synchronizing venues using dll ") + sStaysSynchroDll ;
    pSuper->trace(&ps, 1) ;

    TModule* pDCModule = new TModule(sStaysSynchroDll.c_str(), TRUE) ;
    if (NULL == pDCModule)
    {
      string sErrorText = pSuper->getText("modulesManagement", "cannotLoadDll") + string(" ") + sStaysSynchroDll ;
      pSuper->trace(&sErrorText, 1) ;
    }
    else
    {
      bool (FAR *pAdresseFct) (NSContexte far *) ;
      (FARPROC) pAdresseFct = pDCModule->GetProcAddress(MAKEINTRESOURCE(1)) ;
      if (NULL == pAdresseFct)
      {
        string sErrorText = pSuper->getText("modulesManagement", "cannotAccessMethodInDll") + string(" ") + sStaysSynchroDll + string(" -> exporter") ;
        pSuper->trace(&sErrorText, 1) ;
        erreur(sErrorText.c_str(), standardError, -1, pContexte->GetMainWindow()->GetHandle()) ;
      }
      else
        /* bool bReussi = */ ((*pAdresseFct)((NSContexte far *) pContexte)) ;

      delete pDCModule ;
    }
  }
}
catch (...)
{
  string sErrorText = string("Exception NSPatientChoisi::Initialisation during synchronization process.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
}

  _progressDialog->setProgress(100) ;
  _progressDialog->Destroy() ;

  delete _progressDialog ;
  _progressDialog = 0 ;

  if (opnOpenReqResultDocum == _iWhatToDoAfterOpening)
  {
    NSResultReqDialog* pReqResult = pContexte->getUtilisateur()->getResult() ;
    if ((NSResultReqDialog*) NULL == pReqResult)
      return ;

    // cas du mode document : on ouvre aussi le document
    //
    NSDocumentInfo DocumentInfo(pContexte) ;

    NSDocumentHisto* pSelectedDoc = pReqResult->getSelectedResultDocum() ;
    if ((NSDocumentHisto*) NULL == pSelectedDoc)
      return ;

    NSDocumentData InfoData ;
    pSelectedDoc->initFromData(&InfoData) ;
    DocumentInfo.setData(&InfoData) ;

    _pDocHis->AutoriserOuverture(&DocumentInfo) ;
  }
}

void
NSPatientChoisi::PostInitialisation()
{
try
{
	string sLang = pContexte->getUserLanguage() ;

  // On fixe les données de la Caption à cet endroit
  // calcul de l'age du patient
  // char    szDateNaiss[9] ;
  // int     age = -1 ;
  // char    szAge[4] ;  // char    szDateJour[10] ;  if (getContribution() == string(""))    CreeContribution() ;  // on fabrique le nom long  pContexte->getPatient()->fabriqueNomLong() ;

  pContexte->setMainCaption() ;

  // Synchronizing mail box
  //
	if ((pContexte->_pMailBoxWindow) && (pContexte->_pMailBoxWindow->getList()))
  	pContexte->_pMailBoxWindow->getList()->selectCurrentPatientFirstMessage() ;

  _bIsOpen = true ;
}
catch (...)
{
	erreur("Exception NSPatientChoisi::PostInitialisation.", standardError, 0) ;
}
}

void
NSPatientChoisi::PatientDataChanged(NSPatPathoArray *pPatPathoArray)
{
  // Update NSPatInfo from the patpatho
  //
  ChargeDonneesPatient(pPatPathoArray) ;
  pContexte->setMainCaption() ;

  //
  //
  NSPersonInfo* pPersonInfo = pContexte->getPersonArray()->lookForPersonInArray(getNss()) ;
  if (pPersonInfo)
  {
    string sCode ;
    string sSex ;

    string sNom       = string("") ;
    string sPrenom    = string("") ;
    string sNaissance = string("") ;
    string sCivilite  = string("") ;
    string sPhone     = string("") ;
    string sMobile    = string("") ;
    string sMail      = string("") ;

    _GraphPerson.ChargeDonneesAdmin(pContexte, pPatPathoArray, sNom, sPrenom, sCode, sSex, sNaissance, sCivilite, sPhone, sMobile, sMail) ;

    if      (string("1") == sSex)
      pPersonInfo->metMasculin() ;
    else if (string("2") == sSex)
      pPersonInfo->metFeminin() ;

    if (string("") != sNom)
      pPersonInfo->setNom(sNom) ;
    pPersonInfo->setPrenom(sPrenom) ;
    pPersonInfo->setNaissance(sNaissance) ;
    pPersonInfo->setPhone(sPhone) ;
    pPersonInfo->setMobile(sMobile) ;
    pPersonInfo->setMail(sMail) ;
    pPersonInfo->_sCivilite = sCivilite ;

    pPersonInfo->updateCalculatedValues(&_GraphPerson, pidsPatient) ;
  }

  //
  //
  if (_pDocLdv)
    _pDocLdv->invalidateViews("MISCELLANOUS") ;
}

bool
NSPatientChoisi::etablirLiensTree(bool &bCreerMetaLien, string sNodeMeta, string sNodeData)
{
	// if (pDocRoot->NSNoyauDocument::bCreerMetaLien)
  if (bCreerMetaLien)
  {
  	NSLinkManager Link(pContexte->getSuperviseur(), _GraphPerson.getDataGraph()) ;
    if (false == Link.etablirLien(sNodeMeta, NSRootLink::docData, sNodeData))    {
    	erreur("Impossible de faire le lien vers le document data.", standardError, 0) ;
      return false ;
    }    bCreerMetaLien = false ;
  }
  else if (string("") != sNodeMeta)             // sCodeDocMeta
  {  	// Note : le lien contributionModified ne peut s'établir que la deuxième fois    // qu'on enregistre les données, car la première fois bCreerMetaLien est true.    string sContribution = pContexte->getPatient()->getContribution() ;    NSLinkManager Link(pContexte->getSuperviseur(), _GraphPerson.getDataGraph()) ;    if (false == Link.existeLien(sNodeMeta, NSRootLink::contributionModified, sContribution))    {    	// lien du méta vers la contribution en cours      if (false == Link.etablirLien(sNodeMeta, NSRootLink::docData, sNodeData))      {
      	string sErrorText = pContexte->getSuperviseur()->getText("documentManagementErrors", "cannotEstablishALinkWithTheContribution") ;
        pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
        erreur(sErrorText.c_str(), standardError, 0) ;
        return false ;
      }    }  }
  return true ;
}

// Création du noeud racine patient
////////////////////////////////////
bool
NSPatientChoisi::CreeRootNode()
{
	// ---------------------------------------------------------------------------
  // Création du noeud personne
	// Person node creation

  NSRefDocument DocRoot(0, 0, 0, pContexte, false) ;

  NSPatPathoArray PptRoot(pContexte->getSuperviseur()) ;
	PptRoot.ajoutePatho("HHUMA3", 0) ;

  DocRoot.setPatPatho(&PptRoot) ;

	string sIdRoot = _GraphPerson.getRootTree() ;
  string sRootDoc = sIdRoot.substr(7, 6) ;

  if (false == DocRoot.Referencer("ZCS00", "Noeud racine patient", "", "", false, false, sRootDoc, NSRootLink::personDocument, 0, ""))
  	return false ;

  string sRootDocData = _GraphPerson.getDataGraph()->getLastTree() ;

  DocRoot.initFromPatPatho(&PptRoot) ;
  _GraphPerson.setTree(&PptRoot, "", sRootDocData) ;

  // etablir lien ZA
  // bool *pCreerMetaLien = &(DocRoot.NSNoyauDocument::bCreerMetaLien) ;
  // etablirLiensTree(pCreerMetaLien, DocRoot.sCodeDocMeta, sRootDocData) ;

  etablirLiensTree(DocRoot.NSNoyauDocument::_bCreerMetaLien, DocRoot.getCodeDocMeta(), sRootDocData) ;

	return true ;
}

// Création de l'index de santé
//////////////////////////////////
bool
NSPatientChoisi::CreeRootIndex()
{
	// ---------------------------------------------------------------------------
  // Création des l'index de santé
	// Frames creation

  for (int i = 0 ; i < FRAMECOUNT ; i++)
  {
    LDVFRAME iFrame = getFrameForIndex(i) ;
    if (ldvframeNotSpecified != iFrame)
    {
      NSRefDocument	PbIndex(0, 0, 0, pContexte, false) ;
      bool bIndexCreated = PbIndex.createFrame(&_GraphPerson, iFrame) ;
      if (false == bIndexCreated)
        return false ;
    }
  }

  return true ;
}

// Création de l'index de santé
//////////////////////////////////
bool
NSPatientChoisi::CreeRootIndexExtensions()
{
  string sRootTree = _GraphPerson.getRootTree() ;

  NSLinkManager* pLink = _GraphPerson.getLinkManager() ;
  if (NULL == pLink)
    return false ;

  VecteurString VectString ;

  // Health index (and other indexes)
  pLink->TousLesVrais(sRootTree, NSRootLink::personHealthIndex, &VectString) ;
  if (VectString.empty())
    return false ;

  string sRootIndexId = **(VectString.begin()) ;

  // Create external tanks for concerns, treatments and goals
  //
  NSRefDocument	concernsTank(0, 0, 0, pContexte, false) ;
  if (false == concernsTank.createFrameConcernsTree(&_GraphPerson, sRootIndexId))
    return false ;

  NSRefDocument	treatmentsTank(0, 0, 0, pContexte, false) ;
  if (false == treatmentsTank.createFrameTreatmentsTree(&_GraphPerson, sRootIndexId))
    return false ;

  NSRefDocument	goalsTank(0, 0, 0, pContexte, false) ;
  if (false == goalsTank.createFrameGoalsTree(&_GraphPerson, sRootIndexId))
    return false ;

  return true ;
}


// Création de la fiche de synthèse
/////////////////////////////////////
bool
NSPatientChoisi::CreeRootSynthese()
{
	// ---------------------------------------------------------------------------
  // Création de la fiche de synthèse
	// Synthesis creation

  NSRefDocument DocSynth(0, 0, 0, pContexte, false) ;
  return DocSynth.createSynthesisFrame(&_GraphPerson) ;
}

// Création de la fiche administrative
////////////////////////////////////////
bool
NSPatientChoisi::CreeRootAdmin(NSPatPathoArray *pPatPathoAdmin)
{
	// ---------------------------------------------------------------------------
  // Création de l'arbre administratif
  // Administrative Tree creation

  NSRefDocument DocAdmin(0, 0, 0, pContexte, false) ;
	DocAdmin.setPatPatho(pPatPathoAdmin) ;

	if (false == DocAdmin.Referencer("ZCQ00", "Fiche administrative", "", "", true, false, "", NSRootLink::personAdminData, 0, ""))
  	return false ;

  // le code est etabli dans la methode referencer
  string sRootDocData = _GraphPerson.getDataGraph()->getLastTree() ;

  NSPatPathoArray PptRoot(pContexte->getSuperviseur()) ;
  DocAdmin.initFromPatPatho(&PptRoot) ;

  _GraphPerson.setTree(&PptRoot, string(""), sRootDocData) ;

  // etablir lien ZA entre les metaDonnées et les données.
  // bool *pCreerMetaLien = &(DocAdmin.NSNoyauDocument::bCreerMetaLien) ;
  // etablirLiensTree(pCreerMetaLien, DocAdmin.sCodeDocMeta, sRootDocData) ;

  etablirLiensTree(DocAdmin.NSNoyauDocument::_bCreerMetaLien, DocAdmin.getCodeDocMeta(), sRootDocData) ;

	return true ;
}

// Création de la bibliothèque de chemises (folders)
/////////////////////////////////////////////////////
bool
NSPatientChoisi::CreeRootLibChem()
{
  // ---------------------------------------------------------------------------
  // Création de l'arbre-bibliothèque de chemises
  // "chemises" library tree creation

  NSRefDocument LibChem(0, 0, 0, pContexte, false) ;
  return LibChem.createFoldersLibrary(&_GraphPerson) ;
}

// Creating Health Team
/////////////////////////////////////////////////////
bool
NSPatientChoisi::CreeRootHealthTeam(string sPersonId)
{
  NSRefDocument HealthTeamDoc(0, 0, 0, pContexte, false) ;
  return HealthTeamDoc.createHealthTeam(&_GraphPerson, sPersonId) ;
}

// Creating Identifiers Library
/////////////////////////////////////////////////////
bool
NSPatientChoisi::createIdentifiersLibrary(string sIpp)
{
  NSRefDocument IdentifiersLibDoc(0, 0, 0, pContexte, false) ;
  return IdentifiersLibDoc.createIdentifiersLibrary(&_GraphPerson, sIpp) ;
}

// Creating identifiers library
// Création de la bibliothèque d'identifiants
/////////////////////////////////////////////////////
bool
NSPatientChoisi::CreeRootLibIDs()
{
  NSRefDocument DocLibId(0, 0, 0, pContexte, false) ;
  return DocLibId.createIdentifiersFrame(&_GraphPerson) ;
}

// -----------------------------------------------------------------------------
// Description	: Crée tous les documents root d'un nouveau patient :
//									* Noeud personne
//									* index de santé
//									* fiche de synthèse
//									* arbre administratif
//									* arbre Equipe de Santé
// Initialise le graphe de liaison entre ces documents
// --
// Returns			: True->Succès False->Echec
// -----------------------------------------------------------------------------
// Description	: Create root documents for a new petient :
//									* person node
//									* health index
//									* Synthesis
//									* administrative tree
//									* health team tree (with just the patient himself)
// Initialize already the graph of links between documents.
// --
// Returns			: if success true else false
// -----------------------------------------------------------------------------
bool
NSPatientChoisi::CreeRootDocs(NSPatPathoArray *pPatPathoAdmin)
{
try
{
	if (false == CreeRootNode())
  	return false ;

	CreeContribution(true) ;

  if (false == CreeRootLibChem())
  	return false ;

	if (false == CreeRootIndex())
  	return false ;

  if (false == CreeRootSynthese())
  	return false ;

  if (false == CreeRootAdmin(pPatPathoAdmin))
  	return false ;

	return true ;
}
catch (...)
{
	erreur("Exception CreeRootDocs.", standardError, 0) ;
	return false ;
}
}

bool
NSPatientChoisi::IsDocRoot(string sCodeDoc)
{
	string sNss = string(sCodeDoc, 0, PAT_NSS_LEN) ;
  string sNodeRoot = sNss + string(1, LocalPatient) + string(DOC_CODE_DOCUM_LEN - 1, '0') ;

	NSLinkManager* pGraphe = _GraphPerson.getLinkManager() ;

	bool bResult = false ;
  if ((sCodeDoc == sNodeRoot) ||
        (pGraphe->existeLien(sNodeRoot, NSRootLink::personHealthIndex, sCodeDoc)) ||
        (pGraphe->existeLien(sNodeRoot, NSRootLink::personSocialIndex, sCodeDoc)) ||
        (pGraphe->existeLien(sNodeRoot, NSRootLink::personRiskManager, sCodeDoc)) ||
        (pGraphe->existeLien(sNodeRoot, NSRootLink::personIndexExtension, sCodeDoc)) ||
        (pGraphe->existeLien(sNodeRoot, NSRootLink::personSynthesis, sCodeDoc)) ||
        (pGraphe->existeLien(sNodeRoot, NSRootLink::personAdminData, sCodeDoc)) ||
        (pGraphe->existeLien(sNodeRoot, NSRootLink::personFolderLibrary, sCodeDoc)) ||
        (pGraphe->existeLien(sNodeRoot, NSRootLink::personHealthTeam, sCodeDoc)))
		bResult = true ;

	return bResult ;
}

bool
NSPatientChoisi::CreeElementFromRef(string sDestinationPath, NSPatPathoArray *pPatPatho, string sDocument, string sRefId, string sProposalId, NSPatPathoArray *pPatPathoMotif, string sConcern)
{
try
{
#ifndef __EPIPUMP__
	if (((NSPatPathoArray*) NULL == pPatPatho) || (pPatPatho->empty()))
		return true ;

	if (string("") == sDestinationPath)
		return true ;

  // Parsing the destination path string
  //
  ClasseStringVector Vect ;
	DecomposeChaine(&sDestinationPath, &Vect, string("/")) ;
  if (Vect.empty())
  	return false ;

	iterClassString iterpVect = Vect.begin() ;

	string sTargetDoc     = (*iterpVect)->getItem() ;
  string sTargetChapter = string("") ;

  iterpVect++ ;
  if (Vect.end() != iterpVect)
    sTargetChapter = (*iterpVect)->getItem() ;

  // Checking if Doc and Chapter are valid
  //
  if (string("") == sTargetChapter)
  {
    string sErrorText = pContexte->getSuperviseur()->getText("referentialErrors", "invalidTargetForElement") ;
    sErrorText += string(" ") + sDestinationPath ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
    return false ;
  }

  // Looking for frame
  //
  LDVFRAME iFrame = _pDocLdv->getFrameTypeFromRoot(sTargetDoc) ;
  if (ldvframeNotSpecified == iFrame)
  {
    string sErrorText = string("NSPatientChoisi::CreeElementFromRef cannot find Frame for root ") + sTargetDoc ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
    return false ;
  }

  // Looking for chapter
  //
  string sChapterSens = string("") ;
  pContexte->getDico()->donneCodeSens(&sTargetChapter, &sChapterSens) ;

  NSRootLink::NODELINKTYPES iNodeLink = NSRootLink::badLink ;

  if      (string("N0000") == sChapterSens)
    iNodeLink = NSRootLink::drugOf ;
  else if (string("0OBJE") == sChapterSens)
    iNodeLink = NSRootLink::indexGoals ;
  else if (string("0PRO1") == sChapterSens)
    iNodeLink = NSRootLink::indexConcerns ;
  else
  {
    string sErrorText = string("NSPatientChoisi::CreeElementFromRef bad chapter: ") + sTargetChapter ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
    return false ;
  }

  // Recherche du document
	// Looking for the document
  //
/*
	DocumentIter iterDoc = findFrameIndex(sTargetDoc) ;
  if (NULL == iterDoc)
  {
    string sErrorText = pContexte->getSuperviseur()->getText("referentialErrors", "invalidTargetForElement") ;
    sErrorText += string(" ") + sDestinationPath ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
    return false ;
  }
*/

  NSDocumentHisto* pIndexDocHisto = _pDocLdv->getStorageDocument(iFrame, iNodeLink) ;
	if ((NSDocumentHisto*) NULL == pIndexDocHisto)
  {
    string sErrorText = string("NSPatientChoisi::CreeElementFromRef cannot get storage document.") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
    return false ;
  }

	// Document found
	NSDocumentInfo Docum(pContexte) ;

  NSDocumentData InfoData ;
  pIndexDocHisto->initFromData(&InfoData) ;
  Docum.setData(&InfoData) ;

	NSNoyauDocument Noyau(0, &Docum, 0, pContexte, false, true) ;

  NSPatPathoArray PptNoyau(pContexte->getSuperviseur()) ;
  Noyau.initFromPatPatho(&PptNoyau) ;

	// Looking for the chapter - On cherche le chapitre
  //
	PatPathoIter iter = PptNoyau.ChercherItem(sTargetChapter) ;
	if ((NULL == iter) || (PptNoyau.end() == iter))
	{
    string sErrorText = string("NSPatientChoisi::CreeElementFromRef cannot get Goals chapter.") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur("Erreur grave : le document Index de santé de ce patient ne possède pas de chapitre Objectifs de santé.", standardError, 0) ;
		return false ;
	}

	// Ajout d'un nouveau segment d'arbre
	// Adding a new tree branch
	// Ajout de la branche à l'arbre Index de santé
	// Adding the branch to the Health index tree

	// On cherche où il faut insérer la branche
	// Looking for the place to insert the branch
	int colonneMere = (*iter)->getColonne() ;
	iter++ ;
	while ((PptNoyau.end() != iter) && ((*iter)->getColonne() > colonneMere))
  {
  	// on vérifie si un fils identique existe déjà
    if ((*iter)->getColonne() == (colonneMere + 1))
    {
    	if ((*iter)->getLexique() == (*(pPatPatho->begin()))->getLexique())
      {
        string sLangue = pContexte->getUserLanguage() ;
        string sCodeLex = (*iter)->getLexique() ;
        string sLibelle ;
        pContexte->getDico()->donneLibelle(sLangue, &sCodeLex, &sLibelle) ;
      }
    }
    iter++ ;
  }

  //
  // On cherche un(les) élément calculé(s)
  //
  NSPatPathoArray pptCopy(pContexte->getSuperviseur()) ;
  LocalPatho(pPatPatho, &pptCopy) ;

  if (pptCopy.empty())
  {
    string sErrorText = string("NSPatientChoisi::CreeElementFromRef empty copy patpatho.") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  	return false ;
  }

  //
  // On insère l'identifiant d'objectif au sein du référentiel
  //
  if ((sProposalId != "") && (strlen(sProposalId.c_str()) <= BASE_COMPLEMENT_LEN))
  {
  	PatPathoIter iterRoot = pptCopy.begin() ;

    int iLocalRootCol = (*iterRoot)->getColonne() ;

    iterRoot++ ;

    Message CodeMsg ;
    CodeMsg.SetLexique("£REFC1") ;
    CodeMsg.SetComplement(sProposalId) ;

    int iDecalLigne ;
    if (pptCopy.end() != iterRoot)
    	iDecalLigne = 0 ;
    else
    	iDecalLigne = 1 ;

    pptCopy.ajoutePatho(iterRoot, "£REFC1", &CodeMsg, iLocalRootCol+1, iDecalLigne, true) ;
  }

  // on note ici la position en ligne et colonne de l'élément que l'on va insérer
  int ligneIns, colonneIns ;

  if (PptNoyau.end() != iter)
  {
  	ligneIns = (*iter)->getLigne() ;
    colonneIns = colonneMere + 1 ;
  }
  else
  {
  	PatPathoIter iterEnd = iter ;
    iterEnd-- ;
    ligneIns = (*iterEnd)->getLigne() + 1 ;
    colonneIns = colonneMere + 1 ;
  }

  // Insertion de l'élément
  PptNoyau.InserePatPatho(iter, &pptCopy, colonneIns) ;

	_GraphPerson.setTree(&PptNoyau, "") ;

	PatPathoIter itRoot = PptNoyau.ChercherItem(ligneIns, colonneIns) ;
	if ((NULL == itRoot) || (PptNoyau.end() == itRoot))
	{
    string sErrorText = string("NSPatientChoisi::CreeElementFromRef information not properly recorded (cannot find target element).") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur("Erreur grave : l'élément n'a pas été correctement enregistré.", standardError, 0) ;
		return false ;
	}

	string sNoeud = (*itRoot)->getNode() ;

	//
	// Lien entre le noeud et son document créateur
	//
	NSLinkManager* pGraphe = _GraphPerson.getLinkManager() ;
  if (string("") != sDocument)
  	pGraphe->etablirLien(sDocument, NSRootLink::refCreator, sNoeud) ;

	//
	// Lien entre le noeud et le référentiel
	//
	if (string("") != sRefId)
	{
		string sRefNode = pContexte->getArcManager()->DonneNoeudArchetype(NSArcManager::referentiel, sRefId) ;
		if (string("") != sRefNode)
			pGraphe->etablirLien(sRefNode, NSRootLink::referentialOf, sNoeud) ;
	}

	//
	// Lien entre le noeud et la préoccupation de santé qu'il concerne (plusieurs ?)
	//

  if (string("") != sConcern)
  {
    if      (string("N0000") == sChapterSens)
      pGraphe->etablirLien(sNoeud, NSRootLink::drugOf, sConcern) ;
    else if (string("0OBJE") == sChapterSens)
      pGraphe->etablirLien(sNoeud, NSRootLink::problemGoals, sConcern) ;
  }
  else if (string("") != sDocument)
  {
  	VecteurString aVecteurString ;
    pGraphe->TousLesVraisDocument(sDocument, NSRootLink::problemRelatedTo, &aVecteurString) ;
    if (false == aVecteurString.empty())
    {
    	string sPrimoPb = *(*(aVecteurString.begin())) ;

      if      (string("N0000") == sChapterSens)
        pGraphe->etablirLien(sNoeud, NSRootLink::drugOf, sPrimoPb) ;
      else if (string("0OBJE") == sChapterSens)
        pGraphe->etablirLien(sNoeud, NSRootLink::problemGoals, sPrimoPb) ;
    }
  }

	// Enregistrement du document modifié
  //
  Noyau.setPatPatho(&PptNoyau) ;
	Noyau.enregistrePatPatho() ;

	// Mise à jour de l'historique
	Noyau.chargePatPatho() ;
	Noyau.initFromPatPatho(&PptNoyau) ;
	_pDocHis->Rafraichir(&Docum, &PptNoyau) ;

	if (PptNoyau.empty())
	{
    string sErrorText = string("NSPatientChoisi::CreeElementFromRef information not properly recorded (empty refreshed patpatho).") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur("Erreur grave : l'élément n'a pas été correctement enregistré.", standardError, 0) ;
		return false ;
	}

  if (string("N0000") == sChapterSens)
  {
    LDVFRAME iFrame = _pDocLdv->getFrameTypeFromRoot(sTargetDoc) ;
    if (ldvframeNotSpecified == iFrame)
      iFrame = ldvframeHealth ;

    // Adding this treatment on the Ligne de vie
    //
    _pDocLdv->getDrugs(iFrame)->loadDrugs(&PptNoyau, itRoot, (*itRoot)->getColonne() - 1, true /*bJustOne*/) ;
    _pDocLdv->showNewDrug(&PptNoyau, itRoot) ;
  }

#endif // !__EPIPUMP__

	return true ;
}
catch (...)
{
	erreur("Exception CreeElementFromRef.", standardError, 0) ;
	return false;
}
}

bool
NSPatientChoisi::CreeObjectif(NSPatPathoArray* pPatPatho, string sDocument, string sRefId, string sGoalId, NSPatPathoArray* pPatPathoMotif, LDVFRAME iFrame, bool bSaveNow, string sGoal)
{
try
{
#ifndef __EPIPUMP__

	if ((NULL == pPatPatho) || (pPatPatho->empty()))
		return true ;

	// Looking for "Goals index" document for frame
  //
  NSDocumentHisto* pGoalsIndexDoc = _pDocLdv->getStorageDocument(iFrame, NSRootLink::indexGoals) ;
	if (NULL == pGoalsIndexDoc)
		return false ;

	// Dimension index found
	NSDocumentInfo Docum(pContexte) ;

  NSDocumentData InfoData ;
  pGoalsIndexDoc->initFromData(&InfoData) ;
  Docum.setData(&InfoData) ;

	NSNoyauDocument Noyau(0, &Docum, 0, pContexte, false, true) ;

  NSPatPathoArray PptNoyau(pContexte->getSuperviseur()) ;
  Noyau.initFromPatPatho(&PptNoyau) ;

	// On cherche le chapitre "objectifs de santé"
  // Looking for "goals" chapter
  //
	PatPathoIter iter = PptNoyau.ChercherItem("0OBJE1") ;
	if ((NULL == iter) || (PptNoyau.end() == iter))
  {
    string sErrorText = pContexte->getSuperviseur()->getText("goalsManagementErrors", "missingGoalsRepositoryBranchInIndex") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
		return false ;
	}

	// Ajout d'un nouveau segment d'arbre
	// Adding a new tree branch
	// Ajout de la branche à l'arbre Index de santé
	// Adding the branch to the Health index tree

	// On cherche où il faut insérer la branche
	// Looking for the place to insert the branch
	int colonneMere = (*iter)->getColonne() ;
	iter++ ;
	while ((PptNoyau.end() != iter) && ((*iter)->getColonne() > colonneMere))
  {
  	// on vérifie si un fils identique existe déjà
    if ((*iter)->getColonne() == (colonneMere + 1))
    {
    	if ((*iter)->getLexique() == (*(pPatPatho->begin()))->getLexique())
      {
        string sLangue = pContexte->getUserLanguage() ;
        string sCodeLex = (*iter)->getLexique() ;
        string sLibelle ;
        pContexte->getDico()->donneLibelle(sLangue, &sCodeLex, &sLibelle) ;
/*
				char msg[255] ;
        sprintf(msg, "L'objectif \"%s\" existe déjà pour ce patient. Voulez-vous en insérer un autre ?", sLibelle.c_str()) ;
        int iRetVal = ::MessageBox(pContexte->GetMainWindow()->GetHandle(),
                                    msg, "Attention", MB_YESNO) ;

				if (iRetVal != IDYES)
					return true ;
*/
      }
    }

    iter++ ;
  }

  //
  // On cherche un(les) élément calculé(s)
  //
  NSPatPathoArray pptCopy(pContexte->getSuperviseur()) ;
  LocalPatho(pPatPatho, &pptCopy) ;

  if (pptCopy.empty())
  	return false ;

  //
  // On insère l'identifiant d'objectif au sein du référentiel
  //
  if ((string("") != sGoalId) && (strlen(sGoalId.c_str()) <= BASE_COMPLEMENT_LEN))
  {
  	PatPathoIter iterRoot = pptCopy.begin() ;

    int colonneMere = (*iterRoot)->getColonne() ;

    iterRoot++ ;

    Message CodeMsg("") ;
    CodeMsg.SetLexique("£REFC1") ;
    CodeMsg.SetComplement(sGoalId) ;
    int iDecalLigne ;
    if (iterRoot != pptCopy.end())
    	iDecalLigne = 0 ;
    else
    	iDecalLigne = 1 ;
    pptCopy.ajoutePatho(iterRoot, "£REFC1", &CodeMsg, colonneMere+1, iDecalLigne, true) ;
  }

  // on note ici la position en ligne et colonne de l'élément que l'on va insérer
  int ligneIns, colonneIns;

  if (PptNoyau.end() != iter)
  {
  	ligneIns = (*iter)->getLigne() ;
    colonneIns = colonneMere + 1 ;
  }
  else
  {
  	PatPathoIter iterEnd = iter ;
    iterEnd-- ;
    ligneIns = (*iterEnd)->getLigne() + 1 ;
    colonneIns = colonneMere + 1 ;
  }

  // Insertion de l'objectif
  // Inserting goal's tree inside tree
  //
  PptNoyau.InserePatPatho(iter, &pptCopy/*pPatPatho*/, colonneIns) ;

  // Updating tree in graph so that new nodes get a memory Id
  //
	_GraphPerson.setTree(&PptNoyau, "") ;

	PatPathoIter itRoot = PptNoyau.ChercherItem(ligneIns, colonneIns) ;
	if ((NULL == itRoot) || (PptNoyau.end() == itRoot))
	{
    string sErrorText = pContexte->getSuperviseur()->getText("goalsManagementErrors", "goalCouldNotBeRecorded") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
		return false;
	}

	string sNoeud = (*itRoot)->getNode() ;

	//
	// Lien entre le noeud et son document créateur
  // Connecting node and document
	//
	NSLinkManager* pGraphe = _GraphPerson.getLinkManager() ;
  if (string("") != sDocument)
  	pGraphe->etablirLien(sDocument, NSRootLink::refCreator, sNoeud) ;
	//
	// Lien entre le noeud et le référentiel
  // Connecting node and referential
	//
	if (string("") != sRefId)
	{
		string sRefNode = pContexte->getArcManager()->DonneNoeudArchetype(NSArcManager::referentiel, sRefId) ;
		if (string("") != sRefNode)
			pGraphe->etablirLien(sRefNode, NSRootLink::referentialOf, sNoeud) ;
	}

	//
	// Lien entre le noeud et la préoccupation de santé qu'il concerne (plusieurs ?)
  // Connecting node and concern(s ?)
	//
  if (string("") != sDocument)
  {
  	VecteurString aVecteurString ;
    pGraphe->TousLesVraisDocument(sDocument, NSRootLink::problemRelatedTo, &aVecteurString) ;
    if (false == aVecteurString.empty())
    {
    	string sPrimoPb = *(*(aVecteurString.begin())) ;
      pGraphe->etablirLien(sNoeud, NSRootLink::problemGoals, sPrimoPb) ;
    }
  }
  else if (string("") != sGoal)
  	pGraphe->etablirLien(sNoeud, NSRootLink::problemGoals, sGoal) ;

  if (false == bSaveNow)
  {
    _pDocHis->Rafraichir(&Docum, &PptNoyau, (NSNoyauDocument*) 0, false) ;
    return true ;
  }

	// Enregistrement du document modifié
  // Recording all this
  //
  Noyau.setPatPatho(&PptNoyau) ;
	Noyau.enregistrePatPatho() ;

	// Mise à jour de l'historique
	/* bool bReload = */ Noyau.chargePatPatho() ;
	Noyau.initFromPatPatho(&PptNoyau) ;
	_pDocHis->Rafraichir(&Docum, &PptNoyau) ;

	if (PptNoyau.empty())
	{
    string sErrorText = pContexte->getSuperviseur()->getText("goalsManagementErrors", "goalCouldNotBeRecorded") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
		return false ;
	}

  UpdateLdvForNewGoal(iFrame, &PptNoyau, ligneIns, colonneIns) ;

/*
	//
  // On reconstitue la pptCopy avec les bons numéros de noeuds (il faut
  // l'extraire de l'index de santé)
  // We have to rebuild pptCopy in order to get nodes ID (we have to get
  // it from health index)
  //

  NSPatPathoArray pptFille(pContexte->getSuperviseur()) ;
  pPt->ExtrairePatPatho(itRoot, &pptFille) ;

  pptCopy.vider() ;

  NSPatPathoInfo racine = *(*itRoot) ;
  racine.setLigne(0) ;
  racine.setColonne(0) ;
  pptCopy.push_back(new NSPatPathoInfo(racine)) ;

  if (false == pptFille.empty())
  {
  	PatPathoIter ssiter ;    for (ssiter = pptFille.begin() ; pptFille.end() != ssiter ; ssiter++)
    {
    	(*ssiter)->setLigne((*ssiter)->getLigne() + 1) ;
      (*ssiter)->setColonne((*ssiter)->getColonne() + 1) ;
      pptCopy.push_back(new NSPatPathoInfo(*(*ssiter))) ;
    }
  }

  NSLdvGoal Goal(pContexte, pDocLdv, iFrame) ;
  PatPathoIter itPpt = itRoot ;
  Goal.initFromPatho(&pptCopy, &itPpt) ;

  // RegleObjectif(&Goal) ;
  pDocLdv->initGoal(&Goal) ;

#ifndef __EPIBRAIN__
  NSDPIO* pDPIO = pContexte->getSuperviseur()->getDPIO() ;
  pDPIO->sSendPage += string("<goal>\r\n") + pptCopy.genereXML() + string("</goal>\r\n") ;
#endif // !__EPIBRAIN__

  //
  // Prise en compte de l'objectif sur la Ligne de vie
  //
  pDocLdv->getGoals(iFrame)->loadGoals(itRoot, (*itRoot)->getColonne() - 1, iFrame, true) ;
*/

  _pDocLdv->invalidateViews("GOAL_NEW") ;

#endif // !__EPIPUMP__

	return true ;
}
catch (...)
{
	erreur("Exception CreeObjectif.", standardError, 0) ;
	return false;
}
}

// Save a batch of goals and update LdV
//
bool
NSPatientChoisi::SaveGoalsBatch(LDVFRAME iFrame)
{
  bool bIsOk = true ;

  // Getting index for frame
  //
  DocumentIter iterDoc = getFrameIndex(iFrame) ;
  if (NULL != iterDoc)
  {
    if (false == SaveGoalsBatch(*iterDoc, iFrame))
      bIsOk = false ;
  }

  VecteurString aGoalIndexesIds ;
  if (_pDocLdv->getStorageDocumentsIds(&aGoalIndexesIds, iFrame, NSRootLink::indexGoals))
  {
    if (false == aGoalIndexesIds.empty())
    {
      for (EquiItemIter i = aGoalIndexesIds.begin() ; aGoalIndexesIds.end() != i ; i++)
      {
        DocumentIter docIter = _pDocHis->TrouveDocHisto(**i) ;
        if (NULL != docIter)
        {
          if (false == SaveGoalsBatch(*docIter, iFrame))
            bIsOk = false ;
        }
      }
    }
  }

  // Refresh Ldv view(s)
  //
  _pDocLdv->invalidateViews("GOAL_NEW") ;

  return bIsOk ;
}

bool
NSPatientChoisi::SaveGoalsBatch(NSDocumentHisto* pDocument, LDVFRAME iFrame)
{
  if (NULL == pDocument)
    return false ;

  // Getting index as a document
  //
	NSDocumentInfo Docum(pContexte) ;

  NSDocumentData InfoData ;
  pDocument->initFromData(&InfoData) ;
  Docum.setData(&InfoData) ;

	NSNoyauDocument Noyau(0, &Docum, 0, pContexte, false, true) ;

  NSPatPathoArray PptNoyau(pContexte->getSuperviseur()) ;
  Noyau.initFromPatPatho(&PptNoyau) ;

	// On cherche le chapitre "objectifs de santé"
  // Looking for "goals" chapter
  //
	PatPathoIter iter = PptNoyau.ChercherItem("0OBJE1") ;
	if ((NULL == iter) || (PptNoyau.end() == iter))
  {
    string sErrorText = pContexte->getSuperviseur()->getText("goalsManagementErrors", "missingGoalsRepositoryBranchInIndex") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
		return false ;
	}

  // Getting the lines of newly created goals
  //
  vector<int> aLines ;

  int iBaseCol = (*iter)->getColonne() ;
	iter++ ;
	while ((PptNoyau.end() != iter) && ((*iter)->getColonne() > iBaseCol))
  {
  	// Is it a new goal ?
    //
    if ((*iter)->getColonne() == (iBaseCol + 1))
    {
      string sNodeId = (*iter)->getNodeID() ;
      if ((string("") != sNodeId) && (INMEMORY_CHAR == sNodeId[0]))
        aLines.push_back((*iter)->getLigne()) ;
    }
    iter++ ;
  }

  // Nothing to do... leaving
  //
  if (aLines.empty())
    return true ;

  // Enregistrement du document modifié
  // Recording all this
  //
  Noyau.setPatPatho(&PptNoyau) ;
	Noyau.enregistrePatPatho() ;

	// Mise à jour de l'historique
	/* bool bReload = */ Noyau.chargePatPatho() ;
	Noyau.initFromPatPatho(&PptNoyau) ;
	_pDocHis->Rafraichir(&Docum, &PptNoyau) ;

	if (PptNoyau.empty())
	{
    string sErrorText = pContexte->getSuperviseur()->getText("goalsManagementErrors", "goalCouldNotBeRecorded") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
		return false ;
	}

  // Now, we have to update LdV for each added goal
  //
  for (vector<int>::iterator iterLines = aLines.begin() ; aLines.end() != iterLines ; iterLines++)
    UpdateLdvForNewGoal(iFrame, &PptNoyau, *iterLines, iBaseCol + 1) ;

  return true ;
}

bool
NSPatientChoisi::UpdateLdvForNewGoal(LDVFRAME iFrame, NSPatPathoArray *pFramePatPatho, int iGoalLine, int iGoalCol)
{
  if (NULL == pFramePatPatho)
    return false ;

  PatPathoIter itRoot = pFramePatPatho->ChercherItem(iGoalLine, iGoalCol) ;
	if ((NULL == itRoot) || (pFramePatPatho->end() == itRoot))
	  return false ;

  // Create a goal object from patpatho
  //
  NSLdvGoal Goal(pContexte, _pDocLdv, iFrame) ;
  PatPathoIter itPpt = itRoot ;
  Goal.initFromPatho(pFramePatPatho, &itPpt) ;

  // Open the goal
  //
  _pDocLdv->initGoal(&Goal) ;

#ifndef __EPIBRAIN__

  NSPredi* pPredi = pContexte->getPredi() ;
  if (pPredi && pPredi->_bSendPage)
  {
    // Get subPatho
    //
    NSPatPathoArray pptFille(pContexte->getSuperviseur()) ;
    pFramePatPatho->ExtrairePatPathoWithRoot(itRoot, &pptFille) ;

    pPredi->_sSendPage += string("<goal>\r\n") + pptFille.genereXML() + string("</goal>\r\n") ;
  }

#endif // !__EPIBRAIN__

  // Ask LdV to update its goals data
  //
  ArrayGoals* pGoals = _pDocLdv->getGoals(iFrame) ;
  if (pGoals)
    pGoals->loadGoals(itRoot, (*itRoot)->getColonne() - 1, pFramePatPatho, iFrame, true /*bJustOne*/) ;

  return true ;
}

//
// Obsolete... use pDocLdv->initGoal(pGoal) instead
//
void
NSPatientChoisi::RegleObjectif(NSLdvGoal* pGoal)
{
#ifndef __EPIPUMP__
    // Objectif qui ne s'ouvre jamais
    if ((pGoal->_sOpenEventNode == "") && (pGoal->_tDateOuverture.estVide()))
      return ;

    //
    // On regarde si les conditions d'ouverture sont réalisées
    //
    NVLdVTemps tNow ;
    tNow.takeTime() ;
    // Date de fermeture déjà atteinte : on sort
    if ((!(pGoal->_tDateFermeture.estVide())) && (pGoal->_tDateFermeture < tNow))
      return ;

    //
    // On regarde le document le plus récent du même type
    //
    string sNewConcept ;
    pContexte->getDico()->donneCodeSens(&(pGoal->getLexique()), &sNewConcept) ;

    NVLdVTemps  tDateDoc ;
    tDateDoc.init() ;

    string sDocId = "" ;

    NSPatPathoArray PatPathoDoc(pContexte->getSuperviseur()) ;
    DocumentIter itDoc = _pDocHis->DonnePatPathoDocument(sNewConcept, &PatPathoDoc) ;
    if ((itDoc != NULL) && (itDoc != _pDocHis->getVectDocument()->end()))
    {
      string sDate = (*itDoc)->getDateDoc() ;
      tDateDoc.initFromDate(sDate) ;

      PatPathoIter iter = PatPathoDoc.begin() ;
      sDocId = (*iter)->getNode() ;
    }

    //
    // Si l'objectif est ponctuel, on regarde si le document est dans les
    // dates prévues
    //
    if (!(pGoal->_iRythme == NSLdvGoal::cyclic))
    {
        // Si il existe une date de début autorisé
        if (pGoal->_sDateDebutAutorise != "")
        {
            NVLdVTemps tDateDebut ;
            tDateDebut.initFromDate(pGoal->_sDateDebutAutorise) ;
            // Si le document est antérieur à la date de début autorisé, on ne
            // doit pas en tenir compte
            if (tDateDoc < tDateDebut)
                return ;
            // Sinon, le document ferme l'objectif
            _pDocLdv->closeGoal(pGoal, &tNow, NULL) ;
            return ;
        }
        string sDateMin = "" ;
        if (pGoal->_sDateDebutConseille != "")
            sDateMin = pGoal->_sDateDebutConseille ;
        else
            if (pGoal->_sDateDebutIdeal != "" )
                sDateMin = pGoal->_sDateDebutIdeal ;
            else
                if (pGoal->_sDateDebutIdealMax != "" )
                    sDateMin = pGoal->_sDateDebutIdealMax ;
                else
                    if (pGoal->_sDateDebutConseilMax != "" )
                        sDateMin = pGoal->_sDateDebutConseilMax ;
                    else
                        if (pGoal->_sDateDebutCritique != "" )
                            sDateMin = pGoal->_sDateDebutCritique ;
        if (sDateMin == "")
            return ;

        // Sinon, le document ferme l'objectif
        NVLdVTemps tDateDebut ;
        tDateDebut.initFromDate(sDateMin) ;
        if (tDateDoc >= tDateDebut)
            _pDocLdv->closeGoal(pGoal, &tNow, NULL) ;
        return ;
    }
    //
    // ------------------------- Objectif cyclique ------------------------
    //

  // On cherche s'il existe un événement d'ouverture
  if (pGoal->_sOpenEventNode != "")
  {
    string sEventConcept ;
    pContexte->getDico()->donneCodeSens(&(pGoal->_sOpenEventNode), &sEventConcept) ;

    NSPatPathoArray PatPathoDoc2(pContexte->getSuperviseur()) ;
    itDoc = _pDocHis->DonnePatPathoDocument(sEventConcept, &PatPathoDoc2) ;
    if ((NULL != itDoc) && (_pDocHis->getVectDocument()->end() != itDoc))
    {
    	string sDate = (*itDoc)->getDateDoc() ;
      tDateDoc.initFromDate(sDate) ;
    }
    else
      return ;

    PatPathoIter iter = PatPathoDoc2.begin() ;
    string sOpener = (*iter)->getNode() ;
    NSLinkManager* pGraphe = _GraphPerson.getLinkManager() ;
    pGraphe->etablirLien(sOpener, NSRootLink::goalOpener, pGoal->getNode()) ;

    NVLdVTemps tDateOpener ;
    tDateOpener.init() ;
    string sDate = (*itDoc)->getDateDoc() ;
    tDateOpener.initFromDate(sDate) ;

    // Si l'événement d'ouverture est plus récent que le document, on sort
    if (tDateOpener > tDateDoc)
    	return ;
  }

	if (sDocId == "")
		return ;

	NSLinkManager* pGraphe = _GraphPerson.getLinkManager() ;

	pGraphe->etablirLien(sDocId, NSRootLink::goalReseter, pGoal->getNode()) ;

#endif // !__EPIPUMP__
}

bool
NSPatientChoisi::LocalPatho(NSPatPathoArray* pPptModele, NSPatPathoArray* pPptInstance)
{
  if ((NULL == pPptInstance) || (NULL == pPptModele) || (pPptModele->empty()))
    return false ;

  *pPptInstance = *pPptModele ;

  NVLdVTemps tpsNow ;
  tpsNow.takeTime() ;

  string sTypeDate = "" ;
  PatPathoIter itCpy = pPptInstance->begin() ;
  for ( ; pPptInstance->end() != itCpy ; itCpy++)
  {
    string sLexique = (*itCpy)->getLexique() ;
    if ((string("") != sLexique) && ('K' == sLexique[0]))
    {
      string sTypeBase = (*itCpy)->getLexique() ;
      pContexte->getDico()->donneCodeSens(&sTypeBase, &sTypeDate) ;
    }

    // Elément calculé : format "$KNAIS+AAMMJJ"
    //                           012345678901234

    if ((string("") != sLexique) && ('£' == sLexique[0]) &&
        (('D' == sLexique[1]) || ('T' == sLexique[1])) &&
        ((*itCpy)->getComplement() != string("")) && ('$' == (*itCpy)->getComplement()[0])
       )
    {
      if (strlen((*itCpy)->getComplement().c_str()) == 13)
      {
        string sUnitSens = (*itCpy)->getUnitSens() ;

        string sDateCalcul = (*itCpy)->getComplement() ;

        string sRefDate = string(sDateCalcul, 1, 5) ;
        string sOperat  = string(sDateCalcul, 6, 1) ;
        string sAjout   = string(sDateCalcul, 7, 6) ;

        string sActualReferenceDate = string("") ;

        if ("KNAIS" == sRefDate)
        {
          string sNaissance = donneNaissance() ;
          if (string("00000000") != sNaissance)
            sActualReferenceDate = sNaissance ;
        }
        else if ("KAUJO" == sRefDate)
        {
          NVLdVTemps TpsObj ;
          TpsObj.takeTime() ;
          sActualReferenceDate = TpsObj.donneDateHeure() ;
        }
        else
          sActualReferenceDate = getDateForEvent(sRefDate) ;

        if (string("") != sActualReferenceDate)
        {
          NVLdVTemps TpsObj ;
          TpsObj.initFromDate(sActualReferenceDate) ;

          string sAjoutAns  = string(sAjout, 0, 2) ;
          string sAjoutMois = string(sAjout, 2, 2) ;
          string sAjoutJour = string(sAjout, 4, 2) ;

          int    iAjoutAns  = atoi(sAjoutAns.c_str()) ;
          int    iAjoutMois = atoi(sAjoutMois.c_str()) ;
          int    iAjoutJour = atoi(sAjoutJour.c_str()) ;

          if (string("-") == sOperat)
          {
            iAjoutAns  = - iAjoutAns ;
            iAjoutMois = - iAjoutMois ;
            iAjoutJour = - iAjoutJour ;
          }

          if (0 != iAjoutAns)
            TpsObj.ajouteAns(iAjoutAns, false /* don't normalize */) ;
          if (0 != iAjoutMois)
            TpsObj.ajouteMois(iAjoutMois, false /* don't normalize */) ;
          if (0 != iAjoutJour)
            TpsObj.ajouteJours(iAjoutJour) ;

          // L'ouverture de l'objectif ne peut pas être
          // antérieure à sa date de création (aujourd'hui
          if ("KOUVR" == sTypeDate)
          {
            if (tpsNow > TpsObj)
              TpsObj = tpsNow ;
          }

          string sDateObj = string("") ;
          if (string("2DA01") == sUnitSens)
            sDateObj = TpsObj.donneDate() ;
          else
            sDateObj = TpsObj.donneDateHeure() ;

          (*itCpy)->setComplement(sDateObj) ;
        }
        else
          (*itCpy)->setComplement("") ;
      }
      else
        (*itCpy)->setComplement("") ;
    }
  }
  return true ;
}

string
NSPatientChoisi::getDateForEvent(string sEventCode)
{
  if (NULL == pContexte->getBBinterface())
    return string("") ;

  // pour l'instant le blackboard ne répond qu'à une question oui/non
  // L'alias doit etre un chemin de codes sens (séparateur : '/')
  //
  string sVarAlias = string("") ;
  pContexte->getDico()->donneCodeSens(&sEventCode, &sVarAlias) ;

  // on pose la question au blackboard
  // the referential will ignore it
  NSPatPathoArray* pPatPathoLocale = NULL ;
  string sAnswerDate ;
  int res = pContexte->getBBinterface()->getAnswer2Question(sVarAlias, "", &pPatPathoLocale, sAnswerDate, true, true) ;

  // Not found
  //
  if (1 != res)
  {
    if (pPatPathoLocale)
      delete pPatPathoLocale ;
    return string("") ;
  }

  // Concept found, but no value
  //
  if ((NSPatPathoArray*) NULL == pPatPathoLocale)
    return string("") ;

  if (pPatPathoLocale->empty())
  {
    delete pPatPathoLocale ;
    return string("") ;
  }

  // on teste d'abord l'existence de la variable
  // (la patpatho résultat est non vide ==> la variable existe)
  //
  PatPathoIter iter = pPatPathoLocale->begin() ;
  int iLigneBase = (*iter)->getLigne() ;
  string sUnit   = string("") ;
  string sFormat = string("") ;
  string sValeur = string("") ;

  while ((pPatPathoLocale->end() != iter) && ((*iter)->getLigne() == iLigneBase))
  {
    string sLexique = (*iter)->getLexique() ;
    if ((string("") != sLexique) && ('£' == sLexique[0]))
    {
      sFormat = (*iter)->getLexiqueSens() ;
      sValeur = (*iter)->getComplement() ;
      sUnit   = (*iter)->getUnitSens() ;

      if ((string("2DA01") == sUnit) || (string("2DA02") == sUnit))
        break ;
      else
        // Be certain not to return an incorrect value
        sValeur = string("") ;
    }
    iter++ ;
  }

  delete pPatPathoLocale ;

  return sValeur ;
}

bool
NSPatientChoisi::updateDemographicsFromCapture(NSCaptureArray* pCapt)
{
  NSSuper *pSuper = pContexte->getSuperviseur() ;

  string sMsg = string("Entering updateDemographicsFromCapture") ;
  pSuper->trace(&sMsg, 1, NSSuper::trDetails) ;

  if ((NSCaptureArray*) NULL == pCapt)
  {
    string sErreur = string("updateDemographicsFromCapture with a NULL NSCaptureArray pointer, leaving") ;
    pSuper->trace(&sErreur, 1, NSSuper::trError) ;
    return false ;
  }

  if (pCapt->empty())
  {
    string sErreur = string("updateDemographicsFromCapture with an empty NSCaptureArray, nothing to do, leaving") ;
    pSuper->trace(&sErreur, 1, NSSuper::trDetails) ;
    return true ;
  }

  NSPersonGraphManager* pPatientGraph = getGraphPerson() ;
  if ((NSPersonGraphManager*) NULL == pPatientGraph)
    return false ;

  // First, get demographics tree
  //
  NSDataGraph* pGraph = pPatientGraph->getDataGraph() ;
  if ((NSDataGraph*) NULL == pGraph)
    return false ;

  NSDataTreeIter iterTree ;
  if (false == pGraph->getTrees()->ExisteTree("ZADMI1", pSuper, &iterTree))
  {
    string sErreur = pSuper->getText("graphAndTreeErrors", "missingTreeInGraph") + string(" (") + string("ZADMI1") + string(")") ;
    pSuper->trace(&sErreur, 1, NSSuper::trError) ;
    erreur(sErreur.c_str(), standardError, 0) ;
    return false ;
  }

  string sTreeId = (*iterTree)->getTreeID() ;

  string sRights = string("") ;
  NSPatPathoArray demographicsPpt(pSuper, graphPerson) ;
  if (false == pGraph->getTree(sTreeId, &demographicsPpt, &sRights, true))
    return false ;

  NSPatPathoArray updatedPpt(pSuper, graphPerson) ;
  updatedPpt = demographicsPpt ;

  // Then update it
  //
  string sAdminArchetype = pSuper->getDemographicArchetypeId() ;
  if (string("") == sAdminArchetype)
  {
    string sError = pSuper->getText("archetypesManagement", "unknownDemographicArchetypeName") ;
    pSuper->trace(&sError, 1, NSSuper::trError) ;
    erreur(sError.c_str(), standardError, 0) ;
    return false ;
  }

  string sArchetypeFile = pSuper->getArcManager()->DonneFichierArchetypeParNom(NSArcManager::archetype, sAdminArchetype) ;
	if (string("") == sArchetypeFile)
	{
  	string sErrorText = pSuper->getText("archetypesManagement", "cannotFindThisArchetypeFile") ;
  	sErrorText += string(" (") + sAdminArchetype + string(")") ;
    pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError, 0) ;
		return false ;
	}

  nsarcParseur ArcParseur(pSuper) ;

  if (false == ArcParseur.open(sArchetypeFile))
	{
    string sErrorText = pSuper->getText("archetypesManagement", "archetypeParsingError") ;
  	sErrorText += string(" (") + sArchetypeFile + string(")") ;
    pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError, 0) ;
		return false ;
	}

  // Update pathpatho from new information
  //
  ArcParseur.updateArchetypedPpt(&updatedPpt, pCapt) ;

  string sIPP ;
  pContexte->getUtilisateur()->getAndRemoveIppFromPatpatho(&updatedPpt, &sIPP) ;

  if ((string("") != sIPP) && (string("") != pSuper->getIppSite()))
    updateIPPEnCours(pSuper->getIppSite(), sIPP) ;

  if (demographicsPpt.estEgal(&updatedPpt))
  {
    string sErreur = string("updateDemographicsFromCapture: nothing new from capture, leaving") ;
    pSuper->trace(&sErreur, 1, NSSuper::trDetails) ;
    return true ;
  }

  // Something changed, we must update demographics
  //

  // pContexte->getPatient()->CreeContribution() ;

  NSVectPatPathoArray VectUpdate ;
  string sCodeDocMeta = string("") ;
	VectUpdate.push_back(new NSPatPathoArray(updatedPpt)) ;

	pPatientGraph->updateTrees(pContexte, &VectUpdate, &sTreeId, &sCodeDocMeta, pidsPatient) ;

	if (string("") == sTreeId)
	{
  	string sErrorText = pContexte->getSuperviseur()->getText("documentManagementErrors", "failedToSave") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError, 0, 0) ;
		return false ;
	}

  string sErreur = string("updateDemographicsFromCapture: demographic information updated, leaving") ;
  pSuper->trace(&sErreur, 1, NSSuper::trDetails) ;

  return true ;
}

bool
NSPatientChoisi::updateVenuesFromCapture(NSCaptureArray* pCapt)
{
  NSSuper *pSuper = pContexte->getSuperviseur() ;

  string sMsg = string("Entering updateVenuesFromCapture") ;
  pSuper->trace(&sMsg, 1, NSSuper::trDetails) ;

  if ((NSCaptureArray*) NULL == pCapt)
  {
    string sErreur = string("updateVenuesFromCapture with a NULL NSCaptureArray pointer, leaving") ;
    pSuper->trace(&sErreur, 1, NSSuper::trError) ;
    return false ;
  }

  if (pCapt->empty())
  {
    string sErreur = string("updateVenuesFromCapture with an empty NSCaptureArray, nothing to do, leaving") ;
    pSuper->trace(&sErreur, 1, NSSuper::trDetails) ;
    return true ;
  }

  // Check that captured data contains venue information
  //
  string sVenueId = string("") ;
  pCapt->trouveChemin(string("LSEJO1/LNUSE1"), &sVenueId) ;

  if (string("") == sVenueId)
  {
    string sErreur = string("updateVenuesFromCapture: no venue ID found from captured data, nothing to do, leaving") ;
    pSuper->trace(&sErreur, 1, NSSuper::trDetails) ;
    return true ;
  }

  if ((NSVenuesDocument*) NULL == _pVenuesDoc)
  	_pVenuesDoc = new NSVenuesDocument((TDocument*) 0, pContexte) ;

  return _pVenuesDoc->updateVenuesFromCapture(pCapt) ;
}

bool
NSPatientChoisi::RefuseObjectif(NSPatPathoArray* pPatPatho, string sDocument, string sRefId, NSPatPathoArray* pPatPathoMotif, LDVFRAME iFrame)
{
try
{
#ifndef __EPIPUMP__

	if (((NSPatPathoArray*) NULL == pPatPatho) || (pPatPatho->empty()))
		return true ;

  DocumentIter iterDoc = getFrameIndex(iFrame) ;
  if (NULL == iterDoc)
    return false ;

	// Health index found
	NSDocumentInfo Docum(pContexte) ;

  NSDocumentData InfoData ;
  (*iterDoc)->initFromData(&InfoData) ;
  Docum.setData(&InfoData) ;

	NSNoyauDocument Noyau(0, &Docum, 0, pContexte, false, true) ;

  NSPatPathoArray PptNoyau(pContexte->getSuperviseur()) ;
  Noyau.initFromPatPatho(&PptNoyau) ;

	// On cherche le chapitre "objectifs de santé"
	PatPathoIter iter = PptNoyau.ChercherItem("0OBJE1") ;

	if ((NULL == iter) || (PptNoyau.end() == iter))
	{
		erreur("Erreur grave : le document Index de santé de ce patient ne possède pas de chapitre Objectifs de santé.", standardError, 0) ;
		return false ;
	}

	// Ajout d'un nouveau segment d'arbre
	// Adding a new tree branch
	// Ajout de la branche à l'arbre Index de santé
	// Adding the branch to the Health index tree

	// On cherche où il faut insérer la branche
	// Looking for the place to insert the branch
	int colonneMere = (*iter)->getColonne() ;
	iter++ ;
	while ((PptNoyau.end() != iter) && ((*iter)->getColonne() > colonneMere))
  {
    // on vérifie si un fils identique existe déjà
    if ((*iter)->getColonne() == (colonneMere + 1))
    {
      if ((*iter)->getLexique() == (*(pPatPatho->begin()))->getLexique())
      {
        string sLangue  = pContexte->getUserLanguage() ;
        string sCodeLex = (*iter)->getLexique() ;
        string sLibelle ;
        pContexte->getDico()->donneLibelle(sLangue, &sCodeLex, &sLibelle) ;

        /*
        char msg[255] ;
        sprintf(msg, "L'objectif \"%s\" existe déjà pour ce patient. Voulez-vous en insérer un autre ?", sLibelle.c_str()) ;
        int iRetVal = ::MessageBox(pContexte->GetMainWindow()->GetHandle(), msg, "Attention", MB_YESNO) ;
        if (iRetVal != IDYES)
          return true ;
        */
      }
    }

		iter++ ;
  }

  // On cherche un(les) élément calculé(s)
  bool bSeenClosingDate = false ;

  NVLdVTemps tpsNow ;
  tpsNow.takeTime() ;
  string sDateObj = tpsNow.donneDateHeure() ;

  NSPatPathoArray pptCopy(pContexte->getSuperviseur()) ;

  // Transformation de la patpatho théorique en patpatho réelle
  LocalPatho(pPatPatho, &pptCopy) ;

  PatPathoIter itCpy = pptCopy.begin() ;
  itCpy++ ;
  while ((pptCopy.end() != itCpy) && (((*itCpy)->getColonne() != 1) || ((*itCpy)->getLexique() != "KFERM1")))
    itCpy++ ;

	// On a trouvé une date de fin, on la modifie
	// We have found a previous ending date, we can change it
	if (itCpy != pptCopy.end())
	{
    bSeenClosingDate = true ;
    itCpy++ ;
		(*itCpy)->setComplement(sDateObj) ;
  }

  if (false == bSeenClosingDate)
  {
    itCpy = pptCopy.begin() ;
    itCpy++ ;
    pptCopy.ajoutePatho(itCpy, "KFERM1", 1, 0) ;
    Message theMessage ;
    theMessage.SetComplement(sDateObj) ;
    theMessage.SetUnit(string("2DA021")) ;
    itCpy++ ;
    pptCopy.ajoutePatho(itCpy, "£T0;19", &theMessage, 2, 0) ;
  }

  // Insertion du motif de refus
  pptCopy.InserePatPatho(pptCopy.end(), pPatPathoMotif, 1) ;

#ifndef __EPIBRAIN__
  NSPredi* pPredi = pContexte->getPredi() ;
  if (pPredi && pPredi->_bSendPage)
    pPredi->_sSendPage += string("<goal>\r\n") + pptCopy.genereXML() + string("</goal>\r\n") ;
#endif // !__EPIBRAIN__

  // on note ici la position en ligne et colonne de l'élément que l'on va insérer
  int ligneIns, colonneIns ;

  if (PptNoyau.end() != iter)
  {
    ligneIns = (*iter)->getLigne() ;
    colonneIns = colonneMere + 1 ;
  }
  else
  {
    PatPathoIter iterEnd = iter ;
    iterEnd-- ;
    ligneIns = (*iterEnd)->getLigne() + 1 ;
    colonneIns = colonneMere + 1 ;
  }

  // Insertion de l'objectif
  PptNoyau.InserePatPatho(iter, &pptCopy/*pPatPatho*/, colonneIns) ;

  // Enregistrement du document modifié
  Noyau.setPatPatho(&PptNoyau) ;
  Noyau.enregistrePatPatho() ;

  // Mise à jour de l'historique
  /* bool bReload = */ Noyau.chargePatPatho() ;
  Noyau.initFromPatPatho(&PptNoyau) ;
  _pDocHis->Rafraichir(&Docum, &PptNoyau) ;

  if (false == PptNoyau.empty())
  {
    PatPathoIter itRoot = PptNoyau.ChercherItem(ligneIns, colonneIns) ;
    string       sNoeud = (*itRoot)->getNode() ;

    // Lien entre le noeud et son document créateur
    NSLinkManager* pGraphe = _GraphPerson.getLinkManager() ;
    pGraphe->etablirLien(sDocument, NSRootLink::refCreator, sNoeud) ;

    // Lien entre le noeud et le référentiel
    if (string("") != sRefId)
    {
      string sRefNode = pContexte->getArcManager()->DonneNoeudArchetype(NSArcManager::referentiel, sRefId) ;
      if (string("") != sRefNode)
        pGraphe->etablirLien(sRefNode, NSRootLink::referentialOf, sNoeud) ;
    }

    // Lien entre le noeud et la préoccupation de santé qu'il concerne (plusieurs ?)
    VecteurString aVecteurString ;
		pGraphe->TousLesVraisDocument(sDocument, NSRootLink::problemRelatedTo, &aVecteurString) ;
    if (false == aVecteurString.empty())
    {
      string sPrimoPb = *(*(aVecteurString.begin())) ;
      pGraphe->etablirLien(sNoeud, NSRootLink::problemGoals, sPrimoPb) ;
    }

    // Prise en compte de l'objectif sur la Ligne de vie
    ArrayGoals* pGoals = _pDocLdv->getGoals(iFrame) ;
    if (pGoals)
      pGoals->loadGoals(itRoot, (*itRoot)->getColonne() - 1, &PptNoyau, iFrame, true /*bJustOne*/) ;
    _pDocLdv->invalidateViews("GOAL_REFUSE") ;
  }

#endif // !__EPIPUMP__
  return true ;
}
catch (...)
{
	erreur("Exception RefuseObjectif.", standardError, 0) ;
	return false ;
}
}


/**
 *  Add a new drug to the list
 */
bool
NSPatientChoisi::CreeTraitement(NSPatPathoArray *pPatPatho, VecteurString* pRelatedConcerns)
{
try
{
#ifndef __EPIPUMP__

	if (((NSPatPathoArray*) NULL == pPatPatho) || (pPatPatho->empty()))
  	return true ;

	// Recherche du document "Index de santé"
	// Looking for "Health index" document
  //
  NSDocumentHisto* pIndexDocHisto = _pDocLdv->getStorageDocument(ldvframeHealth, NSRootLink::drugOf) ;
	if ((NSDocumentHisto*) NULL == pIndexDocHisto)
		return false ;

	// Health index found
	NSDocumentInfo Docum(pContexte) ;

  NSDocumentData InfoData ;
  pIndexDocHisto->initFromData(&InfoData) ;
  Docum.setData(&InfoData) ;

	NSNoyauDocument Noyau(0, &Docum, 0, pContexte, false, true) ;

  NSPatPathoArray PptNoyau(pContexte->getSuperviseur()) ;
  Noyau.initFromPatPatho(&PptNoyau) ;

	// On cherche le chapitre "traitement"
	PatPathoIter iter = PptNoyau.ChercherItem("N00001") ;
	if ((NULL == iter) || (PptNoyau.end() == iter))
	{
  	// ajout des traitements dans l'index de Santé
    PptNoyau.ajoutePatho("N00001", 1) ;

    PatPathoIter iter = PptNoyau.ChercherItem("N00001") ;
    if ((iter == NULL) || (PptNoyau.end() == iter))
    {
    	erreur("Erreur grave : le document Index de santé de ce patient est endommagé.", standardError, 0) ;
      return false ;
    }
	}

	// Ajout d'un nouveau segment d'arbre
	// Adding a new tree branch
	// Ajout de la branche à l'arbre Index de santé
	// Adding the branch to the Health index tree

	// On cherche où il faut insérer la branche
	// Looking for the place to insert the branch
  // on se place ici après le chapître "traitement"
	int colonneMere = (*iter)->getColonne() ;
	iter++ ;
	while ((PptNoyau.end() != iter) && ((*iter)->getColonne() > colonneMere))
		iter++ ;

  	// on note ici la position en ligne et colonne de l'élément que l'on va insérer
	int ligneIns, colonneIns ;

	if (PptNoyau.end() != iter)
	{
  	ligneIns = (*iter)->getLigne() ;
    colonneIns = colonneMere + 1 ;
	}
  else
	{
  	PatPathoIter iterEnd = iter ;
    iterEnd-- ;
    ligneIns = (*iterEnd)->getLigne() + 1 ;
    colonneIns = colonneMere + 1 ;
  }

	// Insertion du médicament
  PptNoyau.InserePatPatho(iter, pPatPatho, colonneIns) ;

	// Enregistrement du document modifié
  Noyau.setPatPatho(&PptNoyau) ;
	Noyau.enregistrePatPatho() ;

	// Mise à jour de l'historique
	/* bool bReload = */ Noyau.chargePatPatho() ;
	Noyau.initFromPatPatho(&PptNoyau) ;
	_pDocHis->Rafraichir(&Docum, &PptNoyau) ;

	bool bRet = true ;
	NSLinkManager* pGraphe = _GraphPerson.getLinkManager() ;

	if (false == PptNoyau.empty())
	{
  	// on retrouve le numéro du node inséré
    PatPathoIter iterIns = PptNoyau.ChercherItem(ligneIns, colonneIns) ;
    if ((PptNoyau.end() != iterIns) && (NULL != iterIns))
    {
    	string sNodeALier = (*iterIns)->getNode() ;

      // on établit le lien entre le médicament et la préoccupation
      if (pRelatedConcerns && (false == pRelatedConcerns->empty()))
      	for (EquiItemIter it = pRelatedConcerns->begin() ; pRelatedConcerns->end() != it ; it++)
        	pGraphe->etablirLien(sNodeALier, NSRootLink::drugOf, **it) ;

      // Prise en compte du  sur la Ligne de vie
      _pDocLdv->getDrugs(ldvframeHealth)->loadDrugs(&PptNoyau, iterIns, (*iterIns)->getColonne() - 1, true /*bJustOne*/) ;

      // fabTODO showNewDrug
      _pDocLdv->showNewDrug(&PptNoyau, iterIns) ;

      _pDocLdv->invalidateViews("DRUG_NEW") ;
    }
    else
    {
    	// fabTODO erreur
    }
	}

	return bRet ;

#endif // !__EPIPUMP__
}
catch (...)
{
	erreur("Exception CreeTraitement.", standardError, 0) ;
	return false ;
}
}

bool
NSPatientChoisi::ArreterElement(VecteurString* pNodeArret, string sDateFin, bool bSave)
{
try
{
#ifndef __EPIPUMP__

  if (((VecteurString*) NULL == pNodeArret) || pNodeArret->empty())
    return false ;

  // temporairement on traite un seul noeud
	string sNodeArret = *(*(pNodeArret->begin())) ;

  // Finding in which frame to work
  //
  LDVFRAME iFrame = ldvframeNotSpecified ;
  NSLdvDrug* pDrug = _pDocLdv->getTreatment(sNodeArret, iFrame) ;
  if ((NSLdvDrug*) NULL == pDrug)
    return false ;

  iFrame = pDrug->getIFrame() ;

  NSFrameInformation* pFrameInfo = _pDocLdv->getFrames()->getFrameInformation(iFrame) ;
  if ((NSFrameInformation*) NULL == pFrameInfo)
    return false ;

  NSDocumentHisto* pDocument = pFrameInfo->findNode(sNodeArret) ;
  if ((NSDocumentHisto*) NULL == pDocument)
    return false ;

	// Health index found
	NSDocumentInfo Docum(pContexte) ;

  NSDocumentData InfoData ;
  pDocument->initFromData(&InfoData) ;
  Docum.setData(&InfoData) ;

	NSNoyauDocument Noyau(0, &Docum, 0, pContexte, false, true) ;

  NSPatPathoArray PptNoyau(pContexte->getSuperviseur()) ;
  Noyau.initFromPatPatho(&PptNoyau) ;
	if (PptNoyau.empty())
		return false ;	//	// Recherche du noeud	//  PatPathoIter pptIt = PptNoyau.ChercherNoeud(sNodeArret) ;  if (((PatPathoIter) NULL == pptIt) || (PptNoyau.end() == pptIt))  	return false ;	// Recherche de la date de fermeture en sous-niveau du noeud	//	PatPathoIter pptItValeur ;	bool bDateTrouvee = false ;	int iColBase = (*pptIt)->getColonne() ;	pptIt++ ;	while ((PptNoyau.end() != pptIt) && ((*pptIt)->getColonne() > iColBase))	{
		string sElemLex = (*pptIt)->getLexique() ;
		string sSens ;
		pContexte->getDico()->donneCodeSens(&sElemLex, &sSens) ;

		if ((*pptIt)->getColonne() == iColBase+1)
		{
    	// Dates
      if (sSens == "KFERM")
      {
      	pptIt++ ;
        int iLigneBase = (*pptIt)->getLigne() ;
        string sUnite  = string("") ;
        string sFormat = string("") ;
        string sValeur = string("") ;
        string sTemp   = string("") ;

        while ((pptIt != PptNoyau.end()) &&
                            ((*pptIt)->getLigne() == iLigneBase))
        {
          string sLexique = (*pptIt)->getLexique() ;
					if ((string("") != sLexique) && ('£' == sLexique[0]))
					{
						sTemp   = (*pptIt)->getLexique() ;
            pContexte->getDico()->donneCodeSens(&sTemp, &sFormat) ;
            sValeur = (*pptIt)->getComplement() ;
            sTemp   = (*pptIt)->getUnit() ;
            pContexte->getDico()->donneCodeSens(&sTemp, &sUnite) ;
            pptItValeur = pptIt;
            break ;
					}
					pptIt++;
				}

				// sFormat est du type £D0;03 ou £T0
				if ((sFormat != "") && ((sFormat[1] == 'D') || (sFormat[1] == 'T')) && (sValeur != ""))
				{
					if ((sUnite == "2DA01") || (sUnite == "2DA02"))
					{
						if (sValeur > sDateFin)
							(*pptItValeur)->setComplement(sDateFin) ;

						bDateTrouvee = true ;
					}
        }
			}
			else
				pptIt++ ;
		}
		else
    	pptIt++ ;
	}

	if (false == bDateTrouvee)
	{
		NSPatPathoArray PatPatho(pContexte->getSuperviseur()) ;

		// Date de fin - Date of ending
		if ((sDateFin != "") &&
				(sDateFin != string("19000000")) && (sDateFin != string("00000000")) && (sDateFin != string("00000000000000")))
		{
			PatPatho.ajoutePatho("KFERM1", 0) ;
			Message	CodeMsg ;
      CodeMsg.SetUnit("2DA021") ;
      CodeMsg.SetComplement(sDateFin) ;
      PatPatho.ajoutePatho("£T0;19", &CodeMsg, 1) ;

			PptNoyau.InserePatPatho(pptIt, &PatPatho, iColBase + 1) ;
		}
	}

	if (false == bSave)
		return true ;

	// Enregistrement du document modifié
	Noyau.setPatPatho(&PptNoyau) ;
  Noyau.enregistrePatPatho() ;

	// Mise à jour de l'historique
	/* bool bReload = */ Noyau.chargePatPatho() ;
	Noyau.initFromPatPatho(&PptNoyau) ;
	_pDocHis->Rafraichir(&Docum, &PptNoyau) ;

	if (false == PptNoyau.empty())
	{
    // on se repositionne sur le noeud à cause de la précédente insertion
    //
    pptIt = PptNoyau.ChercherNoeud(sNodeArret) ;
    //
    // Prise en compte du  sur la Ligne de vie
    //
    _pDocLdv->getDrugs(iFrame)->reloadDrugs(&PptNoyau, pptIt, (*pptIt)->getColonne() - 1, true /*bJustOne*/) ;
    _pDocLdv->invalidateViews("CONCERN_STOP") ;
	}

#endif // !__EPIPUMP__

	return true ;
}
catch (...)
{
	erreur("Exception ArreterElement.", standardError, 0) ;
	return false ;
}
}

bool
NSPatientChoisi::addSubElement(string motherNode, NSPatPathoArray *pPPT, bool bSave)
{
try
{
#ifndef __EPIPUMP__

	if (((NSPatPathoArray*) NULL == pPPT) || (string("") == motherNode))
		return false ;

  LDVFRAME iFrame = ldvframeNotSpecified ;
  NSLdvDrug* pDrug = _pDocLdv->getTreatment(motherNode, iFrame) ;
  if ((NSLdvDrug*) NULL == pDrug)
    return false ;

	DocumentIter iterDoc = getFrameIndex(iFrame) ;
	if ((DocumentIter) NULL == iterDoc)
		return false ;

	// Health index found
	NSDocumentInfo Docum(pContexte) ;

  NSDocumentData InfoData ;
  (*iterDoc)->initFromData(&InfoData) ;
  Docum.setData(&InfoData) ;

	NSNoyauDocument Noyau(0, &Docum, 0, pContexte, false, true) ;

  NSPatPathoArray PptNoyau(pContexte->getSuperviseur()) ;
  Noyau.initFromPatPatho(&PptNoyau) ;

  if (PptNoyau.empty())
  	return false ;  //  // Recherche du noeud  //  PatPathoIter pptIt = PptNoyau.ChercherNoeud(motherNode) ;  if (((PatPathoIter) NULL == pptIt) || (PptNoyau.end() == pptIt))  	return false ;	PptNoyau.InserePatPathoFille(pptIt, pPPT) ;

	if (false == bSave)
		return true ;

	// Enregistrement du document modifié
	Noyau.setPatPatho(&PptNoyau) ;
  Noyau.enregistrePatPatho() ;

	// Mise à jour de l'historique
	/* bool bReload = */ Noyau.chargePatPatho() ;
	Noyau.initFromPatPatho(&PptNoyau) ;
	_pDocHis->Rafraichir(&Docum, &PptNoyau) ;

	if (false == PptNoyau.empty())
	{
    // on se repositionne sur le noeud à cause de la précédente insertion
    //
    pptIt = PptNoyau.ChercherNoeud(motherNode) ;
    //
    // Prise en compte du  sur la Ligne de vie
    //
    _pDocLdv->getDrugs(iFrame)->reloadDrugs(&PptNoyau, pptIt, (*pptIt)->getColonne() - 1, true /*bJustOne*/) ;
    _pDocLdv->invalidateViews("CONCERN_STOP") ;
	}

#endif // !__EPIPUMP__

	return true ;
}
catch (...)
{
	erreur("Exception addSubElement.", standardError, 0) ;
	return false ;
}
}

bool
NSPatientChoisi::RenouvelerTraitement(VecteurString* pNodeMedic, NSPatPathoArray* pPPT)
{
try
{
#ifndef __EPIPUMP__

  if (((VecteurString*) NULL == pNodeMedic) || pNodeMedic->empty())
    return false ;

  // temporairement on traite un seul noeud
	string sNodeMedic = *(*(pNodeMedic->begin())) ;  LDVFRAME   iFrame = ldvframeNotSpecified ;  NSLdvDrug* pDrug  = _pDocLdv->getTreatment(sNodeMedic, iFrame) ;
  if ((NSLdvDrug*) NULL == pDrug)
    return false ;

	DocumentIter iterDoc = getFrameIndex(iFrame) ;
	if ((DocumentIter) NULL == iterDoc)
		return false ;
	if (pNodeMedic->empty())
  	return false ;

	// Health index found
	NSDocumentInfo Docum(pContexte) ;

  NSDocumentData InfoData ;
  (*iterDoc)->initFromData(&InfoData) ;
  Docum.setData(&InfoData) ;

	NSNoyauDocument Noyau(0, &Docum, 0, pContexte, false, true) ;

  NSPatPathoArray PptNoyau(pContexte->getSuperviseur()) ;
  Noyau.initFromPatPatho(&PptNoyau) ;

  if (PptNoyau.empty())
  	return false ;  //  // Recherche du noeud  //  PatPathoIter pptIt = PptNoyau.ChercherNoeud(sNodeMedic) ;  if (((PatPathoIter) NULL == pptIt) || (PptNoyau.end() == pptIt))  	return false ;	// Recherche de la dernière phase en sous-niveau du noeud  //  PatPathoIter pptItValeur ;  bool bEnterPhase = false ;  int iColBase = (*pptIt)->getColonne() ;  int iColPhase ;  pptIt++ ;  while ((PptNoyau.end() != pptIt) && ((*pptIt)->getColonne() > iColBase))  {
  	string sElemLex = (*pptIt)->getLexique() ;
    string sSens ;
    pContexte->getDico()->donneCodeSens(&sElemLex, &sSens) ;

    if ((*pptIt)->getColonne() == iColBase+1)
    {
    	// Dates
      if (string("KPHAT") == sSens)
      {
      	if (false == bEnterPhase)
        	bEnterPhase = true ;

        iColPhase = (*pptIt)->getColonne() ;
        pptIt++ ;

        // on "skippe" la phase
        while ((PptNoyau.end() != pptIt) && ((*pptIt)->getColonne() > iColPhase))
        	pptIt++ ;
      }
      else if (bEnterPhase)
      {
      	// on suppose que s'il y a plusieurs phases, elles se succèdent
        // on est donc ici après la dernière phase.
        break ;
      }
      else
      	pptIt++ ;
    }
    else
    	pptIt++ ;
  }

  pptItValeur = pptIt ;

  if (bEnterPhase)
  	PptNoyau.InserePatPatho(pptItValeur, pPPT, iColPhase + 1) ;
  else
  {
  	string sErrorText = pContexte->getSuperviseur()->getText("drugManagementErrors", "noTreatmentPhaseDefined") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  	erreur(sErrorText.c_str(), standardError, 0) ;
    return false ;
  }

	// Enregistrement du document modifié
	Noyau.setPatPatho(&PptNoyau) ;
  Noyau.enregistrePatPatho() ;

	// Mise à jour de l'historique
	/* bool bReload = */ Noyau.chargePatPatho() ;
	Noyau.initFromPatPatho(&PptNoyau) ;
	_pDocHis->Rafraichir(&Docum, &PptNoyau) ;

  if (false == PptNoyau.empty())
  {
  	// on se repositionne sur le noeud à cause de la précédente insertion
    //
    pptIt = PptNoyau.ChercherNoeud(sNodeMedic) ;
    //
    // Prise en compte du  sur la Ligne de vie
    //
    _pDocLdv->getDrugs(iFrame)->reloadDrugs(&PptNoyau, pptIt, (*pptIt)->getColonne() - 1, true /*bJustOne*/) ;
    _pDocLdv->invalidateViews("DRUG_RENEW") ;
  }

#endif // !__EPIPUMP__

	return true ;
}
catch (...)
{
	erreur("Exception RenouvelerTraitement.", standardError, 0) ;
	return false ;
}
}

bool
NSPatientChoisi::SupprimerElement(string sNodeArret, bool bAvecTransa)
{
try
{
	// Recherche du document
	// Looking for document
  //
  NSFrameInformation* pFrameInfo = _pDocLdv->getFrameForNode(sNodeArret) ;
  if (NULL == pFrameInfo)
    return false ;

  NSDocumentHisto* pDocHisto = pFrameInfo->findNode(sNodeArret) ;
  if (NULL == pDocHisto)
    return false ;

	// Health index found
	NSDocumentInfo Docum(pContexte) ;

  NSDocumentData InfoData ;
  pDocHisto->initFromData(&InfoData) ;
  Docum.setData(&InfoData) ;

	NSNoyauDocument Noyau(0, &Docum, 0, pContexte, false, true) ;

  NSPatPathoArray PptNoyau(pContexte->getSuperviseur()) ;
  Noyau.initFromPatPatho(&PptNoyau) ;

  if (PptNoyau.empty())
  	return false ;  //  // Recherche du noeud  //  PatPathoIter pptIt = PptNoyau.ChercherNoeud(sNodeArret) ;  if ((NULL == pptIt) || (PptNoyau.end() == pptIt))  	return false ;  // Suppression du médicament (sous patpatho qui commence à pptIt)  PptNoyau.SupprimerItem(pptIt) ;  // Enregistrement du document modifié	Noyau.setPatPatho(&PptNoyau) ;
  Noyau.enregistrePatPatho() ;

	// Mise à jour de l'historique
	/* bool bReload = */ Noyau.chargePatPatho() ;
	Noyau.initFromPatPatho(&PptNoyau) ;
	_pDocHis->Rafraichir(&Docum, &PptNoyau) ;

	return true ;
}
catch (...)
{
	erreur("Exception SupprimerElement.", standardError, 0) ;
	return false ;
}
}

/**
 *  Replace Lexicon code for a given node
 */
bool
NSPatientChoisi::SwitchElementLexicon(ArrayLdvDrugs* pModifiedDrugs)
{
  if (((ArrayLdvDrugs*) NULL == pModifiedDrugs) || pModifiedDrugs->empty())
    return false ;

  drugsIter it = pModifiedDrugs->begin() ;

  string sNodeId = (*it)->getNoeud() ;

try
{
	// Recherche du document
	// Looking for document
  //
  NSFrameInformation* pFrameInfo = _pDocLdv->getFrameForNode(sNodeId) ;
  if ((NSFrameInformation*) NULL == pFrameInfo)
    return false ;

  NSDocumentHisto* pDocHisto = pFrameInfo->findNode(sNodeId) ;
  if ((NSDocumentHisto*) NULL == pDocHisto)
    return false ;

	// Health index found
	NSDocumentInfo Docum(pContexte) ;

  NSDocumentData InfoData ;
  pDocHisto->initFromData(&InfoData) ;
  Docum.setData(&InfoData) ;

	NSNoyauDocument Noyau(0, &Docum, 0, pContexte, false, true) ;

  NSPatPathoArray PptNoyau(pContexte->getSuperviseur()) ;
  Noyau.initFromPatPatho(&PptNoyau) ;

  if (PptNoyau.empty())
  	return false ;  for ( ; pModifiedDrugs->end() != it ; it++)  {    //    // Finding the node to be modified and change its lexicon code    //    PatPathoIter pptIt = PptNoyau.ChercherNoeud((*it)->getNoeud()) ;    if (pptIt && (PptNoyau.end() != pptIt))      (*pptIt)->setLexique((*it)->getLexique()) ;  }  // Enregistrement du document modifié	Noyau.setPatPatho(&PptNoyau) ;
  Noyau.enregistrePatPatho() ;

	// Mise à jour de l'historique
	/* bool bReload = */ Noyau.chargePatPatho() ;
	Noyau.initFromPatPatho(&PptNoyau) ;
	_pDocHis->Rafraichir(&Docum, &PptNoyau) ;

	return true ;
}
catch (...)
{
	erreur("Exception SupprimerElement.", standardError, 0) ;
	return false ;
}
}

// Create a prescription document
//
bool
NSPatientChoisi::CreeOrdonnance(bool bGereALD, bool bForceSecured)
{
try
{
#ifndef __EPIPUMP__

	if (!(pContexte->userHasPrivilege(NSContexte::createDocument, -1, -1, string(""), string(""), NULL, NULL)))
	{
		string sErrorText = pContexte->getSuperviseur()->getText("privilegeManagement", "insufficientRights") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), warningError, 0) ;
		return false ;
	}

  // Getting Frame information for Health
  //
  NSFrameInformationArray *pFrames = _pDocLdv->getFrames() ;
  if (NULL == pFrames)
    return false ;

  NSFrameInformation* pFrameInformation = pFrames->getFrameInformation(ldvframeHealth) ;
  if (NULL == pFrameInformation)
    return false ;

	VecteurString NewTraitement ;
	VecteurString NewLexique ;
	VecteurString NewTrtALD ;
	VecteurString NewLexALD ;

  // Scan all documents from frame
  //
  NSDocumentHisto* pFrameIndexDoc = pFrameInformation->getIndexDocument() ;
  if (pFrameIndexDoc)
  {
    NSPatPathoArray PathoIndex(pContexte->getSuperviseur()) ;
    pFrameIndexDoc->initFromPatPatho(&PathoIndex) ;

    if (false == PathoIndex.empty())
    {
      // Initialize the list of nodes and Lexion codes for non ALD and ALD drugs 
      //
      initNewPrescriptionArrays(&PathoIndex, bGereALD, &NewTraitement, &NewLexique, &NewTrtALD, &NewLexALD) ;
      initRenewPrescriptionArrays(&PathoIndex, bGereALD, &NewTraitement, &NewLexique, &NewTrtALD, &NewLexALD) ;
    }
  }

  NSDocumentHistoArray* pTreatmentsDocs = pFrameInformation->getTreatmentDocs() ;
  if (pTreatmentsDocs && (false == pTreatmentsDocs->empty()))
  {
    for (DocumentIter i = pTreatmentsDocs->begin() ; pTreatmentsDocs->end() != i ; i++)
      if (*i)
      {
        NSPatPathoArray PathoDrug(pContexte->getSuperviseur()) ;
        (*i)->initFromPatPatho(&PathoDrug) ;

        if (false == PathoDrug.empty())
        {
          initNewPrescriptionArrays(&PathoDrug, bGereALD, &NewTraitement, &NewLexique, &NewTrtALD, &NewLexALD) ;
          initRenewPrescriptionArrays(&PathoDrug, bGereALD, &NewTraitement, &NewLexique, &NewTrtALD, &NewLexALD) ;
        }
      }
  }

	// Le traitement est constitué
	if ((NewTraitement.empty()) && (NewTrtALD.empty()))
		return false ;

	// constitution de la patpatho correspondant au nouveau traitement
	NSPatPathoArray PatPatho(pContexte->getSuperviseur()) ;

  bool bSecuredPrescription = false ;

  if (false == buildPrescriptionPatpatho(pFrameInformation, &PatPatho, bGereALD, &NewTraitement, &NewLexique, &NewTrtALD, &NewLexALD, false, bSecuredPrescription))
		return false ;

  if (PatPatho.empty() == true)
		return false ;

  PatPatho.clearDocAndNodeIDs() ;

	// enregistrement du document
	NSCRDocument CRDoc(0, pContexte) ;
	CRDoc.setPatPatho(&PatPatho) ;

	bool existeInfo = CRDoc.Referencer("ZCN00", "Ordonnance", "", "", true, false) ;

	if (existeInfo)
		existeInfo = CRDoc.enregistrePatPatho() ;

	if (existeInfo)
	{
    NSLinkManager* pGraphe = _GraphPerson.getLinkManager() ;
  	//
    // Rafraichir l'historique
    //
    /* bool bReload = */ CRDoc.NSNoyauDocument::chargePatPatho() ;
    CRDoc.initFromPatPatho(&PatPatho) ;

    _pDocHis->Rafraichir(CRDoc._pDocInfo, &PatPatho) ;

    CRDoc.initFromPatPatho(&PatPatho) ;
    PatPathoIter iter ;
    string sNodeALier ;

    EquiItemIter i = NewLexique.begin() ;
    EquiItemIter j = NewTraitement.begin() ;
    // on a maintenant les numeros de noeuds, on peut établir les liens restants
    while ((NewLexique.end() != i) && (NewTraitement.end() != j))
    {
    	iter = PatPatho.ChercherItem(*(*i)) ;
      if ((iter != NULL) && (iter != PatPatho.end()))
      {
      	sNodeALier = (*iter)->getNode() ;
        /* bool bRes = */ pGraphe->etablirLien(*(*j), NSRootLink::treatmentOf, sNodeALier);
      }

      i++ ;
      j++ ;
    }
    if (bGereALD)
    {
    	EquiItemIter _iALD = NewLexALD.begin() ;
      EquiItemIter _jALD = NewTrtALD.begin() ;
      // on a maintenant les numeros de noeuds, on peut établir les liens restants
      while ((NewLexALD.end() != _iALD) && (NewTrtALD.end() != _jALD))
      {
      	iter = PatPatho.ChercherItem(*(*_iALD)) ;
        if ((NULL != iter) && (PatPatho.end() != iter))
        {
        	sNodeALier = (*iter)->getNode() ;
          /* bool bRes = */ pGraphe->etablirLien(*(*_jALD), NSRootLink::treatmentOf, sNodeALier);
        }

        _iALD++ ;
        _jALD++ ;
      }
    }
	}

	if (existeInfo)
		existeInfo = CRDoc.enregistrePatPatho() ;

	// on ouvre l'ordonnance en édition
	_pDocHis->AutoriserEdition(CRDoc._pDocInfo) ;

#endif // !__EPIPUMP__
	return true;
}
catch (...)
{
	erreur("Exception CreeOrdonnance.", standardError, 0) ;
	return false ;
}
}

bool
NSPatientChoisi::CreeOrdonnanceFromSelection(bool bGereALD, VecteurString* pDrugsRefs, bool bDCI, bool bForceSecured)
{
try
{
#ifndef __EPIPUMP__

	if (((VecteurString*) NULL == pDrugsRefs) || pDrugsRefs->empty())
		return false ;

	if (false == pContexte->userHasPrivilege(NSContexte::createDocument, -1, -1, string(""), string(""), NULL, NULL))
	{
		string sErrorText = pContexte->getSuperviseur()->getText("privilegeManagement", "insufficientRights") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), warningError, 0) ;
		return false ;
	}

  // Getting Frame information for Health
  //
  NSFrameInformationArray *pFrames = _pDocLdv->getFrames() ;
  if ((NSFrameInformationArray*) NULL == pFrames)
    return false ;

  NSFrameInformation* pFrameInformation = pFrames->getFrameInformation(ldvframeHealth) ;
  if ((NSFrameInformation*) NULL == pFrameInformation)
    return false ;

	VecteurString NewTraitement ;
	VecteurString NewLexique ;

	VecteurString NewTrtALD ;
	VecteurString NewLexALD ;

  // Browse drugs' nodes to sort ALD and non ALD drugs
  //
	EquiItemIter itemsIter = pDrugsRefs->begin() ;
	for ( ; pDrugsRefs->end() != itemsIter ; itemsIter++)
	{
  	string sNodeMedic = **itemsIter ;

    // Get the node in its tree
    //
    NSDocumentHisto* pFoundDoc = pFrameInformation->findNode(sNodeMedic) ;
    if (pFoundDoc)
    {
      // Get the project tree that contains this node
      //
      NSPatPathoArray PathoIndex(pContexte->getSuperviseur()) ;
      pFoundDoc->initFromPatPatho(&PathoIndex) ;

      if (false == PathoIndex.empty())
      {
        // Find the node in the project tree
        //
        PatPathoIter pptIt = PathoIndex.ChercherNoeud(sNodeMedic) ;
        if (((PatPathoIter) NULL != pptIt) && (PathoIndex.end() != pptIt))
        {
          string sCodeMedic = (*pptIt)->getLexique() ;

          if (bGereALD)
          {
            if (_pDocLdv && _pDocLdv->isALDDrug(sNodeMedic))
            {
              NewTrtALD.AddString(sNodeMedic) ;
              NewLexALD.AddString(sCodeMedic) ;
            }
            else
            {
              NewTraitement.AddString(sNodeMedic) ;
              NewLexique.AddString(sCodeMedic) ;
            }
          }
          else
          {
            NewTraitement.AddString(sNodeMedic) ;
            NewLexique.AddString(sCodeMedic) ;
          }
        }
      }
    }
  }

	// Le traitement est constitué
	if ((NewTraitement.empty()) && (NewTrtALD.empty()))
		return false ;

	// constitution de la patpatho correspondant au nouveau traitement
	NSPatPathoArray PatPatho(pContexte->getSuperviseur()) ;

  bool bSecuredPrescription = false ;

  if (false == buildPrescriptionPatpatho(pFrameInformation, &PatPatho, bGereALD, &NewTraitement, &NewLexique, &NewTrtALD, &NewLexALD, bDCI, bSecuredPrescription))
		return false ;

  if (PatPatho.empty() == true)
		return false ;

	PatPatho.clearDocAndNodeIDs() ;

	// enregistrement du document
	NSCRDocument CRDoc(0, pContexte) ;
	CRDoc.setPatPatho(&PatPatho) ;

  // Add BdM version
  //
  NSPatPathoArray PatPathoSpecificInfo(pContexte->getSuperviseur()) ;
  NSBdmDriver* pDriver = pContexte->getBdmDriver() ;
  if (pDriver)
  {
    string sBamVersion = pDriver->getBamVersionId() ;

    if (string("") != sBamVersion)
    {
      PatPathoSpecificInfo.ajoutePatho("0BDCM1", 0) ;

      Message Msg ;
      Msg.SetTexteLibre(sBamVersion) ;
      PatPathoSpecificInfo.ajoutePatho(string("£?????"), &Msg, 1) ;
    }
  }

	bool existeInfo = false ;

  if (PatPathoSpecificInfo.empty())
    existeInfo = CRDoc.Referencer(string("ZCN00"), string("Ordonnance"), string(""), string(""), true, false) ;
  else
    existeInfo = CRDoc.Referencer(string("ZCN00"), string("Ordonnance"), string(""),
                     string(""), true, false, string(""),
                     NSRootLink::personDocument, (NSPersonGraphManager*) 0,
                     string("_User_"), string(""), string(""), string(""),
                     string(""), string(""), NSRootLink::viewOfDocument,
                     &PatPathoSpecificInfo) ;

	if (existeInfo)
		existeInfo = CRDoc.enregistrePatPatho() ;

	if (existeInfo)
	{
    NSLinkManager* pGraphe = _GraphPerson.getLinkManager() ;
  	//
    // Rafraichir l'historique
    //
    /* bool bReload = */ CRDoc.NSNoyauDocument::chargePatPatho() ;
    CRDoc.initFromPatPatho(&PatPatho) ;

    _pDocHis->Rafraichir(CRDoc._pDocInfo, &PatPatho) ;

    CRDoc.initFromPatPatho(&PatPatho) ;
    PatPathoIter iter ;
    string sNodeALier ;

    EquiItemIter i = NewLexique.begin() ;
    EquiItemIter j = NewTraitement.begin() ;
    // on a maintenant les numeros de noeuds, on peut établir les liens restants
    while ((NewLexique.end() != i) && (NewTraitement.end() != j))
    {
    	iter = PatPatho.ChercherItem(*(*i)) ;
      if ((NULL != iter) && (PatPatho.end() != iter))
      {
      	sNodeALier = (*iter)->getNode() ;
        /* bool bRes = */ pGraphe->etablirLien(*(*j), NSRootLink::treatmentOf, sNodeALier) ;
      }

      i++ ;
      j++ ;
    }
    if (bGereALD)
    {
    	EquiItemIter _iALD = NewLexALD.begin() ;
      EquiItemIter _jALD = NewTrtALD.begin() ;
      // on a maintenant les numeros de noeuds, on peut établir les liens restants
      while ((_iALD != NewLexALD.end()) && (_jALD != NewTrtALD.end()))
      {
      	iter = PatPatho.ChercherItem(*(*_iALD)) ;
        if ((iter != NULL) && (iter != PatPatho.end()))
        {
        	sNodeALier = (*iter)->getNode() ;
          /* bool bRes = */ pGraphe->etablirLien(*(*_jALD), NSRootLink::treatmentOf, sNodeALier) ;
        }

        _iALD++ ;
        _jALD++ ;
      }
    }
	}

	if (existeInfo)
		existeInfo = CRDoc.enregistrePatPatho() ;

	// on ouvre l'ordonnance en édition
	_pDocHis->AutoriserEdition(CRDoc._pDocInfo) ;

#endif // !__EPIPUMP__
	return true;
}
catch (...)
{
	erreur("Exception CreeOrdonnanceFromSelection.", standardError, 0) ;
	return false ;
}
}

bool
NSPatientChoisi::initNewPrescriptionArrays(NSPatPathoArray* pPpt, bool bGereALD, VecteurString* pNewTraitement, VecteurString* pNewLexique, VecteurString* pNewTrtALD, VecteurString* pNewLexALD)
{
	if (((NSPatPathoArray*) NULL == pPpt) || (true == pPpt->empty()))
		return false ;

	PatPathoIter pptIt = pPpt->ChercherItem("N00001") ;
	if (((PatPathoIter) NULL == pptIt) || (pPpt->end() == pptIt))
		return false ;   // cas aucun traitement déjà effectué

	NSLinkManager* pGraphe = _GraphPerson.getLinkManager() ;

	// récupération de la date du jour au format AAAAMMJJ
  //
	NVLdVTemps tpsNow ;
  tpsNow.takeTime() ;

	VecteurString OldTraitement ;

	int iColBase = (*pptIt)->getColonne() ;
	pptIt++ ;	while ((pPpt->end() != pptIt) && ((*pptIt)->getColonne() > iColBase))	{
		if ((*pptIt)->getColonne() == iColBase+1)
		{
  		// on est sur un noeud médicament
      string sNodeMedic = (*pptIt)->getNode() ;
      string sCodeMedic = (*pptIt)->getLexique() ;

      // initialisation des données à récupérer pour ce médicament
      string sUnite1  = "" ;
      string sFormat1 = "" ;
      string sValeur1 = "" ;
      string sTemp1   = "" ;
      string sUnite2  = "" ;
      string sFormat2 = "" ;
      string sValeur2 = "" ;
      string sTemp2   = "" ;
      bool   bAvecFermeture = false ;
      bool   bAvecOuverture = false ;

      // On regarde si le médicament est déjà prescrit dans un ancien traitement (ou ordonnance)
      // Si c'est le cas, on ne doit pas le reproposer.
      OldTraitement.vider() ;
      pGraphe->TousLesVrais(sNodeMedic, NSRootLink::treatmentOf, &OldTraitement) ;

      if (OldTraitement.empty())
      {
      	pptIt++ ;
        while ((pptIt != pPpt->end()) && ((*pptIt)->getColonne() > iColBase + 1))        {
        	string sSens = (*pptIt)->getLexiqueSens() ;

          if ((*pptIt)->getColonne() == iColBase + 2)
          {
          	// Dates
            if (sSens == "KOUVR")
            {
            	pptIt++;
              int iLigneBase = (*pptIt)->getLigne();
              bAvecOuverture = true;

              while ((pPpt->end() != pptIt) &&
                                ((*pptIt)->getLigne() == iLigneBase))
              {
                string sLexique = (*pptIt)->getLexique() ;
								if ((string("") != sLexique) && ('£' == sLexique[0]))
                {
                	sTemp1   = (*pptIt)->getLexique() ;
                  pContexte->getDico()->donneCodeSens(&sTemp1, &sFormat1) ;
                  sValeur1 = (*pptIt)->getComplement() ;
                  sTemp1   = (*pptIt)->getUnit() ;
                  pContexte->getDico()->donneCodeSens(&sTemp1, &sUnite1) ;
                  break ;
                }
								pptIt++;
							}
						}
            else if (sSens == "KFERM")
            {
            	pptIt++ ;
              int iLigneBase = (*pptIt)->getLigne() ;
              bAvecFermeture = true ;

              while ((pptIt != pPpt->end()) &&
                                ((*pptIt)->getLigne() == iLigneBase))
              {
                string sLexique = (*pptIt)->getLexique() ;
								if ((string("") != sLexique) && ('£' == sLexique[0]))
                {
                	sTemp2   = (*pptIt)->getLexique() ;
                  pContexte->getDico()->donneCodeSens(&sTemp2, &sFormat2) ;
                  sValeur2 = (*pptIt)->getComplement() ;
                  sTemp2   = (*pptIt)->getUnit() ;
                  pContexte->getDico()->donneCodeSens(&sTemp2, &sUnite2) ;
                  break ;
                }
								pptIt++ ;
							}
						}
            else
            	pptIt++ ;
					}
          else
          	pptIt++ ;
				} // fin du while sur les dates
			}
      else  // if (pOldTraitement->empty())
      	pptIt++;

      // On regarde ici si le médicament peut appartenir au nouveau traitement
      // vérification des dates d'ouverture et de fermeture
      // vérification des formats (sFormat est du type £D0;03)
      if ((bAvecOuverture) && (string("") != sFormat1) && (('D' == sFormat1[1]) || ('T' == sFormat1[1])) && (string("") != sValeur1))
      {
      	if ((string("2DA01") == sUnite1) || (string("2DA02") == sUnite1))
        {
        	// Si le date d'ouverture est >= dateJour - 1 :
          // on retient le noeud pour ajouter au nouveau traitement
          incremente_date(sValeur1);

          if (sValeur1 >= tpsNow.donneDateHeure())
          {
          	// Si le médicament n'a pas de date de fermeture (cas chronique)
            // ou si la date de fermeture est future
            if ((false == bAvecFermeture) ||
                        		  ((sFormat2 != "") && ((sFormat2[1] == 'D') || (sFormat2[1] == 'T')) &&
                               (sValeur2 != "") && ((sUnite2 == "2DA01") ||
                               											(sUnite2 == "2DA02")) &&
                               (sValeur2 > tpsNow.donneDateHeure())))
            {
              if (bGereALD)
              {
              	if (_pDocLdv && _pDocLdv->isALDDrug(sNodeMedic))
                {
                	pNewTrtALD->AddString(sNodeMedic) ;
                  pNewLexALD->AddString(sCodeMedic) ;
                }
                else
                {
                	pNewTraitement->AddString(sNodeMedic) ;
                  pNewLexique->AddString(sCodeMedic) ;
                }
              }
              else
              {
              	pNewTraitement->AddString(sNodeMedic) ;
                pNewLexique->AddString(sCodeMedic) ;
              }
            }
          }
        }
      }
		}
    else
    	pptIt++ ;
	} // fin du while sur les médicaments

  return true ;
}

bool
NSPatientChoisi::initRenewPrescriptionArrays(NSPatPathoArray* pPpt, bool bGereALD, VecteurString* pNewTraitement, VecteurString* pNewLexique, VecteurString* pNewTrtALD, VecteurString* pNewLexALD)
{
	if ((NULL == pPpt) || (true == pPpt->empty()))
		return false ;

	PatPathoIter pptIt = pPpt->ChercherItem("N00001") ;

  if ((pptIt == NULL) || (pptIt == pPpt->end()))
  	return false ;   // cas aucun traitement déjà effectué

  string sNodeRenouv, sCodeMedic, sNodeMedic ;

  NSLinkManager* pGraphe = _GraphPerson.getLinkManager() ;

	// récupération de la date du jour au format AAAAMMJJ
  //
	NVLdVTemps tpsNow ;
  tpsNow.takeTime() ;

	VecteurString OldTraitement ;

  int iColBase = (*pptIt)->getColonne() ;
  pptIt++ ;

  while (pPpt->end() != pptIt)
  {
  	pptIt = pPpt->ChercherItem("GRENT", true, pptIt) ;

    if ((NULL != pptIt) && (pPpt->end() != pptIt))
    {
    	sNodeRenouv = (*pptIt)->getNode() ;

      // On regarde si le renouvelement est déjà prescrit dans un ancien traitement (ou ordonnance)
      // Si c'est le cas, on ne doit pas le reproposer.
      OldTraitement.vider() ;
      pGraphe->TousLesVrais(sNodeRenouv, NSRootLink::treatmentOf, &OldTraitement) ;

      if (OldTraitement.empty())
      {
      	// on remonte en arrière jusqu'au noeud médicament
        PatPathoIter pptItMedic = pptIt ;

        while ((*pptItMedic)->getColonne() > iColBase+1)
        	pptItMedic-- ;

        if ((*pptItMedic)->getColonne() == iColBase+1)
        {
        	sCodeMedic = (*pptItMedic)->getLexique() ;
          sNodeMedic = (*pptItMedic)->getNode() ;
        }
        else
        {
        	sCodeMedic = "" ; // cas d'erreur : il n'existe pas de médicament
          sNodeMedic = "" ;
        }

        if (sCodeMedic != "")
        {
        	// initialisation des données à récupérer pour ce médicament
      		string sUnite1  = "" ;
      		string sFormat1 = "" ;
      		string sValeur1 = "" ;
      		string sTemp1   = "" ;

        	PatPathoIter pptItDate = pptIt ;
          int iColRenouv = (*pptItDate)->getColonne() ;
          pptItDate++ ;
          bool bAvecOuverture = false ;

          while ((pptItDate != pPpt->end()) && ((*pptItDate)->getColonne() > iColRenouv))
          {
          	string sSens = (*pptItDate)->getLexiqueSens() ;

            if ((*pptItDate)->getColonne() == iColRenouv + 1)
            {
            	// Dates
              if (sSens == "KOUVR")
              {
              	pptItDate++ ;
                int iLigneBase = (*pptItDate)->getLigne() ;
                bAvecOuverture = true ;

                while ((pptItDate != pPpt->end()) &&
                       ((*pptItDate)->getLigne() == iLigneBase))
                {
                  string sLexique = (*pptItDate)->getLexique() ;
                	if ((string("") != sLexique) && ('£' == sLexique[0]))
                  {
                  	sTemp1   = (*pptItDate)->getLexique() ;
                    pContexte->getDico()->donneCodeSens(&sTemp1, &sFormat1) ;
                    sValeur1 = (*pptItDate)->getComplement() ;
                    sTemp1   = (*pptItDate)->getUnit() ;
                    pContexte->getDico()->donneCodeSens(&sTemp1, &sUnite1) ;
                    break ;
                  }
                  pptItDate++ ;
                }
              }
              else
              	pptItDate++ ;
            }
            else
            	pptItDate++ ;
          } // fin du while sur les dates

          // On regarde ici si le médicament peut appartenir au nouveau traitement
          // vérification de la date d'ouverture
          // vérification des formats (sFormat est du type £D0;03)
          if ((bAvecOuverture) && (sFormat1 != "") && ((sFormat1[1] == 'D') || (sFormat1[1] == 'T')) && (sValeur1 != ""))
          {
          	if ((sUnite1 == "2DA01") || (sUnite1 == "2DA02"))
            {
            	if (strlen(sValeur1.c_str()) > 8)
              	sValeur1 = string(sValeur1, 0, 8) ;

              // Si le date d'ouverture est >= dateJour - 1 :
              // on retient le noeud pour ajouter au nouveau traitement
              incremente_date(sValeur1) ;

              if (sValeur1 >= tpsNow.donneDateHeure())
              {
              	if (bGereALD)
                {
                	if (_pDocLdv && _pDocLdv->isALDDrug(sNodeMedic))
                  {
                  	pNewTrtALD->AddString(sNodeRenouv) ;
                    pNewLexALD->AddString(sCodeMedic) ;
                  }
                  else
                  {
                  	pNewTraitement->AddString(sNodeRenouv) ;
                    pNewLexique->AddString(sCodeMedic) ;
                  }
                }
                else
                {
                	pNewTraitement->AddString(sNodeRenouv) ;
                  pNewLexique->AddString(sCodeMedic) ;
                }
              }
            }
          }
        }
      }
    }
  }
	return true ;
}

/**
 * Build a prescription document patpatho
 */
bool
NSPatientChoisi::buildPrescriptionPatpatho(NSFrameInformation* pFrameInfo, NSPatPathoArray* pPrescriPpt, bool bGereALD, VecteurString* pNewTraitement, VecteurString* pNewLexique, VecteurString* pNewTrtALD, VecteurString* pNewLexALD, bool bDCI, bool bMustSecure)
{
	if (((NSFrameInformation*) NULL == pFrameInfo) || ((NSPatPathoArray*) NULL == pPrescriPpt))
		return false ;

	pPrescriPpt->vider() ;

  string sRootCode = string("ZORDO1") ;

  // Secured prescription
  //
  if (bMustSecure)
    sRootCode = string("ZORDS1") ;

  // Bizone ?
  //
  else if (bGereALD && pNewTrtALD && (false == pNewTrtALD->empty()))
    sRootCode = string("ZORDB1") ;

	pPrescriPpt->ajoutePatho(sRootCode, 0) ;

  // Add known drug allergies
  //
  NSPatPathoArray allergiesPpt(pContexte->getSuperviseur()) ;
  getDrugsAllergiesPatpatho(&allergiesPpt) ;

  if (false == allergiesPpt.empty())
  {
    pPrescriPpt->ajoutePatho(string("EALMD1"), 1) ;
    pPrescriPpt->InserePatPatho(pPrescriPpt->end(), &allergiesPpt, 2) ;
  }

	if (false == bGereALD)
		return buildSimplePrescriptionPatpatho(pFrameInfo, pPrescriPpt, pNewTraitement, pNewLexique, bDCI) ;
	else
  	return buildBizonePrescriptionPatpatho(pFrameInfo, pPrescriPpt, pNewTraitement, pNewLexique, pNewTrtALD, pNewLexALD, bDCI) ;
}

/**
 * Build a prescription document patpatho for ALD and non ALD drugs
 */
bool
NSPatientChoisi::buildBizonePrescriptionPatpatho(NSFrameInformation* pFrameInfo, NSPatPathoArray* pPrescriPpt, VecteurString* pNewTraitement, VecteurString* pNewLexique, VecteurString* pNewTrtALD, VecteurString* pNewLexALD, bool bDCI)
{
	if (((NSFrameInformation*) NULL == pFrameInfo) || ((NSPatPathoArray*) NULL == pPrescriPpt))
		return false ;

	if (((VecteurString*) NULL == pNewLexique) ||
      ((VecteurString*) NULL == pNewTraitement) ||
      ((VecteurString*) NULL == pNewLexALD)  ||
      ((VecteurString*) NULL == pNewTrtALD))
		return false ;

	NSPatPathoArray SousPPT(pContexte->getSuperviseur()) ;

	// ALD
	if (false == pNewTrtALD->empty())
	{
  	if (true == pNewLexALD->empty())
    	return false ;

  	EquiItemIter iALD = pNewLexALD->begin() ;
    EquiItemIter jALD = pNewTrtALD->begin() ;

    pPrescriPpt->ajoutePatho(string("GPALD1"), 1) ;

    while ((pNewLexALD->end() != iALD) && (pNewTrtALD->end() != jALD))
    {
    	// constitution de la sous-patpatho du noeud
      // on cherche d'abord le code lexique correspondant au traitement
      // pour vérifier s'il s'agit d'un médicament ou d'un renouvelement
      //
      string sNodeId = *(*jALD) ;

      // Get the project tree that contains this node
      //
      NSDocumentHisto* pFoundDoc = pFrameInfo->findNode(sNodeId) ;
      if (pFoundDoc)
      {
        NSPatPathoArray PathoIndex(pContexte->getSuperviseur()) ;
        pFoundDoc->initFromPatPatho(&PathoIndex) ;

        if (false == PathoIndex.empty())
        {
          // Get the node inside the project tree
          //
          PatPathoIter pptItTrait = PathoIndex.ChercherNoeud(sNodeId) ;
          if (((PatPathoIter) NULL != pptItTrait) && (PathoIndex.end() != pptItTrait))
          {
            string sElemLex = (*pptItTrait)->getLexique() ;
            string sSens ;
            pContexte->getDico()->donneCodeSens(&sElemLex, &sSens) ;
            SousPPT.vider() ;

            if (string("GRENT") == sSens)
            {
              // In which frame are we working?
              LDVFRAME   iFrame = ldvframeNotSpecified ;
              NSLdvDrug* pDrug  = _pDocLdv->getTreatment(*(*jALD), iFrame) ;
              if (pDrug)
              {
                if (_pDocLdv && _pDocLdv->getDrugs(iFrame))
                  _pDocLdv->getDrugs(iFrame)->getRenewPatPatho(&SousPPT, *(*jALD)) ;
                else
                  return false ;
              }
              else
                return false ;

              // Insertion en queue (iter doit être ici egal à end())
              pPrescriPpt->InserePatPatho(pPrescriPpt->end(), &SousPPT, 2) ;
            }
            else
            {
              // Get the sub-patpatho of the node
              //
              pContexte->getPatient()->DonneSousArray(*(*jALD), &SousPPT) ;

              string sDrugCode = **iALD ;

              if (bDCI)
              {
                sDrugCode = pContexte->getUtilisateur()->getVirtualDrug(sDrugCode) ;
                if (string("") == sDrugCode)
                  sDrugCode = **iALD ;
              }

              // on reconstitue la patpatho à partir du noeud
              pPrescriPpt->ajoutePatho(sDrugCode, 2) ;
              // Insertion en queue (iter doit être ici egal à end())
              pPrescriPpt->InserePatPatho(pPrescriPpt->end(), &SousPPT, 3) ;
            }
          }
        }
      }

      iALD++ ;
      jALD++ ;
    }
  }
  // non ALD
  if (false == pNewTraitement->empty())
  {
  	if (true == pNewLexique->empty())
    	return false ;

  	EquiItemIter iNonALD = pNewLexique->begin() ;
    EquiItemIter jNonALD = pNewTraitement->begin() ;

    pPrescriPpt->ajoutePatho(string("GPCOD1"), 1) ;

    while ((pNewLexique->end() != iNonALD) && (pNewLexALD->end() != jNonALD))
    {
    	// constitution de la sous-patpatho du noeud
      // on cherche d'abord le code lexique correspondant au traitement
      // pour vérifier s'il s'agit d'un médicament ou d'un renouvelement
      //
      string sNodeId = *(*jNonALD) ;

      NSDocumentHisto* pFoundDoc = pFrameInfo->findNode(sNodeId) ;
      if (pFoundDoc)
      {
        NSPatPathoArray PathoIndex(pContexte->getSuperviseur()) ;
        pFoundDoc->initFromPatPatho(&PathoIndex) ;

        if (false == PathoIndex.empty())
        {
          PatPathoIter pptItTrait = PathoIndex.ChercherNoeud(sNodeId) ;
          if (((PatPathoIter) NULL != pptItTrait) && (PathoIndex.end() != pptItTrait))
          {
            string sElemLex = (*pptItTrait)->getLexique() ;
            string sSens ;
            pContexte->getDico()->donneCodeSens(&sElemLex, &sSens) ;
            SousPPT.vider() ;

            if (string("GRENT") == sSens)
            {
              // In which frame are we working?
              LDVFRAME iFrame = ldvframeNotSpecified ;
              NSLdvDrug* pDrug = _pDocLdv->getTreatment(*(*jNonALD), iFrame) ;
              if (pDrug)
              {
                if (_pDocLdv && _pDocLdv->getDrugs(iFrame))
                  _pDocLdv->getDrugs(iFrame)->getRenewPatPatho(&SousPPT, *(*jNonALD)) ;
                else
                  return false ;
              }
              else
                return false ;

              // Insertion en queue (iter doit être ici egal à end())
              pPrescriPpt->InserePatPatho(pPrescriPpt->end(), &SousPPT, 2) ;
            }
            else
            {
              pContexte->getPatient()->DonneSousArray(*(*jNonALD), &SousPPT) ;

              string sDrugCode = **iNonALD ;

              if (bDCI)
              {
                sDrugCode = pContexte->getUtilisateur()->getVirtualDrug(sDrugCode) ;
                if (string("") == sDrugCode)
                  sDrugCode = **iNonALD ;
              }

              // on reconstitue la patpatho à partir du noeud
              pPrescriPpt->ajoutePatho(sDrugCode, 2) ;
              // Insertion en queue (iter doit être ici egal à end())
              pPrescriPpt->InserePatPatho(pPrescriPpt->end(), &SousPPT, 3) ;
            }
          }
        }
      }

      iNonALD++ ;
      jNonALD++ ;
    }
  }
  return true ;
}

bool
NSPatientChoisi::buildSimplePrescriptionPatpatho(NSFrameInformation* pFrameInfo, NSPatPathoArray* pPrescriPpt, VecteurString* pNewTraitement, VecteurString* pNewLexique, bool bDCI)
{
	if (((NSFrameInformation*) NULL == pFrameInfo) || ((NSPatPathoArray*) NULL == pPrescriPpt))
		return false ;

	if (((VecteurString*) NULL == pNewLexique) || (true == pNewLexique->empty()))
		return false ;
	if (((VecteurString*) NULL == pNewTraitement) || (true == pNewTraitement->empty()))
		return false ;

	NSPatPathoArray SousPPT(pContexte->getSuperviseur()) ;

	EquiItemIter i = pNewLexique->begin() ;
	EquiItemIter j = pNewTraitement->begin() ;

	while ((pNewLexique->end() != i) && (pNewTraitement->end() != j))
	{
  	// on cherche d'abord le code lexique correspondant au traitement
    // pour vérifier s'il s'agit d'un médicament ou d'un renouvelement
    //
    string sNodeId = *(*j) ;

    NSDocumentHisto* pFoundDoc = pFrameInfo->findNode(sNodeId) ;
    if (pFoundDoc)
    {
      NSPatPathoArray PathoIndex(pContexte->getSuperviseur()) ;
      pFoundDoc->initFromPatPatho(&PathoIndex) ;

      if (false == PathoIndex.empty())
      {
        PatPathoIter pptItTrait = PathoIndex.ChercherNoeud(sNodeId) ;
        if ((NULL != pptItTrait) && (PathoIndex.end() != pptItTrait))
        {
          string sElemLex = (*pptItTrait)->getLexique() ;
          string sSens ;
          pContexte->getDico()->donneCodeSens(&sElemLex, &sSens) ;
          SousPPT.vider() ;

          if (string("GRENT") == sSens)
          {
            // In which frame are we working?
            LDVFRAME iFrame = ldvframeNotSpecified ;
            NSLdvDrug* pDrug = _pDocLdv->getTreatment(*(*j), iFrame) ;
            if (pDrug)
            {
              if (_pDocLdv && _pDocLdv->getDrugs(iFrame))
                _pDocLdv->getDrugs(iFrame)->getRenewPatPatho(&SousPPT, *(*j)) ;
              else
                return false ;
            }
            else
              return false ;

            // Insertion en queue (iter doit être ici egal à end())
            pPrescriPpt->InserePatPatho(pPrescriPpt->end(), &SousPPT, 1) ;
          }
          else
          {
            // prescription d'un nouveau médicament
            // constitution de la sous-patpatho du noeud
            pContexte->getPatient()->DonneSousArray(*(*j), &SousPPT) ;

            string sDrugCode = **i ;

            if (bDCI)
            {
              sDrugCode = pContexte->getUtilisateur()->getVirtualDrug(sDrugCode) ;
              if (string("") == sDrugCode)
                sDrugCode = **i ;
            }

            // on reconstitue la patpatho à partir du noeud
            pPrescriPpt->ajoutePatho(sDrugCode, 1) ;
            // Insertion en queue (iter doit être ici egal à end())
            pPrescriPpt->InserePatPatho(pPrescriPpt->end(), &SousPPT, 2) ;
          }
        }
      }
    }

    i++ ;
    j++ ;
	}
  return true ;
}

void
NSPatientChoisi::getDrugsAllergiesPatpatho(NSPatPathoArray* pAllergiesPpt)
{
  if ((NSPatPathoArray*) NULL == pAllergiesPpt)
    return ;

  // Find the proper chapter in synthesis
  //
  NSSearchStruct searchStruct ;
  string sSynthesisPath = string("ZSYNT/QANTP/EALMD") ;

  bool bOk = ChercheChemin(sSynthesisPath, &searchStruct) ;
  if ((false == bOk) || (NSSearchStruct::foundNothing == searchStruct.getFoundStatus()))
    return ;

  std::string date   = string("") ;
  std::string sNoeud = string("") ;

  MappingNSSearchResult::MMapIt it = searchStruct.getFoundNodes()->begin() ;
  searchStruct.getFoundNodes()->fullRData(it, date, sNoeud) ;

  NSPatPathoArray Ppt(pContexte->getSuperviseur()) ;

  // Get its ppt
  //
  DonneArray(sNoeud, &Ppt) ;
  if (Ppt.empty())
    return ;

  for (PatPathoIter iter = Ppt.begin() ; Ppt.end() != iter ;  iter++)
  {
    // Get node label
    //
    string sElemLex = (*iter)->getLexique() ;

    if (('_' == sElemLex[0]) || ('I' == sElemLex[0]) || ('8' == sElemLex[0]) ||
        ('N' == sElemLex[0]) || ('O' == sElemLex[0]))
    {
      pAllergiesPpt->ajoutePatho(sElemLex, 0) ;
    }
    else
    {
      string sSens ;
      pContexte->getDico()->donneCodeSens(&sElemLex, &sSens) ;

      if ((string("£??") == sSens) || (string("£CL") == sSens))
      {
        Message Msg ;
        Msg.SetLexique(sElemLex) ;
        Msg.SetTexteLibre((*iter)->getTexteLibre()) ;
        pAllergiesPpt->ajoutePatho(sElemLex, &Msg, 0) ;
      }
    }
  }
}

DocumentIter
NSPatientChoisi::getFrameIndex(LDVFRAME iFrame, bool bVerbose)
{
  return findFrameIndex(getRootForFrame(iFrame), bVerbose) ;
}

DocumentIter
NSPatientChoisi::findFrameIndex(string sRootCode, bool bVerbose)
{
  if (string("") == sRootCode)
    return (DocumentIter) 0 ;

  string sRootSens = string("") ;
  pContexte->getDico()->donneCodeSens(&sRootCode, &sRootSens) ;

	if ((NSHISTODocument*) NULL == _pDocHis)
	{
    string sErrorText = pContexte->getSuperviseur()->getText("documentManagementErrors", "cannotFindHistoryManager") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
    if (bVerbose)
		  erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
		return (DocumentIter) 0 ;
	}

  NSDocHistoArray* pVectDocument = _pDocHis->getVectDocument() ;

	if (((NSDocHistoArray*) NULL == pVectDocument) || pVectDocument->empty())
	{
    string sErrorText = pContexte->getSuperviseur()->getText("documentManagementErrors", "emptyHistoryManager") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
    if (bVerbose)
		  erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
		return (DocumentIter) 0 ;
	}

	// Recherche du document "Index de santé"
	// Looking for "Health index" document
	DocumentIter iterDoc ;

	bool bSearchIndex = true ;
	while (bSearchIndex)
	{
		bool bContinuer = true ;
		iterDoc = pVectDocument->begin() ;
		while ((pVectDocument->end() != iterDoc) && bContinuer)
		{
      NSPatPathoArray PPT(pContexte->getSuperviseur()) ;
      (*iterDoc)->initFromPatPatho(&PPT) ;

    	if (false == PPT.empty())
			{
				PatPathoIter iter = PPT.begin() ;
				if ((*iter)->getLexiqueSens() == sRootSens)
					bContinuer = false ;
				else
					iterDoc++ ;
			}
      else
				iterDoc++ ;
		}

		// Index de santé non trouvé - Health index not found
		if (pVectDocument->end() == iterDoc)
		{
			// En MUE on signale une erreur car l'index de santé est normalement créé
      // lors de l'initialisation du patient (méthode NSPatientChoisi::CreeRootDocs())

      string sMissingChapter = string("healthIndexIsMissing") ;
      if      (string("ZPOMR") == sRootSens)
        sMissingChapter = string("healthIndexIsMissing") ;
      else if (string("ORISK") == sRootSens)
        sMissingChapter = string("riskIndexIsMissing") ;
      else if (string("ZPSOC") == sRootSens)
        sMissingChapter = string("socialIndexIsMissing") ;

      string sErrorText = pContexte->getSuperviseur()->getText("LigneDeVieErrors", sMissingChapter) ;
      pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
      if (bVerbose)
      {
        string sWarningText = pContexte->getSuperviseur()->getText("generalLanguage", "warning") ;
        ::MessageBox(pContexte->GetMainWindow()->GetHandle(),
                            sErrorText.c_str(),
                            sWarningText.c_str(), MB_OK) ;
      }
      return NULL ;
		}
		else
			bSearchIndex = false ;
	}

    // ???????????????????
	if (pVectDocument->end() == iterDoc)
		return (DocumentIter) 0 ;

  return iterDoc ;
}

//---------------------------------------------------------------------------
//
// Recherche du document Index de santé
// Finding the Health index document
//
//---------------------------------------------------------------------------
DocumentIter
NSPatientChoisi::trouveIndexSante()
{
  return findFrameIndex(string("ZPOMR1")) ;
}

DocumentIter
NSPatientChoisi::findRiskIndex()
{
  return findFrameIndex(string("ORISK1")) ;
}

DocumentIter
NSPatientChoisi::findSocialIndex()
{
  return findFrameIndex(string("ZPSOC1")) ;
}

/**
 * Get the drugs view, if opened
 */
NSDrugView*
NSPatientChoisi::getDrugView()
{
  if ((NSLdvDocument*) NULL == _pDocLdv)
		return (NSDrugView*) 0 ;

	TView* pView = _pDocLdv->GetViewList() ;
  if (pView)
  {
		do
		{
			NSDrugView* pDrugView = TYPESAFE_DOWNCAST(pView, NSDrugView) ;
			if (pDrugView)
				return pDrugView ;

			pView = _pDocLdv->NextView(pView) ;
		}
		while (pView) ;
  }

  return (NSDrugView*) 0 ;
}

void
NSPatientChoisi::remplaceTagsChemin(string& sHtml, NSPatPathoArray* pPatPathoArray, map<string, string> *pTagsBuffer, map<string, bool> *pCondBuffer, HWND interfaceHandle)
{
	if (string("") == sHtml)
		return ;

  bool bAvecPrecoche = false ;
  if ((NSPatPathoArray*) NULL == pPatPathoArray)
  	bAvecPrecoche = true ;

  // Step 1 : processing [[IF
  //
  remplaceTagsIfElse(sHtml, pPatPathoArray, pCondBuffer) ;

  // Step 2 : processing other tags
  //
  remplaceOrdinaryTags(sHtml, bAvecPrecoche, pPatPathoArray, pTagsBuffer) ;
}

void
NSPatientChoisi::remplaceTagsIfElse(string& sHtml, NSPatPathoArray* pPatPathoArray, map<string, bool> *pCondBuffer)
{
  // Etape 1 : on cherche les [[IF
  //
  // Attention : ils pourraient être imbriqués

  size_t pos1 = sHtml.find("[[IF") ;

  while (NPOS != pos1)
  {
  	// Etape 1 : On prend le chemin et son contenu (Metatag)
    //
    size_t pos2 = sHtml.find("]", pos1) ;
    if (NPOS == pos2)
    {
    	erreur("Tag incorrect dans le html de présentation du document.", standardError, 0) ;
      sHtml = string("") ;
      return ;
    }
    // If a tag was opened before the closing ']', it means that this ']' didn't
    // close the [IF
    //
    size_t posOpenTag = sHtml.find("[", pos1+2) ;  // +2 because pos1 -> [[IF
    while (posOpenTag < pos2)
    {
      pos2       = sHtml.find("]", pos2+1) ;
      posOpenTag = sHtml.find("[", posOpenTag+1) ;
    }

    string sChemin = string(sHtml, pos1+4, pos2-pos1-4) ;
    strip(sChemin, stripBoth) ;

    // On cherche le ] qui clot le IF
    //
    // On en profite pour trouver s'il y a un [ELSE]
    //
    size_t pos1_else, pos2_else ;
    bool   bElse = false ;

    size_t iHtmlLen  = strlen(sHtml.c_str()) ;
    size_t pos3      = pos2 + 1 ;
    bool   bTourner  = true ;
    int    iCompteur = 1 ;
    while (bTourner && (pos3 < iHtmlLen))
    {
    	if      ('[' == sHtml[pos3])
      {
        if ((1 == iCompteur) && (pos3 <= iHtmlLen - 6) &&
            ('E' == sHtml[pos3+1]) &&
            ('L' == sHtml[pos3+2]) &&
            ('S' == sHtml[pos3+3]) &&
            ('E' == sHtml[pos3+4]) && (']' == sHtml[pos3+5]))
        {
          bElse = true ;
          pos1_else = pos3 ;
          pos2_else = pos3 + 5 ;
        }
        iCompteur++ ;
      }
      else if (']' == sHtml[pos3])
      	iCompteur-- ;

      if (iCompteur > 0)
      	pos3++ ;
      else
      	bTourner = false ;
    }
    if (iCompteur > 0)
    {
    	erreur("MetaTag incorrect dans le html de présentation du document.", standardError, 0) ;
      sHtml = string("") ;
      return ;
    }
    string sMetaTag = string(sHtml, pos2 + 1, pos3 - pos2 - 1) ;

    int iBoolValue = 0 ;

    // Is the answer already known?
    //
    bool bFoundInBuffer = false ;
    if (pCondBuffer && (false == pCondBuffer->empty()))
    {
      map<string, bool>::iterator tagsIter = pCondBuffer->begin() ;
      for ( ; (pCondBuffer->end() != tagsIter) && ((*tagsIter).first != sChemin) ; tagsIter++) ;
      if (pCondBuffer->end() != tagsIter)
      {
        iBoolValue = (*tagsIter).second ? 1 : 0 ;
        bFoundInBuffer = true ;
      }
    }

    //
    // On regarde si le tag est valide
    //
    if (false == bFoundInBuffer)
    {
      iBoolValue = getLogicalStringValue(sChemin, pPatPathoArray) ;

      if ((map<string, bool>*) NULL != pCondBuffer)
        (*pCondBuffer)[sChemin] = iBoolValue > 0 ? true : false ;
    }

    bool bPathFound = (iBoolValue > 0) ;

    size_t pos4 ;

    if (true == bPathFound)
    {
      if (bElse)
      	pos4 = pos1_else ;
      else
      	pos4 = pos3 ;

      if (0 != pos1)
      {
      	if (pos3 < iHtmlLen - 1)
        	sHtml = string(sHtml, 0, pos1) + string(sHtml, pos2+1, pos4-pos2-1) + string(sHtml, pos3+1, iHtmlLen-pos3-1) ;
      	else
        	sHtml = string(sHtml, 0, pos1) + string(sHtml, pos2+1, pos4-pos2-1) ;
      }
      else
      {
      	if (pos3 < iHtmlLen - 1)
        	sHtml = string(sHtml, pos2+1, pos4-pos2-1) + string(sHtml, pos3+1, iHtmlLen-pos3-1) ;
        else
        	sHtml = string(sHtml, pos2+1, pos4-pos2-1) ;
      }
    }
    //
    // Pas trouvé avec else
    //
    else if (bElse)
    {
      pos2 = pos2_else ;
      pos4 = pos3 ;

      if (0 != pos1)
      {
        if (pos3 < iHtmlLen - 1)
        	sHtml = string(sHtml, 0, pos1) + string(sHtml, pos2+1, pos4-pos2-1) + string(sHtml, pos3+1, iHtmlLen-pos3-1) ;
        else
        	sHtml = string(sHtml, 0, pos1) + string(sHtml, pos2+1, pos4-pos2-1) ;
      }
      else
      {
        if (pos3 < iHtmlLen - 1)
        	sHtml = string(sHtml, pos2+1, pos4-pos2-1) + string(sHtml, pos3+1, iHtmlLen-pos3-1) ;
        else
        	sHtml = string(sHtml, pos2+1, pos4-pos2-1) ;
      }
    }
    //
    // Pas trouvé sans else
    //
    else
    {
    	if (0 != pos1)
      {
        if (pos3 < iHtmlLen - 1)
        	sHtml = string(sHtml, 0, pos1) + string(sHtml, pos3+1, iHtmlLen-pos3-1) ;
        else
        	sHtml = string(sHtml, 0, pos1) ;
      }
      else
      {
        if (pos3 < iHtmlLen - 1)
        	sHtml = string(sHtml, pos3+1, iHtmlLen-pos3-1) ;
        else
        {
        	sHtml = string("") ;
          return ;
        }
      }
    }

    // Repartir au début pour gérer les
    pos1 = sHtml.find("[[IF", pos1) ;
  }
}

void
NSPatientChoisi::remplaceOrdinaryTags(string& sHtml, bool bAvecPrecoche, NSPatPathoArray* pPatPathoArray, map<string, string> *pTagsBuffer, HWND interfaceHandle)
{
  if (string("") == sHtml)
    return ;

  size_t pos1 = sHtml.find("[") ;
  if (NPOS == pos1)
    return ;

  while (NPOS != pos1)
  {
    size_t pos2 = sHtml.find("]", pos1);
    if (NPOS == pos2)
    {
      string sErrorText = pContexte->getSuperviseur()->getText("wordProcessorManagement", "noClosingTagFound") ;
      sErrorText += string(" ( ") + sHtml + string(" ).") ;
      pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		  erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
      sHtml = string("") ;
      return ;
    }

    string sTag = string(sHtml, pos1 + 1, pos2 - pos1 - 1) ;
    size_t pos3 = sTag.find(" ") ;
    if (NPOS == pos3)
    {
      string sErrorText = pContexte->getSuperviseur()->getText("wordProcessorManagement", "missingSubjectInTag") ;
      sErrorText += string(" ( ") + sTag + string(" ).") ;
      pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		  erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
      sHtml = string("") ;
      return ;
    }

    // Type de tag : LABEL ou VALUE
    string sTypeTag = string(sTag, 0, pos3) ;

    if ((string("LABEL")     != sTypeTag) &&
        (string("VALUE")     != sTypeTag) &&
        (string("VALUE?")    != sTypeTag) &&
        (string("TREEVALUE") != sTypeTag) &&
        (string("FREELABEL") != sTypeTag))
    {
    	string sErrorText = pContexte->getSuperviseur()->getText("wordProcessorManagement", "unknownTag") ;
      sErrorText += string(" ( ") + sTag + string(" ).") ;
      pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		  erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
      sHtml = string("") ;
      return ;
    }

    string sLabel = string("") ;

    // Le chemin récupéré est par convention un chemin de codes lexiques séparés par "/"
    //
    string sChemin = string(sTag, pos3+1, strlen(sTag.c_str())-pos3-1) ;

    string sModifiers = getQueryPathModifiers(sChemin) ;

    // Le tag FREELABEL est considéré comme n'ayant rien à voir avec la patpatho
    // contrairement au tag LABEL qui est traité comme un tag VALUE sans fils

    if (string("FREELABEL") == sTypeTag)
      remplaceFreeLabel(sHtml, sChemin, &pos1, &pos2) ;
    else
      remplaceStandardLabel(sHtml, sTypeTag, sModifiers, bAvecPrecoche, sChemin, &pos1, &pos2, pPatPathoArray, pTagsBuffer) ;

    pos1 = sHtml.find("[", pos2) ;
  }
}

void
NSPatientChoisi::remplaceFreeLabel(string& sHtml, string sChemin, size_t *pPos1, size_t *pPos2)
{
  if ((string("") == sHtml) || (NULL == pPos1) || (NULL == pPos2))
    return ;

  // Décodage de l'élément lexique
  // on forme une patpatho contenant l'élément
  NSPatPathoArray PT(pContexte->getSuperviseur()) ;

  if (string("") == sChemin)
  {
    // iterNode n'est pas trouvé => on efface le tag
    sHtml.replace(*pPos1, *pPos2 - *pPos1 + 1, "") ;
    *pPos2 = *pPos1 ;
    return ;
  }

  string sElement        = string("") ;
  string sCheminNode     = string("") ;
  string sCheminSensNode = string("") ;

  size_t szPosLastSeparator = sChemin.find_last_of(string(1, cheminSeparationMARK)) ;
  if (szPosLastSeparator != string::npos)
  {
    sCheminNode = string(sChemin, 0, szPosLastSeparator) ;
    sElement    = string(sChemin, szPosLastSeparator+1, strlen(sChemin.c_str())-szPosLastSeparator-1) ;
    pContexte->getDico()->donneCodeSens(&sCheminNode, &sCheminSensNode) ;
  }
  else
    sElement = sChemin ;

  PT.ajoutePatho(sElement, 0) ;

  GlobalDkd Dcode(pContexte, pContexte->getUserLanguage(), sCheminSensNode, &PT) ;
  Dcode.decodeNoeud(sCheminNode) ;
  string sLabel = Dcode.getDcodeur() ;

  // pour un libellé, on met la première lettre en majuscule
  sLabel[0] = pseumaj(sLabel[0]) ;

  sHtml.replace(*pPos1, *pPos2 - *pPos1 + 1, sLabel) ;
  *pPos2 = *pPos1 + strlen(sLabel.c_str()) ;
}

void
NSPatientChoisi::remplaceStandardLabel(string& sHtml, string sTypeTag, string sModifiers, bool bAvecPrecoche, string sChemin, size_t *pPos1, size_t *pPos2, NSPatPathoArray* pPatPathoArray, map<string, string> *pTagsBuffer, HWND interfaceHandle)
{
  bool bMustDeleteSearchPpt = false ;
  NSPatPathoArray* pSearchPPt = (NSPatPathoArray*) 0 ;

  // Starting by "?/" : if not found in pPatPathoArray, search in whole record
  //
  bool bGlobalSearch = false ;
  if ((string("") != sModifiers) && (string::npos != sModifiers.find("S=G")))
    bGlobalSearch = true ;

  string sWishedUnit = string("") ;
  size_t iUnitPos = sModifiers.find("U=") ;
  if ((string::npos != iUnitPos) && (strlen(sModifiers.c_str()) > iUnitPos + 2))
  {
    size_t iEndUnit = sModifiers.find("|", iUnitPos) ;
    if (string::npos != iEndUnit)
    {
      if (iEndUnit - iUnitPos > 2)
        sWishedUnit = string(sModifiers, iUnitPos+2, iEndUnit-iUnitPos-2) ;
    }
    else
      sWishedUnit = string(sModifiers, iUnitPos+2, strlen(sModifiers.c_str())-iUnitPos-2) ;
  }

  bool bGetTreePath = true ;
  if ((string("") != sModifiers) && (string::npos != sModifiers.find("P=L")))
    bGetTreePath = false ;

  bool bFirstLetterUpperCase = true ;
  if ((string("") != sModifiers) && (string::npos != sModifiers.find("F=L")))
    bFirstLetterUpperCase = false ;

  string sLabel = string("") ;

  // Special case, reserved terms "nomPatient" or "dateJour"
  //
  if      (string("nomPatient") == sChemin)
    sLabel = getNomLong() ;
  else if (string("signature") == sChemin)
    sLabel = pContexte->getUtilisateur()->getCivilProf() ;
  else if (string("dateJour") == sChemin)
  {
    string sLang = pContexte->getUserLanguage() ;

    NVLdVTemps tpsNow ;
    tpsNow.takeTime() ;
    sLabel = tpsNow.donneFormattedDate(pContexte, sLang) ;
  }

  // Standard case
  //
  if (string("") == sLabel)
  {
    // In case of a label to be taken literally, no need to search inside tree
    //
    if ((string("LABEL") == sTypeTag) && (false == bGetTreePath))
    {
	    ClasseStringVector Vect ;
	    DecomposeChaine(&sChemin, &Vect, string(1, cheminSeparationMARK)) ;
      if (false == Vect.empty())
      {
        NSPatPathoArray PT(pContexte->getSuperviseur()) ;

        string sCheminNode = string("") ;

        iterClassString iterpVect = Vect.begin() ;
        string sItem = (*iterpVect)->getItem() ;
        iterpVect++ ;
        for ( ; Vect.end() != iterpVect ; iterpVect++)
        {
          if (string("") != sCheminNode)
            sCheminNode += string(1, cheminSeparationMARK) ;
          sCheminNode += sItem ;

          sItem = (*iterpVect)->getItem() ;
        }

        Message insertMsg ;
        insertMsg.InitFromEtiquette(sItem) ;

        PT.ajoutePatho(insertMsg.GetLexique(), &insertMsg, 0) ;

        string sCheminSensNode = string("") ;
        pContexte->getDico()->donneCodeSens(&sCheminNode, &sCheminSensNode) ;

        GlobalDkd Dcode(pContexte, pContexte->getUserLanguage(), sCheminSensNode, &PT) ;
        Dcode.decodeNoeud(sCheminNode) ;
        sLabel = Dcode.getDcodeur() ;

        // pour un libellé, on met la première lettre en majuscule
        if ((string("LABEL") == sTypeTag) && (string("") != sLabel) && bFirstLetterUpperCase)
          sLabel[0] = pseumaj(sLabel[0]) ;
      }
    }
    // Not a LABEL, need to search path in tree(s)
    //
    else
    {
      PatPathoIter iterNode = NULL ;

      // on crée le chemin de codes sens
      //
      string sCheminSens = string("") ;
      pContexte->getDico()->donneCodeSens(&sChemin, &sCheminSens) ;

      // on récupère la valeur du chemin spécifié dans la patpatho du document
      //
      bool bPathFound = false ;
      if (false == bAvecPrecoche)
      {
        if (pPatPathoArray->CheminDansPatpatho(sCheminSens, string(1, cheminSeparationMARK), &iterNode))
        {
          bPathFound = true ;
          pSearchPPt = pPatPathoArray ;
        }
        else if (bGlobalSearch)
        {
          string answer_date ;
          bPathFound = pContexte->getBBinterface()->synchronousCall(sCheminSens, &pSearchPPt, interfaceHandle, &answer_date) ;
          if (bPathFound)
            iterNode = pSearchPPt->begin() ;
          bMustDeleteSearchPpt = true ;
        }
      }
      else
      {
        string answer_date = string("") ;
        bPathFound = pContexte->getBBinterface()->synchronousCall(sCheminSens, &pSearchPPt, interfaceHandle, &answer_date) ;
        if (bPathFound)
          iterNode = pSearchPPt->begin() ;
        bMustDeleteSearchPpt = true ;
      }

      if (true == bPathFound)
      {
        string sCheminNode = string("") ;

        if (false == bAvecPrecoche)
        {
          // pour une valeur, on doit récupérer le noeud suivant le chemin
          if ((string("VALUE") == sTypeTag) || (string("VALUE?") == sTypeTag))
            if (iterNode)
              iterNode++ ;

          // on récupère le chemin effectif (du pere) du node
          sCheminNode = pSearchPPt->donneCheminItem(iterNode) ;
        }
        else
        {
          if (pSearchPPt->empty())
          {
            sHtml = string("") ;
            if ((true == bMustDeleteSearchPpt) && (NULL != pSearchPPt))
              delete pSearchPPt ;
            return ;
          }

          iterNode = pSearchPPt->begin() ;
          // pour une valeur, on doit récupérer le noeud suivant le chemin
          if ((string("VALUE") == sTypeTag) || (string("VALUE?") == sTypeTag))
            iterNode++ ;

          if (pSearchPPt->end() == iterNode)
          {
            sHtml = string("") ;
            if ((true == bMustDeleteSearchPpt) && pSearchPPt)
              delete pSearchPPt ;
            return ;
          }

          sCheminNode = sChemin ;
        }

        // on crée le chemin de codes sens
        //
        string sCheminSensNode = string("") ;
        pContexte->getDico()->donneCodeSens(&sCheminNode, &sCheminSensNode) ;

        // Décodage de l'élément lexique
        // on forme une patpatho contenant l'élément
        NSPatPathoArray PT(pContexte->getSuperviseur()) ;

        if ((string("VALUE") == sTypeTag) || (string("VALUE?") == sTypeTag) || (string("LABEL") == sTypeTag))
        {
          if (pSearchPPt->end() != iterNode)
          {
            if (string("") == sWishedUnit)
              PT.ajoutePatho(iterNode, 0, 0) ;
            else
            {
              PatPathoIter iterPere = pSearchPPt->ChercherPere(iterNode) ;
              if ((NULL != iterPere) && (pSearchPPt->end() != iterPere))
              {
                NSPatPathoArray subPatPatho(pContexte->getSuperviseur()) ;
                pSearchPPt->ExtrairePatPatho(iterPere, &subPatPatho) ;

                double dValue ;
                bool bPathFound = subPatPatho.getNumericalValueForUnit(sWishedUnit, &dValue, (*iterPere)->getLexique()) ;

                if (bPathFound)
                {
                  Message Msg ;
                  Msg.SetLexique((*iterNode)->getLexique()) ;
                  Msg.SetUnit(sWishedUnit) ;
                  Msg.SetComplement(DoubleToString(&dValue, -1, -1)) ;
                  PT.ajoutePatho((*iterNode)->getLexique(), &Msg, 0) ;
                }
                else
                  PT.ajoutePatho(iterNode, 0, 0) ;
              }
            }
          }
        }
        else if (string("TREEVALUE") == sTypeTag)
          pSearchPPt->ExtrairePatPatho(iterNode, &PT) ;

        if (false == PT.empty())
        {
          // TreeValue : the whole tree must be published
          //
          if (string("TREEVALUE") == sTypeTag)
            sLabel = getTreeAsHtml(sCheminNode, &PT) ;
          else
          {
            GlobalDkd Dcode(pContexte, pContexte->getUserLanguage(), sCheminSensNode, &PT) ;
            Dcode.decodeNoeud(sCheminNode) ;
            sLabel = Dcode.getDcodeur() ;

            // pour un libellé, on met la première lettre en majuscule
            if ((string("LABEL") == sTypeTag) && (string("") != sLabel) && bFirstLetterUpperCase)
              sLabel[0] = pseumaj(sLabel[0]) ;
          }
        }
      }
    }
  }

  // Asking user for confirmation
  //
  if (string("VALUE?") == sTypeTag)
  {
    // Have a look in the buffer
    //
    bool bFoundInBuffer = false ;
    if ((NULL != pTagsBuffer) && (false == pTagsBuffer->empty()))
    {
      map<string, string>::iterator tagsIter = pTagsBuffer->begin() ;
      for ( ; (pTagsBuffer->end() != tagsIter) && ((*tagsIter).first != sChemin) ; tagsIter++) ;
      if (pTagsBuffer->end() != tagsIter)
      {
        sLabel = (*tagsIter).second ;
        bFoundInBuffer = true ;
      }
    }
    // Not found, ask user
    //
    if (false == bFoundInBuffer)
    {
      NSValueInfoDlg *pConfirmDlg = new NSValueInfoDlg(pContexte->GetMainWindow(), pContexte, &sChemin, &sLabel) ;
      int iUserChoice = pConfirmDlg->Execute() ;
      if (IDOK == iUserChoice)
      {
        sLabel = pConfirmDlg->getContent() ;
        if (NULL != pTagsBuffer)
          (*pTagsBuffer)[sChemin] = sLabel ;
      }
      delete pConfirmDlg ;
    }
  }

  sHtml.replace(*pPos1, *pPos2 - *pPos1 + 1, sLabel) ;
  *pPos2 = *pPos1 + strlen(sLabel.c_str()) ;

  if ((true == bMustDeleteSearchPpt) && pSearchPPt)
    delete pSearchPPt ;
}

string
NSPatientChoisi::getTreeAsHtml(string sPath, NSPatPathoArray* pPatPathoArray)
{
  if (((NSPatPathoArray*) NULL == pPatPathoArray) || pPatPathoArray->empty())
    return string("") ;

  string sHtmlValue = string("<ul>") ;

  for (PatPathoIter it = pPatPathoArray->begin(); pPatPathoArray->end() != it; it++)
	{
		if ((*it)->getColonne() == 0)
    {
      // decodeNoeud only treats the first node in the tree, so we have to
      // extract a ppt whose root is current node
      //
      NSPatPathoArray pptDecod(pContexte->getSuperviseur()) ;
      pPatPathoArray->ExtrairePatPathoFreres(it, &pptDecod) ;

      GlobalDkd Dcode(pContexte, pContexte->getUserLanguage(), sPath, &pptDecod) ;
      Dcode.decodeNoeud(sPath) ;
      sHtmlValue += string("<li>") + Dcode.getDcodeur() ;

      NSPatPathoArray pptFils(pContexte->getSuperviseur()) ;
      pPatPathoArray->ExtrairePatPatho(it, &pptFils) ;
      if (false == pptFils.empty())
      {
        string sNewPath = sPath ;
        if (string("") != sNewPath)
          sNewPath += string(1, cheminSeparationMARK) ;
        sNewPath += (*it)->getNodeLabel() ;

        string sSonsHtmlValues = getTreeAsHtml(sNewPath, &pptFils) ;

        sHtmlValue += sSonsHtmlValues ;
      }

      sHtmlValue += string("</li>") ;
    }
  }

  sHtmlValue += string("</ul>") ;

  return sHtmlValue ;
}

//
// return : 0 false, 1 true, -1 invalid entry
//
int
NSPatientChoisi::getLogicalStringValue(string sFormula, NSPatPathoArray* pPatPathoArray)
{
  string sMsg = string("getLogicalStringValue for \"") + sFormula + string("\"") ;
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubSteps) ;

  // Convention: an empty Formula is true
  //
  if (string("") == sFormula)
    return 1 ;

  // First step: just store distinct logical blocks
  //
  map<string, string> aBlocks ;

  parseLogicalBlocks(sFormula, &aBlocks) ;

  if (aBlocks.empty())
  {
    sMsg = string("getLogicalStringValue: no logical block found") ;
    pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trDetails) ;
    return -1 ;
  }

  map<string, bool> mBoolVars ; // large streets :-)

  // Now, let give a boolean value to each block and assign them to a var inside
  // formula
  //
  // size_t iBlockNb = 1 ;

  for (map<string, string>::iterator it = aBlocks.begin() ; aBlocks.end() != it ; it++)
  {
    // 2 possibilities :
    //        - just a path (and just need to check that it exists)
    //        - Lvalue operator Rvalue
    //          where Lvalue and Rvalue can be numerical operations
    //
    string sPathToFind = (*it).first ;
    string sExpression = string("") ;

    sMsg = string("getLogicalStringValue: processing block \"") + sPathToFind + string("\"") ;
    pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trDetails) ;

    string sLValue   = string("") ;
    string sOperator = string("") ;
    string sRValue   = string("") ;

    string sErrorMsg = string("") ;

    if (false == parseComparisonBlock(sPathToFind, sLValue, sOperator, sRValue, &sErrorMsg, true))
      return -1 ;

    bool bCompRes = resolveComparison(sLValue, sOperator, sRValue, &sErrorMsg, pPatPathoArray) ;

/*
    // Now it's time to add this information to mBoolVars and update sFormula
    //
    string sBlockName = string("block") + IntToString(iBlockNb) ;
    iBlockNb++ ;

    size_t iBlockPos = sFormula.find(**it) ;
    while (string::npos != iBlockPos)
    {
      sFormula.replace(iBlockPos, strlen((*it)->c_str()), sBlockName) ;
      iBlockPos = sFormula.find(**it) ;
    }
*/

    mBoolVars[(*it).second] = bCompRes ;

    sMsg = string("getLogicalStringValue: block \"") + sPathToFind + string("\" becomes var \"") + (*it).second + string("\" with value ") ;
    if (bCompRes)
      sMsg += string("true") ;
    else
      sMsg += string("false") ;
    pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trDetails) ;
  }

  // Now, we just have to evaluate sFormula
  //
  size_t iFormulaStart = 0 ;
  int iEvaluationResult = evaluate(&mBoolVars, sFormula, iFormulaStart) ;

  sMsg = string("getLogicalStringValue for \"") + sFormula + string("\" returns ") ;
  switch(iEvaluationResult)
  {
    case  0 : sMsg += string("false") ; break ;
    case  1 : sMsg += string("true") ;  break ;
    case -1 : sMsg += string("invalid entry") ; break ;
  }
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubSteps) ;

  return iEvaluationResult ;
}

bool
NSPatientChoisi::resolveComparison(string sLValue, string sOperator, string sRValue, string *psErrorMsg, NSPatPathoArray* pPatPathoArray)
{
  if ((string("") == sLValue) && (string("") == sOperator) && (string("") == sRValue))
    return true ;

  string sMsg = string("resolveComparison for LValue \"") + sLValue + string("\", Operator \"") + sOperator + string("\", RValue \"") + sRValue + string("\"") ;
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trDetails) ;

  // First case, no operator and no RValue: just "exist"
  //
  if ((string("") == sOperator) && (string("") == sRValue))
    return resolveExistsComparison(sLValue, psErrorMsg, pPatPathoArray) ;

  // Second case: "LValue operator RValue"
  //
  map<string, double> aValues ;
  map<string, string> aVarAlias ;

  // This is the unit that must be used for comparing LValue and RValue
  // Example 2KGM2 in formula: VIMC0 *> 35[2KGM2]
  //
  // Must be done before calling initMapForNumericalExpression (which replaces
  // concepts by aliases)
  //
  string sComparisonUnit = getComparisonUnit(sLValue, sRValue) ;

  // Fills the "field - value" map
  //
  string sErrorMsg = string("") ;
  if (false == initMapForNumericalExpression(&sLValue, &aVarAlias, &sErrorMsg))
    return false ;
  if (false == initMapForNumericalExpression(&sRValue, &aVarAlias, &sErrorMsg))
    return false ;

  // Get values when no vars
  //
  if (aVarAlias.empty())
  {
    double dLResult = calculate(NULL, sLValue) ;
    double dRResult = calculate(NULL, sRValue) ;

    bool bResult = compare(dLResult, sOperator, dRResult) ;

    sMsg = string("resolveComparison for constants only returns ") ;
    if (bResult)
      sMsg += string("true") ;
    else
      sMsg += string("false") ;
    pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trDetails) ;

    return bResult ;
  }

  // Now, let give a boolean value to each block and assign them to a var inside
  // formula
  //
  // size_t iBlockNb = 1 ;

  map<string, string>::iterator mapIter ;
  for (mapIter = aVarAlias.begin() ; aVarAlias.end() != mapIter ; mapIter++)
  {
    string sVar   = (*mapIter).first ;
    string sAlias = (*mapIter).second ;

    sMsg = string("resolveComparison evaluates var \"") + sVar + string("\"") ;
    pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;

    string sModifiers = getQueryPathModifiers(sVar) ;

    string sLocalUnit = sComparisonUnit ;

    size_t posStartUnit = sVar.find("[") ;
    if (string::npos != posStartUnit)
    {
      sLocalUnit = getPathUnit(sVar) ;
      sVar = string(sVar, 0, posStartUnit) ;
    }

    string sConcept = string("") ;

    size_t posSeparator = sVar.find_last_of(cheminSeparationMARK) ;
    if (string::npos != posSeparator)
      sConcept = string(sVar, posSeparator+1, strlen(sVar.c_str())-posSeparator-1) ;
    else
      sConcept = sVar ;

    if (sVar != (*mapIter).first)
    {
      sMsg = string("resolveComparison parsed var \"") + (*mapIter).first + string("\" into var \"") + sVar + string("\" with modifier=\"") + sModifiers + string("\" and unit=\"") + sLocalUnit + string("\"") ;
      pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;
    }

    sMsg = string("resolveComparison selected concept \"") + sConcept + string("\"") ;
    pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;

    string sConversionError = string("") ;

    // First, check if it is a numerical value and not a var
    //
    double dValue = StringToDouble(sVar, &sConversionError) ;

    // If not, "resolve" the path into a value
    //
    if (string("") != sConversionError)
      dValue = getValueForPath(sVar, sLocalUnit, sModifiers, sConcept, &sConversionError, pPatPathoArray) ;

    if (string("") != sConversionError)
    {
      sMsg = string("resolveComparison failed for \"") + sVar + string("\" - aborting") ;
      pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;

      return false ;
    }

    aValues[sAlias] = dValue ;

    sMsg = string("resolveComparison \"") + sVar + string("\" = ") + DoubleToString(&dValue, -1, 2) ;
    pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;
  }

  // Now compare
  //

  double dLResult = calculate(&aValues, sLValue) ;

  sMsg = string("resolveComparison \"") + sLValue + string("\" = ") + DoubleToString(&dLResult, -1, 2) ;
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;

  double dRResult = calculate(&aValues, sRValue) ;

  sMsg = string("resolveComparison \"") + sRValue + string("\" = ") + DoubleToString(&dRResult, -1, 2) ;
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;

  bool bResult = compare(dLResult, sOperator, dRResult) ;

  sMsg = string("resolveComparison returns ") ;
  if (bResult)
    sMsg += string("true") ;
  else
    sMsg += string("false") ;
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trDetails) ;

  return bResult ;
}

double
NSPatientChoisi::getValueForPath(string sPath, string sNeededUnit, string sModifiers, string sConcept, string *psErrorMsg, NSPatPathoArray* pPatPathoArray, HWND interfaceHandle)
{
  if (string("") == sPath)
  {
    if (psErrorMsg)
      *psErrorMsg = string("Empty string") ;
    return (double) 0 ;
  }

  double dValue ;

  if (pPatPathoArray)
  {
    PatPathoIter iterNode ;
    if (pPatPathoArray->CheminDansPatpatho(sPath, string(1, cheminSeparationMARK), &iterNode))
    {
      NSPatPathoArray subPatPatho(pContexte->getSuperviseur()) ;
      pPatPathoArray->ExtrairePatPatho(iterNode, &subPatPatho) ;

      bool bPathFound = subPatPatho.getNumericalValueForUnit(sNeededUnit, &dValue, sConcept) ;

      if (bPathFound)
      {
        if (psErrorMsg)
          *psErrorMsg = string("") ;

        return dValue ;
      }
    }

    // If not found in Ppt but Scope == Global, then look in patient record
    //
    if (string::npos == sModifiers.find("S=G"))
    {
      string sMsg = string("getValueForPath var \"") + sPath + string("\" not found in local array, leaving") ;
      pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trDetails) ;

      if (psErrorMsg)
        *psErrorMsg = string("Not found in local array") ;

      return (double) 0 ;
    }
    else
    {
      string sMsg = string("getValueForPath var not found in local array, searching in patient record") ;
      pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;
    }
  }

  NSPatPathoArray* pSearchPPt = (NSPatPathoArray*) 0 ;

  string answer_date ;
  bool bSuccess = pContexte->getBBinterface()->synchronousCall(sPath, &pSearchPPt, interfaceHandle, &answer_date) ;
  if (bSuccess && (pSearchPPt) && (false == pSearchPPt->empty()))
  {
    bool bPathFound = pSearchPPt->getNumericalValueForUnit(sNeededUnit, &dValue, sConcept) ;

    if (bPathFound)
    {
      if (psErrorMsg)
        *psErrorMsg = string("") ;

      if (pSearchPPt)
        delete pSearchPPt ;

      return dValue ;
    }
  }

  string sMsg = string("getValueForPath var \"") + sPath + string("\" not found in patient record, leaving") ;
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trDetails) ;

  if (pSearchPPt)
    delete pSearchPPt ;

  if (psErrorMsg)
    *psErrorMsg = string("Not found in patient record") ;

  return (double) 0 ;
}

bool
NSPatientChoisi::resolveExistsComparison(string sLValue, string *psErrorMsg, NSPatPathoArray* pPatPathoArray, HWND interfaceHandle)
{
  // Get modifiers
  //
  string sModifiers = getQueryPathModifiers(sLValue) ;

  bool bPathFound = false ;

  // Search in pPatPathoArray
  //
  if (pPatPathoArray)
  {
    PatPathoIter iterNode ;
    bPathFound = pPatPathoArray->CheminDansPatpatho(sLValue, string(1, cheminSeparationMARK), &iterNode) ;
    if (bPathFound)
      return true ;

    // If global scope is NOT specified, then fail
    //
    if (string::npos == sModifiers.find("S=G"))
      return false ;
  }

  // Search in patient file
  //
  NSPatPathoArray* pSearchPPt = (NSPatPathoArray*) 0 ;

  string answer_date ;
  bool bSuccess = pContexte->getBBinterface()->synchronousCall(sLValue, &pSearchPPt, interfaceHandle, &answer_date) ;
  if (bSuccess && (pSearchPPt) && (false == pSearchPPt->empty()))
    bPathFound = true ;

  if (pSearchPPt)
    delete pSearchPPt ;

  return bPathFound ;
}

// We suppose here that elements (other than '(' and ')') are separated by
// spaces: e.g. "var1 + var2" is valid while "var1+var2" is not
//
bool
NSPatientChoisi::initMapForNumericalExpression(string *psFormula, map<string, string> *paAliases, string *psErrorMsg)
{
  if ((NULL == psFormula) || (string("") == *psFormula) || (NULL == paAliases))
    return false ;

  size_t cc    = 0 ;
  size_t iSize = strlen(psFormula->c_str()) ;

  string sCurrentVar = string("") ;

  string sSeparators = string("() \t") ;
  string sOperators  = string("+-*/^£") ;

  // size_t iBlockNb = 1 ;
  size_t iVarStart = 0 ;

  while (cc < iSize)
  {
    // This char is a separator
    //
    if (sSeparators.find((*psFormula)[cc]) != string::npos)
    {
      // If a var was being built, let's treat it
      //
      if (string("") != sCurrentVar)
      {
        string sAlias = replaceVarByAlias(psFormula, paAliases, iVarStart, sCurrentVar) ;

        cc = iVarStart + strlen(sAlias.c_str()) ;
        iSize = strlen(psFormula->c_str()) ;

        sCurrentVar = string("") ;
      }
    }
    // This char is an operator enclosed with 2 separators
    //
    else if ((sOperators.find((*psFormula)[cc]) != string::npos) &&
             (cc > 0) && (cc < iSize - 1) &&
             (sSeparators.find((*psFormula)[cc-1]) != string::npos) &&
             (sSeparators.find((*psFormula)[cc+1]) != string::npos))
    {
    }
    // else, it is a char that belongs to a var
    //
    else
    {
      if (string("") == sCurrentVar)
        iVarStart = cc ;

      sCurrentVar += string(1, (*psFormula)[cc]) ;
    }

    cc++ ;
  }

  if (string("") != sCurrentVar)
    replaceVarByAlias(psFormula, paAliases, iVarStart, sCurrentVar) ;

  return true ;
}


//---------------------------------------------------------------------------
//  bloquer()
//
//  Inscrit le patient comme étant bloqué par cette console
//---------------------------------------------------------------------------
void
NSPatientChoisi::bloquer()
{
try
{
}
catch (...)
{
	erreur("Exception bloquer.", standardError, 0) ;
}
}

//---------------------------------------------------------------------------
//  bloquer()
//
//  Efface le patient du fichier de blocage
//---------------------------------------------------------------------------
void
NSPatientChoisi::debloquer()
{
try
{
	if (false == getADebloquer())
		return ;

  string sMsg = string("Unlocking patient") ;
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;

  NSSuper* pSuper = pContexte->getSuperviseur() ;

	NSBasicAttributeArray BAttrArray ;
  BAttrArray.push_back(new NSBasicAttribute(CONSOLE,	pSuper->getConsoleString())) ;
  BAttrArray.push_back(new NSBasicAttribute(INSTANCE,	pSuper->getInstanceString())) ;
  BAttrArray.push_back(new NSBasicAttribute(PERSON,		getNss())) ;
  if (false == pSuper->getPilot()->unlockPatient(NautilusPilot::SERV_UNLOCK.c_str(), &BAttrArray))
  {
    string sErrorText = pSuper->getText("NTIERS", "unlockError") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;

    return ;
  }

	setADebloquer(false) ;

  sMsg = string("Patient unlocked") ;
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;
}
catch (...)
{
	erreur("Exception debloquer.", standardError, 0) ;
}
}

/*
void
NSPatientChoisi::DetruireDocument(NSDocumentInfo* pDocInfo)
{
try
{
	if (NULL == pDocInfo)
		return ;

	if (getReadOnly())
	{
		string sErrorText = pContexte->getSuperviseur()->getText("privilegeManagement", "patientFileIsReadOnly") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
		return ;
	}

	string sCodeDoc = pDocInfo->getID() ;

	NSLinkManager* pGraphe = pGraphPerson->getLinkManager() ;

  // on rend d'abord le document invisible
  //
  pDocInfo->rendInvisible() ;

  // on doit ré-enregistrer le document Meta pour mettre visible à 0
  //
  NSDocumentInfo* pDocInfoMeta = new NSDocumentInfo(pContexte) ;
  //
  // remise à jour du pDocInfoMeta pour charger le document Meta
  // on précise ici uniquement codePatient et codeDocument sans préciser le type
  // on doit ensuite charger la patpatho à la main (cf NSDocumentInfo::ChargeDocMeta())
	//
  pDocInfoMeta->setPatient(pDocInfo->getPatient()) ;
  string sNewCodeDoc = string(pDocInfo->sCodeDocMeta, PAT_NSS_LEN, DOC_CODE_DOCUM_LEN) ;
  pDocInfoMeta->setDocument(sNewCodeDoc) ;

  // on recharge la patpatho avec les arrays de données
  //
  NSNoyauDocument DocMeta(0, pDocInfoMeta, 0, pContexte, false, true) ;
  bool resultat = DocMeta.chargePatPatho() ;
  if ((false == resultat) || (true == DocMeta.pPatPathoArray->empty()))
  	return ;

  // on met à zéro la donnée visible du noeud racine
  (*(DocMeta.pPatPathoArray->begin()))->setVisible("0") ;
	if (DocMeta.enregistrePatPatho())
		DocMeta.NSNoyauDocument::chargePatPatho() ;  // on remet à jour le pMeta de la pDocInfo  *(pDocInfo->pMeta) = *(DocMeta.pPatPathoArray) ;

  // on supprime le lien personDocument de la partie Meta du document
  string sCodeDocMeta = pDocInfo->sCodeDocMeta ;

  VecteurString aVecteurString ;
  pGraphe->TousLesVrais(sCodeDocMeta, NSRootLink::personDocument, &aVecteurString, "ENVERS") ;
  if (aVecteurString.empty())
	{
    string sErrorText = pContexte->getSuperviseur()->getText("documentManagement", "thisDocumentHasAlreadyBeenDeleted") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), warningError, 0, pContexte->GetMainWindow()->GetHandle()) ;
		return ;
	}

	string sNodeRoot = *(*(aVecteurString.begin())) ;
	pGraphe->detruireLien(sNodeRoot, NSRootLink::personDocument, sCodeDocMeta) ;

	// on détruit le lien sur l'ancienne chemise et on le relie à la corbeille
  //
	aVecteurString.vider() ;
	pGraphe->TousLesVrais(sCodeDocMeta, NSRootLink::docFolder, &aVecteurString, "ENVERS") ;

	if (false == aVecteurString.empty())
	{
		string sNodeChemise = *(*(aVecteurString.begin())) ;
		pGraphe->detruireLien(sNodeChemise, NSRootLink::docFolder, sCodeDocMeta) ;
	}

	if ((NULL == pDocHis) || (NULL == pDocHis->pLibChem) || (NULL == pDocHis->pLibChem->pPatPathoArray))
		return ;
	if (pDocHis->pLibChem->pPatPathoArray->empty())
		return ;

	// On relie à la corbeille
	//
	// Le noeud corbeille est par construction la première chemise
	//
	PatPathoIter iter = pDocHis->pLibChem->pPatPathoArray->begin() ;
	iter++ ;
	if (pDocHis->pLibChem->pPatPathoArray->end() == iter)
		return ;

	string sSens = (*iter)->getLexiqueSens(pContexte->getSuperviseur()) ;
	if (sSens == string("0CHEM"))
	{
		string sNodeCorbeille = (*iter)->getNode() ;
		bool bRet = pGraphe->etablirLien(sNodeCorbeille, NSRootLink::docFolder, sCodeDocMeta) ;
	}
	else
	{
		string sErrorText = pContexte->getSuperviseur()->getText("documentManagement", "cannotFindTrashToPutTheDocumentIntoIt") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), warningError, 0, pContexte->GetMainWindow()->GetHandle()) ;
	}
}
catch (...)
{
	erreur("Exception NSPatientChoisi::DetruireDocument.", standardError, 0) ;
}
}
*/

void
NSPatientChoisi::DetruireDocument(NSDocumentInfo* pDocInfo)
{
try
{
	if ((NSDocumentInfo*) NULL == pDocInfo)
		return ;

  string ps = string("Entering NSPatientChoisi::DetruireDocument") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

	if (getReadOnly())
	{
		string sErrorText = pContexte->getSuperviseur()->getText("privilegeManagement", "patientFileIsReadOnly") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
		return ;
	}

	string sCodeDoc = pDocInfo->getID() ;

	NSLinkManager* pGraphe = _GraphPerson.getLinkManager() ;

  // Removing the "personDocument" links from Root to document's Meta data
  // on supprime le lien personDocument de la partie Meta du document
  //
  string sCodeDocMeta = pDocInfo->getCodeDocMeta() ;

  VecteurString aVecteurString ;
  pGraphe->TousLesVrais(sCodeDocMeta, NSRootLink::personDocument, &aVecteurString, "ENVERS") ;
  if (aVecteurString.empty())
	{
    string sErrorText = pContexte->getSuperviseur()->getText("documentManagement", "thisDocumentHasAlreadyBeenDeleted") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), warningError, 0, pContexte->GetMainWindow()->GetHandle()) ;
		return ;
	}

	string sNodeRoot = *(*(aVecteurString.begin())) ;
	pGraphe->detruireLien(sNodeRoot, NSRootLink::personDocument, sCodeDocMeta) ;

  // Removing the link to previous folder and connectind to Trash folder
	// on détruit le lien sur l'ancienne chemise et on le relie à la corbeille
  //
	aVecteurString.vider() ;
	pGraphe->TousLesVrais(sCodeDocMeta, NSRootLink::docFolder, &aVecteurString, "ENVERS") ;

	if (false == aVecteurString.empty())
	{
		string sNodeChemise = *(*(aVecteurString.begin())) ;
		pGraphe->detruireLien(sNodeChemise, NSRootLink::docFolder, sCodeDocMeta) ;
	}

	if (_pDocHis && _pDocHis->getLibChem())
  {
    NSPatPathoArray LibChemPpt(pContexte->getSuperviseur()) ;
    _pDocHis->getLibChem()->initFromPatPatho(&LibChemPpt) ;

    if (false == LibChemPpt.empty())
    {
      // Connecting to Trash folder
      // On relie à la corbeille
      //
      // Trash is the first folder
      // Le noeud corbeille est par construction la première chemise
      //
      PatPathoIter iter = LibChemPpt.begin() ;
      iter++ ;
      if (LibChemPpt.end() != iter)
      {
        string sSens = (*iter)->getLexiqueSens() ;
        if (sSens == string("0CHEM"))
        {
          string sNodeCorbeille = (*iter)->getNode() ;
          /* bool bRet = */ pGraphe->etablirLien(sNodeCorbeille, NSRootLink::docFolder, sCodeDocMeta) ;
        }
        else
        {
          string sErrorText = pContexte->getSuperviseur()->getText("documentManagement", "cannotFindTrashToPutTheDocumentIntoIt") ;
          pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
          erreur(sErrorText.c_str(), warningError, 0, pContexte->GetMainWindow()->GetHandle()) ;
        }
      }
    }
  }

  // Setting Meta's root node visibility status to 0
  // on doit ré-enregistrer le document Meta pour mettre visible à 0
  //
  string sRights ;
  NSPatPathoArray PptMeta(pContexte->getSuperviseur()) ;
  if ((false == _GraphPerson.getTree(sCodeDocMeta, &PptMeta, &sRights)) || (PptMeta.empty()))
  {
    string sErrorText = pContexte->getSuperviseur()->getText("documentManagementErrors", "cannotAccessDocumentMetaData") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
    erreur(sErrorText.c_str(), warningError, 0, pContexte->GetMainWindow()->GetHandle()) ;
    return ;
  }

  // on met à zéro la donnée visible du noeud racine
  (*(PptMeta.begin()))->setVisible("0") ;

  _GraphPerson.setTree(&PptMeta, sRights, sCodeDocMeta) ;
  if (false == _GraphPerson.commitGraphTree(pContexte, sCodeDocMeta))
  {
    string sErrorText = pContexte->getSuperviseur()->getText("documentManagementErrors", "failedToSave") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
    erreur(sErrorText.c_str(), warningError, 0, pContexte->GetMainWindow()->GetHandle()) ;
    return ;
  }

  // on remet à jour le pMeta de la pDocInfo
  pDocInfo->setMeta(&PptMeta) ;

  // on rend d'abord le document invisible
  //
  pDocInfo->rendInvisible() ;

	// Fonction d'exportation
	//
  string sExportDll = pContexte->getSuperviseur()->getExportDll() ;

	if (string("") != sExportDll)
	{
    NSDocStatus docStatus ;
    docStatus.setStatus(NSDocStatus::StatusDeleted) ;

    string ps = string("Export for deleted using dll ") + sExportDll ;
    pContexte->getSuperviseur()->trace(&ps, 1) ;

		TModule* pDCModule = new TModule(sExportDll.c_str(), TRUE) ;
		if ((TModule*) NULL == pDCModule)
		{
			string sErreur = "Impossible d'ouvrir la DLL d'exportation : " + sExportDll ;
			erreur(sErreur.c_str(), standardError, -1, pContexte->GetMainWindow()->GetHandle()) ;
			pContexte->getSuperviseur()->afficheStatusMessage(string("")) ;
			return ;
		}

    bool (FAR *pAdresseFct) (NSContexte far *, NSDocumentInfo far *, NSDocStatus far *) ;
		(FARPROC) pAdresseFct = pDCModule->GetProcAddress(MAKEINTRESOURCE(1)) ;
		if (NULL == pAdresseFct)
		{
      string sErreur = "Impossible d'accéder à la méthode deletedDoc de la DLL d'exportation : " + sExportDll ;
      pContexte->getSuperviseur()->trace(&sErreur, 1) ;
			erreur(sErreur.c_str(), standardError, -1, pContexte->GetMainWindow()->GetHandle()) ;

			delete pDCModule ;
			return ;
		}

		/* bool bReussi = */ ((*pAdresseFct)((NSContexte far *)   pContexte,
                                         (NSDocumentInfo far *) pDocInfo,
                                         (NSDocStatus far *)  &docStatus)) ;

		delete pDCModule ;
  }
}
catch (...)
{
	erreur("Exception NSPatientChoisi::DetruireDocument.", standardError, 0) ;
}
}

//
// Build an XML file from document's information
//
void
NSPatientChoisi::DocumentXmlExport(NSDocumentHisto* pDocHisto)
{
	if ((NSDocumentHisto*) NULL == pDocHisto)
		return ;

  string ps = string("Entering NSPatientChoisi::DocumentXmlExport") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

  string sXmlContent = string("<Document>\r\n") ;

  NSPatPathoArray PptMeta(pContexte->getSuperviseur()) ;
  pDocHisto->initFromMeta(&PptMeta) ;
  sXmlContent += string("  <LabelTree>\r\n") ;
  sXmlContent += PptMeta.genereXML() ;
  sXmlContent += string("  </LabelTree>\r\n") ;

  NSPatPathoArray DocHistoPpt(pContexte->getSuperviseur()) ;
  pDocHisto->initFromPatPatho(&DocHistoPpt) ;
  if (false == DocHistoPpt.empty())
  {
    sXmlContent += string("  <DataTree>\r\n") ;
    sXmlContent += DocHistoPpt.genereXML() ;
    sXmlContent += string("  </DataTree>\r\n") ;
  }

  sXmlContent += string("</Document>\r\n") ;

  string sExportPath = pContexte->PathName("EHTM") ;
  string sDocId      = pDocHisto->getID() ;
  string sFileName   = sExportPath + nomSansDoublons(sExportPath, sDocId, string("xml")) ;

  ofstream outFile ;
  outFile.open(sFileName.c_str()) ;
	if (!outFile)
	{
    string sErrorText = pContexte->getSuperviseur()->getText("fileErrors", "errorOpeningOutputFile") + string(" ") + sFileName ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError, 0) ;
    return ;
	}

  outFile << sXmlContent ;

  outFile.close() ;

  string sMessage = pContexte->getSuperviseur()->getText("exportManagement", "ExportSucceededInFile") + string(" ") + sFileName ;
  string sCaption = pContexte->getSuperviseur()->getAppName().c_str();
  ::MessageBox(pContexte->GetMainWindow()->GetHandle(), sMessage.c_str(), sCaption.c_str(), MB_OK);

  ps = string("Leaving NSPatientChoisi::DocumentXmlExport") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;
}

bool
NSPatientChoisi::updateIPPEnCours(string sSite, string sNewIpp)
{
  string ps = string("Entering NSPatientChoisi::updateIPPEnCours with IPP=") + sNewIpp ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

	if (string("") == sNewIpp)
		return false ;

	NSPatPathoArray pptIdent(pContexte->getSuperviseur()) ;
	bool bGetPpt = _GraphPerson.getLibIDsPpt(&pptIdent) ;

	// Does the IDs Library exist?
  //
	if (false == bGetPpt)
	{
  	NSDataGraph* pGraph = _GraphPerson.getDataGraph() ;
    NSDataTreeIter iterTree ;
    if (false == pGraph->getTrees()->ExisteTree("0LIBI1", pContexte->getSuperviseur(), &iterTree))
    {
      ps = string("NSPatientChoisi::updateIPPEnCours creating an Identifiers library") ;
      pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

    	// Create an Identifiers Library
      //
    	bool bSuccess = CreeRootLibIDs() ;
      if (false == bSuccess)
      {
      	string sErrorText = pContexte->getSuperviseur()->getText("identifiersManagement", "cannotCreateIdentifiersLibrary") ;
    		pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
				erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
      	return false ;
      }

      bool bGetNewPpt = _GraphPerson.getLibIDsPpt(&pptIdent) ;
      if (false == bGetNewPpt)
      {
      	string sErrorText = pContexte->getSuperviseur()->getText("identifiersManagement", "cannotAccessIdentifiersLibrary") ;
    		pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
				erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
      	return false ;
      }
    }
  }

  string sCurrentIpp ;
  bool bExistIPP = _GraphPerson.IPPEnCours(&pptIdent, sSite, &sCurrentIpp) ;
  if ((true == bExistIPP) && (sCurrentIpp == sNewIpp))
  {
    ps = string("NSPatientChoisi::updateIPPEnCours IPP \"") + sCurrentIpp + string("\" already exist for site \"") + sSite + string("\" - leaving.") ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;
		return true ;
  }

	if (true == bExistIPP)
  {
    ps = string("NSPatientChoisi::updateIPPEnCours Closing IPP \"") + sCurrentIpp + string("\" for site \"") + sSite + string("\".") ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;
		_GraphPerson.ClotureIPP(&pptIdent, sSite, sCurrentIpp) ;
  }

	// Adding the new identifier for this site
  //
  ps = string("NSPatientChoisi::updateIPPEnCours Adding IPP \"") + sCurrentIpp + string("\" for site \"") + sSite + string("\" to the identifiers library.") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

  NSPatPathoArray pptIdLib(pContexte->getSuperviseur()) ;
  pptIdLib.ajoutePatho("LIPP01", 0) ;

  Message Msg ;
  Msg.SetTexteLibre(sNewIpp) ;
  pptIdLib.ajoutePatho("£CL000", &Msg, 1) ;

  pptIdLib.ajoutePatho("KOUVR1", 1) ;

  NVLdVTemps tpsNow ;
  tpsNow.takeTime() ;

  Msg.Reset() ;
  Msg.SetComplement(tpsNow.donneDateHeure()) ;
  Msg.SetUnit("2DA021") ;
  pptIdLib.ajoutePatho("£T0;19", &Msg, 2) ;

	bool bGraphUpdate = _GraphPerson.InsereIPPFusion(&pptIdent, sSite, &pptIdLib) ;

  if (false == bGraphUpdate)
  {
  	string sErrorText = pContexte->getSuperviseur()->getText("identifiersManagement", "cannotUpdateIdentifiersLibrary") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
    erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
    return false ;
  }

	// Time to save
  //
	NSBasicAttributeArray *pAttrList = _GraphPerson.getAttrArray() ;
  pAttrList->changeAttributeValue(IPP, sNewIpp) ;

	NSVectPatPathoArray VectUpdate ;
	string sTreeID      = string("") ;
  string sCodeDocMeta = string("") ;
	VectUpdate.push_back(new NSPatPathoArray(pptIdent)) ;

	_GraphPerson.updateTrees(pContexte, &VectUpdate, &sTreeID, &sCodeDocMeta, pidsPatient) ;

	if (string("") == sTreeID)
	{
  	string sErrorText = pContexte->getSuperviseur()->getText("documentManagementErrors", "failedToSave") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError, 0, 0) ;
		return false ;
	}

  return true ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSPatientChoisi::operator=(NSPatInfo src)
//  Description:	Opérateur d'affectation
//  Retour:			Référence de l'objet cible
//---------------------------------------------------------------------------
NSPatientChoisi&
NSPatientChoisi::operator=(const NSPatInfo& src)
{
	if (this == &src)
  	return *this ;

	_Donnees 	    = *(src.getConstData()) ;
	_CorrespArray = src._CorrespArray ;
	pContexte 	  = src.pContexte ;

  _GraphPerson  = *(src.getConstGraphPerson()) ;
  _GraphAdr     = src._GraphAdr ;
  _sChez        = src._sChez ;

  if (_pDocHis)
  {
    delete _pDocHis ;
    _pDocHis = 0 ;
  }
  if (src.getConstDocHis())
    _pDocHis = new NSHISTODocument(*(src.getConstDocHis())) ;

	return *this ;
}


//***************************************************************************
//
// Implémentation des méthodes NSUtilisateurChoisi
//
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur à partir d'un NSSuper*
//---------------------------------------------------------------------------
NSUtilisateurChoisi::NSUtilisateurChoisi(NSContexte* pCtx)
                    :NSUtiliInfo(pCtx), _aWinProp(pCtx)
{
	_pPatRech      = (NSPatInfo*) 0 ;
	_pDocCompta    = new NSDocCompta ;
	_pDocRefHisto  = (NSHistoRefDocument*) 0 ;
	_pMail         = new NSMailParams ;
  _pMailApicrypt = new NSMailParams ;
	_pRequete      = (NSRequeteDialog*)   0 ;
	_pResult       = (NSResultReqDialog*) 0 ;
  _pReqRefDoc    = (NSReqRefDocument*)  0 ;
  _bEnregWin     = false ;

  _progressDialog       = (NSProgressAndMessageDlg*) 0 ;
  _iPatientStatus       = psUndefined ;
  _iCurrentClosingStep  = cpsUndefined ;
  _pToDoAfterClosingPat = (NSToDoTask*) 0 ;
}

//---------------------------------------------------------------------------
//  Constructeur à partir de dataGraph et login + password
//---------------------------------------------------------------------------
/*
NSUtilisateurChoisi::NSUtilisateurChoisi(NSContexte* pCtx, NSDataGraph *pDataGraph)
                    :NSUtiliInfo(pCtx), aWinProp(pCtx)
{
	pPatRech      = 0 ;
	pDocCompta    = new NSDocCompta ;
	pDocRefHisto  = 0 ;
	pMail         = new NSMailParams ;
  pMailApicrypt = new NSMailParams ;
	pRequete      = 0 ;
	pResult       = 0 ;
  bEnregWin     = false ;

	_GraphPerson.setDataGraph(pDataGraph) ;
}
*/

//---------------------------------------------------------------------------
//      Destructeur
//---------------------------------------------------------------------------
NSUtilisateurChoisi::~NSUtilisateurChoisi()
{
try
{
	// Ces documents sont détruits directement par la VisualView
	// en fin d'impression...
	// delete pDocCompta;
	// if (pDocRefHisto)
	//  	delete pDocRefHisto;

  bool bWasActiveUser = (pContexte->getUtilisateur() == this) ;

	//
	// A faire avant la fermeture de la transaction
	//
	if (bWasActiveUser && pContexte->getPatient())
  {
    /* bool bOkToClosePatient = */ fermePatEnCours() ;
  }

	//
	// Fermeture de la transaction
	//
	// NSTransaction Transa(pContexte) ;
	// Transa.fermeTransaction() ;

	if (_pMail)
		delete _pMail ;
  if (_pMailApicrypt)
    delete _pMailApicrypt ;

	//
	// Destruction du patient en cours
	//

#ifndef __EPIPUMP__
#ifndef __EPIBRAIN__
	if (bWasActiveUser && (pContexte->getPatient() == 0))
		if (pContexte->getSuperviseur()->getApplication()->GetMainWindow())
			pContexte->getSuperviseur()->getApplication()->prendClient()->CloseChildrenButServicesWindows(pContexte) ;
#endif
#endif

	if (_pPatRech)
		delete _pPatRech ;

  if (_pDocCompta)
    delete _pDocCompta ;

  if (_pToDoAfterClosingPat)
    delete _pToDoAfterClosingPat ;

  if (_pResult)
    delete _pResult ;

  if (_pRequete)
    delete _pRequete ;
}
catch (...)
{
	erreur("Exception NSUtilisateurChoisi destructor", standardError, 0) ;
}
}

//---------------------------------------------------------------------------
//  Fonction :		NSUtilisateurChoisi::SetUtilisateurChoisi()
//
//  Description :	Met à jour les variables de l'utilisateur.
//---------------------------------------------------------------------------
void
NSUtilisateurChoisi::SetUtilisateurChoisi(NSBasicAttributeArray *pAttrArray)
{
	if (NULL == pAttrArray)
		return ;

  _sLogin                = pAttrArray->getAttributeValue(LOGIN) ;
  _sPasswd               = pAttrArray->getAttributeValue(PASSWORD) ;
  _sUserType             = pAttrArray->getAttributeValue(USERTYPE) ;
  _sPasswordType         = pAttrArray->getAttributeValue(PASSWDTYPE) ;
  _sDatePasswordCreation = pAttrArray->getAttributeValue(STARTDATE) ;
  _sValidityDuration     = pAttrArray->getAttributeValue(DAYSNB) ;

  // string sIdRoot = pGraphPerson->pDataGraph->getGraphID() ;
  // string sPids = sIdRoot.substr(0, PAT_NSS_LEN) ;
  
  string sPids = pAttrArray->getAttributeValue(PIDS) ;
  setNss(sPids) ;
  _GraphPerson.setAttributeValue(PIDS, sPids) ;

  // pGraphPerson->setRootTree(sIdRoot) ; If root tree is set, then GetGraphUtil will not load the graph
  _GraphPerson.setRootTree(string("")) ;

	string ps = string("Initializing windows position") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

  // Initialisation des positions des fenetres
	_aWinProp.initWindsChild(getNss(), pContexte->PathName("FGLO")) ;
  //
  // Setting user's Hot Keys
  //
  /*
  if (false == aWinProp.empty())
	{
		ArrayWPIter iter = aWinProp.begin() ;
  	for ( ; aWinProp.end() != iter ; iter++)
    {
    	if (string("") != (*iter)->getModifiers())
      {
      }
    }
	}
  */

  ps = string("Getting user's graph") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

  GetGraphUtil(this) ;
}

//---------------------------------------------------------------------------
//  Fonction :		NSUtilisateurChoisi::AppelPatient()
//
//  Description :	Crée le patient principal.
//---------------------------------------------------------------------------
void
NSUtilisateurChoisi::AppelPatient()
{
try
{
  NSPilotProxyWindow* pPilotProxy = pContexte->getSuperviseur()->getPilotProxyWindow() ;
  if (pPilotProxy)
    pPilotProxy->Hide() ;

#ifndef __EPIPUMP__
	string sNom       = string("") ;
	string sPrenom    = string("") ;
	string sPatronyme = string("") ;

	// char Nom[PAT_NOM_LEN + 1] = "" ;
	// char Prenom[PAT_PRENOM_LEN + 1] = "" ;
	// char NSS[PAT_NSS_LEN + 1] = "";
	// char Code[PAT_CODE_LEN + 1] = "";
	int  idRetour ;

	// Existe-t-il des données de capture Episodus ?

	if (pContexte->getEpisodus())
	{
		NSCaptureArray* pCapt = &(pContexte->getEpisodus()->newCaptureArray) ;
    pCapt->getIdentity(sNom, sPrenom) ;
	}

	//
	// Création de l'objet patient
	//
  if ((NSPatInfo*) NULL == _pPatRech)
  	_pPatRech = new NSPatInfo(pContexte) ;

	// Note : En principe on execute ici en fonction du résultat
	// de la recherche qui sera stocké dans pPatRech (NSPatInfo*)
  //
	if (false == pContexte->isClientGroup())
	{
		NSNTiersRechPatDialog* pPatDlg = new NSNTiersRechPatDialog(pContexte->GetMainWindow(), pContexte, _pPatRech) ;
		idRetour = pPatDlg->Execute() ;
    delete pPatDlg ;
	}
	else
	{
		NSListeClientGroupDialog* pPatDlgGroup = new NSListeClientGroupDialog(pContexte->GetMainWindow(), pContexte, _pPatRech) ;
		idRetour = pPatDlgGroup->Execute() ;
    // iGroupMode = pPatDlgGroup->mode ;
    delete pPatDlgGroup ;
	}

  if (IDOK == idRetour)
  {
    if (_pPatRech)
      OuvrePatient(_pPatRech->getNss()) ;
  }
#endif // !__EPIPUMP__
}
catch (...)
{
	erreur("Exception NSUtilisateurChoisi::AppelPatient().", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------
// Function     : NSUtilisateurChoisi::OuvrePatient(string sNss)
// Description  : Ouvre un nouveau patient de code sNss et ferme le patient en
//                cours s'il existe
// -----------------------------------------------------------------------------
bool
NSUtilisateurChoisi::OuvrePatient(string sNss, NSPatientChoisi::OPENPATNEXT iWhatToDoNext)
{
try
{
	// Création de l'objet patient
  if ((NSPatInfo*) NULL == _pPatRech)
    _pPatRech = new NSPatInfo(pContexte) ;

  // N.B. En mode MUE le fermeTransaction() est fait dans fermePatEnCours()
	// car la transaction dépend du patient
	if (pContexte->getPatient())
  {
		// bSwitcher = fermePatEnCours() ;
    if (false == CanClosePatient())
      return false ;

    delete _pPatRech ;
    _pPatRech = new NSPatInfo(pContexte) ;
    _pPatRech->setNss(sNss) ;

    ClosePatientAsynchronous(cpnOpenNextPat) ;

    return true ;
  }
  else
    //
    // Take care that it remains no data from previous patient
    //
    _pPatRech->getData()->init() ;

  bool bCanOpenPat = OuvrePatient(sNss, _pPatRech) ;
  if (false == bCanOpenPat)
    return false ;

  bool bSwitchPat = ChangePatientChoisi() ;
  if (false == bSwitchPat)
    return false ;

  pContexte->getPatient()->InitialisationAsynchronous(iWhatToDoNext) ;

  return true ;
}
catch (...)
{
	erreur("Exception NSUtilisateurChoisi::OuvrePatient.", standardError, 0) ;
	return false ;
}
}

bool
NSUtilisateurChoisi::OuvrePatient(string sNss, NSPatInfo *pTargetPat)
{
  if ((NSPatInfo*) NULL == pTargetPat)
    return false ;

  return pTargetPat->open(sNss, true) ;

/*
try
{
  NSSuper* pSuper = pContexte->getSuperviseur() ;

	NSBasicAttributeArray LocalAttrArray ;
	LocalAttrArray.push_back(new NSBasicAttribute(PERSON,   sNss)) ;
	LocalAttrArray.push_back(new NSBasicAttribute(OPERATOR, pContexte->getUtilisateurID())) ;
  LocalAttrArray.push_back(new NSBasicAttribute(CONSOLE,  pSuper->getConsoleString())) ;
  LocalAttrArray.push_back(new NSBasicAttribute(INSTANCE, pSuper->getInstanceString())) ;

  NSPersonsAttributesArray PatientList ;

  pContexte->GetMainWindow()->SetCursor(0, IDC_WAIT) ;

  string ps = string("Calling Pilot service \"searchPatient\"") ;
  pSuper->trace(&ps, 1, NSSuper::trDetails) ;

	// bool res = pSuper->getPilot()->searchPatient(NautilusPilot::SERV_OPEN_PATIENT_DATA_FROM_TRAITS.c_str(), pPatRech->getGraphPerson()->getDataGraph(), &PatientList, &LocalAttrArray) ;
  bool res = pSuper->getPilot()->searchPatient(NautilusPilot::SERV_SEARCH_PATIENT.c_str(), pTargetPat->getGraphPerson()->getDataGraph(), &PatientList, &LocalAttrArray) ;

	if (false == res)
	{
  	pContexte->GetMainWindow()->SetCursor(0, IDC_ARROW) ;
  	string tempMessage = pSuper->getPilot()->getWarningMessage() ;
    string tempError   = pSuper->getPilot()->getErrorMessage() ;
    if (string("") != tempMessage)
    {
      pSuper->trace(&tempMessage, 1, NSSuper::trWarning) ;
    	::MessageBox(pContexte->GetMainWindow()->GetHandle(), tempMessage.c_str(), "Message Nautilus", MB_OK) ;
    }
    if (string("") != tempError)
    {
      pSuper->trace(&tempError, 1, NSSuper::trError) ;
    	::MessageBox(pContexte->GetMainWindow()->GetHandle(), tempError.c_str(), "Message Nautilus", MB_OK) ;
    }
    return false ;
	}

  ps = string("Pilot service \"searchPatient\" ended") ;
  pSuper->trace(&ps, 1, NSSuper::trDetails) ;

  // Check that patient graph is not empty and valid
  //
  if ((NULL == pTargetPat->getGraphPerson()->getDataGraph()) ||
    	(false == pTargetPat->getGraphPerson()->graphPrepare()))
  {
    if (NULL == pTargetPat->getGraphPerson()->getDataGraph())
      ps = string("Empty graph.") ;
    else
      ps = string("Failure of the graphPrepare function.") ;
    pSuper->trace(&ps, 1, NSSuper::trWarning) ;
		return false ;
  }

  ps = string("Patient's graph prepared") ;
  pSuper->trace(&ps, 1, NSSuper::trDetails) ;

  // Address "locking" and "read only" issues
  //
  pTargetPat->getGraphPerson()->setNeedUnlock(false) ;
	pTargetPat->getGraphPerson()->setReadOnly(true) ;

  if (false == PatientList.empty())
	{
		string sIsLocked = PatientList.getAttributeValue(TO_UNLOCK) ;
		if (string("ok") == sIsLocked)
			pTargetPat->getGraphPerson()->setNeedUnlock(true) ;
		string sOperationType	= PatientList.getAttributeValue(OPERATION_TYPE) ;
		if (string("readWrite") == sOperationType)
			pTargetPat->getGraphPerson()->setReadOnly(false) ;
	}

  if (true == pTargetPat->getGraphPerson()->getReadOnly())
  {
    string sLockingConsoleId = PatientList.getAttributeValue(LOCKED) ;
    string sWarnText = string("") ;
		if (string("") != sLockingConsoleId)
      sWarnText = pSuper->getText("privilegeManagement", "patientFileIsAlreadyLocked") + string(" (") + sLockingConsoleId + string(")") ;
    else
      sWarnText = pSuper->getText("privilegeManagement", "patientFileIsReadOnly") ;

    string sCaption = pSuper->getAppName() ;
    ::MessageBox(pContexte->GetMainWindow()->GetHandle(), sWarnText.c_str(), sCaption.c_str(), MB_OK) ;
  }

  ps = string("Locking and read-only issues adressed") ;
  pSuper->trace(&ps, 1, NSSuper::trDetails) ;

	pTargetPat->getGraphPerson()->setInfoPids(&LocalAttrArray) ;
	pTargetPat->getGraphPerson()->getDataGraph()->setLastTree() ;

	pContexte->GetMainWindow()->SetCursor(0, IDC_ARROW) ;

  return true ;
}
catch (...)
{
	erreur("Exception NSUtilisateurChoisi::OuvrePatient.", standardError, 0) ;
	return false ;
}
*/
}

//------------------------------------------------------------------------------
//  Fonction :		NSUtilisateurChoisi::ChangePatientChoisi()
//
//  Description :	Ferme le patient principal et reprend pPatRech comme patient
//------------------------------------------------------------------------------
bool
NSUtilisateurChoisi::ChangePatientChoisi()
{
try
{
	// bool bSwitcher = true ;

/*
	// N.B. En mode MUE le fermeTransaction() est fait dans fermePatEnCours()
	// car la transaction dépend du patient
	if (pContexte->getPatient())
		bSwitcher = fermePatEnCours() ;
*/

	// if (bSwitcher)
	// {
    //set patient actif
    pContexte->setPatient(new NSPatientChoisi(_pPatRech)) ;
    NSDataGraph* pGraph = _pPatRech->getGraphPerson()->getDataGraph() ;
    string sIdRoot = pGraph->getGraphID() ;
    NSPatientChoisi* pPat = pContexte->getPatient() ;
    pPat->setNss(string(sIdRoot, 0, PAT_NSS_LEN)) ;
    pPat->getGraphPerson()->setRootTree(sIdRoot) ;
    // Temporaire

    // string sLastTree = pContexte->getPatient()->pGraphPerson->pDataGraph->getLastTree();
	// }

  // return bSwitcher ;
  return true ;
}
catch (...)
{
	erreur("Exception NSUtilisateurChoisi::ChangePatientChoisi().", standardError, 0) ;
  return false ;
}
}

//---------------------------------------------------------------------------
//  Fonction :		NSUtilisateurChoisi::fermePatEnCours()
//
//  Description :	Ferme le patient principal.
//---------------------------------------------------------------------------
bool
NSUtilisateurChoisi::fermePatEnCours(bool bWithSynchro)
{
  NSProgressAndMessageDlg progressDialog(pContexte->GetMainWindow(), pContexte) ;
  progressDialog.Create() ;
  progressDialog.ShowWindow(SW_SHOW) ;

	pContexte->getSuperviseur()->PurgeImpression() ;

	if (pContexte->getPatient())
	{
#ifndef __EPIPUMP__
		pContexte->getEpisodus()->PatChanged() ;
# ifndef __EPIBRAIN__
		pContexte->getPredi()->PatChanged() ;
# endif // !__EPIBRAIN__
#endif // !__EPIPUMP__
	}

  TMyApp *pApplication = pContexte->getSuperviseur()->getApplication() ;

	if (pApplication && pApplication->GetMainWindow())
	{
#ifndef __EPIPUMP__
# ifndef __EPIBRAIN__
    string sMsg = string("Closing all windows but history and services windows") ;
    pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSteps) ;
    progressDialog.setText(sMsg) ;

    bool bAllClosed = pApplication->prendClient()->CloseChildrenButServicesWindows(pContexte) ;

    // If close was cancelled, return false
    if (false == bAllClosed)
    {
      sMsg = string("Some windows didn't close ; stopping patient closing") ;
      pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSteps) ;
      return false ;
    }

    progressDialog.setProgress(25) ;

    // cas de fermeture historique (autre cas CmExit() dans nautilus.cpp)
    if (pContexte->getPatient())
    {
		  pContexte->getPatient()->_bCanCloseHisto = true ;

      sMsg = string("Closing history window") ;
      pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSteps) ;
      progressDialog.setText(sMsg) ;
    }

		bAllClosed = pApplication->prendClient()->CloseChildrenButServicesWindows(pContexte) ;

    // If close was cancelled, return false
    if (false == bAllClosed)
    {
      sMsg = string("Some windows didn't close ; stopping patient closing") ;
      pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSteps) ;
      return false ;
    }

    // NSHistoryClosingDlg waitDialog(pContexte->GetMainWindow(), pContexte) ;
    // waitDialog.Execute() ;

    progressDialog.setProgress(45) ;

    sMsg = string("All windows closed ; closing stripes") ;
    pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSteps) ;

    progressDialog.setText(sMsg) ;

    pApplication->getFrame()->CloseStripes() ;

    progressDialog.setProgress(50) ;

    sMsg = string("Calling garbage collector for MDI children") ;
    pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSteps) ;

    pApplication->prendClient()->MdiChildrenGarbageCollector() ;

# endif // !__EPIBRAIN__
#endif // !__EPIPUMP__
	}

	if (pContexte->getPatient())
	{
		// enregistrement des documents contenant des liens en attente
		NSPersonGraphManager* pGraphManager = pContexte->getPatient()->getGraphPerson() ;
		string sNextDoc = pGraphManager->getLinkManager()->nextDocToSave() ;
		while (string("") != sNextDoc)
		{
      string sTrace = string("Recording document with waiting link: ") + sNextDoc ;
      pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubSteps) ;

      progressDialog.setText(sTrace) ;

			pGraphManager->commitGraphTree(pContexte, sNextDoc) ;

      progressDialog.setProgress(60) ;

      // We must prevent an infinite loop if the document can't be saved
      //
			string sNewNextDoc = pGraphManager->getLinkManager()->nextDocToSave() ;
      if (sNewNextDoc == sNextDoc)
        break ;
      else
        sNextDoc = sNewNextDoc ;
		}

		// pContexte->getSuperviseur()->pEpisodus->PatChanged() ;
		// pContexte->getSuperviseur()->pDPIO->PatChanged() ;
		// on ferme la contribution juste avant de fermer la transaction
    string sTrace = string("Closing contribution") ;
    pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubSteps) ;

    progressDialog.setText(sTrace) ;

		pContexte->getPatient()->FermeContribution() ;

    progressDialog.setProgress(90) ;

    if (pContexte->isClientGroup() && !pContexte->isGroupInterfaceOnly())
    {
      synchroImportThisPat(_sLogin, _sPasswd) ;
      synchroExportThisPat(_sLogin, _sPasswd) ;
    }

    pContexte->getUtilisateur()->setEnregWin(false) ;

    // pour la synchro on ne delete pas le patient en cours
    // pour pouvoir le synchroniser et le réouvrir.
    if (false == bWithSynchro)
    {
      sTrace = string("Deleting patient") ;
      pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubSteps) ;

      progressDialog.setText(sTrace) ;

      NSPatientChoisi* pPat = pContexte->getPatient() ;
      pContexte->setPatient((NSPatientChoisi*) 0) ;
      if (pPat)
        delete pPat ;

      // on rétablit le titre de l'application sans les infos patient
      pContexte->setMainCaption() ;
    }
	}

  progressDialog.setProgress(100) ;

	return true ;
}

bool
NSUtilisateurChoisi::CanClosePatient()
{
  string sMsg = string("Entering CanClosePatient") ;
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSteps) ;

  TMyApp *pApplication = pContexte->getSuperviseur()->getApplication() ;

	if (pApplication && pApplication->GetMainWindow())
	{
#ifndef __EPIPUMP__
# ifndef __EPIBRAIN__
    sMsg = string("CanClosePatient: Checking if all windows, but history and services windows, can close") ;
    pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trDetails) ;

    bool bAllClosed = pApplication->prendClient()->CanCloseChildrenButServicesWindows(pContexte) ;

    // If close was cancelled, return false
    if (false == bAllClosed)
    {
      sMsg = string("CanClosePatient: Some windows didn't close ; stopping patient closing") ;
      pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSteps) ;
      return false ;
    }
    else
    {
      sMsg = string("CanClosePatient returns Ok") ;
      pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubSteps) ;

      return true ;
    }
# endif
#endif
  }

  sMsg = string("CanClosePatient returns False") ;
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSteps) ;

  return false ;
}

void
NSUtilisateurChoisi::ClosePatientAsynchronous(CLOSEPATNEXT iWhatToDoNext)
{
  string sMsg = string("Entering ClosePatientAsynchronous") ;
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSteps) ;

  if (pContexte->getPatient())
    pContexte->getPatient()->switchToClosingStatus() ;

  TMyApp *pApplication = pContexte->getSuperviseur()->getApplication() ;

	if (pApplication && pApplication->GetMainWindow())
	{
    string sMsg = string("Closing all windows but history and services windows") ;
    pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSteps) ;

    bool bAllClosed = pApplication->prendClient()->CloseChildrenButServicesWindows(pContexte) ;

    // If close was cancelled, return false
    if (false == bAllClosed)
    {
      sMsg = string("Some windows didn't close ; stopping patient closing") ;
      pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSteps) ;
    }

    pApplication->prendClient()->MdiChildrenGarbageCollector() ;
  }

  _iPatientStatus        = psClosing ;
  _iWhatToDoAfterClosing = iWhatToDoNext ;
  _iCurrentClosingStep   = cpsUndefined ;

  NSToDoTask *pToDoTask = new NSToDoTask ;
  pToDoTask->setWhatToDo(string("ClosePatientNextStep")) ;
  pToDoTask->sendMe(pContexte->getSuperviseur()) ;
}

void
NSUtilisateurChoisi::ClosePatientNextStep()
{
  switch(_iCurrentClosingStep)
  {
    case cpsUndefined :
      ClosePatientStart() ;
      break ;
    case cpsStart :
      ClosePatientCloseEpisodus() ;
      break ;
    case cpsCloseEpisodus :
      ClosePatientClosePredi() ;
      break ;
    case cpsClosePredi :
      ClosePatientCloseHisto() ;
      break ;
    case cpsCloseHisto :
      ClosePatientCloseHistoCheck() ;
      break ;
    case cpsCloseHistoCheck :
      ClosePatientCloseStrips() ;
      break ;
    case cpsCloseStrips :
      ClosePatientGarbageCollect() ;
      break ;
    case cpsGarbageCollector :
      ClosePatientConnectIrresolvedLinks() ;
      break ;
    case cpsConnectIrresolvedLinks :
      ClosePatientCloseContribution() ;
      break ;
    case cpsCloseContribution :
      ClosePatientEnd() ;
      break ;
  }
}

void
NSUtilisateurChoisi::ClosePatientContinue()
{
  NSToDoTask *pToDoTask = new NSToDoTask ;
  pToDoTask->setWhatToDo(string("ClosePatientNextStep")) ;
  pToDoTask->sendMe(pContexte->getSuperviseur()) ;
}

void
NSUtilisateurChoisi::ClosePatientStart()
{
  _iCurrentClosingStep = cpsStart ;

  if (_progressDialog)
    delete _progressDialog ;

  _progressDialog = new NSProgressAndMessageDlg(pContexte->GetMainWindow(), pContexte) ;
  _progressDialog->Create() ;
  _progressDialog->SetCaption("Fermeture du patient") ;
  _progressDialog->ShowWindow(SW_SHOW) ;

  ClosePatientContinue() ;
}

void
NSUtilisateurChoisi::ClosePatientCloseEpisodus()
{
  _iCurrentClosingStep = cpsCloseEpisodus ;

  string sTrace = string("Calling PatChanged() for Episodus") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubSteps) ;

  if (pContexte->getPatient())
	{
#ifndef __EPIPUMP__
    _progressDialog->setText(string("Avis de fermeture à Episodus")) ;

		pContexte->getEpisodus()->PatChanged() ;
#endif
  }

  _progressDialog->setProgress(2) ;

  ClosePatientContinue() ;
}

void
NSUtilisateurChoisi::ClosePatientClosePredi()
{
  _iCurrentClosingStep = cpsClosePredi ;

  string sTrace = string("Calling PatChanged() for PreDi") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubSteps) ;

  if (pContexte->getPatient())
	{
#ifndef __EPIPUMP__
# ifndef __EPIBRAIN__
    _progressDialog->setText(string("Avis de fermeture à PreDi")) ;

		pContexte->getPredi()->PatChanged() ;
# endif
#endif
  }

  _progressDialog->setProgress(4) ;

  ClosePatientContinue() ;
}

void
NSUtilisateurChoisi::ClosePatientCloseHisto()
{
  _iCurrentClosingStep = cpsCloseHisto ;

  // cas de fermeture historique (autre cas CmExit() dans nautilus.cpp)
  if (pContexte->getPatient())
  {
    pContexte->getPatient()->_bCanCloseHisto = true ;

    string sMsg = string("Closing history window") ;
    pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubSteps) ;

    _progressDialog->setText(string("Fermeture de l'historique")) ;

    TMyApp *pApplication = pContexte->getSuperviseur()->getApplication() ;
    if (pApplication)
		  /* bool bAllClosed = */ pApplication->prendClient()->CloseChildrenButServicesWindows(pContexte) ;
  }

  ClosePatientContinue() ;
}

void
NSUtilisateurChoisi::ClosePatientCloseHistoCheck()
{
  _iCurrentClosingStep = cpsCloseHistoCheck ;

  string sMsg = string("Waiting for history window to close") ;
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubSteps) ;

  _progressDialog->setText(string("Attente de la fermeture de l'historique")) ;

  // This step is specific, it starts a waiting timer to make sure that
  // History window is closed before the MDI garbage collector is called
  //
  NSToDoTask *pToDoTask = new NSToDoTask ;
  pToDoTask->setWhatToDo(string("ClosePatientWaitForHistory")) ;
  pToDoTask->sendMe(pContexte->getSuperviseur()) ;
}

void
NSUtilisateurChoisi::ClosePatientCloseStrips()
{
  _iCurrentClosingStep = cpsCloseStrips ;

  string sMsg = string("All windows closed ; closing stripes") ;
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubSteps) ;

  _progressDialog->setText(string("Fermeture des barres d'alerte")) ;

  TMyApp *pApplication = pContexte->getSuperviseur()->getApplication() ;
  if (pApplication)
    pApplication->getFrame()->CloseStripes() ;

  _progressDialog->setProgress(50) ;

  ClosePatientContinue() ;
}

void
NSUtilisateurChoisi::ClosePatientGarbageCollect()
{
  _iCurrentClosingStep = cpsGarbageCollector ;

  string sMsg = string("Calling garbage collector for MDI children") ;
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubSteps) ;

  _progressDialog->setText(string("Fermeture des fenêtres MDI oubliées")) ;

  TMyApp *pApplication = pContexte->getSuperviseur()->getApplication() ;
  if (pApplication)
    pApplication->prendClient()->MdiChildrenGarbageCollector() ;

  _progressDialog->setProgress(55) ;

  ClosePatientContinue() ;
}

void
NSUtilisateurChoisi::ClosePatientConnectIrresolvedLinks()
{
  _iCurrentClosingStep = cpsConnectIrresolvedLinks ;

  if (pContexte->getPatient())
	{
    string sMsg = string("Referencing orphan graph links.") ;
    pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubSteps) ;

    _progressDialog->setText(string("Connexion des liens orphelins")) ;

		// enregistrement des documents contenant des liens en attente
		NSPersonGraphManager* pGraphManager = pContexte->getPatient()->getGraphPerson() ;
		string sNextDoc = pGraphManager->getLinkManager()->nextDocToSave() ;
		while (string("") != sNextDoc)
		{
      string sTrace = string("Recording document with waiting link: ") + sNextDoc ;
      pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubSteps) ;

			pGraphManager->commitGraphTree(pContexte, sNextDoc) ;

      _progressDialog->setProgress(60) ;

      // We must prevent an infinite loop if the document can't be saved
      //
			string sNewNextDoc = pGraphManager->getLinkManager()->nextDocToSave() ;
      if (sNewNextDoc == sNextDoc)
        break ;
      else
        sNextDoc = sNewNextDoc ;
		}
  }

  _progressDialog->setProgress(60) ;

  ClosePatientContinue() ;
}

void
NSUtilisateurChoisi::ClosePatientCloseContribution()
{
  _iCurrentClosingStep = cpsCloseContribution ;

  string sTrace = string("Closing contribution") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubSteps) ;

  _progressDialog->setText(string("Fermeture de la contribution")) ;

  if (pContexte->getPatient())
    pContexte->getPatient()->FermeContribution() ;

  _progressDialog->setProgress(90) ;

  ClosePatientContinue() ;
}

void
NSUtilisateurChoisi::ClosePatientEnd()
{
  _iCurrentClosingStep = cpsFinal ;

  string sTrace = string("Deleting patient object") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubSteps) ;

  _progressDialog->setText(string("Suppression de l'objet Patient")) ;

  NSPatientChoisi* pPat = pContexte->getPatient() ;
  pContexte->setPatient((NSPatientChoisi*) 0) ;
  if (pPat)
    delete pPat ;

  // on rétablit le titre de l'application sans les infos patient
  pContexte->setMainCaption() ;

  _progressDialog->setProgress(100) ;
  _progressDialog->Destroy() ;

  delete _progressDialog ;
  _progressDialog = 0 ;

  if (cpnDoNothing == _iWhatToDoAfterClosing)
    return ;

  if (cpnExecToDo == _iWhatToDoAfterClosing)
  {
    if ((NSToDoTask*) NULL == _pToDoAfterClosingPat)
      return ;

    string sTrace = string("Patient closed, sending ToDo \"") + _pToDoAfterClosingPat->getWhatToDo() + string("\"") ;
    pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubSteps) ;

    NSToDoTask* pTask = new NSToDoTask(*_pToDoAfterClosingPat) ;
    pTask->sendMe(pContexte->getSuperviseur()) ;

    delete _pToDoAfterClosingPat ;
    _pToDoAfterClosingPat = (NSToDoTask*) 0 ;

    return ;
  }

  if (pContexte->isClientGroup() && (false == pContexte->isGroupInterfaceOnly()))
  {
    synchroImportThisPat(_sLogin, _sPasswd) ;
    synchroExportThisPat(_sLogin, _sPasswd) ;
  }

  pContexte->getUtilisateur()->setEnregWin(false) ;

  // pour la synchro on ne delete pas le patient en cours
  // pour pouvoir le synchroniser et le réouvrir.
  // if (false == bWithSynchro)
  //{

  if (_pPatRech && (cpnOpenNextPat == _iWhatToDoAfterClosing))
  {
    string sTrace = string("Previous patient closed, opening new patient.") ;
    pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubSteps) ;

    bool bCanOpenPat = OuvrePatient(_pPatRech->getNss(), _pPatRech) ;
    if (false == bCanOpenPat)
      return ;

    bool bSwitchPat = ChangePatientChoisi() ;
    if (false == bSwitchPat)
      return ;

    pContexte->getPatient()->InitialisationAsynchronous() ;
  }

  if (cpnOpenFromCapture == _iWhatToDoAfterClosing)
  {
    string sTrace = string("Previous patient closed, opening new patient from captured information.") ;
    pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubSteps) ;

    NSEpisodus* pEpisod = pContexte->getEpisodus() ;
    if ((NSEpisodus*) NULL == pEpisod)
      return ;

    pContexte->captureData(&(pEpisod->CaptureArray)) ;
  }
}

bool
NSUtilisateurChoisi::getAndRemoveIppFromPatpatho(NSPatPathoArray* pPpt, string* psIpp)
{
	if (((NSPatPathoArray*)  NULL == pPpt) || (NULL == psIpp))
		return false ;

	*psIpp = string("") ;

	if (pPpt->empty())
		return true ;

	// Looking for an Identifier Library root concept
  //
  PatPathoIter It = pPpt->begin() ;
  while ((NULL != It) && (pPpt->end() != It) &&
                      (string("0LIBI") != (*It)->getLexiqueSens()))
		It = pPpt->ChercherFrereSuivant(It) ;

	// No Identifier in ppt
	//
	if ((NULL == It) || (pPpt->end() == It))
		return true ;

	NSPatPathoArray pptIdsLibrary(pContexte->getSuperviseur()) ;
  pPpt->ExtrairePatPatho(It, &pptIdsLibrary) ;

	// Suppress the identifier tree
  //
  pPpt->SupprimerItem(It) ;

	// Identifier tree contains no identifier
  //
	if (false == pptIdsLibrary.CheminDansPatpatho(string("LSITE1/LIPP01"), string(1, cheminSeparationMARK), &It))
		return true ;

	int Col = (*It)->getColonne() ;
	It++ ;
  if ((pptIdsLibrary.end() != It) && ((*It)->getColonne() == Col+1) &&
         			        (string("£CL") == (*It)->getLexiqueSens()))
  	*psIpp = (*It)->getTexteLibre() ;

	return true ;
}

void
NSUtilisateurChoisi::CallPatientInSlaveMode()
{
  string sMsg = string("Calling patient in slave mode") ;
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSteps) ;

  NSSuper *pSuper = pContexte->getSuperviseur() ;

  string sSlaveDllName = pSuper->getSlaveDll() ;
  if (string("") == sSlaveDllName)
    return ;

  TModule* pSlaveModule = new TModule(sSlaveDllName.c_str(), TRUE) ;
  if (NULL == pSlaveModule)
  {
    string sErreur = pSuper->getText("modulesManagement", "cannotLoadDll") + string(" ") + sSlaveDllName ;
    erreur(sErreur.c_str(), standardError, 0) ;
    pSuper->trace(&sErreur, 1, NSSuper::trError) ;
    return ;
  }

  bool (FAR *pAdrFctPutPatInfoInCapture) (NSContexte far *) ;
  (FARPROC) pAdrFctPutPatInfoInCapture = pSlaveModule->GetProcAddress(MAKEINTRESOURCE(2)) ;
  if (NULL == pAdrFctPutPatInfoInCapture)
  {
    delete pSlaveModule ;
    return ;
  }

  sMsg = string("Calling patient in slave mode: calling dll to load patient data as captured information") ;
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubSteps) ;

  bool bSuccess = ((*pAdrFctPutPatInfoInCapture)((NSContexte far *) pContexte)) ;
  if (false == bSuccess)
  {
    delete pSlaveModule ;
    return ;
  }

  delete pSlaveModule ;

  // In slave mode, the reference trait is IPP
  //
  NSCaptureArray* pCapt = &(pContexte->getEpisodus()->newCaptureArray) ;
  string sIPP = string("") ;
  pCapt->trouveChemin(string("0LIBI1/LSITE1/LIPP01"), &sIPP) ;

  if (string("") == sIPP)
  {
    sMsg = string("Calling patient in slave mode: No IPP found, leaving") ;
    pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trWarning) ;
    return ;
  }

  sMsg = string("Calling patient in slave mode: getting patient for IPP ") + sIPP ;
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubSteps) ;

  NSPatInfo* pPatWithIpp = getPatientForIpp(sIPP) ;

  AutomaticOpenOrCreate(pPatWithIpp) ;

  if (pPatWithIpp)
    delete pPatWithIpp ;

  sMsg = string("Leaving patient call in slave mode") ;
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSteps) ;
}

void
NSUtilisateurChoisi::AutomaticOpenOrCreate(NSPatInfo* pTargetPat)
{
  if ((NSPatInfo*) NULL == pTargetPat)
  {
    string sMsg = string("Creating a new patient in slave mode") ;
    pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubSteps) ;

    CreatePatientInSlaveMode() ;
    return ;
  }

  string sMsg = string("Opening an already existing patient in slave mode") ;
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubSteps) ;

  OuvrePatient(pTargetPat->getNss(), NSPatientChoisi::opnUpdateFromCapture) ;
}

bool
NSUtilisateurChoisi::UpdatePatientInSlaveMode()
{
  if ((NSPatientChoisi*) NULL == pContexte->getPatient())
    return false ;

  NSPersonGraphManager* pPatientGraph = pContexte->getPatient()->getGraphPerson() ;
  if ((NSPersonGraphManager*) NULL == pPatientGraph)
    return false ;

  // First, get demographics tree
  //
  NSDataGraph* pGraph = pPatientGraph->getDataGraph() ;
  if ((NSDataGraph*) NULL == pGraph)
    return false ;

  NSSuper *pSuper = pContexte->getSuperviseur() ;

  NSDataTreeIter iterTree ;
  if (false == pGraph->getTrees()->ExisteTree("ZADMI1", pSuper, &iterTree))
  {
    string sErreur = pSuper->getText("graphAndTreeErrors", "missingTreeInGraph") + string(" (") + string("ZADMI1") + string(")") ;
    pSuper->trace(&sErreur, 1, NSSuper::trError) ;
    erreur(sErreur.c_str(), standardError, 0) ;
    return false ;
  }

  string sTreeId = (*iterTree)->getTreeID() ;

  string sRights = string("") ;
  NSPatPathoArray demographicsPpt(pSuper, graphPerson) ;
  if (false == pGraph->getTree(sTreeId, &demographicsPpt, &sRights, true))
    return false ;

  NSPatPathoArray updatedPpt(pSuper, graphPerson) ;
  updatedPpt = demographicsPpt ;

  // Then update it
  //
  NSCaptureArray* pCapt = &(pContexte->getEpisodus()->newCaptureArray) ;

  string sAdminArchetype = pSuper->getDemographicArchetypeId() ;
  if (string("") == sAdminArchetype)
  {
    string sError = pSuper->getText("archetypesManagement", "unknownDemographicArchetypeName") ;
    pSuper->trace(&sError, 1, NSSuper::trError) ;
    erreur(sError.c_str(), standardError, 0) ;
    return false ;
  }

  string sArchetypeFile = pSuper->getArcManager()->DonneFichierArchetypeParNom(NSArcManager::archetype, sAdminArchetype) ;
	if (string("") == sArchetypeFile)
	{
  	string sErrorText = pSuper->getText("archetypesManagement", "cannotFindThisArchetypeFile") ;
  	sErrorText += string(" (") + sAdminArchetype + string(")") ;
    pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError, 0) ;
		return false ;
	}

  nsarcParseur ArcParseur(pSuper) ;

  if (false == ArcParseur.open(sArchetypeFile))
	{
    string sErrorText = pSuper->getText("archetypesManagement", "archetypeParsingError") ;
  	sErrorText += string(" (") + sArchetypeFile + string(")") ;
    pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError, 0) ;
		return false ;
	}

  // Update pathpatho from new information
  //
  ArcParseur.updateArchetypedPpt(&updatedPpt, pCapt) ;

  string sIPP ;
  getAndRemoveIppFromPatpatho(&updatedPpt, &sIPP) ;

  // TODO is it needed?
  // if ((string("") != sIPP) && (string("") != pSuper->getIppSite()))
  //  updateIPPEnCours(pSuper->getIppSite(), sIPP) ;

  if (demographicsPpt.estEgal(&updatedPpt))
    return true ;

  // Something changed, we must update demographics
  //

  pContexte->getPatient()->CreeContribution() ;

  NSVectPatPathoArray VectUpdate ;
  string sCodeDocMeta = string("") ;
	VectUpdate.push_back(new NSPatPathoArray(updatedPpt)) ;

	pPatientGraph->updateTrees(pContexte, &VectUpdate, &sTreeId, &sCodeDocMeta, pidsPatient) ;

	if (string("") == sTreeId)
	{
  	string sErrorText = pContexte->getSuperviseur()->getText("documentManagementErrors", "failedToSave") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError, 0, 0) ;
		return false ;
	}

  return true ;
}

bool
NSUtilisateurChoisi::CreatePatientInSlaveMode()
{
  NSSuper *pSuper = pContexte->getSuperviseur() ;

  string sMsg = string("Entering patient creation in slave mode.") ;
  pSuper->trace(&sMsg, 1, NSSuper::trSteps) ;

  // Getting the demographics Archetype
  //
  string sAdminArchetype = pSuper->getDemographicArchetypeId() ;
  if (string("") == sAdminArchetype)
  {
    string sError = pSuper->getText("archetypesManagement", "unknownDemographicArchetypeName") ;
    pSuper->trace(&sError, 1, NSSuper::trError) ;
    erreur(sError.c_str(), standardError, 0) ;
    return false ;
  }

  string sArchetypeFile = pSuper->getArcManager()->DonneFichierArchetypeParNom(NSArcManager::archetype, sAdminArchetype) ;
	if (string("") == sArchetypeFile)
	{
  	string sErrorText = pSuper->getText("archetypesManagement", "cannotFindThisArchetypeFile") ;
  	sErrorText += string(" (") + sAdminArchetype + string(")") ;
    pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError, 0) ;
		return false ;
	}

  nsarcParseur ArcParseur(pSuper) ;

  if (false == ArcParseur.open(sArchetypeFile))
	{
    string sErrorText = pSuper->getText("archetypesManagement", "archetypeParsingError") ;
  	sErrorText += string(" (") + sArchetypeFile + string(")") ;
    pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError, 0) ;
		return false ;
	}

  NSPatPathoArray PPT(pSuper, graphPerson) ;
  NSCaptureArray* pCapt = &(pContexte->getEpisodus()->newCaptureArray) ;

  // Building the tree (filling the Archetype with captured information)
  //
  if ((false == ArcParseur.createArchetypedPpt(&PPT, pCapt)) || (PPT.empty()))
  {
    string sErrorText = string("Can't create PPT from capture.") ;
    pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError, 0) ;
		return false ;
  }

  // Adding the Archetype stamp
  //
  PatPathoIter pPptIter = PPT.begin() ;
  if (*pPptIter)
    (*pPptIter)->setArchetype(sAdminArchetype) ;

  // Creating the patient
  //
  sMsg = string("Creating patient in slave mode.") ;
  pSuper->trace(&sMsg, 1, NSSuper::trSubSteps) ;

  bool bCreationSuceeded = createPatient(&PPT) ;

  // Opening the patient
  //
  if (false == bCreationSuceeded)
  {
    sMsg = string("Patient creation in slave mode failed.") ;
    pSuper->trace(&sMsg, 1, NSSuper::trError) ;
    return false ;
  }

  sMsg = string("Patient successfuly created in slave mode.") ;
  pSuper->trace(&sMsg, 1, NSSuper::trSubSteps) ;

  pContexte->getPatient()->InitialisationAsynchronous() ;

  sMsg = string("Leaving patient creation in slave mode.") ;
  pSuper->trace(&sMsg, 1, NSSuper::trSteps) ;

  return true ;
}

bool
NSUtilisateurChoisi::createPatient(NSPatPathoArray *pPatPathoAdmin)
{
	if (((NSPatPathoArray*) NULL == pPatPathoAdmin) || (true == pPatPathoAdmin->empty()))
		return false ;

  NSSuper *pSuper = pContexte->getSuperviseur() ;

  string sMsg = string("Entering createPatient") ;
  pSuper->trace(&sMsg, 1, NSSuper::trSubSteps) ;

//	int typeCode ;

  pContexte->setPatient(new NSPatientChoisi(pContexte)) ;

  // Remove IPP if exists
  string sIPP = string("") ;
  getAndRemoveIppFromPatpatho(pPatPathoAdmin, &sIPP) ;

  NSPatientChoisi* pNewPatient = pContexte->getPatient() ;
  if ((NSPatientChoisi*) NULL == pNewPatient)
    return false ;

  //données patient correspondantes au PIDS
  pNewPatient->ChargeDonneesPatient(pPatPathoAdmin) ;

  //l'dentifiant PIDS in memoire
  pNewPatient->setNss(string(1, INMEMORY_CHAR) + string(PIDS_NSS_LEN - 1, '0')) ;

  string sRootDocGraph = string(1, INMEMORY_CHAR) + string(DOC_CODE_DOCUM_LEN - 1, '0') ;
  string sRootGraph = pNewPatient->getNss() + sRootDocGraph ;

  //on fixe la racine du graphe du patient
  pNewPatient->getGraphPerson()->setRootTree(sRootGraph) ;
  pNewPatient->getGraphPerson()->getDataGraph()->setLastTree(sRootGraph) ;

  // Creating all basic documents
  //
  pNewPatient->CreeRootDocs(pPatPathoAdmin) ;

  string user = pContexte->getUtilisateurID() ;

  NSDataGraph* pGraph = pNewPatient->getGraphPerson()->getDataGraph() ;
  NSPersonsAttributesArray PatiensList ;

  // NSBasicAttributeArray *pAttrList = new NSBasicAttributeArray() ;
  NSBasicAttributeArray *pAttrList = pNewPatient->getGraphPerson()->getAttrArray() ;
  pAttrList->push_back(new NSBasicAttribute(LAST_NAME,  pNewPatient->getNom())) ;
  pAttrList->push_back(new NSBasicAttribute(FIRST_NAME, pNewPatient->getPrenom())) ;
  pAttrList->push_back(new NSBasicAttribute(SEX,        pNewPatient->getSexe())) ;
  pAttrList->push_back(new NSBasicAttribute(BIRTHDATE,  pNewPatient->getNaissance())) ;
  pAttrList->push_back(new NSBasicAttribute(OPERATOR,   user)) ;
  pAttrList->push_back(new NSBasicAttribute(CONSOLE,    pSuper->getConsoleString())) ;
  pAttrList->push_back(new NSBasicAttribute(INSTANCE,   pSuper->getInstanceString())) ;
  if (string("") != sIPP)
  	pAttrList->push_back(new NSBasicAttribute(IPP, sIPP)) ;

/*
  bool bRes = pContexte->pPilot->createPersonOrObject(NautilusPilot::SERV_CREATE_PATIENT.c_str(), pGraph, &PatiensList, pAttrList) ;
  if ((!bRes) || (!pGraph))
  {
  	string sErrorText = pContexte->getSuperviseur()->getText("patientManagement", "patientCreationError") ;
    erreur(sErrorText.c_str(), standardError, 0) ;
    return false ;
  }
*/

	bool bRes = pNewPatient->getGraphPerson()->writeGraph(pContexte, pidsPatient, (NSDataGraph *) 0) ;
  if (false == bRes)
  {
    string sErrorText = pSuper->getText("patientManagement", "patientIdCreationError") ;
    pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
    erreur(sErrorText.c_str(), standardError, 0) ;
  	return false ;
  }

  // ---------------------------------------------------------------------------
	// Ugly patch : createPersonOrObject should give these status... but doesn't
  pNewPatient->setADebloquer(true) ;
  pNewPatient->setReadOnly(false) ;

  pNewPatient->setContribution(pGraph->getLastContribution()) ;

  string sPersonId = pNewPatient->getGraphPerson()->getPersonID() ;
  if ((string("") == sPersonId) || (INMEMORY_CHAR == sPersonId[0]))
  {
    string sErrorText = pSuper->getText("patientManagement", "patientIdCreationError") ;
    pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
    erreur(sErrorText.c_str(), standardError, 0) ;
    return false ;
  }
  pAttrList->push_back(new NSBasicAttribute(PERSON, sPersonId)) ;

  NSDataTreeIter iterTree ;

  // Health Team creation
  //
  pNewPatient->CreeRootHealthTeam(sPersonId) ;

	//
  // Identifiers Library creation
  //
	if (string("") != sIPP)
    pNewPatient->createIdentifiersLibrary(sIPP) ;

  if (pSuper->getIndexManagementRule() == NSSuper::umMultiTrees)
    pNewPatient->CreeRootIndexExtensions() ;

  bRes = pSuper->getPilot()->modifyPersonOrObject(NautilusPilot::SERV_MODIFY_PERSON.c_str(), pGraph, &PatiensList, pAttrList) ;

  if ((false == bRes) || ((NSDataGraph*) NULL == pGraph))
  {
  	string sErrorText = pSuper->getText("HealthTeamManagement", "patientModifyError") ;
    pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
    erreur(sErrorText.c_str(), standardError, 0) ;
    return false ;
  }

  // Update du nss et de la contribution initiale LCTRI1
  pNewPatient->setNss(pContexte->getPatient()->getGraphPerson()->getPersonID()) ;
  if (pGraph->getTrees()->ExisteTree("LCTRI1", pSuper, &iterTree))
    pNewPatient->setContribution((*iterTree)->getTreeID()) ;
  else
    erreur("Erreur à la recherche de la contribution initiale du patient.", standardError, 0) ;

  pNewPatient->setReadOnly(false) ;

  sMsg = string("Leaving createPatient") ;
  pSuper->trace(&sMsg, 1, NSSuper::trSubSteps) ;

  return true ;
}

bool
NSUtilisateurChoisi::createCorrespondant(NSCorrespondantInfo* pNewCorresp)
{
	if ((NSCorrespondantInfo*) NULL == pNewCorresp)
		return false ;

try
{
	NSPatPathoArray PatPathoCor(pContexte->getSuperviseur(), graphPerson) ;

	NSSmallBrother BigBoss(pContexte, &PatPathoCor) ;
	BigBoss.setMotherWindow(pContexte->GetMainWindow()) ;
	// Ici on lance une boite de dialogue modale
	// BB1BBInterfaceForKs InterfaceForKs(-1, pContexte->getSuperviseur()->getAddressArchetypeId(), "", false) ;
  BB1BBInterfaceForKs InterfaceForKs(-1, pContexte->getSuperviseur()->getReferalProfessionnalArchetypeId(), "", false) ;
	BigBoss.lanceBbkArchetypeInDialog(pContexte->getSuperviseur()->getReferalProfessionnalArchetypeId(), 0, 0, &InterfaceForKs, true) ;

	// on teste le code de retour du dialogue, qui est stocké dans le
	// BBItem root créé par le pBigBoss.
	if (BigBoss.getBBItem()->getRetourDlg() == 0)     // CmOK
	{
		// on enregistre la patpatho du pBigBoss, qui contient les nouvelles données
		// si elle n'est pas vide.
		if (false == BigBoss.getPatPatho()->empty())
		{
    	PatPathoCor = *(BigBoss.getPatPatho()) ;

			pNewCorresp->GetDataFromGraph(&PatPathoCor) ;

			string sNewPids = string(1, INMEMORY_CHAR) + string(PAT_NSS_LEN - 1, '_') ;
			NSBasicAttributeArray AttrList ;
			AttrList.push_back(new NSBasicAttribute(OPERATOR,   pContexte->getUtilisateurID())) ;
			AttrList.push_back(new NSBasicAttribute(FIRST_NAME, pNewCorresp->getPrenom())) ;
			AttrList.push_back(new NSBasicAttribute(LAST_NAME,  pNewCorresp->getNom())) ;
			AttrList.push_back(new NSBasicAttribute(PIDS,       sNewPids)) ;
			pNewCorresp->getGraph()->setInfoPids(&AttrList) ;

			if (pNewCorresp->getGraph()->setGraphAdmin(pContexte, &PatPathoCor, pidsCorresp))
			{
				// on récupère le personID pour le stocker dans nss
				string sPersonID = _GraphPerson.getPersonID() ;  ///
				if (string("") != sPersonID)
        	pNewCorresp->setNss(sPersonID) ;
			}
		}
	}
	return true ;
}
catch (...)
{
	erreur("Exception NSUtilisateurChoisi::CreePIDS().", standardError, 0) ;
	return false ;
}
}

bool
NSUtilisateurChoisi::importPatientLdv()
{
try
{
	NSPatientImportDialog* pDialog = new NSPatientImportDialog(pContexte->GetMainWindow(), pContexte) ;
	if (pDialog->Execute() != IDOK)
  {
		delete pDialog ;
		return true ;
	}

  NSSuper *pSuper = pContexte->getSuperviseur() ;

  const char* serviceName = (NautilusPilot::SERV_PATIENT_IMPORT).c_str() ;
  NSPersonGraphManager GraphManager(pContexte->getSuperviseur()) ;
  NSDataGraph *pGraph = GraphManager.getDataGraph() ;

  NSBasicAttributeArray AttrList ;
  AttrList.push_back(new NSBasicAttribute(ROLE,     PATIENT_ROLE)) ;
  AttrList.push_back(new NSBasicAttribute(LIDEN,    pDialog->sNumLdv)) ;
  AttrList.push_back(new NSBasicAttribute(LOGIN,    pDialog->sLogin)) ;
  AttrList.push_back(new NSBasicAttribute(PASSWORD, pDialog->sPasswd)) ;
  AttrList.push_back(new NSBasicAttribute(OPERATOR, pContexte->getUtilisateurID())) ;

  delete pDialog ;

  bool bRes = pSuper->getPilot()->importPerson(serviceName, &AttrList, pGraph) ;

  if ((!bRes)||(!pGraph))
  {
    /*string sErrorText = pContexte->getSuperviseur()->getText("patientManagement", "patientImportError") ;  */
		string sWarningText = pSuper->getPilot()->getWarningMessage() ;
		if (string("") != sWarningText)
    {
    	pSuper->trace(&sWarningText, 1, NSSuper::trWarning) ;
			erreur(sWarningText.c_str(), warningError, 0) ;
    }
		else
		{
			string sErrorText = pSuper->getPilot()->getErrorMessage() ;
			if (string("") != sErrorText)
      {
      	pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
				erreur(sErrorText.c_str(), standardError, 0) ;
      }
		}
    return false ;
  }

  if (pContexte->getPatient())
	{
    if (false == fermePatEnCours())
      return false ;
  }

  // =========== PARSING DU GRAPHE et sauvegarde des traits ==================

	GraphManager.parseMainAttributes(pContexte) ;

  string user = pContexte->getUtilisateurID() ;
  serviceName = (NautilusPilot::SERV_CREATE_IMPORTED_PATIENT).c_str() ;

  NSPersonsAttributesArray PersonsList ;

//  NSBasicAttributeArray *pAttrArray = pGraphManager->pAttrArray ;
	AttrList.vider() ;

  AttrList.push_back(new NSBasicAttribute(LAST_NAME,  GraphManager.getAttributeValue(LAST_NAME))) ;
  AttrList.push_back(new NSBasicAttribute(FIRST_NAME, GraphManager.getAttributeValue(FIRST_NAME))) ;
  AttrList.push_back(new NSBasicAttribute(SEX,        GraphManager.getAttributeValue(SEX))) ;
  AttrList.push_back(new NSBasicAttribute(BIRTHDATE,  GraphManager.getAttributeValue(BIRTHDATE))) ;
  AttrList.push_back(new NSBasicAttribute(OPERATOR,   user)) ;
  AttrList.push_back(new NSBasicAttribute(CONSOLE,    pSuper->getConsoleString())) ;
  AttrList.push_back(new NSBasicAttribute(INSTANCE,   pSuper->getInstanceString())) ;
  AttrList.push_back(new NSBasicAttribute(PERSON,     GraphManager.getPersonID())) ;
  string sRootDoc = string(GraphManager.getRootTree(), PAT_NSS_LEN, DOC_CODE_DOCUM_LEN) ;
  AttrList.push_back(new NSBasicAttribute(ROOTDOC,    sRootDoc)) ;

  bRes = pSuper->getPilot()->createImportedPerson(serviceName, pGraph, &PersonsList, &AttrList) ;

  if (!bRes)
  {
    string sErrorText = pSuper->getText("patientManagement", "importedPatientCreationError") ;
    pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
    erreur(sErrorText.c_str(), standardError, 0) ;
    return false ;
  }

  // import all objects for this patient
  NSBasicAttributeArray AttrObjectsArray ;
  AttrObjectsArray.push_back(new NSBasicAttribute(OPERATOR, user)) ;
  AttrObjectsArray.push_back(new NSBasicAttribute(PERSON,   GraphManager.getPersonID())) ;

  bRes = pSuper->getPilot()->updateObjectList(NautilusPilot::SERV_UPDATE_ALL_LDV_OBJECTS.c_str(), pGraph, &AttrObjectsArray) ;
  if (!bRes)
  {
  	string sErrorText = pSuper->getPilot()->getWarningMessage() ;
    if (string("") != sErrorText)
    {
    	pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
    	erreur(sErrorText.c_str(), standardError, 0) ;
    }

    sErrorText = pSuper->getText("patientManagement", "errorImportObjects") ;
    pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
    erreur(sErrorText.c_str(), standardError, 0) ;
    return true ;
  }

  // on ouvre maintenant le patient importé
  // on doit mettre celui-ci dans le patient de recherche (pPatRech)
  // avant de faire le changement de patient et l'appel de la routine d'initialisation
  //
  if (_pPatRech)
    delete _pPatRech ;

  GraphManager.setAttributeValue(PERSON,   GraphManager.getPersonID()) ;
  // pAttrArray->setAttributeValue(ROOTDOC, sRootDoc);
  GraphManager.setAttributeValue(OPERATOR, user) ;
	GraphManager.setAttributeValue(CONSOLE,  pSuper->getConsoleString()) ;
	GraphManager.setAttributeValue(INSTANCE, pSuper->getInstanceString()) ;

  _pPatRech = new NSPatInfo(&AttrList, pContexte) ;
  *(_pPatRech->getGraphPerson()) = GraphManager ;

  ChangePatientChoisi() ;
  pContexte->getPatient()->getGraphPerson()->setNeedUnlock(true) ;
  pContexte->getPatient()->getGraphPerson()->setReadOnly(false) ;
	pContexte->getPatient()->getGraphPerson()->getDataGraph()->setLastTree("") ;
  // On lance le patient :
  // Récupération des données rattachées au patient : adresse et correspondants
  pContexte->getPatient()->InitialisationAsynchronous() ;

	return true ;
}
catch (...)
{
	erreur("Exception NSUtilisateurChoisi::importPatientLdv().", standardError, 0) ;
	return false ;
}
}

bool
NSUtilisateurChoisi::importUserLdv()
{
try
{
  NSUserImportDialog userImportDlg(pContexte->GetMainWindow(), pContexte) ;
	if (userImportDlg.Execute() != IDOK)
		return true ;

	NSPersonGraphManager gManager(pContexte->getSuperviseur()) ;
	NSDataGraph *pGraph = gManager.getDataGraph() ;

  NSBasicAttributeArray AttrList ;
  // pAttrList->push_back(new NSBasicAttribute(ROLE, pDialog->sRole));
	AttrList.push_back(new NSBasicAttribute(LOGIN,    userImportDlg.sLogin)) ;
	AttrList.push_back(new NSBasicAttribute(PASSWORD, userImportDlg.sPasswd)) ;

  const char* serviceName = (NautilusPilot::SERV_USER_IMPORT).c_str() ;

	bool bRes = pContexte->getSuperviseur()->getPilot()->importPerson(serviceName, &AttrList, pGraph) ;
	if ((false == bRes) || ((NSDataGraph*) NULL == pGraph))
	{
		// string sErrorText = pContexte->getSuperviseur()->getText("patientManagement", "patientCreationError") ;
		/*	string sErrorText = "Echec de la procédure d'importation." ;    */
		string sWarningText = pContexte->getSuperviseur()->getPilot()->getWarningMessage() ;
		if (sWarningText != "")
    {
    	pContexte->getSuperviseur()->trace(&sWarningText, 1, NSSuper::trWarning) ;
			erreur(sWarningText.c_str(), warningError, 0) ;
    }
		else
		{
			string sErrorText = pContexte->getSuperviseur()->getPilot()->getErrorMessage() ;
			if (sErrorText != "")
      {
      	pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
				erreur(sErrorText.c_str(), standardError, 0) ;
      }
		}
		//	erreur(sErrorText.c_str(), standardError, 0) ;

		return false ;
	}

	// =========== PARSING DU GRAPHE et sauvegarde des traits ==================

	gManager.parseMainAttributes(pContexte);

	// Appeler la boite de dialogue de création / modification
	// pour définir login et mot de passe, type, etc...
	// puis appeler createImportedPerson
	TModule                 *module = pContexte->GetMainWindow()->GetModule() ;
	CreerUtilisateurDialog  newUtilDlg(pContexte->GetMainWindow(), pContexte, module) ;
	NSUtiliInfo             newUtilInfo(gManager.getAttrArray(), pContexte) ;

	serviceName = (NautilusPilot::SERV_CREATE_IMPORTED_USER).c_str() ;
	AttrList.vider() ;

	// on charge ici les MainAttributes dans le dialogue
	*(newUtilDlg.pData) = *(newUtilInfo.getData()) ;

	while ((newUtilDlg.Execute()) == IDCANCEL)
	{
		int retVal = ::MessageBox(0, "L'annulation entrainera l'échec de la procédure d'importation de cet utilisateur. Voulez-vous continuer ?", "Message Nautilus", MB_YESNO) ;
		if (retVal == IDYES)
			return false ;
	}

	// on stocke les donnees du dialogue dans les Data
	*(newUtilInfo.getData())          = *(newUtilDlg.pData) ;
	newUtilInfo.setLogin(newUtilDlg.getLogin()) ;
	newUtilInfo.setPasswd(newUtilDlg.getPasswd()) ;
	newUtilInfo.setUserType(newUtilDlg.getUserType()) ;
	newUtilInfo.setPassType(newUtilDlg.getPassType()) ;
	newUtilInfo.setPassDate(newUtilDlg.getPassDate()) ;
	newUtilInfo.setValidity(newUtilDlg.getValidity()) ;

	// on construit maintenant les nouvelles valeurs de PIDS pour le pilote
	string sLogin     = newUtilInfo.getLogin() ;
	string sPasswd    = newUtilInfo.getPasswd() ;
	string sNss       = gManager.getPersonID() ;
  string sUserType  = newUtilInfo.getUserType() ;

	// On met à jour le graphe de l'utilisateur de pUtilInfo
	AttrList.push_back(new NSBasicAttribute(LOGIN,      sLogin)) ;
	AttrList.push_back(new NSBasicAttribute(PASSWORD,   sPasswd)) ;
	// pAttrList->push_back(new NSBasicAttribute(PIDS, sNss));
	AttrList.push_back(new NSBasicAttribute(PERSON,     sNss)) ;
	AttrList.push_back(new NSBasicAttribute(OPERATOR,   pContexte->getUtilisateurID())) ;
	AttrList.push_back(new NSBasicAttribute(USERTYPE,   newUtilInfo.getUserType())) ;
	AttrList.push_back(new NSBasicAttribute(PASSWDTYPE, newUtilInfo.getPassType())) ;
	if (string("") != newUtilInfo.getValidity())
  	AttrList.push_back(new NSBasicAttribute(DAYSNB,   newUtilInfo.getValidity())) ;
	AttrList.push_back(new NSBasicAttribute(FIRST_NAME, newUtilInfo.getPrenom())) ;
	AttrList.push_back(new NSBasicAttribute(LAST_NAME,  newUtilInfo.getNom())) ;
	AttrList.push_back(new NSBasicAttribute(ROOTDOC,    string(DOC_CODE_DOCUM_LEN, '0'))) ;
	gManager.setInfoPids(&AttrList);

	NSPersonsAttributesArray PersonsList ;
	bRes = pContexte->getSuperviseur()->getPilot()->createImportedPerson(serviceName, pGraph, &PersonsList, &AttrList) ;
	if (false == bRes)
	{
		// string sErrorText = pContexte->getSuperviseur()->getText("patientManagement", "patientCreationError") ;
		erreur("Echec de la procédure d'importation (phase 2).", standardError, 0) ;
		return false ;
	}

  if (sUserType.find("U") != string::npos)
  {
    // FIXME TODO -- FLP
    // here we have to insert imported user in local medical team
    // il faut récupérer l'ID de la personne créée et l'ajouter dans l'Equipe Médicale locale (ou toutes les equipes medicales locales)
    //
    NSTeamSupervisor teamSupervisor(pContexte) ;
    vector<string> vLocalTeams = teamSupervisor.getLocalTeams() ;
    if (false == vLocalTeams.empty())
    {
      for (vector<string>::iterator lTeamIter = vLocalTeams.begin() ; vLocalTeams.end() != lTeamIter ; lTeamIter++)
      {
        NVLdVTemps ldvCurDate ;	ldvCurDate.takeTime() ;
        NVLdVTemps ldvNoLimit ;	ldvNoLimit.setNoLimit() ;

        // récupération de l'objet Team et ajout de la nouvelle personne
        NSTeamGraphManager teamManager(pContexte->getSuperviseur()) ;
        NSPatPathoArray	ppt(pContexte->getSuperviseur()) ;
        teamManager.getTeamGraph(*lTeamIter, &ppt) ;

        NSMoralPerson MoralPerson(pContexte, &ppt) ;
        NSMoralPersonManager mpManager(&MoralPerson) ;

        NSHTMMandateArray MandatesArray ;
        MandatesArray.push_back(new NSHealthTeamMandate(nearDist, 3, "", "", ldvCurDate.donneDateHeure(), ldvNoLimit.donneDateHeure(), "", "", NSHealthTeamMandate::user)) ;
        mpManager.addMember(new NSHealthTeamMember(sNss, NSHealthTeamMember::person, &MandatesArray), NSHealthTeamMember::person) ;

        // + appel pilot pour modifier la Team
        teamManager.setTeamGraph(mpManager.getPatho(), pContexte->getUtilisateurID()) ;
      }
    }
  }

  // Create user directory and specific files
	//
  CreateUserFiles(sNss) ;

	return true ;
}
catch (...)
{
	erreur("Exception NSUtilisateurChoisi::importUserLdv().", standardError, 0) ;
	return false ;
}
}

// -----------------------------------------------------------------------------
// NSUtilisateur::importCorrespLdv
// -----------------------------------------------------------------------------
// function that imports a "correspondant" (a contact) from LdV server.
// -----------------------------------------------------------------------------
bool
NSUtilisateurChoisi::importCorrespLdv()
{
try
{
  NSCorrespImportDialog* pDialog = new NSCorrespImportDialog(pContexte->GetMainWindow(), pContexte) ;
  if (pDialog->Execute() != IDOK)
  {
    delete pDialog ;
    return false ;
  }

  string sLogin   = pDialog->sLogin ;
  string sPasswd  = pDialog->sPasswd ;
  string sAdeliID = pDialog->sAdeli ;
  string sLdVID   = pDialog->sNumLdv ;
  delete pDialog ;

  NSPersonGraphManager GraphManager(pContexte->getSuperviseur()) ;
  NSDataGraph *pGraph = GraphManager.getDataGraph() ;
  // pAttrList->push_back(new NSBasicAttribute(ROLE, pDialog->sRole)) ;

  const char* serviceName = (NautilusPilot::SERV_CORRESPONDENT_IMPORT).c_str() ;

  NSBasicAttributeArray AttrList ;
  if (sAdeliID != "")
		AttrList.push_back(new NSBasicAttribute(ADELI_ID, sAdeliID)) ;
  else if (sLdVID != "")
		AttrList.push_back(new NSBasicAttribute(LIDEN, sLdVID)) ;
  else
    return false ;

	AttrList.push_back(new NSBasicAttribute(LOGIN, sLogin)) ;
	AttrList.push_back(new NSBasicAttribute(PASSWORD, sPasswd)) ;
  bool bRes = pContexte->getSuperviseur()->getPilot()->importPerson(serviceName, &AttrList, pGraph) ;
  if ((!bRes) || (!pGraph))
  {
    // string sErrorText = pContexte->getSuperviseur()->getText("patientManagement", "patientCreationError") ;
    /*	string sErrorText = "Echec de la procédure d'importation." ;    */
    string sWarningText = pContexte->getSuperviseur()->getPilot()->getWarningMessage() ;
    if (sWarningText != "")
    {
    	pContexte->getSuperviseur()->trace(&sWarningText, 1, NSSuper::trError) ;
      erreur(sWarningText.c_str(), standardError, 0) ;
    }
    else
    {
      string sErrorText = pContexte->getSuperviseur()->getPilot()->getErrorMessage() ;
      if (sErrorText != "")
      {
      	pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
        erreur(sErrorText.c_str(), standardError, 0) ;
      }
    }
    //	erreur(sErrorText.c_str(), standardError, 0) ;

    return false ;
  }

  // =========== PARSING DU GRAPHE et sauvegarde des traits ==================

	GraphManager.parseMainAttributes(pContexte) ;

  string sCorrespId = GraphManager.getPersonID() ;
  serviceName = (NautilusPilot::SERV_CREATE_IMPORTED_CORRESP).c_str() ;
  AttrList.vider() ;
  AttrList.push_back(new NSBasicAttribute(OPERATOR,   pContexte->getUtilisateurID())) ;
  AttrList.push_back(new NSBasicAttribute(LAST_NAME,  GraphManager.getAttributeValue(LAST_NAME))) ;
  AttrList.push_back(new NSBasicAttribute(FIRST_NAME, GraphManager.getAttributeValue(FIRST_NAME))) ;
  AttrList.push_back(new NSBasicAttribute(PERSON,     sCorrespId)) ;
  string sRootDoc = string(GraphManager.getRootTree(), PAT_NSS_LEN, DOC_CODE_DOCUM_LEN) ;
  AttrList.push_back(new NSBasicAttribute(ROOTDOC,    sRootDoc)) ;

  NSPersonsAttributesArray PersonsList ;
  bRes = pContexte->getSuperviseur()->getPilot()->createImportedPerson(serviceName, pGraph, &PersonsList, &AttrList) ;
  if (!bRes)
  {
    // string sErrorText = pContexte->getSuperviseur()->getText("patientManagement", "patientCreationError") ;
    string sErrorText = "Echec de la procédure d'importation (phase 2)." ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
    erreur(sErrorText.c_str(), standardError, 0) ;
    return false ;
  }

  if (sCorrespId != "")
  {
    char  szBuffer[1024] ;
    NSPersonInfo* pPersonInfo = pContexte->getPersonArray()->getPerson(pContexte, sCorrespId, pidsCorresp) ;
    sprintf(szBuffer, "Le correspondant \"%s\" a bien été importé", pPersonInfo->getCivilite().c_str()) ;
    MessageBox(NULL, szBuffer, "Importation d'un correspondant", MB_OK) ;
  }  return true ;
}
catch (...)
{
  erreur("Exception NSUtilisateurChoisi::importCorrespLdv().", standardError, 0) ;
  return false ;
}
}

bool
NSUtilisateurChoisi::synchroImportThisPat(string sGlobalLogin, string sGlobalPasswd)
{
try
{
	const char* serviceName = (NautilusPilot::SERV_IMPORT_DATA).c_str();
	NSPersonGraphManager* pGraphManager = pContexte->getPatient()->getGraphPerson() ;
	NSDataGraph*          pGraph = pGraphManager->getDataGraph() ;

	NSBasicAttributeArray AttrList ;
	AttrList.push_back(new NSBasicAttribute(LOGIN,    sGlobalLogin)) ;
	AttrList.push_back(new NSBasicAttribute(PASSWORD, sGlobalPasswd)) ;
	AttrList.push_back(new NSBasicAttribute(PERSON,   pGraphManager->getPersonID())) ;

	bool bRes = pContexte->getSuperviseur()->getPilot()->importNewDataGraphPerson(serviceName, &AttrList, pGraph) ;

	if ((!bRes)||(!pGraph))
	{
		string sErrorText = pContexte->getSuperviseur()->getText("patientSynchronization", "patientImportError") ;
		if (sErrorText == "")
			sErrorText = "Echec de la procédure de mise à jour depuis la Ligne de vie.";
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError, 0) ;
		return false ;
	}

	// pGraphManager->setInfoPids(pAttrArray);
	pGraphManager->getDataGraph()->setLastTree() ;

  return true ;
}
catch (...)
{
	erreur("Exception NSUtilisateurChoisi::synchroImportThisPat().", standardError, 0) ;
	return false ;
}
}

bool
NSUtilisateurChoisi::synchroExportThisPat(string sGlobalLogin, string sGlobalPasswd)
{
try
{
	const char* serviceName = (NautilusPilot::SERV_EXPORT_DATA).c_str() ;
	NSPersonGraphManager* pGraphManager = pContexte->getPatient()->getGraphPerson() ;
	NSDataGraph *pGraph = pGraphManager->getDataGraph() ;

  NSBasicAttributeArray AttrList ;
	string sPID = pGraphManager->getPersonID() ;
	AttrList.push_back(new NSBasicAttribute(LOGIN,    sGlobalLogin)) ;
	AttrList.push_back(new NSBasicAttribute(PASSWORD, sGlobalPasswd)) ;
	AttrList.push_back(new NSBasicAttribute(PERSON,   sPID)) ;

	bool bRes = pContexte->getSuperviseur()->getPilot()->exportNewDataGraphPerson(serviceName, &AttrList) ;

	if ((false == bRes) || ((NSDataGraph*) NULL == pGraph))
	{
		string sErrorText = pContexte->getSuperviseur()->getText("patientManagement", "patientExportError") ;
		if (string("") == sErrorText)
			sErrorText = string("Echec de la procédure de synchronisation vers la Ligne de vie.") ;
		string sExecError = pContexte->getSuperviseur()->getPilot()->getWarningMessage() + pContexte->getSuperviseur()->getPilot()->getErrorMessage() ;
		sErrorText =  sErrorText + string(1, '\n') + sExecError ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError, 0) ;
		return false ;
	}

  string sText = pContexte->getSuperviseur()->getText("patientSynchronization", "operationOk") ;
  pContexte->getSuperviseur()->trace(&sText, 1, NSSuper::trError) ;
  erreur(sText.c_str(), warningError, 0) ;
  return true ;
}
catch (...)
{
	erreur("Exception NSUtilisateurChoisi::synchroExportThisPat().", standardError, 0) ;
	return false ;
}
}

NSPatInfo*
NSUtilisateurChoisi::getPatientForIpp(string sIPP)
{
  if (string("") == sIPP)
    return (NSPatInfo*) 0 ;

  NSSuper *pSuper = pContexte->getSuperviseur() ;

  NautilusPilot* pPilot = pSuper->getPilot() ;
  if ((NautilusPilot*) NULL == pPilot)
    return (NSPatInfo*) 0 ;

  // Check that there is a single person with this IPP
  //
  NSPersonsAttributesArray PatiensList ;
  NSBasicAttributeArray    AttrArray ;
  AttrArray.push_back(new NSBasicAttribute(IPP, sIPP)) ;

  bool bListOk = pPilot->personList((NautilusPilot::SERV_PERSON_LIST_FROM_TRAITS).c_str(), &PatiensList, &AttrArray) ;
  if (false == bListOk)
  {
    string sErreur = pPilot->getWarningMessage() ;
    pSuper->trace(&sErreur, 1, NSSuper::trError) ;
    erreur(sErreur.c_str(), standardError, 0) ;
    return (NSPatInfo*) 0 ;
  }

  // IPP not found
  //
  if (PatiensList.empty())
    return (NSPatInfo*) 0 ;

  if (PatiensList.size() > 1)
  {
    string sWarning = string("Multiple patient with IPP = ") + sIPP + string(" ; opening first") ;
    pSuper->trace(&sWarning, 1, NSSuper::trWarning) ;
  }

  NSPersonsAttributeIter iterPatient = PatiensList.begin() ;
  return new NSPatInfo(*iterPatient, pContexte) ;
}

//---------------------------------------------------------------------------
//  Fonction :		NSUtilisateurChoisi::initMail()
//
//  Description :	Charge les paramètres SMTP et POP3 à partir de email.dat.
//---------------------------------------------------------------------------
bool
NSUtilisateurChoisi::initMail()
{
  initSpecificMail(string("email"),         _pMail) ;
  initSpecificMail(string("emailApicrypt"), _pMailApicrypt) ;

  return true ;
}

bool
NSUtilisateurChoisi::initSpecificMail(string sFileName, NSMailParams* pMailParams)
{
  if ((string("") == sFileName) || ((NSMailParams*) NULL == pMailParams))
    return false ;

  string sData = string("") ;

  // First, try to open a user specific configuration file
  //
	string sFichier = sFileName + getNss() + string(".dat") ;

  // une erreur bizarre pousse à définir deux ifstreams
  //
	ifstream inFile1 ;
	inFile1.open(sFichier.c_str()) ;
	if (!inFile1)
	{
    // If no user specific configuration file found, try a default one
    //
    sFichier = sFileName + string(".dat") ;

    ifstream inFile2 ;
		inFile2.open(sFichier.c_str()) ;
		if (!inFile2)
		{
    	string sErrorText = pContexte->getSuperviseur()->getText("fileErrors", "errorOpeningInputFile") + string(" ") + sFichier ;
    	pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
			erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
			return false ;
		}

		while (!inFile2.eof())
		{
      string sLine ;
			getline(inFile2, sLine) ;
			if (string("") != sLine)
				sData += sLine + string(1, '\n') ;
		}

		inFile2.close() ;
	}
	else
	{
		while (!inFile1.eof())
		{
      string sLine ;
			getline(inFile1, sLine) ;
			if (string("") != sLine)
				sData += sLine + string(1, '\n') ;
		}

		inFile1.close() ;
	}

  size_t iDataLen = strlen(sData.c_str()) ;

	// boucle de chargement des attributs
  //
	size_t i = 0 ;
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

		if      ((sNomAttrib == "URLSMTP") && (string("") != sValAttrib))
			pMailParams->setUrlSmtp(sValAttrib) ;
		else if ((sNomAttrib == "PORTSMTP") && (string("") != sValAttrib))
      pMailParams->setPortSmtp(atoi((sValAttrib).c_str())) ;
    else if ((sNomAttrib == "USERSMTP") && (string("") != sValAttrib))
      pMailParams->setUserSmtp(sValAttrib) ;
    else if ((sNomAttrib == "PASSSMTP") && (string("") != sValAttrib))
      pMailParams->setPassSmtp(sValAttrib) ;
    else if (sNomAttrib == "AUTHENTICATE")
    {
      if (IsYes(sValAttrib))
        pMailParams->mustAuthenticate(true) ;
      else
        pMailParams->mustAuthenticate(false) ;
    }
    else if ((sNomAttrib == "NAMEFROM") && (string("") != sValAttrib))
      pMailParams->setName(sValAttrib) ;
    else if ((sNomAttrib == "MAILEXP") && (string("") != sValAttrib))
      pMailParams->setFrom(sValAttrib) ;
    else if ((sNomAttrib == "MAILSENDER") && (string("") != sValAttrib))
      pMailParams->setSender(sValAttrib) ;
    else if ((sNomAttrib == "MAILREPLYTO") && (string("") != sValAttrib))
      pMailParams->setReplyTo(sValAttrib) ;
    else if ((sNomAttrib == "URLPOP3") && (string("") != sValAttrib))
      pMailParams->setUrlPop3(sValAttrib) ;
    else if ((sNomAttrib == "PORTPOP3") && (string("") != sValAttrib))
      pMailParams->setPortPop3(atoi((sValAttrib).c_str())) ;
    else if ((sNomAttrib == "USERPOP3") && (string("") != sValAttrib))
      pMailParams->setUserPop3(sValAttrib) ;
    else if ((sNomAttrib == "PASSPOP3") && (string("") != sValAttrib))
      pMailParams->setPassPop3(sValAttrib) ;
    else if ((sNomAttrib == "APIKEYSDIR") && (string("") != sValAttrib))
      pMailParams->setApiKeyDir(sValAttrib) ;
    else if ((sNomAttrib == "USERAPICRYPT") && (string("") != sValAttrib))
      pMailParams->setApiUser(sValAttrib) ;
    else if (string("USEAPIPROXY") == sNomAttrib)
    {
      if (IsYes(sValAttrib))
        pMailParams->setUseApiProxy(true) ;
      else
        pMailParams->setUseApiProxy(false) ;
    }
    else if ((string("METHODE") == sNomAttrib) && (string("") != sValAttrib))
    {
      if      (sValAttrib == "0")
        pMailParams->setSendingModality(NSMAIL_NAUTILUS) ;
      else if (sValAttrib == "1")
        pMailParams->setSendingModality(NSMAIL_MAPI) ;
      else
      {
        sValAttrib = pseumaj(sValAttrib) ;
        if      (sValAttrib == "NAUTILUS")
          pMailParams->setSendingModality(NSMAIL_NAUTILUS) ;
        else if (sValAttrib == "MAPI")
          pMailParams->setSendingModality(NSMAIL_MAPI) ;
      }
    }
	}

	return true ;
}

void
NSUtilisateurChoisi::CmRequete()
{
try
{
	if (_pResult)
	{
		string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str() ;
		::MessageBox(0, "Vous devez fermer le dialogue des résultats avant de lancer une nouvelle requête.", sCaption.c_str(), MB_OK) ;
		return;
	}

	if ((NSRequeteDialog*) NULL == _pRequete)
		_pRequete = new NSRequeteDialog(pContexte->GetMainWindow(), pContexte) ;

	if (_pRequete->Execute() == IDCANCEL)
	{
		delete _pRequete ;
		_pRequete = (NSRequeteDialog*) 0 ;
		return ;
	}

	CmResult() ;
}
catch (...)
{
	erreur("Exception NSUtilisateurChoisi::CmRequete().", standardError, 0) ;
}
}

void
NSUtilisateurChoisi::CmResult()
{
try
{
	if ((NSResultReqDialog*) NULL == _pResult)
		_pResult = new NSResultReqDialog(pContexte->GetMainWindow(), pContexte, _pRequete) ;

	if (_pResult->Execute() == IDCANCEL) // cas Masquer : on ne fait rien
		return ;

  int iSelectedPatientIndex = _pResult->getSelectedItem() ;

	if (iSelectedPatientIndex > -1)
	{
    string sNss = string("") ;

    NSPatInfo* pSelectedPat = _pResult->getResultPatient(iSelectedPatientIndex) ;
    if (pSelectedPat)
      sNss = pSelectedPat->getNss() ;

		if (OuvrePatient(sNss))
		{
      bool bCanClosePatient = ChangePatientChoisi() ;

			if (bCanClosePatient && _pResult->isReqModeDoc())
			{
        pContexte->getPatient()->InitialisationAsynchronous(NSPatientChoisi::opnOpenReqResultDocum) ;
        return ;
			}
		}
	}
	else
	{
		delete _pResult ;
		_pResult = (NSResultReqDialog*) 0 ;
		// if (pRequete)
		// 	CmRequete() ;
	}
}
catch (...)
{
	erreur("Exception NSUtilisateurChoisi::CmResult().", standardError, 0) ;
}
}

void NSUtilisateurChoisi::CmEditRef()
{
try
{
/*
	NSRefEditeur* pDlg = new NSRefEditeur(pContexte->GetMainWindow(), pContexte);
	pDlg->Execute();
	delete pDlg;
*/
}
catch (...)
{
	erreur("Exception NSUtilisateurChoisi::CmEditRef().", standardError, 0) ;
}
}

void NSUtilisateurChoisi::CmEditArchetype()
{
try
{
/*
    NSArcEditeur* pDlg = new NSArcEditeur(pContexte->GetMainWindow(), pContexte);
    pDlg->Execute();
    delete pDlg;
*/
}
catch (...)
{
	erreur("Exception NSUtilisateurChoisi::CmEditArchetype().", standardError, 0) ;
}
}

void NSUtilisateurChoisi::CmEditTemplate()
{
try
{
	NSEditTemplateDialog* pDlg = new NSEditTemplateDialog(pContexte->GetMainWindow(), pContexte);
	if (pDlg->Execute() == IDOK)
	{
    NSObjectGraphManager GraphObject(pContexte->getSuperviseur()) ;
    string sRootId = GraphObject.setTree(pDlg->pPatPatho, "") ;
    GraphObject.setRootObject(sRootId) ;

    // Appel du pilote
    NSDataGraph* pGraph = GraphObject.getDataGraph() ;
    //pObectsList la liste d'objects qui correspondent a ces criteres
    NSPersonsAttributesArray ObjectsList ;

    // Elaboration des traits
    string user = "_000000" ;
    if (pContexte->getUtilisateur()!= NULL)
    	user = pContexte->getUtilisateurID() ;

    string sTraitType = string("_0OTPL") + string("_0TYPE") ;
    string sTraitOper = string("_0OTPL") + string("_DOPER") ;
    string sTraitCons = string("_0OTPL") + string("_LNUCO") ;
    string sTraitRoot = string("_0OTPL") + string("_0TYPC") ;
    string sTraitComp = string("_0OTPL") + string("_0COMD") ;
    string sTraitDefa = string("_0OTPL") + string("_0DEFA") ;

    NSBasicAttributeArray AttrList ;
    AttrList.push_back(new NSBasicAttribute("user",     user)) ;
    AttrList.push_back(new NSBasicAttribute(sTraitType, pDlg->sTypeDoc)) ;
    AttrList.push_back(new NSBasicAttribute(sTraitOper, pDlg->sOper)) ;
    AttrList.push_back(new NSBasicAttribute(sTraitCons, pDlg->sNoConsole)) ;
    AttrList.push_back(new NSBasicAttribute(sTraitRoot, pDlg->sLexiqueRoot)) ;
    AttrList.push_back(new NSBasicAttribute(sTraitComp, pDlg->sCompo)) ;
    AttrList.push_back(new NSBasicAttribute(sTraitDefa, pDlg->sDefaut)) ;

    pContexte->getSuperviseur()->getPilot()->createPersonOrObject(NautilusPilot::SERV_CREATE_OBJECT.c_str(),
                        pGraph, &ObjectsList, &AttrList, OBJECT_TYPE, false);

    if (!pGraph)
    {
    	string sErrorText = pContexte->getSuperviseur()->getText("templateManagement", "templateCreationError") ;
      pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
    	erreur(sErrorText.c_str(), standardError, 0) ;
      delete pDlg ;
      return ;
    }

    string sErrorText = pContexte->getSuperviseur()->getText("templateManagement", "templateSuccessfullyCreated") ;
    ::MessageBox(0, sErrorText.c_str(), "", MB_OK) ;
	}
  delete pDlg ;
}
catch (...)
{
	erreur("Exception NSUtilisateurChoisi::CmEditTemplate().", standardError, 0) ;
}
}


// -----------------------------------------------------------------------------
// TODO : cette méthode a semble-t-il été déconnecté en grande partie en N_TIERS
//        il reste des balises N_TIERS à l'intérieur de !N_TIERS
// Refactor programmé pour RS
// TODO FIXME -- RS
// --
// FLP
// -----------------------------------------------------------------------------
void
NSUtilisateurChoisi::CmNewEnTete()
{
try
{
  WIN32_FIND_DATA FileData ;
  HANDLE          hSearch ;  char            szMask[255] ;  bool            bFinish = false ;  bool            bExisteModele = false ;  size_t          pos, pos1, pos2 ;  string          sPathName, sFileName, sExtension ;  string          sHTM ;  DWORD           dwAttr ;  VecteurString   aFiles ;  VecteurString   aNames ;  pContexte->getSuperviseur()->afficheStatusMessage(string("Recherche de la liste des modeles")) ;  sPathName = pContexte->PathName("NTPL") ;  strcpy(szMask, sPathName.c_str()) ;  strcat(szMask, "*.*") ;
  hSearch = FindFirstFile(szMask, &FileData) ;  if (hSearch == INVALID_HANDLE_VALUE)
  {    erreur("Aucun fichier trouvé dans le répertoire template.", standardError, 0) ;    pContexte->getSuperviseur()->afficheStatusMessage(string("")) ;    return ;  }
  while (!bFinish)  {    dwAttr = FileData.dwFileAttributes ;
    if (!(dwAttr & FILE_ATTRIBUTE_DIRECTORY))
    {      sFileName = string(FileData.cFileName) ;      // Récupération de l'extension      pos = sFileName.find(".") ;    }
    if ((!(dwAttr & FILE_ATTRIBUTE_DIRECTORY)) && (pos != string::npos))    {      sExtension = string(sFileName, pos + 1, strlen(sFileName.c_str()) - pos - 1) ;      if ((sExtension == "htm") || (sExtension == "HTM") || (sExtension == "html") || (sExtension == "HTML"))
      {        pos1 = sFileName.find("modele") ;        if (pos1 != string::npos)        {          ifstream inFile ;          string   line ;          string   sModele, sTitre ;          string   sTagModele = string("{modele}") ;          // recherche du titre du modele (à afficher dans la liste)          sHTM = sPathName + sFileName ;          if (false == bExisteModele)            bExisteModele = true ;          inFile.open(sHTM.c_str()) ;          if (!inFile)
          {
            string sErrorText = "Erreur d'ouverture du fichier modèle " + sHTM ;
            pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
            erreur(sErrorText.c_str(), standardError, 0) ;
            return ;
          }
          while (!inFile.eof())
          {
            getline(inFile,line) ;
            if (line != "")
              sModele += line + "\n" ;
          }
          inFile.close() ;
          pos1 = sModele.find(sTagModele) ;          if (pos1 != string::npos)          {            pos1 += strlen(sTagModele.c_str()) ;            pos2 = sModele.find("{/modele}") ;            if (pos2 != string::npos)            {              sTitre = string(sModele, pos1, pos2 - pos1) ;            }            else            {              string sErrorText = string("Erreur de syntaxe du titre du fichier modèle ") + sHTM ;              pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
              erreur(sErrorText.c_str(), standardError, 0) ;              sTitre = string("modele : ") + sFileName ;            }          }          else          {            sTitre = string("modele : ") + sFileName ;          }          aFiles.AddString(sHTM) ;          aNames.AddString(sTitre) ;        }      }    }
    if (!FindNextFile(hSearch, &FileData))    {      if (GetLastError() == ERROR_NO_MORE_FILES)        bFinish = true ;      else      {        erreur("Impossible de trouver le fichier suivant dans le répertoire des templates.", standardError, 0) ;        pContexte->getSuperviseur()->afficheStatusMessage(string("")) ;        return ;      }    }  }

  pContexte->getSuperviseur()->afficheStatusMessage(string("")) ;

  if (false == bExisteModele)
  {
    erreur("Aucun fichier modele trouvé dans le répertoire des templates.", standardError, 0) ;
    return ;
  }

  ChoixDansListeDialog listDialog(pContexte->GetMainWindow(), pContexte, &aNames) ;
  if ((listDialog.Execute() == IDCANCEL) || (listDialog.itemChoisi == -1))
    return ;

  // pListeDialog->pListBox->GetString(str, pListeDialog->itemChoisi);
  // string modele = pContexte->PathName("NTPL") + string("modele.htm");
  string modele = aFiles.GetElementAt(listDialog.itemChoisi) ;
//  delete pListeDialog ;

  NSEnTeteEditDialog enteteDlg(pContexte->GetMainWindow(), pContexte, modele) ;
  if (enteteDlg.Execute() != IDOK)
  {
//    delete pDlg ;
    return ;
  }
//  delete pDlg ;

	string fichier = string("entete_") + pContexte->getUtilisateur()->getNss() + string(".htm") ;

  string sFirstCode, sLastCode, sNewCode, sCode ;
	// int    iNewCode ;
  // char   szNewCode[10] ;
  // bool   bPremier = true ;

	string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str() ;
	::MessageBox(0, "Nouvel en-tête créé avec succès.", sCaption.c_str(), MB_OK) ;
}
catch (...)
{
	erreur("Exception NSUtilisateurChoisi::CmNewEnTete().", standardError, 0) ;
}
}

void NSUtilisateurChoisi::CmModEnTete()
{
try
{
	string entete = pContexte->PathName("NTPL") + string("entete_") +
                    pContexte->getUtilisateur()->getNss() + string(".htm") ;

	NSEnTeteEditDialog* pDlg = new NSEnTeteEditDialog(pContexte->GetMainWindow(), pContexte, entete) ;
	if (pDlg->Execute() == IDOK)
	{
  	string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str() ;
    ::MessageBox(0, "En-tête modifié avec succès.", sCaption.c_str(), MB_OK) ;
  }
  delete pDlg ;
}
catch (...)
{
	erreur("Exception NSUtilisateurChoisi::CmModEnTete().", standardError, 0) ;
}
}

void NSUtilisateurChoisi::CmRefArchetype()
{
try
{
	// on choisi d'abord le répertoire par défaut des archetypes (IXML)
	string sPath = pContexte->PathName("IXML") ;

	TFileOpenDialog::TData initData(OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,
																	"Tous fichiers (*.XML)|*.xml|",
																	0,
																	(char*) sPath.c_str(),
																	"XML");

	int iUserChoice = TFileOpenDialog(pContexte->GetMainWindow(), initData).Execute() ;

	pContexte->reallocateBaseDirectory() ;

	if (IDOK != iUserChoice)
		return ;

	string sFileName = string(initData.FileName);
	string sFichArc, sPathArc ;

	size_t pos = sFileName.find_last_of("\\");
	if (NPOS != pos)
	{
		sPathArc = string(sFileName, 0, pos+1) ;
		sFichArc = string(sFileName, pos+1, strlen(sFileName.c_str())-pos-1) ;
	}
	else
	{
		sPathArc = "" ;
		sFichArc = sFileName ;
	}

	nsarcParseur ArcParseur(pContexte->getSuperviseur()) ;

	if (ArcParseur.open(sFileName))
	{
		string sRes = pContexte->getArcManager()->AjouteArchetype(pContexte, NSArcManager::archetype,
                                                    ArcParseur.getArchetype()->getName(),
                                                    sFichArc, sPathArc) ;
		if (string("") != sRes)
    	::MessageBox(0, "L'archetype est référencé dans la librairie XML.", "Message Nautilus", MB_OK) ;

    return ;
	}

  nsrefParseur RefParseur(pContexte->getSuperviseur()) ;

	if (RefParseur.open(sFileName))
	{
    string sTypeName   = string("") ;
    string sDomainName = string("") ;
    Creferences* pRefRef = RefParseur.getReferentiel()->getReference() ;
    if (pRefRef)
    {
      Cconcern *pConcern = pRefRef->getFirstCconcern() ;
      if (pConcern)
      {
        sTypeName   = pConcern->getCode() ;
        sDomainName = pConcern->getCategory() ;
      }
    }

    string sTitle = RefParseur.getReferentiel()->getTitle() ;

		string sRes = pContexte->getArcManager()->AjouteArchetype(pContexte, NSArcManager::referentiel,
                                                    RefParseur.getReferentiel()->getName(),
                                                    sFichArc, sPathArc, sTypeName,
                                                    sDomainName, sTitle) ;

		if (string("") != sRes)
			::MessageBox(0, "Le référentiel est référencé dans la librairie XML.", "Message Nautilus", MB_OK) ;

    return ;
	}

  nsGuidelineParseur DecisionTreeParseur(pContexte->getSuperviseur()) ;

  if (DecisionTreeParseur.open(sFileName))
	{
		string sRes = pContexte->getArcManager()->AjouteArchetype(pContexte, NSArcManager::decisionTree,
                                                    DecisionTreeParseur.getGuideline()->getName(),
                                                    sFichArc, sPathArc) ;
		if (string("") != sRes)
    	::MessageBox(0, "L'archetype est référencé dans la librairie XML.", "Message Nautilus", MB_OK) ;

    return ;
	}

  erreur("Le fichier XML sélectionné n'est ni un archétype, ni un référentiel valide.", standardError, 0) ;
}
catch (...)
{
	erreur("Exception NSUtilisateurChoisi::CmRefArchetype().", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------
// Function    : NSUtilisateurChoisi::debloqueConsole()
// Description : Débloque tous les patients qui n'ont pas été débloqués par une
//               précedente session de cette console.
// -----------------------------------------------------------------------------
void
NSUtilisateurChoisi::debloqueConsole()
{
  int iCurrentInstance = pContexte->getSuperviseur()->getInstance() ;

  for (int i = 0 ; i < 3 ; i++)
  {
	  NSBasicAttributeArray AttrList ;

    AttrList.push_back(new NSBasicAttribute(CONSOLE,  pContexte->getSuperviseur()->getConsoleString())) ;
    AttrList.push_back(new NSBasicAttribute(INSTANCE, IntToString(iCurrentInstance + i))) ;

    bool bReturn = pContexte->getSuperviseur()->getPilot()->unlockPatient(NautilusPilot::SERV_UNLOCK_ALL_PATIENTS.c_str(), &AttrList) ;
    if (false == bReturn)
    {
  	  string sWarningMsg = pContexte->getSuperviseur()->getPilot()->getWarningMessage() ;
      string sErrorMsg = pContexte->getSuperviseur()->getPilot()->getErrorMessage() ;
      if (string("") != sWarningMsg)
      {
    	  pContexte->getSuperviseur()->trace(&sWarningMsg, 1, NSSuper::trWarning) ;
    	  erreur(sWarningMsg.c_str(), warningError, 0) ;
      }
      if (string("") != sErrorMsg)
      {
    	  pContexte->getSuperviseur()->trace(&sErrorMsg, 1, NSSuper::trError) ;
    	  erreur(sErrorMsg.c_str(), standardError, 0) ;
      }
      return ;
    }
  }
}

//---------------------------------------------------------------------------
//  Function :		NSSuper::nouveauCR(string sCompteRendu)
//
//  Arguments :	sCompteRendu : string qui reçoit le CR
//
//  Description :	Génère un nouveau compte rendu.
//
//  Retour :		TRUE si OK, FALSE sinon
//---------------------------------------------------------------------------
bool
NSUtilisateurChoisi::nouveauCR(string* sCompteRendu)
{
	/*
	char codeModule[MODU_CODE_LEN+1], bigModule[MODU_BIG_LEN+1];
	char contexteModule;
	//
	// Lancement de la boite de dialogue de choix de module
	//
	ChoixCRDialog* ChoixCRDl =
		new ChoixCRDialog(pNSSuperviseur->pNSApplication->GetMainWindow());
	ChoixCRDl->Execute();
	//
	// Si un module a été choisi, on le lance
	//
	if (ChoixCRDl->EnteteCR[0] != '\0')
	{
		strcpy(codeModule, ChoixCRDl->EnteteCR);
		strcpy(bigModule, ChoixCRDl->NomScript);
		contexteModule = ChoixCRDl->Contexte;
		delete ChoixCRDl;
		return BigBrother('C', bigModule, sCompteRendu, "", pNSSuperviseur, codeModule, contexteModule);
	}
	else
	{
		delete ChoixCRDl;
		return FALSE;
	}
	*/
    return true ;
}

void
NSUtilisateurChoisi::RemplirIndex(NSPatPathoArray* pPPT)
{
}

void
NSUtilisateurChoisi::RemplirSynthese(NSPatPathoArray* pPPT)
{
}

void
NSUtilisateurChoisi::RemplirIdentifiant(NSPatPathoArray* pPPT)
{
}

void
NSUtilisateurChoisi::RemplirLibChem(NSPatPathoArray* pPPT)
{
}


// -----------------------------------------------------------------------------
// Function     : NSUtilisateurChoisi::InitNewData(NSUtiliInfo *pUtilInfo, bool bCreer)
// -----------------------------------------------------------------------------
// Arguments    : Les infos utilisateur à initialiser ou à modifer
// -----------------------------------------------------------------------------
// Description  : Appel du dialogue de création/modification d'un utilisateur
// -----------------------------------------------------------------------------
// Returns      : true->Les Data sont modifiées false->Sinon
// -----------------------------------------------------------------------------
bool
NSUtilisateurChoisi::InitNewData(NSUtiliInfo *pUtilInfo, bool bCreer, TWindow* parent)
{
	if ((NSUtiliInfo*) NULL == pUtilInfo)
		return false ;

	TModule *module = (TModule*) 0 ;
	if (parent)
		module = parent->GetModule() ;

  CreerUtilisateurDialog *pNewUtilDlg = new CreerUtilisateurDialog(parent, pContexte, module) ;

	if (false == bCreer)	// on est en modification
	{
  	// on regarde si le graphe n'est pas déjà chargé
    // sinon, on le charge ici avant le setGraph qui est dans Modifier()
    GetGraphUtil(pUtilInfo) ;
    *(pNewUtilDlg->pData) = *(pUtilInfo->getData()) ;

    pNewUtilDlg->setTitre(pUtilInfo->getTitre()) ;
    pNewUtilDlg->setCivilPro(pUtilInfo->getCivilProf()) ;
    pNewUtilDlg->setCivil(pUtilInfo->getCivil()) ;
    pNewUtilDlg->setLogin(pUtilInfo->getLogin()) ;
    pNewUtilDlg->setPasswd(pUtilInfo->getPasswd()) ;
    pNewUtilDlg->setUserType(pUtilInfo->getUserType()) ;
    pNewUtilDlg->setPassType(pUtilInfo->getPassType()) ;
    pNewUtilDlg->setPassDate(pUtilInfo->getPassDate()) ;
    pNewUtilDlg->setValidity(pUtilInfo->getValidity()) ;
	}

	if ((pNewUtilDlg->Execute()) == IDCANCEL)
	{
		delete pNewUtilDlg ;
		return false ;
	}

	// on stocke les donnees du dialogue dans les Data
	*(pUtilInfo->getData()) = *(pNewUtilDlg->pData) ;

	pUtilInfo->setTitre(pNewUtilDlg->getTitre()) ;
	pUtilInfo->setCivilProf(pNewUtilDlg->getCivilPro()) ;
	pUtilInfo->setCivil(pNewUtilDlg->getCivil()) ;
	pUtilInfo->setLogin(pNewUtilDlg->getLogin()) ;
	pUtilInfo->setPasswd(pNewUtilDlg->getPasswd()) ;
	pUtilInfo->setUserType(pNewUtilDlg->getUserType()) ;
	pUtilInfo->setPassType(pNewUtilDlg->getPassType()) ;
	pUtilInfo->setPassDate(pNewUtilDlg->getPassDate()) ;
	pUtilInfo->setValidity(pNewUtilDlg->getValidity()) ;

	delete pNewUtilDlg ;
	return true ;
}

// -----------------------------------------------------------------------------
// Function     : NSUtilisateurChoisi::Creer()
// Arguments    : none
// Description  : Création d'un nouvel utilisateur
// Returns      : true -> Succès ; false -> Echec
// -----------------------------------------------------------------------------
bool
NSUtilisateurChoisi::Creer(TWindow* parent)
{
	NSUtiliInfo NewUtilInfo(pContexte) ;
	string 		 sCode = "" ;

	// On met à jour les nouvelles Data (sauf code)
	if (false == InitNewData(&NewUtilInfo, true, parent))
		return true ;

  SetGraphUtil(&NewUtilInfo, true) ;
  // FIXME TODO -- FLP
  // here we have to insert new user in local medical team
  // il faut récupérer l'ID de la personne créée et l'ajouter dans l'Equipe Médicale locale (ou toutes les equipes medicales locales)
  string sNewUserID = NewUtilInfo.getGraph()->getPersonID() ; // FIXME TODO

  if (NewUtilInfo.isUser())
  {
    NSTeamSupervisor teamSupervisor(pContexte) ;
    vector<string>  vLocalTeams = teamSupervisor.getLocalTeams() ;
    if (false == vLocalTeams.empty())
    {
      NVLdVTemps	ldvCurDate ;	ldvCurDate.takeTime() ;
      NVLdVTemps	ldvNoLimit ;	ldvNoLimit.setNoLimit() ;

      for (vector<string>::iterator lTeamIter = vLocalTeams.begin() ; vLocalTeams.end() != lTeamIter ; lTeamIter++)
      {
        // récupération de l'objet Team et ajout de la nouvelle personne
        NSTeamGraphManager teamManager(pContexte->getSuperviseur()) ;
        NSPatPathoArray	Ppt(pContexte->getSuperviseur()) ;
        teamManager.getTeamGraph(*lTeamIter, &Ppt) ;

        NSMoralPerson MoralPerson(pContexte, &Ppt) ;
        NSMoralPersonManager mpManager(&MoralPerson) ;

        NSHTMMandateArray MandatesArray ;
        MandatesArray.push_back(new NSHealthTeamMandate(nearDist, 3, "", "", ldvCurDate.donneDateHeure(), ldvNoLimit.donneDateHeure(), "", "", NSHealthTeamMandate::user)) ;
        mpManager.addMember(new NSHealthTeamMember(sNewUserID, NSHealthTeamMember::person, &MandatesArray), NSHealthTeamMember::person) ;

        // + appel pilot pour modifier la Team
        teamManager.setTeamGraph(mpManager.getPatho(), pContexte->getUtilisateurID()) ;
      }
    }
  }

  // Create User specific directory and files
  //
  CreateUserFiles(sNewUserID) ;

	return true ;
}

// -----------------------------------------------------------------------------
// Function     : NSUtilisateurChoisi::Modifier()
// Arguments    :
// Description  : Remplace les valeurs de la fiche utilisateur
// Returns      : true->Succès false->Echec
// -----------------------------------------------------------------------------
bool
NSUtilisateurChoisi::Modifier(TWindow* parent)
{
	NSUtiliInfo NewUtilInfo(this) ;

  string sRootTree = NewUtilInfo.getGraph()->getRootTree() ;
//  NSDataGraph* pDataGraph = pNewUtilInfo->pGraphPerson->pDataGraph ;

	// On met à jour les nouvelles Data
  if (false == InitNewData(&NewUtilInfo, false, parent))
		return true ;

  SetGraphUtil(&NewUtilInfo, false) ;

	// on rafraichit les donnees en memoire
	_Donnees     = *(NewUtilInfo.getData()) ;
	_GraphPerson = NewUtilInfo._GraphPerson ;

  pContexte->setMainCaption() ;
	return true ;
}


bool
NSUtilisateurChoisi::ModifierAutreUtilisateur(TWindow* parent)
{
	// on présente d'abord à l'administrateur une liste des utilisateurs
	// s'il se choisit lui-même, on appelle Modifier()
	NSTPersonListDialog Liste(parent, pidsUtilisat, false, pContexte) ;
	if (Liste.Execute() != IDOK)
		return true ;

	string sNssUser = Liste.pPersonSelect->getNss() ;
	if (string("") == sNssUser)
		return false ;

	if (_GraphPerson.getPersonID() == sNssUser)
	{
		Modifier(parent) ;
		return true ;
	}

	// on récupère le login et le password de l'utilisateur à modifier
	const char* serviceName = (NautilusPilot::SERV_USER_PROPERTIES_LIST).c_str() ;

	NSBasicAttributeArray AttrList ;
	AttrList.push_back(new NSBasicAttribute(LOGIN,    _sLogin)) ;
	AttrList.push_back(new NSBasicAttribute(PASSWORD, _sPasswd)) ;
	AttrList.push_back(new NSBasicAttribute(PERSON,   sNssUser)) ;

  NSPersonsAttributesArray UserList ;

	bool res = pContexte->getSuperviseur()->getPilot()->getUserProperties(serviceName, &UserList, &AttrList) ;
	if (false == res)
	{
		string sErrorText = pContexte->getSuperviseur()->getText("NTIERS", "personNotFound") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError, 0) ;
		return false ;
	}

  NSUtiliInfo OtherUtilInfo(pContexte) ;
	OtherUtilInfo.setNss(sNssUser) ;

	//ajout de la balise PERSON dans pPatList
	OtherUtilInfo.setLogin(UserList.getAttributeValue(LOGIN)) ;
	OtherUtilInfo.setPasswd(UserList.getAttributeValue(PASSWORD)) ;
	OtherUtilInfo.setUserType(UserList.getAttributeValue(USERTYPE)) ;
	OtherUtilInfo.setPassType(UserList.getAttributeValue(PASSWDTYPE)) ;
	OtherUtilInfo.setPassDate(UserList.getAttributeValue(STARTDATE)) ;
	OtherUtilInfo.setValidity(UserList.getAttributeValue(DAYSNB)) ;

	// On met à jour les nouvelles Data
	// Note : dans cette fonction GetGraphUtil charge le graphe et remplit les pDonnees
	if (false == InitNewData(&OtherUtilInfo, false, parent))
		return true ;

	// on met à jour le graphe
	SetGraphUtil(&OtherUtilInfo, false) ;

	return true ;
}

bool
NSUtilisateurChoisi::SetGraphUtil(NSUtiliInfo* pUtilInfo, bool bCreer)
{
	if ((NSUtiliInfo*) NULL == pUtilInfo)
		return false ;

  // on construit d'abord une patpatho admin et pds pour l'utilisateur.
  NSPatPathoArray PPT(pContexte->getSuperviseur(), graphPerson) ;

  bool bRet ;

  if (bCreer)
  {
    PPT.ajoutePatho("ZADMI1", 0) ;
    PPT.ajoutePatho("LIDET1", 1) ;

    // nom de l'utilisateur
    PPT.ajoutePatho("LNOM01", 2) ;
    Message Msg ;
    Msg.SetTexteLibre(pUtilInfo->getNom()) ;
    PPT.ajoutePatho("£CL000", &Msg, 3) ;

    // prenom
    PPT.ajoutePatho("LNOM21", 2) ;
    Msg.Reset() ;
    Msg.SetTexteLibre(pUtilInfo->getPrenom()) ;
    PPT.ajoutePatho("£CL000", &Msg, 3) ;

    // sexe
    PPT.ajoutePatho("LSEXE1", 2) ;
    if (pUtilInfo->estMasculin())
    {
      PPT.ajoutePatho("HMASC2", 3) ;
      if (string("HMONP1") == pUtilInfo->getCivil())
      {
        PPT.ajoutePatho("HCIVO1", 4) ;
        PPT.ajoutePatho("HMONP1", 5) ;
      }
    }
    else if (pUtilInfo->estFeminin())
    {
      PPT.ajoutePatho("HFEMI2", 3) ;
      if (string("HMADR1") == pUtilInfo->getCivil())
      {
        PPT.ajoutePatho("HCIVO1", 4) ;
        PPT.ajoutePatho("HMADR1", 5) ;
      }
      else if (string("HMADE1") == pUtilInfo->getCivil())
      {
        PPT.ajoutePatho("HCIVO1", 4) ;
        PPT.ajoutePatho("HMADE1", 5) ;
      }
    }

    // construction de la patpatho professionnel de santé (point de vue)
    PPT.ajoutePatho("DPROS1", 0) ;

    // sous-chapitre "Langue"
    if (string("") != pUtilInfo->getLang())
		{
    	PPT.ajoutePatho("LLANG1", 1) ;

      Message localMessage ;
      localMessage.SetComplement(pUtilInfo->getLang()) ;
      PPT.ajoutePatho("663911", &localMessage, 2) ;
    }

    // sous-chapitre "Compétences médicales"
    PPT.ajoutePatho("LCOMP1", 1) ;

    // Métier
    if (string("") != pUtilInfo->getJob())
    {
      PPT.ajoutePatho("LMETI1", 2) ;
      PPT.ajoutePatho(pUtilInfo->getJob(), 3) ;
    }

    // Spécialité
    if (string("") != pUtilInfo->getSpecialty())
    {
      PPT.ajoutePatho("LSPEC1", 2) ;
      PPT.ajoutePatho(pUtilInfo->getSpecialty(), 3) ;
    }

    // titre
    if (string("") != pUtilInfo->getTitre())
    {
      PPT.ajoutePatho("LTITR1", 1) ;
      PPT.ajoutePatho(pUtilInfo->getTitre(), 2) ;
    }

    // civilité professionnelle
    if (string("") != pUtilInfo->getCivilProf())
    {
      PPT.ajoutePatho("HCIVO1", 1) ;
      PPT.ajoutePatho(pUtilInfo->getCivilProf(), 2) ;
    }

    // ADELI et RPPS
    if ((string("") != pUtilInfo->getProId()) || (string("") != pUtilInfo->getRpps()))
    {
      PPT.ajoutePatho("LIPRO1", 1) ;
      if (string("") != pUtilInfo->getProId())
      {
        PPT.ajoutePatho("LADEL1", 2) ;
        Message localMessage ;
        localMessage.SetTexteLibre(pUtilInfo->getProId()) ;
        PPT.ajoutePatho("£CL000", &localMessage, 3) ;
      }
      if (string("") != pUtilInfo->getRpps())
      {
        PPT.ajoutePatho("LRPPS1", 2) ;
        Message localMessage ;
        localMessage.SetTexteLibre(pUtilInfo->getRpps()) ;
        PPT.ajoutePatho("£CL000", &localMessage, 3) ;
      }
    }

    // e-mail
    if (string("") != pUtilInfo->getMail())
    {
      PPT.ajoutePatho("ULIEX1", 1) ;
      PPT.ajoutePatho("LMAIL1", 2) ;
      Message localMessage ;
      localMessage.SetTexteLibre(pUtilInfo->getMail()) ;
      PPT.ajoutePatho("£CL000", &localMessage, 3) ;
    }
  }
  else // cas de la modification
  {
    NSDataTreeIter iterTree ;
    NSDataGraph* pDataGraph = pUtilInfo->getGraph()->getDataGraph() ;

    if (pDataGraph->getTrees()->ExisteTree("ZADMI1", pContexte->getSuperviseur(), &iterTree))
      (*iterTree)->getRawPatPatho(&PPT) ;
    else
    {
      string sErrorText = string("Impossible de trouver l'arbre [ZADMI1] dans le graphe administratif.") ;
      pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
      erreur(sErrorText.c_str(), standardError, 0) ;
      return false ;
    }

    if (pDataGraph->getTrees()->ExisteTree("DPROS1", pContexte->getSuperviseur(), &iterTree))
    {
      NSPatPathoArray PatPathoPDS(pContexte->getSuperviseur()) ;
      (*iterTree)->getRawPatPatho(&PatPathoPDS) ;
      PPT.InserePatPatho(PPT.end(), &PatPathoPDS, 0, true) ;
    }

    // on modifie les éléments dans la patpatho
    PatPathoIter iter ;
    if (PPT.CheminDansPatpatho("ZADMI1/LIDET1/LNOM01/£CL000", string(1, cheminSeparationMARK), &iter))
      (*iter)->setTexteLibre(pUtilInfo->getNom()) ;
    else
    {
      Message Msg ;
      Msg.SetLexique("£CL000") ;
      Msg.SetTexteLibre(pUtilInfo->getNom()) ;
      PPT.AjouterChemin("ZADMI1/LIDET1/LNOM01", &Msg) ;
    }

    if (PPT.CheminDansPatpatho("ZADMI1/LIDET1/LNOM21/£CL000", string(1, cheminSeparationMARK), &iter))
      (*iter)->setTexteLibre(pUtilInfo->getPrenom()) ;
    else
    {
      Message Msg ;
      Msg.SetLexique("£CL000") ;
      Msg.SetTexteLibre(pUtilInfo->getPrenom()) ;
      PPT.AjouterChemin("ZADMI1/LIDET1/LNOM21", &Msg) ;
    }

    if (pUtilInfo->estMasculin())
    {
      if (false == PPT.CheminDansPatpatho("ZADMI1/LIDET1/LSEXE1/HMASC2"))
        PPT.AjouterChemin("ZADMI1/LIDET1/LSEXE1", "HMASC2") ;

      if (string("HMONP1") == pUtilInfo->getCivil())
      {
        if (false == PPT.CheminDansPatpatho("ZADMI1/LIDET1/LSEXE1/HMASC2/HCIVO1/HMONP1"))
          PPT.AjouterChemin("ZADMI1/LIDET1/LSEXE1/HMASC2/HCIVO1", "HMONP1") ;
      }
      else
      {
        if (PPT.CheminDansPatpatho("ZADMI1/LIDET1/LSEXE1/HMASC2/HCIVO1", string(1, cheminSeparationMARK), &iter))
          PPT.SupprimerItem(iter) ;
      }
    }
    else if (pUtilInfo->estFeminin())
    {
      if (false == PPT.CheminDansPatpatho("ZADMI1/LIDET1/LSEXE1/HFEMI2"))
        PPT.AjouterChemin("ZADMI1/LIDET1/LSEXE1", "HFEMI2") ;

      if (string("HMADR1") == pUtilInfo->getCivil())
      {
        if (false == PPT.CheminDansPatpatho("ZADMI1/LIDET1/LSEXE1/HFEMI2/HCIVO1/HMADR1"))
          PPT.AjouterChemin("ZADMI1/LIDET1/LSEXE1/HFEMI2/HCIVO1", "HMADR1") ;
      }
      else if (string("HMADE1") == pUtilInfo->getCivil())
      {
        if (false == PPT.CheminDansPatpatho("ZADMI1/LIDET1/LSEXE1/HFEMI2/HCIVO1/HMADE1"))
          PPT.AjouterChemin("ZADMI1/LIDET1/LSEXE1/HFEMI2/HCIVO1", "HMADE1") ;
      }
      else
      {
        if (PPT.CheminDansPatpatho("ZADMI1/LIDET1/LSEXE1/HFEMI2/HCIVO1", string(1, cheminSeparationMARK), &iter))
          PPT.SupprimerItem(iter) ;
      }
    }

		// Langue
    if (string("") != pUtilInfo->getLang())
    {
    	if (PPT.CheminDansPatpatho("DPROS1/LLANG1/663911", string(1, cheminSeparationMARK), &iter))
      	(*iter)->setComplement(pUtilInfo->getLang()) ;
    	else
    	{
      	Message Msg ;
      	Msg.SetLexique("663911") ;
      	Msg.SetComplement(pUtilInfo->getLang()) ;
      	PPT.AjouterChemin("DPROS1/LLANG1", &Msg) ;
      }
    }
    else
    {
    	if (PPT.CheminDansPatpatho("DPROS1/LLANG1/663911", string(1, cheminSeparationMARK), &iter))
        PPT.SupprimerItem(iter) ;
    }

    // Métier
    if (string("") != pUtilInfo->getJob())
    {
      string sCheminMetier = string("DPROS1/LCOMP1/LMETI1/") + pUtilInfo->getJob() ;
      if (false == PPT.CheminDansPatpatho(sCheminMetier))
        PPT.AjouterChemin("DPROS1/LCOMP1/LMETI1", pUtilInfo->getJob()) ;
    }
    else
    {
      if (PPT.CheminDansPatpatho("DPROS1/LCOMP1/LMETI1", string(1, cheminSeparationMARK), &iter))
        PPT.SupprimerItem(iter) ;
    }

    // Spécialité
    if (string("") != pUtilInfo->getSpecialty())
    {
      string sCheminSpec = string("DPROS1/LCOMP1/LSPEC1/") + pUtilInfo->getSpecialty() ;
      if (false == PPT.CheminDansPatpatho(sCheminSpec))
        PPT.AjouterChemin("DPROS1/LCOMP1/LSPEC1", pUtilInfo->getSpecialty()) ;
    }
    else
    {
      if (PPT.CheminDansPatpatho("DPROS1/LCOMP1/LSPEC1", string(1, cheminSeparationMARK), &iter))
        PPT.SupprimerItem(iter) ;
    }

    // Titre
    if (string("") != pUtilInfo->getTitre())
    {
      string sCheminTitre = string("DPROS1/LTITR1/") + pUtilInfo->getTitre() ;
      if (false == PPT.CheminDansPatpatho(sCheminTitre))
        PPT.AjouterChemin("DPROS1/LTITR1", pUtilInfo->getTitre()) ;
    }
    else
    {
      if (PPT.CheminDansPatpatho("DPROS1/LTITR1", string(1, cheminSeparationMARK), &iter))
        PPT.SupprimerItem(iter) ;
    }

    // Civilite professionnelle
    if (string("") != pUtilInfo->getCivilProf())
    {
      string sCheminCivp = string("DPROS1/HCIVO1/") + pUtilInfo->getCivilProf() ;
      if (false == PPT.CheminDansPatpatho(sCheminCivp))
        PPT.AjouterChemin("DPROS1/HCIVO1", pUtilInfo->getCivilProf()) ;
    }
    else
    {
      if (PPT.CheminDansPatpatho("DPROS1/HCIVO1", string(1, cheminSeparationMARK), &iter))
        PPT.SupprimerItem(iter) ;
    }

    // ADELI
    if (PPT.CheminDansPatpatho("DPROS1/LIPRO1/LADEL1/£CL000", string(1, cheminSeparationMARK), &iter))
      (*iter)->setTexteLibre(pUtilInfo->getProId()) ;
    else
    {
      Message Msg ;
      Msg.SetLexique("£CL000") ;
      Msg.SetTexteLibre(pUtilInfo->getProId()) ;
      PPT.AjouterChemin("DPROS1/LIPRO1/LADEL1", &Msg) ;
    }

    // RPPS
    if (PPT.CheminDansPatpatho("DPROS1/LIPRO1/LRPPS1/£CL000", string(1, cheminSeparationMARK), &iter))
      (*iter)->setTexteLibre(pUtilInfo->getRpps()) ;
    else
    {
      Message Msg ;
      Msg.SetLexique("£CL000") ;
      Msg.SetTexteLibre(pUtilInfo->getRpps()) ;
      PPT.AjouterChemin("DPROS1/LIPRO1/LRPPS1", &Msg) ;
    }

    // messagerie
    if (PPT.CheminDansPatpatho("DPROS1/ULIEX1/LMAIL1/£CL000", string(1, cheminSeparationMARK), &iter))
      (*iter)->setTexteLibre(pUtilInfo->getMail()) ;
    else
    {
      Message Msg ;
      Msg.SetLexique("£CL000") ;
      Msg.SetTexteLibre(pUtilInfo->getMail()) ;
      PPT.AjouterChemin("DPROS1/ULIEX1/LMAIL1", &Msg) ;
    }
  }

  // on construit maintenant le graphe pour le pilote
  string sLogin = pUtilInfo->getLogin() ;
  string sPasswd = pUtilInfo->getPasswd() ;

  string sNss ;

  if (bCreer)
    sNss = string(1, INMEMORY_CHAR) + string(PAT_NSS_LEN - 1, '0') ;
  else
    sNss = pUtilInfo->getGraph()->getPersonID() ;

  // On met à jour le graphe de l'utilisateur de pUtilInfo
  NSBasicAttributeArray AttrList ;
  AttrList.push_back(new NSBasicAttribute(LOGIN,      sLogin)) ;
  AttrList.push_back(new NSBasicAttribute(PASSWORD,   sPasswd)) ;
  AttrList.push_back(new NSBasicAttribute(PERSON,     sNss)) ;
  AttrList.push_back(new NSBasicAttribute(ADELI_ID,   pUtilInfo->getProId())) ;

  AttrList.push_back(new NSBasicAttribute(OPERATOR,   pContexte->getUtilisateurID())) ;
  AttrList.push_back(new NSBasicAttribute(USERTYPE,   pUtilInfo->getUserType())) ;
  AttrList.push_back(new NSBasicAttribute(PASSWDTYPE, pUtilInfo->getPassType())) ;
  if (string(pUtilInfo->getValidity()) != "")
  	AttrList.push_back(new NSBasicAttribute(DAYSNB,   pUtilInfo->getValidity())) ;

  AttrList.push_back(new NSBasicAttribute(FIRST_NAME, pUtilInfo->getPrenom())) ;
  AttrList.push_back(new NSBasicAttribute(LAST_NAME,  pUtilInfo->getNom())) ;

  pUtilInfo->getGraph()->setInfoPids(&AttrList) ;

  string sContrib = string("") ;
  if (false == bCreer)
    pContexte->getPersonArray()->createContribution(pContexte, pUtilInfo->getGraph(), pidsUtilisat) ;

  bRet = pUtilInfo->getGraph()->setGraphAdmin(pContexte, &PPT, pidsUtilisat) ;

  if (false == bCreer)
    pContexte->getPersonArray()->closeContribution(pContexte, pUtilInfo->getGraph(), sContrib) ;

  // TODO FIXME -- FLP
  // en fonction de pUtilInfo->sUserType on doit ajouter ou retirer
  // l'utilisateur de l'Equipe Médicale Locale

  // il faut récupérer l'ID de la personne créée et l'ajouter dans l'Equipe Médicale locale (ou toutes les equipes medicales locales)
  //
  NSTeamSupervisor teamSupervisor(pContexte) ;
  vector<string> vLocalTeams = teamSupervisor.getLocalTeams() ;
  if (false == vLocalTeams.empty())
  {
    if ((pUtilInfo->getUserType().find(ADMIN_ROLE) != string::npos) && (pUtilInfo->getUserType().find(USER_ROLE) == string::npos))
    {
      // si seulement administrateur --> retirer s'il existe de l'Equipe Médicale Locale
      // ==> retirer de l'Equipe Medicale Locale (s'il y est déjà)

      for (vector<string>::iterator lTeamIter = vLocalTeams.begin() ; vLocalTeams.end() != lTeamIter ; lTeamIter++)
      {
        // récupération de l'objet Team et ajout de la nouvelle personne
        NSTeamGraphManager teamManager(pContexte->getSuperviseur()) ;
        NSPatPathoArray	ppt(pContexte->getSuperviseur()) ;
        teamManager.getTeamGraph(*lTeamIter, &ppt) ;

        NSMoralPerson MoralPerson(pContexte, &ppt) ;
        NSMoralPersonManager mpManager(&MoralPerson) ;

        mpManager.closeAllMandates(pContexte, sNss) ;

        // + appel pilot pour modifier la Team
        teamManager.setTeamGraph(mpManager.getPatho(), pContexte->getUtilisateurID()) ;
      }
    }
    else if (pUtilInfo->getUserType().find(USER_ROLE) != string::npos)
    {
      // si au moins utilisateur --> ajouter (s'il n'y est pas déjà) de l'Equipe Médicale Locale
      // ==> ajouter à l'Equipe Médicale Locale (s'il n'y est pas déjà)

      for (vector<string>::iterator lTeamIter = vLocalTeams.begin() ; lTeamIter != vLocalTeams.end() ; lTeamIter++)
      {
        // récupération de l'objet Team et ajout de la nouvelle personne
        NSTeamGraphManager teamManager(pContexte->getSuperviseur()) ;
        NSPatPathoArray	ppt(pContexte->getSuperviseur()) ;
        teamManager.getTeamGraph(*lTeamIter, &ppt) ;

        NSMoralPerson MoralPerson(pContexte, &ppt) ;
        NSMoralPersonManager mpManager(&MoralPerson) ;

        mpManager.addMember(pContexte, sNss) ;

        // + appel pilot pour modifier la Team
        teamManager.setTeamGraph(mpManager.getPatho(), pContexte->getUtilisateurID()) ;
      }
    }
  }

  return bRet ;
}

bool
NSUtilisateurChoisi::GetGraphUtil(NSUtiliInfo* pUtilInfo)
{
try
{
	NSPatPathoArray PatPathoArray(pContexte->getSuperviseur(), graphPerson) ;
	bool bGraphOK = false ;

	PatPathoIter    iter ;
	string          sElemLex, sSens, sType, sTemp ;
	string lang = "" ;
//=============================================================================
//il faut recuperer la langue d'utilisateur de la base ou de l'interface
//Voir dans la racine du DPROS!!!!
//=============================================================================
	string sLang  = donneLang() ;
	if (sLang == "")
	{
		sLang ="fr" ;
		pUtilInfo->setLang(sLang) ;
	}

	string sNom    = "" , sPrenom = "" , sSexe   = "" ;
	string sCivilite = "", sTitre = "", sCivilProf = "", sEMail = "";
	string sMetier = "", sSpecialite = "", sADELI = string(""), sRPPS = string("") ;

/********************************
#ifdef N_TIERS
     //string sIdRoot = pGraphPerson->pDataGraph->getGraphID();
        if(!bCreat)
            return true;
        string sTreeRoot = pGraphPerson->pDataGraph->getGraphID();
        if (pGraphPerson->getGraphAdmin(sTreeRoot, pidsUtilisat, pPatPathoArray))
            bGraphOK = true;
#else
*********************************/

	// on regarde si le graphe n'est pas déjà chargé
	if (pUtilInfo->getGraph()->getRootTree() == "")
	{
		string sPids = pUtilInfo->getNss() ;
		if (pUtilInfo->getGraph()->getGraphAdmin(sPids, true, pidsUtilisat, &PatPathoArray))
			bGraphOK = true ;
	}
	else // on ne sort pas pour rafraichir quand même les pDonnees (obligatoire à l'ouverture)
	{
		NSDataTreeIter iterTree ;
		NSDataGraph* pDataGraph = pUtilInfo->getGraph()->getDataGraph() ;

		if (pDataGraph->getTrees()->ExisteTree("ZADMI1", pContexte->getSuperviseur(), &iterTree))
		{
			(*iterTree)->getRawPatPatho(&PatPathoArray);
		}
		else
		{
			string sErrorText = string("Impossible de trouver l'arbre [ZADMI1] dans le graphe administratif.") ;
      pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
			erreur(sErrorText.c_str(), standardError, 0) ;
			return false ;
		}

		if (pDataGraph->getTrees()->ExisteTree("DPROS1", pContexte->getSuperviseur(), &iterTree))
		{
			NSPatPathoArray PatPathoPDS(pContexte->getSuperviseur()) ;
			(*iterTree)->getRawPatPatho(&PatPathoPDS) ;
			PatPathoArray.InserePatPatho(PatPathoArray.end(), &PatPathoPDS, 0) ;
		}

		bGraphOK = true;
	}

	if (bGraphOK == false)
		return false ;

	iter = PatPathoArray.begin() ;
	int iColBase = (*iter)->getColonne() ;

	while ((iter != PatPathoArray.end()) && ((*iter)->getColonne() >= iColBase))
	{
		sElemLex = (*iter)->getLexique() ;
		pContexte->getDico()->donneCodeSens(&sElemLex, &sSens) ;

    if (sSens == string("ZADMI"))
    {
    	iter++ ;

      while ((iter != PatPathoArray.end()) && ((*iter)->getColonne() > iColBase))
      {
      	sElemLex = (*iter)->getLexique() ;
        pContexte->getDico()->donneCodeSens(&sElemLex, &sSens) ;

        // Chapitre "identité" / Identity chapter
        if (string("LIDET") == sSens)
        {
        	int iColIdentity = (*iter)->getColonne() ;
          iter++ ;

          while ((iter != PatPathoArray.end()) && ((*iter)->getColonne() > iColIdentity))
          {
          	sElemLex = (*iter)->getLexique() ;
            pContexte->getDico()->donneCodeSens(&sElemLex, &sSens) ;

            // Nom de l'utilisateur
            if ((sSens == string("LNOM0")) && (sNom == ""))
            {
              sNom = PatPathoArray.getFreeTextValue(&iter) ;

              if (string("") != sNom)
              	pUtilInfo->setNom(sNom) ;
            }
            // Prénom de l'utilisateur
            else if ((sSens == string("LNOM2")) && (sPrenom == ""))
            {
              sPrenom = PatPathoArray.getFreeTextValue(&iter) ;

              if (string("") != sPrenom)
              	pUtilInfo->setPrenom(sPrenom) ;
            }
            // Sexe de l'utilisateur
            else if (sSens == string("LSEXE"))
            {
            	iter++ ;
              while ((iter != PatPathoArray.end()) && ((*iter)->getColonne() > iColIdentity+1))
              {
              	// on cherche ici un code lexique
                sElemLex = (*iter)->getLexique() ;
                pContexte->getDico()->donneCodeSens(&sElemLex, &sTemp) ;

                sSexe = sTemp ;

                iter++ ;

                while ((iter != PatPathoArray.end()) && ((*iter)->getColonne() > iColIdentity + 2))
                {
                	sElemLex = (*iter)->getLexique() ;
                  pContexte->getDico()->donneCodeSens(&sElemLex, &sSens) ;

                  // Civilité
                  if (sSens == string("HCIVO"))
                  {
                  	iter++ ;
                    while ((iter != PatPathoArray.end()) && ((*iter)->getColonne() > iColIdentity + 3))
                    {
                    	// on cherche ici un code lexique pour un libelle
                      string sCodeLex = (*iter)->getLexique() ;
                      sCivilite = sCodeLex ;
                      iter++ ;
                    }
                  }
                  else
                  	iter++ ;
                }
              }

              if (sSexe != "")
              	pUtilInfo->setSexe(sSexe) ;

              if (sCivilite != "")
              	pUtilInfo->setCivil(sCivilite) ;
            }
            else
            	iter++ ;
          }
        }
        else
        	iter++ ;
      } // while
    } // if (sSens == string("ZADMI"))
    else if (sSens == string("DPROS"))
    {
    	iter++ ;

      while ((iter != PatPathoArray.end()) && ((*iter)->getColonne() > iColBase))
      {
      	sElemLex = (*iter)->getLexique() ;
        pContexte->getDico()->donneCodeSens(&sElemLex, &sSens) ;

        // Chapitre "Langue" / Langage chapter
        //
        if (sSens == string("LLANG"))
        {
        	iter++ ;
          while ((iter != PatPathoArray.end()) && ((*iter)->getColonne() > iColBase + 1))
          {
          	// on cherche ici un texte libre
            sElemLex = (*iter)->getLexique() ;
            pContexte->getDico()->donneCodeSens(&sElemLex, &sSens) ;

            if (sSens == string("66391"))
            	lang = (*iter)->getComplement() ;
            iter++ ;
          }
        }

        // Chapitre "Compétences" / Competence chapter
        //
        else if (sSens == string("LCOMP"))
        {
        	int iColCompetences = (*iter)->getColonne() ;
          iter++ ;

          while ((iter != PatPathoArray.end()) && ((*iter)->getColonne() > iColCompetences))
          {
          	sElemLex = (*iter)->getLexique() ;
            pContexte->getDico()->donneCodeSens(&sElemLex, &sSens) ;

            // metier
            if ((sSens == string("LMETI")) && (sMetier == ""))
            {
            	// on cherche ici un edit lexique (ayant comme fils un code lexique)
              iter++ ;
              sElemLex = (*iter)->getLexique() ;
              if (sElemLex != "")
              	if (string(sElemLex, 0, 3) == string("£C;"))
                {
                	iter++ ;
                  while ((iter != PatPathoArray.end()) && ((*iter)->getColonne() > iColCompetences+2))
                  {
                  	// on cherche ici un code lexique pour un libelle
                    string sCode = string((*iter)->getLexique()) ;
                    pContexte->getDico()->donneLibelle(sLang, &sCode, &sMetier) ;

                    iter++ ;
                  }
                }
                else
                {
                	sMetier = (*iter)->getLexique() ;
                  if (iter != PatPathoArray.end())
                  	iter++ ;
                }
              else
              	iter++ ;

              if (sMetier != "")
              	pUtilInfo->setJob(sMetier) ;
            }
            // Specilité de l'utilisateur
            else if ((sSens == string("LSPEC")) && (sSpecialite == ""))
            {
            	// on cherche ici un edit lexique (ayant comme fils un code lexique)
              iter++ ;
              sElemLex = (*iter)->getLexique() ;
              if (sElemLex != "")
              	if (string(sElemLex, 0, 3) == string("£C;"))
                {
                	iter++ ;
                  while ((iter != PatPathoArray.end()) && ((*iter)->getColonne() > iColCompetences+2))
                  {
                  	// on cherche ici un code lexique pour un libelle
                    string sCode = string((*iter)->getLexique()) ;
                    pContexte->getDico()->donneLibelle(sLang, &sCode, &sSpecialite) ;

                    iter++ ;
                  }
                }
                else
                {
                	sSpecialite = string((*iter)->getLexique()) ;
                  if (iter != PatPathoArray.end())
                  	iter++ ;
                }
              else    //si "" on avanse
              	iter++ ;

              if (sSpecialite != "")
              	pUtilInfo->setSpecialty(sSpecialite) ;
            }
            else
            	iter++ ;
          }
        }
        // Chapitre "titre"
        else if (sSens == string("LTITR"))
        {
        	int iColTitre = (*iter)->getColonne() ;
          iter++ ;
          while ((iter != PatPathoArray.end()) && ((*iter)->getColonne() > iColTitre))
          {
          	// on cherche ici un code lexique pour un libelle
            sTitre = string((*iter)->getLexique()) ;
            // pContexte->getDico()->donneLibelle(sLang, &sCodeLex, &sCivilite) ;

            iter++ ;
          }

          if (sTitre != "")
          	pUtilInfo->setTitre(sTitre) ;
        }
        // Chapitre "civilité"
        else if (sSens == string("HCIVO"))
        {
        	int iColCivilite = (*iter)->getColonne() ;
          iter++ ;
          while ((iter != PatPathoArray.end()) && ((*iter)->getColonne() > iColCivilite))
          {
          	// on cherche ici un code lexique pour un libelle
            sCivilProf = (*iter)->getLexique() ;
            // pContexte->getDico()->donneLibelle(sLang, &sCodeLex, &sCivilite) ;

            iter++ ;
          }

          if (sCivilProf != "")
          	pUtilInfo->setCivilProf(sCivilProf) ;

                    /*******************************************
                    if (sCivilite != "")
                    {
                        if (sCivilite == string("HDOCT1"))
                            strcpy(pUtilInfo->pDonnees->type, "O");
                        else if (sCivilite == string("HPROF1"))
                            strcpy(pUtilInfo->pDonnees->type, "P");
                        else if (sCivilite == string("HMOND1"))
                            strcpy(pUtilInfo->pDonnees->type, "1");
                        else if (sCivilite == string("HMADD1"))
                            strcpy(pUtilInfo->pDonnees->type, "2");
                        else if (sCivilite == string("HMONF1"))
                            strcpy(pUtilInfo->pDonnees->type, "R");
                        else if (sCivilite == string("HMADP1"))
                            strcpy(pUtilInfo->pDonnees->type, "M");
                        else if (sCivilite == string("HMONP1"))
                            strcpy(pUtilInfo->pDonnees->type, "A");
                        else if (sCivilite == string("HMADR1"))
                            strcpy(pUtilInfo->pDonnees->type, "B");
                        else if (sCivilite == string("HMADE1"))
                            strcpy(pUtilInfo->pDonnees->type, "C");
                    }
                    *******************************************/
        }
        // "professional ID" chapter
        else if (string("LIPRO") == sSens)
        {
        	int iColLiex = (*iter)->getColonne() ;
          iter++ ;

          while ((iter != PatPathoArray.end()) && ((*iter)->getColonne() > iColLiex))
          {
          	sElemLex = (*iter)->getLexique() ;
            pContexte->getDico()->donneCodeSens(&sElemLex, &sSens) ;

            // ADELI
            if ((string("LADEL") == sSens) && (string("") == sADELI))
            {
              sADELI = PatPathoArray.getFreeTextValue(&iter) ;

              if (string("") != sADELI)
              	pUtilInfo->setProId(sADELI) ;
            }
            // RPPS
            else if ((string("LRPPS") == sSens) && (string("") == sRPPS))
            {
              sRPPS = PatPathoArray.getFreeTextValue(&iter) ;

              if (string("") != sRPPS)
              	pUtilInfo->setRpps(sRPPS) ;
            }
            else
              	iter++ ;
          } // while
        }
        // Chapitre "lieu d'exercice"
        else if (string("ULIEX") == sSens)
        {
        	int iColLiex = (*iter)->getColonne() ;
          iter++ ;

          while ((iter != PatPathoArray.end()) && ((*iter)->getColonne() > iColLiex))
          {
          	sElemLex = (*iter)->getLexique() ;
            pContexte->getDico()->donneCodeSens(&sElemLex, &sSens) ;

            // Nom de l'utilisateur
            if ((sSens == string("LMAIL")) && (sEMail == ""))
            {
              sEMail = PatPathoArray.getFreeTextValue(&iter) ;

              if (sEMail != "")
              	pUtilInfo->setMail(sEMail) ;
            }
            else
              	iter++ ;
          } // while
        }
        else
        	iter++ ;
      } // while
    } // else if (sSens == string("DPROS"))
    else
    	iter++ ;
  } // while de base

  //la langue par defaut = "fr"
  if (string("") == lang)
  	pUtilInfo->setLang("fr") ;
  else
  	pUtilInfo->setLang(lang) ;

 	return true ;
}
catch (...)
{
	erreur("Exception NSUtilisateurChoisi::GetGraphUtil.", standardError, 0) ;
  return false ;
}
}

void
NSUtilisateurChoisi::CreateUserFiles(string sUserID)
{
	if (string("") == sUserID)
  	sUserID = getID() ;

	string sGlobalPersoDir = pContexte->PathName("FGLO") ;
  char cDirSeparator = sGlobalPersoDir[strlen(sGlobalPersoDir.c_str()) - 1] ;
	string sPersoDir = sGlobalPersoDir + sUserID + string(1, cDirSeparator) ;

  // First, create user specific Directory
  //
  if (strlen(sPersoDir.c_str()) > MAX_PATH)
  	sPersoDir = string("\\\\?\\") + sPersoDir ;

  BOOL bSuccess = ::CreateDirectory(sPersoDir.c_str(), NULL) ;
  if (false == bSuccess)
	{
  	string sErrorText = pContexte->getSuperviseur()->getText("fileErrors", "errorCreatingDirectory") ;
    sErrorText += string(" ") + sPersoDir ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
    return ;
  }

  // Then, copy user specific files
  //
  ifstream inFile ;
  string sFileListFile = sGlobalPersoDir + "userFiles.txt" ;

  inFile.open(sFileListFile.c_str()) ;
	if (!inFile)
  {
  	string sErrMess = pContexte->getSuperviseur()->getText("fileErrors", "errorOpeningInputFile") + string(" ") + sFileListFile ;
    pContexte->getSuperviseur()->trace(&sErrMess, 1, NSSuper::trError) ;
    erreur(sErrMess.c_str(), standardError, 0) ;
    return ;
  }

  string sLine ;
	while (!inFile.eof())
	{
		getline(inFile, sLine) ;
		if (string("") != sLine)
    {
    	string sModelFileName = sGlobalPersoDir + sLine ;
      string sUserFileName  = sPersoDir + sLine ;
    	bSuccess = ::CopyFile(sModelFileName.c_str(), sUserFileName.c_str(), TRUE) ;

      if (false == bSuccess)
			{
  			string sErrorText = pContexte->getSuperviseur()->getText("fileErrors", "errorCopyingFile") ;
    		sErrorText += string(" ") + sModelFileName + string(" -> ") + sUserFileName ;
        pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
				erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
  		}
   	}
	}
	inFile.close() ;

  return ;
}

void
NSUtilisateurChoisi::RenameUserFilesDirectory(string sPreviousID, string sNewID)
{
	if (string("") == sNewID)
  	sNewID = getID() ;

  string sPreviousDir = pContexte->PathName("FGLO") + sPreviousID ;
	string sPersoDir    = pContexte->PathName("FGLO") + sNewID ;

  ::MoveFile(sPreviousDir.c_str(), sPersoDir.c_str()) ;
}

/**
 * Get the virtual drug lexicon code from the lexicon code
 */
string
NSUtilisateurChoisi::getVirtualDrug(const string sSpeciality)
{
  if (string("") == sSpeciality)
    return string("") ;

  // If already a virtual drug, return the code itself
  //
  string sLang = getLang() ;

  string sLibelleTrouve = string("") ;
  if (pContexte->getDico()->isVirtualDrug(sLang, &sSpeciality, &sLibelleTrouve))
    return sSpeciality ;

  // If there is a BDM driver, ask it to do the job
  //
  NSBdmDriver* pDriver = pContexte->getBdmDriver() ;
  if (pDriver)
  {
    string sFoundCode = pDriver->getVirtualDrug(sSpeciality) ;
    if (string("") != sFoundCode)
      return sFoundCode ;
  }

  // Get all elements such as sSpeciality - is a -> element
  //
  string sCodeSens = pContexte->getDico()->donneCodeSens(&sSpeciality) ;

  VecteurString aVecteurString ;
  pContexte->getSuperviseur()->getFilGuide()->chercheEquivalent(sCodeSens, &aVecteurString, string("ES")) ;

  if (aVecteurString.empty())
    return string("") ;

  // Find if there is a virtual in the list
  //
  EquiItemIter itCodes = aVecteurString.begin() ;
  for ( ; itCodes != aVecteurString.end() ; itCodes++)
  {
    // Don't process the original concept
    //
    if (sCodeSens != **itCodes)
    {
      string sLibelleTrouve = string("") ;
      if (pContexte->getDico()->isVirtualDrug(sLang, *itCodes, &sLibelleTrouve))
      {
        string sFoundCode = **itCodes ;
        pContexte->getDico()->donneCodeComplet(sFoundCode) ;
        return sFoundCode ;
      }
    }
  }

  return string("") ;
}

void
NSUtilisateurChoisi::setupToolbar()
{
  TMyApp* pMyApp = pContexte->getSuperviseur()->getApplication() ;
  if ((TMyApp*) NULL == pMyApp)
    return ;

  TDockableControlBarEx* pCb1 = pMyApp->getMainControlBar() ;
  if ((TDockableControlBarEx*) NULL == pCb1)
    return ;

  //if (pMyApp->GetToolBarWindow() != 0)
  //{
    // on enlève les boutons de la control bar
  TGadget* pGadget = pCb1->FirstGadget() ;
  TGadget* pGadgetRemoved ;

  while (pGadget)  {
    pGadgetRemoved = pCb1->Remove(*pGadget) ;
    if (NULL == pGadgetRemoved)
    {
      erreur("Erreur à la destruction d'un bouton dans la barre d'outils", standardError, 0) ;
      return ;
    }
    else
      delete pGadgetRemoved ;
    pGadget = pCb1->FirstGadget() ;
  }

  pCb1->LayoutSession() ;  //}

  ifstream inFile ;

  bool bFileOpened = pContexte->getUtilisateur()->OpenUserFile(&inFile, string("icons.dat"), string("icons[user].dat"), pContexte->PathName("FGLO")) ;

  if (false == bFileOpened || !inFile)
  {
  	pCb1->Insert(*new TButtonGadget(CM_NEWDOC, 		CM_NEWDOC, 	 	  TButtonGadget::Command)) ;
    pCb1->Insert(*new TButtonGadget(CM_NEWPAT, 		CM_NEWPAT, 	 	  TButtonGadget::Command)) ;
    pCb1->Insert(*new TButtonGadget(CM_BUREAU, 		CM_BUREAU, 	 	  TButtonGadget::Command)) ;
    pCb1->Insert(*new TSeparatorGadget) ;
    pCb1->Insert(*new TButtonGadget(CM_IMPORTE, 		CM_IMPORTE, 	  TButtonGadget::Command)) ;
    pCb1->Insert(*new TButtonGadget(CM_CHEMISE, 		CM_CHEMISE,  	  TButtonGadget::Command)) ;
    pCb1->Insert(*new TButtonGadget(CM_IMPORTIMG, 	CM_IMPORTIMG, 	TButtonGadget::Command)) ;
    pCb1->Insert(*new TButtonGadget(CM_CAPTUREIMG, CM_CAPTUREIMG,  TButtonGadget::Command)) ;
    pCb1->Insert(*new TSeparatorGadget) ;
    pCb1->Insert(*new TButtonGadget(CM_ADMIN, 		  CM_ADMIN,  	    TButtonGadget::Command)) ;
    pCb1->Insert(*new TButtonGadget(CM_LDV, 		    CM_LDV,  	      TButtonGadget::Command)) ;
    pCb1->Insert(*new TButtonGadget(CM_LDV_DRUGS, 	CM_LDV_DRUGS,  	TButtonGadget::Command)) ;
    pCb1->Insert(*new TButtonGadget(CM_PROCESS, 	  CM_PROCESS,  	  TButtonGadget::Command)) ;
    pCb1->Insert(*new TButtonGadget(CM_LDV_GOALS, 	CM_LDV_GOALS,  	TButtonGadget::Command)) ;
    pCb1->Insert(*new TSeparatorGadget) ;
    pCb1->Insert(*new TButtonGadget(CM_NEWTTXT, 		CM_NEWTTXT,  	  TButtonGadget::Command)) ;

    // Insertion du bouton Word si Word.Basic existe dans la base de registre
    HKEY hKeyResult ;
    // clé du programme Word.Basic dans HKEY_LOCAL_MACHINE
    LPCTSTR lpszKey = _T("SOFTWARE\\CLASSES\\Word.Basic") ;
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpszKey, (DWORD)0, KEY_READ, &hKeyResult) == ERROR_SUCCESS)
    	pCb1->Insert(*new TButtonGadget(CM_NEWTMPL, 	CM_NEWTMPL,  	  TButtonGadget::Command)) ;

    pCb1->Insert(*new TButtonGadget(CM_NEWCR, 		  CM_NEWCR, 	 	  TButtonGadget::Command)) ;
    pCb1->Insert(*new TButtonGadget(CM_NEWCS, 		  CM_NEWCS, 	 	  TButtonGadget::Command)) ;
    pCb1->Insert(*new TButtonGadget(CM_NEWCONSULT, CM_NEWCONSULT, 	TButtonGadget::Command)) ;
    pCb1->Insert(*new TSeparatorGadget) ;
    pCb1->Insert(*new TButtonGadget(CM_ENREGISTRE, CM_ENREGISTRE, 	TButtonGadget::Command)) ;
    pCb1->Insert(*new TButtonGadget(CM_COMPOSE, 	 CM_COMPOSE,  	  TButtonGadget::Command)) ;
    pCb1->Insert(*new TButtonGadget(CM_IMPRIME, 	 CM_IMPRIME,  	  TButtonGadget::Command)) ;
    pCb1->Insert(*new TButtonGadget(CM_VISUAL, 		 CM_VISUAL,  	  TButtonGadget::Command)) ;
    pCb1->Insert(*new TSeparatorGadget) ;
    pCb1->Insert(*new TButtonGadget(CM_MAILBOX, 	 CM_MAILBOX, 	  TButtonGadget::Command)) ;
    pCb1->Insert(*new TButtonGadget(CM_OUTILS, 		 CM_OUTILS, 	 	  TButtonGadget::Command)) ;
    pCb1->Insert(*new TButtonGadget(CM_ABOUT, 		 CM_ABOUT, 	 	  TButtonGadget::Command)) ;

    pCb1->LayoutSession() ;

    return ;
  }

	string line ;
	string sData = string("") ;

  while (!inFile.eof())
	{
  	getline(inFile, line) ;
    if (string("") != line)
    	sData += line + "\n" ;
	}
	inFile.close() ;

	// boucle de chargement des attributs
	size_t i = 0 ;
	while (i < strlen(sData.c_str()))
	{
		string sNomAttrib = "" ;

		while ((i < strlen(sData.c_str())) && (' ' != sData[i]) && ('\t' != sData[i]) && ('\n' != sData[i]))
      sNomAttrib += pseumaj(sData[i++]) ;

		while ('\n' != sData[i])
			i++ ;

		i++ ;

    if (string("SEPARATOR") == sNomAttrib)
      pCb1->Insert(*new NSFlatSeparatorGadget) ;
    else
    {
      int iID = getIdFromString(sNomAttrib) ;
      if (-1 != iID)
      {
        // First, look if there is a skin defined for this gadget
        //
        string sCodeGadget = string("gadget_") + sNomAttrib ;
        nsSkin *pSkin = pContexte->getSkins()->donneSkin(sCodeGadget) ;

        if (pSkin && pSkin->getBackBmp())
          pCb1->Insert(*new NSFlatButtonGadget(pSkin->getBackBmp(), iID, NSFlatButtonGadget::Command)) ;
        else
          pCb1->Insert(*new NSFlatButtonGadget(iID, iID, NSFlatButtonGadget::Command)) ;
      }
    }
  }

  pCb1->LayoutSession() ;

/*
  NsStripe* stripe = new NsStripe(pContexte, pMyApp->frame, NsStripe::isInformation) ;
  stripe->Create() ;
  // for (int i = 0 ; i < 30 ; i++)
  //  stripe->Insert(*new TSeparatorGadget) ;
  stripe->Insert(*new NSFlatButtonGadget(CM_HEALTHTEAM_ROSE, CM_HEALTHTEAM_ROSE, NSFlatButtonGadget::Command)) ;
  for (int i = 0 ; i < 30 ; i++)
    stripe->Insert(*new TSeparatorGadget) ;
  stripe->Insert(*new NSFlatButtonGadget(CM_HEALTHTEAM_LIST, CM_HEALTHTEAM_LIST, NSFlatButtonGadget::Command)) ;
  pMyApp->Harbor->Insert(*stripe, OWL::alTop) ;
  stripe->LayoutSession() ;
*/

/*
  NsAlertStripe* stripe = new NsAlertStripe(pContexte, string("Test d'alerte"), string(""), 90, pMyApp->frame, NsStripe::isAlert) ;
  stripe->Create() ;
  pMyApp->Harbor->Insert(*stripe, OWL::alTop) ;
  stripe->LayoutSession() ;
*/
}

int
NSUtilisateurChoisi::getIdFromString(string sTextId)
{
  if (sTextId == "CM_NEWDOC")
    return CM_NEWDOC ;
  if (sTextId == "CM_NEWPAT")
    return CM_NEWPAT ;
  if (sTextId == "CM_BUREAU")
    return CM_BUREAU ;
  if (sTextId == "CM_IMPORTE")
    return CM_IMPORTE ;
  if (sTextId == "CM_CHEMISE")
    return CM_CHEMISE ;
  if (sTextId == "CM_IMPORTIMG")
    return CM_IMPORTIMG ;
  if (sTextId == "CM_CAPTUREIMG")
    return CM_CAPTUREIMG ;
  if (sTextId == "CM_ADMIN")
    return CM_ADMIN ;
  if (sTextId == "CM_LDV")
    return CM_LDV ;
  if (sTextId == "CM_LDV_DRUGS")
    return CM_LDV_DRUGS ;
  if (sTextId == "CM_LDV_GOALS")
    return CM_LDV_GOALS ;
  if (sTextId == "CM_PROCESS")
    return CM_PROCESS ;
  if (sTextId == "CM_NEWTTXT")
    return CM_NEWTTXT ;
  if (sTextId == "CM_NEWTMPL")
    return CM_NEWTMPL ;
  if (sTextId == "CM_NEWCR")
    return CM_NEWCR ;
  if (sTextId == "CM_NEWCS")
    return CM_NEWCS ;
  if (sTextId == "CM_NEWCONSULT")
    return CM_NEWCONSULT ;
  if (sTextId == "CM_ENREGISTRE")
    return CM_ENREGISTRE ;
  if (sTextId == "CM_PANNEL")
    return CM_PANNEL ;
  if (sTextId == "CM_COMPOSE")
    return CM_COMPOSE ;
  if (sTextId == "CM_IMPRIME")
    return CM_IMPRIME ;
  if (sTextId == "CM_VISUAL")
    return CM_VISUAL ;
  if (sTextId == "CM_MAILBOX")
    return CM_MAILBOX ;
  if (sTextId == "CM_OUTILS")
    return CM_OUTILS ;
  if (sTextId == "CM_ABOUT")
    return CM_ABOUT ;
  if (sTextId == "CM_PREDI")
    return CM_PREDI ;
  if (sTextId == "CM_PROJET")
    return CM_PROJET ;
  if (sTextId == "CM_QUESTIONNAIRE")
    return CM_QUESTIONNAIRE ;
  if (sTextId == "CM_HEALTHTEAM_ROSE")
    return CM_HEALTHTEAM_ROSE ;
  if (sTextId == "CM_HEALTHTEAM_LIST")
    return CM_HEALTHTEAM_LIST ;
  if (sTextId == "CM_SET_RECONVOC")
    return CM_SET_RECONVOC ;
  if (sTextId == "IDC_COOP_CHARTS")
    return IDC_COOP_CHARTS ;
  if (sTextId == "CM_BBK")
    return CM_BBK ;
  if (sTextId == "CM_USER_1")
    return CM_USER_1 ;
  if (sTextId == "CM_USER_2")
    return CM_USER_2 ;
  if (sTextId == "CM_USER_3")
    return CM_USER_3 ;
  if (sTextId == "CM_USER_4")
    return CM_USER_4 ;
  if (sTextId == "CM_USER_5")
    return CM_USER_5 ;
  if (sTextId == "CM_USER_6")
    return CM_USER_6 ;
  if (sTextId == "CM_USER_7")
    return CM_USER_7 ;
  if (sTextId == "CM_USER_8")
    return CM_USER_8 ;
  if (sTextId == "CM_USER_9")
    return CM_USER_9 ;

  return -1 ;
}

void
NSUtilisateurChoisi::setToDoAfterPatClose(NSToDoTask* pToDo)
{
  if ((NSToDoTask*) NULL == pToDo)
    return ;

  if (_pToDoAfterClosingPat)
    delete _pToDoAfterClosingPat ;

  _pToDoAfterClosingPat = new NSToDoTask(*pToDo) ;
}

//---------------------------------------------------------------------
//               NSMailParams object
//---------------------------------------------------------------------
NSMailParams::NSMailParams()
{
  _iTypeEnvoi   = NSMAIL_NAUTILUS ;

  _sUrlSMTP     = string("") ;
  _iPortSMTP    = 25 ;
  _sUserSMTP    = string("") ;
  _sPassSMTP    = string("") ;
  _sMailSender  = string("") ;
  _sMailFrom    = string("") ;
  _sMailReplyTo = string("") ;
  _sUrlPOP3     = string("") ;
  _iPortPOP3    = 110 ;
  _sUserPOP3    = string("") ;
  _sPassPOP3    = string("") ;

  _sApiKeysDir  = string("") ;
  _sApiUser     = string("") ;

  _bUseApiProxy = false ;
  _bUseAuthentification = false ;
}

//***********************************************************************//
//							      Classe NSProgressAndMessageDlg
//***********************************************************************//
DEFINE_RESPONSE_TABLE1(NSProgressAndMessageDlg, NSUtilDialog)
END_RESPONSE_TABLE;

NSProgressAndMessageDlg::NSProgressAndMessageDlg(TWindow* parent, NSContexte* pCtx, TModule* module)
                        :NSUtilDialog(parent, pCtx, "PROGRESS_DISPLAY", module)
{
	_pProgressBar = new TGauge(this, IDC_PROGRESS_BAR) ;
	_pTextDisplay = new TStatic(this, IDC_PATCLOSE_TEXT) ;
}

NSProgressAndMessageDlg::~NSProgressAndMessageDlg(){
	delete _pProgressBar ;
	delete _pTextDisplay ;
}

voidNSProgressAndMessageDlg::SetupWindow()
{
	TDialog::SetupWindow() ;

	_pProgressBar->SetRange(0, 100) ;  _pProgressBar->SetLed(4, 80) ;
  _pProgressBar->SetStep(1) ;

  resetProgressGauge() ;
}

voidNSProgressAndMessageDlg::resetProgressGauge()
{
	_pProgressBar->SetValue(0) ;
}

//***********************************************************************////							      Classe NSHistoryClosingDlg
//***********************************************************************//
/*
DEFINE_RESPONSE_TABLE1(NSHistoryClosingDlg, TWindow)
  EV_WM_TIMER,
END_RESPONSE_TABLE;
NSHistoryClosingDlg::NSHistoryClosingDlg(TWindow* parent, NSContexte* pCtx)                    :TWindow(parent), NSRoot(pCtx){}NSHistoryClosingDlg::~NSHistoryClosingDlg()
{
}

void
NSHistoryClosingDlg::SetupWindow()
{
  TWindow::SetupWindow() ;

  Show(SW_HIDE) ;

  SetTimer(ID_HISTORY_TIMER, 50) ;
}

voidNSHistoryClosingDlg::EvTimer(uint timerId){  if (ID_HISTORY_TIMER != timerId)		return ;  if ((NULL == pContexte->getPatient()) ||      (false == pContexte->getPatient()->_bCanCloseHisto))  {    KillTimer(ID_HISTORY_TIMER) ;    CloseWindow() ;  }  NSHISTODocument* pDocHis = pContexte->getPatient()->getDocHis() ;
  if ((NSHISTODocument*) NULL == pDocHis)
  {
    KillTimer(ID_HISTORY_TIMER) ;
    CloseWindow() ;
  }

  string sMsg = string("Waiting for history to close.") ;
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSteps) ;
}
*/