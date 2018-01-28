// NSFseUti : Structures nécéssaires aux dialogues de la fiche Compt, Fact et TPayant//            + Dialogue NSListComptDialog pour les situations
// Rémi SPAAK Avril 99
/////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <classlib\date.h>
#include <classlib\time.h>

#include "partage\nsdivfct.h"
#include "nautilus\nssuper.h"
#include "nautilus\nspatdlg.h"
#include "nautilus\nsadmiwd.h"
#include "nscompta\nscompta.h"
#include "nscompta\nscompta.rh"
#include "nscompta\nsf16dlg.h"
#include "nscompta\nsfsedlg.h"
#include "nscompta\nsfseuti.h"

//***************************************************************************
// 								Classe NSBlocFse16
//***************************************************************************

NSBlocFse16::NSBlocFse16(const NSFse1610* pFse1610)
{
  _iTypePrest = bloc1610 ;

  _p1610  = new NSFse1610Info(pFse1610) ;
  _p1620  = (NSFse1620Info*) 0 ;
  _p1630  = (NSFse1630Info*) 0 ;
  _pCCAM  = (NSFseCCAMInfo*) 0 ;
  _pLibre = (NSFseCCAMInfo*) 0 ;
  _pMat   = (NSFseMatInfo*)  0 ;

  _sNumPrest = _p1610->getNumPrest() ;
}

NSBlocFse16::NSBlocFse16(const NSFse1610Info* pFse1610Info)
{
  _iTypePrest = bloc1610 ;

  _p1610  = new NSFse1610Info(*pFse1610Info) ;
  _p1620  = (NSFse1620Info*) 0 ;
  _p1630  = (NSFse1630Info*) 0 ;
  _pCCAM  = (NSFseCCAMInfo*) 0 ;
  _pLibre = (NSFseCCAMInfo*) 0 ;
  _pMat   = (NSFseMatInfo*)  0 ;

  _sNumPrest = _p1610->getNumPrest() ;
}

NSBlocFse16::NSBlocFse16(const NSFse1620* pFse1620)
{
  _iTypePrest = bloc1620 ;

  _p1610  = (NSFse1610Info*) 0 ;
  _p1620  = new NSFse1620Info(pFse1620) ;
  _p1630  = (NSFse1630Info*) 0 ;
  _pCCAM  = (NSFseCCAMInfo*) 0 ;
  _pLibre = (NSFseCCAMInfo*) 0 ;
  _pMat   = (NSFseMatInfo*)  0 ;

  _sNumPrest = _p1620->getNumPrest() ;
}

NSBlocFse16::NSBlocFse16(const NSFse1620Info* pFse1620Info)
{
  _iTypePrest = bloc1620 ;

  _p1610  = (NSFse1610Info*) 0 ;
  _p1620  = new NSFse1620Info(*pFse1620Info) ;
  _p1630  = (NSFse1630Info*) 0 ;
  _pCCAM  = (NSFseCCAMInfo*) 0 ;
  _pLibre = (NSFseCCAMInfo*) 0 ;
  _pMat   = (NSFseMatInfo*)  0 ;

  _sNumPrest = _p1620->getNumPrest() ;
}

NSBlocFse16::NSBlocFse16(const NSFse1630* pFse1630)
{
  _iTypePrest = bloc1630 ;

  _p1610  = (NSFse1610Info*) 0 ;
  _p1620  = (NSFse1620Info*) 0 ;
  _p1630  = new NSFse1630Info(pFse1630) ;
  _pCCAM  = (NSFseCCAMInfo*) 0 ;
  _pLibre = (NSFseCCAMInfo*) 0 ;
  _pMat   = (NSFseMatInfo*)  0 ;

  _sNumPrest = _p1630->getNumPrest() ;
}

NSBlocFse16::NSBlocFse16(const NSFse1630Info* pFse1630Info)
{
  _iTypePrest = bloc1630 ;

  _p1610  = (NSFse1610Info*) 0 ;
  _p1620  = (NSFse1620Info*) 0 ;
  _p1630  = new NSFse1630Info(*pFse1630Info) ;
  _pCCAM  = (NSFseCCAMInfo*) 0 ;
  _pLibre = (NSFseCCAMInfo*) 0 ;
  _pMat   = (NSFseMatInfo*)  0 ;

  _sNumPrest = _p1630->getNumPrest() ;
}

NSBlocFse16::NSBlocFse16(const NSFseCCAM* pFseCCAM)
{  _iTypePrest = blocCcam ;  _p1610  = (NSFse1610Info*) 0 ;  _p1620  = (NSFse1620Info*) 0 ;  _p1630  = (NSFse1630Info*) 0 ;  _pCCAM  = new NSFseCCAMInfo(pFseCCAM) ;
  _pLibre = (NSFseCCAMInfo*) 0 ;
  _pMat   = (NSFseMatInfo*)  0 ;

  _sNumPrest = _pCCAM->getNumPrest() ;
}

NSBlocFse16::NSBlocFse16(const NSFseCCAMInfo* pFseCCAMInfo)
{
  _iTypePrest = blocCcam ;

  _p1610  = (NSFse1610Info*) 0 ;
  _p1620  = (NSFse1620Info*) 0 ;  _p1630  = (NSFse1630Info*) 0 ;
  _pMat   = (NSFseMatInfo*)  0 ;
  _pCCAM  = new NSFseCCAMInfo(*pFseCCAMInfo) ;
  _pLibre = (NSFseLibreInfo*) 0 ;

  _sNumPrest = pFseCCAMInfo->getNumPrest() ;
}

NSBlocFse16::NSBlocFse16(const NSFseLibre* pFseLibre)
{  _iTypePrest = blocLibre ;  _p1610  = (NSFse1610Info*) 0 ;  _p1620  = (NSFse1620Info*) 0 ;  _p1630  = (NSFse1630Info*) 0 ;  _pCCAM  = (NSFseCCAMInfo*) 0 ;
  _pLibre = new NSFseLibreInfo(pFseLibre) ;
  _pMat   = (NSFseMatInfo*)  0 ;

  _sNumPrest = pFseLibre->getNumPrest() ;
}

NSBlocFse16::NSBlocFse16(const NSFseLibreInfo* pFseLibreInfo)
{
  _iTypePrest = blocLibre ;

  _p1610  = (NSFse1610Info*) 0 ;
  _p1620  = (NSFse1620Info*) 0 ;  _p1630  = (NSFse1630Info*) 0 ;
  _pCCAM  = (NSFseCCAMInfo*) 0 ;
  _pLibre = new NSFseLibreInfo(*pFseLibreInfo) ;
  _pMat   = (NSFseMatInfo*)  0 ;

  _sNumPrest = pFseLibreInfo->getNumPrest() ;
}

NSBlocFse16::NSBlocFse16(const NSFseMat* pFseMat)
{  _iTypePrest = blocMat ;  _p1610  = (NSFse1610Info*) 0 ;  _p1620  = (NSFse1620Info*) 0 ;  _p1630  = (NSFse1630Info*) 0 ;  _pCCAM  = (NSFseCCAMInfo*) 0 ;
  _pLibre = (NSFseCCAMInfo*) 0 ;
  _pMat   = new NSFseMatInfo(pFseMat) ;

  _sNumPrest = pFseMat->getNumPrest() ;
}

NSBlocFse16::NSBlocFse16(const NSFseMatInfo* pFseMatInfo)
{  _iTypePrest = blocMat ;  _p1610  = (NSFse1610Info*) 0 ;  _p1620  = (NSFse1620Info*) 0 ;  _p1630  = (NSFse1630Info*) 0 ;  _pCCAM  = (NSFseCCAMInfo*) 0 ;
  _pLibre = (NSFseCCAMInfo*) 0 ;
  _pMat   = new NSFseMatInfo(*pFseMatInfo) ;

  _sNumPrest = pFseMatInfo->getNumPrest() ;
}
// constructeur copie
NSBlocFse16::NSBlocFse16(const NSBlocFse16& rv)
{
  if (rv._p1610)
    _p1610 = new NSFse1610Info(*(rv._p1610)) ;
  else
    _p1610 = (NSFse1610Info*) 0 ;

  if (rv._p1620)
    _p1620 = new NSFse1620Info(*(rv._p1620)) ;
  else
    _p1620 = (NSFse1620Info*) 0 ;

  if (rv._p1630)
    _p1630 = new NSFse1630Info(*(rv._p1630)) ;
  else
    _p1630 = (NSFse1630Info*) 0 ;
  if (rv._pCCAM)    _pCCAM = new NSFseCCAMInfo(*(rv._pCCAM)) ;  else    _pCCAM = (NSFseCCAMInfo*) 0 ;  if (rv._pLibre)    _pLibre = new NSFseLibreInfo(*(rv._pLibre)) ;  else    _pLibre = (NSFseLibreInfo*) 0 ;  if (rv._pMat)    _pMat = new NSFseMatInfo(*(rv._pMat)) ;  else    _pMat = (NSFseMatInfo*) 0 ;
  _iTypePrest = rv._iTypePrest ;
  _sNumPrest  = rv._sNumPrest ;
}

NSBlocFse16&
NSBlocFse16::operator=(const NSBlocFse16& rv)
{
  if (this == &rv)
		return *this ;

  if (_p1610)
    delete _p1610 ;
  if (rv._p1610)
    _p1610 = new NSFse1610Info(*(rv._p1610)) ;
  else
    _p1610 = (NSFse1610Info*) 0 ;

  if (_p1620)
    delete _p1620 ;
  if (rv._p1620)
    _p1620 = new NSFse1620Info(*(rv._p1620)) ;
  else
    _p1620 = (NSFse1620Info*) 0 ;

  if (_p1630)
    delete _p1630 ;
  if (rv._p1630)
    _p1630 = new NSFse1630Info(*(rv._p1630)) ;
  else
    _p1630 = (NSFse1630Info*) 0 ;
  if (_pCCAM)    delete _pCCAM ;  if (rv._pCCAM)    _pCCAM = new NSFseCCAMInfo(*(rv._pCCAM)) ;  else    _pCCAM = (NSFseCCAMInfo*) 0 ;
  if (_pLibre)
    delete _pLibre ;  if (rv._pLibre)    _pLibre = new NSFseLibreInfo(*(rv._pLibre)) ;  else    _pLibre = (NSFseLibreInfo*) 0 ;

  if (_pMat)
    delete _pMat ;  if (rv._pMat)    _pMat = new NSFseMatInfo(*(rv._pMat)) ;  else    _pMat = (NSFseMatInfo*) 0 ;

  _iTypePrest = rv._iTypePrest ;
  _sNumPrest  = rv._sNumPrest ;

  return *this ;
}

// destructeur
NSBlocFse16::~NSBlocFse16()
{
  if (_p1610)
    delete _p1610 ;

  if (_p1620)
    delete _p1620 ;

  if (_p1630)
    delete _p1630 ;
  if (_pCCAM)    delete _pCCAM ;  if (_pLibre)    delete _pLibre ;  if (_pMat)
    delete _pMat ;}

void
NSBlocFse16::setNumprest(string sNumprest)
{
  _sNumPrest = sNumprest ;

  switch(_iTypePrest)
  {      
    case bloc1610  : _p1610->setNumPrest(sNumprest) ;  break ;
    case bloc1620  : _p1620->setNumPrest(sNumprest) ;  break ;
    case bloc1630  : _p1630->setNumPrest(sNumprest) ;  break ;
    case blocCcam  : _pCCAM->setNumPrest(sNumprest) ;  break ;
    case blocLibre : _pLibre->setNumPrest(sNumprest) ; break ;
    case blocMat   : _pMat->setNumPrest(sNumprest) ;   break ;
  }
}

void
NSBlocFse16::setNumprest(int iNumprest)
{
  string sNum = IntToString(iNumprest) ;

  size_t iLen = strlen(sNum.c_str()) ;
  if (iLen > FSE_NUMPREST_LEN)
  {
    setNumprest(string("")) ;
    return ;
  }

  if (iLen < FSE_NUMPREST_LEN)
    sNum = string(FSE_NUMPREST_LEN - iLen, '0') + sNum ;

  setNumprest(sNum) ;
}

string
NSBlocFse16::GetSumInUnits(string sSumInCents)
{
  if ((string("") == sSumInCents) || (string("0") == sSumInCents))
    return string("0,00") ;

  size_t iLen = strlen(sSumInCents.c_str()) ;
  if (iLen > 2)
    return string(sSumInCents, 0, iLen - 2) + string(",") + string(sSumInCents, iLen - 2, 2) ;

  if (1 == iLen)
    sSumInCents = string("0") + sSumInCents ;

  return string("0,") + sSumInCents ;
}

//***************************************************************************
// 								Classe NSFse16Array
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSFse16Array::NSFse16Array(const NSFse16Array& rv)
             :NSFse16xxArray()
{
	if (false == rv.empty())
		for (NSFse16ConstIter i = rv.begin() ; rv.end() != i ; i++)
   		push_back(new NSBlocFse16(**i)) ;
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
void
NSFse16Array::vider()
{
	if (empty())
		return ;

	for (NSFse16Iter i = begin() ; end() != i ; )
	{
		delete *i ;
		erase(i) ;
	}
}

NSFse16Array::~NSFse16Array(){
	vider() ;
}

NSFse16Array&
NSFse16Array::operator=(const NSFse16Array& rv)
{
  if (this == &rv)
  	return *this ;

	vider() ;

	if (false == rv.empty())
		for (NSFse16ConstIter i = rv.begin() ; rv.end() != i ; i++)
   		push_back(new NSBlocFse16(**i)) ;

	return *this ;
}

//-----------------------------------------------------------------------// fonction globale de tri des blocs Fse 16xx
//-----------------------------------------------------------------------
bool
blocInferieur(NSBlocFse16* a, NSBlocFse16* b)
{
	if ((a != NULL) && (b != NULL) && (a->_sNumPrest < b->_sNumPrest))
  	return true ;

	return false ;
}

//-----------------------------------------------------------------------// fonction globale de tri des FactInfo
//-----------------------------------------------------------------------
bool
factAnterieure(NSFactInfo* a, NSFactInfo* b)
{
	if (((NSFactInfo*) NULL != a) && ((NSFactInfo*) NULL != b) &&
      (a->_Donnees.getPaidDate() < b->_Donnees.getPaidDate()))
		return true ;

	return false ;
}

// -----------------------------------------------------------------//
//  Méthodes de NSEditDateC
//
// -----------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSEditDateC, NSUtilEditDate)
	EV_WM_KILLFOCUS,
END_RESPONSE_TABLE;

NSEditDateC::NSEditDateC(NSContexte *pCtx, CreerFicheComptDialog* pere, int resId, int iTextLen)
            :NSUtilEditDate(pCtx, pere, resId, iTextLen)
{
	pDlg = pere ;
}

voidNSEditDateC::EvKillFocus(HWND hWndGetFocus)
{
	NSUtilEditDate::EvKillFocus(hWndGetFocus) ;

	pDlg->MajDateC() ;
}

// -----------------------------------------------------------------//
//  Méthodes de NSEditHeureC
//
// -----------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSEditHeureC, NSUtilEditHeure)
	EV_WM_KILLFOCUS,
END_RESPONSE_TABLE;

NSEditHeureC::NSEditHeureC(NSContexte *pCtx, CreerFicheComptDialog* pere, int resId, int iTextLen)             :NSUtilEditHeure(pCtx, pere, resId, iTextLen){
	pDlg = pere ;
}

voidNSEditHeureC::EvKillFocus(HWND hWndGetFocus)
{
  NSUtilEditHeure::EvKillFocus(hWndGetFocus) ;

  pDlg->MajHeureC() ;}

// -----------------------------------------------------------------//
//  Méthodes de NSEditSommeDue
//
// -----------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSEditSommeDue, NSUtilEditSomme)
	EV_WM_KILLFOCUS,
END_RESPONSE_TABLE;

NSEditSommeDue::NSEditSommeDue(NSContexte *pCtx, CreerFicheComptDialog* pere, int resId, int iTextLen)
               :NSUtilEditSomme(pCtx, pere, resId, iTextLen)
{
  pDlg = pere ;
}

voidNSEditSommeDue::EvKillFocus(HWND hWndGetFocus)
{
  NSUtilEditSomme::EvKillFocus(hWndGetFocus) ;

  pDlg->AfficheDepass() ;  pDlg->AfficheSommeDue() ;
  pDlg->AffichePaye() ;
}

// -----------------------------------------------------------------//
//  Méthodes de NSListFseWindow
//
// -----------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSListFseWindow, TListWindow)   EV_WM_KEYDOWN,
   EV_WM_LBUTTONDBLCLK,
END_RESPONSE_TABLE;

NSListFseWindow::NSListFseWindow(CreerFicheComptDialog* pere, int resId) :                 TListWindow(pere, resId)
{
  pDlg = pere ;

  Attr.Style |= LVS_REPORT | LVS_SHOWSELALWAYS ;  Attr.ExStyle |= WS_EX_NOPARENTNOTIFY ;
}
void
NSListFseWindow::SetupWindow()
{
  ListView_SetExtendedListViewStyle(this->HWindow, LVS_EX_FULLROWSELECT) ;

  TListWindow::SetupWindow() ;
}
// Fonction pour la suppression des fiches Fse16xx
void
NSListFseWindow::EvKeyDown(uint key, uint /* repeatCount */, uint /* flags */)
{
  if (VK_DELETE == key)
  {
    int indexDel = IndexItemSelect() ;
    if (-1 == indexDel)
    {
      erreur("Vous devez sélectionner une prestation.", standardError, 0) ;
      return ;
    }

    string sCaption = string("Message ") + pDlg->pContexte->getSuperviseur()->getAppName() ;
    int retVal = MessageBox("Etes-vous sûr de vouloir supprimer cette prestation ?", sCaption.c_str(), MB_YESNO) ;
    if (IDYES != retVal)
      return ;

    NSFse16Iter it = pDlg->pFseArray->begin() ;
    for (int j = 0 ; pDlg->pFseArray->end() != it ; it++, j++)
    {
      if (j == indexDel)
      {
        pDlg->removeFromFseArray(it) ;
        break ;
      }
    }
  }}

//---------------------------------------------------------------------------//  Function: NSListFseWindow::EvLButtonDblClk(uint modKeys, TPoint& point)
//
//  Arguments:	  les modKeys et le point double-clické
//
//  Description: Fonction de réponse au double-click
//
//  Returns:     Rien
//---------------------------------------------------------------------------
void
NSListFseWindow::EvLButtonDblClk(uint /* modKeys */, NS_CLASSLIB::TPoint& point)
{
  TLwHitTestInfo info(point) ;

  HitTest(info) ;

  if (info.GetFlags() & LVHT_ONITEM)
    pDlg->CmModifFse() ;
}

//---------------------------------------------------------------------------
//  Function: NSListFseWindow::IndexItemSelect()
//
//  Arguments:	  Aucun
//
//  Description: Retourne l'index du premier item sélectionné
//
//  Returns:     index si item est sélectionné, -1 sinon
//---------------------------------------------------------------------------
int
NSListFseWindow::IndexItemSelect()
{
	int count = GetItemCount();
   int index = -1;

   for (int i = 0; i < count; i++)
   	if (GetItemState(i, LVIS_SELECTED))
      {
      	index = i;
         break;
      }

   return index;
}

// -----------------------------------------------------------------
//
//  Méthodes de NSListFactWindow
//
// -----------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSListFactWindow, TListWindow)
   EV_WM_KEYDOWN,
   EV_WM_LBUTTONDBLCLK,
END_RESPONSE_TABLE;

NSListFactWindow::NSListFactWindow(CreerFicheComptDialog* pere, int resId) :
                  TListWindow(pere, resId)
{
   	pDlg = pere;
    Attr.Style |= LVS_REPORT | LVS_SHOWSELALWAYS ;
    Attr.ExStyle |= WS_EX_NOPARENTNOTIFY ;
}voidNSListFactWindow::SetupWindow(){  ListView_SetExtendedListViewStyle(this->HWindow, LVS_EX_FULLROWSELECT) ;
  TListWindow::SetupWindow() ;}
// Fonction pour la suppression des fiches Factvoid
NSListFactWindow::EvKeyDown(uint key, uint /* repeatCount */, uint /* flags */)
{
  if (VK_DELETE == key)
  {
    int indexDel = IndexItemSelect() ;
    if (-1 == indexDel)
    {
      erreur("Vous devez sélectionner un paiement.", standardError, 0) ;
      return ;
    }

    string sCaption = string("Message ") + pDlg->pContexte->getSuperviseur()->getAppName() ;
    int retVal = MessageBox("Etes-vous sûr de vouloir supprimer ce paiement ?", sCaption.c_str(), MB_YESNO) ;
    if (IDNO == retVal)
      return ;

    if (false == pDlg->pFactArray->empty())
    {
      NSFactIter i = pDlg->pFactArray->begin() ;
      int        j = 0 ;
      
      for ( ; pDlg->pFactArray->end() != i ; i++, j++)      {
        if (j == indexDel)
        {
          // on annule le précédent paiement retrouvé dans FactInfo
          pDlg->IncrementeResteDu((*i)->_Donnees.organisme,
                                  (*i)->_Donnees.montant,
                                  (*i)->_Donnees.unite) ;

          // on supprime la fiche          delete *i ;
          pDlg->pFactArray->erase(i) ;
          pDlg->nbFact -= 1 ;
          break ;
        }
      }
    }

    // on remet à jour les numéros de prestations ultérieurs à indexDel
    for (int k = indexDel; k < pDlg->nbFact; k++)
    {
      int numFact = atoi(((*(pDlg->pFactArray))[k])->_Donnees.numero) ;
      sprintf(((*(pDlg->pFactArray))[k])->_Donnees.numero, "%02d", numFact - 1) ;
    }

    // on remet la liste à jour
    pDlg->AfficheListeFact() ;
    pDlg->AffichePaye() ;
  }
}

//---------------------------------------------------------------------------//  Function: NSListFactWindow::EvLButtonDblClk(uint modKeys, TPoint& point)
//
//  Arguments:	  les modKeys et le point double-clické
//
//  Description: Fonction de réponse au double-click
//
//  Returns:     Rien
//---------------------------------------------------------------------------
void
NSListFactWindow::EvLButtonDblClk(uint /* modKeys */, NS_CLASSLIB::TPoint& point)
{
  TLwHitTestInfo info(point) ;

  HitTest(info) ;

  if (info.GetFlags() & LVHT_ONITEM)
    pDlg->CmModifFact() ;
}

//---------------------------------------------------------------------------
//  Function: NSListFactWindow::IndexItemSelect()
//
//  Arguments:	  Aucun
//
//  Description: Retourne l'index du premier item sélectionné
//
//  Returns:     index si item est sélectionné, -1 sinon
//---------------------------------------------------------------------------
int
NSListFactWindow::IndexItemSelect()
{
	int count = GetItemCount();
   int index = -1;

   for (int i = 0; i < count; i++)
   	if (GetItemState(i, LVIS_SELECTED))
      {
      	index = i;
         break;
      }

   return index;
}

// -----------------------------------------------------------------//
//  Méthodes de NSListTPWindow
//
// -----------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSListTPWindow, TListWindow)
   EV_WM_KEYDOWN,
   EV_WM_LBUTTONDBLCLK,
END_RESPONSE_TABLE;


NSListTPWindow::NSListTPWindow(CreerFicheComptDialog* pere, int resId) :
                TListWindow(pere, resId)
{
   	pDlg = pere;
    Attr.Style |= LVS_REPORT | LVS_SHOWSELALWAYS ;
    Attr.ExStyle |= WS_EX_NOPARENTNOTIFY ;
}
void
NSListTPWindow::SetupWindow()
{
  ListView_SetExtendedListViewStyle(this->HWindow, LVS_EX_FULLROWSELECT) ;

  TListWindow::SetupWindow() ;
}
// Fonction pour la suppression des fiches TPayant
void
NSListTPWindow::EvKeyDown(uint key, uint /* repeatCount */, uint /* flags */)
{
  if (VK_DELETE == key)
  {
    int indexDel = IndexItemSelect() ;
    if (-1 == indexDel)
    {
      erreur("Vous devez sélectionner un tiers-payant.", standardError, 0) ;
      return ;
    }

    string sCaption = string("Message ") + pDlg->pContexte->getSuperviseur()->getAppName() ;
    int retVal = MessageBox("Etes-vous sûr de vouloir supprimer ce tiers-payant ?", sCaption.c_str(), MB_YESNO) ;
    if (IDNO == retVal)
      return ;

    if (false == pDlg->pTPArray->empty())
    {
      NSTPIter i = pDlg->pTPArray->begin() ;
      int      j = 0 ;

      for ( ; pDlg->pTPArray->end() != i ; i++, j++)
      {
        if (j == indexDel)
        {
          delete *i ;
          pDlg->pTPArray->erase(i) ;
          pDlg->nbTP -= 1 ;
          break ;
        }
      }
    }

    // on remet à jour les numéros de prestations ultérieurs à indexDel
    for (int k = indexDel ; k < pDlg->nbTP ; k++)
    {
      int numTP = atoi(((*(pDlg->pTPArray))[k])->_Donnees.numero) ;
      sprintf(((*(pDlg->pTPArray))[k])->_Donnees.numero, "%02d", numTP - 1) ;
    }

    // on remet la liste à jour
    pDlg->AfficheListeTP() ;
  }
}

//---------------------------------------------------------------------------//  Function: NSListTPWindow::EvLButtonDblClk(uint modKeys, TPoint& point)
//
//  Arguments:	  les modKeys et le point double-clické
//
//  Description: Fonction de réponse au double-click
//
//  Returns:     Rien
//---------------------------------------------------------------------------
void
NSListTPWindow::EvLButtonDblClk(uint /* modKeys */, NS_CLASSLIB::TPoint& point)
{
  TLwHitTestInfo info(point) ;

  HitTest(info) ;

  if (info.GetFlags() & LVHT_ONITEM)
    pDlg->CmModifTP() ;
}

//---------------------------------------------------------------------------
//  Function: NSListTPWindow::IndexItemSelect()
//
//  Arguments:	  Aucun
//
//  Description: Retourne l'index du premier item sélectionné
//
//  Returns:     index si item est sélectionné, -1 sinon
//---------------------------------------------------------------------------
int
NSListTPWindow::IndexItemSelect()
{
  int index = -1 ;

  int count = GetItemCount() ;  for (int i = 0 ; i < count ; i++)
    if (GetItemState(i, LVIS_SELECTED))
    {
      index = i ;
      break ;
    }

  return index ;
}

// -----------------------------------------------------------------//
//  Méthodes de NSListComptWindow
//
// -----------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSListComptWindow, TListWindow)
   EV_WM_KEYDOWN,
   EV_WM_LBUTTONDBLCLK,
END_RESPONSE_TABLE;

void
NSListComptWindow::SetupWindow()
{
    ListView_SetExtendedListViewStyle(this->HWindow, LVS_EX_FULLROWSELECT) ;

    TListWindow::SetupWindow();
}
//---------------------------------------------------------------------------
//  Function: NSListComptWindow::EvLButtonDblClk(uint modKeys, TPoint& point)
//
//  Arguments:	  les modKeys et le point double-clické
//
//  Description: Fonction de réponse au double-click
//
//  Returns:     Rien
//---------------------------------------------------------------------------
void
NSListComptWindow::EvLButtonDblClk(uint /* modKeys */, NS_CLASSLIB::TPoint& point)
{
  TLwHitTestInfo info(point) ;

  HitTest(info) ;

  if (info.GetFlags() & LVHT_ONITEM)
    pDlg->CmModifCompt() ;
}

// Fonction pour la suppression des fiches comptvoid
NSListComptWindow::EvKeyDown(uint key, uint /* repeatCount */, uint /* flags */)
{
  if (VK_DELETE == key)
  {
    int indexDel = IndexItemSelect() ;
    if (-1 == indexDel)
    {
      erreur("Vous devez sélectionner une fiche de compta.", standardError, 0) ;
      return ;
    }

    string sCaption = string("Message ") + pDlg->pContexte->getSuperviseur()->getAppName();
    int retVal = MessageBox("Etes-vous sûr de vouloir supprimer cette fiche de compta ?", sCaption.c_str(), MB_YESNO);
    if (IDNO == retVal)
      return ;

    pDlg->CmDeleteCompt() ;
  }
}

//---------------------------------------------------------------------------//  Function: NSListComptWindow::IndexItemSelect()
//
//  Arguments:	  Aucun
//
//  Description: Retourne l'index du premier item sélectionné
//
//  Returns:     index si item est sélectionné, -1 sinon
//---------------------------------------------------------------------------
int
NSListComptWindow::IndexItemSelect()
{
  int index = -1 ;

  int count = GetItemCount() ;
  for (int i = 0; i < count; i++)
    if (GetItemState(i, LVIS_SELECTED))
    {
      index = i ;
      break ;
    }

  return index ;
}

// -----------------------------------------------------------------//
//  Méthodes de NSListComptDialog
//
// -----------------------------------------------------------------

// ---------------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NSListComptDialog, NSUtilDialog)
	EV_LV_DISPINFO_NOTIFY(IDC_LC_LW, LVN_GETDISPINFO, DispInfoListeCompt),
  EV_BN_CLICKED(IDC_LC_LOC, SwitchLocEuro),
  EV_BN_CLICKED(IDC_LC_EURO, SwitchLocEuro),
  EV_COMMAND(IDOK, CmOk),
  EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE;

//// Constructeur
//
NSListComptDialog::NSListComptDialog(TWindow* pere, NSContexte* pCtx, NSPersonInfo* pPat)
							    :NSUtilDialog(pere, pCtx, "IDD_LISTCOMPT", pNSResModule)
{
	pPatient		= new NSPersonInfo(*pPat) ;

	pMonnaie		= new TGroupBox(this, IDC_LC_MONNAIE) ;
	pLocal			= new TRadioButton(this, IDC_LC_LOC, pMonnaie) ;
  pEuro				= new TRadioButton(this, IDC_LC_EURO, pMonnaie) ;
	pTotalDu		= new NSUtilEdit2(pContexte, this, IDC_LC_TOTALDU, 8) ;
  pTotalPaye	= new NSUtilEdit2(pContexte, this, IDC_LC_TOTALPAYE, 8) ;

	pListeCompt = new NSListComptWindow(this, IDC_LC_LW) ;

	pComptArray = new NSComptArray ;
  nbCompt 		= 0 ;
  pVar        = new NSVarCompta(pCtx) ;

	bErreur = !InitComptArray() ;
}

//// Destructeur
//
NSListComptDialog::~NSListComptDialog()
{
	delete pPatient ;
	delete pMonnaie ;
  delete pLocal ;
  delete pEuro ;
  delete pTotalDu ;
  delete pTotalPaye ;
  delete pListeCompt ;
  delete pComptArray ;
	delete pVar ;
}

//// Fonction SetupWindow
//
void
NSListComptDialog::SetupWindow()
{
	NSUtilDialog::SetupWindow() ;

	pLocal->SetCaption(pVar->getSigle().c_str()) ;

	if (pVar->getMonnaieRef() == MONNAIE_LOCALE)
	{
		pLocal->Check() ;
		bEuro = false ;
	}
  else
  {
  	pEuro->Check() ;
    bEuro = true ;
  }

  InitListeCompt() ;
  AfficheListeCompt() ;
	AfficheTotaux() ;
}

boolNSListComptDialog::InitComptArray()
{
	NSCompt compt(pContexte->getSuperviseur()) ;

	compt.lastError = compt.open() ;
	if (compt.lastError != DBIERR_NONE)	{
		erreur("Erreur à l'ouverture de la base Compt.", standardError, compt.lastError) ;
    return false ;
  }

	string sCodePatient = pPatient->getNss() ;
	compt.lastError = compt.chercheClef(&sCodePatient,
                                      "PATCOMPT",
                                      NODEFAULTINDEX,
                                      keySEARCHEQ,
                                      dbiWRITELOCK) ;

	if ((compt.lastError != DBIERR_NONE) && (compt.lastError != DBIERR_RECNOTFOUND))	{
		erreur("Erreur à la recherche de la fiche Compt du patient.", standardError, compt.lastError) ;
    compt.close() ;
		return false ;
  }

  if (compt.lastError == DBIERR_RECNOTFOUND)
  {
  	erreur("Ce patient n'a pas de fiche comptable.", standardError, compt.lastError) ;
    compt.close() ;
		return true ;
  }

  while (compt.lastError != DBIERR_EOF)
  {
  	compt.lastError = compt.getRecord() ;
    if (compt.lastError != DBIERR_NONE)
    {
    	erreur("Erreur à la lecture d'une fiche Compt.", standardError, compt.lastError) ;
      compt.close() ;
      return false ;
    }

    if (sCodePatient != compt._Donnees.getPatientId())
    	break ;

    // on remplit le tableau
    pComptArray->push_back(new NSComptInfo(&compt)) ;
    nbCompt++ ;

    // ... on passe au composant suivant
    compt.lastError = compt.suivant(dbiWRITELOCK) ;
    if ((compt.lastError != DBIERR_NONE) && (compt.lastError != DBIERR_EOF))
    {
    	erreur("Erreur d'acces à une fiche Compt.", standardError, compt.lastError) ;
      compt.close() ;
      return false ;
    }
  } // fin du while (recherche des composants images)

  // on ferme la base CARTE_SV2
  compt.lastError = compt.close() ;
  if (compt.lastError != DBIERR_NONE)
  {
  	erreur("Erreur de fermeture du fichier Compt.", standardError, compt.lastError) ;
    return false ;
  }

  return true ;
}

voidNSListComptDialog::InitListeCompt()
{
	TListWindColumn colDate("Date examen", 80, TListWindColumn::Left) ;
	pListeCompt->InsertColumn(0, colDate) ;
  TListWindColumn colExam("Examen", 80, TListWindColumn::Left) ;
  pListeCompt->InsertColumn(1, colExam) ;
  TListWindColumn colSommeDue("Somme dûe", 80, TListWindColumn::Right) ;
  pListeCompt->InsertColumn(2, colSommeDue) ;
  TListWindColumn colSommePayee("Somme payée", 80, TListWindColumn::Right) ;
  pListeCompt->InsertColumn(3, colSommePayee) ;
}

void
NSListComptDialog::AfficheListeCompt()
{
	pListeCompt->DeleteAllItems() ;

	string sLang = pContexte->getUserLanguage() ;

	for (int i = nbCompt - 1 ; i >= 0 ; i--)
  {
    string sDateCompt = donne_date(((*pComptArray)[i])->_Donnees.date, sLang) ;
   	TListWindItem Item(sDateCompt.c_str(), 0) ;
    pListeCompt->InsertItem(Item) ;
  }
}

voidNSListComptDialog::DispInfoListeCompt(TLwDispInfoNotify& dispInfo)
{
    const int 	    BufLen = 255;
    static char     buffer[BufLen];
    TListWindItem&  dispInfoItem = *(TListWindItem*)&dispInfo.item;
    int 		    index;
    int			    montant;
    string		    sCodeLexique;
    string		    sBuffer;

    index = dispInfoItem.GetIndex();

    string sLang = pContexte->getUserLanguage() ;
    // Affiche les informations en fonction de la colonne

    switch (dispInfoItem.GetSubItem())
    {
   	    case 1: 	// examen
      	    // si le code examen existe, on récupère son libellé
      	    if (strcmp(((*pComptArray)[index])->_Donnees.examen, ""))
            {
         	    sprintf(buffer, "%s%s", ((*pComptArray)[index])->_Donnees.examen,
                                    ((*pComptArray)[index])->_Donnees.synonyme);
         	    sCodeLexique = string(buffer);
         	    pContexte->getDico()->donneLibelle(sLang, &sCodeLexique, &sBuffer);
         	    strcpy(buffer, sBuffer.c_str());
         	    if (strlen(buffer))
         		    buffer[0] = pseumaj(buffer[0]);
            }
            else
                strcpy(buffer, "");

            dispInfoItem.SetText(buffer);
            break;

   	    case 2: // somme due

            if (!bEuro)
         	    sprintf(buffer, "%s", ((*pComptArray)[index])->_Donnees.duFranc);
            else
         	    sprintf(buffer, "%s", ((*pComptArray)[index])->_Donnees.duEuro);

            montant = atoi(buffer);

            sprintf(buffer, "%d,%02d", montant/100, montant%100);
            dispInfoItem.SetText(buffer);
            break;

        case 3: // somme payee

            if (!bEuro)
         	    sprintf(buffer, "%s", ((*pComptArray)[index])->_Donnees.payeFranc);
            else
         	    sprintf(buffer, "%s", ((*pComptArray)[index])->_Donnees.payeEuro);

            montant = atoi(buffer);

            sprintf(buffer, "%d,%02d", montant/100, montant%100);
            dispInfoItem.SetText(buffer);
            break;

    } // fin du switch
}

voidNSListComptDialog::EvSize(uint sizeType, ClassLib::TSize& size){	TDialog::EvSize(sizeType, size) ;}
voidNSListComptDialog::AfficheTotaux()
{
	char montantDu[10] ;
  char montantPaye[10] ;

	totalDu = 0 ;
  totalPaye = 0 ;

	for (int i = 0; i < nbCompt; i++)
  {
   	if (!bEuro)
    {
    	totalDu		+= atoi(((*pComptArray)[i])->_Donnees.duFranc) ;
      totalPaye	+= atoi(((*pComptArray)[i])->_Donnees.payeFranc) ;
    }
    else
    {
    	totalDu		+= atoi(((*pComptArray)[i])->_Donnees.duEuro) ;
      totalPaye	+= atoi(((*pComptArray)[i])->_Donnees.payeEuro) ;
    }
  }

	sprintf(montantDu, "%5d,%02d", totalDu/100, totalDu%100) ;
  sprintf(montantPaye, "%5d,%02d", totalPaye/100, totalPaye%100) ;

	pTotalDu->SetText(montantDu) ;
	pTotalPaye->SetText(montantPaye) ;
}

voidNSListComptDialog::SwitchLocEuro()
{
	if (bEuro)
		bEuro = false ;
	else
		bEuro = true ;

	AfficheListeCompt() ;
	AfficheTotaux() ;
}

void
NSListComptDialog::CmModifCompt()
{
  int index = pListeCompt->IndexItemSelect() ;

  if (-1 == index)
  {
    erreur("Vous devez sélectionner une fiche compt.", standardError, 0) ;
    return ;
  }

  string      sNumCompt = ((*pComptArray)[index])->_Donnees.getNumCompt() ;
  NSComptInfo ComptInfo ;

  NSComptaPatient compta(pContexte, pPatient) ;
  if (false == compta.EditCompt(sNumCompt, this, &ComptInfo))
    return ;

/*
    CreerFicheComptDialog* 	pComptDlg;
   	NSCompt* pCompt = new NSCompt(pContexte->getSuperviseur());

   	pCompt->lastError = pCompt->open();
   	if (pCompt->lastError != DBIERR_NONE)
   	{
   		erreur("Erreur à l'ouverture de la base Compt.", standardError, 0) ;
      	delete pCompt;
      	return;
   	}

   	pCompt->lastError = pCompt->chercheClef(&sNumCompt,
												"",
												0,
												keySEARCHEQ,
                                                dbiWRITELOCK);

   	if (pCompt->lastError != DBIERR_NONE)
   	{
   		erreur("Erreur à la recherche d'une fiche compt.", standardError, pCompt->lastError) ;
      	pCompt->close();
      	delete pCompt;
      	return;
   	}

   	pCompt->lastError = pCompt->getRecord();
   	if (pCompt->lastError != DBIERR_NONE)
   	{
   		erreur("Erreur à lecture du fichier Compt.db", standardError, pCompt->lastError);
      	pCompt->close();
      	delete pCompt;
      	return;
   	}

   	pComptDlg = new CreerFicheComptDialog(this, pContexte, pPatient, false);

	*(pComptDlg->pData) = *(pCompt->pDonnees);

   	if ((pComptDlg->Execute()) == IDCANCEL)
   	{
    	pCompt->close();
      	delete pCompt;
   		delete pComptDlg;
   		return;
   	}

   	*(pCompt->pDonnees) = *(pComptDlg->pData);

   	pCompt->lastError = pCompt->modifyRecord(TRUE);
   	if (pCompt->lastError != DBIERR_NONE)
   	{
   		erreur("Erreur à la modification de la fiche compt.", standardError, pCompt->lastError) ;
      	pCompt->close();
      	delete pCompt;
      	delete pComptDlg;
      	return;
   	}

   	// on remet à jour l'array
   	*(((*pComptArray)[index])->pDonnees) = *(pCompt->pDonnees);

   	pCompt->lastError = pCompt->close();
   	if (pCompt->lastError != DBIERR_NONE)
   		erreur("Erreur à la fermeture de la base Compt.db.", standardError, pCompt->lastError) ;

    pComptDlg->EnregDonneesCompt(sNumCompt);

   	delete pCompt;
   	delete pComptDlg;
*/

  // on remet à jour l'array
  ((*pComptArray)[index])->_Donnees = ComptInfo._Donnees ;

  AfficheListeCompt() ;
  AfficheTotaux() ;
}

void
NSListComptDialog::CmDeleteCompt()
{
	int indexDel = pListeCompt->IndexItemSelect() ;
  NSComptIter i ;
  int         j ;

  sNumCompt = ((*pComptArray)[indexDel])->_Donnees.getNumCompt() ;

  // on commence par enlever la fiche compt du tableau
  for (i = pComptArray->begin(), j = 0; i != pComptArray->end(); i++, j++)
  {
    if (j == indexDel)
    {
      delete *i ;
      pComptArray->erase(i) ;
      nbCompt -= 1 ;
      break ;
    }
  }

  NSComptaPatient compta(pContexte, pPatient) ;
  compta.DeleteCompt(sNumCompt) ;

  // On remet à jour la liste et les totaux
  AfficheListeCompt() ;
  AfficheTotaux() ;
}

voidNSListComptDialog::CmOk()
{
	NSUtilDialog::CmOk();
}

voidNSListComptDialog::CmCancel()
{
	NSUtilDialog::CmCancel();
}

// fin de nsfseuti.cpp///////////////////////////////////////////////////////

