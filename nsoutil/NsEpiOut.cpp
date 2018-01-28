#include <owl\edit.h>#include <owl\checkbox.h>

#include "partage\nsdivfct.h"
#include "nautilus\nssuper.h"
#include "nautilus\nsepicap.h"
#include "nsoutil\nsoutil.h"
#include "nsepisod\flechiesDB.h"
#include "nsepisod\nsclasser.h"
#include "nsepisod\nsMailManager.h"
#include "nsepisod\nsToDo.h"
#include "nsbb\nsednum.h"

#include "nsoutil\nsEpiOut.rh"
#include "nsoutil\nsEpiOut.h"

// ***************************************************************************
//
//                   Méthodes de NsEpisodusOutilDialog
//
// ***************************************************************************
DEFINE_RESPONSE_TABLE1(NsEpisodusOutilDialog, TDialog)
	EV_CHILD_NOTIFY(IDC_FLECHIES,   BN_CLICKED, lanceBuildFlechies),
    EV_CHILD_NOTIFY(IDC_IMP_CISP,   BN_CLICKED, lanceImportCISP),
    EV_CHILD_NOTIFY(IDC_IMP_CIM,    BN_CLICKED, lanceImportCIM),
    EV_CHILD_NOTIFY(IDC_IMP_LISTE,  BN_CLICKED, lanceCodeList),
    EV_CHILD_NOTIFY(IDC_CLASS_CTRL, BN_CLICKED, gereClassCtrl),
END_RESPONSE_TABLE;

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NsEpisodusOutilDialog::NsEpisodusOutilDialog(TWindow* pere, NSContexte* pCtx)
				      :NSUtilDialog(pere, pCtx, "OUTILS_EPISODUS", pNSResModule)
{
    pBuildFlechies  = new TCheckBox(this, IDC_FLECHIES) ;
    pImportCISP     = new TCheckBox(this, IDC_IMP_CISP) ;
    pImportCIM      = new TCheckBox(this, IDC_IMP_CIM) ;
    pCodeList       = new TCheckBox(this, IDC_IMP_LISTE) ;
    pClassCtrl      = new TCheckBox(this, IDC_CLASS_CTRL) ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NsEpisodusOutilDialog::~NsEpisodusOutilDialog()
{
	//
	// Suppression de tous les objets
	//
	delete pBuildFlechies ;
    delete pImportCISP ;
    delete pImportCIM ;
    delete pCodeList ;
    delete pClassCtrl ;
}

//---------------------------------------------------------------------------
//  Initialisation de la boite de dialogue
//---------------------------------------------------------------------------
void
NsEpisodusOutilDialog::SetupWindow()
{
    TDialog::SetupWindow();
}

//---------------------------------------------------------------------------
//  Lancement de la reconstruction de la base FLECHIES.DB
//---------------------------------------------------------------------------
void
NsEpisodusOutilDialog::lanceBuildFlechies()
{
	string sTypeDocument;
    NSFlechiesBuildDlg* pDlg = new NSFlechiesBuildDlg(pContexte->GetMainWindow(),
                                                      pContexte);
    pDlg->Execute();
    delete pDlg;
    return;
}

//---------------------------------------------------------------------------
//  Lancement de l'importation de la CISP
//---------------------------------------------------------------------------
void
NsEpisodusOutilDialog::lanceImportCISP()
{
    int iResult = MessageBox("Vous devez disposer d'un fichier CISP.TXT dans le répertoire de lancement du logiciel. Ce fichier doit contenir le libellé et le code, séparés par une tabulation. Souhaitez vous continuer ?", "Import de la CISP", MB_YESNO);
    if (iResult != IDYES)
        return;

    NSEpiClassifDB* pClassifDB = new NSEpiClassifDB(pContexte);
    pClassifDB->rempliClassifCISP("CISP.TXT") ;
    delete pClassifDB;
    return;
}

//---------------------------------------------------------------------------
//  Lancement de l'importation de la CIM
//---------------------------------------------------------------------------
void
NsEpisodusOutilDialog::lanceImportCIM()
{
    int iResult = MessageBox("Vous devez disposer d'un fichier CIM.TXT dans le répertoire de lancement du logiciel. Ce fichier doit contenir le libellé et le code, séparés par une tabulation. Souhaitez vous continuer ?", "Import CIM", MB_YESNO);
    if (iResult != IDYES)
        return;

    NSEpiClassifDB* pClassifDB = new NSEpiClassifDB(pContexte);
    pClassifDB->rempliClassifCIM("CIM.TXT") ;
    delete pClassifDB;
    return;
}

//---------------------------------------------------------------------------
//  Lancement de la constitution de la liste codes-libellés
//---------------------------------------------------------------------------
void
NsEpisodusOutilDialog::lanceCodeList()
{
    NSCodeListDlg* pDlg = new NSCodeListDlg(pContexte->GetMainWindow(),
                                                      pContexte);
    pDlg->Execute();
    delete pDlg;
    return;
}

//---------------------------------------------------------------------------
//  Lancement du traitement du fichier classXXX.xml
//---------------------------------------------------------------------------
void
NsEpisodusOutilDialog::gereClassCtrl()
{
    string sFichier =   string("class") +
#ifndef _MUE
                        string(pContexte->getUtilisateur()->pDonnees->indice) +
#else
                        string(pContexte->getUtilisateurID()) +
#endif
                        string(".xml") ;
    sFichier = pContexte->PathName("BGLO") + sFichier;

    string sLine;
    ifstream inFile ;
    inFile.open(sFichier.c_str());

    if (!inFile)
   	{
        string sErrMess = string("Erreur d'ouverture du fichier ") + sFichier;
        erreur(sErrMess.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
   		return ;
   	}

    string sSaveFile = string("class") + donne_date_duJour() + donne_heure() +
#ifndef _MUE
                string(pContexte->getUtilisateur()->pDonnees->indice) +
#else
                string(pContexte->getUtilisateurID()) +
#endif
                string(".xml");
    sSaveFile = pContexte->PathName("BGLO") + sFichier;
    ofstream outFile ;
    outFile.open(sSaveFile.c_str(), ios::ate);

    if (!outFile)
   	{
        string sErrMess = string("Erreur d'ouverture en écriture du fichier ")
                                                                    + sSaveFile;
        erreur(sErrMess.c_str(), standardError, 0) ;
   		return;
   	}

    //
    //
    outFile << "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" ?>\n" ;
    outFile << "<classif_analyse>\n" ;

    while (!inFile.eof())
    {
        getline(inFile, sLine) ;
        outFile << (sLine + "\n") ;
    }
    inFile.close();

    outFile << "</classif_analyse>\n" ;
    outFile.close() ;

    DeleteFile(sFichier.c_str()) ;

    return;
}

// ***************************************************************************
//
//                   Méthodes de NSFlechiesBuildDlg
//                reconstruction de la base Flechies.db
//
// ***************************************************************************

DEFINE_RESPONSE_TABLE1(NSFlechiesBuildDlg, TDialog)
   EV_CHILD_NOTIFY(IDC_CVT_START, BN_CLICKED, demarre),
   EV_CHILD_NOTIFY(IDC_CVT_STOP,  BN_CLICKED, stoppe),
END_RESPONSE_TABLE;

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NSFlechiesBuildDlg::NSFlechiesBuildDlg(TWindow* pere, NSContexte* pCtx)
                   :NSUtilDialog(pere, pCtx, "BUILD_FLECHIES", pNSResModule)
{
  pCurrentLex = new OWL::TEdit(this, IDC_CURRENT) ;
  pEmpty      = new OWL::TCheckBox(this, IDC_EMPTY) ;
  pAddLexique = new OWL::TCheckBox(this, IDC_ADDLEX) ;
  pAddOrtho   = new OWL::TCheckBox(this, IDC_ADDORTHO) ;
  pAddLocal   = new OWL::TCheckBox(this, IDC_ADDLOCAL) ;
  pAddExpress = new OWL::TCheckBox(this, IDC_ADDEXPRES) ;

  bTourner    = true ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSFlechiesBuildDlg::~NSFlechiesBuildDlg()
{
  delete pCurrentLex ;
  delete pEmpty ;
  delete pAddLexique ;
  delete pAddOrtho ;
  delete pAddLocal ;
  delete pAddExpress ;
}

//---------------------------------------------------------------------------
//  Initialisation de la boite de dialogue
//---------------------------------------------------------------------------
void
NSFlechiesBuildDlg::SetupWindow()
{
	NSUtilDialog::SetupWindow() ;

	pCurrentLex->SetReadOnly(true) ;

  pEmpty->Uncheck() ;
  pAddLexique->Uncheck() ;
  pAddOrtho->Uncheck() ;
  pAddLocal->Uncheck() ;
  pAddExpress->Uncheck() ;

  pEmpty->SetCheck(BF_GRAYED) ;
  pAddLexique->SetCheck(BF_GRAYED) ;
  pAddOrtho->SetCheck(BF_GRAYED) ;
  pAddLocal->SetCheck(BF_GRAYED) ;
  pAddExpress->SetCheck(BF_GRAYED) ;
}

//---------------------------------------------------------------------------
//  Lancement du mécanisme
//---------------------------------------------------------------------------
void
NSFlechiesBuildDlg::demarre()
{
    if (bTourner)
        videTable();
    else
        return;

    if (bTourner)
        importeLexique();
    else
        return;

    if (bTourner)
    {
        if (importeLocal())
            pAddLocal->SetCheck(BF_CHECKED) ;
    }
    else
        return;

    if (bTourner)
    {
        if (importeExpressions())
            pAddExpress->SetCheck(BF_CHECKED) ;
    }
    else
        return;

    if (bTourner)
    {
        if (importeOrtho())
            pAddOrtho->SetCheck(BF_CHECKED) ;
    }
    else
        return;
}

//---------------------------------------------------------------------------
//  Lancement de la conversion en boucle
//---------------------------------------------------------------------------
void
NSFlechiesBuildDlg::stoppe()
{
    bTourner = false;
}

void
NSFlechiesBuildDlg::videTable()
{
	NSFlechies *pFlechies = new NSFlechies(pContexte->getSuperviseur()) ;

	pFlechies->lastError = pFlechies->open(false) ; // Ouverture accès exclusif

	if (pFlechies->lastError != DBIERR_NONE)
	{
		erreur("flechies.db -- Erreur à l'ouverture du fichier flechies.db", standardError, pFlechies->lastError, pContexte->GetMainWindow()->GetHandle()) ;
		delete pFlechies;
		bTourner = false;
		return;
	}

	pFlechies->lastError = pFlechies->emptyTable() ;
	if (pFlechies->lastError != DBIERR_NONE)
	{
		erreur("flechies.db -- Erreur à la réinitialisation de la table", standardError, pFlechies->lastError, pContexte->GetMainWindow()->GetHandle()) ;
		bTourner = false;
	}
	else
		pEmpty->SetCheck(BF_CHECKED) ;

	pFlechies->lastError = pFlechies->close();
	if (pFlechies->lastError != DBIERR_NONE)
		erreur("flechies.db -- Erreur de fermeture de la base flechies.db.", standardError, pFlechies->lastError, pContexte->GetMainWindow()->GetHandle()) ;

	delete pFlechies ;

	pContexte->getSuperviseur()->getApplication()->PumpWaitingMessages() ;
}

void
NSFlechiesBuildDlg::importeLexique()
{
    NSEpiFlechiesDB* pFlechieDB = new NSEpiFlechiesDB(pContexte);
    pFlechieDB->CreateDatabaseFlechies(&bTourner, pCurrentLex);
    delete pFlechieDB;

    if (bTourner)
        pAddLexique->SetCheck(BF_CHECKED) ;
}

bool
NSFlechiesBuildDlg::importeSpecifique(int iBase)
{
    NSFlechies *pFlex = new NSFlechies(pContexte->getSuperviseur(), iBase) ;

    pFlex->lastError = pFlex->open() ; // Ouverture accès exclusif
    if (pFlex->lastError != DBIERR_NONE)
    {
        string sErreur = string("Erreur à l'ouverture du fichier ") + pFlex->sFileName;
        erreur(sErreur.c_str(), standardError, pFlex->lastError, pContexte->GetMainWindow()->GetHandle()) ;
        delete pFlex;
        return false;
    }

    pFlex->lastError = pFlex->debut(dbiWRITELOCK) ;
    //
    // Table vide
    //
    if (pFlex->lastError == DBIERR_EOF)
    {
        string sErreur = string("La table des ") + pFlex->sFileLib +
                            string(" (") + pFlex->sFileName + string(") est vide");
        erreur(sErreur.c_str(), standardError, pFlex->lastError) ;
        pFlex->lastError = pFlex->close();
        delete pFlex;
        return false;
    }
    if (pFlex->lastError != DBIERR_NONE)
    {
        string sErreur = pFlex->sFileName + string(" : Impossible de se positionner sur la première fiche");
        erreur(sErreur.c_str(), standardError, pFlex->lastError) ;
        pFlex->lastError = pFlex->close();
        delete pFlex;
        return false;
    }
    pFlex->lastError = pFlex->getRecord();
    if (pFlex->lastError != DBIERR_NONE)
    {
        string sErreur = pFlex->sFileName + string(" : Erreur à la lecture de la première fiche");
        erreur(sErreur.c_str(), standardError, pFlex->lastError) ;
        pFlex->lastError = pFlex->close();
        delete pFlex;
        return false;
    }

    NSFlechies *pFlechies = new NSFlechies(pContexte->getSuperviseur()) ;

    pFlechies->lastError = pFlechies->open() ;
    if (pFlechies->lastError != DBIERR_NONE)
    {
        erreur("flechies.db -- Erreur à l'ouverture du fichier flechies.db", standardError, pFlechies->lastError, pContexte->GetMainWindow()->GetHandle()) ;
        pFlex->lastError = pFlex->close();
        delete pFlex;
        delete pFlechies;
        return false;
    }

    while ((pFlex->lastError == DBIERR_NONE) && bTourner)
    {
        *(pFlechies->pDonnees) = (*pFlex->pDonnees);

        pFlechies->lastError = pFlechies->appendRecord();
        if (pFlechies->lastError != DBIERR_NONE)
        {
            erreur("flechies.db -- Erreur à l'ajout d'une fiche", standardError, pFlechies->lastError, pContexte->GetMainWindow()->GetHandle()) ;
            pFlechies->lastError = pFlechies->close();
            delete pFlechies;
            pFlex->lastError = pFlex->close();
            delete pFlex;
            bTourner = false;
            return false;
        }

        pFlex->lastError = pFlex->suivant(dbiWRITELOCK);
        if (pFlex->lastError != DBIERR_NONE)
        {
            if (pFlex->lastError != DBIERR_EOF)
            {
                string sErreur = pFlex->sFileName + string(" : Erreur à l'accès à la fiche suivante");
                erreur(sErreur.c_str(), standardError, pFlex->lastError) ;
            }
        }
        else
        {
            pFlex->lastError = pFlex->getRecord();
            if (pFlex->lastError != DBIERR_NONE)
            {
                string sErreur = pFlex->sFileName + string(" : Erreur à la lecture de la fiche suivante");
                erreur(sErreur.c_str(), standardError, pFlex->lastError) ;
            }
        }

        pContexte->getSuperviseur()->getApplication()->PumpWaitingMessages();
    }
    pFlechies->lastError = pFlechies->close();
    delete pFlechies;
    pFlex->lastError = pFlex->close();
    delete pFlex;
    return true;
}

//***********************************************************************************************                             /*CLASSE PROMETHE  */
//************************************************************************************************
DEFINE_RESPONSE_TABLE1(NSPrometheParamsDlg, TDialog)
  EV_COMMAND(IDOK, CmOk),
  EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE ;

NSPrometheParamsDlg::NSPrometheParamsDlg(TWindow* pere, NSContexte* pCtx, string* psLog,
                                         string* psPrinc, bool* pbSend,
                                         bool* pbHa, string*  psUserPas,
                                         string* psUser, string* psMail,
                                         bool* pbAuto, string* psURLServ,
                                         bool* pbCapt, string* psCaptKey,
                                         bool* pbBabyl, string* psBabylKey,
                                         bool* pbAutoSave, string* psOpenModeLdv,
                                         bool* pbAutoOpen, bool* pbTLcrn,
                                         string* psServUrl, string* psServTitle)
                    :NSUtilDialog(pere, pCtx, "IDD_PROMETHEPARAMSDLG", pNSResModule)
{
  pUserId             = new TEdit(this, IDC_USERID );
  pMail               = new TEdit(this, IDC_MAIL );
  pAutoFloculeNon     = new TRadioButton(this, IDC_AUTOFLOCULE_NON );
  pAutoFloculeOui     = new TRadioButton(this, IDC_AUTOFLOCULE_OUI );
  pSend_Now_Non       = new TRadioButton(this, IDC_SEND_NOW_NON );
  pSend_Now_Oui       = new TRadioButton(this, IDC_SEND_NOW_OUI );
  pHash_Oui           = new TRadioButton(this, IDC_HASH_OUI );
  pHash_Non           = new TRadioButton(this, IDC_HASH_NON );
  pLogin              = new TEdit(this, IDC_LOGIN );
  pUserPassword       = new TEdit(this, IDC_USERPASSWORD );
  pServerURL          = new TEdit(this, IDC_URLSERVEUR );
  pPrincipeCim        = new TRadioButton(this, IDC_PRINCIPE_CIM );
  pPrincipeCisp       = new TRadioButton(this, IDC_PRINCIPE_CISP );
  pCapture_Oui        = new TRadioButton(this, IDC_CAPTURE_OUI );
  pCapture_Non        = new TRadioButton(this, IDC_CAPTURE_NON );
  pCapture_Key        = new TEdit(this, IDC_CAPTURE_KEY );
  pBabylon_Oui        = new TRadioButton(this, IDC_BABYLON_OUI );
  pBabylon_Non        = new TRadioButton(this, IDC_BABYLON_NON );
  pBabylon_Key        = new TEdit(this, IDC_BABYLON_KEY );
  pAutoSave_Oui       = new TRadioButton(this, IDC_SAVEPOS_OUI );
  pAutoSave_Non       = new TRadioButton(this, IDC_SAVEPOS_NON );
  pOpenMode_Heure     = new TRadioButton(this, IDC_OPENMODE_H );
  pOpenMode_Jour      = new TRadioButton(this, IDC_OPENMODE_J );
  pOpenMode_Semaine   = new TRadioButton(this, IDC_OPENMODE_S );
  pOpenMode_Mois      = new TRadioButton(this, IDC_OPENMODE_M );
  pAutoOpen_Oui       = new TRadioButton(this, IDC_AUTOOPEN_OUI );
  pAutoOpen_Non       = new TRadioButton(this, IDC_AUTOOPEN_NON );
  pTLconcen_Oui       = new TRadioButton(this, IDC_TLCONCERN_OUI );
  pTLconcen_Non       = new TRadioButton(this, IDC_TLCONCERN_NON );
  pServiceUrl         = new TEdit(this, IDC_URLSERVICE );
	pServiceTitle       = new TEdit(this, IDC_TITLESERVICE );

  ptMail              = new OWL::TStatic(this,   IDS_MAIL) ;
  ptUserId            = new OWL::TStatic(this,   IDS_USERID) ;
  ptServerUrl         = new OWL::TStatic(this,   IDS_URLSERVEUR) ;
  ptLogin             = new OWL::TStatic(this,   IDS_LOGIN) ;
  ptPass              = new OWL::TStatic(this,   IDS_USERPASSWORD) ;
  ptServiceUrl        = new OWL::TStatic(this,   IDS_URLSERVICE) ;
  ptServiceTitle      = new OWL::TStatic(this,   IDS_TITLESERVICE) ;
  ptClassif           = new OWL::TGroupBox(this, IDS_CLASSIF) ;
  ptAutoFlocule       = new OWL::TGroupBox(this, IDS_AUTOFLOCULE) ;
  ptSendNow           = new OWL::TGroupBox(this, IDS_SEND_NOW) ;
  ptHash              = new OWL::TGroupBox(this, IDS_HASH) ;
  ptSavePos           = new OWL::TGroupBox(this, IDS_SAVEPOS) ;
  ptGeneral           = new OWL::TGroupBox(this, IDS_GENERAL) ;
  ptCapture           = new OWL::TGroupBox(this, IDS_CAPTURE) ;
  ptCaptureKey        = new OWL::TStatic(this,   IDS_CAPTURE_KEY) ;
  ptBabylon           = new OWL::TGroupBox(this, IDS_BABYLON) ;
  ptBabylonKey        = new OWL::TStatic(this,   IDS_BABYLON_KEY) ;
  ptPrincipe          = new OWL::TGroupBox(this, IDS_PRINCIPE) ;
  ptLdV               = new OWL::TGroupBox(this, IDS_LDV) ;
  ptOpenMode          = new OWL::TGroupBox(this, IDS_OPENMODE) ;
  ptAutoOpen          = new OWL::TGroupBox(this, IDS_AUTOOPEN) ;
  ptTlConcern         = new OWL::TGroupBox(this, IDS_TLCONCERN) ;

  pbCapture       = pbCapt ;
  pbBabylon       = pbBabyl ;
  psCaptureKey    = psCaptKey ;
  psBabylonKey    = psBabylKey ;
  pbAutoSauve     = pbAutoSave ;
  psOpenMode      = psOpenModeLdv ;
  pbAutoOpenLdv   = pbAutoOpen ;
  pbTLconcernLdv  = pbTLcrn ;

  psLogin         = psLog ;
  psPrincipe      = psPrinc ;
  pbSendNow       = pbSend ;
  pbHash          = pbHa ;
  psUserPassword  = psUserPas ;
  psUserId        = psUser ;
  psMailSeveur    = psMail ;
  pbAutoFlocule   = pbAuto ;
  psURLServeur    = psURLServ ;
  psServiceUrl    = psServUrl ;
  psServiceTitle  = psServTitle ;
}

NSPrometheParamsDlg::~NSPrometheParamsDlg()
{
  delete  pCapture_Oui ;
  delete  pCapture_Non ;
  delete  pCapture_Key ;
  delete  pBabylon_Oui ;
  delete  pBabylon_Non ;
  delete  pBabylon_Key ;
  delete  pUserId ;
  delete  pMail ;
  delete  pAutoFloculeNon ;
  delete  pAutoFloculeOui ;
  delete  pPrincipeCim ;
  delete  pPrincipeCisp ;
  delete  pSend_Now_Non ;
  delete  pSend_Now_Oui ;
  delete  pHash_Oui ;
  delete  pHash_Non ;
  delete  pAutoSave_Oui ;
  delete  pAutoSave_Non ;
  delete  pLogin ;
  delete  pUserPassword ;
  delete  pServerURL ;
  delete  pOpenMode_Heure ;
  delete  pOpenMode_Jour ;
  delete  pOpenMode_Semaine ;
  delete  pOpenMode_Mois ;
  delete  pAutoOpen_Oui ;
  delete  pAutoOpen_Non ;
  delete  pTLconcen_Oui ;
  delete  pTLconcen_Non ;
  delete  pServiceUrl ;
  delete  pServiceTitle ;

	delete ptMail ;
  delete ptUserId ;
  delete ptServerUrl ;
	delete ptLogin ;
	delete ptPass ;
	delete ptServiceUrl ;
	delete ptServiceTitle ;
	delete ptClassif ;
	delete ptAutoFlocule ;
	delete ptSendNow ;
	delete ptHash ;
	delete ptSavePos ;
	delete ptGeneral ;
	delete ptCapture ;
	delete ptCaptureKey ;
	delete ptBabylon ;
	delete ptBabylonKey ;
	delete ptPrincipe ;
	delete ptLdV ;
	delete ptOpenMode ;
	delete ptAutoOpen ;
	delete ptTlConcern ;
}

void
NSPrometheParamsDlg::CmCancel()
{
	Destroy(IDCANCEL) ;
}

void
NSPrometheParamsDlg::SetupWindow()
{
	// fichiers d'aide
	sHindex = "" ;
	sHcorps = "EPISODUS.html" ;
	NSUtilDialog::SetupWindow() ;

	/* init de la fenetre */

  string sYes = pContexte->getSuperviseur()->getText("generalLanguage", "yes") ;
  string sNo  = pContexte->getSuperviseur()->getText("generalLanguage", "no") ;
  string sActive   = pContexte->getSuperviseur()->getText("generalLanguage", "active") ;
  string sInactive = pContexte->getSuperviseur()->getText("generalLanguage", "inactive") ;

  string sText = pContexte->getSuperviseur()->getText("episodusParameters", "StudyPersonalIdentifier") ;
  ptUserId->SetText(sText.c_str()) ;
	pUserId->SetText(psUserId->c_str()) ;

  sText = pContexte->getSuperviseur()->getText("episodusParameters", "classification") ;
  ptClassif->SetText(sText.c_str()) ;
	pMail->SetText(psMailSeveur->c_str()) ;

  sText = pContexte->getSuperviseur()->getText("episodusParameters", "classificationPrinciple") ;
  ptPrincipe->SetCaption(sText.c_str()) ;

  sText = pContexte->getSuperviseur()->getText("episodusParameters", "automaticSoapDispatching") ;
  ptAutoFlocule->SetText(sText.c_str()) ;
  pAutoFloculeOui->SetCaption(sYes.c_str()) ;
	if (*pbAutoFlocule == true)
		pAutoFloculeOui->SetCheck(BF_CHECKED) ;
  pAutoFloculeNon->SetCaption(sNo.c_str()) ;
	if (*pbAutoFlocule == false)
		pAutoFloculeNon->SetCheck(BF_CHECKED) ;

  sText = pContexte->getSuperviseur()->getText("episodusParameters", "classificationPrinciple") ;
  ptPrincipe->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("generalLanguage", "ICD") ;
  pPrincipeCim->SetCaption(sText.c_str()) ;
	if (*psPrincipe == "CIM")
		pPrincipeCim->SetCheck(BF_CHECKED) ;
  sText = pContexte->getSuperviseur()->getText("generalLanguage", "ICPC") ;
  pPrincipeCisp->SetCaption(sText.c_str()) ;
	if (*psPrincipe == "CISP")
		pPrincipeCisp->SetCheck(BF_CHECKED) ;

  sText = pContexte->getSuperviseur()->getText("episodusParameters", "general") ;
  ptGeneral->SetCaption(sText.c_str()) ;

	sText = pContexte->getSuperviseur()->getText("episodusParameters", "completeScreenCapture") ;
  ptCapture->SetCaption(sText.c_str()) ;
  pCapture_Non->SetCaption(sNo.c_str()) ;
  if (*pbCapture == false)
  	pCapture_Non->SetCheck(BF_CHECKED) ;
  pCapture_Oui->SetCaption(sYes.c_str()) ;
  if (*pbCapture == true)
  	pCapture_Oui->SetCheck(BF_CHECKED) ;

  sText = pContexte->getSuperviseur()->getText("episodusParameters", "ctrl") ;
  ptCaptureKey->SetCaption(sText.c_str()) ;
  pCapture_Key->SetText(psCaptureKey->c_str()) ;

  sText = pContexte->getSuperviseur()->getText("episodusParameters", "babylonScreenCapture") ;
  ptBabylon->SetCaption(sText.c_str()) ;
  pBabylon_Non->SetCaption(sNo.c_str()) ;
  if (*pbBabylon == false)
  	pBabylon_Non->SetCheck(BF_CHECKED) ;
  pBabylon_Oui->SetCaption(sYes.c_str()) ;
  if (*pbBabylon == true)
  	pBabylon_Oui->SetCheck(BF_CHECKED) ;

  sText = pContexte->getSuperviseur()->getText("episodusParameters", "shift") ;
  ptBabylonKey->SetCaption(sText.c_str()) ;
  pBabylon_Key->SetText(psBabylonKey->c_str()) ;

  sText = pContexte->getSuperviseur()->getText("episodusParameters", "sendImmediately") ;
  ptSendNow->SetCaption(sText.c_str()) ;
  pSend_Now_Non->SetCaption(sNo.c_str()) ;
  if (*pbSendNow == false)
  	pSend_Now_Non->SetCheck(BF_CHECKED) ;
  pSend_Now_Oui->SetCaption(sYes.c_str()) ;
  if (*pbSendNow == true)
  	pSend_Now_Oui->SetCheck(BF_CHECKED) ;

  sText = pContexte->getSuperviseur()->getText("episodusParameters", "hash") ;
  ptHash->SetCaption(sText.c_str()) ;
  pHash_Oui->SetCaption(sYes.c_str()) ;
  if (*pbHash == true)
  	pHash_Oui->SetCheck(BF_CHECKED) ;
  pHash_Non->SetCaption(sNo.c_str()) ;
  if (*pbHash == false)
  	pHash_Non->SetCheck(BF_CHECKED) ;

  sText = pContexte->getSuperviseur()->getText("episodusParameters", "positionRecord") ;
  ptSavePos->SetCaption(sText.c_str()) ;
  pAutoSave_Oui->SetCaption(sYes.c_str()) ;
  if (*pbAutoSauve == true)
  	pAutoSave_Oui->SetCheck(BF_CHECKED) ;
  pAutoSave_Non->SetCaption(sNo.c_str()) ;
  if (*pbAutoSauve == false)
  	pAutoSave_Non->SetCheck(BF_CHECKED) ;

  sText = pContexte->getSuperviseur()->getText("episodusParameters", "ligneDeVie") ;
  ptLdV->SetCaption(sText.c_str()) ;

  sText = pContexte->getSuperviseur()->getText("episodusParameters", "automaticOpening") ;
  ptAutoOpen->SetCaption(sText.c_str()) ;
  pAutoOpen_Oui->SetCaption(sYes.c_str()) ;
  if (*pbAutoOpenLdv == true)
  	pAutoOpen_Oui->SetCheck(BF_CHECKED) ;
  pAutoOpen_Non->SetCaption(sNo.c_str()) ;
  if (*pbAutoOpenLdv == false)
  	pAutoOpen_Non->SetCheck(BF_CHECKED) ;

	sText = pContexte->getSuperviseur()->getText("episodusParameters", "defaultScale") ;
  ptOpenMode->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("episodusParameters", "pixelHour") ;
  pOpenMode_Heure->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("episodusParameters", "pixelDay") ;
  pOpenMode_Jour->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("episodusParameters", "pixelWeek") ;
  pOpenMode_Semaine->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("episodusParameters", "pixelMonth") ;
  pOpenMode_Mois->SetCaption(sText.c_str()) ;
  if      (*psOpenMode == "H")
  	pOpenMode_Heure->SetCheck(BF_CHECKED) ;
  else if (*psOpenMode == "J")
  	pOpenMode_Jour->SetCheck(BF_CHECKED) ;
  else if (*psOpenMode == "S")
  	pOpenMode_Semaine->SetCheck(BF_CHECKED) ;
  else if (*psOpenMode == "M")
  	pOpenMode_Mois->SetCheck(BF_CHECKED) ;

  sText = pContexte->getSuperviseur()->getText("episodusParameters", "allowFreeTextForConcerns") ;
  ptTlConcern->SetCaption(sText.c_str()) ;
  if (*pbTLconcernLdv == true)
      pTLconcen_Oui->SetCheck(BF_CHECKED) ;
  if (*pbTLconcernLdv == false)
      pTLconcen_Non->SetCheck(BF_CHECKED) ;

  sText = pContexte->getSuperviseur()->getText("episodusParameters", "serverLogin") ;
  ptLogin->SetText(sText.c_str()) ;
  pLogin->SetText(psLogin->c_str()) ;

  sText = pContexte->getSuperviseur()->getText("episodusParameters", "serverPassword") ;
  ptPass->SetText(sText.c_str()) ;
  pUserPassword->SetText(psUserPassword->c_str()) ;
  sText = pContexte->getSuperviseur()->getText("episodusParameters", "serverWebAddress") ;
  ptServerUrl->SetText(sText.c_str()) ;
  pServerURL->SetText(psURLServeur->c_str()) ;

  pServiceUrl->SetText(psServiceUrl->c_str()) ;
  sText = pContexte->getSuperviseur()->getText("episodusParameters", "serviceWebAddress") ;
  ptServiceUrl->SetText(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("episodusParameters", "serviceTitle") ;
  ptServiceTitle->SetText(sText.c_str()) ;
  pServiceTitle->SetText(psServiceTitle->c_str()) ;
}

void
NSPrometheParamsDlg::CmOk()
{
  // traitement de l'AutoFlocule

  if (pAutoFloculeOui->GetCheck() == BF_CHECKED)
      *pbAutoFlocule = true;
  if (pAutoFloculeNon->GetCheck() == BF_CHECKED)
      *pbAutoFlocule = false;

  // traitement de la classification

  if (pPrincipeCim->GetCheck() == BF_CHECKED)
      *psPrincipe = "CIM";
  if (pPrincipeCisp->GetCheck() == BF_CHECKED)
      *psPrincipe = "CISP";

  // traitement de la capture

  if (pCapture_Oui->GetCheck() == BF_CHECKED)
      *pbCapture = true;
  if (pCapture_Non->GetCheck() == BF_CHECKED)
      *pbCapture = false;

  char szCaptureKey[128];
  pCapture_Key->GetText(szCaptureKey, 127);
  *psCaptureKey = string(szCaptureKey) ;

  // traitement du mode Babylon

  if (pBabylon_Oui->GetCheck() == BF_CHECKED)
      *pbBabylon = true;
  if (pBabylon_Non->GetCheck() == BF_CHECKED)
      *pbBabylon = false;

  char szBabylonKey[128];
  pBabylon_Key->GetText(szBabylonKey, 127);
  *psBabylonKey = string(szBabylonKey) ;

  // traitement de la variable Send

  if (pSend_Now_Oui->GetCheck() == BF_CHECKED)
      *pbSendNow = true;
  if (pSend_Now_Non->GetCheck() == BF_CHECKED)
      *pbSendNow = false;

  // traitement de la variable Hash

  if (pHash_Oui->GetCheck() == BF_CHECKED)
      *pbHash = true;
  if (pHash_Non->GetCheck() == BF_CHECKED)
      *pbHash = false;

  if (pAutoSave_Oui->GetCheck() == BF_CHECKED)
      *pbAutoSauve = true;
  if (pAutoSave_Non->GetCheck() == BF_CHECKED)
      *pbAutoSauve = false;

  if (pAutoOpen_Oui->GetCheck() == BF_CHECKED)
      *pbAutoOpenLdv = true;
  if (pAutoOpen_Non->GetCheck() == BF_CHECKED)
      *pbAutoOpenLdv = false;

  if (pOpenMode_Heure->GetCheck() == BF_CHECKED)
      *psOpenMode = "H";
  if (pOpenMode_Jour->GetCheck() == BF_CHECKED)
      *psOpenMode = "J";
  if (pOpenMode_Semaine->GetCheck() == BF_CHECKED)
      *psOpenMode = "S";
  if (pOpenMode_Mois->GetCheck() == BF_CHECKED)
      *psOpenMode = "M";

  if (pTLconcen_Oui->GetCheck() == BF_CHECKED)
      *pbTLconcernLdv = true;
  if (pTLconcen_Non->GetCheck() == BF_CHECKED)
      *pbTLconcernLdv = false;

  // traitement du Login
  char szLogin[128];
  pLogin->GetText(szLogin, 127);
  *psLogin = string(szLogin) ;

  // traitement du password
  char szUserPas[128] ;
  pUserPassword->GetText(szUserPas, 127) ;
  *psUserPassword = string(szUserPas);

  // traitement du mail
  char szMail[512];
  pMail->GetText(szMail, 511);
  *psMailSeveur = string(szMail);

  // traitement de l'UserID  char szUserId[128];  pUserId->GetText(szUserId, 127) ;
  *psUserId = string(szUserId) ;

  // traitement de l'URL serveur
  char szURLServeur[128];
  pServerURL->GetText(szURLServeur, 127) ;
  *psURLServeur = string(szURLServeur) ;

  // traitement de l'URL service
  char szURLService[512];
  pServiceUrl->GetText(szURLService, 511) ;
  *psServiceUrl = string(szURLService) ;

  // traitement du titre du service
  char szURLTitle[512];
  pServiceTitle->GetText(szURLTitle, 511) ;
  *psServiceTitle = string(szURLTitle) ;

  TDialog::CmOk() ;
}

/*
//***********************************************************************************************
                             /*CLASSE PROMETHEFILE  */
//************************************************************************************************


NSPrometheFile::NSPrometheFile(NSContexte* pCtx)
               :NSRoot(pCtx)
{
}

NSPrometheFile::~NSPrometheFile()
{
}

void
NSPrometheFile::Lancer()
{
	NSEpisodus* pEpisodus = pContexte->getEpisodus() ;

	bool    bHashFalse = false ;

  //
  // ---------Instance de classe, passage de paramètres par adresse--------
  //
  NSPrometheParamsDlg *pPrometheParams
                    = new NSPrometheParamsDlg(NULL, pContexte,
                                              &(pEpisodus->sPrometheUserLogin),
                                              &(pEpisodus->sPromethePrincipe),
                                              &(pEpisodus->bSendNow),
                                              &(bHashFalse),
                                              &(pEpisodus->sPrometheUserPaswd),
                                              &(pEpisodus->sPrometheUserId),
                                              &(pEpisodus->sPrometheMail),
                                              &(pEpisodus->bAutoFlocule),
                                              &(pEpisodus->sPrometheURL),
                                              &(pEpisodus->bActiveGlobal),
                                              &(pEpisodus->sCaptureKey),
                                              &(pEpisodus->bActiveBabylon),
                                              &(pEpisodus->sBabylonKey),
                                              &(pEpisodus->bAutoSave),
                                              &(pEpisodus->sOpenModeLdV),
                                              &(pEpisodus->bAutoOpenLdV),
                                              &(pEpisodus->bAllowFreeTextLdV),
                                              &(pEpisodus->sServiceUrl),
                                              &(pEpisodus->sServiceTitle));

	//
  // ----------------Lance boîte de dialogue ----------------------
  //
  int iresult = pPrometheParams->Execute() ;
  if (iresult == IDOK)
  {
    string sUserId = string("") ;
    NSUtilisateurChoisi* pUser = pContexte->getUtilisateur() ;
	  if (pUser)
      sUserId = pUser->getID() ;

  	EcrireFichier(sUserId) ;
  }

	delete pPrometheParams ;
}

boolNSPrometheFile::EcrireFichier(string sUserId)
{
  NSToDoTask *pToDoTask = new NSToDoTask ;
  pToDoTask->setWhatToDo(string("SaveEpisodusData")) ;
  pToDoTask->setParam1(sUserId) ;
  pToDoTask->sendMe(pContexte->getSuperviseur()) ;

	// return pContexte->getSuperviseur()->getEpisodus()->saveParams() ;
  return true ;
}

// -----------------------------------------------------------------
//
//  Méthodes de NsMailToolsDialog
//
// -----------------------------------------------------------------
// -----------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NsMailToolsDialog, NSUtilDialog)
	EV_COMMAND(IDOK, CmOk),
	EV_CHILD_NOTIFY(IDC_CLASSICAL, BN_CLICKED, startMailEdit),
	EV_CHILD_NOTIFY(IDC_APICRYPT,  BN_CLICKED, startApicryptEdit),
END_RESPONSE_TABLE;

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NsMailToolsDialog::NsMailToolsDialog(TWindow* pere, NSContexte* pCtx)
                  :NSUtilDialog(pere, pCtx, "IDD_EMAIL_MANAGER_DLG", pNSResModule)
{
try
{
	pMail = new TButton(this, IDC_CLASSICAL) ;
	pApi  = new TButton(this, IDC_APICRYPT) ;
} // try
catch (...)
{
	erreur("Exception NsMailToolsDialog ctor.", standardError, 0) ;
}
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NsMailToolsDialog::~NsMailToolsDialog()
{
	delete pMail ;
	delete pApi ;
}

//---------------------------------------------------------------------------
//  Initialise la boite de dialogue
//---------------------------------------------------------------------------
void
NsMailToolsDialog::SetupWindow()
{
	NSUtilDialog::SetupWindow() ;
}

void
NsMailToolsDialog::startMailEdit()
{
try
{
	NSEmailFile fileManager(pContexte) ;
	fileManager.Lancer(this) ;
}
catch (...)
{
	erreur("NsMailToolsDialog::startMailEdit", standardError, 0) ;
	return ;
}
}

void
NsMailToolsDialog::startApicryptEdit()
{
try
{
	NSApiEmailFile fileManager(pContexte) ;
	fileManager.Lancer(this) ;
}
catch (...)
{
	erreur("NsMailToolsDialog::startApicryptEdit", standardError, 0) ;
	return ;
}
}

void
NsMailToolsDialog::CmOk()
{
	CloseWindow(IDOK) ;
}

//****************************************************************************                             /*CLASSE NSEMAIL  */
//****************************************************************************

/*****************************************************************************/

NSEmailFile::NSEmailFile(NSContexte* pCtx)            :NSRoot(pCtx)
{
}

NSEmailFile::~NSEmailFile()
{
}

void
NSEmailFile::Lancer(TWindow* pere)
{
  NSUtilisateurChoisi* pUtilisateurChoisi = pContexte->getUtilisateur() ;

  //---------Instance de classe, passage de paramètres par adresse--------
  //
/*
  NSEMAIL_DAT_FileDLG *pEMAIL_DAT_File = new NSEMAIL_DAT_FileDLG(pere,
                              &(pUtilisateurChoisi->pMail->sUrlSMTP),
                              &(pUtilisateurChoisi->pMail->iPortSMTP),
                              &(pUtilisateurChoisi->pMail->sUserSMTP),
                              &(pUtilisateurChoisi->pMail->sMailExp),
                              &(pUtilisateurChoisi->pMail->sUrlPOP3),
                              &(pUtilisateurChoisi->pMail->iPortPOP3),
                              &(pUtilisateurChoisi->pMail->sUserPOP3),
                              &(pUtilisateurChoisi->pMail->sPassPOP3),
                              &(pUtilisateurChoisi->pMail->iTypeEnvoi));
*/

  NSMailParams* pMail = pUtilisateurChoisi->getRegularMail() ;
  if ((NSMailParams*) NULL == pMail)
    return ;

  string sUrlSMTP  = pMail->getUrlSmtp() ;
  int    iPortSMTP = pMail->getPortSmtp() ;
  string sUserSMTP = pMail->getUserSmtp() ;
  string sPassSMTP = pMail->getPassSmtp() ;
  bool   bNeedPass = pMail->isPassNeeded() ;
  string sName     = pMail->getName() ;
  string sFrom     = pMail->getFrom() ;
  string sReplyTo  = pMail->getReplyTo() ;
  int    iTypeEmit = pMail->getSendingModality() ;

  NSEMAILSMTP_DAT_FileDLG *pEMAIL_DAT_File = new NSEMAILSMTP_DAT_FileDLG(pere, pContexte,
                                                        &sUrlSMTP,
                                                        &iPortSMTP,
                                                        &sUserSMTP,
                                                        &sFrom,
                                                        &iTypeEmit,
                                                        &sPassSMTP,
                                                        &sReplyTo,
                                                        &sName,
                                                        &bNeedPass) ;

  //----------------Lance boîte de dialog----------------------
  //
  int iresult = pEMAIL_DAT_File->Execute() ;

  delete pEMAIL_DAT_File ;
  if (IDOK != iresult)
    return ;

  pMail->setUrlSmtp(sUrlSMTP) ;
  pMail->setPortSmtp(iPortSMTP) ;
  pMail->setUserSmtp(sUserSMTP) ;
  pMail->setPassSmtp(sPassSMTP) ;
  pMail->mustAuthenticate(bNeedPass) ;
  pMail->setFrom(sFrom) ;
  pMail->setName(sName) ;
  pMail->setReplyTo(sReplyTo) ;
  pMail->setSendingModality(iTypeEmit) ;

  EcrireFichier() ;
}

bool
NSEmailFile::EcrireFichier()
{
  //NSEpisodus* pEpisodus = pContexte->getSuperviseur()->pEpisodus;

  /*-----------Remplace variables Episodus par les variables (par pointeur)------------*/
  NSUtilisateurChoisi* pUtilisateurChoisi = pContexte->getUtilisateur() ;  if ((NSUtilisateurChoisi*) NULL == pUtilisateurChoisi)
    return false ;

  NSMailParams* pMail = pUtilisateurChoisi->getRegularMail() ;
  if ((NSMailParams*) NULL == pMail)
    return false ;

  string sFileName = string("email") + pUtilisateurChoisi->getNss() + string(".dat") ;

  ofstream outFile ;
  outFile.open(sFileName.c_str(), ios::out) ;
  if (!outFile)
  {
    string sErreur = "Erreur d'ouverture du fichier " + sFileName ;
    erreur(sErreur.c_str(), standardError, 0) ;
    return false ;
  }

  outFile << (string("UrlSMTP  ") + pMail->getUrlSmtp() + string("\n")) ;

  char szPort[31] ;
  sprintf(szPort, "%d", pMail->getPortSmtp()) ;
  outFile << (string("PortSMTP ") + string(szPort) + string("\n")) ;

  outFile << (string("UserSMTP ") + pMail->getUserSmtp() + string("\n")) ;
  if (string("") != pMail->getPassSmtp())
  {
    outFile << (string("PassSMTP ") + pMail->getPassSmtp() + string("\n")) ;
    if (pMail->isPassNeeded())
      outFile << (string("Authenticate Yes") + string("\n")) ;
    else
      outFile << (string("Authenticate No") + string("\n")) ;
  }

  if (string("") != pMail->getName())
    outFile << (string("NameFrom ") + pMail->getName()  + string("\n"))  ;
  outFile << (string("MailExp  ") + pMail->getFrom()  + string("\n"))  ;
  if (string("") != pMail->getReplyTo())
    outFile << (string("MailReplyTo ") + pMail->getReplyTo()  + string("\n"))  ;
  if (string("") != pMail->getSender())
    outFile << (string("MailSender ") + pMail->getSender()  + string("\n"))  ;

  outFile << (string("UrlPOP3  ") + pMail->getUrlPop3()  + string("\n")) ;

  sprintf(szPort, "%d", pMail->getPortPop3()) ;
  outFile << (string("PortPOP3 ") + string(szPort) + string("\n")) ;

  outFile << (string("UserPOP3 ") + pMail->getUserPop3() + string("\n")) ;
  outFile << (string("PassPOP3 ") + pMail->getPassPop3() + string("\n")) ;

  if      (NSMAIL_MAPI == pMail->getSendingModality())
    outFile << (string("Methode  MAPI") + string("\n")) ;
  else if (NSMAIL_NAUTILUS == pMail->getSendingModality())
    outFile << (string("Methode  NAUTILUS") + string("\n")) ;

  outFile.close() ;
  return true ;
}

DEFINE_RESPONSE_TABLE1(NSEMAIL_DAT_FileDLG, TDialog)
  EV_COMMAND(IDOK, CmOk),
  EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE ;

NSEMAIL_DAT_FileDLG::NSEMAIL_DAT_FileDLG(TWindow* pere, string*   psUrl, int* piPort,
                                            string*  psUser, string*  psMail, string*  psUrlPOP,
                                            int*  piPortPOP, string*  psUserPOP, string*  psPassPOP,
                                            int* piMethode, string* psPass, string* psReplyTo,
                                            string* psName, bool* pbNeedPass)
                    :TDialog(pere, "IDD_EMAIL_DLG", pNSResModule)
{
  pPortSMTP    = new TEdit(this, IDC_PORT_SMTP ) ;
  pUrlSMTP     = new TEdit(this, IDC_URL_SMTP  ) ;
  pUserSMTP    = new TEdit(this, IDC_USER_SMTP ) ;
  pPassSMTP    = new TEdit(this, IDC_PASS_SMTP ) ;

  pMailFrom    = new TEdit(this, IDC_MAIL_FROM  ) ;
  pMailReplyTo = new TEdit(this, IDC_MAIL_REPLY_TO  ) ;

  pUrlPOP3     = new TEdit(this, IDC_URL_POP3 ) ;
  pPortPOP3    = new TEdit(this, IDC_PORT_POP3 ) ;
  pUserPOP3    = new TEdit(this, IDC_USER_POP3 ) ;
  pPassPOP3    = new TEdit(this, IDC_PASS_POP3 );

  pClassique   = new TRadioButton(this, IDC_CLASSIQUE );
  pMAPI        = new TRadioButton(this, IDC_MAPI );

  psUrlSMTP     = psUrl ;
  piPortSMTP    = piPort ;
  psUserSMTP    = psUser ;
  psPassSMTP    = psPass ;
  pbUsePass     = pbNeedPass ;
  psNameFrom    = psName ;
  psMailFrom    = psMail ;
  psMailReplyTo = psReplyTo ;
  psUrlPOP3     = psUrlPOP ;
  piPortPOP3    = piPortPOP ;
  psUserPOP3    = psUserPOP ;
  psPassPOP3    = psPassPOP ;
  piSendMode    = piMethode ;
}

NSEMAIL_DAT_FileDLG::~NSEMAIL_DAT_FileDLG()
{
  delete pPortSMTP ;
  delete pUrlSMTP ;
  delete pUserSMTP ;
  delete pPassSMTP ;

  delete pMailReplyTo ;
  delete pMailFrom ;

  delete pUrlPOP3 ;
  delete pPortPOP3 ;
  delete pUserPOP3 ;
  delete pPassPOP3 ;

  delete pClassique ;
  delete pMAPI ;
}

voidNSEMAIL_DAT_FileDLG::CmCancel()
{
  Destroy(IDCANCEL);
}

void
NSEMAIL_DAT_FileDLG::SetupWindow()
{
  TDialog::SetupWindow() ;

      /*init de la fenetre */

  pUrlSMTP->SetText(psUrlSMTP->c_str()) ;

  char szPortSMTP[128] ;
  sprintf(szPortSMTP, "%d", *piPortSMTP) ;
  pPortSMTP->SetText(szPortSMTP) ;

  pUserSMTP->SetText(psUserSMTP->c_str()) ;
  pPassSMTP->SetText(psPassSMTP->c_str()) ;

  pMailFrom->SetText(psMailFrom->c_str()) ;
  pMailReplyTo->SetText(psMailReplyTo->c_str()) ;

  pUrlPOP3->SetText(psUrlPOP3->c_str()) ;

  char szPortPOP3[128] ;
  sprintf(szPortPOP3, "%d", *piPortPOP3) ;
  pPortPOP3->SetText(szPortPOP3) ;

  pUserPOP3->SetText(psUserPOP3->c_str()) ;
  pPassPOP3->SetText(psPassPOP3->c_str()) ;

  if (*piSendMode == NSMAIL_MAPI)
    pMAPI->SetCheck(BF_CHECKED) ;
  else
    pClassique->SetCheck(BF_CHECKED) ;
}

void
NSEMAIL_DAT_FileDLG::CmOk()
{
  char szBuffer[128] ;

  pUrlSMTP->GetText(szBuffer, 127) ;
  *psUrlSMTP = szBuffer ;

  char szPortSMTP[31] ;
  pPortSMTP->GetText(szPortSMTP, 30);
  *piPortSMTP = atoi(szPortSMTP) ;

  pUserSMTP->GetText(szBuffer, 127) ;
  *psUserSMTP = szBuffer ;
  pPassSMTP->GetText(szBuffer, 127) ;
  *psPassSMTP = szBuffer ;

  pMailFrom->GetText(szBuffer, 127) ;
  *psMailFrom = szBuffer ;
  pMailReplyTo->GetText(szBuffer, 127) ;
  *psMailReplyTo = szBuffer ;

  // traitement de l'urlpop3
  pUrlPOP3->GetText(szBuffer, 127) ;
  *psUrlPOP3 = szBuffer ;

  char szPortPOP3[31];
  pPortPOP3->GetText(szPortPOP3, 30) ;
  *piPortPOP3 = atoi(szPortPOP3);

  pUserPOP3->GetText(szBuffer, 127) ;
  *psUserPOP3 = szBuffer ;

  pPassPOP3->GetText(szBuffer, 127) ;
  *psPassPOP3 = szBuffer ;

  if      (pMAPI->GetCheck() == BF_CHECKED)
    *piSendMode = NSMAIL_MAPI;
  else if (pClassique->GetCheck() == BF_CHECKED)
    *piSendMode = NSMAIL_NAUTILUS;

  TDialog::CmOk() ;
}

DEFINE_RESPONSE_TABLE1(NSEMAILSMTP_DAT_FileDLG, TDialog)
  EV_COMMAND(IDOK, CmOk),
  EV_COMMAND(IDCANCEL, CmCancel),
  EV_COMMAND(IDC_SMTP_LUCKY, setMailParams),
END_RESPONSE_TABLE ;

NSEMAILSMTP_DAT_FileDLG::NSEMAILSMTP_DAT_FileDLG(TWindow* pere, NSContexte* pCtx,
                                                 string* psUrl, int* piPort,
                                                 string* psUser, string* psMail,
                                                 int* piMethode, string* psPass,
                                                 string* psReplyTo, string* psName,
                                                 bool* pbNeedPass, TResId resID)
                        :NSUtilDialog(pere, pCtx, resID, pNSResModule)
{
  pNameFrom    = new TEdit(this, IDC_NAME_FROM) ;
  pMailFrom    = new TEdit(this, IDC_MAIL_FROM) ;
  pMailReplyTo = new TEdit(this, IDC_MAIL_REPLY_TO) ;

  pPortSMTP   = new TEdit(this, IDC_PORT_SMTP) ;
  pUrlSMTP    = new TEdit(this, IDC_URL_SMTP) ;
  pUserSMTP   = new TEdit(this, IDC_USER_SMTP) ;
  pPassSMTP   = new TEdit(this, IDC_PASS_SMTP) ;

  pClassique  = new TRadioButton(this, IDC_CLASSIQUE) ;
  pMAPI       = new TRadioButton(this, IDC_MAPI) ;

  psUrlSMTP     = psUrl ;
  piPortSMTP    = piPort ;
  psUserSMTP    = psUser ;
  psPassSMTP    = psPass ;
  pbUsePass     = pbNeedPass ;
  psNameFrom    = psName ;
  psMailFrom    = psMail ;
  psMailReplyTo = psReplyTo ;
  piSendMode    = piMethode ;
}

NSEMAILSMTP_DAT_FileDLG::~NSEMAILSMTP_DAT_FileDLG()
{
  delete pNameFrom ;
  delete pMailFrom ;
  delete pMailReplyTo ;
  delete pPortSMTP ;
  delete pUrlSMTP ;
  delete pUserSMTP ;
  delete pPassSMTP ;
  delete pClassique ;
  delete pMAPI ;
}

voidNSEMAILSMTP_DAT_FileDLG::CmCancel()
{
  Destroy(IDCANCEL) ;
}

void
NSEMAILSMTP_DAT_FileDLG::SetupWindow()
{
  TDialog::SetupWindow() ;

  // Setting windows controls

  pUrlSMTP->SetText(psUrlSMTP->c_str()) ;

  char szPortSMTP[128] ;
  sprintf(szPortSMTP, "%d", *piPortSMTP) ;
  pPortSMTP->SetText(szPortSMTP) ;

  pUserSMTP->SetText(psUserSMTP->c_str()) ;
  pPassSMTP->SetText(psPassSMTP->c_str()) ;

  pNameFrom->SetText(psNameFrom->c_str()) ;
  pMailFrom->SetText(psMailFrom->c_str()) ;
  pMailReplyTo->SetText(psMailReplyTo->c_str()) ;

  if (*piSendMode == NSMAIL_MAPI)
    pMAPI->SetCheck(BF_CHECKED) ;
  else
    pClassique->SetCheck(BF_CHECKED) ;
}

void
NSEMAILSMTP_DAT_FileDLG::CmOk()
{
  char szBuffer[128];

  pUrlSMTP->GetText(szBuffer, 127) ;
  *psUrlSMTP = string(szBuffer) ;

  char szPortSMTP[31] ;
  pPortSMTP->GetText(szPortSMTP, 30);
  *piPortSMTP = atoi(szPortSMTP) ;

  pUserSMTP->GetText(szBuffer, 127) ;
  *psUserSMTP = string(szBuffer) ;

  pPassSMTP->GetText(szBuffer, 127) ;
  *psPassSMTP = string(szBuffer) ;
  if ('\0' == szBuffer[0])
    *pbUsePass = false ;
  else
    *pbUsePass = true ;

  pNameFrom->GetText(szBuffer, 127) ;
  *psNameFrom = string(szBuffer) ;
  pMailFrom->GetText(szBuffer, 127) ;
  *psMailFrom = string(szBuffer) ;
  pMailReplyTo->GetText(szBuffer, 127) ;
  *psMailReplyTo = string(szBuffer) ;

  if      (pMAPI->GetCheck() == BF_CHECKED)
    *piSendMode = NSMAIL_MAPI;
  else if (pClassique->GetCheck() == BF_CHECKED)
    *piSendMode = NSMAIL_NAUTILUS;

  TDialog::CmOk() ;
}

void
NSEMAILSMTP_DAT_FileDLG::setMailParams()
{
  char buffer[128] ;
  pMailFrom->GetText(buffer, 127) ;

  if ('\0' == buffer[0])
    return ;

  string sBuffer = string(buffer) ;
  size_t iAtPos = sBuffer.find("@") ;
  if ((string::npos == iAtPos) || (0 == iAtPos))
    return ;

  size_t iBuffLen = strlen(sBuffer.c_str()) ;
  if (iBuffLen - 1 == iAtPos)
    return ;

  string sMailUser = string(sBuffer, 0, iAtPos) ;
  string sDomain   = string(sBuffer, iAtPos + 1, iBuffLen - iAtPos - 1) ;

  pUserSMTP->SetText(sMailUser.c_str()) ;

  NSMailManager mailManager(pContexte) ;

  string sUrlSmtp = string("") ;
  string sUrlPop  = string("") ;
  string sUrlImap = string("") ;
  mailManager.getUrlsForDomain(sDomain, sUrlSmtp, sUrlPop, sUrlImap) ;

  if (string("") != sUrlSmtp)
    pUrlSMTP->SetText(sUrlSmtp.c_str()) ;
}

//***********************************************************************************************
                             /*CLASSE NSAPIEMAIL  */
//************************************************************************************************

NSApiEmailFile::NSApiEmailFile(NSContexte* pCtx)
               :NSRoot(pCtx)
{
}

NSApiEmailFile::~NSApiEmailFile()
{
}

void
NSApiEmailFile::Lancer(TWindow* pere)
{
  NSUtilisateurChoisi* pUtilisateurChoisi = pContexte->getUtilisateur() ;

  //---------Instance de classe, passage de paramètres par adresse--------
  //
/*
  NSAPIEMAIL_DAT_FileDLG *pEMAIL_DAT_File = new NSAPIEMAIL_DAT_FileDLG(pere,
                              &(pUtilisateurChoisi->pMailApicrypt->sUrlSMTP),
                              &(pUtilisateurChoisi->pMailApicrypt->iPortSMTP),
                              &(pUtilisateurChoisi->pMailApicrypt->sUserSMTP),
                              &(pUtilisateurChoisi->pMailApicrypt->sMailExp),
                              &(pUtilisateurChoisi->pMailApicrypt->sUrlPOP3),
                              &(pUtilisateurChoisi->pMailApicrypt->iPortPOP3),
                              &(pUtilisateurChoisi->pMailApicrypt->sUserPOP3),
                              &(pUtilisateurChoisi->pMailApicrypt->sPassPOP3),
                              &(pUtilisateurChoisi->pMailApicrypt->iTypeEnvoi),
                              &(pUtilisateurChoisi->pMailApicrypt->sApiUser),
                              &(pUtilisateurChoisi->pMailApicrypt->sApiKeysDir)) ;
*/

  NSMailParams* pMailApicrypt = pUtilisateurChoisi->getMailApicrypt() ;
  if ((NSMailParams*) NULL == pMailApicrypt)
    return ;

  string sUrlSMTP  = pMailApicrypt->getUrlSmtp() ;
  int    iPortSMTP = pMailApicrypt->getPortSmtp() ;
  string sUserSMTP = pMailApicrypt->getUserSmtp() ;
  string sPassSMTP = pMailApicrypt->getPassSmtp() ;
  bool   bNeedPass = pMailApicrypt->isPassNeeded() ;
  string sReplyTo  = pMailApicrypt->getReplyTo() ;
  string sFrom     = pMailApicrypt->getFrom() ;
  string sName     = pMailApicrypt->getName() ;
  int    iTypeEmit = pMailApicrypt->getSendingModality() ;

  string sApiUser     = pMailApicrypt->getApiUser() ;
  string sApiKeysDir  = pMailApicrypt->getApiKeyDir() ;
  bool   bUseApiProxy = pMailApicrypt->mustUseApiProxy() ;

  NSAPIEMAILSMTP_DAT_FileDLG *pEMAIL_DAT_File = new NSAPIEMAILSMTP_DAT_FileDLG(pere, pContexte,
                                                     &sUrlSMTP,
                                                     &iPortSMTP,
                                                     &sUserSMTP,
                                                     &sPassSMTP,
                                                     &bNeedPass,
                                                     &sFrom,
                                                     &sReplyTo,
                                                     &sName,
                                                     &iTypeEmit,
                                                     &sApiUser,
                                                     &sApiKeysDir,
                                                     &bUseApiProxy) ;

  //----------------Lance boîte de dialog----------------------
  //
  int iresult = pEMAIL_DAT_File->Execute() ;
  delete pEMAIL_DAT_File ;

  if (IDOK != iresult)
    return ;

  pMailApicrypt->setUrlSmtp(sUrlSMTP) ;
  pMailApicrypt->setPortSmtp(iPortSMTP) ;
  pMailApicrypt->setPassSmtp(sPassSMTP) ;
  pMailApicrypt->setUserSmtp(sUserSMTP) ;
  pMailApicrypt->mustAuthenticate(bNeedPass) ;
  pMailApicrypt->setName(sName) ;
  pMailApicrypt->setFrom(sFrom) ;
  pMailApicrypt->setReplyTo(sReplyTo) ;
  pMailApicrypt->setSendingModality(iTypeEmit) ;

  pMailApicrypt->setApiUser(sApiUser) ;
  pMailApicrypt->setApiKeyDir(sApiKeysDir) ;
  pMailApicrypt->setUseApiProxy(bUseApiProxy) ;

  EcrireFichier() ;
}

bool
NSApiEmailFile::EcrireFichier()
{
  //NSEpisodus* pEpisodus = pContexte->getSuperviseur()->pEpisodus;

  /*-----------Remplace variables Episodus par les variables (par pointeur)------------*/
  NSUtilisateurChoisi* pUtilisateurChoisi = pContexte->getUtilisateur() ;  if (NULL == pUtilisateurChoisi)
    return false ;

  NSMailParams* pMailApicrypt = pUtilisateurChoisi->getMailApicrypt() ;
  if ((NSMailParams*) NULL == pMailApicrypt)
    return false ;

	string sFileName = string("emailApicrypt") + pUtilisateurChoisi->getNss() + string(".dat") ;

  ofstream outFile ;
  outFile.open(sFileName.c_str(), ios::out) ;
  if (!outFile)
  {
    string sErreur = "Erreur d'ouverture du fichier " + sFileName ;
    erreur(sErreur.c_str(), standardError, 0) ;
    return false;
  }

  outFile << (string("UrlSMTP  ") + pMailApicrypt->getUrlSmtp() + string("\n")) ;

  char szPort[31] ;
  sprintf(szPort, "%d", pMailApicrypt->getPortSmtp()) ;
  outFile << (string("PortSMTP ") + string(szPort) + string("\n")) ;

  outFile << (string("UserSMTP ") + pMailApicrypt->getUserSmtp() + string("\n")) ;
  if (string("") != pMailApicrypt->getPassSmtp())
  {
    outFile << (string("PassSMTP ") + pMailApicrypt->getPassSmtp() + string("\n")) ;
    if (pMailApicrypt->isPassNeeded())
      outFile << (string("Authenticate Yes") + string("\n")) ;
    else
      outFile << (string("Authenticate No") + string("\n")) ;
  }

  if (string("") != pMailApicrypt->getName())
    outFile << (string("NameFrom ") + pMailApicrypt->getName() + string("\n"))  ;
  outFile << (string("MailExp  ") + pMailApicrypt->getFrom() + string("\n"))  ;
  if (string("") != pMailApicrypt->getReplyTo())
    outFile << (string("MailReplyTo ") + pMailApicrypt->getReplyTo() + string("\n"))  ;
  if (string("") != pMailApicrypt->getSender())
    outFile << (string("MailSender ") + pMailApicrypt->getSender() + string("\n"))  ;

  outFile << (string("UrlPOP3  ") + pMailApicrypt->getUrlPop3()  + string("\n")) ;

  sprintf(szPort, "%d", pMailApicrypt->getPortPop3());
  outFile << (string("PortPOP3 ") + string(szPort) + string("\n")) ;

  outFile << (string("UserPOP3 ") + pMailApicrypt->getUserPop3() + string("\n")) ;
  outFile << (string("PassPOP3 ") + pMailApicrypt->getPassPop3() + string("\n")) ;

  if      (NSMAIL_MAPI == pMailApicrypt->getSendingModality())
    outFile << (string("Methode  MAPI") + string("\n")) ;
  else if (NSMAIL_NAUTILUS == pMailApicrypt->getSendingModality())
    outFile << (string("Methode  NAUTILUS") + string("\n")) ;

  if (string("") != pMailApicrypt->getApiUser())
    outFile << (string("UserApicrypt ") + pMailApicrypt->getApiUser() + string("\n")) ;
  if (string("") != pMailApicrypt->getApiKeyDir())
    outFile << (string("ApiKeysDir ")   + pMailApicrypt->getApiKeyDir() + string("\n")) ;
  if (pMailApicrypt->mustUseApiProxy())
    outFile << (string("UseApiProxy true\n")) ;

  outFile.close() ;
  return true ;
}

DEFINE_RESPONSE_TABLE1(NSAPIEMAIL_DAT_FileDLG, TDialog)
  EV_COMMAND(IDOK, CmOk),
  EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE ;

NSAPIEMAIL_DAT_FileDLG::NSAPIEMAIL_DAT_FileDLG(TWindow* pere, string* psUrl, int* piPort,
                                            string* psUser, string* psMail, string* psUrlPOP,
                                            int* piPortPOP, string* psUserPOP, string* psPassPOP,
                                            int* piMethode, string* psApiUser, string* psApiKeyDir,
                                            string* psPass, string* psReplyTo, bool* pbNeedPass)
                       :TDialog(pere, "IDD_APICRYPT_DLG", pNSResModule)
{
  pApiUser   = new TEdit(this, IDC_API_USER  ) ;
  pApiKeyDir = new TEdit(this, IDC_API_DIR   ) ;
  pPortSMTP  = new TEdit(this, IDC_PORT_SMTP ) ;
  pUserSMTP  = new TEdit(this, IDC_USER_SMTP ) ;
  pMailFrom  = new TEdit(this, IDC_MAIL_FROM  ) ;
  pUrlPOP3   = new TEdit(this, IDC_URL_POP3  ) ;
  pPortPOP3  = new TEdit(this, IDC_PORT_POP3 ) ;
  pUserPOP3  = new TEdit(this, IDC_USER_POP3 ) ;
  pPassPOP3  = new TEdit(this, IDC_PASS_POP3 ) ;
  pUrlSMTP   = new TEdit(this, IDC_URL_SMTP  ) ;
  pClassique = new TRadioButton(this, IDC_CLASSIQUE ) ;
  pMAPI      = new TRadioButton(this, IDC_MAPI ) ;

  psApicryptUser   = psApiUser ;
  psApicryptKeyDir = psApiKeyDir ;

  psUrlSMTP     = psUrl ;
  piPortSMTP    = piPort ;
  psUserSMTP    = psUser ;
  psPassSMTP    = psPass ;
  pbUsePassSMTP = pbNeedPass ;
  psMailReplyTo = psReplyTo ;
  psMailFrom    = psMail ;
  psUrlPOP3     = psUrlPOP ;
  piPortPOP3    = piPortPOP ;
  psUserPOP3    = psUserPOP ;
  psPassPOP3    = psPassPOP ;
  piSendMode    = piMethode ;
}

NSAPIEMAIL_DAT_FileDLG::~NSAPIEMAIL_DAT_FileDLG()
{
  delete pApiUser ;
  delete pApiKeyDir ;
  delete pPortSMTP ;
  delete pUserSMTP ;
  delete pMailFrom ;
  delete pUrlPOP3 ;
  delete pPortPOP3 ;
  delete pUserPOP3 ;
  delete pPassPOP3 ;
  delete pUrlSMTP ;
  delete pClassique ;
  delete pMAPI ;
}

voidNSAPIEMAIL_DAT_FileDLG::CmCancel()
{
  Destroy(IDCANCEL) ;
}

void
NSAPIEMAIL_DAT_FileDLG::SetupWindow()
{
  TDialog::SetupWindow();

      /*init de la fenetre */

  pApiUser->SetText(psApicryptUser->c_str()) ;
  pApiKeyDir->SetText(psApicryptKeyDir->c_str()) ;

  pUrlSMTP->SetText(psUrlSMTP->c_str()) ;

  char szPortSMTP[128];
  sprintf(szPortSMTP, "%d", *piPortSMTP);
  pPortSMTP->SetText(szPortSMTP);

  pUserSMTP->SetText(psUserSMTP->c_str());

  pMailFrom->SetText(psMailFrom->c_str());

  pUrlPOP3->SetText(psUrlPOP3->c_str());

  char szPortPOP3[128];
  sprintf(szPortPOP3, "%d", *piPortPOP3);
  pPortPOP3->SetText(szPortPOP3);

  pUserPOP3->SetText(psUserPOP3->c_str());

  pPassPOP3->SetText(psPassPOP3->c_str());

  if (*piSendMode == NSMAIL_MAPI)
      pMAPI->SetCheck(BF_CHECKED);
  else
      pClassique->SetCheck(BF_CHECKED);
}

void
NSAPIEMAIL_DAT_FileDLG::CmOk()
{
  char buffer[1024];

  pApiUser->GetText(buffer, 1024) ;
  *psApicryptUser = string(buffer) ;

  pApiKeyDir->GetText(buffer, 1024) ;
  *psApicryptKeyDir = string(buffer) ;

  pUrlSMTP->GetText(buffer, 1024) ;
  *psUrlSMTP = string(buffer) ;

  pPortSMTP->GetText(buffer, 30);
  *piPortSMTP = atoi(buffer) ;

  pUserSMTP->GetText(buffer, 1024) ;
  *psUserSMTP = string(buffer) ;

  pMailFrom->GetText(buffer, 1024) ;
  *psMailFrom = string(buffer) ;

  // traitement de l'urlpop3
  pUrlPOP3->GetText(buffer, 1024) ;
  *psUrlPOP3 = string(buffer) ;

  pPortPOP3->GetText(buffer, 30) ;
  *piPortPOP3 = atoi(buffer);

  pUserPOP3->GetText(buffer, 1024) ;
  *psUserPOP3 = string(buffer);

  pPassPOP3->GetText(buffer, 1024) ;
  *psPassPOP3 = string(buffer);

  if      (pMAPI->GetCheck() == BF_CHECKED)
    *piSendMode = NSMAIL_MAPI;
  else if (pClassique->GetCheck() == BF_CHECKED)
    *piSendMode = NSMAIL_NAUTILUS;

  TDialog::CmOk() ;
}


DEFINE_RESPONSE_TABLE1(NSAPIEMAILSMTP_DAT_FileDLG, NSEMAILSMTP_DAT_FileDLG)
  EV_COMMAND(IDOK, CmOk),
  EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE ;

NSAPIEMAILSMTP_DAT_FileDLG::NSAPIEMAILSMTP_DAT_FileDLG(TWindow* pere,
                               NSContexte* pCtx, string* psUrl,
                               int* piPort, string* psUser, string* psPass,
                               bool* pbUsePass, string* psMail, string* psReplyTo,
                               string* psName, int* piMethode, string* psApiUser,
                               string* psApiKeyDir, bool* pBehindProxy)
                       :NSEMAILSMTP_DAT_FileDLG(pere, pCtx, psUrl, piPort,
                               psUser, psMail, piMethode, psPass, psReplyTo, psName, pbUsePass,
                               "IDD_APICRYPT_SMTP_DLG")
{
  _pApiUser     = new TEdit(this, IDC_API_USER) ;
  _pApiKeyDir   = new TEdit(this, IDC_API_DIR) ;
  _pBehindProxy = new TCheckBox(this, IDC_PROXY) ;

  _psApicryptUser   = psApiUser ;
  _psApicryptKeyDir = psApiKeyDir ;
  _pbBehindProxy    = pBehindProxy ;
}

NSAPIEMAILSMTP_DAT_FileDLG::~NSAPIEMAILSMTP_DAT_FileDLG()
{
  delete _pApiUser ;
  delete _pApiKeyDir ;
  delete _pBehindProxy ;
}

voidNSAPIEMAILSMTP_DAT_FileDLG::CmCancel()
{
  Destroy(IDCANCEL) ;
}

void
NSAPIEMAILSMTP_DAT_FileDLG::SetupWindow()
{
  TDialog::SetupWindow() ;

      /*init de la fenetre */

  _pApiUser->SetText(_psApicryptUser->c_str()) ;
  _pApiKeyDir->SetText(_psApicryptKeyDir->c_str()) ;

  if (*_pbBehindProxy)
    _pBehindProxy->SetCheck(BF_CHECKED) ;
  else
    _pBehindProxy->SetCheck(BF_UNCHECKED) ;

  pUrlSMTP->SetText(psUrlSMTP->c_str()) ;

  char szPortSMTP[128] ;
  sprintf(szPortSMTP, "%d", *piPortSMTP) ;
  pPortSMTP->SetText(szPortSMTP) ;

  pUserSMTP->SetText(psUserSMTP->c_str()) ;
  pPassSMTP->SetText(psPassSMTP->c_str()) ;

  pMailFrom->SetText(psMailFrom->c_str()) ;
  pMailReplyTo->SetText(psMailReplyTo->c_str()) ;

  if (*piSendMode == NSMAIL_MAPI)
      pMAPI->SetCheck(BF_CHECKED) ;
  else
      pClassique->SetCheck(BF_CHECKED) ;
}

void
NSAPIEMAILSMTP_DAT_FileDLG::CmOk()
{
  char buffer[1024];

  _pApiUser->GetText(buffer, 1024) ;
  *_psApicryptUser = string(buffer) ;

  _pApiKeyDir->GetText(buffer, 1024) ;
  *_psApicryptKeyDir = string(buffer) ;

  if (_pBehindProxy->GetCheck() == BF_CHECKED)
    *_pbBehindProxy = true ;
  else
    *_pbBehindProxy = false ;

  pUrlSMTP->GetText(buffer, 1024) ;
  *psUrlSMTP = string(buffer) ;
  pPortSMTP->GetText(buffer, 30);
  *piPortSMTP = atoi(buffer) ;

  pUserSMTP->GetText(buffer, 1024) ;
  *psUserSMTP = string(buffer) ;

  pPassSMTP->GetText(buffer, 1024) ;
  *psPassSMTP = string(buffer) ;
  if ('\0' == buffer[0])
    *pbUsePass = false ;
  else
    *pbUsePass = true ;

  pMailFrom->GetText(buffer, 1024) ;
  *psMailFrom = string(buffer) ;
  pMailReplyTo->GetText(buffer, 1024) ;
  *psMailReplyTo = string(buffer) ;

  if      (pMAPI->GetCheck() == BF_CHECKED)
    *piSendMode = NSMAIL_MAPI;
  else if (pClassique->GetCheck() == BF_CHECKED)
    *piSendMode = NSMAIL_NAUTILUS;

  TDialog::CmOk() ;
}

//***********************************************************************************************
                             /* CLASSE NSCodeListDlg  */
//************************************************************************************************

DEFINE_RESPONSE_TABLE1(NSCodeListDlg, TDialog)
   EV_CHILD_NOTIFY(IDC_CVT_START, BN_CLICKED, demarre),
   EV_CHILD_NOTIFY(IDC_CVT_STOP,  BN_CLICKED, stoppe),
END_RESPONSE_TABLE ;

NSCodeListDlg::NSCodeListDlg(TWindow* pere, NSContexte* pCtx)
              :NSUtilDialog(pere, pCtx, "IDD_LISTECODES", pNSResModule)
{
  pDateDeb    = new NSUtilEditDate(pContexte, this, IDC_DATE_DEB) ;
  pDateFin    = new NSUtilEditDate(pContexte, this, IDC_DATE_FIN) ;
  pActesPerso = new TRadioButton(this, IDC_LIST_ACTESPERSO) ;
  pActesTous  = new TRadioButton(this, IDC_LIST_ACTESTOUS) ;

  bTourner    = false ;
}

NSCodeListDlg::~NSCodeListDlg()
{
  delete pDateDeb ;
  delete pDateFin ;
  delete pActesPerso ;
  delete pActesTous ;
}

void
NSCodeListDlg::SetupWindow()
{
	NSUtilDialog::SetupWindow() ;
}

void
NSCodeListDlg::demarre()
{
/*
    string sLang = pContexte->getUserLanguage() ;

    bTourner = true ;

    NSPatPaDat* pData = new NSPatPaDat(pContexte);

    pData->lastError = pData->open();
    if (pData->lastError != DBIERR_NONE)
    {
        erreur("Erreur à l'ouverture du fichier NsPatDat.db.", standardError, pData->lastError) ;
        delete pData ;
        return ;
    }

    pData->lastError = pData->debut(dbiWRITELOCK) ;
    if (pData->lastError != DBIERR_NONE)
    {
        erreur("Erreur au positionnement au début de NsPatDat.db.", standardError, pData->lastError) ;
        pData->close() ;
        delete pData ;
        return ;
    }

    char fichier[255] ;
#ifdef _MUE
    strcpy(fichier, "CodeLib1.txt") ;
#else
    strcpy(fichier, "CodeLib0.txt") ;
#endif

    ofstream outFile ;
    string sLine ;

    outFile.open(fichier, ios::out) ;
    if (!outFile)
    {
        string sErreur = "Erreur d'ouverture du fichier " + string(fichier) ;
        erreur(sErreur.c_str(), standardError, 0) ;
        pData->close() ;
        delete pData ;
        return ;
    }

    string sLastSelectedDoc = "" ;

    while (bTourner)
    {
        pData->lastError = pData->getRecord() ;
        if (pData->lastError != DBIERR_NONE)
        {
            erreur("Le fichier PatPaDat est défectueux.", standardError, pData->lastError) ;
            pData->close() ;
            delete pData ;
        }

        if (string(pData->pDonnees->lexique) == "6CISP1")
        {
            string sCodeDoc = pData->pDonnees->getID() ;
            if (sCodeDoc != sLastSelectedDoc)
            {
                sLastSelectedDoc = sCodeDoc ;

                NSPatPathoArray* pPPt ;
#ifndef _MUE
                // NSDocumentHisto* pNSDocumentHisto = new NSDocumentHisto(pContexte) ;
                // pNSDocumentHisto->DonnePatPatho(pNSDocumentHisto->pPatPathoArray) ;
                // pPPt = pNSDocumentHisto->pPatPathoArray ;

                NSDocumentHisto* pNSDocumentHisto = new NSDocumentHisto(new NSDocumentInfo(sCodeDoc, pContexte)) ;
                pPPt = pNSDocumentHisto->pMeta ;
#else
                NSDocumentHisto* pNSDocumentHisto = new NSDocumentHisto(new NSDocumentInfo(sCodeDoc, pContexte)) ;
                pPPt = pNSDocumentHisto->pMeta ;
#endif
                PatPathoIter itPpt = pPPt->ChercherItem("6CISP1") ;
                while ((itPpt != NULL) && (itPpt != pPPt->end()))
                {
                    string sCode = (*itPpt)->getComplement() ;
                    PatPathoIter itPptPere = pPPt->ChercherPere(itPpt) ;
                    if ((itPptPere != NULL) && (itPptPere != pPPt->end()))
                    {
                        string sLexPere = (*itPptPere)->getLexique() ;
                        string sLibelle = "" ;
                        if (sLexPere == "£?????")
                            sLibelle = (*itPptPere)->pDonnees->getTexteLibre() ;
                        else
                        {
                            pContexte->getDico()->donneLibelle(sLang, &sLexPere, &sLibelle) ;
                            sLibelle += string(" (") + sLexPere + string(")") ;
                        }
                        outFile << (sCode + string("\t") + sLibelle + string("\n")) ;
                    }

                    itPpt++;
                    while ((itPpt != pPPt->end()) &&
                            ((*itPpt)->getLexique() != "6CISP1"))
                        itPpt++;
                }
                delete pNSDocumentHisto ;
            }
        }

        pContexte->getSuperviseur()->getApplication()->PumpWaitingMessages() ;

        pData->lastError = pData->suivant(dbiWRITELOCK) ;
        if (pData->lastError != DBIERR_NONE)
        {
            if (pData->lastError != DBIERR_EOF)
                erreur("Erreur au positionnement au début de NsPatDat.db.", standardError, pData->lastError) ;
            bTourner = false ;
        }
    }

    outFile.close() ;

    pData->close();
    delete pData ;
*/
}

void
NSCodeListDlg::stoppe()
{
    bTourner = false ;
}


//****************************************************************************
						/*  CLASSE NSPrintParamsDlg  */
//****************************************************************************

DEFINE_RESPONSE_TABLE1(NSPrintParamsDlg, TDialog)
  EV_COMMAND(IDOK, CmOk),
  EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE ;

NSPrintParamsDlg::NSPrintParamsDlg(TWindow* pere, NSContexte* pCtx)
                 :NSUtilDialog(pere, pCtx, "IDD_PRINT_PARAMS_DLG", pNSResModule)
{
	pTempo		= new TEdit(this, IDC_TEMPO ) ;
	pTempo->SetValidator(new TFilterValidator("0-9")) ;

	pHookOui 	= new TRadioButton(this, IDC_HOOKOUI ) ;
	pHookNon 	= new TRadioButton(this, IDC_HOOKNON ) ;
	pVerboseOui = new TRadioButton(this, IDC_VERBOSEOUI ) ;
	pVerboseNon	= new TRadioButton(this, IDC_VERBOSENON ) ;
	pKillTmpNon = new TRadioButton(this, IDC_KILLTMPNON ) ;
	pKillTmpOui = new TRadioButton(this, IDC_KILLTMPOUI ) ;
	pWaitImpOui	= new TRadioButton(this, IDC_WAITIMPOUI ) ;
	pWaitImpNon = new TRadioButton(this, IDC_WAITIMPNON ) ;

	pTempImp	= new TEdit(this, IDC_TEMPIMP ) ;
	pTempImp->SetValidator(new TFilterValidator("0-9")) ;

	pSimple_Oui	= new TRadioButton(this, IDC_SIMPLEOUI ) ;
	pSimple_Non = new TRadioButton(this, IDC_SIMPLENON ) ;

	pTimePostNav = new TEdit(this, IDC_TEMPOSTNAV ) ;
	pTimePostNav->SetValidator(new TFilterValidator("0-9")) ;
}

NSPrintParamsDlg::~NSPrintParamsDlg()
{
	delete pTempo ;
	delete pHookOui ;
	delete pHookNon ;
	delete pVerboseOui ;
	delete pVerboseNon ;
	delete pKillTmpNon ;
	delete pKillTmpOui ;
	delete pWaitImpOui ;
	delete pWaitImpNon ;
	delete pTempImp ;
	delete pSimple_Oui ;
	delete pSimple_Non ;
	delete pTimePostNav ;
}

voidNSPrintParamsDlg::CmCancel()
{
	Destroy(IDCANCEL) ;
}

void
NSPrintParamsDlg::SetupWindow()
{
	NSUtilDialog::SetupWindow() ;

	// Default params
	//
	pHookOui->Uncheck() ;
	pHookNon->Check() ;
	pVerboseOui->Check() ;
	pVerboseNon->Uncheck() ;
	pKillTmpNon->Uncheck() ;
	pKillTmpOui->Check() ;
	pWaitImpOui->Uncheck() ;
	pWaitImpNon->Check() ;
	pSimple_Oui->Uncheck() ;
	pSimple_Non->Check() ;

	// Parsing du fichier
	//
	ifstream inFile;
	string  line;
	string  sData = "";
	string  sNomAttrib = "";
	string  sValAttrib = "";
	size_t  i = 0, j = 0;

	string sFichierTempo = pContexte->PathName("FPER") + string("tempo.dat") ;
	inFile.open(sFichierTempo.c_str());	if (!inFile)
	{
		erreur("Erreur d'ouverture du fichier tempo.dat.", standardError, 0, GetHandle()) ;
    return ;
	}

	while (!inFile.eof())	{
		getline(inFile,line) ;
    if (line != "")
    	sData += line + "\n" ;
	}

  inFile.close() ;
  // boucle de chargement des attributs de tempo.dat  while (i < strlen(sData.c_str()))
  {
    sNomAttrib = string("") ;

    while ((i < strlen(sData.c_str())) && (' ' != sData[i]) && ('\t' != sData[i]))      sNomAttrib += pseumaj(sData[i++]) ;

    while ((i < strlen(sData.c_str())) && ((' ' == sData[i]) || ('\t' == sData[i])))      i++ ;

    sValAttrib = string("") ;
    while ((i < strlen(sData.c_str())) && ('\n' != sData[i]))      sValAttrib += sData[i++] ;

    if      (sNomAttrib == "TEMPO")      pTempo->SetText(sValAttrib.c_str()) ;
    else if (sNomAttrib == "HOOK")
    {
      if (IsYes(sValAttrib))      {        pHookOui->Check() ;
        pHookNon->Uncheck() ;
      }
    }
    else if (sNomAttrib == "VERBOSE")
    {
      if (IsNo(sValAttrib))
      {        pVerboseOui->Uncheck() ;
        pVerboseNon->Check() ;
      }
    }
    else if (sNomAttrib == "KILLTMP")    {
      if (IsNo(sValAttrib))
      {        pKillTmpNon->Check() ;
				pKillTmpOui->Uncheck() ;
      }
    }
    else if (sNomAttrib == "WAITIMP")
    {
      if (IsYes(sValAttrib))
      {        pWaitImpOui->Check() ;
				pWaitImpNon->Uncheck() ;
      }
    }
    else if (sNomAttrib == "SIMPLECTRL")
    {
      if (IsYes(sValAttrib))
      {        pSimple_Oui->Check() ;
				pSimple_Non->Uncheck() ;
      }
    }
    else if (sNomAttrib == "TEMPOSTNAV")
    {
      pTimePostNav->SetText(sValAttrib.c_str()) ;
    }
    else if (sNomAttrib == "TEMPIMP")
    {
      pTempImp->SetText(sValAttrib.c_str()) ;
    }

    i++ ;  }
}

void
NSPrintParamsDlg::CmOk()
{
	string sFichierTempo = pContexte->PathName("FPER") + string("tempo.dat") ;

  ofstream outFile ;
  string sLine ;

  outFile.open(sFichierTempo.c_str(), ios::out);
  if (!outFile)
  {
    string sErreur = "Erreur d'ouverture du fichier " + string(sFichierTempo);
    erreur(sErreur.c_str(), standardError, 0) ;
    return ;
  }

  char szText[33] ;
  pTempo->GetText(szText, 32) ;
  if (szText[0] != '\0')
  {
    size_t iLen = strlen(szText) ;
    size_t i = 0 ;
    for (; (i < iLen) && (isdigit(szText[i])); i++) ;
      if (i >= iLen)
        outFile << (string("TEMPO      ")  + string(szText) + string("\n")) ;
  }

  if (pHookOui->GetCheck() == BF_CHECKED)
    outFile << (string("HOOK       ")  + string("Oui") + string("\n")) ;
  else
    outFile << (string("HOOK       ")  + string("Non") + string("\n")) ;

  if (pVerboseOui->GetCheck() == BF_CHECKED)
    outFile << (string("VERBOSE    ")  + string("Oui") + string("\n")) ;
  else
    outFile << (string("VERBOSE    ")  + string("Non") + string("\n")) ;

  if (pKillTmpOui->GetCheck() == BF_CHECKED)
    outFile << (string("KILLTMP    ")  + string("Oui") + string("\n")) ;
  else
    outFile << (string("KILLTMP    ")  + string("Non") + string("\n")) ;

  if (pWaitImpOui->GetCheck() == BF_CHECKED)
    outFile << (string("WAITIMP    ")  + string("Oui") + string("\n")) ;
  else
    outFile << (string("WAITIMP    ")  + string("Non") + string("\n")) ;

  pTempImp->GetText(szText, 32) ;
  if (szText[0] != '\0')
  {
    size_t iLen = strlen(szText) ;
    size_t i = 0 ;
    for (; (i < iLen) && (isdigit(szText[i])); i++) ;
      if (i >= iLen)
        outFile << (string("TEMPIMP    ")  + string(szText) + string("\n")) ;
  }

  if (pSimple_Oui->GetCheck() == BF_CHECKED)
    outFile << (string("SIMPLECTRL ")  + string("Oui") + string("\n")) ;
  else
    outFile << (string("SIMPLECTRL ")  + string("Non") + string("\n")) ;

  pTimePostNav->GetText(szText, 32) ;
  if (szText[0] != '\0')
  {
    size_t iLen = strlen(szText) ;
    size_t i = 0 ;
    for (; (i < iLen) && (isdigit(szText[i])); i++) ;
      if (i >= iLen)
        outFile << (string("TEMPOSTNAV ")  + string(szText) + string("\n")) ;
  }

  outFile.close() ;

  NSUtilDialog::CmOk() ;
}

//
// ------------------------------- NSUpdateFileManager ----------------------
//
NSUpdateFileManager::NSUpdateFileManager(NSContexte* pCtx)
                    :NSRoot(pCtx)
{
	_sSettingsFile = string("update.dat") ;

	_sURL        = string("") ;
	_sLogin      = string("") ;
	_sPassword   = string("") ;
	_sProtocol   = string("") ;
	_sUpdateDir  = string("") ;

  _bLastUpdate = false ;
	_tLastUpdate.init() ;

  _iInterval   = 0 ;
	_bLastUpdateCheck = true ;
	_bInterval        = true ;
  _tLastUpdateCheck.init() ;
	_tNextUpdateCheck.init() ;

  _bSilentMode      = false ;
}

NSUpdateFileManager::~NSUpdateFileManager()
{
}

bool
NSUpdateFileManager::readFromFile()
{
	ifstream inFile ;
	// on ouvre le fichier de configuration
  inFile.open(_sSettingsFile.c_str()) ;
  if (!inFile)
  {
  	string sErrorMsg = pContexte->getSuperviseur()->getText("fileErrors", "errorOpeningInputFile") ;
    sErrorMsg += string(" ") + _sSettingsFile ;
    erreur(sErrorMsg.c_str(), warningError, 0) ;
    return false ;
  }

	string sLine ;
  string sData = string("") ;
  // on lit les paramètres de backup
	while (!inFile.eof())
	{
		getline(inFile, sLine) ;
    sData += sLine + "\n" ;
	}

  inFile.close() ;

  size_t i = 0 ;

  string sNomAttrib ;
  string sValAttrib ;

	while (i < strlen(sData.c_str()))
	{
		sNomAttrib = string("") ;
		sValAttrib = string("") ;

		while ((i < strlen(sData.c_str())) && (sData[i] != ' ') && (sData[i] != '\t'))
			sNomAttrib += pseumaj(sData[i++]) ;
		while ((i < strlen(sData.c_str())) && ((sData[i] == ' ') || (sData[i] == '\t')))
			i++ ;

		while ((i < strlen(sData.c_str())) && (sData[i] != '\n'))
			sValAttrib += sData[i++] ;

		i++ ;

		if 		((sNomAttrib == "URL") && (sValAttrib != ""))
      _sURL = sValAttrib ;
    else if ((sNomAttrib == "LOGIN") && (sValAttrib != ""))
      _sLogin = sValAttrib ;
    else if ((sNomAttrib == "PASSWORD") && (sValAttrib != ""))
      _sPassword = sValAttrib ;
    else if ((sNomAttrib == "PROTOCOL") && (sValAttrib != ""))
		{
			for (size_t j = 0 ; j < strlen(sValAttrib.c_str()) ; j++)
				sValAttrib[j] = pseumaj(sValAttrib[j]) ;
      _sProtocol = sValAttrib ;
    }
    else if ((sNomAttrib == "UPDATE_DIR") && (sValAttrib != ""))
      _sUpdateDir = sValAttrib ;
		else if ((sNomAttrib == "LAST_UPDATE") && (sValAttrib != ""))
		{
			for (size_t j = 0 ; j < strlen(sValAttrib.c_str()) ; j++)
				sValAttrib[j] = pseumaj(sValAttrib[j]) ;

			if ((sValAttrib == string("NEVER")) || (sValAttrib == string("JAMAIS")))
				_bLastUpdate = false ;
      else
        _tLastUpdate.initFromDateHeure(sValAttrib) ;
		}
    else if ((sNomAttrib == "LAST_CHECK") && (sValAttrib != ""))
		{
			for (size_t j = 0 ; j < strlen(sValAttrib.c_str()) ; j++)
				sValAttrib[j] = pseumaj(sValAttrib[j]) ;

			if ((sValAttrib == string("NEVER")) || (sValAttrib == string("JAMAIS")))
				_bLastUpdateCheck = false ;
      else
        _tLastUpdateCheck.initFromDateHeure(sValAttrib) ;
		}
		else if ((sNomAttrib == "INTERVAL_DAYS") && (sValAttrib != ""))
		{
			for (size_t j = 0 ; j < strlen(sValAttrib.c_str()) ; j++)
				sValAttrib[j] = pseumaj(sValAttrib[j]) ;

			if ((sValAttrib == string("NEVER")) || (sValAttrib == string("JAMAIS")))
				_bInterval = false ;
      else
      {
      	_iInterval = atoi(sValAttrib.c_str()) ;
      	if (true == _bLastUpdate)
        {
        	_tNextUpdateCheck = _tLastUpdate ;
        	_tNextUpdateCheck.ajouteJours(_iInterval) ;
        }
      }
		}
    else if ((sNomAttrib == "SILENT") && (string("") != sValAttrib))
		{
			if      (IsYes(sValAttrib))
				_bSilentMode = true ;
      else if (IsNo(sValAttrib))
        _bSilentMode = false ;
		}
	}
  return true ;
}

bool
NSUpdateFileManager::writeToFile()
{
	ofstream  outFile ;
	outFile.open(_sSettingsFile.c_str(), ios::out) ;
	if (!outFile)
		return false ;

	NVLdVTemps  tNow ;
	tNow.takeTime() ;

  string sLastUpdateDate ;
	if (true == _tLastUpdate.estVide())
		sLastUpdateDate = string("never") ;
	else
  	sLastUpdateDate = _tLastUpdate.donneDateHeure() ;

	char outLine[2048] ;
  sprintf(outLine, "URL %s\nLOGIN %s\nPASSWORD %s\nPROTOCOL %s\nUPDATE_DIR %s\nLAST_UPDATE %s\nLAST_CHECK %s\n", _sURL.c_str(), _sLogin.c_str(), _sPassword.c_str(), _sProtocol.c_str(), _sUpdateDir.c_str(), sLastUpdateDate.c_str(), tNow.donneDateHeure().c_str()) ;
  outFile.write(outLine, strlen(outLine)) ;

  if (true == _bInterval)
		sprintf(outLine, "INTERVAL_DAYS %d\n", _iInterval) ;
  else
  	strcpy(outLine, "INTERVAL_DAYS never") ;
	outFile.write(outLine, strlen(outLine)) ;

  if (true == _bSilentMode)
		sprintf(outLine, "SILENT yes\n") ;
  else
  	strcpy(outLine, "SILENT no\n") ;
	outFile.write(outLine, strlen(outLine)) ;

	outFile.close() ;

	return true ;
}

//
// ------------------------------- NSUpdateParamsDlg ----------------------
//

DEFINE_RESPONSE_TABLE1(NSUpdateParamsDlg, TDialog)
  EV_COMMAND(IDOK, CmOk),
  EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE ;

NSUpdateParamsDlg::NSUpdateParamsDlg(TWindow* pere, NSContexte* pCtx)
                  :NSUtilDialog(pere, pCtx, "IDD_UPDATE_PARAMS_DLG", pNSResModule),
                   _fileManager(pCtx)
{
	_pIDsGroup        = new TGroupBox(this, IDC_TXT_IDS) ;
	_pIDTxt           = new TStatic(this, IDC_TXT_ID) ;
	_pLogin           = new TEdit(this, IDC_ED_ID) ;
	_pPassTxt         = new TStatic(this, IDC_TXT_PASS) ;
	_pPassword        = new TEdit(this, IDC_ED_PASS) ;
	_pLocationGroup   = new TGroupBox(this, IDC_TXT_LOCATION) ;
  _pUrlTxt          = new TStatic(this, IDC_TXT_FROM) ;
	_pURL             = new TEdit(this, IDC_ED_FROM) ;
	_pUpdateDirTxt    = new TStatic(this, IDC_TXT_TO) ;
	_pUpdateDir       = new TEdit(this, IDC_ED_TO) ;
	_pAutoUpdateGroup = new TGroupBox(this, IDC_TXT_AUTO) ;
	_pUpdateYes       = new TRadioButton(this, IDC_UPDT_YES, _pAutoUpdateGroup) ;
	_pUpdateNo        = new TRadioButton(this, IDC_UPDT_NO,  _pAutoUpdateGroup) ;
	_pEveryTxt        = new TStatic(this, IDC_TXT_EVERY) ;
	_pInterval        = new TEdit(this, IDC_ED_EVERY) ;
  _pInterval->SetValidator(new TFilterValidator("0-9")) ;
	_pDaysTxt         = new TStatic(this, IDC_TXT_DAYS) ;
  _pSilentModeGroup = new TGroupBox(this, IDC_TXT_SILENT) ;
	_pSilentYes       = new TRadioButton(this, IDC_SILENT_YES, _pSilentModeGroup) ;
	_pSilentNo        = new TRadioButton(this, IDC_SILENT_NO,  _pSilentModeGroup) ;
	_pLastUpdate = 0 ;

  _fileManager.readFromFile() ;
}

NSUpdateParamsDlg::~NSUpdateParamsDlg()
{
	delete _pIDsGroup ;
	delete _pIDTxt ;
	delete _pLogin ;
	delete _pPassTxt ;
	delete _pPassword ;
	delete _pLocationGroup ;
  delete _pUrlTxt ;
	delete _pURL ;
	delete _pUpdateDirTxt ;
	delete _pUpdateDir ;
	delete _pAutoUpdateGroup ;
	delete _pUpdateYes ;
	delete _pUpdateNo ;
	delete _pEveryTxt ;
	delete _pInterval ;
	delete _pDaysTxt ;
	delete _pSilentModeGroup ;
	delete _pSilentYes ;
	delete _pSilentNo ;
}

void
NSUpdateParamsDlg::SetupWindow()
{
	NSUtilDialog::SetupWindow() ;

	// Set texts
  //
  string sTxt = pContexte->getSuperviseur()->getText("updateParametersManagement", "settingUpdateParameters") ;
  SetCaption(sTxt.c_str()) ;

	sTxt = pContexte->getSuperviseur()->getText("updateParametersManagement", "identifiersManagement") ;
  _pIDsGroup->SetCaption(sTxt.c_str()) ;
	sTxt = pContexte->getSuperviseur()->getText("updateParametersManagement", "login") ;
  _pIDTxt->SetText(sTxt.c_str()) ;
	sTxt = pContexte->getSuperviseur()->getText("updateParametersManagement", "password") ;
  _pPassTxt->SetText(sTxt.c_str()) ;
	sTxt = pContexte->getSuperviseur()->getText("updateParametersManagement", "locationManagement") ;
  _pLocationGroup->SetCaption(sTxt.c_str()) ;
	sTxt = pContexte->getSuperviseur()->getText("updateParametersManagement", "from") ;
  _pUrlTxt->SetText(sTxt.c_str()) ;
	sTxt = pContexte->getSuperviseur()->getText("updateParametersManagement", "to") ;
  _pUpdateDirTxt->SetText(sTxt.c_str()) ;
	sTxt = pContexte->getSuperviseur()->getText("updateParametersManagement", "automaticUpdate") ;
  _pAutoUpdateGroup->SetCaption(sTxt.c_str()) ;
	sTxt = pContexte->getSuperviseur()->getText("updateParametersManagement", "activate") ;
  _pUpdateYes->SetCaption(sTxt.c_str()) ;
	sTxt = pContexte->getSuperviseur()->getText("updateParametersManagement", "doNotActivate") ;
  _pUpdateNo->SetCaption(sTxt.c_str()) ;
	sTxt = pContexte->getSuperviseur()->getText("updateParametersManagement", "every") ;
  _pEveryTxt->SetText(sTxt.c_str()) ;
	sTxt = pContexte->getSuperviseur()->getText("updateParametersManagement", "days") ;
  _pDaysTxt->SetText(sTxt.c_str()) ;
  sTxt = pContexte->getSuperviseur()->getText("updateParametersManagement", "silentUpdate") ;
  _pSilentModeGroup->SetText(sTxt.c_str()) ;
  sTxt = pContexte->getSuperviseur()->getText("generalLanguageforDialog", "yes") ;
  _pSilentYes->SetCaption(sTxt.c_str()) ;
  sTxt = pContexte->getSuperviseur()->getText("generalLanguageforDialog", "no") ;
  _pSilentNo->SetCaption(sTxt.c_str()) ;

  // Init Edits
  //
  _pLogin->SetText(_fileManager.getLogin().c_str()) ;
  _pPassword->SetText(_fileManager.getPassword().c_str()) ;
  _pURL->SetText(_fileManager.getURL().c_str()) ;
  _pUpdateDir->SetText(_fileManager.getUpdateDir().c_str()) ;

  if (true == _fileManager.getIsInterval())
  {
  	_pUpdateYes->Check() ;
    _pUpdateNo->Uncheck() ;
  }
  else
  {
  	_pUpdateYes->Uncheck() ;
    _pUpdateNo->Check() ;
  }

  if (true == _fileManager.getIsSilentMode())
  {
  	_pSilentYes->Check() ;
    _pSilentNo->Uncheck() ;
  }
  else
  {
  	_pSilentYes->Uncheck() ;
    _pSilentNo->Check() ;
  }

	char outLine[30] ;
  sprintf(outLine, "%d", _fileManager.getInterval()) ;
  _pInterval->SetText(outLine) ;
}

void
NSUpdateParamsDlg::CmCancel()
{
	Destroy(IDCANCEL) ;
}

void
NSUpdateParamsDlg::CmOk()
{
	int iBuffLen = _pLogin->GetTextLen() ;
	char far* szBuff = new char[iBuffLen+1] ;
	_pLogin->GetText(szBuff, iBuffLen+1) ;
  _fileManager.setLogin(string(szBuff)) ;
  delete[] szBuff ;

  iBuffLen = _pPassword->GetTextLen() ;
	szBuff = new char[iBuffLen+1] ;
	_pPassword->GetText(szBuff, iBuffLen+1) ;
  _fileManager.setPassword(string(szBuff)) ;
  delete[] szBuff ;

  iBuffLen = _pURL->GetTextLen() ;
	szBuff = new char[iBuffLen+1] ;
	_pURL->GetText(szBuff, iBuffLen+1) ;
  _fileManager.setURL(string(szBuff)) ;
  delete[] szBuff ;

  iBuffLen = _pUpdateDir->GetTextLen() ;
	szBuff = new char[iBuffLen+1] ;
	_pUpdateDir->GetText(szBuff, iBuffLen+1) ;
  _fileManager.setUpdateDir(string(szBuff)) ;
  delete[] szBuff ;

	if (BF_CHECKED == _pUpdateYes->GetCheck())
  {
  	_fileManager.setIsInterval(true) ;

    iBuffLen = _pInterval->GetTextLen() ;
		szBuff = new char[iBuffLen+1] ;
		_pInterval->GetText(szBuff, iBuffLen+1) ;
  	int iInterval = atoi(szBuff) ;
  	delete[] szBuff ;
    _fileManager.setInterval(iInterval) ;
  }
  else
  	_fileManager.setIsInterval(false) ;

  if (BF_CHECKED == _pSilentYes->GetCheck())
    _fileManager.setSilentMode(true) ;
  if (BF_CHECKED == _pSilentNo->GetCheck())
    _fileManager.setSilentMode(false) ;

	bool bWriteOk = _fileManager.writeToFile() ;

	if (true == bWriteOk)
  	NSUtilDialog::CmOk() ;
}

//
// ------------------------------- NSInitFileManager ----------------------
//
NSInitFileManager::NSInitFileManager(NSContexte* pCtx)
                  :NSRoot(pCtx)
{
	_sSettingsFile = string("chemins.dat") ;

	_sConsole            = string("") ;
	_sIdFileName         = string("") ;
	_sPathBase           = string("") ;
	_sSlaveDllName       = string("") ;

  _bModeClientGroup    = false ;
  _bGroupInterfaceOnly = false ;
	_bSlaveMode          = false ;

  _iTraceLevel         = NSSuper::trNone ;
  _iConsoleTraceLevel  = NSSuper::trNone ;
}

NSInitFileManager::~NSInitFileManager()
{
}

char
NSInitFileManager::getTraceString(NSSuper::TRACETYPE iTraceLevel)
{
  switch(iTraceLevel)
  {
    case NSSuper::trError      : return '0' ;
    case NSSuper::trWarning    : return '1' ;
    case NSSuper::trSteps      : return '2' ;
    case NSSuper::trSubSteps   : return '3' ;
    case NSSuper::trDetails    : return '4' ;
    case NSSuper::trSubDetails : return '5' ;
  }

  return '1' ;
}

/*
NSSuper::TRACETYPE
NSInitFileManager::getTraceLevel(string sTraceString)
{
  char szTraceLevel[2] ;
  szTraceLevel[0] = cTraceString ;
  szTraceLevel[1] = '\0' ;

  int iLevel = atoi(szTraceLevel) ;

  switch(iLevel)
  {
    case 0  : return NSSuper::trError ;
    case 1  : return NSSuper::trWarning ;
    case 2  : return NSSuper::trSteps ;
    case 3  : return NSSuper::trSubSteps ;
    case 4  : return NSSuper::trDetails ;
    case 5  : return NSSuper::trSubDetails ;
  }

  if (iLevel > 5)
    return NSSuper::trSubDetails ;

  return NSSuper::trNone ;
}
*/

NSSuper::TRACETYPE
NSInitFileManager::getTraceLevelFromString(string sTraceLevel)
{
  strip(sTraceLevel, stripBoth) ;

  if (string("") == sTraceLevel)
    return NSSuper::trNone ;

  int iLevel = atoi(sTraceLevel.c_str()) ;
  switch(iLevel)
  {
    case 0  : return NSSuper::trError ;
    case 1  : return NSSuper::trWarning ;
    case 2  : return NSSuper::trSteps ;
    case 3  : return NSSuper::trSubSteps ;
    case 4  : return NSSuper::trDetails ;
    case 5  : return NSSuper::trSubDetails ;
  }

  if (iLevel > 5)
    return NSSuper::trSubDetails ;

  return NSSuper::trNone ;
}

bool
NSInitFileManager::readFromFile()
{
	ifstream inFile ;
	// on ouvre le fichier de configuration
  inFile.open(_sSettingsFile.c_str()) ;
  if (!inFile)
  {
  	string sErrorMsg = pContexte->getSuperviseur()->getText("fileErrors", "errorOpeningInputFile") ;
    sErrorMsg += string(" ") + _sSettingsFile ;
    pContexte->getSuperviseur()->trace(&sErrorMsg, 1, NSSuper::trError) ;
    erreur(sErrorMsg.c_str(), warningError, 0) ;
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

	// First line: Console ID
  //
  string sConsoleLine = string("") ;
	for (i = 0; (i < strlen(sFichierDat.c_str())) && ('\n' != sFichierDat[i]); i++)
		sConsoleLine += sFichierDat[i] ;

	if (strlen(sConsoleLine.c_str()) == 2)
  	_sConsole = sConsoleLine ;

  // Incremental console id with centralized file
  //
  else if ((string(sConsoleLine, 0, 2) == string("??")) && (strlen(sConsoleLine.c_str()) > 3))
  {
    _sConsole    = string(sConsoleLine, 0, 2) ;
    _sIdFileName = string(sConsoleLine, 3, strlen(sConsoleLine.c_str()) - 3) ;
  }

	// Second line: path to path files
  //
	for (i = i+1; (i < strlen(sFichierDat.c_str())) && ('\n' != sFichierDat[i]); i++)
  	_sPathBase += sFichierDat[i] ;

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
          _iTraceLevel = getTraceLevelFromString(sTraceLevel) ;
        }
      }
      // Console level
      //
      else if (('C' == sParamerString[0]) || ('c' == sParamerString[0]))
	    {
        if ((iParamsLength > 1) && (isdigit(sParamerString[iParamsLength-1])))
        {
          string sTraceLevel = string(1, sParamerString[iParamsLength-1]) ;
          _iConsoleTraceLevel = getTraceLevelFromString(sTraceLevel) ;
        }
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
	    }
    }
  }

  return true ;
}

bool
NSInitFileManager::writeToFile()
{
	ofstream outFile ;
	outFile.open(_sSettingsFile.c_str(), ios::out) ;
	if (!outFile)
	{
  	string sErrorMsg = pContexte->getSuperviseur()->getText("fileErrors", "errorOpeningOuputFile") ;
    sErrorMsg += string(" ") + _sSettingsFile ;
    pContexte->getSuperviseur()->trace(&sErrorMsg, 1, NSSuper::trError) ;
    erreur(sErrorMsg.c_str(), warningError, 0) ;
    return false ;
  }

  // Console ID
  //
  outFile << _sConsole ;
  if (string("") != _sIdFileName)
    outFile << string(" ") << _sIdFileName ;
  outFile << string("\n") ;

  // Path
  //
  outFile << _sPathBase << string("\n") ;

  // Trace
  //
  if (NSSuper::trNone != _iTraceLevel)
  {
    char cTrace = getTraceString(_iTraceLevel) ;
    outFile << string(1, 'T') << string(1, cTrace) << string("\n") ;
  }

  // Blackboard trace
  //
  if (NSSuper::trNone != _iTraceBBK)
  {
    char cTrace = getTraceString(_iTraceBBK) ;
    outFile << string(1, 'B') << string(1, cTrace) << string("\n") ;
  }

  // Console trace
  //
  if (NSSuper::trNone != _iConsoleTraceLevel)
  {
    char cTrace = getTraceString(_iConsoleTraceLevel) ;
    outFile << string(1, 'C') << string(1, cTrace) << string("\n") ;
  }

  // Client group
  //
  if (_bModeClientGroup)
  {
    if (_bGroupInterfaceOnly)
      outFile << string(1, 'I') << string("\n") ;
    else
      outFile << string(1, 'G') << string("\n") ;
  }

  // Slave mode
  //
  if (_bSlaveMode)
  {
    outFile << string(1, 'S') ;
    if (string("") != _sSlaveDllName)
      outFile << string(" ") << _sSlaveDllName ;
    outFile << string("\n") ;
  }

	outFile.close() ;

	return true ;
}

//
// ------------------------------- NSInitParamsDlg ----------------------
//

DEFINE_RESPONSE_TABLE1(NSInitParamsDlg, TDialog)
  EV_COMMAND(IDOK, CmOk),
  EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE ;

NSInitParamsDlg::NSInitParamsDlg(TWindow* pere, NSContexte* pCtx)
                :NSUtilDialog(pere, pCtx, "IDD_INIT_PARAMS_DLG", pNSResModule),
                   _fileManager(pCtx)
{
	_pConsoleIDGroup  = new TGroupBox(this, IDC_TXT_IDCG) ;
	_pConsoleIDTxt    = new TStatic(this, IDC_TXT_IDC) ;
	_pConsoleID       = new TEdit(this, IDC_ED_IDC) ;
	_pConsoleIDTxt2   = new TStatic(this, IDC_TXT_IDC2) ;
	_pPathGroup       = new TGroupBox(this, IDC_TXT_PATH) ;
	_pPath            = new TEdit(this, IDC_ED_PATH) ;
	_pTraceLevelGroup = new TGroupBox(this, IDC_TXT_TRACES) ;
	_pTraceLevel1     = new TRadioButton(this, IDC_TRACE_1, _pTraceLevelGroup) ;
  _pTraceLevel2     = new TRadioButton(this, IDC_TRACE_2, _pTraceLevelGroup) ;
  _pTraceLevel3     = new TRadioButton(this, IDC_TRACE_3, _pTraceLevelGroup) ;
  _pTraceLevel4     = new TRadioButton(this, IDC_TRACE_4, _pTraceLevelGroup) ;
  _pTraceLevel5     = new TRadioButton(this, IDC_TRACE_5, _pTraceLevelGroup) ;
  _pTraceLevel6     = new TRadioButton(this, IDC_TRACE_6, _pTraceLevelGroup) ;

  _fileManager.readFromFile() ;
}

NSInitParamsDlg::~NSInitParamsDlg()
{
	delete _pConsoleIDGroup ;
	delete _pConsoleIDTxt ;
	delete _pConsoleID ;
	delete _pConsoleIDTxt2 ;
	delete _pPathGroup ;
	delete _pPath ;
	delete _pTraceLevelGroup ;
	delete _pTraceLevel1 ;
  delete _pTraceLevel2 ;
  delete _pTraceLevel3 ;
  delete _pTraceLevel4 ;
  delete _pTraceLevel5 ;
  delete _pTraceLevel6 ;
}

void
NSInitParamsDlg::SetupWindow()
{
	NSUtilDialog::SetupWindow() ;

	// Set texts
  //
  string sTxt = pContexte->getSuperviseur()->getText("initParametersManagement", "settingInitParameters") ;
  SetCaption(sTxt.c_str()) ;

	sTxt = pContexte->getSuperviseur()->getText("initParametersManagement", "stationUniqueIdentifie") ;
  _pConsoleIDTxt->SetCaption(sTxt.c_str()) ;
	sTxt = pContexte->getSuperviseur()->getText("initParametersManagement", "twoChars") ;
  _pConsoleIDTxt2->SetText(sTxt.c_str()) ;
	sTxt = pContexte->getSuperviseur()->getText("initParametersManagement", "path") ;
  _pPathGroup->SetText(sTxt.c_str()) ;
	sTxt = pContexte->getSuperviseur()->getText("initParametersManagement", "traceLevel") ;
  _pTraceLevelGroup->SetCaption(sTxt.c_str()) ;
	sTxt = pContexte->getSuperviseur()->getText("initParametersManagement", "errorsOnly") ;
  _pTraceLevel1->SetCaption(sTxt.c_str()) ;
  sTxt = pContexte->getSuperviseur()->getText("initParametersManagement", "errorsAndWarnings") ;
  _pTraceLevel2->SetCaption(sTxt.c_str()) ;
  sTxt = pContexte->getSuperviseur()->getText("initParametersManagement", "mainSteps") ;
  _pTraceLevel3->SetCaption(sTxt.c_str()) ;
  sTxt = pContexte->getSuperviseur()->getText("initParametersManagement", "allSteps") ;
  _pTraceLevel4->SetCaption(sTxt.c_str()) ;
  sTxt = pContexte->getSuperviseur()->getText("initParametersManagement", "mainDetails") ;
  _pTraceLevel5->SetCaption(sTxt.c_str()) ;
  sTxt = pContexte->getSuperviseur()->getText("initParametersManagement", "allDetails") ;
  _pTraceLevel6->SetCaption(sTxt.c_str()) ;

  // Init Edits
  //
  _pConsoleID->SetText(_fileManager.getConsole().c_str()) ;
  _pPath->SetText(_fileManager.getPathBase().c_str()) ;

  // Init traces
  //
  SetTraces(_fileManager.getTraceLevel()) ;
}

void
NSInitParamsDlg::CmCancel()
{
	Destroy(IDCANCEL) ;
}

void
NSInitParamsDlg::CmOk()
{
	int iBuffLen = _pConsoleID->GetTextLen() ;
	char far* szBuff = new char[iBuffLen+1] ;
	_pConsoleID->GetText(szBuff, iBuffLen+1) ;
  _fileManager.setConsole(string(szBuff)) ;
  delete[] szBuff ;

  iBuffLen = _pPath->GetTextLen() ;
	szBuff = new char[iBuffLen+1] ;
	_pPath->GetText(szBuff, iBuffLen+1) ;
  _fileManager.setPathBase(string(szBuff)) ;
  delete[] szBuff ;

  NSSuper::TRACETYPE iTraceLevel = getTraceLevel() ;
  _fileManager.setTraceLevel(iTraceLevel) ;

  // Change is applied immediately for trace level
  //
  pContexte->getSuperviseur()->setTraceLevel(iTraceLevel) ;

	bool bWriteOk = _fileManager.writeToFile() ;

	if (true == bWriteOk)
  	NSUtilDialog::CmOk() ;
}

void
NSInitParamsDlg::SetTraces(NSSuper::TRACETYPE iTraceLevel)
{
  ResetTraces() ;

  switch(iTraceLevel)
  {
    case NSSuper::trError      : _pTraceLevel1->Check() ; break ;
    case NSSuper::trWarning    : _pTraceLevel2->Check() ; break ;
    case NSSuper::trSteps      : _pTraceLevel3->Check() ; break ;
    case NSSuper::trSubSteps   : _pTraceLevel4->Check() ; break ;
    case NSSuper::trDetails    : _pTraceLevel5->Check() ; break ;
    case NSSuper::trSubDetails : _pTraceLevel6->Check() ; break ;
  }
}

void
NSInitParamsDlg::ResetTraces()
{
  _pTraceLevel1->Uncheck() ;
  _pTraceLevel2->Uncheck() ;
  _pTraceLevel3->Uncheck() ;
  _pTraceLevel4->Uncheck() ;
  _pTraceLevel5->Uncheck() ;
  _pTraceLevel6->Uncheck() ;
}

NSSuper::TRACETYPE
NSInitParamsDlg::getTraceLevel()
{
  if (BF_CHECKED == _pTraceLevel1->GetCheck())
    return NSSuper::trError ;
  if (BF_CHECKED == _pTraceLevel2->GetCheck())
    return NSSuper::trWarning ;
  if (BF_CHECKED == _pTraceLevel3->GetCheck())
    return NSSuper::trSteps ;
  if (BF_CHECKED == _pTraceLevel4->GetCheck())
    return NSSuper::trSubSteps ;
  if (BF_CHECKED == _pTraceLevel5->GetCheck())
    return NSSuper::trDetails ;
  if (BF_CHECKED == _pTraceLevel6->GetCheck())
    return NSSuper::trSubDetails ;

  return NSSuper::trNone ;
}

