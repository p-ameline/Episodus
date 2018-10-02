#include <owl\applicat.h>#include <owl\olemdifr.h>
#include <owl\mdichild.h>
#include <owl\edit.h>
#include <owl\combobox.h>
#include <owl\button.h>
#include <owl\static.h>

#include <bde.hpp>
#include <stdio.h>
#include <cstring.h>
#include <winsys/registry.h> //base de registre

#include "nautilus\nssuper.h"
#include "partage\nsdivfct.h"
#include "nautilus\nsresour.h"
#include "nautilus\nautilus.rh"
#include "nautilus\nsadmiwd.h"
// #include "nautilus\nsanxary.h"
#include "nautilus\nsperary.h"
#include "nautilus\nspatdlg.h"
#include "nsdn\nsdocdlg.h"
#include "nscompta\nscomdlg.h"
#include "nscompta\nsfsedlg.h"
#include "nscompta\nsdepens.h"
#include "nscompta\nsf16dlg.h"

// *****************************************************************************************
//									Méthodes de NSComptaPatient
// *****************************************************************************************

NSComptaPatient::NSComptaPatient(NSContexte* pCtx, NSPersonInfo* pPat)
                :NSRoot(pCtx)
{
	if (pPat)
		_pPatient = new NSPersonInfo(*pPat) ;
	else
		_pPatient = NULL ;
}

NSComptaPatient::~NSComptaPatient()
{
	if (_pPatient)
		delete _pPatient ;
}

//------------------------------------------------------------------------------------------
//  Function:  NSComptaPatient::CherchePatEnCours(string& sNumSS)
//
//  Arguments: Une chaine donnee / resultat sNumSS
//
//  Description: retrouve le numSS du patient dans son blocs 5 ou renvoie sNumSS == ""
//
//  Returns: true->OK false->Sinon
//
//------------------------------------------------------------------------------------------
bool
NSComptaPatient::CherchePatEnCours(string& sNumSS)
{
	NSVitale2 benef(pContexte->getSuperviseur()) ;

	string sCodePatient = _pPatient->getNss() ;

	// on ajoute les Data à la base des carte vitales	benef.lastError = benef.open();
	if (benef.lastError != DBIERR_NONE)
	{
		erreur("Erreur à l'ouverture de la base carte vitale.", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
		return false ;
	}

	benef.lastError = benef.chercheClef(&sCodePatient,                                      "NUMPATIENT",
                                      NODEFAULTINDEX,
                                      keySEARCHEQ,
                                      dbiWRITELOCK) ;

	if ((benef.lastError != DBIERR_NONE) && (benef.lastError != DBIERR_RECNOTFOUND))	{
		erreur("Erreur à la recherche d'un bénéficiaire.", standardError, benef.lastError, pContexte->GetMainWindow()->GetHandle()) ;
		benef.close() ;
		return false ;
	}

	if (benef.lastError != DBIERR_RECNOTFOUND)	{
		benef.lastError = benef.getRecord() ;
   	if (benef.lastError != DBIERR_NONE)
		{
			erreur("Erreur à la lecture d'un bénéficiaire.", standardError, benef.lastError, pContexte->GetMainWindow()->GetHandle());
      benef.close() ;
      return false ;
		}

    // patient trouvé : on renvoie son numSS		sNumSS = string(benef.pDonnees->immatricul) ;
	}
	else
   	sNumSS = "" ;

	// on ferme la base CARTE_SV2	benef.lastError = benef.close() ;

	if (benef.lastError != DBIERR_NONE)	{
		erreur("Erreur de fermeture du fichier ChemDoc.", standardError, benef.lastError, pContexte->GetMainWindow()->GetHandle());
		return false ;
	}

	return true ;
}

//------------------------------------------------------------------------------------------//  Function:  NSComptaPatient::SaisieNumSS(string& sNumSS)
//
//  Arguments: Une chaine resultat sNumSS
//
//  Description: récupère le numéro de sécurité sociale (clé des blocs 4)
//
//  Returns: true->OK false->Sinon
//
//------------------------------------------------------------------------------------------
bool
NSComptaPatient::SaisieNumSS(string& sNumSS)
{
	NumSSDialog* pNumSSDlg = new NumSSDialog(pContexte->GetMainWindow(), pContexte) ;

	if ((pNumSSDlg->Execute()) == IDCANCEL)
	{
		delete pNumSSDlg ;
   	return false ;
	}

	// on stocke les donnees du dialogue dans la string
	sNumSS = string(pNumSSDlg->numImma) ;
	delete pNumSSDlg ;

	return true ;
}

//------------------------------------------------------------------------------------------//  Function:  NSComptaPatient::ChercheNumSS(string& sNumSS, NSVitale1Info* pCarteInfo)
//
//  Arguments: Une chaine donnee / resultat sNumSS
//
//  Description: récupère les infos de la carte si elle existe, ou renvoie une chaine vide
//
//  Returns: true->OK false->Sinon
//
//------------------------------------------------------------------------------------------
bool
NSComptaPatient::ChercheNumSS(string& sNumSS, NSVitale1Info* pCarteInfo)
{
  // on recherche le numéro dans le fichier Carte_SV1
  NSVitale1* pCarte = new NSVitale1(pContexte->getSuperviseur()) ;

	pCarte->lastError = pCarte->open() ;
	if (pCarte->lastError != DBIERR_NONE)
	{
		erreur("Erreur à l'ouverture du fichier Carte_SV1.db", standardError, pCarte->lastError, pContexte->GetMainWindow()->GetHandle()) ;
		delete pCarte;
		return false;
	}

   pCarte->lastError = pCarte->chercheClef(&sNumSS,													 		 "",
													 		 0,													 		 keySEARCHEQ,
                               dbiWRITELOCK) ;

   if (pCarte->lastError != DBIERR_NONE)
   {
   	if (pCarte->lastError != DBIERR_RECNOTFOUND)
      {
   		erreur("Erreur à la recherche de la carte vitale.", standardError, pCarte->lastError, pContexte->GetMainWindow()->GetHandle()) ;
      	pCarte->close();
			delete pCarte;
			return false;
      }
   }

   // si la carte existe on récupère les infos
   if (pCarte->lastError != DBIERR_RECNOTFOUND)
   {
   	pCarte->lastError = pCarte->getRecord();
   	if (pCarte->lastError != DBIERR_NONE)
   	{
   		erreur("Erreur à lecture du fichier Carte_SV1.db", standardError, pCarte->lastError, pContexte->GetMainWindow()->GetHandle()) ;
      	pCarte->close();
			delete pCarte;
			return false;
   	}

      *pCarteInfo = NSVitale1Info(pCarte);
   }
   else // sinon on renvoie une string vide et on stocke dans pCarteInfo
   {
   	strcpy(pCarteInfo->pDonnees->immatricul, sNumSS.c_str());
      sNumSS = "";
   }

   pCarte->lastError = pCarte->close();
	if (pCarte->lastError != DBIERR_NONE)
	{
		erreur("Erreur à la fermeture du fichier Carte_SV1.db", standardError, pCarte->lastError, pContexte->GetMainWindow()->GetHandle()) ;
		delete pCarte;
		return false;
	}

   delete pCarte;

   return true;
}

//------------------------------------------------------------------------------------------//  Function:  NSComptaPatient::InitDataCarteVitale(NSVitale1Info* pCarteInfo, bool bCreer)
//
//  Arguments: Les infos carte à initialiser ou à modifer
//
//  Description: Appel du dialogue de création/modification d'une carte
//
//  Returns: true->Les Data sont modifiées false->Sinon
//
//------------------------------------------------------------------------------------------
bool
NSComptaPatient::InitDataCarteVitale(NSVitale1Info* pCarteInfo, bool bCreer)
{
	CreerCarteVitaleDialog* pCVDlg;

	pCVDlg = new CreerCarteVitaleDialog(pContexte->GetMainWindow(), pContexte);

	if (bCreer) // on conserve le numSS
		strcpy(pCVDlg->pData->immatricul, pCarteInfo->pDonnees->immatricul);
	else	// on est en modification
		*(pCVDlg->pData) = *(pCarteInfo->pDonnees);

	// pCVDlg->Create(); : ne marche pas non plus

	if ((pCVDlg->Execute()) == IDCANCEL)
	{
		delete pCVDlg;
		return false;
	}

	// on stocke les donnees du dialogue dans les Data
	*(pCarteInfo->pDonnees) = *(pCVDlg->pData);

	delete pCVDlg;

	return true;
}

//---------------------------------------------------------------------------//  Function:  NSComptaPatient::CmCarteVitale()
//
//  Arguments:
//
//  Description: Création d'une nouvelle carte vitale
//
//---------------------------------------------------------------------------
void NSComptaPatient::CmCarteVitale()
{
	NSVitale1Info* pCarteInfo = new NSVitale1Info();
   string 		 	sNumImma = "", sCode = "";
   bool				bCreerCarte = true;

   if (!CherchePatEnCours(sNumImma))
   {
   	delete pCarteInfo;
      return;
   }

   // si le patient n'appartient pas à un bloc 5
   if (sNumImma == "")
   {
   	// on lance le dialogue de saisie du numéro
   	SaisieNumSS(sNumImma);
   }

   // NOTE : si le numéro n'existe pas, le nouveau numéro
   // est conservé dans pCarteInfo, pour etre ensuite
   // repassé à InitDataCarteVitale.
   if (!ChercheNumSS(sNumImma, pCarteInfo))
   {
   	delete pCarteInfo;
      return;
   }

   // On vérifie si la carte existe
   if (sNumImma != "")
   {
   	bCreerCarte = false;
   }

   if (!InitDataCarteVitale(pCarteInfo,bCreerCarte))
   {
   	delete pCarteInfo;
      return;
   }

   NSVitale1* pCarte = new NSVitale1(pContexte->getSuperviseur());

	// on ajoute les Data à la base des carte vitales
   pCarte->lastError = pCarte->open();
   if (pCarte->lastError != DBIERR_NONE)
   {
   	erreur("Erreur à l'ouverture de la base carte vitale.", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
      delete pCarte;
      delete pCarteInfo;
      return;
   }

   *(pCarte->pDonnees) = *(pCarteInfo->pDonnees);

   if (bCreerCarte)
   {
   	pCarte->lastError = pCarte->appendRecord();
   	if (pCarte->lastError != DBIERR_NONE)
   	{
   		erreur("Erreur à l'ajout de la fiche carte vitale.", standardError, pCarte->lastError, pContexte->GetMainWindow()->GetHandle()) ;
      	pCarte->close();
      	delete pCarte;
      	delete pCarteInfo;
      	return;
   	}
   }
   else // on modifie la carte vitale correspondant à sNumImma
   {
   	pCarte->lastError = pCarte->chercheClef(&sNumImma,
													 		 "",
													 		 0,
													 		 keySEARCHEQ,
                                              dbiWRITELOCK);

      if (pCarte->lastError != DBIERR_NONE)
   	{
   		erreur("Erreur à la recherche de la carte vitale.", standardError, pCarte->lastError, pContexte->GetMainWindow()->GetHandle()) ;
      	pCarte->close();
			delete pCarte;
         delete pCarteInfo;
			return;
   	}

   	pCarte->lastError = pCarte->modifyRecord(TRUE);
   	if (pCarte->lastError != DBIERR_NONE)
   	{
   		erreur("Erreur à la modification de la fiche carte vitale.", standardError, pCarte->lastError, pContexte->GetMainWindow()->GetHandle()) ;
      	pCarte->close();
      	delete pCarte;
      	delete pCarteInfo;
      	return;
   	}
   }

   pCarte->lastError = pCarte->close();
   if (pCarte->lastError != DBIERR_NONE)
   	erreur("Erreur à la fermeture de la base carte vitale.", standardError, pCarte->lastError, pContexte->GetMainWindow()->GetHandle()) ;

   delete pCarte;
   delete pCarteInfo;
}

bool
NSComptaPatient::GetComptForDoc(string& sNumCompt, string sDocID, string sPatID)
{
  if (string("") == sDocID)
    return false ;

  if ((string("") == sPatID) && (NULL != _pPatient))
    sPatID = _pPatient->getNss() ;

  if (string("") == sPatID)
    return false ;

  NSComptDocu dbCompDoc(pContexte->getSuperviseur()) ;

  // Opening table
  //
  dbCompDoc.lastError = dbCompDoc.open() ;
  if (DBIERR_NONE != dbCompDoc.lastError)
  {
    erreur("Erreur à l'ouverture de la base nsm_cp_docu.db.", standardError, dbCompDoc.lastError, pContexte->GetMainWindow()->GetHandle()) ;
    return false ;
  }

  // Building the composite index search structure
  //
  CURProps curProps ;
  DBIResult ErrDBI = DbiGetCursorProps(dbCompDoc.PrendCurseur(), curProps) ;
  Byte* pIndexRec = new Byte[curProps.iRecBufSize] ;
  memset(pIndexRec, 0, curProps.iRecBufSize) ;

  DbiPutField(dbCompDoc.PrendCurseur(), CPTDOCU_PATIENT_FIELD,  pIndexRec, (Byte*)sPatID.c_str()) ;
  DbiPutField(dbCompDoc.PrendCurseur(), CPTDOCU_DOCUMENT_FIELD, pIndexRec, (Byte*)sDocID.c_str()) ;

  // Searching for this document
  //
  ErrDBI = dbCompDoc.chercheClefComposite("DOCUMENT_INDEX",
                                          NODEFAULTINDEX,
                                          keySEARCHGEQ,
                                          dbiWRITELOCK,
                                          pIndexRec) ;
  delete[] pIndexRec ;

  if ((DBIERR_NONE != ErrDBI) && (DBIERR_EOF != ErrDBI))
		erreur("Erreur de positionnement dans la base nsm_cp_docu.db.", standardError, ErrDBI, pContexte->GetMainWindow()->GetHandle()) ;

  //
	// Si on se trouve positionné en fin de fichier on recule
	//
	if (DBIERR_EOF == dbCompDoc.lastError)
    dbCompDoc.lastError = dbCompDoc.precedent(dbiWRITELOCK) ;

  dbCompDoc.lastError = dbCompDoc.getRecord() ;

  if (DBIERR_NONE != dbCompDoc.lastError)  {
    erreur("erreur à la lecture du fichier nsm_cp_docu.db.", standardError, dbCompDoc.lastError, pContexte->GetMainWindow()->GetHandle()) ;
    dbCompDoc.close() ;
    return false ;
  }

  if ((string(dbCompDoc._Donnees.patient)  == sPatID) &&
      (string(dbCompDoc._Donnees.document) == sDocID))
    sNumCompt = string(dbCompDoc._Donnees.numcompt) ;
  else
    sNumCompt = string("") ;

  dbCompDoc.close() ;

  return true ;
}

bool
NSComptaPatient::GetDocsForCompt(string sNumCompt, NSComptDocuArray *pComptDocuArray)
{
  if ((string("") == sNumCompt) || (NULL == pComptDocuArray))
    return false ;

  NSComptDocu dbCompDoc(pContexte->getSuperviseur()) ;

  // Opening table
  //
  dbCompDoc.lastError = dbCompDoc.open() ;
  if (DBIERR_NONE != dbCompDoc.lastError)
  {
    string sErreur = string("Erreur à l'ouverture de la base nsm_cp_docu.db.") ;
    erreur(sErreur.c_str(), standardError, dbCompDoc.lastError, pContexte->GetMainWindow()->GetHandle()) ;
    pContexte->getSuperviseur()->trace(&sErreur, 1, NSSuper::trError) ;
    return false ;
  }

  string sIndice = sNumCompt + string(CPTDOCU_INDICE_LEN, '0') ;

  // Searching for this accounting record
  //
  dbCompDoc.lastError = dbCompDoc.chercheClef(&sIndice,
													 		                "",
                                              0,
                                              keySEARCHGEQ,
                                              dbiWRITELOCK) ;

  if ((DBIERR_NONE != dbCompDoc.lastError) && (DBIERR_EOF != dbCompDoc.lastError))
		erreur("Erreur de positionnement dans la base nsm_cp_docu.db.", standardError, dbCompDoc.lastError, pContexte->GetMainWindow()->GetHandle()) ;

  //
	// Si on se trouve positionné en fin de fichier on recule
	//
	if (DBIERR_EOF == dbCompDoc.lastError)
    dbCompDoc.lastError = dbCompDoc.precedent(dbiWRITELOCK) ;

  while (DBIERR_NONE == dbCompDoc.lastError)
  {
    dbCompDoc.lastError = dbCompDoc.getRecord() ;

    if (DBIERR_NONE != dbCompDoc.lastError)    {
      string sErreur = string("Erreur à la lecture de la table nsm_cp_docu.db.") ;
      erreur(sErreur.c_str(), standardError, dbCompDoc.lastError, pContexte->GetMainWindow()->GetHandle()) ;
      pContexte->getSuperviseur()->trace(&sErreur, 1, NSSuper::trError) ;
      dbCompDoc.close() ;
      return false ;
    }

    if (string(dbCompDoc._Donnees.numcompt) != sNumCompt)
      break ;

    pComptDocuArray->push_back(new NSComptDocuInfo(&dbCompDoc)) ;

    dbCompDoc.lastError = dbCompDoc.suivant(dbiWRITELOCK) ;
  }

  dbCompDoc.close() ;

  return true ;
}

bool
NSComptaPatient::SetComptForDoc(string sNumCompt, string sDocID, string sPatID)
{
  if ((string("") == sNumCompt) || (string("") == sDocID))
    return false ;

  if ((string("") == sPatID) && (NULL != _pPatient))
    sPatID = _pPatient->getNss() ;

  if (string("") == sPatID)
    return false ;

  // If the "doc - compt" link is already set, there is nothing to do
  //
  string sExistingNumCompt = string("") ;
  if (GetComptForDoc(sExistingNumCompt, sDocID, sPatID) && (sExistingNumCompt == sNumCompt))
    return true ;

  NSComptDocu dbCompDoc(pContexte->getSuperviseur()) ;

  // Opening table
  //
  dbCompDoc.lastError = dbCompDoc.open() ;
  if (DBIERR_NONE != dbCompDoc.lastError)
  {
    erreur("Erreur à l'ouverture de la base nsm_cp_docu.db.", standardError, dbCompDoc.lastError, pContexte->GetMainWindow()->GetHandle()) ;
    return false ;
  }

  //
	// On se "positionne" sur le dernier code possible
	//
  string sMaxCode = sNumCompt + string(CPTDOCU_INDICE_LEN, 'z') ;

  dbCompDoc.lastError = dbCompDoc.chercheClef(&sMaxCode,
                                              "",
                                              0,
                                              keySEARCHGEQ,
                                              dbiWRITELOCK) ;

	//	// Si on se trouve positionné en fin de fichier on recule
	//
	if (DBIERR_EOF == dbCompDoc.lastError)
    dbCompDoc.lastError = dbCompDoc.precedent(dbiWRITELOCK) ;
	//
	// Toute autre erreur est anormale
	//
	else if ((DBIERR_NONE != dbCompDoc.lastError) && (DBIERR_BOF != dbCompDoc.lastError))
	{
		erreur("Le fichier nsm_cp_docu.db est endommagé.", standardError, dbCompDoc.lastError, pContexte->GetMainWindow()->GetHandle()) ;
    dbCompDoc.close() ;
		return false ;
	}
	//
	// On récupère l'enregistrement si fichier non vide
	//
  if (DBIERR_BOF != dbCompDoc.lastError)
  {
    dbCompDoc.lastError = dbCompDoc.getRecord() ;

    if (DBIERR_NONE != dbCompDoc.lastError)		{
      erreur("erreur à la lecture du fichier nsm_cp_docu.db.", standardError, dbCompDoc.lastError, pContexte->GetMainWindow()->GetHandle()) ;
      dbCompDoc.close() ;
			return false ;
    }
  }

	//	// Si on est en fin ou début de fichier, c'est qu'il est vide
	//
	if ((DBIERR_BOF       == dbCompDoc.lastError) ||
      (DBIERR_EOF       == dbCompDoc.lastError) ||
      (DBIERR_NOCURRREC == dbCompDoc.lastError))
  {
    dbCompDoc._Donnees.metAZero() ;
    strcpy(dbCompDoc._Donnees.numcompt, sNumCompt.c_str()) ;
    strcpy(dbCompDoc._Donnees.indice,   string(CPTDOCU_INDICE_LEN, '0').c_str()) ;
    strcpy(dbCompDoc._Donnees.patient,  sPatID.c_str()) ;
    strcpy(dbCompDoc._Donnees.document, sDocID.c_str()) ;

    dbCompDoc.lastError = dbCompDoc.appendRecord() ;
    if (DBIERR_NONE != dbCompDoc.lastError)    {
      erreur("Erreur à l'ajout d'une fiche nsm_cp_docu vierge.", standardError, dbCompDoc.lastError, pContexte->GetMainWindow()->GetHandle()) ;
      dbCompDoc.close() ;
      return false ;
    }
    dbCompDoc.close() ;

		return true ;  }

	//	// Toute autre erreur est anormale
	//
	if (DBIERR_NONE != dbCompDoc.lastError)
	{
		erreur("Le fichier nsm_cp_docu.db est endommagé.", standardError, dbCompDoc.lastError, pContexte->GetMainWindow()->GetHandle()) ;
    dbCompDoc.close() ;
		return false ;
	}

  string sNewCode = string(CPTDOCU_INDICE_LEN, '0') ;

  //
	// Si le document trouvé n'appartient pas à cette fiche, on recule d'un cran
	//
	if (strcmp(dbCompDoc._Donnees.numcompt, sNumCompt.c_str()) != 0)
	{
		dbCompDoc.lastError = dbCompDoc.precedent(dbiWRITELOCK);

		//
		// Toute autre erreur est anormale
		//
		if (DBIERR_NONE == dbCompDoc.lastError)
			dbCompDoc.lastError = dbCompDoc.getRecord() ;
		if ((DBIERR_NONE != dbCompDoc.lastError) && (DBIERR_BOF != dbCompDoc.lastError))
		{
			erreur("Le fichier nsm_cp_docu.db est endommagé.", standardError, dbCompDoc.lastError) ;
			dbCompDoc.close() ;
		  return false ;
		}
		//
		// Si le document trouvé n'appartient pas au patient, il n'y a pas
		// de document
		//
		if (strcmp(dbCompDoc._Donnees.numcompt, sNumCompt.c_str()) == 0)
			sNewCode = string(dbCompDoc._Donnees.indice) ;
	}
  else
    sNewCode = string(dbCompDoc._Donnees.indice) ;

	//
	// On incrémente le compteur
	//
	int calculer = 1 ;
	size_t i = strlen(sNewCode.c_str()) - 1 ;

	while (calculer)  {
    int j = (int) sNewCode[i] ;
		j++ ;

		if (((j >= '0') && (j <= '9')) || ((j >= 'A') && (j <= 'Z')))		{
			sNewCode[i] = (char) j ;
			calculer = 0 ;
		}
		else if ((j > '9') && (j < 'A'))
		{
			sNewCode[i] = 'A' ;
			calculer = 0 ;
		}
		else
		{
			sNewCode[i] = '0' ;
			if (0 == i)
			{
				erreur("Compteur de fiches compt saturé pour nsm_cp_docu.db.", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
        dbCompDoc.close() ;
				return false ;
			}
			i-- ;
		}
	}

  // on enregistre  //  dbCompDoc._Donnees.metAZero();
  strcpy(dbCompDoc._Donnees.numcompt, sNumCompt.c_str()) ;
  strcpy(dbCompDoc._Donnees.indice,   sNewCode.c_str()) ;
  strcpy(dbCompDoc._Donnees.patient,  sPatID.c_str()) ;
  strcpy(dbCompDoc._Donnees.document, sDocID.c_str()) ;

  dbCompDoc.lastError = dbCompDoc.appendRecord() ;
  if (DBIERR_NONE != dbCompDoc.lastError)
  {
    erreur("Erreur à l'ajout d'une fiche nsm_cp_docu vierge.", standardError, dbCompDoc.lastError, pContexte->GetMainWindow()->GetHandle()) ;
    dbCompDoc.close() ;
    return false ;
  }

  dbCompDoc.close() ;

  return true ;
}

bool
NSComptaPatient::GetCcamInfoForCompt(string sNumCompt, NSFse16Array *paFseCCAM)
{
  if ((string("") == sNumCompt) || (NULL == paFseCCAM))
    return false ;

  NSFseCCAM fseCCAM(pContexte->getSuperviseur()) ;

  return fseCCAM.initArrayForCompt(sNumCompt, paFseCCAM) ;
}

boolNSComptaPatient::ChercheNumCompt(string& sNumCompt)
{
	string 	sNumero ;
	char 	PremOrdre[CPTA_NUMCOMPT_LEN + 1],
		  	DernOrdre[CPTA_NUMCOMPT_LEN + 1] ;

	//
	// Préparation des compteurs mini et maxi
	//
	int i;
	for (i = 0; i < CPTA_NUMCOMPT_LEN; i++)
	{
		PremOrdre[i] = '0';
		DernOrdre[i] = 'z';
	}
	PremOrdre[i] 	= '\0';
	DernOrdre[i] 	= '\0';
	PremOrdre[i-1] = '1';

    // Entrée en section critique
#ifndef N_TIERS
    if (!pContexte->DemandeAcces("COMPT"))    {
    	erreur("Impossible d'obtenir l'accès à la base Compt.", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
        return false;
    }
#endif
	//
	// Prise d'un objet NSCompt
	//
  NSCompt ComptTest(pContexte->getSuperviseur()) ;

  ComptTest.lastError = ComptTest.open() ;
  if (DBIERR_NONE != ComptTest.lastError)
  {
    erreur("Erreur à l'ouverture de la base nscompt.db.", standardError, ComptTest.lastError, pContexte->GetMainWindow()->GetHandle()) ;
#ifndef N_TIERS
    pContexte->LibereAcces("COMPT");
#endif
    return false ;
  }

	//
	// On tente de se positionner sur le dernier code possible : "zzzzzzzz"
	//
  sNumero = string(DernOrdre) ;

  ComptTest.lastError = ComptTest.chercheClef(&sNumero,
                                              "",
                                              0,
                                              keySEARCHGEQ,
                                              dbiWRITELOCK) ;

	//	// Si on se trouve positionné en fin de fichier on recule
	//
	if (DBIERR_EOF == ComptTest.lastError)
    ComptTest.lastError = ComptTest.precedent(dbiWRITELOCK) ;
	//
	// Toute autre erreur est anormale
	//
	else if ((DBIERR_NONE != ComptTest.lastError) && (DBIERR_BOF != ComptTest.lastError))
	{
		erreur("Le fichier nscompt.db est endommagé.", standardError, ComptTest.lastError, pContexte->GetMainWindow()->GetHandle()) ;
    ComptTest.close() ;
#ifndef N_TIERS
    pContexte->LibereAcces("COMPT");#endif
		return false ;	}
	//
	// On récupère l'enregistrement si fichier non vide
	//
  if (DBIERR_BOF != ComptTest.lastError)
  {
    ComptTest.lastError = ComptTest.getRecord() ;

    if (DBIERR_NONE != ComptTest.lastError)		{
      erreur("erreur à la lecture du fichier compt.db.", standardError, ComptTest.lastError, pContexte->GetMainWindow()->GetHandle()) ;
      ComptTest.close() ;
#ifndef N_TIERS
      pContexte->LibereAcces("COMPT");#endif
			return false ;    }
  }

	//	// Si on est en fin ou début de fichier, c'est qu'il est vide
	//
	if ((DBIERR_BOF       == ComptTest.lastError) ||
      (DBIERR_EOF       == ComptTest.lastError) ||
      (DBIERR_NOCURRREC == ComptTest.lastError))
  {
    sNumCompt = string(PremOrdre) ;
    ComptTest._Donnees.metAZero() ;
    strcpy(ComptTest._Donnees.numcompt, sNumCompt.c_str()) ;

    ComptTest.lastError = ComptTest.appendRecord() ;
    if (DBIERR_NONE != ComptTest.lastError)    {
      erreur("Erreur à l'ajout d'une fiche Compt vierge.", standardError, ComptTest.lastError, pContexte->GetMainWindow()->GetHandle()) ;
      ComptTest.close() ;
#ifndef N_TIERS
      pContexte->LibereAcces("COMPT") ;
#endif
      return false ;    }
    ComptTest.close() ;

#ifndef N_TIERS    pContexte->LibereAcces("COMPT");#endif
		return true ;  }

	//	// Toute autre erreur est anormale
	//
	if (DBIERR_NONE != ComptTest.lastError)
	{
		erreur("Le fichier compt.db est endommagé.", standardError, ComptTest.lastError, pContexte->GetMainWindow()->GetHandle()) ;
    ComptTest.close() ;
#ifndef N_TIERS
    pContexte->LibereAcces("COMPT") ;#endif
		return false ;	}

	//	// On est positionné sur le dernier document du patient, que l'on doit
	// incrémenter pour avoir le nouveau code
	//
	char compteur[CPTA_NUMCOMPT_LEN + 1] ;
	strcpy(compteur, ComptTest._Donnees.numcompt) ;

	//
	// On incrémente le compteur
	//
	int calculer = 1 ;
	i = strlen(compteur) - 1 ;

	while (calculer)  {
    int j = (int) compteur[i] ;
		j++ ;

		if (((j >= '0') && (j <= '9')) || ((j >= 'A') && (j <= 'Z')))		{
			compteur[i] = (char) j ;
			calculer = 0 ;
		}
		else if ((j > '9') && (j < 'A'))
		{
			compteur[i] = 'A' ;
			calculer = 0 ;
		}
		else
		{
			compteur[i] = '0' ;
			if (0 == i)
			{
				erreur("Compteur de fiches compt saturé.", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
        ComptTest.close() ;
#ifndef N_TIERS
        pContexte->LibereAcces("COMPT");#endif
				return false ;			}
			i-- ;
		}
	}

	sNumCompt = string(compteur) ;
  // on stocke une fiche vierge pour etre plus sur  ComptTest._Donnees.metAZero();
  strcpy(ComptTest._Donnees.numcompt, sNumCompt.c_str()) ;

  ComptTest.lastError = ComptTest.appendRecord() ;
  if (DBIERR_NONE != ComptTest.lastError)
  {
    erreur("Erreur à l'ajout d'une fiche Compt vierge.", standardError, ComptTest.lastError, pContexte->GetMainWindow()->GetHandle()) ;
    ComptTest.close() ;
#ifndef N_TIERS
    pContexte->LibereAcces("COMPT");#endif
    return false ;  }
  ComptTest.lastError = ComptTest.close() ;  if (DBIERR_NONE != ComptTest.lastError)
  {
    erreur("Erreur à la fermeture de la base nscompt.db.", standardError, ComptTest.lastError, pContexte->GetMainWindow()->GetHandle()) ;
#ifndef N_TIERS
    pContexte->LibereAcces("COMPT") ;#endif
    return false ;  }

#ifndef N_TIERS  // Sortie de section critique  pContexte->LibereAcces("COMPT");
#endif
	return true ;
}

//// Fonction CreerFicheCompt pour la création des fiches compt :
// on passe le nouveau numéro de la fiche dans sNumCompt
//
bool
NSComptaPatient::CreerFicheCompt(NSComptInfo* pComptInfo, NSFse1610Info* pFse1610Info, NSFse16Array* pFseArray, string sDocID)
{
try
{
	string sCodeExam = string("") ;
  NSComptData* pCompData = (NSComptData*) 0 ;

  // si la fiche compt a des données initiales
  //
	if (pComptInfo)
	{
		sCodeExam = string(pComptInfo->_Donnees.examen) + string(pComptInfo->_Donnees.synonyme) ;
		pCompData = &(pComptInfo->_Donnees) ;
	}
	else
		pComptInfo = new NSComptInfo() ;

	CreerFicheComptDialog ComptDlg(pContexte->GetMainWindow(), pContexte, _pPatient, true, pCompData) ;

  ComptDlg.nbPrest = 0 ;

  if (pFseArray && (false == pFseArray->empty()))
  	for (NSFse16Iter i = pFseArray->begin() ; pFseArray->end() != i ; i++)
    {
   		ComptDlg.pFseArray->push_back(new NSBlocFse16(*(*i))) ;
      ComptDlg.nbPrest++ ;
    }

	if (pFse1610Info)
	{
		InitFse1610Dialog Fse1610Dlg(pContexte->GetMainWindow(), pContexte, sCodeExam) ;
		*(Fse1610Dlg.pData) = pFse1610Info->_Donnees ;

		if ((Fse1610Dlg.Execute()) == IDOK)		{
			// on ajoute une Fse1610 au tableau des Fse
			pFse1610Info->_Donnees = *(Fse1610Dlg.pData) ;

			ComptDlg.pFseArray->push_back(new NSBlocFse16(pFse1610Info)) ;
			ComptDlg.nbPrest++ ;
		}
		else
			return true ; 		// pour ne pas afficher de message d'erreur en sortie
	}

	// on execute le dialogue de la fiche compt
	// avec éventuellement les données stockées dans les datas
	if ((ComptDlg.Execute()) == IDCANCEL)
		return true ; 		// pour ne pas afficher de message d'erreur en sortie

	// on stocke les donnees du dialogue dans les Info
	pComptInfo->_Donnees = *(ComptDlg.pData) ;

	// ... SECTION CRITIQUE ...
	// on cherche le numéro compt pour enregistrement

  string sNum = string("") ;
	if (false == ChercheNumCompt(sNum))
		return false ;

	// ........................

	// on enregistre la fiche compt dans la base
	// (par modification de la fiche vierge créee sous le numéro sNum)
	NSCompt compt(pContexte->getSuperviseur()) ;

	compt.lastError = compt.open() ;
	if (compt.lastError != DBIERR_NONE)
	{
		erreur("Erreur à l'ouverture de la base Compt.", standardError, compt.lastError, pContexte->GetMainWindow()->GetHandle()) ;
		return false ;
	}

	compt.lastError = compt.chercheClef(&sNum,
                                      "",
												              0,
												              keySEARCHEQ,
                                      dbiWRITELOCK) ;

	if (DBIERR_NONE != compt.lastError)
	{
		erreur("Erreur à la recherche de la nouvelle fiche compt.", standardError, compt.lastError, pContexte->GetMainWindow()->GetHandle()) ;
		compt.close() ;
		return false ;
	}

	compt.lastError = compt.getRecord() ;
	if (DBIERR_NONE != compt.lastError)
	{
		erreur("Erreur à lecture du fichier Compt.db", standardError, compt.lastError, pContexte->GetMainWindow()->GetHandle()) ;
		compt.close() ;
		return false ;
	}

	compt._Donnees = pComptInfo->_Donnees ;
	// on enregistre le numéro compt
	strcpy(compt._Donnees.numcompt, sNum.c_str()) ;

	compt.lastError = compt.modifyRecord(TRUE) ;
	if (DBIERR_NONE != compt.lastError)
	{
		erreur("Erreur à la modification de la fiche compt.", standardError, compt.lastError, pContexte->GetMainWindow()->GetHandle()) ;
		compt.close() ;
		return false ;
	}

	compt.lastError = compt.close() ;
	if (DBIERR_NONE != compt.lastError)
		erreur("Erreur à la fermeture de la base Compt.", standardError, compt.lastError, pContexte->GetMainWindow()->GetHandle()) ;

	// on enregistre les autres données sous le meme numéro
	ComptDlg.EnregDonneesCompt(sNum) ;

  Export("ADD", sNum, string(compt._Donnees.patient)) ;

  strcpy(pComptInfo->_Donnees.numcompt, compt._Donnees.numcompt) ;

	return true ;
}
catch (...)
{
	erreur("Exception NSComptaPatient::CreerFicheCompt", standardError, 0) ;
  return false ;
}
}voidNSComptaPatient::InitComptInfoFromDocument(NSComptInfo* pComptInfo, NSFse16Array* pFseCcamArray, NVLdVTemps *pDate, const NSPatPathoArray *pPpt){  if ((NULL == pComptInfo) || (NULL == pFseCcamArray) || (NULL == pPpt) || (NULL == pDate))    return ;  // Date  //  // Warning : in the accounting module, hour just means hhmm and not hhmmss  string sAccountingHour = string(pDate->donneHeure(), 0, 4) ;

  strcpy(pComptInfo->_Donnees.date,   pDate->donneDate().c_str()) ;
  strcpy(pComptInfo->_Donnees.heure,  sAccountingHour.c_str()) ;

  // Context  //  string sExtHosp = string("") ;
	pPpt->CheminDansPatpatho((NSSmallBrother*) 0, "LADMI/LCONT", &sExtHosp) ;
	if (string("") != sExtHosp)
		sExtHosp = string(sExtHosp, 0, 5) ;

  // Root concept
  //
  string sExam = string("") ;
  string sSyn  = string("") ;

	string sCode = (*(pPpt->begin()))->getLexique() ;
	if (string("") != sCode)
	{
		sExam = string(sCode, 0, 5) ;
		sSyn  = string(sCode, 5, 1) ;
	}  // CCAM code  //
  InitCcamFromDocument(pComptInfo, pFseCcamArray, pPpt) ;

	strcpy(pComptInfo->_Donnees.examen,   sExam.c_str()) ;
	strcpy(pComptInfo->_Donnees.synonyme, sSyn.c_str()) ;
	strcpy(pComptInfo->_Donnees.contexte, sExtHosp.c_str()) ;
}stringNSComptaPatient::GetCcamFromDocument(const NSPatPathoArray *pPpt){  if ((NULL == pPpt) || pPpt->empty())    return string("") ;  // CCAM code  //
	string sCodeActe = string("") ;
  string sChemin   = string("0CODE/6CCAM") ;

  pPpt->CheminDansPatpatho((NSSmallBrother*) 0, sChemin, &sCodeActe) ;

  return sCodeActe ;
}

// ------------ Get CCAM code from patpatho
//
void
NSComptaPatient::InitCcamFromDocument(NSComptInfo* pComptInfo, NSFse16Array* pFseCcamArray, const NSPatPathoArray *pPpt){  if ((NULL == pComptInfo) || (NULL == pFseCcamArray) || (NULL == pPpt))    return ;  // CCAM code  //
	string sCodeActe = GetCcamFromDocument(pPpt) ;
  if (string("") == sCodeActe)
    return ;

  NSCcam     ccamFile(pContexte->getSuperviseur()) ;
  NSCcamInfo ccamInfo ;
  DBIResult  result = ccamFile.getRecordByCode(sCodeActe, &ccamInfo, true, true) ;
  int        nbPrest = 0 ;

  if (ccamInfo.getCode() != sCodeActe)
    return ;

  NSFseCCAMInfo ccamFSEInfo ;
  ccamFSEInfo.initFromCCAMinfo(&ccamInfo) ;

  // on cale la date de la FseCCAM sur celle de la fiche Compt
  strcpy(ccamFSEInfo._Donnees.date, pComptInfo->_Donnees.date) ;
  strcat(ccamFSEInfo._Donnees.date, pComptInfo->_Donnees.heure) ;

  // on copie le numéro de la prestation
  char numero[FSE_NUMPREST_LEN + 1] ;
  sprintf(numero, "%04d", nbPrest) ;
  strcpy(ccamFSEInfo._Donnees.numprest, numero) ;
  nbPrest++ ;

  pFseCcamArray->push_back(new NSBlocFse16(&ccamFSEInfo)) ;
}

boolNSComptaPatient::CmFicheCompt(NSComptInfo* pComptInit, NSFse1610Info* pFse1610Init, NSFse16Array* pFseArray)
{
	if (pFse1610Init)
		strcpy(pFse1610Init->_Donnees.numprest, "0000") ;

	if (false == CreerFicheCompt(pComptInit, pFse1610Init, pFseArray))
		erreur("La fiche compt n'a pas pu être enregistrée...", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;

  return true ;
}

bool
NSComptaPatient::getComptInfo(string sNumCompt, NSComptInfo *pComptInfo)
{
  if ((string("") == sNumCompt) || (NULL == pComptInfo))
    return false ;

  NSCompt Compt(pContexte->getSuperviseur()) ;

	Compt.lastError = Compt.open() ;
	if (Compt.lastError != DBIERR_NONE)
	{
		erreur("Erreur à l'ouverture de la base Compt.", standardError, 0) ;
    return false ;
	}

	Compt.lastError = Compt.chercheClef(&sNumCompt,
                                      "",
                                      0,
                                      keySEARCHEQ,
                                      dbiWRITELOCK) ;

	if (Compt.lastError != DBIERR_NONE)
	{
		erreur("Erreur à la recherche d'une fiche compt.", standardError, Compt.lastError) ;
    Compt.close() ;
    return false ;
	}

	Compt.lastError = Compt.getRecord() ;  if (Compt.lastError != DBIERR_NONE)
	{
  	erreur("Erreur à lecture du fichier Compt.db", standardError, Compt.lastError);
    Compt.close() ;
    return false ;
  }

  if (string(Compt._Donnees.numcompt) != sNumCompt)
  {
    Compt.close() ;
    return false ;
	}

  pComptInfo->_Donnees = Compt._Donnees ;

  Compt.lastError = Compt.close() ;
	if (Compt.lastError != DBIERR_NONE)
  	erreur("Erreur à la fermeture de la base Compt.db.", standardError, Compt.lastError) ;

  return true ;
}

bool
NSComptaPatient::EditCompt(string sNumCompt, TWindow* pere, NSComptInfo *pComptInfo, NSFse1610Info* pFse1610Info, NSFse16Array* pFseArray)
{
  if (string("") == sNumCompt)
    return false ;

  NSCompt Compt(pContexte->getSuperviseur()) ;

	Compt.lastError = Compt.open() ;
	if (Compt.lastError != DBIERR_NONE)
	{
		erreur("Erreur à l'ouverture de la base Compt.", standardError, 0) ;
    return false ;
	}

	Compt.lastError = Compt.chercheClef(&sNumCompt,
                                      "",
                                      0,
                                      keySEARCHEQ,
                                      dbiWRITELOCK) ;

	if (Compt.lastError != DBIERR_NONE)
	{
		erreur("Erreur à la recherche d'une fiche compt.", standardError, Compt.lastError) ;
    Compt.close() ;
    return false ;
	}

	Compt.lastError = Compt.getRecord() ;  if (Compt.lastError != DBIERR_NONE)
	{
  	erreur("Erreur à lecture du fichier Compt.db", standardError, Compt.lastError);
    Compt.close() ;
    return false ;
  }

	// on lance la modif des infos récupérées
  //
	CreerFicheComptDialog ComptDlg(pere, pContexte, _pPatient, false, &(Compt._Donnees)) ;

  if (pFseArray)
    *(ComptDlg.pFseArray) = *pFseArray ;

	if (ComptDlg.Execute() == IDCANCEL)
	{
  	Compt.close() ;
    return false ;
	}

	// on stocke les donnees du dialogue dans les Data
	Compt._Donnees = *(ComptDlg.pData) ;

	Compt.lastError = Compt.modifyRecord(TRUE) ;
	if (Compt.lastError != DBIERR_NONE)
	{
		erreur("Erreur à la modification de la fiche compt.", standardError, Compt.lastError) ;
    Compt.close() ;
    return false ;
	}

  if (pComptInfo)
    pComptInfo->_Donnees = Compt._Donnees ;

	Compt.lastError = Compt.close() ;
	if (Compt.lastError != DBIERR_NONE)
  	erreur("Erreur à la fermeture de la base Compt.db.", standardError, Compt.lastError) ;

	// on enregistre les autres donnees sous le meme numcompt
	ComptDlg.EnregDonneesCompt(sNumCompt) ;

  return true ;
}

bool
NSComptaPatient::DeleteCompt(string sNumCompt, bool bCallCptaDll)
{
  if (string("") == sNumCompt)
    return false ;

  // on enleve la fiche compt de la base
  //
  NSCompt Compt(pContexte->getSuperviseur()) ;

  Compt.lastError = Compt.open() ;
  if (DBIERR_NONE != Compt.lastError)
  {
    string sErreur = string("NSComptaPatient::DeleteCompt -> Erreur à l'ouverture de la base Compt.db.") ;
    erreur(sErreur.c_str(), standardError, Compt.lastError, pContexte->GetMainWindow()->GetHandle()) ;
    pContexte->getSuperviseur()->trace(&sErreur, 1, NSSuper::trError) ;
    return false ;
  }

  Compt.lastError = Compt.chercheClef(&sNumCompt,
												              "",
                                      0,
                                      keySEARCHEQ,
                                      dbiWRITELOCK) ;

  if (DBIERR_NONE != Compt.lastError)
  {
    string sErreur = string("NSComptaPatient::DeleteCompt -> Erreur à la recherche de la fiche compt.") ;
    erreur(sErreur.c_str(), standardError, Compt.lastError, pContexte->GetMainWindow()->GetHandle()) ;
    pContexte->getSuperviseur()->trace(&sErreur, 1, NSSuper::trError) ;
    Compt.close() ;
    return false ;
  }

  Compt.lastError = Compt.getRecord() ;
  if (Compt.lastError != DBIERR_NONE)
	{
  	erreur("Erreur à lecture du fichier Compt.db", standardError, Compt.lastError);
    string sErreur = string("NSComptaPatient::DeleteCompt -> Erreur à lecture dans Compt.db.") ;
    pContexte->getSuperviseur()->trace(&sErreur, 1, NSSuper::trError) ;
    Compt.close() ;
    return false ;
  }

  if (string(Compt._Donnees.numcompt) != sNumCompt)
  {
    erreur("Erreur à lecture du fichier Compt.db", standardError, Compt.lastError);
    string sErreur = string("NSComptaPatient::DeleteCompt -> Erreur de positionnement dans Compt.db.") ;
    pContexte->getSuperviseur()->trace(&sErreur, 1, NSSuper::trError) ;
    Compt.close() ;
    return false ;
	}

  string sPatientId = string(Compt._Donnees.patient) ;

  CreerFicheComptDialog ComptDlg(pContexte->GetMainWindow(), pContexte, _pPatient, false, (NSComptData*) 0, true /* mettre bNoInit à true */) ;

  // Note : on enregistre les données en laissant tous les tableaux vides
  // puisqu'on ne passe pas par SetupWindow -> suppression des données liées
  // à la fiche compt
  ComptDlg.EnregDonneesCompt(sNumCompt) ;

  Compt.lastError = Compt.deleteRecord() ;
  if (Compt.lastError != DBIERR_NONE)
  {
    string sErreur = string("NSComptaPatient::DeleteCompt -> Erreur à la destruction de la fiche compt.") ;
    erreur(sErreur.c_str(), standardError, Compt.lastError, pContexte->GetMainWindow()->GetHandle()) ;
    pContexte->getSuperviseur()->trace(&sErreur, 1, NSSuper::trError) ;
    Compt.close() ;
    return false ;
  }

  Compt.lastError = Compt.close() ;
  if (Compt.lastError != DBIERR_NONE)
    erreur("Erreur à la fermeture de la base Compt.db.", standardError, Compt.lastError) ;

  if (bCallCptaDll)
    Export(string("DELETED"), sNumCompt, sPatientId) ;

  DeleteAllComptDocu(sNumCompt) ;

  return true ;
}

// Removing a record from nsm_cp_docu
//
bool
NSComptaPatient::DeleteComptDocu(string sNumCompt, string sPatId, string sDocId)
{
  if ((string("") == sNumCompt) || (string("") == sPatId) || (string("") == sDocId))
    return false ;

  NSComptDocu dbCompDoc(pContexte->getSuperviseur()) ;

  dbCompDoc.lastError = dbCompDoc.open() ;
  if (DBIERR_NONE != dbCompDoc.lastError)
  {
    string sErreur = string("NSComptaPatient::DeleteComptDocu -> Erreur à l'ouverture de la table nsm_cp_docu.db.") ;
    erreur(sErreur.c_str(), standardError, dbCompDoc.lastError, pContexte->GetMainWindow()->GetHandle()) ;
    pContexte->getSuperviseur()->trace(&sErreur, 1, NSSuper::trError) ;
    return false ;
  }

  dbCompDoc.lastError = dbCompDoc.chercheClef(&sNumCompt,
												                      "",
                                              0,
                                              keySEARCHEQ,
                                              dbiWRITELOCK) ;

  if (DBIERR_NONE != dbCompDoc.lastError)
  {
    string sErreur = string("NSComptaPatient::DeleteComptDocu -> Erreur à la recherche de la fiche compt ") + sNumCompt ;
    erreur(sErreur.c_str(), standardError, dbCompDoc.lastError, pContexte->GetMainWindow()->GetHandle()) ;
    pContexte->getSuperviseur()->trace(&sErreur, 1, NSSuper::trError) ;
    erreur("Erreur à la recherche de la fiche compt.", standardError, dbCompDoc.lastError) ;
    dbCompDoc.close() ;
    return false ;
  }

  while (DBIERR_NONE == dbCompDoc.lastError)
  {
    dbCompDoc.lastError = dbCompDoc.getRecord() ;

    if (DBIERR_NONE != dbCompDoc.lastError)    {
      string sErreur = string("Erreur à la lecture de la table nsm_cp_docu.db.") ;
      erreur(sErreur.c_str(), standardError, dbCompDoc.lastError, pContexte->GetMainWindow()->GetHandle()) ;
      pContexte->getSuperviseur()->trace(&sErreur, 1, NSSuper::trError) ;
      dbCompDoc.close() ;
      return false ;
    }

    // Is it the good record?
    //
    if ((string(dbCompDoc._Donnees.numcompt) == sNumCompt) &&
        (string(dbCompDoc._Donnees.patient)  == sPatId) &&
        (string(dbCompDoc._Donnees.document) == sDocId))
    {
      dbCompDoc.lastError = dbCompDoc.deleteRecord() ;
      if (DBIERR_NONE != dbCompDoc.lastError)
      {
        erreur("Erreur à la destruction de la fiche compt.", standardError, dbCompDoc.lastError) ;
        dbCompDoc.close() ;
        return false ;
      }
      dbCompDoc.close() ;
      return true ;
    }

    dbCompDoc.lastError = dbCompDoc.suivant(dbiWRITELOCK) ;
  }

  string sErreur = string("NSComptaPatient::DeleteComptDocu -> Erreur à la recherche de la fiche") ;
  erreur(sErreur.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
  pContexte->getSuperviseur()->trace(&sErreur, 1, NSSuper::trError) ;

  dbCompDoc.lastError = dbCompDoc.close() ;
  if (DBIERR_NONE != dbCompDoc.lastError)
    erreur("Erreur à la fermeture de la base Compt.db.", standardError, dbCompDoc.lastError) ;

  return true ;
}

// Removing all records from nsm_cp_docu for a given numCompt
//
bool
NSComptaPatient::DeleteAllComptDocu(string sNumCompt)
{
  if (string("") == sNumCompt)
    return false ;

  NSComptDocu dbCompDoc(pContexte->getSuperviseur()) ;

  dbCompDoc.lastError = dbCompDoc.open() ;
  if (DBIERR_NONE != dbCompDoc.lastError)
  {
    string sErreur = string("NSComptaPatient::DeleteComptDocu -> Erreur à l'ouverture de la table nsm_cp_docu.db.") ;
    erreur(sErreur.c_str(), standardError, dbCompDoc.lastError, pContexte->GetMainWindow()->GetHandle()) ;
    pContexte->getSuperviseur()->trace(&sErreur, 1, NSSuper::trError) ;
    return false ;
  }

  string sIndice = sNumCompt + string(CPTDOCU_INDICE_LEN, '0') ;

  while (DBIERR_NONE == dbCompDoc.lastError)
  {
    dbCompDoc.lastError = dbCompDoc.chercheClef(&sIndice,
												                        "",
                                                0,
                                                keySEARCHGEQ,
                                                dbiWRITELOCK) ;

    if (DBIERR_NONE != dbCompDoc.lastError)
    {
      dbCompDoc.close() ;
      return true ;
    }

    dbCompDoc.lastError = dbCompDoc.getRecord() ;

    if (DBIERR_NONE != dbCompDoc.lastError)    {
      string sErreur = string("Erreur à la lecture de la table nsm_cp_docu.db.") ;
      erreur(sErreur.c_str(), standardError, dbCompDoc.lastError, pContexte->GetMainWindow()->GetHandle()) ;
      pContexte->getSuperviseur()->trace(&sErreur, 1, NSSuper::trError) ;
      dbCompDoc.close() ;
      return false ;
    }

    // Is it the good record?
    //
    if (string(dbCompDoc._Donnees.numcompt) == sNumCompt)
    {
      dbCompDoc.lastError = dbCompDoc.deleteRecord() ;
      if (DBIERR_NONE != dbCompDoc.lastError)
      {
        erreur("Erreur à la destruction de la fiche compt.", standardError, dbCompDoc.lastError) ;
        dbCompDoc.close() ;
        return false ;
      }
    }
    else
      break ;
  }

  dbCompDoc.lastError = dbCompDoc.close() ;
  if (DBIERR_NONE != dbCompDoc.lastError)
    erreur("Erreur à la fermeture de la base Compt.db.", standardError, dbCompDoc.lastError) ;

  return true ;
}

voidNSComptaPatient::CmSituation()
{
	NSListComptDialog* pListComptDlg =
   	    new NSListComptDialog(pContexte->GetMainWindow(), pContexte, _pPatient) ;

	if ((pListComptDlg->bErreur) || (pListComptDlg->nbCompt == 0))	{
		delete pListComptDlg ;
		return ;
	}

	// on affiche le dialogue de situation	if ((pListComptDlg->Execute()) == IDCANCEL)
	{
		delete pListComptDlg ;
		return ;
	}
	delete pListComptDlg;
}

void
NSComptaPatient::Export(string sAction, string sNumCompt, string sPatientId)
{
  if ((string("") == sAction) || (string("") == sNumCompt))
    return ;

  string sExportDll = pContexte->getSuperviseur()->getAccountingExportDll() ;

  if (string("") == sExportDll)
    return ;

  TModule* pExportModule = new TModule(sExportDll.c_str(), TRUE) ;
  if (NULL == pExportModule)
  {
    string sErreur = pContexte->getSuperviseur()->getText("modulesManagement", "cannotLoadDll") + string(" ") + sExportDll ;
    erreur(sErreur.c_str(), standardError, 0) ;
    pContexte->getSuperviseur()->trace(&sErreur, 1, NSSuper::trError) ;
    return ;
  }

  bool (FAR *pAdrFctExport) (NSContexte far *, string far *, string far *, string far *) ;
  (FARPROC) pAdrFctExport = pExportModule->GetProcAddress(MAKEINTRESOURCE(1)) ;
  if (NULL == pAdrFctExport)
  {
    delete pExportModule ;
    return ;
  }

  if ((string("") == sPatientId) && (NULL != _pPatient))
    sPatientId = _pPatient->getNss() ;

  if (string("") == sPatientId)
    return ;

  bool bSuccess = ((*pAdrFctExport)((NSContexte far *) pContexte, (string far *) &sAction, (string far *) &sNumCompt, (string far *) &sPatientId)) ;
  if (false == bSuccess)
  {
    delete pExportModule ;
    return ;
  }

  delete pExportModule ;
}

// fin de nsadmiwd.cpp
