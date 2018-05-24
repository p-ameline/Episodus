// -----------------------------------------------------------------------------// NsCaptur.cpp// -----------------------------------------------------------------------------
// Implémentation des méthodes de capture
// 1ère version : PA Septembre 2001    Dernière modif : Septembre 2001
// -----------------------------------------------------------------------------

#ifdef _ENTERPRISE_DLL
  #include "nsdivfctForCgi.h"
  #include "enterpriseLus\superLus.h"
#else
  #include "partage\nsdivfct.h"
  #include "nautilus\nssuper.h"
#endif

#include "nautilus\nsepicap.h"
#include "nsepisod\nsclasser.h"
#include "nssavoir\nscaptur.h"
#include "nssavoir\nssavoir.h"
#include "nssavoir\nsguide.h"
#include "nssavoir\nsfilgd.h"
#include "nssavoir\nspatbas.h"
#include "nsbb\nspatpat.h"
#include "nsbb\nsbbtran.h"
#include "nsbb\nsednum.h"
#include "nsdn\nsdocum.h"

#include "nssavoir\nssavoir.rh"

#ifndef _EXT_CAPTURE

long NSCapture::lObjectCount       = 0 ;
long NSCaptureArray::lObjectCount  = 0 ;
long analysedCapture::lObjectCount = 0 ;

NSCapture::NSCapture(NSContexte* pCtx)
          :NSRoot(pCtx)
{
  lObjectCount++ ;

  _sChemin         = string("") ;
  _sLibelle        = string("") ;
  _sUnit           = string("") ;
  _sClassifier     = string("") ;
  _sClassifResultO = string("") ;
  _sClassifResultP = string("") ;
  _sClassifResultI = string("") ;
  _sClassifResult3 = string("") ;
  _pOCRBloc        = (NSOCRbloc*) 0 ;
  _iCapturedFrom   = FromUnknown ;
}


NSCapture::NSCapture(NSContexte* pCtx, string sChem, string sLibel, CAPTUREFROM iFrom, string sClassif, string sUnite, string sDataDate)
          :NSRoot(pCtx)
{
  lObjectCount++ ;

try
{
  _sChemin         = sChem ;
  _sLibelle        = sLibel ;
  _sUnit           = sUnite ;
  _sDate           = sDataDate ;
  _iCapturedFrom   = iFrom ;

  _sClassifier     = sClassif ;
  _sClassifResultO = string("") ;
  _sClassifResultP = string("") ;
  _sClassifResultI = string("") ;
  _sClassifResult3 = string("") ;
  _pOCRBloc        = 0 ;

  if (string("") != _sChemin)
    return ;

/*
  ChoixCaptureContexte* pChercheDlg = new ChoixCaptureContexte(0, pContexte, &sChemin, &sClassifier, &sLibelle, true) ;
  pChercheDlg->Execute() ;
  delete pChercheDlg ;
*/
}
catch (...)
{
  erreur("Exception NSCapture ctor1.", standardError, 0) ;
}
}

NSCapture::NSCapture(NSContexte* pCtx, NSOCRbloc* pBloc)
          :NSRoot(pCtx)
{
  lObjectCount++ ;

try
{
  _pOCRBloc        = pBloc ;
  _sChemin         = string("") ;
  _sLibelle        = _pOCRBloc->sTexte ;
  _sUnit           = string("") ;
  _sClassifier     = string("") ;
  _sClassifResultO = string("") ;
  _sClassifResultP = string("") ;
  _sClassifResultI = string("") ;
  _sClassifResult3 = string("") ;
  _iCapturedFrom   = FromOutside ;

  if (string("") != _sChemin)
    return ;

  ChoixCaptureContexte* pChercheDlg = new ChoixCaptureContexte(0, pContexte, &_sChemin, &_sClassifier, &_sLibelle, true, _pOCRBloc) ;
  pChercheDlg->Execute() ;
  delete pChercheDlg ;
}
catch (...)
{
  erreur("Exception NSCapture ctor2.", standardError, 0) ;
}
}

NSCapture::NSCapture(const NSCapture& rv)          :NSRoot(rv.pContexte)
{
  lObjectCount++ ;

  _sChemin         = rv._sChemin ;
  _sLibelle        = rv._sLibelle ;
  _sUnit           = rv._sUnit ;
  _sClassifier     = rv._sClassifier ;
  _sClassifResultO = rv._sClassifResultO ;
  _sClassifResultP = rv._sClassifResultP ;
  _sClassifResultI = rv._sClassifResultI ;
  _sClassifResult3 = rv._sClassifResult3 ;
  _pOCRBloc        = rv._pOCRBloc ;
  _iCapturedFrom   = rv._iCapturedFrom ;
}

boolNSCapture::prendChemin()
{
  return false ;
}

NSCapture&
NSCapture::operator=(const NSCapture& src)
{
  if (this == &src)
    return (*this) ;

  _sChemin         = src._sChemin ;
  _sLibelle        = src._sLibelle ;
  _sUnit           = src._sUnit ;
  _sClassifier     = src._sClassifier ;
  _sClassifResultO = src._sClassifResultO ;
  _sClassifResultP = src._sClassifResultP ;
  _sClassifResultI = src._sClassifResultI ;
  _sClassifResult3 = src._sClassifResult3 ;
  _pOCRBloc        = src._pOCRBloc ;
  _iCapturedFrom   = src._iCapturedFrom ;

  return (*this) ;
}

NSCaptureArray::NSCaptureArray(NSContexte* pCtx)
               :NSCaptureVector(), NSRoot(pCtx)
{
  lObjectCount++ ;
}

// -----------------------------------------------------------------------------// Constructeur copie
// -----------------------------------------------------------------------------
NSCaptureArray::NSCaptureArray(const NSCaptureArray& rv)
               :NSCaptureVector(), NSRoot(rv.pContexte)
{
  lObjectCount++ ;

try
{
  if (false == rv.empty())
    for (CaptureCIter i = rv.begin() ; rv.end() != i ; i++)
      push_back(new NSCapture(*(*i))) ;
}
catch (...)
{
  erreur("Exception NSCaptureArray copy ctor.", standardError, 0) ;
}
}

int
NSCaptureArray::trouveChemin(string sChem, string* pLib, int pos)
{
  if ((string*) NULL == pLib)
    return string::npos ;

  *pLib = string("") ;

  if (empty())
    return string::npos ;

  int         i    = 0 ;
  CaptureIter iter = begin() ;

  // On avance jusqu'à pos
  // Going to "pos"
  for ( ; (i < pos) && (end() != iter) ; i++, iter++)
    ;
  if (end() == iter)
    return string::npos ;

  // On avance tant qu'on n'a pas trouvé le chemin voulu  // Step by step until we find the right path
  bool bChercher = true ;
  for ( ; (end() != iter) && bChercher ; i++)
  {
    if ((*iter)->getChemin().find(sChem) != string::npos)
      bChercher = false ;

    if (bChercher)
      iter++ ;
  }

  if (end() == iter)
    return string::npos ;

  *pLib = (*iter)->getLibelle() ;
  return i ;
}

NSCapture*
NSCaptureArray::trouveCheminExact(string sChem, string* pLib, NSCapture* pStartPoint)
{
  if ((string*) NULL == pLib)
    return (NSCapture*) 0 ;

  *pLib = string("") ;

  if ((string("") == sChem) || empty())
    return (NSCapture*) 0 ;

  string sChemSens ;
  NSDico::donneCodeSens(&sChem, &sChemSens) ;

  bool bStartPointFound = false ;
  if ((NSCapture*) NULL == pStartPoint)
    bStartPointFound = true ;

  for (CaptureIter iter = begin() ; end() != iter ; iter++)
  {
    if (false == bStartPointFound)
    {
      if (pStartPoint == *iter)
        bStartPointFound = true ;
    }
    else
    {
      string sChemCaptureSens ;
      string sPath = (*iter)->getChemin() ;
      NSDico::donneCodeSens(&sPath, &sChemCaptureSens) ;

      if (sChemCaptureSens == sChemSens)
      {
        *pLib = (*iter)->getLibelle() ;
        return *iter ;
      }
    }
  }

  return (NSCapture*) 0 ;
}

voidNSCaptureArray::ajouter(NSCapture* pNewCap)
{
  if ((NSCapture*) NULL == pNewCap)
    return ;

try
{
  bool bTrouve = false ;

  if ((string("") != pNewCap->_sClassifier) &&
      (string("") != pNewCap->_sClassifResultO) &&
      (string("") != pNewCap->_sClassifResultP) &&
      (string("") != pNewCap->_sClassifResultI))
  {
    // Recherche des codes correspondants au libelle
    // Searching of the codes related to Libelle
    ParseSOAP Parser(pContexte, pNewCap->getClassifier(), string("")) ;
    Parser.computeParsing(&(pNewCap->_sLibelle), &(pNewCap->_sChemin), &(pNewCap->_sClassifResultO), &(pNewCap->_sClassifResultP), &(pNewCap->_sClassifResultI), &(pNewCap->_sClassifResult3)) ;
  }
/*
  else
  {
    // Si ce n'est pas une donnée de classification, on s'assure qu'on ne
    // conserve qu'un seul exemplaire pour un chemin donné
    if (!(empty()))
    {
      for (CaptureIter iter = begin() ; (!bTrouve) && (iter != end()) ; iter++)
      {
        if (pNewCap->sChemin == (*iter)->sChemin)
        {
          **iter  = *pNewCap ;
          delete pNewCap ;
          pNewCap = 0 ;
          bTrouve = true ;
        }
      }
    }
  }
*/
  if (false == bTrouve)
    push_back(pNewCap) ;
}
catch (...)
{
  erreur("Exception NSCaptureArray::ajouter.", standardError, 0) ;
}
}

void
NSCaptureArray::eraseEntry(string sPath)
{
	if (empty() || (string("") == sPath))
		return ;

	for (CaptureIter captIter = begin() ; end() != captIter ; )
	{
  	if (sPath == (*captIter)->getChemin())
    {
    	delete (*captIter) ;
      erase(captIter) ;
		}
    else
    	captIter++ ;
  }
}

// -----------------------------------------------------------------------------
// Destructeur
// -----------------------------------------------------------------------------
void
NSCaptureArray::vider()
{
  if (false == empty())
    for (CaptureIter i = begin() ; end() != i ; )
    {
      delete *i;
      erase(i) ;
    }
}

void
NSCaptureArray::viderForOrigin(CAPTUREFROM iFrom)
{
  if (empty())
    return ;

  for (CaptureIter i = begin() ; end() != i ; )
    if ((*i)->getFrom() == iFrom)
    {
      delete *i ;
      erase(i) ;
    }
    else
      i++ ;
}

NSCaptureArray::~NSCaptureArray()
{
	vider() ;

  lObjectCount-- ;
}

void
NSCaptureArray::getIdentity(string &sNom, string &sPrenom, bool bUsePatronym)
{
  sNom    = string("") ;
  sPrenom = string("") ;

  trouveChemin(string("LNOM01"), &sNom) ;
  trouveChemin(string("LNOM21"), &sPrenom) ;

  if ((false == bUsePatronym) ||
      ((string("") != sNom) && (string("") != sPrenom)))
    return ;

  string sPatronyme = string("") ;
  trouveChemin(string("LPATR1"), &sPatronyme) ;
  if (string("") == sPatronyme)
    return ;

  size_t i ;
  //
  // Looking for the first letter that is not uppercase
  // Recherche de la première lettre non majuscule
  //
  for (i = 0; (i < strlen(sPatronyme.c_str())) && (pseumaj(sPatronyme[i]) == sPatronyme[i]) ; i++)
				;

  if (i >= strlen(sPatronyme.c_str()))
  {
    if (string("") == sNom)
      sNom = sPatronyme ;

    return ;
  }

  if (i > 0)
  {
    for (; (i > 0) && (sPatronyme[i] != ' '); i--)
      ;

    if (i > 0)
    {
      if (string("") == sNom)
        sNom    = string(sPatronyme, 0, i) ;
      if (string("") == sPrenom)
        sPrenom = string(sPatronyme, i+1, strlen(sPatronyme.c_str()) - i - 1) ;

      return ;
    }
  }

  if (string("") == sPrenom)
    sPrenom = sPatronyme ;
}

void
NSCaptureArray::getSejourInfo(NSSejourInfo *pSejour)
{
  if ((NSSejourInfo*) NULL == pSejour)
    return ;

  pSejour->metAZero() ;

  string sIEP       = string("") ;
  string sDateDeb   = string("") ;
  string sDateFin   = string("") ;
  string sUnit      = string("") ;
  string sCarePlace = string("") ;

  trouveChemin(string("LSEJO1/LNUSE1"), &sIEP) ;
  trouveChemin(string("LSEJO1/KOUVR1"), &sDateDeb) ;
  trouveChemin(string("LSEJO1/KFERM1"), &sDateFin) ;
  trouveChemin(string("LSEJO1/LUNIF1/0CODE1"), &sUnit) ;
  trouveChemin(string("LSEJO1/UETAB1/LFINE1"), &sCarePlace) ;
}

NSCaptureArray&
NSCaptureArray::operator=(const NSCaptureArray& src)
{
try
{
  if (this == &src)
    return (*this) ;

  vider() ;

  if (src.empty())
    return (*this) ;

  for (CaptureCIter CaptIter = src.begin() ; src.end() != CaptIter ; CaptIter++)
    push_back(new NSCapture(*(*CaptIter))) ;

  return (*this) ;
}
catch (...)
{
  erreur("Exception NSCaptureArray (=).", standardError, 0) ;
  return (*this) ;
}
}

void
NSCaptureArray::append(NSCaptureArray* pNewCapture)
{
  if (((NSCaptureArray*) NULL == pNewCapture) || pNewCapture->empty())
    return ;

  for (CaptureIter newIter = pNewCapture->begin() ; pNewCapture->end() != newIter ; newIter++)
  {
    bool bTrouve = false ;
    if (false == empty())
    {
      for (CaptureIter iter = begin() ; (false == bTrouve) && (end() != iter) ; iter++)
      {
        if ((*newIter)->getChemin() == (*iter)->getChemin())
        {
          **iter = **newIter ;
          bTrouve = true ;
        }
      }
    }
    if (false == bTrouve)
      push_back(new NSCapture(**newIter)) ;
  }
}

// -----------------------------------------------------------------------------//
// Méthodes de NSCaptureContexte
//
// -----------------------------------------------------------------------------

NSCaptureContexte::NSCaptureContexte(string sChem, string sLib, string sClassif)
{
  sChemin     = sChem ;
  sLibelle    = sLib ;
  sClassifier = sClassif ;
}

NSCaptureContexte::NSCaptureContexte(const NSCaptureContexte& rv)
{
  sChemin     = rv.sChemin ;
  sLibelle    = rv.sLibelle ;
  sClassifier = rv.sClassifier ;
}

NSCaptureContexte::~NSCaptureContexte()
{
}

NSCaptureContexte&
NSCaptureContexte::operator=(const NSCaptureContexte& src)
{
  if (&src == this)
    return (*this) ;

  sChemin     = src.sChemin ;
  sLibelle    = src.sLibelle ;
  sClassifier = src.sClassifier ;

  return (*this) ;
}

// -----------------------------------------------------------------------------
//
// Méthodes de NSCaptureCtxArray
//
// -----------------------------------------------------------------

// -----------------------------------------------------------------------------
// Constructeur copie
// -----------------------------------------------------------------------------
NSCaptureCtxArray::NSCaptureCtxArray(const NSCaptureCtxArray& rv)
                  :NSCaptCtxArray(),
                   NSRoot(rv.pContexte)
{
try
{
  if (false == empty())
    for (CIterCaptCtx i = rv.begin() ; rv.end() != i ; i++)
      push_back(new NSCaptureContexte(*(*i))) ;
}
catch (...)
{
  erreur("Exception NSCaptureCtxArray copy ctor.", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------
// Destructeur
// -----------------------------------------------------------------------------
NSCaptureCtxArray::~NSCaptureCtxArray()
{
	vider() ;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void
NSCaptureCtxArray::vider()
{
  if (empty())
    return ;

	for (IterCaptCtx i = begin() ; end() != i ; )
  {
    delete (*i) ;
    erase(i) ;
  }
}

boolNSCaptureCtxArray::charger()
{
try
{
  string sFichierTempo = pContexte->PathName("FPER") + string("capture.dat") ;

  ifstream    inFile ;
  inFile.open(sFichierTempo.c_str()) ;
  if (!inFile)
  {
    erreur("Erreur d'ouverture du fichier capture.dat.", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
    return false ;
  }

  string sLang = pContexte->getUserLanguage() ;

  while (!inFile.eof())
  {
    string sLine = string("") ;
    getline(inFile, sLine) ;

    if (string("") != sLine)
    {
      string sClassifier = string("") ;

      strip(sLine, stripBoth) ;
      size_t posBlanc = sLine.find(" ") ;
      if (string::npos != posBlanc)
      {
        sClassifier = string(sLine, posBlanc + 1, strlen(sLine.c_str()) - posBlanc - 1) ;
        sLine = string(sLine, 0, posBlanc) ;
      }

      string sLibelle = string("") ;
      string sLibelleTrouve ;
      posBlanc = sLine.find(" ") ;

      size_t fin   = sLine.find("/") ;
      size_t debut = 0 ;

      while (string::npos != fin)
      {
        string sousChaine = string(sLine, debut, fin - debut) ;
        pContexte->getDico()->donneLibelle(sLang, &sousChaine, &sLibelleTrouve) ;
        if (string("") != sLibelle)
          sLibelle += string("/") ;
        sLibelle += sLibelleTrouve ;

        debut = fin + 1 ;
        fin   = sLine.find("/", debut) ;
      }

      string endChaine = string(sLine, debut, strlen(sLine.c_str()) - debut) ;
      pContexte->getDico()->donneLibelle(sLang, &endChaine, &sLibelleTrouve) ;
      if (string("") != sLibelle)
        sLibelle += string("/") ;
      sLibelle += sLibelleTrouve ;

      push_back(new NSCaptureContexte(sLine, sLibelle, sClassifier)) ;
    }
  }

  inFile.close() ;
  return true ;
}
catch (...)
{
  erreur("Exception NSCaptureCtxArray::charger.", standardError, 0) ;
  return false ;
}
}
#endif // !_EXT_CAPTURE

// -----------------------------------------------------------------------------//// Méthodes de ChoixCaptureContexte//
// -----------------------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(ChoixCaptureContexte, TDialog)  EV_COMMAND(IDOK, CmOk),
	EV_CHILD_NOTIFY_AND_CODE(IDC_CAPTCTXBOX, LBN_SELCHANGE, CmSelectProto),
	EV_CHILD_NOTIFY_AND_CODE(IDC_CAPTCTXBOX, LBN_DBLCLK, 	CmProtoDblClk),
  EV_CHILD_NOTIFY(IDC_LEARN, BN_CLICKED, CmLearn),
END_RESPONSE_TABLE ;


ChoixCaptureContexte::ChoixCaptureContexte(TWindow* pere, NSContexte* pCtx, string* pChemin, string* pClassif, string* pLibel, bool bCanChange, NSOCRbloc* pBloc)
                     :TDialog(pere, "IDD_CAPTURE", pNSResModule),
                      NSRoot(pCtx),
#ifndef _EXT_CAPTURE
                      contexteArray(pCtx)#else
                      contexteArray()
#endif // _EXT_CAPTURE
{try
{
  pContexteBox    = new TListBox(this, IDC_CAPTCTXBOX) ;
  pEditLib        = new TEdit(this, IDC_LIBELLE) ;
  pCheminChoisi   = pChemin ;
  pClassifier     = pClassif ;
  pLibelle        = pLibel ;
  bCanChangeLib   = bCanChange ;
  pOCRBloc        = pBloc ;
}
catch (...)
{
  erreur("Exception ChoixCaptureContexte ctor.", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------
// Destructeur.
// -----------------------------------------------------------------------------
ChoixCaptureContexte::~ChoixCaptureContexte()
{
  delete pContexteBox ;
  delete pEditLib ;
}

// -----------------------------------------------------------------------------
// Initialisation
// -----------------------------------------------------------------------------
void
ChoixCaptureContexte::SetupWindow()
{
	TDialog::SetupWindow() ;

	// Remplissage de ContextBox avec les contextes possibles
#ifndef _EXT_CAPTURE
	contexteArray.charger() ;
#else
	contexteArray.charger("") ;
#endif

  if (false == contexteArray.empty())
    for (IterCaptCtx i = contexteArray.begin() ; contexteArray.end() != i ; i++)
      pContexteBox->AddString((*i)->sLibelle.c_str()) ;

  // Mise en place du libellé dans le contrôle Edit
  pEditLib->SetText(pLibelle->c_str()) ;
  if (false == bCanChangeLib)
    pEditLib->SetReadOnly(true) ;
}

// -----------------------------------------------------------------------------// Prend acte de la sélection du protocole// -----------------------------------------------------------------------------
void
ChoixCaptureContexte::CmSelectProto(WPARAM /* Cmd */)
{
  // Récupération de l'indice de la chemise sélectionnée
  iContexteChoisi = pContexteBox->GetSelIndex() ;

  if (false == contexteArray.empty())
  {
    IterCaptCtx i = contexteArray.begin() ;
    for (size_t j = 0 ; (contexteArray.end() != i) && (j < iContexteChoisi) ; i++, j++)
      ;
    if (contexteArray.end() != i)
    {
      *pCheminChoisi = (*i)->sChemin ;
      *pClassifier   = (*i)->sClassifier ;
    }
  }
}

// -----------------------------------------------------------------------------
// Sélectionne un protocole
// -----------------------------------------------------------------------------
void
ChoixCaptureContexte::CmProtoDblClk(WPARAM /* Cmd */)
{
  CmOk() ;
}

// -----------------------------------------------------------------------------// Annule ProtocoleChoisi et appelle Cancel()// -----------------------------------------------------------------------------
void
ChoixCaptureContexte::CmCancel()
{
  *pCheminChoisi  = string("") ;
  *pClassifier    = string("") ;

  iContexteChoisi = 0 ;
  TDialog::CmCancel() ;
}

void
ChoixCaptureContexte::CmOk()
{
  // Contexte sélectionné
  iContexteChoisi = pContexteBox->GetSelIndex() ;

  if (false == contexteArray.empty())
  {
    IterCaptCtx i = contexteArray.begin() ;
    for (size_t j = 0 ; (contexteArray.end() != i) && (j < iContexteChoisi) ; i++, j++)
      ;
    if (contexteArray.end() != i)
    {
      *pCheminChoisi = (*i)->sChemin ;
      *pClassifier   = (*i)->sClassifier ;
    }
  }

  // Prise en compte de l'éventuelle modification du libellé
  if ((bCanChangeLib) && (pEditLib->IsModified()))
  {
    int iLen = pEditLib->GetTextLen() ;
    char far* texte = new char[iLen + 1] ;
    pEditLib->GetText(texte, iLen + 1) ;
    *pLibelle = string(texte) ;
    delete[] texte ;
  }

  TDialog::CmOk() ;
}

voidChoixCaptureContexte::CmLearn(){
try
{
  if (NULL == pOCRBloc)
    return ;

  // Récupération de la Bitmap en cours
  NSEpisodus* pEpiso = pContexte->getEpisodus() ;
  if ((NSEpisodus*) NULL == pEpiso)
    return ;

  NSBitmap* pScreenMap = pEpiso->getScreenMap() ;

#ifndef _MUE
  // Chargement de la dll d'OCR / Loading the OCR dll
  TModule* pDCModule = new TModule("ns_ocr.dll", TRUE) ;
#else
  // Chargement de la dll d'OCR / Loading the OCR dll
  TModule* pDCModule = new TModule("nsm_ocr.dll", TRUE) ;
#endif // _MUE

  bool (FAR *pAdresseFct)(NSContexte far *, NSOCRbloc far *, NSBitmap far *) ;
  if ((TModule*) NULL == pDCModule)
  {
    erreur("Impossible d'ouvrir la DLL ns_ocr.dll.", standardError, -1, pContexte->GetMainWindow()->GetHandle()) ;
    pContexte->getSuperviseur()->afficheStatusMessage("") ;
    return ;
  }

  // Récupération du pointeur sur la fonction Learn
  // Getting Learn function's pointer
  (FARPROC) pAdresseFct = pDCModule->GetProcAddress(MAKEINTRESOURCE(1)) ;
  if (NULL == pAdresseFct)
  {
    delete pDCModule ;
    return ;
  }

  // Learning
  bool bReussi = ((*pAdresseFct)((NSContexte far *)pContexte, (NSOCRbloc far *)pOCRBloc, (NSBitmap far *)pScreenMap)) ;

  // OCR after learning
  bool bReussiOCR = false ;
  if (bReussi)
  {
    // Récupération du pointeur sur la fonction OCR
    // Getting OCR function's pointer
    (FARPROC) pAdresseFct = pDCModule->GetProcAddress(MAKEINTRESOURCE(2)) ;
    if (NULL == pAdresseFct)
    {
      delete pDCModule ;
      return ;
    }

    // OCR
    bReussiOCR = ((*pAdresseFct)((NSContexte far *)pContexte, (NSOCRbloc far *)pOCRBloc, (NSBitmap far *)pScreenMap)) ;
  }
  delete pDCModule ;

  if ((false == bReussiOCR) || (string("") == pOCRBloc->sTexte))
    return ;

  *pLibelle = pOCRBloc->sTexte ;

  // Mise en place du libellé dans le contrôle Edit
  pEditLib->SetText(pLibelle->c_str()) ;
}
catch (...)
{
  erreur("Exception ChoixCaptureContexte::CmLearn.", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------
//
// Méthodes de NSCaptureFormat
//
// -----------------------------------------------------------------------------

NSCaptureFormat::NSCaptureFormat(string sTit, string sExpres)
{
  sTitre      = sTit ;
  sExpression = sExpres ;
}

NSCaptureFormat::NSCaptureFormat(const NSCaptureFormat& rv)
{
  sTitre      = rv.sTitre ;
  sExpression = rv.sExpression ;
}

NSCaptureFormat::~NSCaptureFormat()
{
}

NSCaptureFormat&
NSCaptureFormat::operator=(const NSCaptureFormat& src)
{
  if (this == &src)
    return (*this) ;

  sTitre      = src.sTitre ;
  sExpression = src.sExpression ;

  return (*this) ;
}

// -----------------------------------------------------------------------------
//
// Méthodes de NSCaptureCtxArray
//
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Constructeur copie
// -----------------------------------------------------------------------------
NSCaptureFormatArray::NSCaptureFormatArray(const NSCaptureFormatArray& rv)
                     : NSCaptFmtArray(), NSRoot(rv.pContexte)
{
try
{
  if (false == empty())
    for (CIterCaptFmt i = rv.begin() ; rv.end() != i ; i++)
      push_back(new NSCaptureFormat(*(*i))) ;
}
catch (...)
{
  erreur("Exception NSCaptureFormatArray copy ctor.", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------// Destructeur// -----------------------------------------------------------------------------
NSCaptureFormatArray::~NSCaptureFormatArray()
{
	vider() ;
}

// -----------------------------------------------------------------------------
// Destructeur
// -----------------------------------------------------------------------------
void
NSCaptureFormatArray::vider()
{
  if (empty())
    return ;

	for (IterCaptFmt i = begin() ; end() != i ; )
  {
    delete (*i) ;
    erase(i) ;
  }
}

boolNSCaptureFormatArray::charger()
{
try
{
  ifstream    inFile ;

  string sLang = pContexte->getUserLanguage() ;

  string sFichierTempo = pContexte->PathName("FPER") + string("captfrmt.dat") ;
  inFile.open(sFichierTempo.c_str()) ;
  if (!inFile)
  {
    erreur("Erreur d'ouverture du fichier des formats de capture Episodus (captfrmt.dat).", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
    return false ;
  }

  size_t  fin ;
  size_t  debut ;

  string  sTitre ;
  string  sLine ;

  int     inumLigne = 0 ;

  while (!inFile.eof())
  {
    getline(inFile, sLine) ;
    inumLigne++ ;

    bool bContinuer = true ;
    if (sLine == "")
      bContinuer = false ;

    if (bContinuer)
    {
      strip(sLine, stripBoth) ;
      if (sLine == "")
        bContinuer = false ;
    }

    if (bContinuer)
    {
      sTitre  = "" ;
      debut = sLine.find("[");
      if ((debut == string::npos) || (debut == strlen(sLine.c_str()) - 1))
      {
        erreur("Le fichier des formats de capture Episodus (captfrmt.dat) est défectueux.", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
        bContinuer = false ;
      }
    }

    if (bContinuer)
    {
      fin = sLine.find("]", debut) ;
      if ((fin == string::npos) || (fin == strlen(sLine.c_str()) - 1))
      {
        erreur("Le fichier des formats de capture Episodus (captfrmt.dat) est défectueux.", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
        bContinuer = false ;
      }
    }

    if (bContinuer)
    {
      sTitre  = string(sLine, debut + 1, fin-debut - 1) ;
      sLine   = string(sLine, fin + 1, strlen(sLine.c_str()) - fin - 1) ;
    }
    if (bContinuer)    {
      string sLineOld = "" ;
      while (sLineOld != sLine)
      {
        sLineOld = sLine ;
        strip(sLine, stripBoth) ;
        strip(sLine, stripBoth, char('/t')) ;
      }
      if (sLine == "")
        bContinuer = false ;
    }

    if (bContinuer)
      push_back(new NSCaptureFormat(sTitre, sLine)) ;
  }

  inFile.close() ;
  return true ;
}
catch (...)
{
  erreur("Exception : il est impossible de charger les formats de capture Episodus.", standardError, 0) ;
  return false ;
}
}

string
NSCaptureFormatArray::getFormatExpression(string sFormatTitle)
{
  if ((string("") == sFormatTitle) || empty())
    return string("") ;

  for (IterCaptFmt iter = begin() ; end() != iter ; iter++)
    if ((*iter)->sTitre == sFormatTitle)
      return (*iter)->sExpression ;

  return string("") ;
}

// -----------------------------------------------------------------------------
// on cherche à partir d'un format référencé par sFormat à renvoyer par sText la
// chaîne telle qu'elle peut être traité directement par Nautilus
// pour les dates on doit renvoyer AAAAMMJJ
// -----------------------------------------------------------------------------
void
NSCaptureFormatArray::formatElement(string *psText, string sFormat, bool bForced)
{
  string sExpr = getFormatExpression(sFormat) ;
  if (string("") == sExpr)
  {
    *psText = string("") ;
    return ;
  }

  // Préparation de la chaîne
  preCompare(psText, &sFormat) ;

  format(psText, sExpr, bForced) ;
}

void
NSCaptureFormatArray::format(string *psText, string sExpr, bool bForced)
{
  if ((string*) NULL == psText)
    return ;

  if (string("") == sExpr)
  {
    *psText = string("") ;
    return ;
  }

try
{
  // Traitement en une passe : on progresse en même temps dans le texte et dans
  // le modèle
  size_t  iPosInText  = 0 ;
  size_t  iPosInExpr  = 0 ;
  bool    bContinue   = true ;

  string  sAnnee      = string("") ;
  string  sMois       = string("") ;
  string  sJour       = string("") ;
  string  sJourSem    = string("") ;
  string  sHeure      = string("") ;
  string  sMinute     = string("") ;
  string  sSeconde    = string("") ;
  string  sSexe       = string("") ;
  string  sEntier     = string("") ;
  string  sDecimal    = string("") ;

  while ((iPosInText < strlen(psText->c_str())) &&
         (iPosInExpr < strlen(sExpr.c_str())) && bContinue)
  {
    // Début d'un "champ"
    if ('$' == sExpr[iPosInExpr])
    {
      bool    bInSetOfValues = false ;
      iPosInExpr++ ;
      size_t iPosEndSet = sExpr.find("$", iPosInExpr) ;

      // on veut récupérer ce qui correspond à ce champ dans sText pour en constituer un format valide
      if (string::npos != iPosEndSet)
      {
        // Is there a defined set of values, for example $SS|M|F|$
        //
        if (sExpr.find("|", iPosInExpr) < iPosEndSet)
          bInSetOfValues = true ;

        // Cas où on a un ensemble prédéterminé de cas possibles
        if (bInSetOfValues)
        {
          // Type de la donnée
          size_t iFinLabel = sExpr.find("|", iPosInExpr) ;
          string sLabel = string(sExpr, iPosInExpr, iFinLabel-iPosInExpr) ;
          iPosInExpr = iFinLabel + 1 ;

          // Recherche du modèle qui correspond au texte
          bool    bContinue = true ;
          size_t  iPosSepar = sExpr.find("|", iPosInExpr) ;
          int     iNumModele = 1 ;
          string  sModel ;
          while ((iPosSepar != string::npos) && (iPosSepar < iPosEndSet) && bContinue)
          {
            string sModel = string(sExpr, iPosInExpr, iPosSepar - iPosInExpr) ;
            size_t iModelLength = strlen(sModel.c_str()) ;

            // Le texte est-il assez long pour contenir le modèle
            if (iPosInText + iModelLength <= strlen(psText->c_str()))
            {
              string sSubText = string(*psText, iPosInText, iModelLength) ;
              if (smartCompare(&sSubText, &sModel))
              {
                bContinue = false ;
                iPosInText += iModelLength ;
                iPosInExpr = iPosEndSet + 1 ;
              }
            }
            if (bContinue)            {
              // Il reste des modèles à tester
              if (iPosSepar + 1 < iPosEndSet)
              {
                iPosInExpr  = iPosSepar + 1 ;
                iPosSepar   = sExpr.find("|", iPosInExpr) ;
                if ((iPosSepar > iPosEndSet) || (iPosSepar == string::npos))
                  iPosSepar = iPosEndSet ;
                iNumModele++ ;
              }
              // Il ne reste plus de modèles à tester
              else
              {
                if (bForced)
                {
                  *psText = string("") ;
                  return ;
                }
                else
                {
                  iNumModele = 0 ;
                  bContinue = false ;
                }
              }
            }
          }

          // Traitement en fonction du type de la donnée

          // jour de la semaine (Lundi, Mardi...)
          if      (sLabel == "j")
          {
            // ignoré
          }
          // Jour calendaire (01, 02... 31)
          else if (string("JJ") == sLabel)
          {
            char    cJour[3] ;
            sprintf(cJour, "%d", iNumModele) ;            sJour = string(cJour) ;
          }
          // Mois (01, 02... 12)
          else if (string("MM") == sLabel)
          {
            char    cMois[3] ;
            sprintf(cMois, "%d", iNumModele) ;
            sMois = string(cMois) ;
          }
          // Année (1900...)
          else if (string("AAAA") == sLabel)
          {
            char    cAnnee[5] ;
            sprintf(cAnnee, "%d", iNumModele) ;
            sAnnee = string(cAnnee) ;
          }
          else if (string("SS") == sLabel)
          {
            switch (iNumModele)
            {
              case 1  : sSexe = "HMASC1" ;
                        break ;
              case 2  : sSexe = "HFEMI1" ;
                        break ;
              default : sSexe = "1INDE2" ;
            }
          }
        }
        // Ce n'est pas un ensemble prédéterminé de cas possibles
        else
        {
          string *pStringToFill = 0 ;

          switch (sExpr[iPosInExpr])
          {
            case 'j' : pStringToFill = &sJourSem ; break ;
            case 'J' : pStringToFill = &sJour ;    break ;
            case 'M' : pStringToFill = &sMois ;    break ;
            case 'A' : pStringToFill = &sAnnee ;   break ;
            case 'h' : pStringToFill = &sHeure ;   break ;
            case 'm' : pStringToFill = &sMinute ;  break ;
            case 's' : pStringToFill = &sSeconde ; break ;
            case 'S' : pStringToFill = &sSexe ;    break ;
            case 'U' : pStringToFill = &sEntier ;  break ;
            case 'd' : pStringToFill = &sDecimal ; break ;
          }

          bool bFixedSize = true ;
          if ((iPosInExpr + 1 < strlen(sExpr.c_str())) && ('+' == sExpr[iPosInExpr+1]))
          {
            bFixedSize = false ;
            iPosInExpr = iPosEndSet + 1 ;
          }

          if (bFixedSize)
            getNonBlankChars(pStringToFill, psText, iPosInText, iPosInExpr, iPosEndSet) ;
          else
            getMultipleNonBlankChars(pStringToFill, psText, iPosInText) ;
        }
      }
    }
    // Pas le début d'un champ
    else
    {
      if (sExpr[iPosInExpr] == (*psText)[iPosInText])
      {
        iPosInExpr++ ;
        iPosInText++ ;
      }
      else
      {
        *psText = string("") ;
        return ;
        // bContinue = false ;
      }
    }
  }
/*
  int iPosBegDollar = sExpr.find("$") ;
  int iPosEndDollar = string::npos ;
  if (iPosBegDollar + 1 < strlen(sExpr.c_str())
    iPosEndDollar = sExpr.find("$", iPosBegDollar + 1) ;

  bool    bSetOfValues = false ;
  if (iPosEndDollar != string::npos)
  {
    if (sExpr.find("|", iPosBegDollar) < iPosEndDollar)
      bSetOfValues = true ;
  }

  int iPosInText = iPosBegDollar ;
  while ((iPosInText < strlen(sText.c_str())) && (iPosBegDollar != string::npos) && (iPosEndDollar != string::npos))
  {
  }
*/

  *psText = string("") ;

  // on fait un champs valide si c'est une date valide :
  if ((string("") != sAnnee) && (string("") != sMois) && (string("") != sJour))
  {
    if (strlen(sMois.c_str()) < 2)
      sMois = "0" + sMois ;

    if (strlen(sJour.c_str()) < 2)
      sJour = "0" + sJour ;

    *psText = sAnnee + sMois + sJour ;
  }

  if (string("") != sHeure)
  {
    if (strlen(sHeure.c_str()) < 2)
      sHeure = string("0") + sHeure ;

    if (string("") != sMinute)
    {
      if (strlen(sMinute.c_str()) < 2)
        sMinute = string("0") + sMinute ;
    }
    else
      sMinute = string("00") ;

    if (string("") != sSeconde)
    {
      if (strlen(sSeconde.c_str()) < 2)
        sSeconde = string("0") + sSeconde ;
    }
    else
      sSeconde = string("00") ;

    *psText += sHeure + sMinute + sSeconde ;
  }

  if (string("") != sSexe)
    *psText = sSexe ;

  if ((string("") != sEntier) || (string("") != sDecimal))
  {
    *psText = sEntier ;
    if (string("") != sDecimal)
    {
      if (string("") != sEntier)
        *psText += string(".") + sDecimal ;
      else
        *psText = string("0.") + sDecimal ;
    }
  }
}
catch (...)
{
	erreur("Exception NSEpisodus::formatElement.", standardError, 0) ;
}
}

void
NSCaptureFormatArray::formatForDisplay(string *psText, string sExpr, string sDisplayFormat, bool bForced)
{
  if ((string*) NULL == psText)
    return ;

  if (string("") == sExpr)
  {
    *psText = string("") ;
    return ;
  }

try
{
  // This Map will contain field/value pairs of the kind $AAAA$/1963
  //
  std::map<std::string, std::string> _aFieldValues ;

  // Traitement en une passe : on progresse en même temps dans le texte et dans
  // le modèle
  size_t  iPosInText  = 0 ;
  size_t  iPosInExpr  = 0 ;
  bool    bContinue   = true ;

  while ((iPosInText < strlen(psText->c_str())) &&
         (iPosInExpr < strlen(sExpr.c_str())) && bContinue)
  {
    // Début d'un "champ"
    //
    if ('$' == sExpr[iPosInExpr])
    {
      bool bInSetOfValues = false ;
      iPosInExpr++ ;
      size_t iPosEndSet = sExpr.find("$", iPosInExpr) ;

      // on veut récupérer ce qui correspond à ce champ dans sText pour en constituer un format valide
      if (string::npos != iPosEndSet)
      {
        // Is there a defined set of values, for example $SS|M|F|$
        //
        if (sExpr.find("|", iPosInExpr) < iPosEndSet)
          bInSetOfValues = true ;

        // In case there is a defined set of values
        //
        if (bInSetOfValues)
        {
          // Type de la donnée
          size_t iFinLabel = sExpr.find("|", iPosInExpr) ;
          string sLabel = string(sExpr, iPosInExpr, iFinLabel-iPosInExpr) ;
          iPosInExpr = iFinLabel + 1 ;

          // Recherche du modèle qui correspond au texte
          bool    bContinue = true ;
          size_t  iPosSepar = sExpr.find("|", iPosInExpr) ;
          int     iNumModele = 1 ;
          string  sModel ;
          while ((iPosSepar != string::npos) && (iPosSepar < iPosEndSet) && bContinue)
          {
            string sModel = string(sExpr, iPosInExpr, iPosSepar - iPosInExpr) ;
            size_t iModelLength = strlen(sModel.c_str()) ;

            // Le texte est-il assez long pour contenir le modèle
            if (iPosInText + iModelLength <= strlen(psText->c_str()))
            {
              string sSubText = string(*psText, iPosInText, iModelLength) ;
              if (smartCompare(&sSubText, &sModel))
              {
                bContinue = false ;
                iPosInText += iModelLength ;
                iPosInExpr = iPosEndSet + 1 ;

                _aFieldValues[sLabel] = sSubText ;
              }
            }
            if (bContinue)            {
              // Il reste des modèles à tester
              if (iPosSepar + 1 < iPosEndSet)
              {
                iPosInExpr  = iPosSepar + 1 ;
                iPosSepar   = sExpr.find("|", iPosInExpr) ;
                if ((iPosSepar > iPosEndSet) || (iPosSepar == string::npos))
                  iPosSepar = iPosEndSet ;
                iNumModele++ ;
              }
              // Il ne reste plus de modèles à tester
              else
              {
                if (bForced)
                {
                  *psText = string("") ;
                  return ;
                }
                else
                {
                  iNumModele = 0 ;
                  bContinue = false ;
                }
              }
            }
          }
        }
        // Ce n'est pas un ensemble prédéterminé de cas possibles
        else
        {
          string sText  = string("") ;
          string sLabel = string(sExpr, iPosInExpr, iPosEndSet - iPosInExpr) ;

          bool bFixedSize = true ;
          if ((iPosInExpr + 1 < strlen(sExpr.c_str())) && ('+' == sExpr[iPosInExpr+1]))
          {
            bFixedSize = false ;
            iPosInExpr = iPosEndSet + 1 ;
          }

          if (bFixedSize)
            getNonBlankChars(&sText, psText, iPosInText, iPosInExpr, iPosEndSet) ;
          else
            getMultipleNonBlankChars(&sText, psText, iPosInText) ;

          _aFieldValues[sLabel] = sText ;
        }
      }
    }
    // Pas le début d'un champ
    else
    {
      if (sExpr[iPosInExpr] == (*psText)[iPosInText])
      {
        iPosInExpr++ ;
        iPosInText++ ;
      }
      else
      {
        *psText = string("") ;
        return ;
        // bContinue = false ;
      }
    }
  }

  if (_aFieldValues.empty())
  {
    *psText = string("") ;
    return ;
  }

  // Very simple
  //
  *psText = sDisplayFormat ;

  std::map<std::string, std::string>::iterator it = _aFieldValues.begin() ;
  for ( ; false == (_aFieldValues.end() == it) ; it++)
  {
    string sToFind = string("$") + (*it).first + string("$") ;

    size_t iPos = psText->find(sToFind) ;
    while (NPOS != iPos)
    {
      psText->replace(iPos, strlen(sToFind.c_str()), (*it).second) ;
      iPos = psText->find(sToFind, iPos + 1) ;
    }
  }
}
catch (...)
{
	erreur("Exception NSEpisodus::formatElement.", standardError, 0) ;
}
}

void
NSCaptureFormatArray::getNonBlankChars(string* psTarget, string* psText, size_t &iPosInText, size_t &iPosInExpr, size_t iPosEndSet)
{
  if (((string*) NULL == psTarget) || ((string*) NULL == psText))
    return ;

  while ((iPosInExpr < iPosEndSet) && ((*psText)[iPosInText]))
  {
    /* Previous code would enter an infinite loop if a blank was encoutered!
    if (' ' == (*psText)[iPosInText])
      iPosInExpr++ ;
    else
    {
      *psTarget += (*psText)[iPosInText] ;
      iPosInExpr++ ;
      iPosInText++ ;
    }
    */
    
    if (' ' != (*psText)[iPosInText])
      *psTarget += (*psText)[iPosInText] ;

    iPosInExpr++ ;
    iPosInText++ ;
  }
  
  iPosInExpr++ ;
}

void
NSCaptureFormatArray::getMultipleNonBlankChars(string* psTarget, string* psText, size_t &iPosInText)
{
  if (((string*) NULL == psTarget) || ((string*) NULL == psText))
    return ;

  size_t iTextLen = strlen(psText->c_str()) ;

  while ((iPosInText < iTextLen) && isdigit((*psText)[iPosInText]))
  {
    *psTarget += (*psText)[iPosInText] ;
    iPosInText++ ;
  }
}

void
NSCaptureFormatArray::preCompare(string * psText, string * /* psModel */)
{
  if (((string*) NULL == psText) || (string("") == *psText))
    return ;

  // Verrue Axilog : on enlève l'espace après les 1
  for (size_t i = 0 ; i < strlen(psText->c_str()) ; i++)
  {
    if (((*psText)[i] == '1') && (i < strlen(psText->c_str()) - 2))
    {
      if (((*psText)[i + 1] == ' ') && ((*psText)[i + 2] >= '0') && ((*psText)[i + 2] <= '9'))
        *psText = string(*psText, 0, i + 1) + string(*psText, i + 2, strlen(psText->c_str()) - i - 2) ;
    }
  }
}

bool
NSCaptureFormatArray::smartCompare(string* psText, string *psModel)
{
  if (((string*) NULL == psText) || ((string*) NULL == psModel))
    return false ;

  if (string("") == *psText)
  {
    if (string("") == *psModel)
      return true ;
    else
      return false ;
  }
  if (string("") == *psModel)
    return false ;

  if (strlen(psText->c_str()) != strlen(psModel->c_str()))
    return false ;

  for (size_t i = 0 ; i < strlen(psText->c_str()) ; i++)
  {
    if ((*psModel)[i] == 'I')
    {
      if (((*psText)[i] != 'I') && ((*psText)[i] != 'l'))
        return false ;
      (*psText)[i] = 'I' ;
    }
    else if ((*psModel)[i] == 'l')
    {
      if (((*psText)[i] != 'I') && ((*psText)[i] != 'l'))
        return false ;
      (*psText)[i] = 'l' ;
    }
    else if ((*psModel)[i] == '0')
    {
      if (((*psText)[i] != 'O') && ((*psText)[i] != '0'))
        return false ;
      (*psText)[i] = '0' ;
    }
    else if ((*psModel)[i] == 'O')
    {
      if (((*psText)[i] != 'O') && ((*psText)[i] != '0'))
        return false ;
      (*psText)[i] = 'O' ;
    }
    else if ((*psModel)[i] != (*psText)[i])
      return false ;
  }
  return true ;
}

// -----------------------------------------------------------------------------
//
//  Méthodes de ChoixCaptureContexte
//
// -----------------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(ChoixCaptureFormat, TDialog)
  EV_COMMAND(IDOK, CmOk),
	EV_CHILD_NOTIFY_AND_CODE(IDC_CAPTFMTBOX, LBN_SELCHANGE, CmSelectProto),
	EV_CHILD_NOTIFY_AND_CODE(IDC_CAPTFMTBOX, LBN_DBLCLK, 	CmProtoDblClk),
END_RESPONSE_TABLE ;

ChoixCaptureFormat::ChoixCaptureFormat(TWindow* pere, NSContexte* pCtx, string* pTitle, string* pModel)  : TDialog(pere, "IDD_CAPTFMT", pNSResModule),
    NSRoot(pCtx),
    formatArray(pCtx)
{
  pContexteBox    = new TListBox(this, IDC_CAPTFMTBOX) ;
	pTitre          = pTitle ;
  pModele         = pModel ;
}

// -----------------------------------------------------------------------------// Destructeur.// -----------------------------------------------------------------------------
ChoixCaptureFormat::~ChoixCaptureFormat()
{
  delete pContexteBox ;
}

// -----------------------------------------------------------------------------// Initialisation// -----------------------------------------------------------------------------
void
ChoixCaptureFormat::SetupWindow()
{
  TDialog::SetupWindow() ;

	// Remplissage de ContextBox avec les contextes possibles
  formatArray.charger();

  if (!(formatArray.empty()))
    for (IterCaptFmt i = formatArray.begin() ; i != formatArray.end() ; i++)
      pContexteBox->AddString((*i)->sTitre.c_str()) ;
}

// -----------------------------------------------------------------------------// Prend acte de la sélection du protocole// -----------------------------------------------------------------------------
void
ChoixCaptureFormat::CmSelectProto(WPARAM /* Cmd */)
{
	// Récupération de l'indice de la chemise sélectionnée
	iContexteChoisi = pContexteBox->GetSelIndex() ;

  if (!(formatArray.empty()))
  {
    IterCaptFmt i = formatArray.begin() ;
    for (size_t j = 0 ; (i != formatArray.end()) && (j < iContexteChoisi) ; i++, j++)
      ;
    if (i != formatArray.end())
    {
      *pTitre     = (*i)->sTitre ;
      *pModele    = (*i)->sExpression ;
    }
  }
}

// -----------------------------------------------------------------------------// Sélectionne un protocole// -----------------------------------------------------------------------------
void
ChoixCaptureFormat::CmProtoDblClk(WPARAM /* Cmd */)
{
  CmOk() ;
}

// -----------------------------------------------------------------------------// Annule ProtocoleChoisi et appelle Cancel()// -----------------------------------------------------------------------------
void
ChoixCaptureFormat::CmCancel()
{
  *pTitre     = "" ;
  *pModele    = "" ;
	iContexteChoisi = 0 ;	TDialog::CmCancel() ;
}

voidChoixCaptureFormat::CmOk(){
  // Contexte sélectionné
	iContexteChoisi = pContexteBox->GetSelIndex() ;
  if (!(formatArray.empty()))
  {
    IterCaptFmt i = formatArray.begin();
    for (size_t j = 0 ; (i != formatArray.end()) && (j < iContexteChoisi) ; i++, j++)
      ;
    if (i != formatArray.end())
    {
      *pTitre     = (*i)->sTitre ;
      *pModele    = (*i)->sExpression ;
    }
  }
	TDialog::CmOk() ;
}

// -----------------------------------------------------------------------------
//
// Classe ChoixCaptureUnit
//
// -----------------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(ChoixCaptureUnit, TDialog)
  EV_COMMAND(IDOK,	    CmOk),
  EV_COMMAND(IDCANCEL,    CmCancel),
END_RESPONSE_TABLE ;

ChoixCaptureUnit::ChoixCaptureUnit(TWindow* parent, NSContexte* pCtx, string* pTypeUnite)
                 :TDialog(parent, "UNIT_CHOICE", pNSResModule), NSRoot(pCtx)
{
  pType = new NSUtilLexique(pContexte, this, IDC_LIBELLE, pCtx->getDico()) ;
	pTypeUnit = pTypeUnite ;
}

ChoixCaptureUnit::~ChoixCaptureUnit()
{
  delete pType ;
}

void
ChoixCaptureUnit::SetupWindow()
{
	TDialog::SetupWindow() ;
}

void
ChoixCaptureUnit::CmOk()
{
	//ne pas accepter les textes libres
	if (pType->getCode() == string("£?????"))
	{
		erreur("Il faut choisir un code lexique et non pas du texte libre ", warningError, 0) ;
		pType->SetFocus() ;
		return ;
	}

	*pTypeUnit = pType->getCode() ;
	CloseWindow(IDOK) ;
}

void
ChoixCaptureUnit::CmCancel()
{
	*pTypeUnit = string("") ;
	Destroy(IDCANCEL) ;
}

// -----------------------------------------------------------------------------
//
// Classe ChoixCaptureModelName
//
// -----------------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(ChoixCaptureModelName, TDialog)
	EV_COMMAND(IDOK,	    CmOk),
	EV_COMMAND(IDCANCEL,    CmCancel),
END_RESPONSE_TABLE ;

ChoixCaptureModelName::ChoixCaptureModelName(TWindow* parent, NSContexte* pCtx, string* pName)
  : TDialog(parent, "NAME_CHOICE", pNSResModule),
    NSRoot(pCtx)
{
  pNameEdit   = new TEdit(this, IDC_LIBELLE) ;
	pNewName    = pName ;
}

ChoixCaptureModelName::~ChoixCaptureModelName()
{
  delete pNameEdit ;
}

void
ChoixCaptureModelName::SetupWindow()
{
	TDialog::SetupWindow() ;
}

void
ChoixCaptureModelName::CmOk()
{
  char szBuffer[256] ;
  int iTextSize = pNameEdit->GetText(szBuffer, 255) ;
  if (iTextSize <= 0)
  {
    ::MessageBox(GetHandle(), "Vous devez donner un nom au modèle", "", MB_OK) ;
    return ;
  }
  *pNewName = szBuffer ;
  CloseWindow(IDOK) ;
}

void
ChoixCaptureModelName::CmCancel()
{
	*pNewName = "" ;
	Destroy(IDCANCEL) ;
}

// -----------------------------------------------------------------------------
//
//                          Class analysedCapture
//
// -----------------------------------------------------------------------------

analysedCapture::analysedCapture(NSContexte *pCtx, string sText, NSCaptureArray* pCaptArray, CAPTUREFROM iFrom, string sPathCapt, string sClassif, string sUnite)
                :NSRoot(pCtx)
{
  lObjectCount++ ;

  _sTexte        = sText ;
  _sPath         = sPathCapt ;
	_sClassifier   = sClassif ;
	_sUnit         = sUnite ;
  _pBloc         = (NSOCRbloc*) 0 ;
  _pPere         = (NSCapture*) 0 ;
  _pFather       = (analysedCapture*) 0 ;
  _bDisplayed    = false ;
  _iCapturedFrom = iFrom ;

	if (pCaptArray)
  	_pCaptureArray = pCaptArray ;
  else
		_pCaptureArray = &(pContexte->getEpisodus()->CaptureArray) ;
}

analysedCapture::analysedCapture(NSContexte *pCtx, NSOCRbloc* pOcrBloc, NSCaptureArray* pCaptArray, CAPTUREFROM iFrom, string sPathCapt, string sClassif, string sUnite)
                :NSRoot(pCtx)
{
  lObjectCount++ ;

  _sPath         = sPathCapt ;
  _pBloc         = pOcrBloc ;
	_sClassifier   = sClassif ;
	_sUnit         = sUnite ;
  _sTexte        = _pBloc->sTexte ;
  _pPere         = (NSCapture*) 0 ;
  _pFather       = (analysedCapture*) 0 ;
  _bDisplayed    = false ;
  _iCapturedFrom = iFrom ;

	if (pCaptArray)
		_pCaptureArray = pCaptArray ;
	else
		_pCaptureArray = &(pContexte->getEpisodus()->CaptureArray) ;
}

analysedCapture::analysedCapture(NSContexte *pCtx, NSCaptureArray* pCaptArray, NSCapture* pCapture, analysedCapture* pCaptFather)
                :NSRoot(pCtx)
{
  lObjectCount++ ;

  _sPath         = pCapture->getChemin() ;
  _pBloc         = pCapture->_pOCRBloc ;
	_sClassifier   = pCapture->_sClassifier ;
	_sUnit         = pCapture->_sUnit ;
  _sTexte        = pCapture->getLibelle() ;
  _bDisplayed    = false ;
  _iCapturedFrom = pCapture->getFrom() ;

  _pPere         = new NSCapture(*pCapture) ;
  _pFather       = pCaptFather ;

	if (pCaptArray)
		_pCaptureArray = pCaptArray ;
	else
		_pCaptureArray = &(pContexte->getEpisodus()->CaptureArray) ;

	if (pCaptFather)
	{
		if (string("") != _sUnit)
		{
    	string sLocalPath = _sPath ;
    	size_t poslast = _sPath.find_last_of(cheminSeparationMARK) ;
      if (NPOS != poslast)
      	sLocalPath = string(_sPath, poslast + 1, strlen(_sPath.c_str())-poslast-1) ;

			string sLang = pContexte->getUserLanguage() ;

  		string sLibellePath = string("") ;
    	string sLibelleUnit = string("") ;

    	pContexte->getDico()->donneLibelle(sLang, &sLocalPath, &sLibellePath) ;
    	pContexte->getDico()->donneLibelle(sLang, &_sUnit, &sLibelleUnit) ;

    	_sTexte = sLibellePath + string(" ") + _sTexte + string(" ") + sLibelleUnit ;
		}
  	_sTexte = string("-> ") + _sTexte ;
	}
}

analysedCapture::analysedCapture(const analysedCapture& rv)
                :NSRoot(rv.pContexte)
{
  lObjectCount++ ;

  _sPath         = rv._sPath ;
  _sTexte        = rv._sTexte ;
  _aFlex         = rv._aFlex ;
  _pBloc         = rv._pBloc ;
	_sClassifier   = rv._sClassifier ;
	_sUnit         = rv._sUnit ;
  _iCapturedFrom = rv._iCapturedFrom ;
  _pCaptureArray = rv._pCaptureArray ;
  _pPere         = new NSCapture(*(rv._pPere)) ;

  _bDisplayed    = false ;
}

analysedCapture::~analysedCapture()
{
  lObjectCount-- ;

  if (_pPere)
    delete _pPere ;
}

NSCapture*
analysedCapture::analyse(analysedCapture** pSecondary)
{
try
{
	*pSecondary = (analysedCapture*) 0 ;

	if (string("") == _sTexte)
		return (NSCapture*) 0 ;

	string sLibelle = _sTexte ;

	string sLang = pContexte->getUserLanguage() ;

	if (string("") != _sUnit)
	{
		string sUnitCode = _sUnit ;
  	string sLibelleUnit = string("") ;
    if (strlen(sUnitCode.c_str()) == BASE_LEXI_LEN - 1)
    	sUnitCode = sUnitCode + string(1, '1') ;
		if (strlen(sUnitCode.c_str()) == BASE_LEXI_LEN)
    	pContexte->getDico()->donneLibelle(sLang, &sUnitCode, &sLibelleUnit) ;
    if (sLibelleUnit != "")
    	sLibelle += string(" ") + sLibelleUnit ;

		if (string("") != _sPath)
		{
    	string sConcept = string("") ;
      string sLibelleConcept = string("") ;
    	size_t posLast = _sPath.find_last_of("/") ;
  		if (posLast != string::npos)
      	sConcept = string(_sPath, posLast+1, strlen(_sPath.c_str())-posLast-1) ;
      else
      	sConcept = _sPath ;
      if (strlen(sConcept.c_str()) == BASE_LEXI_LEN - 1)
    		sConcept = sConcept + string(1, '1') ;
			if (strlen(sConcept.c_str()) == BASE_LEXI_LEN)
    		pContexte->getDico()->donneLibelle(sLang, &sConcept, &sLibelleConcept) ;
    	if (sLibelleConcept != "")
    		sLibelle = sLibelleConcept + string(" : ") + sLibelle ;
		}
	}
  else
	{
  	size_t posGcons = _sPath.find_last_of("GCONS") ;
		if (posGcons != string::npos)
    {
    	ParseElemArray aRawParseResult ;
      ParseElemArray aSemanticParseResult ;
      getSemanticParseElemArray(&aRawParseResult, &aSemanticParseResult) ;
      analysedCapture* pResult = findNumValues(&aRawParseResult, &aSemanticParseResult) ;
      *pSecondary = pResult ;
    }
	}

#ifndef _EXT_CAPTURE
	NSCapture* pCapt = new NSCapture(pContexte, _sPath, _sTexte, _iCapturedFrom, _sClassifier, _sUnit) ;
#else
	NSCapture* pCapt = new NSCapture(_sPath, _sTexte, _iCapturedFrom, _sClassifier, _sUnit) ;
#endif
	_pCaptureArray->ajouter(pCapt) ;

	// Attention : la fonction "ajouter" peut supprimer pCapt s'il doublonne un élément existant
	if (pCapt)
  {
		_pPere  = new NSCapture(*pCapt) ;
    _sTexte = sLibelle ;
  }

  return pCapt ;
}
catch (...)
{
	erreur("Exception analysedCapture::analyse", standardError, 0) ;
  return (NSCapture*) 0 ;
}
}

void
analysedCapture::getSemanticParseElemArray(ParseElemArray* pRawTermsArray, ParseElemArray* pSemTermsArray)
{
	if ((ParseElemArray*) NULL == pRawTermsArray)
		return ;

	pRawTermsArray->vider() ;

	ParseSOAP Soaper(pContexte, string(""), string("")) ;

	Soaper.initParseElemArray(&_sTexte, pRawTermsArray) ;

	if (((ParseElemArray*) NULL == pSemTermsArray) || (pRawTermsArray->empty()))
		return ;

  pSemTermsArray->vider() ;

	Soaper.initGroupElemArray(pRawTermsArray, pSemTermsArray) ;
	if (pSemTermsArray->empty())
		return ;

	Soaper.cleanGroupElemArray(pSemTermsArray) ;
}

analysedCapture*
analysedCapture::findNumValues(ParseElemArray* pRawTermsArray, ParseElemArray* pSemTermsArray)
{
  string sPattern = getNumPattern(pRawTermsArray, pSemTermsArray) ;

  return computeNumPattern(&sPattern) ;
}

string
analysedCapture::getNumPattern(ParseElemArray* pRawTermsArray, ParseElemArray* pSemTermsArray, bool* pbFullSuccess)
{
	if (((ParseElemArray*) NULL == pRawTermsArray) || (pRawTermsArray->empty()))
		return string("") ;

	if (((ParseElemArray*) NULL == pSemTermsArray) || (pSemTermsArray->empty()))
		return string("") ;

	ParseElemIter pSemIter = pSemTermsArray->begin() ;

  NSEpiFlechiesDB flechiesDB(pContexte) ;

	string sPattern = string("") ;

  bool bFullSuccess = true ;

	int Pos = 0 ;
	for (ParseElemIter pRefIter = pRawTermsArray->begin() ; pRawTermsArray->end() != pRefIter ; pRefIter++, Pos++)
	{
  	int iRefPos = (*pRefIter)->getPos() ;

  	// We keep pSemIter >= pRefIter
    //
    if (pSemTermsArray->end() != pSemIter)
		{
    	if (iRefPos > (*pSemIter)->getPos() + (*pSemIter)->getNbOfTerms() - 1)
      	pSemIter++ ;
		}
    //
    // if pSemIter pos = pRefIter pos, we use pSemIter
    //
    if ((pSemTermsArray->end() != pSemIter) && ((*pSemIter)->getPos() == iRefPos))
    {
    	string sSemContent = (*pSemIter)->getContent() ;
      // is it a unit ?
			//
  		string sResult = flechiesDB.getCodeLexiq(sSemContent, '2') ;
  		if (string("") != sResult)
				sPattern += string("[U:") + sResult + string("]") ;
      else
      {
  			// Now, is it a value ?
				//
  			sResult = flechiesDB.getCodeLexiq(sSemContent, 'V') ;
  			if (string("") != sResult)
					sPattern += string("[V:") + sResult + string("]") ;
        else
        	bFullSuccess = false ;
  		}
    }
    //
    // if pRefIter outside pSemIter, we check if it is a number
    //
    if ((pSemIter == pSemTermsArray->end()) ||
        (iRefPos < (*pSemIter)->getPos()) ||
        (iRefPos > (*pSemIter)->getPos() + (*pSemIter)->getNbOfTerms() - 1)
       )
		{
    	string sContent = (*pRefIter)->getContent() ;
      if (!addToNumPattern(&sPattern, &sContent))
      	bFullSuccess = false ;
		}
	}

  if (pbFullSuccess)
  	*pbFullSuccess = bFullSuccess ;

	return sPattern ;
}

string
analysedCapture::getElementByTagInNumPattern(string sPattern, char cTag)
{
	if (sPattern == string(""))
		return string("") ;

	string sSearchTag = string("[") + string(1, cTag) ;

  string sElement = string("") ;

	size_t iPos = sPattern.find(sSearchTag) ;
  if (iPos != NPOS)
  {
  	size_t iEndPos = sPattern.find(']', iPos) ;
    if ((iEndPos != NPOS) && (iEndPos > iPos + 4))
    {
    	string sInfo = string(sPattern, iPos + 1, iEndPos - iPos - 1) ;
      sElement = string(sInfo, 2, strlen(sInfo.c_str()) - 2) ;
    }
  }

  return sElement ;
}

void
analysedCapture::numPatternToTree(string* pPattern, NSPatPathoArray* pPPT)
{
	if (!pPattern || !pPPT)
		return ;

	if (*pPattern == string(""))
		return ;

  string sBuffPattern = *pPattern ;

  int iCol = 0 ;

  size_t iPos = sBuffPattern.find('[') ;
	while (iPos != NPOS)
	{
		size_t iEndPos = sBuffPattern.find(']', iPos) ;
		if ((iEndPos == NPOS) || (iEndPos < iPos + 4))
  		return ;

    string sInfo = string(sBuffPattern, iPos + 1, iEndPos - iPos - 1) ;

    if (sInfo[0] != 'N')
		{
    	string sLexiqueCode = string(sInfo, 2, strlen(sInfo.c_str())-2) ;
    	pPPT->ajoutePatho(sLexiqueCode, iCol) ;
    	iCol++ ;
    }
    else
    {
    	string sValue = string(sInfo, 2, strlen(sInfo.c_str())-2) ;

      Message theMessage ;
      theMessage.SetComplement(sValue) ;

      iPos = sBuffPattern.find('[', iEndPos) ;
      if ((iPos != NPOS) && (sBuffPattern[iPos+1] == 'U'))
      {
      	iEndPos = sBuffPattern.find(']', iPos) ;
        sInfo = string(sBuffPattern, iPos + 1, iEndPos - iPos - 1) ;
        string sUnit = string(sInfo, 2, strlen(sInfo.c_str())-2) ;
        theMessage.SetUnit(sUnit) ;
      }
      else
      	theMessage.SetUnit("200001") ;

      pPPT->ajoutePatho("£N0;03", &theMessage, iCol) ;
    }

    if (iEndPos < strlen(sBuffPattern.c_str()) - 1)
    {
    	sBuffPattern = string(sBuffPattern, iEndPos+1, strlen(sBuffPattern.c_str()) - iEndPos - 1) ;
      iPos = sBuffPattern.find('[') ;
    }
    else
      break ;
	}
}

bool
analysedCapture::addToNumPattern(string* pPattern, string* pLibelle)
{
	if (!pPattern || !pLibelle || (*pLibelle == string("")))
		return false ;

	string sValue = getEpisodusNumValueFromRawCapturedNum(*pLibelle) ;

	if (sValue != string(""))
  {
		*pPattern += string("[N:") + sValue + string("]") ;
    return true ;
  }

  return false ;
}

analysedCapture*
analysedCapture::computeNumPattern(string* pPattern)
{
	if (!pPattern || (*pPattern == string("")))
		return NULL ;

	string sPath  = "" ;
	string sValue = "" ;
	string sUnit  = "" ;

	getWaitingNumInfo(pPattern, &sPath, &sValue, &sUnit) ;
  analysedCapture* pResult = treatWaitingNumInfo(&sPath, &sValue, &sUnit) ;

  return pResult ;
}

void
analysedCapture::getWaitingNumInfo(string* pPattern, string* pPath, string* pValue, string* pUnit)
{
	string sBuffPattern = *pPattern ;

	*pPath  = "" ;
	*pValue = "" ;
	*pUnit  = "" ;

  size_t iPos = sBuffPattern.find('[') ;
	while (iPos != NPOS)
	{
		size_t iEndPos = sBuffPattern.find(']', iPos) ;
		if ((iEndPos == NPOS) || (iEndPos < iPos + 4))
  		return ;

    string sInfo = string(sBuffPattern, iPos + 1, iEndPos - iPos - 1) ;

    switch(sInfo[0])
		{
			case 'N' :
      	// There is already a value : treat waiting informations
      	if (string("") != *pValue)
        	treatWaitingNumInfo(pPath, pValue, pUnit) ;
      	*pValue = string(sInfo, 2, strlen(sInfo.c_str()) - 2) ;
				break ;
			case 'V' :
      	if (string("") != _sPath)
        	treatWaitingNumInfo(pPath, pValue, pUnit) ;
      	*pPath = string(sInfo, 2, strlen(sInfo.c_str()) - 2) ;
				break ;
			case 'U' :
      	if (string("") != _sUnit)
        	treatWaitingNumInfo(pPath, pValue, pUnit) ;
      	*pUnit = string(sInfo, 2, strlen(sInfo.c_str()) - 2) ;
				break ;
		}

    if (iEndPos < strlen(sBuffPattern.c_str()) - 1)
    {
    	sBuffPattern = string(sBuffPattern, iEndPos+1, strlen(sBuffPattern.c_str()) - iEndPos - 1) ;
      iPos = sBuffPattern.find('[') ;
    }
    else
      break ;
	}
}

analysedCapture*
analysedCapture::treatWaitingNumInfo(string* pPath, string* pValue, string* pUnit)
{
	if (!pPath || !pValue || !pUnit)
		return NULL ;

	analysedCapture* pResult = NULL ;

	if ((string("") != *pPath) && (string("") != *pValue))
	{
  	string sLocalPath = _sPath ;
  	if (sLocalPath != string(""))
    	sLocalPath += string(1, cheminSeparationMARK) ;
    sLocalPath += *pPath ;

		string sLang = pContexte->getUserLanguage() ;

  	string sLibellePath = string("") ;
    string sLibelleUnit = string("") ;

    pContexte->getDico()->donneLibelle(sLang, pPath, &sLibellePath) ;
    if (string("") != *pUnit)
    	pContexte->getDico()->donneLibelle(sLang, pUnit, &sLibelleUnit) ;

    string sTexte = string("-> ") + sLibellePath + string(" ") + *pValue ;
    if (string("") != sLibelleUnit)
    	sTexte += string(" ") + sLibelleUnit ;

		string sCodeUnit = *pUnit ;
    if (string("") == sCodeUnit)
    	sCodeUnit = "200001" ;

#ifndef _EXT_CAPTURE
		NSCapture* pCapt = new NSCapture(pContexte, sLocalPath, *pValue, FromOutside, "" /*sClassifier*/, sCodeUnit) ;
#else
		NSCapture* pCapt = new NSCapture(sLocalPath, *pValue, FromOutside, "" /*sClassifier*/, sCodeUnit) ;
#endif
		_pCaptureArray->ajouter(pCapt) ;

    // NSCaptDocument* pCaptDoc = pContexte->getSuperviseur()->getEpisodus()->getCaptureDoc() ;
    // pCaptDoc->aCaptures.push_back(new analysedCapture(pContexte, pCaptureArray, pCapt, this)) ;
		// pCaptDoc->refreshViewsNewLigne() ;

    pResult = new analysedCapture(pContexte, _pCaptureArray, pCapt, this) ;
	}

	*pPath  = string("") ;
  *pValue = string("") ;
  *pUnit  = string("") ;

	return pResult ;
}

void
analysedCapture::findPathologies(ParseElemArray* pRawTermsArray, ParseElemArray* pSemTermsArray)
{
}

analysedCapture&
analysedCapture::operator=(const analysedCapture& src)
{
  if (this == &src)
    return (*this) ;

  _sPath         = src._sPath ;
  _sTexte        = src._sTexte ;
  _aFlex         = src._aFlex ;
  _pBloc         = src._pBloc ;
	_sClassifier   = src._sClassifier ;
	_sUnit         = src._sUnit ;
  _iCapturedFrom = src._iCapturedFrom ;
  _pCaptureArray = src._pCaptureArray ;
  _pPere         = new NSCapture(*(src._pPere)) ;

  return (*this) ;
}

bool
analysedCapture::operator==(const analysedCapture& src)
{
  if ((_sTexte == src._sTexte) && (_sPath == src._sPath))
    return true ;
  else
    return false ;
}

// -----------------------------------------------------------------------------
//
// Class flexObject
//
// -----------------------------------------------------------------------------

flexObject::flexObject(string sLibel, int iStartPos, int iEndPos, string sCod)
{
  sLibelle        = sLibel ;
  sCode           = sCod ;
  iStartPosition  = iStartPos ;
  iEndPosition    = iEndPos ;
  pGenerated      = 0 ;
}

flexObject::flexObject(const flexObject& rv)
{
  sLibelle        = rv.sLibelle ;
  sCode           = rv.sCode ;
  iStartPosition  = rv.iStartPosition ;
  iEndPosition    = rv.iEndPosition ;
  pGenerated      = rv.pGenerated ;
}

flexObject::~flexObject()
{
}

flexObject&
flexObject::operator=(const flexObject& src)
{
  if (this == &src)
    return (*this) ;

  sLibelle        = src.sLibelle ;
  sCode           = src.sCode ;
  iStartPosition  = src.iStartPosition ;
  iEndPosition    = src.iEndPosition ;
  pGenerated      = src.pGenerated ;

  return (*this) ;
}

bool
flexObject::operator==(const flexObject& src)
{
  if ((sLibelle       == src.sLibelle)        &&
      (sCode          == src.sCode)           &&
      (iStartPosition == src.iStartPosition)  &&
      (iEndPosition   == src.iEndPosition))
    return true ;
  else
    return false ;
}

//
// *************** CAPTURE MODELS ***************
//

#ifndef _EXT_CAPTURE

// -----------------------------------------------------------------------------
//
// Class ChildEpisodusData
//
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Constructeur
// -----------------------------------------------------------------------------
EpisodusElemData::EpisodusElemData()
{
  AppWindow       = 0 ;
  AppWndChild     = 0 ;
  sClassChild     = string("") ;
  sTextChild      = string("") ;
  sCorrespondence = string("") ;
  sClassifier     = string("") ;
  sFormat         = string("") ;
  sUnit           = string("") ;
  iModeCapture    = captureSystem ;
  iModeAnalyse    = analyseDefault ;
  iOrder          = -1 ;
}

EpisodusElemData::EpisodusElemData( TWindow::THandle aWindow,
                                    TWindow::THandle aWndChild,
                                    string           aClassChild,
                                    string           aTextChild,
                                    ClassLib::TRect  aPlacement,
                                    int              iOrd,
                                    string           aCorrespondence,
                                    int              iModCapt,
                                    int              iModAnalys,
                                    string           sClassif,
                                    string           sFormatage,
                                    string           sUnite)
{
  AppWindow       = (unsigned long) aWindow ;
  AppWndChild     = (unsigned long) aWndChild ;
  sClassChild     = aClassChild ;
  sTextChild      = aTextChild ;
  sCorrespondence = aCorrespondence ;
  sClassifier     = sClassif ;
  sFormat         = sFormatage ;
  sUnit           = sUnite ;
  Placement       = aPlacement ;
  iModeCapture    = iModCapt ;
  iModeAnalyse    = iModAnalys ;
  iOrder          = iOrd ;
}

// -----------------------------------------------------------------------------
// Constructeur Copie
// -----------------------------------------------------------------------------
EpisodusElemData::EpisodusElemData(const EpisodusElemData& rv)
{
  AppWindow       = rv.AppWindow ;
  AppWndChild     = rv.AppWndChild ;
  sClassChild     = rv.sClassChild ;
  sTextChild      = rv.sTextChild ;
  sCorrespondence = rv.sCorrespondence ;
  sClassifier     = rv.sClassifier ;
  sFormat         = rv.sFormat ;
  sUnit           = rv.sUnit ;
  Placement       = rv.Placement ;
  iModeCapture    = rv.iModeCapture ;
  iModeAnalyse    = rv.iModeAnalyse ;
  iOrder          = rv.iOrder ;
}

EpisodusElemData::EpisodusElemData(EpisodusElemData *pNewEpisodusElemData)
{
  if (pNewEpisodusElemData)
  {
    AppWindow       = pNewEpisodusElemData->AppWindow ;
    AppWndChild     = pNewEpisodusElemData->AppWndChild ;
    sClassChild     = pNewEpisodusElemData->sClassChild ;
    sTextChild      = pNewEpisodusElemData->sTextChild ;
    sCorrespondence = pNewEpisodusElemData->sCorrespondence ;
    sClassifier     = pNewEpisodusElemData->sClassifier ;
    sFormat         = pNewEpisodusElemData->sFormat ;
    sUnit           = pNewEpisodusElemData->sUnit ;
    Placement       = pNewEpisodusElemData->Placement ;
    iModeCapture    = pNewEpisodusElemData->iModeCapture ;
    iModeAnalyse    = pNewEpisodusElemData->iModeAnalyse ;
    iOrder          = pNewEpisodusElemData->iOrder ;
  }
  else
  {
    AppWindow       = 0 ;
    AppWndChild     = 0 ;
    sClassChild     = string("") ;
    sTextChild      = string("") ;
    sCorrespondence = string("") ;
    sClassifier     = string("") ;
    sFormat         = string("") ;
    sUnit           = string("") ;
    iModeCapture    = captureSystem ;
    iModeAnalyse    = analyseDefault ;
    iOrder          = -1 ;
  }
}

EpisodusElemData::EpisodusElemData(string sXMLBaliseContenu, int iGivenOrder)
{
  EpisodusModelXML ModelXML ;
  ModelXML.ImportModelXML(sXMLBaliseContenu) ;

  // Pré-init
  iOrder          = iGivenOrder ;
  iModeAnalyse    = analyseDefault ;

  if (!ModelXML.sArrayBalise.empty() && !ModelXML.sArrayBaliseContenu.empty())
  {
    vector<string>::iterator XMLModelBaliseIter         = ModelXML.sArrayBalise.begin() ;
    vector<string>::iterator XMLModelBaliseContenuIter  = ModelXML.sArrayBaliseContenu.begin() ;

    for ( ; XMLModelBaliseIter != ModelXML.sArrayBalise.end() ; XMLModelBaliseIter++)
    {
      if (XMLModelBaliseContenuIter != ModelXML.sArrayBaliseContenu.end())
      {
        if (LABEL_CAPTURE_COORDS == *XMLModelBaliseIter)
        {
          EpisodusModelXML CoordsXML ;
          CoordsXML.ImportModelXML(*XMLModelBaliseContenuIter) ;

          if (!CoordsXML.sArrayBalise.empty() && !CoordsXML.sArrayBaliseContenu.empty())
          {
            vector<string>::iterator XMLCoordsBaliseIter = CoordsXML.sArrayBalise.begin() ;
            vector<string>::iterator XMLCoordsBaliseContenuIter = CoordsXML.sArrayBaliseContenu.begin() ;

            for ( ; XMLCoordsBaliseIter != CoordsXML.sArrayBalise.end() ; XMLCoordsBaliseIter++)
            {
              if (XMLCoordsBaliseContenuIter != CoordsXML.sArrayBaliseContenu.end())
              {
                if      (LABEL_CAPTURE_COORDS_TOP    == *XMLCoordsBaliseIter)
                  Placement.top = atoi((*XMLCoordsBaliseContenuIter).c_str()) ;
                else if (LABEL_CAPTURE_COORDS_LEFT   == *XMLCoordsBaliseIter)
                  Placement.left = atoi((*XMLCoordsBaliseContenuIter).c_str()) ;
                else if (LABEL_CAPTURE_COORDS_RIGHT  == *XMLCoordsBaliseIter)
                  Placement.right = atoi((*XMLCoordsBaliseContenuIter).c_str()) ;
                else if (LABEL_CAPTURE_COORDS_BOTTOM == *XMLCoordsBaliseIter)
                  Placement.bottom = atoi((*XMLCoordsBaliseContenuIter).c_str()) ;
              }
              XMLCoordsBaliseContenuIter++ ;
            }
          }
        }
        if      (LABEL_CAPTURE_CLASS == *XMLModelBaliseIter)
          sClassChild = *XMLModelBaliseContenuIter ;
        else if (LABEL_CAPTURE_ORDER == *XMLModelBaliseIter)
          iOrder = atoi((*XMLModelBaliseContenuIter).c_str()) ;
        else if (LABEL_CAPTURE_LABEL == *XMLModelBaliseIter)
          sTextChild = *XMLModelBaliseContenuIter ;
        else if (LABEL_CAPTURE_CORRESPONDCE == *XMLModelBaliseIter)
          sCorrespondence = *XMLModelBaliseContenuIter ;
        else if (LABEL_CAPTURE_CAPTURE_TYPE == *XMLModelBaliseIter)
        {
          if      (LABEL_CAPTURE_CAPTURE_SYSTEM == *XMLModelBaliseContenuIter)
            iModeCapture = captureSystem ;
          else if (LABEL_CAPTURE_CAPTURE_OCR == *XMLModelBaliseContenuIter)
            iModeCapture = captureOCR ;
        }
        else if (LABEL_CAPTURE_ANALYSE_TYPE == *XMLModelBaliseIter)
        {
          if      (LABEL_CAPTURE_ANALYSE_DFLT   == *XMLModelBaliseContenuIter)
            iModeAnalyse = analyseDefault ;
          else if (LABEL_CAPTURE_ANALYSE_PLACMT == *XMLModelBaliseContenuIter)
            iModeAnalyse = analysePlacement ;
          else if (LABEL_CAPTURE_ANALYSE_ORDER  == *XMLModelBaliseContenuIter)
            iModeAnalyse = analyseOrder ;
          else if (LABEL_CAPTURE_ANALYSE_PANDO  == *XMLModelBaliseContenuIter)
            iModeAnalyse = analysePlacementAndOrder ;
          else if (LABEL_CAPTURE_ANALYSE_LIBEL  == *XMLModelBaliseContenuIter)
            iModeAnalyse = analyseLibelle ;
          else if (LABEL_CAPTURE_ANALYSE_REL_O  == *XMLModelBaliseContenuIter)
            iModeAnalyse = analyseRelativeOrder ;
          else if (LABEL_CAPTURE_ANALYSE_REL_P  == *XMLModelBaliseContenuIter)
            iModeAnalyse = analyseRelativePlacement ;
          else if (LABEL_CAPTURE_ANALYSE_NEVER  == *XMLModelBaliseContenuIter)
            iModeAnalyse = analyseNever ;
        }
        else if (LABEL_CAPTURE_CLASSIFY == *XMLModelBaliseIter)
          sClassifier = *XMLModelBaliseContenuIter ;
        else if (LABEL_CAPTURE_FORMAT   == *XMLModelBaliseIter)
          sFormat = *XMLModelBaliseContenuIter ;
        else if (LABEL_CAPTURE_UNIT     == *XMLModelBaliseIter)
          sUnit = *XMLModelBaliseContenuIter ;

        XMLModelBaliseContenuIter++ ;
      }
    }
  }
}

// -----------------------------------------------------------------------------
// Destructeur// -----------------------------------------------------------------------------
EpisodusElemData::~EpisodusElemData()
{
}

bool
EpisodusElemData::ImportModelXML(string sXMLBaliseContenu)
{
  return false ;
}
// -----------------------------------------------------------------------------
// comparaison avec un ChildEpisodusData
// -----------------------------------------------------------------------------
bool
EpisodusElemData::CompareWithElementModel(EpisodusElemData * pTheElementModel, int iDefaultAnalyse)
{
  if ((EpisodusElemData*) NULL == pTheElementModel)
    return false ;

  int _iAnalyse = pTheElementModel->iModeAnalyse ;
  if (pTheElementModel->iModeAnalyse == analyseDefault)
  {
    switch (iDefaultAnalyse)
    {
      case EpisodusData::byPlacement          : _iAnalyse = analysePlacement ;          break ;
      case EpisodusData::byOrder              : _iAnalyse = analyseOrder ;              break ;
      case EpisodusData::byPlacementAndOrder  : _iAnalyse = analysePlacementAndOrder ;  break ;
      default                                 : _iAnalyse = analysePlacement ;          break ;
    }
  }

  switch (_iAnalyse)
  {
    case analysePlacement :
      if ((pTheElementModel->Placement   == Placement) &&
          (pTheElementModel->sClassChild == sClassChild))
        return true ;
      break ;

    case analyseOrder :
      if ((pTheElementModel->iOrder      == iOrder)    &&
          (pTheElementModel->sClassChild == sClassChild))
        return true ;
      break ;

    case analysePlacementAndOrder :
      if ((pTheElementModel->Placement   == Placement) &&
          (pTheElementModel->iOrder      == iOrder)    &&
          (pTheElementModel->sClassChild == sClassChild))
        return true ;
      break ;

    case analyseLibelle :
      if ((pTheElementModel->sTextChild  == sTextChild) &&
          (pTheElementModel->sClassChild == sClassChild))
        return true ;
      break ;
  }
  return false ;
}

bool
EpisodusElemData::CompareWithRelativeElementModel(EpisodusElemData * pModel, EpisodusElemData * pRepereCapture, EpisodusElemData * pRepereModel)
{
  if ((!pModel) || (!pRepereCapture) || (!pRepereModel))
    return false ;

  if (pModel->sClassChild != sClassChild)
    return false ;

  if (pModel->iModeAnalyse == analyseRelativeOrder)
  {
    int iDeltaCaptOrder   = iOrder - pRepereCapture->iOrder ;
    int iDeltaModeleOrder = pModel->iOrder - pRepereModel->iOrder ;

    if (iDeltaCaptOrder == iDeltaModeleOrder)
      return true ;
    else
      return false ;
  }
  else if (pModel->iModeAnalyse == analyseRelativePlacement)
  {
    int iDeltaCaptX     = Placement.X() - (pRepereCapture->Placement.X() + pRepereCapture->Placement.Width()) ;
    int iDeltaModeleX   = pModel->Placement.X() - (pRepereModel->Placement.X() + pRepereModel->Placement.Width()) ;

    int iDeltaCaptY     = Placement.Y() - pRepereCapture->Placement.Y() ;
    int iDeltaModeleY   = pModel->Placement.Y() - pRepereModel->Placement.Y() ;

    if ((iDeltaCaptX == iDeltaModeleX) && (iDeltaCaptY == iDeltaModeleY))
        return true ;
    else
        return false ;
  }
  return false ;
}

// -----------------------------------------------------------------------------
//
// Classe EpisodusData
//
// -----------------------------------------------------------------------------
EpisodusData::EpisodusData()
{
  iModeAnalyse = byPlacement ;
}

EpisodusData::EpisodusData(const EpisodusData& rv)
{
try
{
  iModeAnalyse = rv.iModeAnalyse ;

  if (rv.empty())
    return ;

  for (EpisodusDataCIter EpisodusIter = rv.begin() ; EpisodusIter != rv.end() ; EpisodusIter++)
    push_back(new EpisodusElemData(*(*EpisodusIter))) ;
}
catch (...)
{
	erreur("Exception EpisodusData copy ctor.", standardError, 0) ;
}
}

EpisodusData&
EpisodusData::operator=(const EpisodusData& src)
{
	if (this == &src)
  	return *this ;

try
{
  iModeAnalyse = src.iModeAnalyse ;

  vider() ;
  if (src.empty())
    return (*this) ;

  for (EpisodusDataCIter EpisodusIter = src.begin() ; EpisodusIter != src.end() ; EpisodusIter++)
    push_back(new EpisodusElemData(*(*EpisodusIter))) ;

  return (*this) ;
}
catch (...)
{
  erreur("Exception EpisodusData = operator.", standardError, 0) ;
  return (*this) ;
}
}

EpisodusData::EpisodusData(EpisodusData *pNewEpisodusData)
{
try
{
  iModeAnalyse = pNewEpisodusData->iModeAnalyse ;

  if ((!pNewEpisodusData) || (pNewEpisodusData->empty()))
    return ;

  for (EpisodusDataIter EpisodusIter = pNewEpisodusData->begin() ; EpisodusIter != pNewEpisodusData->end() ; EpisodusIter++)
    push_back(new EpisodusElemData(*(*EpisodusIter))) ;
}
catch (...)
{
  erreur("Exception EpisodusData ctor(EpisodusData*).", standardError, 0) ;
}
}

EpisodusData::EpisodusData(string sXMLBaliseContenu){
try
{
  iModeAnalyse = byPlacement ;

  EpisodusModelXML ModelXML ;
  ModelXML.ImportModelXML(sXMLBaliseContenu) ;

  if (!ModelXML.sArrayBalise.empty() && !ModelXML.sArrayBaliseContenu.empty())
  {
    vector<string>::iterator XMLModelBaliseIter = ModelXML.sArrayBalise.begin() ;
    vector<string>::iterator XMLModelBaliseContenuIter = ModelXML.sArrayBaliseContenu.begin() ;

    int iOrder = 0 ;
    for ( ; (XMLModelBaliseIter != ModelXML.sArrayBalise.end()) && (XMLModelBaliseContenuIter != ModelXML.sArrayBaliseContenu.end()) ; XMLModelBaliseIter++, XMLModelBaliseContenuIter++)
      if (*XMLModelBaliseIter == "entry")
        push_back(new EpisodusElemData(*XMLModelBaliseContenuIter, iOrder++)) ;
  }
}
catch (...)
{
	erreur("Exception EpisodusData ctor.", standardError, 0) ;
}
}

EpisodusData::~EpisodusData()
{
	vider() ;
}

void
EpisodusData::Reorder()
{
  if (empty())
    return ;

  ArrayEpisodusDataIter i = begin() ;
  for (int iNum = 0 ; i != end() ; i++, iNum++)
    (*i)->iOrder = iNum ;
}

void
EpisodusData::vider()
{
  if (empty())
    return ;

  for (ArrayEpisodusDataIter i = begin() ; i != end() ; )
  {
    delete (*i) ;
    erase(i) ;
  }
}

// -----------------------------------------------------------------------------
// Fonctions de Correspondance
// -----------------------------------------------------------------------------
bool
EpisodusData::SetElement(size_t index, string sLabel)
{
  if ((empty()) || (index > size()))
    return false ;

  EpisodusDataIter myIter = begin() ;
  for (size_t i = 0 ; (myIter != end()) && (i < index) ; myIter++, i++)
    ;
  if (myIter != end())
  {
    (*myIter)->sCorrespondence = sLabel ;
    return true ;
  }
  return false ;
}

bool
EpisodusData::SetClassifElement(size_t index, string sLabel)
{
  if ((empty()) || (index > size()))
    return false ;

  EpisodusDataIter myIter = begin() ;
  for (size_t i = 0 ; (myIter != end()) && (i < index) ; myIter++, i++)
    ;
  if (myIter != end())
  {
    (*myIter)->sClassifier = sLabel ;
    return true ;
  }
  return false ;
}

bool
EpisodusData::SetFormatElement(size_t index, string sLabel)
{
  if ((empty()) || (index > size()))
    return false ;

  EpisodusDataIter myIter = begin() ;

  for (size_t i = 0 ; (myIter != end()) && (i < index) ; myIter++, i++)
    ;
  if (myIter != end())
  {
    (*myIter)->sFormat = sLabel ;
    return true ;
  }
  return false ;
}

bool
EpisodusData::SetUnitElement(size_t index, string sLabel)
{
  if ((empty()) || (index > size()))
    return false ;

  EpisodusDataIter myIter = begin() ;

  for (size_t i = 0 ; (myIter != end()) && (i < index) ; myIter++, i++)
    ;
  if (myIter != end())
  {
    (*myIter)->sUnit = sLabel ;
    return true ;
  }
  return false ;
}

bool
EpisodusData::SetAnalyseTypeElement(size_t index, int iAnalyse)
{
  if ((empty()) || (index > size()))
    return false ;

  EpisodusDataIter myIter = begin() ;
  for (size_t i = 0 ; (myIter != end()) && (i < index) ; myIter++, i++)
    ;
  if (myIter != end())
  {
    (*myIter)->iModeAnalyse = iAnalyse ;
    return true ;
  }
  return false ;
}

EpisodusElemData*
EpisodusData::getElement(size_t index)
{
  if ((empty()) || (index > size()))
    return (EpisodusElemData*) 0 ;

  EpisodusDataIter myIter = begin() ;
  for (size_t i = 0 ; (end() != myIter) && (i < index) ; myIter++, i++)
    ;

  if (end() != myIter)
    return (*myIter) ;

  return (EpisodusElemData*) 0 ;
}

bool
EpisodusData::CompareWithModel(EpisodusData *pCompareData, bool bExact)
{
  if ((empty()) || ((EpisodusData *) NULL == pCompareData) || (pCompareData->empty()))
    return false ;

  // On teste si tous les éléments sont dans le modèle, pas si les deux
  // arrays sont égales
  // return true if every element is present in the model (not is each
  // and every element is equal)

  int iNotFoundNb     = 0 ;
  int iNeverAnalyseNb = 0 ;

  for (EpisodusDataIter myElement = begin() ; myElement != end() ; myElement++)
  {
    iNeverAnalyseNb = 0 ;
    EpisodusDataIter myCompareElement = pCompareData->begin() ;
    for ( ; (myCompareElement != pCompareData->end()) ; myCompareElement++)
    {
      // Si l'élément n'est jamais pris en compte, on ne compare pas
      if      ((*myCompareElement)->iModeAnalyse == EpisodusElemData::analyseNever)
        iNeverAnalyseNb++ ;
      else if ((*myElement)->CompareWithElementModel(*myCompareElement, pCompareData->iModeAnalyse))
        break ;
    }
    if (myCompareElement == pCompareData->end())
      iNotFoundNb++ ;
  }

  // Si tous les éléments n'ont pas été trouvés, et que le nombre d'éléments
  // non trouvés est différent du nombre d'éléments à ignorer dans le modèle,
  // on considère que la comparaison a échoué
  if (0 != iNotFoundNb)
  {
    if (bExact)
      return false ;
    if (iNotFoundNb != iNeverAnalyseNb)
      return false ;
  }

  return true ;
}

bool
EpisodusData::captureSubModel(EpisodusData* pCompareData, NSCaptureArray* pCaptureArray, ArrayEpisodusData* pCapturedElements, ArrayEpisodusData* pModelElements)
{
  if ((empty()) || (!pCompareData) || (pCompareData->empty()) || (!pCaptureArray))
    return false ;

  // On teste si tous les éléments sont dans le modèle, pas si les deux
  // arrays sont égales
  // return true if every element is present in the model (not is each
  // and every element is equal)

  EpisodusElemData * pRepereModel      = 0 ;
  ArrayEpisodusData aRelativeElements ;
  ArrayEpisodusData aNewCapturedElements ;
  ArrayEpisodusData aModelElements ;
  ArrayEpisodusData aNewModelElements ;

  EpisodusDataIter myCompareElement = pCompareData->begin() ;
  for ( ; myCompareElement != pCompareData->end() ; myCompareElement++)
  {
    // Si c'est un élément relatif, on le met de côté
    if (((*myCompareElement)->iModeAnalyse == EpisodusElemData::analyseRelativeOrder) || ((*myCompareElement)->iModeAnalyse == EpisodusElemData::analyseRelativePlacement))
      aRelativeElements.push_back(*myCompareElement) ;
    else
    {
      pRepereModel = *myCompareElement ;

      // On met de côté tous les éléments relatifs
      myCompareElement++ ;
      for ( ; (myCompareElement != pCompareData->end()) && (((*myCompareElement)->iModeAnalyse == EpisodusElemData::analyseRelativeOrder) || ((*myCompareElement)->iModeAnalyse == EpisodusElemData::analyseRelativePlacement)); myCompareElement++)
        aRelativeElements.push_back(*myCompareElement) ;
      myCompareElement-- ;

      // On cherche l'élément repère dans la capture
      EpisodusDataIter myRepereElement = begin() ;

      // On cherche l'élément repère dans la capture
      bool bChercher = true ;
      for ( ; (myRepereElement != end()) && (bChercher) ; myRepereElement++)
      {
        // Si l'élément est bon, on l'utilise comme repère pour tous les éléments relatifs
        if ((*myRepereElement)->CompareWithElementModel(pRepereModel, pCompareData->iModeAnalyse))
        {
          if (aRelativeElements.empty())
          {
            bChercher = false ;
            break ;
          }
          else
          {
            // On remet à zero les éléments en cours de capture
            if (!(aNewCapturedElements.empty()))
            {
              for (ArrayEpisodusDataIter iCapt = aNewCapturedElements.begin() ; iCapt != aNewCapturedElements.end() ; )
                aNewCapturedElements.erase(iCapt) ;
            }
            if (!(aNewModelElements.empty()))
            {
              for (ArrayEpisodusDataIter iModel = aNewModelElements.begin() ; iModel != aNewModelElements.end() ; )
                aNewModelElements.erase(iModel) ;
            }
            // Recherche des éléments relatifs dans la capture
            EpisodusDataIter iRelativeElmt = aRelativeElements.begin() ;
            for ( ; iRelativeElmt != aRelativeElements.end() ; iRelativeElmt++)
            {
              EpisodusDataIter iCaptureElmt = begin() ;
              for ( ; iCaptureElmt != end() ; iCaptureElmt++)
              {
                if ((*iCaptureElmt)->CompareWithRelativeElementModel(*iRelativeElmt, *myRepereElement, pRepereModel))
                {
                  if ((*iRelativeElmt)->sCorrespondence != "")
                  {
                    aNewCapturedElements.push_back(*iCaptureElmt) ;
                    aNewModelElements.push_back(*iRelativeElmt) ;
                  }
                  break ;
                }
              }
              // Si un élément relatif n'a pas été trouvé : échec
              if (iCaptureElmt == end())
                break ;
            }

            // Si tous les éléments relatifs on été trouvés : succès
            if (iRelativeElmt == aRelativeElements.end())
            {
              bChercher = false ;
              break ;
            }
          }
        }
      }

      // L'élément a été trouvé : on l'ajoute, ainsi que ses éléments relatifs aux éléments capturables
      if (myRepereElement != end())
      {
        if (pRepereModel->sCorrespondence != "")
        {
          pCapturedElements->push_back(*myRepereElement) ;
          pModelElements->push_back(pRepereModel) ;
        }
        if (!(aNewCapturedElements.empty()))
        {
          for (ArrayEpisodusDataIter iCapt = aNewCapturedElements.begin() ; iCapt != aNewCapturedElements.end() ; )
          {
            pCapturedElements->push_back(*iCapt) ;
            aNewCapturedElements.erase(iCapt) ;
          }
        }
        if (!(aNewModelElements.empty()))
        {
          for (ArrayEpisodusDataIter iModel = aNewModelElements.begin() ; iModel != aNewModelElements.end() ; )
          {
            pModelElements->push_back(*iModel) ;
            aNewModelElements.erase(iModel) ;
          }
        }
      }
      // Si l'élément n'a pas été trouvé : échec pour le sous-modèle
      else
        break ;
    }
  }
  if (myCompareElement != pCompareData->end())
    return false  ;

  if ((pCapturedElements->empty()) || (pModelElements->empty()))
    return false ;

  return true ;
}

bool
EpisodusData::SaveModel()
{
  return false ;
}
bool
EpisodusData::ExportModel(ofstream* pOutFile)
{
  char        buffer[1024] ;

  sprintf(buffer, "<model>\n") ;
  pOutFile->write(buffer, strlen(buffer)) ;
  for (EpisodusDataIter EpisodModelIter = begin() ; EpisodModelIter != end() ; EpisodModelIter++)
  {
    sprintf(buffer, "  <entry>\n") ;
    pOutFile->write(buffer, strlen(buffer)) ;
    sprintf(buffer, "    <coords>\n") ;
    pOutFile->write(buffer, strlen(buffer)) ;
    sprintf(buffer, "      <top>%d</top>\n", (*EpisodModelIter)->Placement.top) ;
    pOutFile->write(buffer, strlen(buffer)) ;
    sprintf(buffer, "      <left>%d</left>\n", (*EpisodModelIter)->Placement.left) ;
    pOutFile->write(buffer, strlen(buffer)) ;
    sprintf(buffer, "      <right>%d</right>\n", (*EpisodModelIter)->Placement.right) ;
    pOutFile->write(buffer, strlen(buffer)) ;
    sprintf(buffer, "      <bottom>%d</bottom>\n", (*EpisodModelIter)->Placement.bottom) ;
    pOutFile->write(buffer, strlen(buffer)) ;
    sprintf(buffer, "    </coords>\n") ;
    pOutFile->write(buffer, strlen(buffer)) ;
    sprintf(buffer, "    <class>%s</class>\n", (*EpisodModelIter)->sClassChild.c_str()) ;
    pOutFile->write(buffer, strlen(buffer)) ;
    sprintf(buffer, "    <correspondence>%s</correspondence>\n", (*EpisodModelIter)->sCorrespondence.c_str()) ;
    pOutFile->write(buffer, strlen(buffer)) ;
    switch ((*EpisodModelIter)->iModeCapture)
    {
      case EpisodusElemData::captureSystem  : strcpy(buffer, "    <capture>system</capture>\n") ;
                                              pOutFile->write(buffer, strlen(buffer)) ;
                                              break ;
      case EpisodusElemData::captureOCR     : strcpy(buffer, "    <capture>OCR</capture>\n") ;
                                              pOutFile->write(buffer, strlen(buffer)) ;
                                              break ;
    }
    switch ((*EpisodModelIter)->iModeAnalyse)
    {
      case EpisodusElemData::analyseDefault           : strcpy(buffer, "    <analyse>default</analyse>\n") ;
                                                        pOutFile->write(buffer, strlen(buffer)) ;
                                                        break ;
      case EpisodusElemData::analysePlacement         : strcpy(buffer, "    <analyse>placement</analyse>\n") ;
                                                        pOutFile->write(buffer, strlen(buffer)) ;
                                                        break ;
      case EpisodusElemData::analyseOrder             : strcpy(buffer, "    <analyse>order</analyse>\n") ;
                                                        pOutFile->write(buffer, strlen(buffer)) ;
                                                        break ;
      case EpisodusElemData::analysePlacementAndOrder : strcpy(buffer, "    <analyse>placement+order</analyse>\n") ;
                                                        pOutFile->write(buffer, strlen(buffer)) ;
                                                        break ;
      case EpisodusElemData::analyseLibelle           : strcpy(buffer, "    <analyse>libelle</analyse>\n") ;
                                                        pOutFile->write(buffer, strlen(buffer)) ;
                                                        break ;
      case EpisodusElemData::analyseRelativeOrder     : strcpy(buffer, "    <analyse>relativeOrder</analyse>\n") ;
                                                        pOutFile->write(buffer, strlen(buffer)) ;
                                                        break ;
      case EpisodusElemData::analyseRelativePlacement : strcpy(buffer, "    <analyse>relativePlacement</analyse>\n") ;
                                                        pOutFile->write(buffer, strlen(buffer)) ;
                                                        break ;
      case EpisodusElemData::analyseNever             : strcpy(buffer, "    <analyse>never</analyse>\n") ;
                                                        pOutFile->write(buffer, strlen(buffer)) ;
                                                        break ;
    }
    sprintf(buffer, "    <classify>%s</classify>\n", (*EpisodModelIter)->sClassifier.c_str()) ;
    pOutFile->write(buffer, strlen(buffer)) ;
    sprintf(buffer, "    <format>%s</format>\n", (*EpisodModelIter)->sFormat.c_str()) ;
    pOutFile->write(buffer, strlen(buffer)) ;
    sprintf(buffer, "    <unit>%s</unit>\n", (*EpisodModelIter)->sUnit.c_str()) ;
    pOutFile->write(buffer, strlen(buffer)) ;
    sprintf(buffer, "  </entry>\n") ;
    pOutFile->write(buffer, strlen(buffer)) ;
  }
  sprintf(buffer, "</model>\n") ;
  pOutFile->write(buffer, strlen(buffer)) ;

  return true ;
}

bool
EpisodusData::Model2View(EpisodusData *pEpisodDataView)
{
  if ((EpisodusData*) NULL == pEpisodDataView)
    return false ;

  EpisodusDataIter myElement = begin() ;

  for (EpisodusDataIter myViewElement = pEpisodDataView->begin() ; pEpisodDataView->end() != myViewElement ; myViewElement++)
  {
    if (end() != myElement)
    {
      (*myElement)->AppWindow   = (*myViewElement)->AppWindow ;
      (*myElement)->AppWndChild = (*myViewElement)->AppWndChild ;
      (*myElement)->sTextChild  = (*myViewElement)->sTextChild ;
      myElement++ ;
    }
    else
      return false ;
  }
  if (end() != myElement)
    return false ;

  return true ;
}

bool
EpisodusData::ImportModel(string sXMLBaliseContenu){
	return false ;
}
// -----------------------------------------------------------------------------
//// Classe EpisodusModel
//
// -----------------------------------------------------------------------------
EpisodusModel::EpisodusModel()
              :EpisodusData()
{
	sNom         = string("") ;
	iModeAnalyse = byPlacement ;
	bModified    = false ;
}

EpisodusModel::EpisodusModel(const EpisodusModel& rv)
{
try
{
	sNom         = rv.sNom ;
	iModeAnalyse = rv.iModeAnalyse ;
	bModified    = rv.bModified ;

	if (rv.empty())
		return ;

	for (EpisodusDataCIter EpisodusIter = rv.begin() ; rv.end() != EpisodusIter ; EpisodusIter++)
		push_back(new EpisodusElemData(*(*EpisodusIter))) ;
}
catch (...)
{
	erreur("Exception EpisodusModel copy ctor.", standardError, 0) ;
}
}

bool
EpisodusModel::useOCR()
{
  if (empty())
    return false ;

  for (EpisodusDataIter iMod = begin() ; end() != iMod ; iMod++)
    if (EpisodusElemData::captureOCR == (*iMod)->iModeCapture)
      return true ;
  return false ;
}

EpisodusModel&
EpisodusModel::operator=(const EpisodusModel& src)
{
	if (this == &src)
  	return *this ;

try
{
  vider() ;

  sNom         = src.sNom ;
  iModeAnalyse = src.iModeAnalyse ;
  bModified    = src.bModified ;

  if (src.empty())
    return (*this) ;

  for (EpisodusDataCIter EpisodusIter = src.begin() ; src.end() != EpisodusIter ; EpisodusIter++)
    push_back(new EpisodusElemData(*(*EpisodusIter))) ;

  return (*this) ;
}
catch (...)
{
  erreur("Exception EpisodusModel operator=.", standardError, 0) ;
  return (*this) ;
}
}

EpisodusModel::EpisodusModel(string sXMLBaliseContenu, string sXMLBaliseVariables)
{
try
{
  EpisodusModelXML ModelXML ;
  ModelXML.ImportModelXML(sXMLBaliseContenu) ;

  string sBaliseNom = string("nom=") ;
  size_t posDeb = sXMLBaliseVariables.find(sBaliseNom) ;
  if (string::npos != posDeb)
  {
    sNom = string("") ;
    posDeb += strlen(sBaliseNom.c_str()) ;
    if (posDeb < strlen(sXMLBaliseVariables.c_str()))
    {
      char cDebChar = sXMLBaliseVariables[posDeb] ;
      if (('\'' == cDebChar) || ('\"' == cDebChar))
      {
        posDeb++ ;
        size_t posFin = sXMLBaliseVariables.find(cDebChar, posDeb) ;
        if (string::npos != posFin)
          sNom = string(sXMLBaliseVariables, posDeb, posFin - posDeb) ;
      }
    }
  }
  string sBaliseAnalyse = string("analyse=") ;
  posDeb = sXMLBaliseVariables.find(sBaliseAnalyse) ;
  string _sAnalyse = string("") ;
  if (string::npos != posDeb)
  {
    posDeb += strlen(sBaliseAnalyse.c_str()) ;
    if (posDeb < strlen(sXMLBaliseVariables.c_str()))
    {
      char cDebChar = sXMLBaliseVariables[posDeb] ;
      if (('\'' == cDebChar) || ('\"' == cDebChar))
      {
        posDeb++ ;
        size_t posFin = sXMLBaliseVariables.find(cDebChar, posDeb) ;
        if (posFin != string::npos)
          _sAnalyse = string(sXMLBaliseVariables, posDeb, posFin-posDeb) ;
      }
    }
  }

  if      (_sAnalyse == "placement")
    iModeAnalyse = byPlacement ;
  else if (_sAnalyse == "order")
    iModeAnalyse = byOrder ;
  else if (_sAnalyse == "placement+order")
    iModeAnalyse = byPlacementAndOrder ;
  else
    iModeAnalyse = byPlacement ;

  bModified = false ;

  if (!ModelXML.sArrayBalise.empty() && !ModelXML.sArrayBaliseContenu.empty())
  {
    vector<string>::iterator XMLModelBaliseIter = ModelXML.sArrayBalise.begin() ;
    vector<string>::iterator XMLModelBaliseContenuIter = ModelXML.sArrayBaliseContenu.begin() ;

    int iOrder = 0 ;
    for ( ; (XMLModelBaliseIter != ModelXML.sArrayBalise.end()) && (XMLModelBaliseContenuIter != ModelXML.sArrayBaliseContenu.end()) ; XMLModelBaliseIter++, XMLModelBaliseContenuIter++)
      if (*XMLModelBaliseIter == "entry")
        push_back(new EpisodusElemData(*XMLModelBaliseContenuIter, iOrder++)) ;
  }
}
catch (...)
{
  erreur("Exception EpisodusModel ctor.", standardError, 0) ;
}
}

EpisodusModel::~EpisodusModel()
{
  vider() ;
}

// -----------------------------------------------------------------------------
//  Ecriture d'un modèle sur disque - Writting a model on disk
//  sType = "model" ou/or "submodel"
// -----------------------------------------------------------------------------
bool
EpisodusModel::ExportModel(ofstream * pOutFile, string sType)
{
  char        buffer[1024] ;

  strcpy(buffer, "<") ;
  strcat(buffer, sType.c_str()) ;
  strcat(buffer, " nom=\"") ;
  strcat(buffer, sNom.c_str()) ;
  strcat(buffer, "\" analyse=\"") ;
  switch (iModeAnalyse)
  {
    case byPlacement          : strcat(buffer, "placement\"") ;       break ;
    case byOrder              : strcat(buffer, "order\"") ;           break ;
    case byPlacementAndOrder  : strcat(buffer, "placement+order\"") ; break ;
    default                   : strcat(buffer, "placement\"") ;
  }
  strcat(buffer, ">\n") ;
  pOutFile->write(buffer, strlen(buffer)) ;
  for (EpisodusDataIter EpisodModelIter = begin() ; EpisodModelIter != end() ; EpisodModelIter++)
  {
    sprintf(buffer, "  <entry>\n") ;
    pOutFile->write(buffer, strlen(buffer)) ;
    if (sType == "submodel")
    {
      char szOrder[34] ;
      itoa((*EpisodModelIter)->iOrder, szOrder, 10) ;
      sprintf(buffer, "    <order>%s</order>\n", szOrder) ;
      pOutFile->write(buffer, strlen(buffer)) ;
    }
    sprintf(buffer, "    <coords>\n") ;
    pOutFile->write(buffer, strlen(buffer)) ;
    sprintf(buffer, "      <top>%d</top>\n", (*EpisodModelIter)->Placement.top) ;
    pOutFile->write(buffer, strlen(buffer)) ;
    sprintf(buffer, "      <left>%d</left>\n", (*EpisodModelIter)->Placement.left) ;
    pOutFile->write(buffer, strlen(buffer)) ;
    sprintf(buffer, "      <right>%d</right>\n", (*EpisodModelIter)->Placement.right) ;
    pOutFile->write(buffer, strlen(buffer)) ;
    sprintf(buffer, "      <bottom>%d</bottom>\n", (*EpisodModelIter)->Placement.bottom) ;
    pOutFile->write(buffer, strlen(buffer)) ;
    sprintf(buffer, "    </coords>\n") ;
    pOutFile->write(buffer, strlen(buffer)) ;
    sprintf(buffer, "    <class>%s</class>\n", (*EpisodModelIter)->sClassChild.c_str()) ;
    pOutFile->write(buffer, strlen(buffer)) ;
    if ((*EpisodModelIter)->sTextChild != "")
    {
      sprintf(buffer, "    <label>%s</label>\n", (*EpisodModelIter)->sTextChild.c_str()) ;
      pOutFile->write(buffer, strlen(buffer)) ;
    }
    if ((*EpisodModelIter)->sCorrespondence != "")
    {
      sprintf(buffer, "    <correspondence>%s</correspondence>\n", (*EpisodModelIter)->sCorrespondence.c_str()) ;
      pOutFile->write(buffer, strlen(buffer)) ;
    }
    switch ((*EpisodModelIter)->iModeCapture)
    {
      case EpisodusElemData::captureSystem  : strcpy(buffer, "    <capture>system</capture>\n") ;
                                              pOutFile->write(buffer, strlen(buffer)) ;
                                              break ;
      case EpisodusElemData::captureOCR     : strcpy(buffer, "    <capture>OCR</capture>\n") ;
                                              pOutFile->write(buffer, strlen(buffer)) ;
                                              break ;
    }
    switch ((*EpisodModelIter)->iModeAnalyse)
    {
      case EpisodusElemData::analyseDefault           : strcpy(buffer, "    <analyse>default</analyse>\n") ;
                                                        pOutFile->write(buffer, strlen(buffer)) ;
                                                        break ;
      case EpisodusElemData::analysePlacement         : strcpy(buffer, "    <analyse>placement</analyse>\n") ;
                                                        pOutFile->write(buffer, strlen(buffer)) ;
                                                        break ;
      case EpisodusElemData::analyseOrder             : strcpy(buffer, "    <analyse>order</analyse>\n") ;
                                                        pOutFile->write(buffer, strlen(buffer)) ;
                                                        break ;
      case EpisodusElemData::analysePlacementAndOrder : strcpy(buffer, "    <analyse>placement+order</analyse>\n") ;
                                                        pOutFile->write(buffer, strlen(buffer)) ;
                                                        break ;
      case EpisodusElemData::analyseLibelle           : strcpy(buffer, "    <analyse>libelle</analyse>\n") ;
                                                        pOutFile->write(buffer, strlen(buffer)) ;
                                                        break ;
      case EpisodusElemData::analyseRelativeOrder     : strcpy(buffer, "    <analyse>relativeOrder</analyse>\n") ;
                                                        pOutFile->write(buffer, strlen(buffer)) ;
                                                        break ;
      case EpisodusElemData::analyseRelativePlacement : strcpy(buffer, "    <analyse>relativePlacement</analyse>\n") ;
                                                        pOutFile->write(buffer, strlen(buffer)) ;
                                                        break ;
      case EpisodusElemData::analyseNever             : strcpy(buffer, "    <analyse>never</analyse>\n") ;
                                                        pOutFile->write(buffer, strlen(buffer)) ;
                                                        break ;
    }
    if ((*EpisodModelIter)->sClassifier != "")
    {
      sprintf(buffer, "    <classify>%s</classify>\n", (*EpisodModelIter)->sClassifier.c_str()) ;
      pOutFile->write(buffer, strlen(buffer)) ;
    }
    if ((*EpisodModelIter)->sFormat != "")
    {
      sprintf(buffer, "    <format>%s</format>\n", (*EpisodModelIter)->sFormat.c_str()) ;
      pOutFile->write(buffer, strlen(buffer)) ;
    }
    if ((*EpisodModelIter)->sUnit != "")
    {
      sprintf(buffer, "    <unit>%s</unit>\n", (*EpisodModelIter)->sUnit.c_str()) ;
      pOutFile->write(buffer, strlen(buffer)) ;
    }
    sprintf(buffer, "  </entry>\n") ;
    pOutFile->write(buffer, strlen(buffer)) ;
  }

  strcpy(buffer, "</") ;
  strcat(buffer, sType.c_str()) ;
  strcat(buffer, ">\n") ;
  pOutFile->write(buffer, strlen(buffer)) ;

  return true ;
}

// -----------------------------------------------------------------------------
//
// Classe ArrayEpisodusModelData
//
// -----------------------------------------------------------------------------
ArrayEpisodusModelData::ArrayEpisodusModelData(const ArrayEpisodusModelData& rv)
                       :ArrayEpisodusModelesData()
{
try
{
  if (!(rv.empty()))
    for (ArrayEpisodusModelCIter i = rv.begin() ; i != rv.end() ; i++)
      push_back(new EpisodusModel(*(*i))) ;
}
catch (...)
{
  erreur("Exception ArrayEpisodusModelData (copy ctor).", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------
// Cherche s'il existe un modèle correspondant à la capture
// -----------------------------------------------------------------------------
EpisodusModel*
ArrayEpisodusModelData::trouveModele(EpisodusData *pEpisodusData)
{
  if ((NULL == pEpisodusData) || (empty()))
    return 0 ;

  // Première passe : comparaison exacte
  for (ArrayEpisodusModelIter i = begin() ; i != end() ; i++)
    if (pEpisodusData->CompareWithModel(*i, true))
      return (*i) ;

  // Seconde passe : comparaison sans tenir compte des éléments inactifs
  for (ArrayEpisodusModelIter i = begin() ; i != end() ; i++)
    if (pEpisodusData->CompareWithModel(*i, false))
      return (*i) ;

  return 0 ;
}

EpisodusModel*
ArrayEpisodusModelData::findModelByName(string sModelName)
{
  if ((string("") == sModelName) || empty())
    return 0 ;

  for (ArrayEpisodusModelIter i = begin() ; i != end() ; i++)
    if ((*i)->sNom == sModelName)
      return (*i) ;

  return 0 ;
}

bool
ArrayEpisodusModelData::sauvegarder(ofstream * pOutFile, string sType)
{
  if ((NULL == pOutFile) || (string("") == sType))
    return false ;

  if (!(empty()))
  {
    for (ArrayEpisodusModelIter i = begin() ; i != end() ; i++)
      (*i)->ExportModel(pOutFile, sType) ;
  }
  return true ;
}

void
ArrayEpisodusModelData::vider()
{
  if (empty())
    return ;

  for (ArrayEpisodusModelIter i = begin() ; i != end() ; )
  {
    delete (*i) ;
    erase(i) ;
  }
}

ArrayEpisodusModelData::~ArrayEpisodusModelData()
{
	vider() ;
}

ArrayEpisodusModelData&
ArrayEpisodusModelData::operator=(const ArrayEpisodusModelData& src)
{
	if (this == &src)
  	return *this ;

try
{
  vider() ;

  if (false == src.empty())
    for (ArrayEpisodusModelCIter i = src.begin() ; src.end() != i ; i++)
      push_back(new EpisodusModel(*(*i))) ;
      
  return (*this) ;
}
catch (...)
{
  erreur("Exception ArrayEpisodusModelData (=).", standardError, 0) ;
  return (*this) ;
}
}

#endif // !_EXT_CAPTURE

// -----------------------------------------------------------------------------
//
// Classe EpisodusModelXML
//
// -----------------------------------------------------------------------------boolEpisodusModelXML::ImportModelXML(string input, size_t pos){
  size_t iBeginBalisePos = input.find('<', pos) ;
  while (iBeginBalisePos != string::npos)
  {
    size_t iEndBalisePos = input.find('>', iBeginBalisePos) ;
    if (iEndBalisePos != string::npos)
    {
      string sBalise = string(input, iBeginBalisePos + 1, iEndBalisePos - iBeginBalisePos - 1) ;

      // Recherche d'éventuels paramètres
      string sParams = "" ;
      pos = sBalise.find(string(" ")) ;
      if (pos != string::npos)
      {
        sParams = string(sBalise, pos + 1, strlen(sBalise.c_str()) - pos - 1) ;
        sBalise = string(sBalise, 0, pos) ;
      }

      string sBaliseEnd = "</" + sBalise + ">" ;
      pos = input.find(sBaliseEnd, iEndBalisePos) ;
      if (pos != string::npos)
      {
        string sBaliseContenu = string(input, iEndBalisePos+1, pos - iEndBalisePos - 1) ;
        iBeginBalisePos = input.find('<', pos + 1) ;
        sArrayBalise.push_back(sBalise) ;
        sArrayBaliseVariables.push_back(sParams) ;
        sArrayBaliseContenu.push_back(sBaliseContenu) ;
      }
      else
        return false ;
    }
    else
      return false ;
  }
  return true ;
}

