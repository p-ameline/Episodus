
#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus\superLus.h"
  #include "mysql\mysql.h"
  #include "enterpriseLus\nsdivfctForCgi.h"
#else
  #include "partage\nsdivfct.h"
  #include "nautilus\nssuper.h"
  #include "partage\nsmatfic.h"
#endif

#include "dcodeur\decoder.h"

#include "partage\nscim10.h"
#include "nssavoir\nspatho.h"
#include "nssavoir\nsfilgd.h"
#include "nssavoir\nsguide.h"
#include "dcodeur\nsphrase.h"
#include "dcodeur\nsphrobj.h"
#include "dcodeur\nsgenlan.h"

GlobalDkd::GlobalDkd(NSContexte* pCtx, string langue, string sChem,
                     NSPatPathoArray* pSPP, ClasseStringVector* pCSV)
          :decodageBase(pCtx, langue)
{
  sChemin       = sChem ;
  pSousPatPatho = pSPP ;
  pVect         = pCSV ;

  sLangue       = langue ;
}

bool
GlobalDkd::decode(int precision)
{
  setPPtArray(pSousPatPatho) ;

  if (decodeSpecifique(precision))
    return true ;

  return decodeGenerique(precision) ;
}

bool
GlobalDkd::decodeSpecifique(int precision)
{
  if (pSousPatPatho->empty())
    return false ;

  string sTexte = "" ;
  char   cTexte[256] ;

  NSSuper* pSuper = pContexte->getSuperviseur() ;

#ifndef _ENTERPRISE_DLL
  if (sLangue == "fr")
  {
        sTexte = "Début du décodage spécifique";
    }
    else if (sLangue == "en")
    {
        sTexte = "Starting specific langage generation";
    }
    strcpy(cTexte, sTexte.c_str());
    pSuper->afficheStatusMessage(cTexte);
#endif

  // Fabrication du vecteur des "fils" à chercher (sur le modèle des
  // fils guides) ; ici il est réduit à un seul élément : la racine de
  // pSousPatPatho
  //
  VecteurRechercheSelonCritere aVecteurSelonCritere(DECODE) ;
  //
  // Recherche de l'Etiquette de la racine de pSousPatPatho
  //
  // On traite le cas simple d'un élément seul sur sa ligne
  //
  PatPathoIter i = pSousPatPatho->begin() ;
  string sEtiquette = (*i)->getLexique() ;
  string sPluriel   = (*i)->getPluriel() ;
  string sCertitude = (*i)->getCertitude() ;

  if (!(sPluriel == string("")))
    sEtiquette += string(1, cheminSeparationMARK) + sPluriel ;

  if (!(sCertitude == string("")))
    sEtiquette += string(1, cheminSeparationMARK) + sCertitude ;
  //
  // On ajoute l'Etiquette au vecteur de recherche
  //
  string sCodeSens ;
  NSDico::donneCodeSens(&sEtiquette, &sCodeSens) ;
  aVecteurSelonCritere.AjouteEtiquette(sCodeSens) ;
  //
  // Recherche d'un décodage compatible
  //
#ifndef _ENTERPRISE_DLL
    if (sLangue == "fr")
    {
      sTexte = "Recherche du chemin";
    }
    else if (sLangue == "en")
    {
        sTexte = "Searching path";
    }
    strcpy(cTexte, sTexte.c_str());
    pSuper->afficheStatusMessage(cTexte);
#endif
  pSuper->getFilDecode()->chercheChemin(&sChemin,
                             &aVecteurSelonCritere, NSFilGuide::compReseau) ;
  //
  bool trouve;
  BBDecodeData decodageData;

  aVecteurSelonCritere.SetData(sCodeSens, &trouve, &decodageData) ;

  if (!trouve)
    return false ;

  NSCRPhraseArray PhrasesDecode ;

#ifndef _ENTERPRISE_DLL
  //
  // Ouverture de la DLL de décodage
  //
  if (sLangue == "fr")
  {
        sTexte = "Ouverture de la DLL";
    }
    else if (sLangue == "en")
    {
        sTexte = "DLL opening";
    }
    strcpy(cTexte, sTexte.c_str());
    pSuper->afficheStatusMessage(cTexte);
    string sLib      = string(decodageData.fichier);
    string sFonction = string(decodageData.nomFonction);
    if ((sLib == "") || (sFonction == ""))
        return false;

    string sNomDLL = sLib + string(".DLL");
    TModule* pDCModule = new TModule(sNomDLL.c_str(), true);
    if (!pDCModule)
    {
        if (sLangue == "fr")
        {
            sTexte = "La DLL spécifique est introuvable (" + sNomDLL + ")";
        }
        else if (sLangue == "en")
        {
            sTexte = "Unable to open DLL (" + sNomDLL + ")";
        }
        erreur(sTexte.c_str(), standardError, -1);
        return false;
    }

    //
    // Préparation des paramètres de décodage
    //
    bool (FAR *pAdresseFct) (char, NSPatPathoArray far *, char far *,
                             NSContexte far *, NSCRPhraseArray far *,
                             string far *, int);

    (FARPROC) pAdresseFct = pDCModule->GetProcAddress(MAKEINTRESOURCE(2));
    if (!pAdresseFct)
    {
        if (sLangue == "fr")
        {
            sTexte = "La DLL spécifique est endommagée (" + sNomDLL + ")";
        }
        else if (sLangue == "en")
        {
            sTexte = "Damaged DLL (" + sNomDLL + ")";
        }
        erreur(sTexte.c_str(), standardError, -1);
        delete pDCModule;
        return false;
    }

    if (sLangue == "fr")
    {
        sTexte = "Elaboration du texte";
    }
    else if (sLangue == "en")
    {
        sTexte = "Text elaboration";
    }
    strcpy(cTexte, sTexte.c_str());
    pSuper->afficheStatusMessage(cTexte);



    bool Reussi = ((*pAdresseFct)
                   (' ',
                   (NSPatPathoArray far *) pSousPatPatho,
                   /*(char far *) sFichDecod.c_str()*/ "",
                   (NSContexte far *) pContexte,
                   (NSCRPhraseArray far *) &PhrasesDecode,
                   (string far *) &sFonction,
                   precision)
         		   );

    delete pDCModule;

    if (false == Reussi)
      return false ;

#endif

  //
  // Remplissage de pVect avec le contenu de pPhrases
  //
  if (PhrasesDecode.empty())
    return false ;

  //
  // Recopie des phrases du décodeur dans pNoyau->pPhrases
  //
  NSCRPhraseArray* pPhrases = getPhA() ;

  if ((NSCRPhraseArray*) NULL == pPhrases)
  {
    pPhrases = new NSCRPhraseArray() ;
    setPhA(pPhrases) ;
  }

  *pPhrases = PhrasesDecode ;

  setDcodeur(((*pPhrases)[0])->sTexte) ;

#ifndef _ENTERPRISE_DLL
  if (sLangue == "fr")
    sTexte = string("Décodage terminé") ;
  else if (sLangue == "en")
    sTexte = string("Text elaborated") ;
  strcpy(cTexte, sTexte.c_str()) ;
  pSuper->afficheStatusMessage(cTexte) ;
#endif

  return true ;
}

bool
GlobalDkd::decodeGenerique(int precision)
{
  setPPtArray(pSousPatPatho) ;
  initialiseIterateurs() ;

  int refCol = getCol() ;
  setDcodeur(string("")) ;

  if (-1 == refCol)
    return true ;

// -------------------------------------------------
// VERSION EXPERIMENTALE
// -------------------------------------------------

  while ((getCol() >= refCol) && iBon())
  {
    string sLex = getSt() ;

    if 	    (sLex[0] == 'G')
    {
      NSPhraseGeste Geste(this, precision, sLangue) ;
      Geste.ammorce() ;
      Avance() ;
      Geste.decode(refCol) ;
      if (iBon())
        Geste.metPhrase() ;
      if (iBon())
        return true ;
    }
    else if (sLex[0] == 'A')
    {
      NSPhraseOrgane Orga(this, precision, sLangue) ;
      Orga.ammorce() ;
      Avance() ;
      Orga.decode(refCol) ;
      if (iBon())
        Orga.metPhrase() ;
      if (iBon())
        return true ;
    }
    else if ((sLex[0] == 'P') || (sLex[0] == 'S'))
    {
      NSPhraseLesion Lesi(this, precision, sLangue) ;
      Lesi.ammorce() ;
      Avance() ;
      Lesi.decode(refCol) ;
      if (iBon())
        Lesi.metPhrase() ;
      if (iBon())
        return true ;
    }
    else if (sLex[0] == 'V')
    {
      NSPhraseBiometrie Chif(this, precision, sLangue) ;
      Chif.ammorce() ;
      Avance() ;
      Chif.decode(refCol) ;
      if (iBon())
        Chif.metPhrase() ;
      if (iBon())
        return true ;
    }
    else if (sLex[0] == 'K')
    {
      NSPhraseTemporel Temp(this, precision, sLangue) ;
      Temp.ammorce() ;
      Avance() ;
      Temp.decode(refCol) ;
      if (iBon())
        Temp.metPhrase() ;
      if (iBon())
        return true ;
    }
    else
      break ;
  }

	initialiseIterateurs() ;

// -------------------------------------------------
// FIN VERSION EXPERIMENTALE
// -------------------------------------------------

  string sCode = getStL() ;
  //
  // Si c'est un texte libre, on arrête tout
  //
  if (sCode[0] == '#')
  {
    setDcodeur(string("...")) ;
    return true ;
  }
  //
  // Fabrication du réseau de lots
  //
  lot Lot(pContexte) ;
  assembleLots(-1, &Lot) ;
  decodeLots(&Lot, "", genreMS) ;

  return true ;
}

void
GlobalDkd::assembleLots(int colonne, lot* pLotPere)
{
  if ((lot*) NULL == pLotPere)
    return ;

  string sLibelle = string("") ;

  set_iBon(1) ;

  int refCol = getCol() ;

  while ((getCol() > colonne) && iBon())
  {
    //
    // On met dans sTemp, l'ensemble des éléments d'une même ligne
    //
    //
    // On synchronise itDcodeTemp avec itDcode
    //
    PatPathoIter itDcodeTemp = getPPtArray()->begin() ;
    while ((getPPtArray()->end() != itDcodeTemp) &&
           ((*itDcodeTemp)->getLocalisation() != (*(getitDcode()))->getLocalisation()))
      itDcodeTemp++ ;

    if (getPPtArray()->end() == itDcodeTemp)
      return ;

    //
    // On remplit sTemp
    //
    string sTemp = (*itDcodeTemp)->getNodeLabel() ;
    //
    // Si c'est un texte libre, on arrête tout
    //
    string sCode = getStL() ;
    if ((sCode == "£?????") || (sCode[0] == '#'))
      return ;

    string sLibelle = "" ;
    //
    // On regarde si c'est une valeur numérique
    //
    gereNum num(pContexte->getSuperviseur(), sLangue) ;
    donneDimension(refCol-1, &num) ;
    if (iBon())
    {
      decodeNum(&sLibelle, &num) ;
      pLotPere->ajouteValeurChiffree(sLibelle) ;
    }
    else
    {
      set_iBon(1) ;
      //
      // On regarde si c'est une date
      //
      gereDate date(pContexte->getSuperviseur(), sLangue) ;
      donneDate(refCol-1, &date) ;
      if (iBon())
      {
        decodeDate(&sLibelle, &date) ;
        pLotPere->ajouteDate(sLibelle) ;
      }
      else
      {
        set_iBon(1) ;
        //
        // On regarde si c'est une heure
        //
        gereHeure heure(pContexte->getSuperviseur(), sLangue) ;
        donneHeure(refCol-1, &heure) ;
        if (iBon())
        {
          decodeHeure(&sLibelle, &heure) ;
          pLotPere->ajouteHeure(sLibelle) ;
        }
        else
        {
          set_iBon(1);
          refCol = getCol();
          LotIter itLot;
          //
          // Si ce n'est ni une date, ni une heure, ni une valeur
          // chiffrée, on classe en fonction du type
          //
          switch (sTemp[0])
          {
            // Connecteur
            case '0' :
              pLotPere->pLotsLies->push_back(new lot(pContexte)) ;
              itLot = pLotPere->pLotsLies->end() ;
              itLot-- ;
              (*itLot)->setLotPere(pLotPere) ;
              //(*itLot)->setConnection(sTemp);
              (*itLot)->setObjet(sTemp) ;
              Avance() ;
              assembleLots(refCol, *itLot) ;
              break ;
              // Lésion ou symptome ou malformation
              case 'S' :
              case 'P' :
              case 'F' :
              // Localisation anatomique
              case 'A' :
              // Geste ou traitement
              case 'G' :
              case 'N' :
                Avance() ;
                if (pLotPere->donneObjet() == "")
                {
                  pLotPere->setObjet(sTemp) ;
                  assembleLots(refCol, pLotPere) ;
                }
                else
                {
                  pLotPere->pLotsLies->push_back(new lot(pContexte)) ;
                  itLot = pLotPere->pLotsLies->end() ;
                  itLot-- ;
                  (*itLot)->setLotPere(pLotPere) ;
                  (*itLot)->setObjet(sTemp) ;
                  assembleLots(refCol, *itLot) ;
                }
                break ;
              default :
                Avance() ;

                size_t posit1 = sTemp.find(string(1, cheminSeparationMARK)) ;
                size_t posit2 = sTemp.find(string(1, nodeSeparationMARK)) ;
                size_t posit = min(posit1, posit2) ;
                string sLexique = string(sTemp, 0, posit) ;

                NSPathologData* pData = new NSPathologData;
                bool bFound = pContexte->getDico()->trouvePathologData(sLangue, &sLexique, pData);
                // Si le mot est un adjectif
                if (bFound && (pData->estNom()) || (pData->estVerbe()))
                {
                  pLotPere->pLotsLies->push_back(new lot(pContexte)) ;
                  itLot = pLotPere->pLotsLies->end() ;
                  itLot-- ;
                  (*itLot)->setLotPere(pLotPere) ;
                  (*itLot)->setObjet(sTemp) ;
                  assembleLots(refCol, *itLot) ;
                }
                else if (bFound && pData->estInvariable())
                {
                  pLotPere->pLotsLies->push_back(new lot(pContexte)) ;
                  itLot = pLotPere->pLotsLies->end() ;
                  itLot-- ;
                  (*itLot)->setLotPere(pLotPere) ;
                  (*itLot)->setConnection(sTemp) ;
                  assembleLots(refCol, *itLot) ;
                }
                else
                {
                  pLotPere->ajouteQualifiant(sTemp) ;
                  assembleLots(refCol, pLotPere) ;
                }
                break ;
          }
        }
      }
    }
  }
}

void
GlobalDkd::decodeLots(lot* pLot, string sObjetMaitre, GENRE iGenreMaitre)
{
  GENRE  iNewGenreMaitre = iGenreMaitre ;
  string sNewObjetMaitre = sObjetMaitre ;

  string sConnect ;
  GENRE  iGenreConnect ;
  int    iCertConnect ;

  if (string("") != pLot->sConnection)
  {
    if (string("") != sObjetMaitre)
      sConnect = decodeMot(&(pLot->sConnection), iGenreMaitre, &iGenreConnect, &iCertConnect) ;
    else
      sConnect = decodeMot(&(pLot->sConnection), genreNull, &iGenreConnect, &iCertConnect) ;

    if (string("") != getDcodeur())
      addToDcodeur(string(" ")) ;
    addToDcodeur(sConnect) ;
  }

  string sObj ;
  GENRE  iGenreObj ;
  int    iCertObj ;

  if (string("") != pLot->sObjet)
  {
    sObj = decodeMot(&(pLot->sObjet), genreMS, &iGenreObj, &iCertObj) ;
    if (string("") != getDcodeur())
      addToDcodeur(string(" ")) ;
    addToDcodeur(sObj) ;

    if (genreNull != iGenreObj)
    {
      iNewGenreMaitre = iGenreObj ;
      sNewObjetMaitre = sObj ;
    }
  }

  string sQualif ;
  if (string("") != pLot->sQualifiant)
  {
    sQualif = decodeGroupe(&(pLot->sQualifiant), iNewGenreMaitre) ;
    if (string("") != getDcodeur())
      addToDcodeur(string(" ")) ;
    addToDcodeur(sQualif) ;
  }

  string sChiffres ;
  if (string("") != pLot->sValeurChiffree)
  {
    sChiffres = decodeGroupeDecode(&(pLot->sValeurChiffree), iNewGenreMaitre) ;
    if (string("") != getDcodeur())
      addToDcodeur(string(" ")) ;
    addToDcodeur(sChiffres) ;
  }

  string sDates ;
  if (string("") != pLot->sDate)
  {
    sDates = decodeGroupeDecode(&(pLot->sDate), iNewGenreMaitre) ;
    if (string("") != getDcodeur())
      addToDcodeur(string(" ")) ;
    addToDcodeur(sDates) ;
  }

  string sHeures ;
  if (string("") != pLot->sHeure)
  {
    sHeures = decodeGroupeDecode(&(pLot->sHeure), iNewGenreMaitre) ;
    if (string("") != getDcodeur())
      addToDcodeur(string(" ")) ;
    addToDcodeur(sHeures) ;
  }

  // Traitement des lots liés
  //
  if (false == pLot->pLotsLies->empty())
    for (LotIter i = pLot->pLotsLies->begin(); i != pLot->pLotsLies->end(); i++)
      decodeLots(*i, sNewObjetMaitre, iNewGenreMaitre) ;
}

string
GlobalDkd::decodeMot(string* pCode, GENRE iGenre, GENRE* iGenreMot, int* iCertMot)
{
	string sLexique    = string("") ;
  string sComplement = string("") ;
  string sPluriel    = string("") ;
  string sCertitude  = string("") ;

	//
  // Séparation de la chaine en ses 4 composants
  //

  string sousChaine ;
	int    iIndex = 0 ;

  size_t debut = 0 ;
  size_t fin   = pCode->find(string(1, intranodeSeparationMARK)) ;

	while (NPOS != fin)
  {
  	if (fin > debut)
    	sousChaine = string(*pCode, debut, fin - debut) ;
    else
    	sousChaine = string("") ;

    if      ((strlen(sousChaine.c_str()) > 3) && (string(sousChaine, 0, 3) == string("WCE")))
      sCertitude = sousChaine ;
    else if ((strlen(sousChaine.c_str()) > 4) && (string(sousChaine, 0, 4) == string("WPLU")))
      sPluriel = sousChaine ;
    else if ((strlen(sousChaine.c_str()) > 1) && (string(sousChaine, 0, 4) == string("$")))
      sComplement = sousChaine ;
    else
    {
      switch (iIndex)
      {
    	  case 0 : sLexique    = sousChaine ; break ;
        case 1 : sComplement = sousChaine ; break ;
        case 2 : sPluriel    = sousChaine ; break ;
        case 3 : sCertitude  = sousChaine ; break ;
      }
    }
    iIndex++ ;
    debut = fin + 1 ;
    fin   = pCode->find(string(1, intranodeSeparationMARK), debut) ;
  }

  size_t iTaille = strlen(pCode->c_str()) ;
  if (iTaille > debut)
  	sousChaine = string(*pCode, debut, iTaille - debut) ;
  else
  	sousChaine = string("") ;

  if      ((strlen(sousChaine.c_str()) > 3) && (string(sousChaine, 0, 3) == string("WCE")))
    sCertitude = sousChaine ;
  else if ((strlen(sousChaine.c_str()) > 4) && (string(sousChaine, 0, 4) == string("WPLU")))
    sPluriel = sousChaine ;
  else if ((strlen(sousChaine.c_str()) > 1) && (string(sousChaine, 0, 4) == string("$")))
    sComplement = sousChaine ;
  else
  {
    switch (iIndex)
    {
  	  case 0 : sLexique    = sousChaine ; break ;
      case 1 : sComplement = sousChaine ; break ;
      case 2 : sPluriel    = sousChaine ; break ;
      case 3 : sCertitude  = sousChaine ; break ;
    }
  }

  //
  // Elaboration du mot
  //
  NSPathologData Data ;
  bool trouve = pContexte->getDico()->trouvePathologData(sLangue, &sLexique, &Data) ;
  if (false == trouve)
  	return string("") ;

  string sLabel = string("") ;
  //
  // Si le mot est un adjectif
  //
  if (Data.estAdjectif())
  {
  	_pGenerateur->donneLibelleAffiche(&sLabel, &Data, iGenre) ;
    //
    // Gestion de la certitude
    //
    int iCert = donneCertitude(sCertitude) ;
    sLabel = donneLibelleAdjectif(sLabel, iCert) ;

    *iGenreMot = genreNull ;
    *iCertMot  = -1 ;

    return sLabel ;
  }
  //
  // Si le mot est un nom
  //
  if (Data.estNom())
  {
  	GENRE iGenreNom ;
    Data.donneGenre(&iGenreNom) ;
    if (string("") != sPluriel)
    	Data.donneGenrePluriel(&iGenreNom) ;
    _pGenerateur->donneLibelleAffiche(&sLabel, &Data, iGenreNom) ;
    *iGenreMot = iGenreNom ;
    //
    // Gestion de la certitude
    //
    int iCert = donneCertitude(sCertitude) ;
    sLabel = donneLibelleNom(sLabel, iCert) ;
    *iCertMot = iCert ;
    return sLabel ;
  }

  _pGenerateur->donneLibelleAffiche(&sLabel, &Data, iGenre) ;

  *iGenreMot = genreNull ;
  *iCertMot  = -1 ;

	return sLabel ;
}

string
GlobalDkd::decodeGroupe(string* pCode, GENRE iGenre)
{
  string sLabel = string("") ;
  string sLab1  = string("") ;
  string sLab2  = string("") ;
  GENRE  iGen ;
  int    iCer ;

  string sousChaine ;
  size_t debut = 0 ;
  size_t fin   = pCode->find("||") ;

  while (fin != NPOS)
  {
    if (fin > debut)
      sousChaine = string(*pCode, debut, fin - debut) ;
    else
      sousChaine = "" ;

    sLab2 = decodeMot(&sousChaine, iGenre, &iGen, &iCer) ;
    etDuMilieu(&sLabel, &sLab1, &sLab2, ", ") ;

    debut = fin + 2 ;
    fin   = pCode->find("||", debut) ;
  }

  size_t iTaille = strlen(pCode->c_str()) ;
  if (iTaille > debut)
    sousChaine = string(*pCode, debut, iTaille - debut) ;
  else
    sousChaine = "" ;

  sLab2 = decodeMot(&sousChaine, iGenre, &iGen, &iCer) ;
  etDuMilieu(&sLabel, &sLab1, &sLab2, ", ") ;

  etFinal(&sLabel, &sLab1, " et ") ;
  return sLabel ;
}

string
GlobalDkd::decodeGroupeDecode(string* pCode, GENRE /* iGenre */)
{
  if ((string*) NULL == pCode)
    return string("") ;

  string sLabel = string("") ;
  string sLab1  = string("") ;
  string sLab2  = string("") ;

  string sousChaine ;
  size_t debut = 0;
  size_t fin   = pCode->find("|");

  while (NPOS != fin)
  {
    if (fin > debut)
      sLab2 = string(*pCode, debut, fin - debut) ;
    else
      sLab2 = string("") ;

    etDuMilieu(&sLabel, &sLab1, &sLab2, string(", ")) ;

    debut = fin + 1 ;
    fin   = pCode->find("|", debut) ;
  }

  size_t iTaille = strlen(pCode->c_str()) ;
  if (iTaille > debut)
    sLab2 = string(*pCode, debut, iTaille - debut) ;
  else
    sLab2 = string("") ;

  etDuMilieu(&sLabel, &sLab1, &sLab2, string(", ")) ;
  etFinal(&sLabel, &sLab1, string(" et ")) ;

  return sLabel ;
}

bool
GlobalDkd::decodeNum(string* sLibelle, gereNum* pNum)
{
  if (((gereNum*) NULL == pNum) || ((string*) NULL == sLibelle))
    return true ;

	if ((pNum->estExact()) || (pNum->estInf()) || (pNum->estSup()))
  {
  	bool bPluriel = false ;

    if (pNum->estExact())
    {
    	*sLibelle = pNum->getNum(pContexte, "") ;
      if (pNum->getValeur() > 1)
      	bPluriel = true ;
    }
    else
    {
      if ((pNum->estInf()) && (pNum->estSup()))
      {
        if      (string("fr") == sLangue)
          *sLibelle = string("entre ") + pNum->getNumInf() + string(" et ") + pNum->getNumSup() ;
        else if (string("en") == sLangue)
          *sLibelle = string("between ") + pNum->getNumInf() + string(" and ") + pNum->getNumSup() ;
      }
      else if (pNum->estInf())
      {
        if      (string("fr") == sLangue)
          *sLibelle = string("plus de ") + pNum->getNumInf() ;
        else if (string("en") == sLangue)
          *sLibelle = string("more than ") + pNum->getNumInf() ;
      }
      else if (pNum->estSup())
      {
        if      (string("fr") == sLangue)
          *sLibelle = string("moins de ") + pNum->getNumSup() ;
        else if (string("en") == sLangue)
          *sLibelle = string("less than ") + pNum->getNumSup() ;
      }
      if ((pNum->getValeurInf() > 1) || (pNum->getValeurSup() > 1))
        bPluriel = true;
    }
    //
    // Libellé de l'unité de l'unité
    //
    if (string("") != *sLibelle)
    {
      string sUniteLibel = pNum->donneLibelleUnite(pContexte) ;

      if (string("") != sUniteLibel)
        *sLibelle += string(" ") + sUniteLibel ;
    }
  }
  else if (string("") != pNum->donneLibelleUnite(pContexte))
    *sLibelle = string("(") + pNum->donneLibelleUnite(pContexte) + string(")") ;

  return true ;
}

bool
GlobalDkd::decodeDate(string* sLibelle, gereDate* pDate)
{
  if (((gereDate*) NULL == pDate) || ((string*) NULL == sLibelle))
    return false ;

  string sMessage, sIntro ;
  pDate->donne_date_breve(&sMessage, &sIntro) ;

  *sLibelle = sIntro + sMessage ;

  return true ;
}

bool
GlobalDkd::decodeHeure(string* sLibelle, gereHeure* pHeure)
{
  if (((gereHeure*) NULL == pHeure) || ((string*) NULL == sLibelle))
    return false ;

  string sMessage ;
  pHeure->donne_heure(&sMessage) ;

  *sLibelle = sMessage ;

  return true ;
}

//
// Décode uniquement le premier noeud
//
bool
GlobalDkd::decodeNoeud(string sCheminLocal, bool bClassifJustLabel)
{
	setPPtArray(pSousPatPatho) ;
	initialiseIterateurs() ;
	int refCol = getCol() ;
	NSSuper* pSuper = pContexte->getSuperviseur() ;

	string sLabel = string("") ;
	setDcodeur(string("")) ;

	//
	// Codes spéciaux : saisie libre
	//
	if (isChampLibre())
	{
		Avance() ;
    if ((false == iBon()) || (getCol() != refCol+1))
    	return false ;
	}

	//
	// Codes spéciaux : texte libre
	//
	if ((string("£??") == getSt()) || (string("£CL") == getSt()))
	{
		setDcodeur(getTexteLibre()) ;
    return true ;
	}

	//
	// Codes spéciaux : CDAM
	//
#ifndef _ENTERPRISE_DLL
	if (string("£CI") == getSt())
	{
		NSCim10Info CimInfo ;
    DBIResult DBIError = getCIM10(&CimInfo) ;
    if (DBIERR_NONE == DBIError)
    {
    	setDcodeur(string(CimInfo.Donnees.code) + string(" ")
            						+ string(CimInfo.Donnees.libelle)) ;
      return true ;
    }
	}
#endif

  //
	// Codes spéciaux : Personne
	//
#ifndef _ENTERPRISE_DLL
  NSPersonInfo personInfo(pSuper) ;
  bool bGotPerson = getCorresp(&personInfo) ;
  if (true == bGotPerson)
  {
    setDcodeur(personInfo.getNomLong()) ;
    return true ;
  }
#endif

  //
	// Codes spéciaux : Matériels
	//
#ifndef _ENTERPRISE_DLL
  NSMaterielInfo matInfo ;
  DBIResult iRes = getMateriel(&matInfo) ;
  if (string("") != matInfo.getFullLabel())
  {
    setDcodeur(matInfo.getFullLabel()) ;
    return true ;
  }
#endif

  //
	// Référentiel
	//
	if (string("£RE") == getSt())
	{
    setDcodeur(string("[") + getCpl() + string("]")) ;
    return true ;
	}

	//
	// On regarde si c'est une valeur numérique
	//
	gereNum num(pContexte->getSuperviseur(), sLangue) ;

	donneDimension(-1, &num) ;
	if (iBon())
	{
  	string sLibelle = string("") ;
    if (decodeNum(&sLibelle, &num))
    	setDcodeur(sLibelle) ;
    return true ;
	}
	set_iBon(true) ;

	//
  // On regarde si c'est une date
  //
  gereDate date(pContexte->getSuperviseur(), sLangue) ;

  donneDate(-1, &date) ;
  if (iBon())
  {
  	string sMessage, sIntro ;
    date.donne_date_breve(&sMessage, &sIntro) ;
    setDcodeur(sIntro + sMessage) ;
    return true ;
	}
  set_iBon(true) ;

	//
	// On regarde si c'est une date
	//
	gereHeure heure(pContexte->getSuperviseur(), sLangue) ;

	donneHeure(-1, &heure) ;
	if (iBon())
	{
  	string sMessage ;
    heure.donne_heure(&sMessage) ;
    setDcodeur(sMessage) ;
    return true ;
	}
	set_iBon(true) ;

	//
	// On décode le libelle
	//
	// string sCode = *getStL() ;
  string sCode = getLexique() ;

	NSPathologData Data ;
	bool trouve = pContexte->getDico()->trouvePathologData(sLangue, &sCode, &Data) ;

	if (false == trouve)
		setDcodeur(string("???")) ;

	GENRE iGenre ;

	//
	// On regarde si c'est un code de classification
	//
	gereCode codage(pContexte, sLangue) ;

	donneCode(-1, &codage);
	if (iBon())
	{
		string sMessage ;
    codage.donne_code(&sMessage) ;
    Data.donneGenre(&iGenre) ;
    _pGenerateur->donneLibelleAffiche(&sLabel, &Data, iGenre) ;

    if ((sCode == "6MGIT1") || bClassifJustLabel)
    {
    	codage.donne_code(&sMessage, false) ;
      setDcodeur(sMessage) ;
    }
    else
    {
    	codage.donne_code(&sMessage) ;
      Data.donneGenre(&iGenre) ;
      _pGenerateur->donneLibelleAffiche(&sLabel, &Data, iGenre) ;
      setDcodeur(sLabel + string(" : ") + sMessage) ;
    }
    return true ;
	}
	set_iBon(true) ;

	//
	// Si le mot est un adjectif
	//
	if (Data.estAdjectif())
	{
  	// Conjugaison de l'adjectif
    //
    // On cherche dans le chemin local le nom qui pilote cet adjectif
    //
    int    posit  = strlen(sCheminLocal.c_str()) ;
    bool   trouve = false ;
    string sBoutDeChemin = "" ;
    string sBoutPluriel  = "" ;
    string sBoutCertitud = "" ;
    NSPathologData BufData ;

    while ((posit > 0) && (!trouve))
    {
    	if (string("") != sBoutDeChemin)
      {
      	sBoutPluriel  = "" ;
        sBoutCertitud = "" ;
      }
      sBoutDeChemin = "" ;

      posit-- ;
      while ((posit >= 0) && (sCheminLocal[posit] != cheminSeparationMARK))
      {
      	sBoutDeChemin = sCheminLocal[posit] + sBoutDeChemin ;
        posit-- ;
      }

      if (string(sBoutDeChemin, 0, 3) == "WCE")
      {
      	sBoutCertitud = sBoutDeChemin ;
        sBoutDeChemin = "" ;
      }
      else if (string(sBoutDeChemin, 0, 4) == "WPLU")
      {
      	sBoutPluriel = sBoutDeChemin ;
        sBoutDeChemin = "" ;
      }
      else if (pContexte->getDico()->trouvePathologData(sLangue, &sBoutDeChemin, &BufData))
      {
      	if (BufData.estNom())
        {
        	GENRE iGenre ;
          BufData.donneGenre(&iGenre) ;

          if (string("") != sBoutPluriel)
          	BufData.donneGenrePluriel(&iGenre) ;

          _pGenerateur->donneLibelleAffiche(&sLabel, &Data, iGenre) ;
          trouve = true ;
        }
      }
    }

    if (false == trouve)
    	_pGenerateur->donneLibelleAffiche(&sLabel, &Data) ;

    //
    // Gestion de la certitude
    //
    string sCert ;
    getCert(&sCert) ;
    int iCert = donneCertitude(sCert) ;
    sLabel = donneLibelleAdjectif(sLabel, iCert) ;

    setDcodeur(sLabel) ;
    return true ;
	}

	//
	// Si le mot est un nom
	//
	if (Data.estNom())
	{
		Data.donneGenre(&iGenre) ;
    string sPlur ;
    getPlur(&sPlur) ;
    if (string("") != sPlur)
    	Data.donneGenrePluriel(&iGenre) ;

		_pGenerateur->donneLibelleAffiche(&sLabel, &Data, iGenre) ;

    //
    // Gestion de la certitude
    //
    string sCert ;
    getCert(&sCert) ;
    int iCert = donneCertitude(sCert) ;
    sLabel = donneLibelleNom(sLabel, iCert) ;

    setDcodeur(sLabel) ;
    return true ;
	}

	//
  // Si le mot n'est ni un nom ni un adjectif (invariable, adverbe...)
  //
  _pGenerateur->donneLibelleAffiche(&sLabel, &Data) ;

  if (string("") == sLabel)
    return true ;

	//
	// Gestion de la certitude
	//
	string sCert ;
	getCert(&sCert) ;
	int iCert = donneCertitude(sCert) ;
	sLabel = donneLibelleAutre(sLabel, iCert) ;

	setDcodeur(sLabel) ;

	return true ;
}

bool
GlobalDkd::CommenceParVoyelle(const string* pLib) const
{
	return _pGenerateur->CommenceParVoyelle(*pLib) ;
}

string
GlobalDkd::donneLibelleAdjectif(string sLabel, int iCertitude)
{
	if      (iCertitude == 0)
	{
		if      (sLangue == "fr")
    	sLabel = string("non ") + sLabel ;
    else if (sLangue == "en")
    	sLabel = string("not ") + sLabel ;
	}
  else if ((iCertitude > 0) && (iCertitude < 40)) //25%
  {
    if      (sLangue == "fr")
            sLabel = "probablement pas " + sLabel;
        else if (sLangue == "en")
            sLabel = "probably not " + sLabel;
    }
    else if ((iCertitude > 39) && (iCertitude < 60)) //50%
    {
        if      (sLangue == "fr")
            sLabel = "éventuellement " + sLabel;
        else if (sLangue == "en")
            sLabel = "maybe " + sLabel;
    }
    else if ((iCertitude > 59) && (iCertitude < 90)) //75%
    {
        if      (sLangue == "fr")
            sLabel = "probablement " + sLabel;
        else if (sLangue == "en")
            sLabel = "probably " + sLabel;
    }
    else if ((iCertitude > 89) && (iCertitude < 100)) //95%
    {
        if      (sLangue == "fr")
            sLabel = "typiquement " + sLabel;
        else if (sLangue == "en")
            sLabel = "typically " + sLabel;
    }
    else if (iCertitude == -1) // NC
        sLabel = sLabel + " (?)" ;
    return sLabel;
}

string
GlobalDkd::donneLibelleNom(string sLabel, int iCertitude)
{
    if    (iCertitude == 0)
    {
        if      (sLangue == "fr")
        {
            if (CommenceParVoyelle(&sLabel))
                sLabel = "absence d'" + sLabel;
            else
                sLabel = "absence de " + sLabel;
        }
        else if (sLangue == "en")
            sLabel = "no " + sLabel;
    }
    else if ((iCertitude > 0) && (iCertitude < 40)) //25%
    {
        if      (sLangue == "fr")
            sLabel = "improbable " + sLabel;
        else if (sLangue == "en")
            sLabel = "improbable " + sLabel;
    }
    else if ((iCertitude > 39) && (iCertitude < 60)) //50%
    {
        if      (sLangue == "fr")
            sLabel = "possible " + sLabel;
        else if (sLangue == "en")
            sLabel = "possible " + sLabel;
    }
    else if ((iCertitude > 59) && (iCertitude < 90)) //75%
    {
        if      (sLangue == "fr")
            sLabel = "probable " + sLabel;
        else if (sLangue == "en")
            sLabel = "probable " + sLabel;
    }
    else if ((iCertitude > 89) && (iCertitude < 100)) //95%
    {
        if      (sLangue == "fr")
        {
            if (CommenceParVoyelle(&sLabel))
                sLabel = "aspect typique d'" + sLabel;
            else
                sLabel = "aspect typique de " + sLabel;
        }
        else if (sLangue == "en")
            sLabel = "typical aspect of " + sLabel;
    }
    else if (iCertitude == -1) // NC
        sLabel = sLabel + string(" (?)");
    return sLabel;
}

string
GlobalDkd::donneLibelleAutre(string sLabel, int iCertitude)
{
    if    (iCertitude == 0)
    {
        if      (sLangue == "fr")
            sLabel = "pas par " + sLabel;
        else if (sLangue == "en")
            sLabel = "not by " + sLabel;
    }
    else if ((iCertitude > 0) && (iCertitude < 40)) //25%
    {
        if      (sLangue == "fr")
            sLabel = "probablement pas par " + sLabel;
        else if (sLangue == "en")
            sLabel = "probably not by " + sLabel;
    }
    else if ((iCertitude > 39) && (iCertitude < 60)) //50%
    {
        if      (sLangue == "fr")
            sLabel = "éventuellement par " + sLabel;
        else if (sLangue == "en")
            sLabel = "maybe by " + sLabel;
    }
    else if ((iCertitude > 59) && (iCertitude < 90)) //75%
    {
        if      (sLangue == "fr")
            sLabel = "si possible par " + sLabel;
        else if (sLangue == "en")
            sLabel = " " + sLabel;
    }
    else if ((iCertitude > 89) && (iCertitude < 100)) //95%
    {
        if      (sLangue == "fr")
            sLabel = "typiquement par " + sLabel;
        else if (sLangue == "en")
            sLabel = "typically by " + sLabel;
    }
    else if (iCertitude == -1) // NC
        sLabel = sLabel + string(" (?)");
    return sLabel;
}

// -------------------------------------------------------------------------
// ------------------------- METHODES DE lot -------------------------------
// -------------------------------------------------------------------------

lot::lot(NSContexte* pCtx)
    :NSRoot(pCtx)
{
    pLotPere    = 0;
    sConnection = "";

    sObjet = "";

    sDate           = "";
    sHeure          = "";
    sValeurChiffree = "";

    sQualifiant = "";

    pLotsLies = new NSlotArray(pCtx);
}

void
lot::ajouteDate(string sDateAjout)
{
    if (sDate != "")
        sDate += "|";
    sDate += sDateAjout;
}

void
lot::ajouteHeure(string sHeureAjout)
{
    if (sHeure != "")
        sHeure += "|";
    sHeure += sHeureAjout;
}

void
lot::ajouteValeurChiffree(string sValeurChiffreeAjout)
{
  if (string("") != sValeurChiffree)
    sValeurChiffree += "|" ;
  sValeurChiffree += sValeurChiffreeAjout ;
}

void
lot::ajouteQualifiant(string sQualifiantAjout)
{
  if (string("") != sQualifiant)
    sQualifiant += "||" ;
  sQualifiant += sQualifiantAjout ;
}

lot::lot(const lot& rv)
    :NSRoot(rv.pContexte)
{
  pLotPere    = rv.pLotPere;
  sConnection = rv.sConnection;

  sObjet = rv.sObjet;

  sDate           = rv.sDate;
  sHeure          = rv.sHeure;
  sValeurChiffree = rv.sValeurChiffree;

  sQualifiant = rv.sQualifiant;

  if (pLotsLies)
    delete pLotsLies ;
  pLotsLies = new NSlotArray(*(rv.pLotsLies));
}

lot::~lot()
{
  if (pLotsLies)
    delete pLotsLies ;
}

// -------------------------------------------------------------------------
// ---------------------- METHODES DE NSlotArray ---------------------------
// -------------------------------------------------------------------------
NSlotArray::NSlotArray(NSContexte* pCtx)
           :NSLotsArray(), NSRoot(pCtx)
{}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSlotArray::NSlotArray(const NSlotArray& rv)
           :NSLotsArray(), NSRoot(rv.pContexte)
{
  if (false == rv.empty())
    for (LotConstIter i = rv.begin() ; rv.end() != i ; i++)
      push_back(new lot(*(*i))) ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
void
NSlotArray::vider()
{
    if (empty())
        return;
    for (LotIter i = begin(); i != end(); )
    {
   	    delete *i;
        erase(i);
    }
}

NSlotArray::~NSlotArray()
{
    vider();
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSlotArray&
NSlotArray::operator=(const NSlotArray& src)
{
  //
  // Effacement des éléments déjà contenus dans le vecteur destination
  //
	vider() ;
	//
  // Copie et insertion des éléments de la source
  //
  if (false == src.empty())
    for (LotConstIter i = src.begin() ; src.end() != i ; i++)
      push_back(new lot(*(*i))) ;

  return *this ;
}

