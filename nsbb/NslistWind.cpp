// -----------------------------------------------------------------------------// NslistWind.cpp// -----------------------------------------------------------------------------
// Fenetre ListWindow
// -----------------------------------------------------------------------------
// $Revision: 1.23 $
// $Author: pameline $
// $Date: 2016/12/29 16:21:49 $
// -----------------------------------------------------------------------------
// FLP - octobre 2004
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

#include "partage\nsdivfct.h"
#include "nsbb\nsbbtran.h"
#include "nsbb\nsbbitem.h"
#include "nsbb\nsbb.h"
#include "nssavoir\nspatho.h"
#include "nssavoir\nsconver.h"
#include "nssavoir\nsguide.h"
#include "nsbb\nstrnode.rh"
#include "nsbb\nsbb.rh"
#include "nsbb\nStlibre.h"
#include "nsbb\nsbb_dlg.h"
#include "nsbb\nspardlg.h"
#include "nsbb\nslistwind.h"
#include "nsbb\nsednum.h"
#include "dcodeur\decoder.h"
#include "nautilus\nssoapview.h"
#include "nautilus\nshistdo.h"
#include "nsbb\nsmanager.h"
#include "nsepisod\nsldvuti.h"
#include "nsbb\ns_skins.h"
#include "nsbb\nsbb_admin.rh"

# include "pilot\NautilusPilot.hpp"
# include "nsbb\tagNames.h"
# include "partage\ns_timer.h"

#if !defined(OWL_LISTWIND_H)
# include <owl/listwind.h>
#endif

#define IDC_LISTBOX    101

long NSHistoryValueManagement::lObjectCount = 0 ;

// -----------------------------------------------------------------------------
// Class NSSkinableListWindow
// -----------------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NSSkinableListWindow, TListWindow)
END_RESPONSE_TABLE ;

NSSkinableListWindow::NSSkinableListWindow(TWindow* parent, NSContexte* pCtx,
                                    int id, int x, int y, int w, int h,
                                    TModule* module)
             :TListWindow(parent, id, x, y, w, h, module), NSRoot(pCtx)
{
}

NSSkinableListWindow::NSSkinableListWindow(TWindow* parent, NSContexte* pCtx,
                                    int resourceId, TModule* module)
             :TListWindow(parent, resourceId, module), NSRoot(pCtx)
{
}

NSSkinableListWindow::~NSSkinableListWindow()
{
}

void
NSSkinableListWindow::SetupWindow()
{
  TListWindow::SetupWindow() ;
}

bool
NSSkinableListWindow::skinSwitch(string sSkinName)
{
  nsSkin* pSkin = pContexte->getSkins()->donneSkin(sSkinName) ;

  if ((nsSkin*) NULL == pSkin)
    return false ;

  ClassLib::TColor* pBackColor     = pSkin->getBackColor() ;
  ClassLib::TColor* pBackTextColor = pSkin->getFontBackColor() ;
  ClassLib::TColor* pTextColor     = pSkin->getFontColor() ;

  if (pBackColor && pBackTextColor)
  {
    SetBkColor(*pBackColor) ;
    SetTextBkColor(*pBackTextColor) ;
    if (pTextColor)
      SetTextColor(*pTextColor) ;

    return true ;
  }

  return false ;
}

void
NSSkinableListWindow::skinSwitchOn(string sSkinName)
{
  bool bSkinable = skinSwitch(sSkinName) ;

  if (false == bSkinable)
  {
    SetBkColor(0x00fff0f0) ; // 0x00bbggrr
    SetTextBkColor(0x00fff0f0) ;
    SetTextColor(0x00000000) ;
  }

  Invalidate() ;
}

void
NSSkinableListWindow::skinSwitchOff(string sSkinName)
{
  bool bSkinable = skinSwitch(sSkinName) ;

  if (false == bSkinable)
  {
    SetBkColor(0x00ffffff) ; // 0x00bbggrr
    SetTextBkColor(0x00ffffff) ;
    SetTextColor(0x00000000) ;
  }

  Invalidate() ;
}

// --------------------------------------------------------------------------
//
// Class NSAdrListWindow
//
// --------------------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSAdrListWindow, TListWindow)
    EV_WM_SETFOCUS,
    EV_WM_KILLFOCUS,
    EV_WM_RBUTTONDOWN,
  	EV_WM_LBUTTONDBLCLK,  //double click
    NS_LVN_GETDISPINFO(LvnGetDispInfo),
    EV_WM_KEYDOWN,
    EV_COMMAND(IDC_NEWADR, CmCreerAdr),
    // EV_COMMAND(IDC_CORADR, CmCorrecAdr),
    EV_COMMAND(IDC_CHGADR, CmChangeAdr),
    EV_COMMAND(IDC_CORADR, CmModifAdr),
    EV_COMMAND(IDC_DELADR, CmSupprAdr),
END_RESPONSE_TABLE;

NSAdrListWindow::NSAdrListWindow(TWindow* parent, NSContexte* pCtx, int id,
                                    int x, int y, int w, int h, TModule* module)
                :NSSkinableListWindow(parent, pCtx, id, x, y, w, h, module)
{
try
{
    pVectData   = new NSVectFatheredPatPathoArray() ;
    pVectLib    = new VecteurString() ;
    pVectTel    = new VecteurString() ;
    pControle   = 0 ;
    bSetupWindow = false ;

    Attr.Style |= LVS_REPORT | LVS_SHOWSELALWAYS ;
    Attr.ExStyle |= WS_EX_NOPARENTNOTIFY ;
}
catch (...)
{
	erreur("Exception NSAdrListWindow ctor 1.", standardError, 0) ;
}
}

NSAdrListWindow::NSAdrListWindow(TWindow* parent, NSContexte* pCtx, int resourceId,
                           TModule* module)
                :NSSkinableListWindow(parent, pCtx, resourceId, module)
{
try
{
    pVectData   = new NSVectFatheredPatPathoArray() ;
    pVectLib    = new VecteurString() ;
    pVectTel    = new VecteurString() ;
    pControle   = 0 ;
    bSetupWindow = false ;

    Attr.Style |= LVS_REPORT | LVS_SHOWSELALWAYS ;
    Attr.ExStyle |= WS_EX_NOPARENTNOTIFY ;
}
catch (...)
{
    erreur("Exception NSAdrListWindow ctor 2.", standardError, 0) ;
}
}

NSAdrListWindow::~NSAdrListWindow()
{
    if (pVectData)
        delete pVectData;

    if (pVectLib)
        delete pVectLib;

    if (pVectTel)
        delete pVectTel;

    if (pControle)
        delete pControle;
}

void
NSAdrListWindow::EvSetFocus(HWND hWndLostFocus)
{
    skinSwitchOn("adressesListOn") ;
    pControle->activateParent();
    TListWindow::EvSetFocus(hWndLostFocus) ;
}

//
// Perte du focus.
//
void
NSAdrListWindow::EvKillFocus(HWND hWndGetFocus)
{
    skinSwitchOff("adressesListOff") ;
    TListWindow::EvKillFocus(hWndGetFocus) ;
}

void
NSAdrListWindow::EvRButtonDown(uint /* modkeys */, NS_CLASSLIB::TPoint& point)
{
	if (!pVectData)
		return ;

try
{
	// création d'un menu popup
	NS_CLASSLIB::TPoint lp = point ;

	TPopupMenu *menu = new TPopupMenu() ;

	if (pVectData->empty())
	{
  	menu->AppendMenu(MF_STRING, IDC_NEWADR, "Créer adresse") ;
    menu->AppendMenu(MF_GRAYED, 0, "Corriger adresse") ;
    // menu->AppendMenu(MF_GRAYED, 0, "Type d'adresse");
    menu->AppendMenu(MF_GRAYED, 0, "Réinitialiser adresse") ;
    menu->AppendMenu(MF_GRAYED, 0, "Supprimer adresse") ;
	}
	else
	{
		// menu->AppendMenu(MF_STRING, IDC_NEWADR, "Créer adresse");
    menu->AppendMenu(MF_GRAYED, 0, "Créer adresse") ;
    menu->AppendMenu(MF_STRING, IDC_CORADR, "Corriger adresse") ;
    // menu->AppendMenu(MF_STRING, IDC_TYPADR, "Type d'adresse");
    menu->AppendMenu(MF_STRING, IDC_CHGADR, "Réinitialiser adresse") ;
    menu->AppendMenu(MF_STRING, IDC_DELADR, "Supprimer adresse") ;
	}

	ClientToScreen(lp) ;
	menu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, lp, 0, HWindow) ;
	delete menu ;
}
catch (...)
{
    erreur("Exception NSAdrListWindow::EvRButtonDown.", standardError, 0) ;
}
}

void
NSAdrListWindow::EvLButtonDblClk(uint /* modKeys */, NS_CLASSLIB::TPoint& point)
{
	if (!pVectData)
		return ;

	TLwHitTestInfo info(point) ;

	HitTest(info) ;
	if (info.GetFlags() & LVHT_ONITEM)		CmModifAdr() ;	// else	else if (pVectData->empty())		CmCreerAdr() ;}

void
NSAdrListWindow::EvKeyDown(uint key, uint repeatCount, uint flags)
{
	if (!pVectData)
		return ;

	if      (key == VK_DELETE)
		CmSupprAdr() ;
	else if (key == VK_INSERT)
	{
		if (GetKeyState(VK_SHIFT) < 0)
    	CmModifAdr() ;
    // else
    else if (pVectData->empty())
    	CmCreerAdr() ;
	}
	else
		TListWindow::EvKeyDown(key, repeatCount, flags) ;
}

void
NSAdrListWindow::SetupWindow()
{
    ListView_SetExtendedListViewStyle(this->HWindow, LVS_EX_FULLROWSELECT) ;

    TListWindow::SetupWindow();
    SetupColumns();
    // On recharge les données à partir du pTransfert
    // *pVectData = *(pControle->getTransfert()->getPatPatho());
    // On rafraichit la liste
    // AfficheListe();
    bSetupWindow = true;

    skinSwitchOff("adressesListOff") ;
}

void
NSAdrListWindow::SetupColumns()
{
    InsertColumn(0, TListWindColumn("Adresse", 120));
    InsertColumn(1, TListWindColumn("Téléphone", 100));
    InsertColumn(2, TListWindColumn("Libelle", 300));
}

int
NSAdrListWindow::IndexItemSelect()
{
    int count = GetItemCount();
    int index = -1;

    for (int i = 0; i < count; i++)    {
   	    if (GetItemState(i, LVIS_SELECTED))        {
      	    index = i;
            break;
        }
    }

    return index;}

string
NSAdrListWindow::TrouveLibelle(NSPatPathoArray* pPatPathoArray)
{
    if ((!pPatPathoArray) || (pPatPathoArray->empty()))
        return "";

    PatPathoIter    iter ;
    string          sElemLex, sSens, sType ;
    string          sLibelle = "" ;
    string          sLang  = pContexte->getUserLanguage() ;

    // Chaque patpatho est une adresse de racine fille d'un élément LADRE
    // Les chapitres recherchés commencent en colonne 0
    iter = pPatPathoArray->begin();
    int iColBase = (*iter)->getColonne() ;

    while ((iter != pPatPathoArray->end()) && ((*iter)->getColonne() >= iColBase))
    {
        sElemLex = (*iter)->getLexique();
        pContexte->getDico()->donneCodeSens(&sElemLex, &sSens);

        // Chapitre "type" / Type chapter
        if (sSens == string("LTYPA"))
        {
            iter++;
            while ((iter != pPatPathoArray->end()) && ((*iter)->getColonne() > iColBase))
            {
                // on récupère ici le code lexique pour le libelle
                // attention cette liste est également utilisée pour les correspondants (LAZRE, LSECO)
                // ou pour les patients (URESI, USECO, UTEMP, ULIEO)
                sElemLex = (*iter)->getLexique() ;
                pContexte->getDico()->donneCodeSens(&sElemLex, &sType) ;
                pContexte->getDico()->donneLibelle(sLang, &sElemLex, &sLibelle) ;

                iter++;
            }

            if (sLibelle != "")
                break;
        }
        else
            iter++;
    }

    return sLibelle;
}

bool
NSAdrListWindow::TrouveObjectAdr(NSPatPathoArray* pPatPathoArray, string& sObject, string& sChez)
{
  if (((NSPatPathoArray*) NULL == pPatPathoArray) || (pPatPathoArray->empty()))
    return false ;

  sObject = string("") ;
  sChez   = string("") ;

  // Chaque patpatho est une adresse de racine fille d'un élément LADRE
  // Les chapitres recherchés commencent en colonne 0
  PatPathoIter iter = pPatPathoArray->begin() ;
  int iColBase = (*iter)->getColonne() ;

  bool bObject = false ;

  while ((pPatPathoArray->end() != iter) && ((*iter)->getColonne() >= iColBase))
  {
    string sElemLex = (*iter)->getLexique() ;
    string sSens    = string("") ;
    pContexte->getDico()->donneCodeSens(&sElemLex, &sSens) ;

    // Chapitre "lieu" / place chapter
    if (sSens == string("LLIEU"))
    {
      iter++;
      while ((iter != pPatPathoArray->end()) && ((*iter)->getColonne() > iColBase))
      {
        // on cherche ici un objet
        sElemLex = (*iter)->getLexique() ;
        if (sElemLex == string("£OBJE1"))
        {
          sObject = (*iter)->getComplement() ;
          bObject = true ;
        }
        iter++;
      }
    }
    else if (sSens == string("LQUIO"))
    {
      sChez = pPatPathoArray->getFreeTextValue(&iter) ;

      if (string("") != sChez)
        sChez = string("Chez ") + sChez ;
    }
    else
      iter++ ;
  }

  return bObject ;
}

void
NSAdrListWindow::TrouveLibelleAdr(string sObject, string& sLib, string& sTel)
{
try
{
  NSAddressGraphManager *pAddressManager = new NSAddressGraphManager(pContexte->getSuperviseur()) ;
  pAddressManager->getGraphAdr(sObject) ;
  sLib = pAddressManager->trouveLibCourtAdr(pContexte, pContexte->getUserLanguage()) ;
  sTel = pAddressManager->trouveTelAdr() ;
  delete pAddressManager ;
}
catch (...)
{
  erreur("Exception NSAdrListWindow::TrouveLibelleAdr.", standardError, 0) ;
}
}

void
NSAdrListWindow::AfficheListe()
{
try
{
	char libelle[255] ;

	DeleteAllItems() ;	pVectLib->vider() ;	pVectTel->vider() ;	if (!pVectData || (pVectData->estVide()))		return ;	FatheredPatPathoIterVect itPPT = pVectData->begin() ;	for ( ; itPPT != pVectData->end() ; itPPT++)	{  	// Libellé de l'adresse    string sObject, sChez, sLib = "" ;    string sLibAdr, sTel ;    if (TrouveObjectAdr((*itPPT)->getPatPatho(), sObject, sChez))
    {
    	if (sChez != "")
      	sLib += (sChez + string(" : ")) ;

      TrouveLibelleAdr(sObject, sLibAdr, sTel) ;

      sLib += sLibAdr ;
    }
    else
    {
    	sLib = string("inconnue") ;
      sTel = string("[vide]") ;
    }

    pVectLib->AddString(sLib) ;    pVectTel->AddString(sTel) ;	}
	for (int i = pVectData->size() - 1; i >= 0; i--)	{
		// principale, secondaire
    string sLibelle = TrouveLibelle(((*pVectData)[i])->getPatPatho()) ;
    if (sLibelle != "")
    {
    	strcpy(libelle, sLibelle.c_str()) ;      libelle[0] = pseumaj(libelle[0]) ;    }    else    	strcpy(libelle, "[adresse]") ;    InsertItem(TListWindItem(libelle, 0)) ;	}
}
catch (...)
{
	erreur("Exception NSAdrListWindow::AfficheListe.", standardError, 0) ;
}
}

void
NSAdrListWindow::LvnGetDispInfo(TLwDispInfoNotify& dispInfo)
{
    const int BufLen = 255;
    static char buffer[BufLen];
    TListWindItem& dispInfoItem = *(TListWindItem*)&dispInfo.item;
    int index;

    index = dispInfoItem.GetIndex();
    // Affiche les informations en fonction de la colonne    switch (dispInfoItem.GetSubItem())
    {
        case 1:     // Téléphone

            sprintf(buffer, "%s", (pVectTel->GetElementAt(index)).c_str()) ;
            dispInfoItem.SetText(buffer) ;
            break ;

        case 2: 	// libellé

            sprintf(buffer, "%s", (pVectLib->GetElementAt(index)).c_str()) ;            dispInfoItem.SetText(buffer) ;
            break ;
    }
}

void
NSAdrListWindow::activeControle(int activation, Message* /* pMessage */)
{
	// on ne gère pas le cas BF_CHECKED avant SetupWindow() pour éviter que les nouveaux fils
	// n'activent la liste en cours de création.
	// le cas BF_UNCHECKED provient en principe d'un père du controle de liste
	// on le conserve pour permettre à la liste de se vider si on la désactive.
	if (!pVectData || !bSetupWindow)
  	return ;

	if (activation == BF_CHECKED)
	{
  	// On recharge les données à partir du pTransfert
    *pVectData = *(pControle->getTransfert()->getPatPatho()) ;
    bSetupWindow = false ;
	}
	if (activation == BF_UNCHECKED)
  	// Le controle de liste est désactivé : on vide l'array des données
		pVectData->vider() ;

	AfficheListe() ;
}

void
NSAdrListWindow::CmCreerAdr()
{
  // on crée ici un nouveau "fils" au pBBFilsItem lié au controle de liste
  // cela crée en fait un nouveau BBItem dans le VecteurFils de pBBFilsItem
  // puis active le BBItem qui lance l'archetype d'édition d'adresse à l'aide
  // d'un fil guide.
  if (pControle->getTransfert())
  {
    pControle->getTransfert()->getFilsItem()->creerNouveauFils() ;

    // on récupère le vecteur de transfert, auquel on vient d'ajouter un fils (une patpatho)
    // si l'utilisateur est sorti par CmCancel(), cette dernière patpatho est vide
    // on doit donc faire l'inverse de creerNouveauFils() : vider la patpatho du transfert
    // et détruire le dernier BBItem du pBBFilsItem père.
    NSVectFatheredPatPathoArray* pVectTransfert    = pControle->getTransfert()->getPatPatho() ;
    NSVectFatheredPatPathoArray* pTmpVectTransfert = pControle->getTransfert()->getTmpPatho() ;

    if (!pVectTransfert->empty())
    {
      FatheredPatPathoIterVect iterVect = pVectTransfert->end() ;
      iterVect-- ;
      if ((*iterVect)->getPatPatho()->empty())
      {
        // on détruit la dernière patpatho ainsi que la dernière patpatho temporaire
        delete (*iterVect) ;
        pVectTransfert->erase(iterVect) ;

        FatheredPatPathoIterVect iterTmpVect = pTmpVectTransfert->end() ;
        iterTmpVect-- ;
        delete (*iterTmpVect) ;
        pTmpVectTransfert->erase(iterTmpVect) ;
      }

      // on vide le vecteur des fils (on delete celui que l'on vient de créer)
      // car ce vecteur doit être vide pour permettre la modification
      pControle->getTransfert()->getFilsItem()->tuerFils();

      // On recharge les données à partir du pTransfert
      *pVectData = *pVectTransfert;
      // On rafraichit la liste
      AfficheListe();
    }
  }
}

void
NSAdrListWindow::CmModifAdr()
{
	int index = IndexItemSelect();

	if (index == -1)	{
		erreur("Vous devez sélectionner une adresse à modifier.", standardError, 0) ;
		return ;
	}

	// on envoie ici un ctrlNotification à l'elt index de pVectData
	// ce qui lance l'infernal processus de modification de l'adresse correspondante
	if (pControle && pControle->getTransfert())
	{
		pControle->getTransfert()->ctrlNotification(BF_UNCHECKED, BF_CHECKED, 0, index) ;

		// On recharge les données à partir du pTransfert
		*pVectData = *(pControle->getTransfert()->getPatPatho()) ;
		// On rafraichit la liste
		AfficheListe() ;
	}
}

void
NSAdrListWindow::CmChangeAdr()
{
  int index = IndexItemSelect() ;
  int k = 0 ;

  if (index == -1)  {
    erreur("Vous devez sélectionner l'adresse à réinitialiser.",standardError, 0) ;
    return ;
  }

  string sText = pContexte->getSuperviseur()->getText("addressManagement", "changeAddressWarning") ;
  int retVal = ::MessageBox(0, sText.c_str(), "Message Episodus", MB_YESNO);
  if (retVal == IDNO)
    return ;

  // on doit supprimer la patpatho n°index du vecteur de transfert
  NSVectFatheredPatPathoArray* pVectTransfert = pControle->getTransfert()->getPatPatho() ;

  for (FatheredPatPathoIterVect i = pVectTransfert->begin(); i != pVectTransfert->end(); i++)
  {
    if (k == index)
    {
      delete (*i) ;
      pVectTransfert->erase(i) ;
      break ;
    }

    k++ ;
  }

  // On recharge les données à partir du pTransfert
  *pVectData = *(pControle->getTransfert()->getPatPatho());

  // On lance la fonction Créer, qui lance la création d'une nouvelle adresse
  // pour remplacer le lieu supprimé précédemment
  CmCreerAdr() ;
}

void
NSAdrListWindow::CmCorrecAdr()
{
}

void
NSAdrListWindow::CmSupprAdr()
{
    int index = IndexItemSelect();
    int k = 0;

   	if (index == -1)   	{
   		erreur("Vous devez sélectionner une adresse à supprimer.",standardError, 0) ;
      	return;
   	}

    string sText = pContexte->getSuperviseur()->getText("addressManagement", "deleteAddressWarning") ;
    int retVal = ::MessageBox(0, sText.c_str(), "Message Episodus", MB_YESNO);
    if (retVal == IDNO)
        return;

    // on doit supprimer la patpatho n°index du vecteur de transfert
    NSVectFatheredPatPathoArray* pVectTransfert = pControle->getTransfert()->getPatPatho();

    for (FatheredPatPathoIterVect i = pVectTransfert->begin(); i != pVectTransfert->end(); i++)
    {
        if (k == index)
        {
            delete (*i);
            pVectTransfert->erase(i);
            break;
        }

        k++;
    }

    // On recharge les données à partir du pTransfert
    *pVectData = *(pControle->getTransfert()->getPatPatho());
    // On rafraichit la liste
    AfficheListe();
}

bool
NSAdrListWindow::canWeClose()
{
  return true ;
}

// --------------------------------------------------------------------------
//
// Class NSCorListWindow
//
// --------------------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSCorListWindow, TListWindow)
    EV_WM_SETFOCUS,
    EV_WM_KILLFOCUS,
    EV_WM_RBUTTONDOWN,
  	EV_WM_LBUTTONDBLCLK,  //double click
    NS_LVN_GETDISPINFO(LvnGetDispInfo),
    EV_WM_KEYDOWN,
    EV_COMMAND(IDC_NEWCORR, CmCreerCorr),
    EV_COMMAND(IDC_MODCORR, CmModifCorr),
    EV_COMMAND(IDC_DELCORR, CmSupprCorr),
END_RESPONSE_TABLE;

NSCorListWindow::NSCorListWindow(TWindow* parent, NSContexte* pCtx, int id,
                                    int x, int y, int w, int h, TModule* module)
                :NSSkinableListWindow(parent, pCtx, id, x, y, w, h, module)
{
  pVectData   = new NSVectFatheredPatPathoArray() ;
  pVectLib    = new VecteurString() ;
  pControle   = 0 ;
  bSetupWindow = false ;

  Attr.Style |= LVS_REPORT | LVS_SHOWSELALWAYS ;
  Attr.ExStyle |= WS_EX_NOPARENTNOTIFY ;
}

NSCorListWindow::NSCorListWindow(TWindow* parent, NSContexte* pCtx, int resourceId,
                           TModule* module)
                :NSSkinableListWindow(parent, pCtx, resourceId, module)
{
  pVectData   = new NSVectFatheredPatPathoArray() ;
  pVectLib    = new VecteurString() ;
  pControle   = 0 ;
  bSetupWindow = false ;

  Attr.Style |= LVS_REPORT | LVS_SHOWSELALWAYS ;
  Attr.ExStyle |= WS_EX_NOPARENTNOTIFY ;
}

NSCorListWindow::~NSCorListWindow()
{
  if (pVectData)
    delete pVectData ;

  if (pVectLib)
    delete pVectLib ;

  if (pControle)
    delete pControle ;
}

void
NSCorListWindow::EvSetFocus(HWND hWndLostFocus)
{
  skinSwitchOn("correspListOn") ;
  TListWindow::EvSetFocus(hWndLostFocus) ;
}

//
// Perte du focus.
//
void
NSCorListWindow::EvKillFocus(HWND hWndGetFocus)
{
  skinSwitchOff("correspListOff") ;
  TListWindow::EvKillFocus(hWndGetFocus) ;
}

void
NSCorListWindow::EvRButtonDown(uint /* modkeys */, NS_CLASSLIB::TPoint& point)
{
    // création d'un menu popup
    NS_CLASSLIB::TPoint lp = point;

    TPopupMenu *menu = new TPopupMenu();

  	if (pVectData->empty())
  	{
        menu->AppendMenu(MF_STRING, IDC_NEWCORR, "Créer correspondant");
        menu->AppendMenu(MF_GRAYED, 0, "Modifier correspondant");
  		menu->AppendMenu(MF_GRAYED, 0, "Supprimer correspondant");
    }
    else
    {
        menu->AppendMenu(MF_STRING, IDC_NEWCORR, "Créer correspondant");
        menu->AppendMenu(MF_STRING, IDC_MODCORR, "Modifier correspondant");
  		menu->AppendMenu(MF_STRING, IDC_DELCORR, "Supprimer correspondant");
    }

    ClientToScreen(lp);
  	menu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, lp, 0, HWindow);
  	delete menu;
}

void
NSCorListWindow::EvLButtonDblClk(uint /* modKeys */, NS_CLASSLIB::TPoint& point)
{
    TLwHitTestInfo info(point);

    HitTest(info);
    if (info.GetFlags() & LVHT_ONITEM)        CmModifCorr();    else        CmCreerCorr();}

void
NSCorListWindow::EvKeyDown(uint key, uint repeatCount, uint flags)
{
    if      (key == VK_DELETE)
        CmSupprCorr() ;
    else if (key == VK_INSERT)
    {
        if (GetKeyState(VK_SHIFT) < 0)
            CmModifCorr() ;
        else
            CmCreerCorr() ;
    }
    else
    	TListWindow::EvKeyDown(key, repeatCount, flags) ;
}

void
NSCorListWindow::SetupWindow()
{
    ListView_SetExtendedListViewStyle(this->HWindow, LVS_EX_FULLROWSELECT) ;

    TListWindow::SetupWindow();
    SetupColumns();
    // On recharge les données à partir du pTransfert
    // *pVectData = *(pControle->getTransfert()->getPatPatho());
    // On rafraichit la liste
    // AfficheListe();
    bSetupWindow = true;

    skinSwitchOff("correspListOff") ;
}

void
NSCorListWindow::SetupColumns()
{
    InsertColumn(0, TListWindColumn("Fonction", 120));
    InsertColumn(1, TListWindColumn("Nom", 300));
}

int
NSCorListWindow::IndexItemSelect()
{
    int count = GetItemCount();
    int index = -1;

    for (int i = 0; i < count; i++)    {
   	    if (GetItemState(i, LVIS_SELECTED))        {
      	    index = i;
            break;
        }
    }

    return index;}

string
NSCorListWindow::TrouveLibelle(NSPatPathoArray* pPatPathoArray)
{
  if ((NULL == pPatPathoArray) || (pPatPathoArray->empty()))
    return string("") ;

  string sLibelle = string("") ;

  string          sElemLex, sSens, sType ;

  string          sCode;
  string          sLang = pContexte->getUserLanguage() ;

  // Chaque patpatho est une adresse de racine fille d'un élément LADRE
  // Les chapitres recherchés commencent en colonne 0
  PatPathoIter iter = pPatPathoArray->begin() ;
  int iColBase = (*iter)->getColonne() ;

  while ((pPatPathoArray->end() != iter) && ((*iter)->getColonne() >= iColBase))
  {
    string sSens = (*iter)->getLexiqueSens() ;

    // Chapitre "type" / Type chapter
    if (string("LFONC") == sSens)
    {
      iter++ ;
      while ((pPatPathoArray->end() != iter) && ((*iter)->getColonne() > iColBase))
      {
        // on cherche ici un edit lexique (ayant comme fils un code lexique)
        string sElemLex = (*iter)->getLexique() ;
        if ((string(sElemLex, 0, 3) == string("£C;")) || (string(sElemLex, 0, 3) == string("£CX")))
        {
          iter++ ;
          while ((pPatPathoArray->end() != iter) && ((*iter)->getColonne() > iColBase+1))
          {
            // on cherche ici un code lexique pour un libelle
            string sCode = (*iter)->getLexique() ;
            if (string("£?????") != sCode)
              pContexte->getDico()->donneLibelle(sLang, &sCode, &sLibelle) ;

            iter++ ;
          }
        }
        else
          iter++ ;
      }

      if (string("") != sLibelle)
        break ;
    }
    else
      iter++ ;
  }

  return sLibelle ;
}

string
NSCorListWindow::TrouvePidsCorr(NSPatPathoArray* pPatPathoArray)
{
    if ((!pPatPathoArray) || (pPatPathoArray->empty()))
        return "";

    PatPathoIter    iter ;
    string          sElemLex, sSens;
    string          sPids = "" ;

    // Chaque patpatho est une adresse de racine fille d'un élément LADRE
    // Les chapitres recherchés commencent en colonne 0
    iter = pPatPathoArray->begin();
    int iColBase = (*iter)->getColonne() ;

    while ((iter != pPatPathoArray->end()) && ((*iter)->getColonne() >= iColBase))
    {
        sElemLex = (*iter)->getLexique();
        pContexte->getDico()->donneCodeSens(&sElemLex, &sSens);

        // Chapitre "lieu" / place chapter
        if (sSens == string("HHUMA"))
        {
            iter++;
            while ((iter != pPatPathoArray->end()) && ((*iter)->getColonne() > iColBase))
            {
                // on cherche ici un objet
                sElemLex = (*iter)->getLexique() ;
                if (sElemLex == string("£SPID1"))
                {
                    sPids = (*iter)->getComplement() ;
                    break;
                }
                iter++;
            }
        }
        else
            iter++;
    }

    return sPids;
}

string
NSCorListWindow::TrouveLibelleCorr(string sPids)
{
try
{
	string sLib = "" ;

#ifndef N_TIERS
    NSPersonGraphManager* pGraphManager = new NSPersonGraphManager(pContexte) ;
    string sTreeID = pGraphManager->GetTreeRootFromPids(sPids, pidsCorresp) ;
    pGraphManager->getGraphAdmin(sTreeID, pidsCorresp) ;
    pGraphManager->trouveNomCorresp(sLib) ;
    delete pGraphManager ;
#endif

    return sLib;
}
catch (...)
{
    erreur("Exception NSCorListWindow::TrouveLibelleCorr.", standardError, 0) ;
    return "" ;
}
}

void
NSCorListWindow::AfficheListe()
{
    char libelle[255];

	DeleteAllItems();    pVectLib->vider();    if (pVectData->estVide())        return ;    FatheredPatPathoIterVect itPPT = pVectData->begin() ;    for ( ; itPPT != pVectData->end() ; itPPT++)    {        // Libellé de l'adresse        string sPids = TrouvePidsCorr((*itPPT)->getPatPatho()) ;
        pVectLib->AddString(TrouveLibelleCorr(sPids)) ;    }
	for (int i = pVectData->size() - 1; i >= 0; i--)    {
        // principale, secondaire
        string sLibelle = TrouveLibelle(((*pVectData)[i])->getPatPatho());
        if (sLibelle != "")
        {
            strcpy(libelle, sLibelle.c_str());            libelle[0] = pseumaj(libelle[0]);        }        else            strcpy(libelle, "[vide]");        InsertItem(TListWindItem(libelle, 0));    }
}

void
NSCorListWindow::LvnGetDispInfo(TLwDispInfoNotify& dispInfo)
{
    const int BufLen = 255;
    static char buffer[BufLen];
    TListWindItem& dispInfoItem = *(TListWindItem*)&dispInfo.item;
    int index;

    index = dispInfoItem.GetIndex();
    // Affiche les informations en fonction de la colonne    switch (dispInfoItem.GetSubItem())
    {
        case 1: 	// libellé

            sprintf(buffer, "%s", (pVectLib->GetElementAt(index)).c_str()) ;            dispInfoItem.SetText(buffer);
            break;
    }
}

void
NSCorListWindow::activeControle(int activation, Message* /* pMessage */)
{
    // on ne gère pas le cas BF_CHECKED avant SetupWindow() pour éviter que les nouveaux fils
    // n'activent la liste en cours de création.
    // le cas BF_UNCHECKED provient en principe d'un père du controle de liste
    // on le conserve pour permettre à la liste de se vider si on la désactive.
    if (!bSetupWindow)
        return;

    if (activation == BF_CHECKED)
    {
        // On recharge les données à partir du pTransfert
        *pVectData = *(pControle->getTransfert()->getPatPatho());
        bSetupWindow = false;
    }
    if (activation == BF_UNCHECKED)
    {
        // Le controle de liste est désactivé : on vide
        // l'array des données
        pVectData->vider();
    }

    AfficheListe();
}

void
NSCorListWindow::CmCreerCorr()
{
  // on crée ici un nouveau "fils" au pBBFilsItem lié au controle de liste
  // cela crée en fait un nouveau BBItem dans le VecteurFils de pBBFilsItem
  // puis active le BBItem qui lance l'archetype d'édition d'adresse à l'aide
  // d'un fil guide.
  if (pControle->getTransfert())
  {
    pControle->getTransfert()->getFilsItem()->creerNouveauFils() ;

    // on récupère le vecteur de transfert, auquel on vient d'ajouter un fils (une patpatho)
    // si l'utilisateur est sorti par CmCancel(), cette dernière patpatho est vide
    // on doit donc faire l'inverse de creerNouveauFils() : vider la patpatho du transfert
    // et détruire le dernier BBItem du pBBFilsItem père.
    NSVectFatheredPatPathoArray* pVectTransfert    = pControle->getTransfert()->getPatPatho() ;
    NSVectFatheredPatPathoArray* pTmpVectTransfert = pControle->getTransfert()->getTmpPatho() ;

    if (!pVectTransfert->empty())
    {
      FatheredPatPathoIterVect iterVect = pVectTransfert->end() ;
      iterVect-- ;
      if ((*iterVect)->getPatPatho()->empty())
      {
        // on détruit la dernière patpatho ainsi que la dernière patpatho temporaire
        delete (*iterVect) ;
        pVectTransfert->erase(iterVect) ;

        FatheredPatPathoIterVect iterTmpVect = pTmpVectTransfert->end() ;
        iterTmpVect-- ;
        delete (*iterTmpVect) ;
        pTmpVectTransfert->erase(iterTmpVect) ;
      }

      // on vide le vecteur des fils (on delete celui que l'on vient de créer)
      // car ce vecteur doit être vide pour permettre la modification
      pControle->getTransfert()->getFilsItem()->tuerFils() ;

      // On recharge les données à partir du pTransfert
      *pVectData = *pVectTransfert ;
      // On rafraichit la liste
      AfficheListe() ;
    }
  }
}

void
NSCorListWindow::CmModifCorr()
{
    int index = IndexItemSelect();

   	if (index == -1)   	{
   		erreur("Vous devez sélectionner une adresse à modifier.",standardError, 0) ;
      	return;
   	}

    // on envoie ici un ctrlNotification à l'elt index de pVectData
    // ce qui lance l'infernal processus de modification de l'adresse correspondante
    if (pControle->getTransfert())
    {
        pControle->getTransfert()->ctrlNotification(BF_UNCHECKED, BF_CHECKED, 0, index);

        // On recharge les données à partir du pTransfert
        *pVectData = *(pControle->getTransfert()->getPatPatho());
        // On rafraichit la liste
        AfficheListe();
    }
}

void
NSCorListWindow::CmSupprCorr()
{
  int index = IndexItemSelect() ;
  int k = 0 ;

  if (index == -1)  {
    erreur("Vous devez sélectionner une adresse à supprimer.", standardError, 0) ;
    return ;
  }

  // on doit supprimer la patpatho n°index du vecteur de transfert
  NSVectFatheredPatPathoArray* pVectTransfert = pControle->getTransfert()->getPatPatho() ;

  for (FatheredPatPathoIterVect i = pVectTransfert->begin(); i != pVectTransfert->end(); i++)
  {
    if (k == index)
    {
      delete (*i) ;
      pVectTransfert->erase(i) ;
      break ;
    }

    k++ ;
  }

  // On recharge les données à partir du pTransfert
  *pVectData = *(pControle->getTransfert()->getPatPatho()) ;
  // On rafraichit la liste
  AfficheListe() ;
}

bool
NSCorListWindow::canWeClose()
{
  return true ;
}

// -----------------------------------------------------------------
//  Méthodes de NSTPersonListDialog
// -----------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NSTPersonListDialog, NSUtilDialog)
	EV_COMMAND(IDOK, 									CmOk),
  EV_COMMAND(IDCANCEL, 							CmCancel),
  EV_COMMAND(IDC_LC_CREER, 					CmCreer),
  EV_COMMAND(IDC_LC_SEARCH_NOW, 		SearchNow),
  EV_BN_CLICKED(IDC_LC_PIDSPATIENT, CmSelectPids),
  EV_BN_CLICKED(IDC_LC_PIDSUSER, 		CmSelectPids),
  EV_BN_CLICKED(IDC_LC_PIDSCORRESP, CmSelectPids),
  EV_BN_CLICKED(IDC_LC_PIDSALL, 		CmSelectPids),
  EV_WM_TIMER,
  EV_LVN_GETDISPINFO(IDC_LC_LW,			LvnGetDispInfo),
END_RESPONSE_TABLE;

NSTPersonListDialog::NSTPersonListDialog(TWindow *pere, PIDSTYPE typePids, bool avecCreer,                        NSContexte *pCtx, NSBasicAttributeArray* pCrit, bool bSimpleMode, string sRole, TModule * /*module*/)                    :NSUtilDialog(pere, pCtx, "IDD_LISTCORRESP", pNSDLLModule)
{
	// Création de tous les "objets de contrôle"
  //
  pNom					  = new NSTPersonListEdit(pContexte, this, IDC_LC_NOM,    COR_NOM_LEN,    pNSDLLModule) ;
	pPrenom				  = new NSTPersonListEdit(pContexte, this, IDC_LC_PRENOM, COR_PRENOM_LEN, pNSDLLModule) ;
  pRole           = new NSUtilLexique(pContexte, this, IDC_LC_ROLE, pContexte->getDico()) ;
  pListe				  = new NSTPersonListWindow(this, IDC_LC_LW, pNSDLLModule) ;

  pFamNameTxt     = new OWL::TStatic(this, IDC_LCTXT_NOM,    0, pNSDLLModule) ;
  pGivenNameTxt   = new OWL::TStatic(this, IDC_LCTXT_PRENOM, 0, pNSDLLModule) ;
  pRoleTxt        = new OWL::TStatic(this, IDC_LCTXT_ROLE, 0, pNSDLLModule) ;

  pGroup				  = new OWL::TGroupBox(this, IDC_LC_GROUP, pNSDLLModule) ;
  pPidsPatient	  = new OWL::TRadioButton(this, IDC_LC_PIDSPATIENT, pGroup, pNSDLLModule) ;
  pPidsUtilisat	  = new OWL::TRadioButton(this, IDC_LC_PIDSUSER,    pGroup, pNSDLLModule) ;
  pPidsCorresp	  = new OWL::TRadioButton(this, IDC_LC_PIDSCORRESP, pGroup, pNSDLLModule) ;
  pPidsTous			  = new OWL::TRadioButton(this, IDC_LC_PIDSALL,     pGroup, pNSDLLModule) ;

	pCreer				  = new OWL::TButton(this, IDC_LC_CREER, pNSDLLModule) ;
  pSearchNow			= new OWL::TButton(this, IDC_LC_SEARCH_NOW, pNSDLLModule) ;
  pVScroll        = (NSTPersonListScrollBar*) 0 ;

	// Création du tableau de Patients	pPersonEnCours  = (NSPersonInfo*) 0 ;	pPersonSelect   = new NSPersonInfo(pContexte->getSuperviseur()) ;
	pPersonArray    = new NSPersonArray(pContexte->getSuperviseur()) ;
	pCriteres       = pCrit ;
	sNumSelect      = string("") ;	bCreer          = false ;	bAvecCreer      = avecCreer ;	iTypePids       = typePids ;  bSinglePidsMode = bSimpleMode ;  _sRolePerson    = sRole ;  _sNomPerson     = string("") ;  _sPrenomPerson  = string("") ;  iStepSize       = 1000 ;}

NSTPersonListDialog::~NSTPersonListDialog(){
	delete pNom ;
	delete pPrenom ;
  delete pRole ;
  delete pFamNameTxt ;
  delete pGivenNameTxt ;
  delete pRoleTxt ;
  delete pPidsTous ;
  delete pPidsCorresp ;
  delete pPidsUtilisat ;
  delete pPidsPatient ;
  delete pGroup ;
  delete pListe ;
  delete pCreer ;
  delete pSearchNow ;
  delete pPersonSelect ;
  delete pPersonArray ;
  if (pPersonEnCours)
		delete pPersonEnCours ;
}

// -----------------------------------------------------------------------------// Function    : NSTPersonListDialog::SetupWindow()
// Arguments   : Aucun
// Description : Initialise la boite de dialogue
// Returns     : Rien
// -----------------------------------------------------------------------------
void
NSTPersonListDialog::SetupWindow()
{
	TDialog::SetupWindow() ;

  string sText = pContexte->getSuperviseur()->getText("dialog_patientList", "famillyName") ;
  pFamNameTxt->SetText(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("dialog_patientList", "givenName") ;
  pGivenNameTxt->SetText(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("dialog_patientList", "function") ;
  pRoleTxt->SetText(sText.c_str()) ;

  sText = pContexte->getSuperviseur()->getText("personSearch", "all") ;
  pPidsTous->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("personSearch", "outers") ;
  pPidsCorresp->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("personSearch", "users") ;
  pPidsUtilisat->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("personSearch", "patients") ;
  pPidsPatient->SetCaption(sText.c_str()) ;

  sText = pContexte->getSuperviseur()->getText("personSearch", "createNewPerson") ;
  pCreer->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("personSearch", "search") ;
  pSearchNow->SetCaption(sText.c_str()) ;

  setAccurateCaption() ;

  if      (iTypePids == pidsPatient)
  {
    pPidsPatient->Check() ;
    iSelectPids = 1 ;
  }
  else if (iTypePids == pidsCorresp)
  {
    pPidsCorresp->Check() ;
    iSelectPids = 2 ;
  }
  else if (iTypePids == pidsUtilisat)
  {
    pPidsUtilisat->Check() ;
    iSelectPids = 3 ;
  }
  else
  {
    pPidsTous->Check() ;
    iSelectPids = 0 ;
  }

  pNom->SetText("") ;  pPrenom->SetText("") ;

  if (string("") != _sRolePerson)
  {
    pContexte->getDico()->donneCodeComplet(_sRolePerson) ;
    pRole->setLabel(_sRolePerson) ;
  }

  if (false == bAvecCreer)
  	pCreer->ShowWindow(SW_HIDE) ;

	if (bSinglePidsMode)
	{
		pGroup->ShowWindow(SW_HIDE) ;
		pPidsPatient->ShowWindow(SW_HIDE) ;
		pPidsUtilisat->ShowWindow(SW_HIDE) ;
		pPidsCorresp->ShowWindow(SW_HIDE) ;
		pPidsTous->ShowWindow(SW_HIDE) ;
	}

  if (false == InitRecherche())  	return ;

  // Positionnement du corresp en fin de fichier  // PosLastPerson();

  // Initialisation du tableau des personnes  InitPersonArray() ;

  // Initialisation de la liste  InitListe() ;
  AfficheListe() ;
}

void
NSTPersonListDialog::setAccurateCaption()
{
	string sText = string("") ;

	if      (iTypePids == pidsPatient)
		sText = pContexte->getSuperviseur()->getText("personSearch", "patientSelection") ;
  else if (iTypePids == pidsCorresp)
		sText = pContexte->getSuperviseur()->getText("personSearch", "outerSelection") ;
  else if (iTypePids == pidsUtilisat)
		sText = pContexte->getSuperviseur()->getText("personSearch", "userSelection") ;
  else
		sText = pContexte->getSuperviseur()->getText("personSearch", "personSelection") ;

  SetCaption(sText.c_str()) ;
}

// -----------------------------------------------------------------------------// Function    : NSTPersonListDialog::InitRecherche()// Arguments   : Aucun
// Description : Initialise la recherche des corresp par nom, prenom
// Returns     : true->OK, false sinon
// -----------------------------------------------------------------------------
bool
NSTPersonListDialog::InitRecherche()
{
	pPersonEnCours = new NSPersonInfo(pContexte->getSuperviseur()) ;

	/*
  // FIXME
  // on doit pouvoir initialiser la recherche en N_TIERS
	ErrDBI = pPersonEnCours->open() ;
	if (ErrDBI != DBIERR_NONE)	{
		erreur("Erreur à l'ouverture du fichier des Pids.", 0, ErrDBI, GetHandle()) ;
		return false ;
	}

  // Pour avoir une liste triée ...  string sNomBlanc    = string(PIDS_NOM_LEN, ' ') ;
  string sPrenomBlanc = string(PIDS_PRENOM_LEN, ' ') ;

  CURProps curProps ;  DbiGetCursorProps(pPersonEnCours->PrendCurseur(), curProps) ;
  Byte *pIndexRec = new Byte[curProps.iRecBufSize] ;
  memset(pIndexRec, 0, curProps.iRecBufSize) ;

  DbiPutField(pPersonEnCours->PrendCurseur(), PIDS_NOM_FIELD,    pIndexRec, (Byte *)sNomBlanc.c_str()) ;  DbiPutField(pPersonEnCours->PrendCurseur(), PIDS_PRENOM_FIELD, pIndexRec, (Byte *)sPrenomBlanc.c_str()) ;

  ErrDBI = pPersonEnCours->chercheClefComposite("NOM_PRENOM", NODEFAULTINDEX, keySEARCHGEQ, dbiWRITELOCK, pIndexRec) ;  delete[] pIndexRec ;

  if (ErrDBI != DBIERR_NONE)  {
    erreur("Erreur à la recherche dans le fichier des Pids.", 0, ErrDBI, GetHandle()) ;
    return false ;
  }
  */

  return true ;}


// -----------------------------------------------------------------------------// Function    : NSTPersonListDialog::InitListe()// Arguments   : Aucun
// Description : Initialise la ListWindow avec ses colonnes
// Returns     : Rien
// -----------------------------------------------------------------------------
void
NSTPersonListDialog::InitListe()
{
	string sFamName   = pContexte->getSuperviseur()->getText("generalLanguage", "famillyName") ;
	string sGivenName = pContexte->getSuperviseur()->getText("generalLanguage", "givenName") ;

  pListe->InsertColumn(0, TListWindColumn((char*)sFamName.c_str(), 100, TListWindColumn::Left, 0)) ;
	pListe->InsertColumn(1, TListWindColumn((char*)sGivenName.c_str(), 100, TListWindColumn::Left, 1)) ;

  if ((iTypePids == pidsPatient) || (iTypePids == pidsCorresp))
  {
    string sCity = pContexte->getSuperviseur()->getText("generalLanguage", "city") ;
    pListe->InsertColumn(2, TListWindColumn((char*)sCity.c_str(), 100, TListWindColumn::Left, 2)) ;
  }
  else
  {
  	string sLogin = pContexte->getSuperviseur()->getText("generalLanguage", "login") ;
    pListe->InsertColumn(2, TListWindColumn((char*)sLogin.c_str(), 100, TListWindColumn::Left, 2)) ;
  }
}

// -----------------------------------------------------------------------------// Function    : NSTPersonListDialog::AfficheListe(int decal)// Arguments   : Aucun
// Description : Affiche le tableau de patients dans la liste
// Returns     : Rien
// -----------------------------------------------------------------------------
void
NSTPersonListDialog::AfficheListe()
{
	char buffer[255] ;

  // On vide la liste et on la remplit	pListe->DeleteAllItems() ;

  if (pPersonArray->empty())
		return ;

	for (NSPersonReverseIter iterReverse = pPersonArray->rbegin(); iterReverse != pPersonArray->rend(); iterReverse++)  {
    sprintf(buffer, "%s", (*iterReverse)->getNom().c_str()) ;
    TListWindItem Item(buffer, 0) ;
    pListe->InsertItem(Item) ;
  }

  pListe->SetSel(0, true) ;
}

// -----------------------------------------------------------------------------// Callback notification to handle additional column information for each item.// -----------------------------------------------------------------------------
void
NSTPersonListDialog::LvnGetDispInfo(TLwDispInfoNotify& dispInfo)
{
  const int BufLen = 255 ;
  static char buffer[BufLen] ;

  TListWindItem& dispInfoItem = *(TListWindItem *)&dispInfo.item ;
  int index = dispInfoItem.GetIndex() ;
  // Affiche les informations en fonction de la colonne  switch (dispInfoItem.GetSubItem())
  {
  	// prénom
   	case 1	:	sprintf(buffer, "%s", ((*pPersonArray)[index])->getPrenom().c_str()) ;
     					dispInfoItem.SetText(buffer) ;
     					break ;

   	// case 2: 	// ville   	//   sprintf(buffer, "%s", ((*pCorrespArray)[index])->pAdresseInfo->pDonnees->ville);
   	//   dispInfoItem.SetText(buffer);
   	//   break;
    // ville ou login
   	case 2	:	if      (pidsUtilisat == iTypePids)
        				sprintf(buffer, "%s", ((*pPersonArray)[index])->getLogin().c_str()) ;
    					else if (pidsCorresp == iTypePids)
              	sprintf(buffer, "%s", ((*pPersonArray)[index])->getVille().c_str()) ;
     					else
        				strcpy(buffer, "") ;
     					dispInfoItem.SetText(buffer) ;
     					break ;
	}
}


// -----------------------------------------------------------------------------// Function    : NSTPersonListDialog::InitPersonArray()// Arguments   : Aucun
// Description : Initialise le tableau de patients ou de corresps et les bookmarks
// Returns     : Rien
// -----------------------------------------------------------------------------
void
NSTPersonListDialog::InitPersonArray()
{
try
{
/*
	// Initialisation du tableau de patients
	// Par convention en N_TIERS le nombre de personnes affichées (nbNom) vaut 10
	pPersonArray->vider() ;

	bool listOk = false ;

	NSPersonsAttributesArray AttsList ;
  NSBasicAttributeArray    AttrArray ;
*/

  // Taking into account initial traits...
  //
  if (pCriteres && (false == pCriteres->empty()))
    for (NSBasicAttributeIter it = pCriteres->begin() ; pCriteres->end() != it ; it++)
    {
      // AttrArray.push_back(new NSBasicAttribute(**it)) ;

      if (LAST_NAME == (*it)->getBalise())
        pNom->SetText((*it)->getText().c_str()) ;
      if (FIRST_NAME == (*it)->getBalise())
        pPrenom->SetText((*it)->getText().c_str()) ;
    }

  ReinitPersonArray() ;

/*
  AttrArray.push_back(new NSBasicAttribute(TRAIT, LAST_NAME)) ;
	AttrArray.push_back(new NSBasicAttribute(TRAIT, FIRST_NAME)) ;
  AttrArray.push_back(new NSBasicAttribute(TRAIT, ROLE)) ;

	AttrArray.push_back(new NSBasicAttribute(STEP,    IntToString(int(iStepSize)))) ;
	AttrArray.push_back(new NSBasicAttribute(NEWLIST, string("1"))) ;

	switch(iTypePids)
	{
  	case pidsPatient :
		{
    	serviceName = (NautilusPilot::SERV_PATIENT_LIST).c_str() ;
			listOk = pContexte->pPilot->personList(serviceName, &AttsList, &AttrArray) ;
			break ;
		}
		case pidsUtilisat :
		{
    	serviceName = (NautilusPilot::SERV_SEARCH_PERSON).c_str() ;
			AttrArray.push_back(new NSBasicAttribute(TRAIT, LOGIN)) ;
			AttrArray.push_back(new NSBasicAttribute(ROLE, USER_ROLE)) ;
			listOk = pContexte->pPilot->personList(serviceName, &AttsList, &AttrArray) ;
			break ;
		}
		case pidsCorresp :
		{
    	serviceName = (NautilusPilot::SERV_SEARCH_PERSON).c_str() ;
      AttrArray.push_back(new NSBasicAttribute(TRAIT, CITY_PRO)) ;
			AttrArray.push_back(new NSBasicAttribute(ROLE, CORRESP_ROLE)) ;
			listOk = pContexte->pPilot->personList(serviceName, &AttsList, &AttrArray) ;
			break ;
		}
		case pidsAll :
		{
    	serviceName = (NautilusPilot::SERV_SEARCH_PERSON).c_str() ;
			AttrArray.push_back(new NSBasicAttribute(ROLE, ALL_ROLES)) ;
			listOk = pContexte->pPilot->personList(serviceName, &AttsList, &AttrArray) ;
			break ;
		}
		default	:
    	erreur("Pids correspondant non encore implémenté dans la PersonList.", standardError, 0) ;
      return ;
	}

  bLimitsReached = false ;

  if ((listOk) && (!(AttsList.empty())))
  {
  	if (iStepSize == AttsList.size())
    	bLimitsReached = true ;

    for (NSPersonsAttributeIter iterPerson = AttsList.begin() ; iterPerson != AttsList.end() ; iterPerson++)
    {
      NSPersonInfo *pPerson = new NSPersonInfo(pContexte, *iterPerson) ;
      pPersonArray->push_back(pPerson) ;
    }
  }
*/
}
catch (...)
{
	erreur("Exception NSTPersonListDialog::InitPersonArray", standardError, 0) ;
}
}

void
NSTPersonListDialog::ReinitPersonArray(bool bForceDisplay)
{
try
{
  // Initialisation du tableau de patients
	// Par convention en N_TIERS le nombre de personnes affichées (nbNom) vaut 10
	pPersonArray->vider() ;

  _sNomPerson    = pNom->GetText() ;
  _sPrenomPerson = pPrenom->GetText() ;

  NSSuper *pSuper = pContexte->getSuperviseur() ;

  string sRoleSens = string("") ;
  string sRole = pRole->getCode() ;
  if ((string("£?????") != sRole) && (string("") != sRole))
    NSDico::donneCodeSens(&sRole, &sRoleSens) ;

  NSPersonsAttributesArray AttsList ;
  NSBasicAttributeArray AttrArray ;

  bool listOk = false ;

  if ((string("") == _sNomPerson) &&
      (string("") == _sPrenomPerson) &&
      (string("") == sRoleSens))
	{
    if (false == bForceDisplay)
      return ;

    AttrArray.push_back(new NSBasicAttribute(TRAIT, LAST_NAME)) ;
	  AttrArray.push_back(new NSBasicAttribute(TRAIT, FIRST_NAME)) ;
    AttrArray.push_back(new NSBasicAttribute(TRAIT, ROLE)) ;

	  AttrArray.push_back(new NSBasicAttribute(STEP,    IntToString(int(iStepSize)))) ;
	  AttrArray.push_back(new NSBasicAttribute(NEWLIST, string("1"))) ;

	  switch(iTypePids)
	  {
  	  case pidsPatient :
		  {
    	  serviceName = (NautilusPilot::SERV_PATIENT_LIST).c_str() ;
			  listOk = pSuper->getPilot()->personList(serviceName, &AttsList, &AttrArray) ;
			  break ;
		  }
		  case pidsUtilisat :
		  {
    	  serviceName = (NautilusPilot::SERV_SEARCH_PERSON).c_str() ;
			  AttrArray.push_back(new NSBasicAttribute(TRAIT, LOGIN)) ;
			  AttrArray.push_back(new NSBasicAttribute(ROLE, USER_ROLE)) ;
			  listOk = pSuper->getPilot()->personList(serviceName, &AttsList, &AttrArray) ;
			  break ;
		  }
		  case pidsCorresp :
		  {
    	  serviceName = (NautilusPilot::SERV_SEARCH_PERSON).c_str() ;
        AttrArray.push_back(new NSBasicAttribute(TRAIT, CITY_PRO)) ;
			  AttrArray.push_back(new NSBasicAttribute(ROLE, CORRESP_ROLE)) ;
			  listOk = pSuper->getPilot()->personList(serviceName, &AttsList, &AttrArray) ;
			  break ;
		  }
		  case pidsAll :
		  {
    	  serviceName = (NautilusPilot::SERV_SEARCH_PERSON).c_str() ;
			  AttrArray.push_back(new NSBasicAttribute(ROLE, ALL_ROLES)) ;
			  listOk = pSuper->getPilot()->personList(serviceName, &AttsList, &AttrArray) ;
			  break ;
		  }
		  default	:
    	  erreur("Pids correspondant non encore implémenté dans la PersonList.", standardError, 0) ;
        return ;
	  }

    bLimitsReached = false ;

    if ((listOk) && (false == AttsList.empty()))
    {
  	  if (iStepSize == AttsList.size())
    	  bLimitsReached = true ;

      for (NSPersonsAttributeIter iterPerson = AttsList.begin() ; AttsList.end() != iterPerson ; iterPerson++)
      {
        NSPersonInfo *pPerson = new NSPersonInfo(pSuper, *iterPerson) ;
        pPersonArray->push_back(pPerson) ;
      }
    }

    return ;
	}

  if (string("") != _sNomPerson)
  {
  	pseumaj(&_sNomPerson) ;
    AttrArray.push_back(new NSBasicAttribute(LAST_NAME, _sNomPerson + string("%"))) ;
	}
	if (string("") != _sPrenomPerson)
  {
  	pseumaj(&_sPrenomPerson) ;
  	AttrArray.push_back(new NSBasicAttribute(FIRST_NAME, _sPrenomPerson + string("%"))) ;
	}
  if (string("") != sRoleSens)
  	AttrArray.push_back(new NSBasicAttribute(ACTIVE_JOB, sRoleSens)) ;

  AttrArray.push_back(new NSBasicAttribute(TRAIT, LAST_NAME)) ;
	AttrArray.push_back(new NSBasicAttribute(TRAIT, FIRST_NAME)) ;
  AttrArray.push_back(new NSBasicAttribute(TRAIT, ROLE)) ;

	switch(iTypePids)
	{
  	case pidsPatient :
		{
    	serviceName = (NautilusPilot::SERV_PERSON_LIST_FROM_TRAITS).c_str() ;
			listOk = pSuper->getPilot()->personList(serviceName, &AttsList, &AttrArray) ;
			break ;
		}
		case pidsUtilisat :
		{
    	serviceName = (NautilusPilot::SERV_PERSON_LIST_FROM_TRAITS).c_str() ;
			AttrArray.push_back(new NSBasicAttribute(TRAIT, LOGIN)) ;
			AttrArray.push_back(new NSBasicAttribute(ROLE, USER_ROLE)) ;
			listOk = pSuper->getPilot()->personList(serviceName, &AttsList, &AttrArray) ;
			break ;
		}
		case  pidsCorresp :
		{
			serviceName = (NautilusPilot::SERV_PERSON_LIST_FROM_TRAITS).c_str() ;
      AttrArray.push_back(new NSBasicAttribute(TRAIT, CITY_PRO)) ;
			AttrArray.push_back(new NSBasicAttribute(ROLE, CORRESP_ROLE)) ;
			listOk = pSuper->getPilot()->personList(serviceName, &AttsList, &AttrArray) ;
			break ;
		}
		case pidsAll :
		{
    	serviceName = (NautilusPilot::SERV_PERSON_LIST_FROM_TRAITS).c_str() ;
			AttrArray.push_back(new NSBasicAttribute(ROLE, ALL_ROLES)) ;
			listOk = pSuper->getPilot()->personList(serviceName, &AttsList, &AttrArray) ;
			break ;
		}
		default	:
    	erreur("Pids correspondant non encore implémenté dans la PersonList.", standardError, 0) ;
      return ;
	}

  if ((listOk) && (false == AttsList.empty()))
  {
    for (NSPersonsAttributeIter iterPerson = AttsList.begin() ; AttsList.end() != iterPerson ; iterPerson++)
    {
    	// string sRole = (*iterPerson)->getAttributeValue(string(ROLE)) ;
      NSPersonInfo *pPerson = new NSPersonInfo(pSuper, *iterPerson) ;
      pPersonArray->push_back(pPerson) ;
    }


  }
}
catch (...)
{
	erreur("Exception NSTPersonListDialog::ReinitPersonArray", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------// Function    : NSTPersonListDialog::PosLastPerson()// Arguments   : Aucun
// Description : Positionne le pPersonEnCours en fonction de la fin de la table
// Returns     : Rien
// -----------------------------------------------------------------------------
void
NSTPersonListDialog::PosLastPerson()
{
/*
	int   nbPrec = 0, limiteSup, totalRec, currentRec ;
  int 	NombreTotal, NumEnCours ;
  int		nbAffListe = 14 ;	// Nombre de corresps affichés dans la liste

  // Calcul du nombre de noms affichés  pPersonEnCours->donneNbTotalRec(&NombreTotal) ;
  totalRec = NombreTotal ;

  if (totalRec < nbAffListe)  {
    nbNom = NombreTotal ;
    bVScroll = false ;
  }
  else
  {
    nbNom = nbAffListe ;
    bVScroll = true ;
  }

  // dimensionnement de la scroll bar  pVScroll->FixeRange(NombreTotal, nbNom) ;

  // Calcul du nombre de précédents	limiteSup = totalRec - nbNom + 1 ;

  // on teste le résultat de la recherche précédente  if (ErrDBI == DBIERR_EOF)
  {
    nbPrec = nbNom ;
    ErrDBI = DBIERR_NONE ;
  }
  else // cas DBIERR_NONE
  {
    pPersonEnCours->donneNumRecEnCours(&NumEnCours) ;
    currentRec = NumEnCours ;
    if (currentRec > limiteSup)
    	nbPrec = currentRec - limiteSup ;
  }

  for (int i = 0 ; (i < nbPrec) && (ErrDBI == DBIERR_NONE) ; i++)  	ErrDBI = pPersonEnCours->precedent(dbiWRITELOCK) ;

  if ((ErrDBI != DBIERR_NONE) && (ErrDBI != DBIERR_BOF))  e	rreur("Erreur de positionnement dans la base des pids.", 0, ErrDBI, GetHandle()) ;
*/
}

// -----------------------------------------------------------------------------// Function    : NSTPersonListDialog::ScrollPerson()// Arguments   : Aucun
// Description : Scrolle la liste en fonction du nom et du prenom
// Returns     : Rien
// -----------------------------------------------------------------------------
void
NSTPersonListDialog::ScrollPerson()
{
	resetTimer() ;
}

void
NSTPersonListDialog::repositScroll()
{
	KillTimer(ID_PAT_TIMER) ;

	if ((true == bLimitsReached) || (true == pPersonArray->empty()))
	{
		Rechercher() ;
		return ;
	}

  int index = 0 ;
  string sNom, sPrenom ;

  _sNomPerson    = pNom->GetText() ;
  pseumaj(&_sNomPerson) ;

  _sPrenomPerson = pPrenom->GetText() ;
  pseumaj(&_sPrenomPerson) ;

  if ((string("") == _sNomPerson) && (string("") == _sPrenomPerson))
  	return ;

  size_t iRefNomLen    = strlen(_sNomPerson.c_str()) ;
  size_t iRefPrenomLen = strlen(_sPrenomPerson.c_str()) ;

  for (NSPersonIter i = pPersonArray->begin() ; pPersonArray->end() != i ; i++)
  {
    sNom    = (*i)->getNom() ;
    sPrenom = (*i)->getPrenom() ;
    pseumaj(&sNom) ;
    pseumaj(&sPrenom) ;

    size_t iNomLen    = strlen(sNom.c_str()) ;
    size_t iPrenomLen = strlen(sPrenom.c_str()) ;

    if ((iNomLen    >= iRefNomLen)    && (string(sNom,    0, iRefNomLen)    == _sNomPerson) &&
        (iPrenomLen >= iRefPrenomLen) && (string(sPrenom, 0, iRefPrenomLen) == _sPrenomPerson))
    // if ((strncmp(nomPerson, sNom.c_str(), strlen(nomPerson)) <= 0)						&&
    //    (strncmp(prenomPerson, sPrenom.c_str(), strlen(prenomPerson)) <= 0))
    {
      pListe->SetSel(index, true) ;
      pListe->EnsureVisible(index, true) ;
      break ;
    }
    index++ ;
  }
}


// -----------------------------------------------------------------------------// Function    : NSTPersonListDialog::GardeNss(int index)// Arguments   : Index de la personne sélectionné
// Description : Conserve le nss de la personne sélectionnée
// Returns     : Rien
// -----------------------------------------------------------------------------
void
NSTPersonListDialog::GardeNum(int index)
{
	sNumSelect = ((*pPersonArray)[index])->getNss() ;
}


// -----------------------------------------------------------------------------// Function    : NSTPersonListDialog::RetrouvePersonSelect()// Arguments   : Aucun
// Description : Retrouve le patient sélectionné
// Returns     : Rien
// -----------------------------------------------------------------------------
void
NSTPersonListDialog::RetrouvePersonSelect()
{
  if (pPersonArray->empty())
		return ;

  int index = 0 ;
	for (NSPersonIter iter = pPersonArray->begin() ; iter != pPersonArray->end() ; iter++)
  {
    string sNumEnCours = ((*pPersonArray)[index])->getNss() ;
    if (sNumSelect == sNumEnCours)
    {
    	pListe->SetSel(index, true) ;
      break ;
    }
    index++ ;
  }
}

// -----------------------------------------------------------------------------
// méthode qui marche avec des autoradiobutton type borland
// -----------------------------------------------------------------------------
void
NSTPersonListDialog::CmSelectPids()
{
  SetCursor(0, IDC_WAIT);

  if      (pPidsPatient->GetCheck() == BF_CHECKED)
  	iTypePids = pidsPatient ;
  else if (pPidsCorresp->GetCheck() == BF_CHECKED)
  	iTypePids = pidsCorresp ;
  else if (pPidsUtilisat->GetCheck() == BF_CHECKED)
  	iTypePids = pidsUtilisat ;
  else
  	iTypePids = pidsAll ; // pidsPatient+pidsUtilisat+pidsCorresp ;

  // Initialisation du tableau des personnes
  InitPersonArray() ;

  // Initialisation de la liste  InitListe() ;
  AfficheListe() ;

  setAccurateCaption() ;

  SetCursor(0, IDC_ARROW) ;
}

// -----------------------------------------------------------------------------
// Function    : NSTPersonListDialog::CmCreer()
// Arguments   : Aucun
// Description : positionne bCreer à true et sort par CmOk()
// Returns     : Rien
// -----------------------------------------------------------------------------
void
NSTPersonListDialog::CmCreer()
{
  bCreer = true ;
  PersonChoisie = -1 ;
  CloseWindow(IDOK) ;
}

void
NSTPersonListDialog::Rechercher()
{
	pContexte->GetMainWindow()->SetCursor(0, IDC_WAIT) ;

	// Initialisation du tableau des personnes
	ReinitPersonArray() ;

	pContexte->GetMainWindow()->SetCursor(0, IDC_ARROW) ;

	// Affichage de la liste
	AfficheListe() ;
}

void
NSTPersonListDialog::SearchNow()
{
	pContexte->GetMainWindow()->SetCursor(0, IDC_WAIT) ;

	// Initialisation du tableau des personnes
	ReinitPersonArray(true) ;

	pContexte->GetMainWindow()->SetCursor(0, IDC_ARROW) ;

	// Affichage de la liste
	AfficheListe() ;
}

void
NSTPersonListDialog::EvTimer(uint id)
{
	if (id != ID_PAT_TIMER)
		return ;

	KillTimer(ID_PAT_TIMER) ;

	// repositScroll() ;
  Rechercher() ;
}

void
NSTPersonListDialog::resetTimer()
{
	KillTimer(ID_PAT_TIMER) ;
	SetTimer(ID_PAT_TIMER, 1000) ;
}

// -----------------------------------------------------------------------------// Function    : NSTPersonListDialog::LineDown()// Arguments   : Aucun
// Description : Gère le défilement de la liste d'une ligne vers le bas
// Returns     : True->Ok False->Echec
// -----------------------------------------------------------------------------
bool
NSTPersonListDialog::LineDown()
{
	if (false == bVScroll)
  	return false ;

  NSSuper *pSuper = pContexte->getSuperviseur() ;

	// Par convention en N_TIERS le nombre de personnes affichées (nbNom) vaut 10  // Pour l'instant : PageDown sera équivalent à LineDown  // Revoir la gestion des curseurs de liste (tableau tampon de 50 pers. par exple)  pPersonArray->vider() ;

  bool listOk = false ;
  NSPersonsAttributesArray AttsList ;

  switch(iTypePids)
  {
  	case pidsPatient :
    {
    	serviceName = (NautilusPilot::SERV_PATIENT_LIST).c_str() ;
      //on a une liste des patients
      // listOk = pContexte->pPilot->patientList(serviceName, pAttsList, "step", "1", "next", NULL) ;
      NSBasicAttributeArray AttrArray ;
      AttrArray.push_back(new NSBasicAttribute("trait", LAST_NAME)) ;
      AttrArray.push_back(new NSBasicAttribute("trait", FIRST_NAME)) ;
      AttrArray.push_back(new NSBasicAttribute(STEP, string("1000"))) ;
      // quand on utlise la balise NEXT même si sa valeur n'est pas utilisée
      // il faut lui ajouter une valeur de preference 1
      AttrArray.push_back(new NSBasicAttribute(NEXT, string("1"))) ;

      listOk = pSuper->getPilot()->personList(serviceName, &AttsList, &AttrArray) ;
      break ;
    }
    case pidsUtilisat	:
    {
      serviceName = (NautilusPilot::SERV_SEARCH_PERSON).c_str() ;
      //on a une liste des utilisateurs
      // listOk = pContexte->pPilot->patientList(serviceName, pAttsList, "step", "1", "next", NULL) ;
      NSBasicAttributeArray AttrArray ;
      AttrArray.push_back(new NSBasicAttribute("trait", LAST_NAME)) ;
      AttrArray.push_back(new NSBasicAttribute("trait", FIRST_NAME)) ;
      AttrArray.push_back(new NSBasicAttribute("trait", LOGIN)) ;
      AttrArray.push_back(new NSBasicAttribute(STEP, string("1000"))) ;
      // quand on utlise la balise NEXT même si sa valeur n'est pas utilisée
      // il faut lui ajouter une valeur de preference 1
      AttrArray.push_back(new NSBasicAttribute(NEXT, string("1"))) ;
      AttrArray.push_back(new NSBasicAttribute(ROLE, USER_ROLE)) ;

      listOk = pSuper->getPilot()->personList(serviceName, &AttsList, &AttrArray) ;
      break ;
    }
    case pidsCorresp	:	break ;

    default						:
    	erreur("Pids correspondant non encore implémenté dans la PersonList.", standardError, 0) ;
      return false ;
  }

  if (listOk)
  {
    NSPersonsAttributeIter iterPerson = AttsList.begin() ;
    NSPersonInfo *pPerson = new NSPersonInfo(pSuper, *iterPerson) ;
    if (false == pPersonArray->empty())
    {
    	NSPersonIter iter = pPersonArray->begin() ;
      delete *iter ;
    	pPersonArray->erase(iter) ;
    }
    // pPersonArray->push_back(new NSPersonInfo(*pPerson)) ;
    pPersonArray->push_back(pPerson) ;
    AfficheListe() ;
    RetrouvePersonSelect() ;
  }

  return true ;
}


// -----------------------------------------------------------------------------// Function    : NSTPersonListDialog::LineUp()// Arguments   : Aucun
// Description : Gère le défilement de la liste d'une ligne vers le haut
// Returns     : Rien
// -----------------------------------------------------------------------------
bool
NSTPersonListDialog::LineUp()
{
  if (false == bVScroll)
  	return false ;

  // Introduire le code pour retour en arrière
  return true ;}

// -----------------------------------------------------------------------------// CanClose()// Répond à OK. Positionne le patient sur le patient sélectionné.
// -----------------------------------------------------------------------------
bool
NSTPersonListDialog::CanClose()
{
	return TDialog::CanClose() ;
}


// -----------------------------------------------------------------------------// CmOk()// Répond à OK. Positionne le patient sur le patient sélectionné.
// -----------------------------------------------------------------------------
void
NSTPersonListDialog::CmOk()
{
	int index = pListe->IndexItemSelect() ;
  PersonChoisie = index ;

  if ((PersonChoisie >= 0) && (PersonChoisie < int(pPersonArray->size())))  	*pPersonSelect = *((*pPersonArray)[PersonChoisie]) ;
  else
  {
  	string sErrorText = pContexte->getSuperviseur()->getText("generalLanguageForLists", "anElementMustBeSelected") ;
  	erreur(sErrorText.c_str(), standardError, 0, GetHandle()) ;
    return ;
  }

  CloseWindow(IDOK) ;}


// -----------------------------------------------------------------------------
// Méthodes de NSTPersonListWindow
// -----------------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NSTPersonListWindow, TListWindow)
	EV_WM_KEYDOWN,
  EV_WM_LBUTTONDBLCLK,
  EV_WM_LBUTTONDOWN,
END_RESPONSE_TABLE ;


// -----------------------------------------------------------------------------// constructor - constructeur// -----------------------------------------------------------------------------NSTPersonListWindow::NSTPersonListWindow(NSTPersonListDialog* pere, int resId, TModule* module)                    :TListWindow(pere, resId, module){
	pDlg = pere ;
  Attr.Style |= LVS_REPORT | LVS_SHOWSELALWAYS ;
  Attr.ExStyle |= WS_EX_NOPARENTNOTIFY ;
}


// -----------------------------------------------------------------------------
// setupwindow
// -----------------------------------------------------------------------------
void
NSTPersonListWindow::SetupWindow()
{
	ListView_SetExtendedListViewStyle(this->HWindow, LVS_EX_FULLROWSELECT) ;
  TListWindow::SetupWindow() ;
}

// -----------------------------------------------------------------------------// on met en place EvKeyDown pour scroller avec les touches ->, <-, etc.// -----------------------------------------------------------------------------
void
NSTPersonListWindow::EvKeyDown(uint key, uint repeatCount, uint flags)
{
	int count, itemSel ;

  switch (key)  {
  	case VK_DOWN	:
    	count = GetItemCount() ;
      itemSel = IndexItemSelect() ;
      if			(itemSel == (count - 1))
      {
      	if ((NULL != pDlg) && (NULL != pDlg->pVScroll))
      		pDlg->pVScroll->SBLineDown() ;
        SetSel(itemSel, true) ;
      }
      else if (itemSel != -1)
      	SetSel(itemSel + 1, true) ;
      break ;

    case VK_UP		:    	itemSel = IndexItemSelect() ;      if (itemSel == 0)
      {
      	if ((NULL != pDlg) && (NULL != pDlg->pVScroll))
      		pDlg->pVScroll->SBLineUp() ;
        SetSel(itemSel, true) ;
      }
      else if (itemSel != -1)
      	SetSel(itemSel - 1, true) ;
      break ;

    case VK_NEXT	:    	itemSel = IndexItemSelect() ;      if (itemSel != -1)
      {
      	if ((NULL != pDlg) && (NULL != pDlg->pVScroll))
      		pDlg->pVScroll->SBPageDown() ;
        SetSel(itemSel, true) ;
      }
      break ;

    case VK_PRIOR	:    	itemSel = IndexItemSelect() ;      if (itemSel != -1)
      {
      	if ((NULL != pDlg) && (NULL != pDlg->pVScroll))
      		pDlg->pVScroll->SBPageUp() ;
        SetSel(itemSel, true) ;
      }
      break ;

    default			:    	TListWindow::EvKeyDown(key, repeatCount, flags) ;  }
}


// -----------------------------------------------------------------------------// Function    : NSTPersonListWindow::EvLButtonDown(uint modKeys, TPoint& point)// Arguments   : les modKeys et le point clické
// Description : Fonction de réponse au click (désactive la multi-sélection)
// Returns     : Rien
// -----------------------------------------------------------------------------
void
NSTPersonListWindow::EvLButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point)
{
  // int count = GetItemCount() ;
	TLwHitTestInfo info(point) ;

  int index = HitTest(info) ;

  if (info.GetFlags() & LVHT_ONITEM)
  	pDlg->GardeNum(index) ;

  // On appelle le Button Down de la classe mère
  TListWindow::EvLButtonDown(modKeys, point) ;
}


// -----------------------------------------------------------------------------// Function    : NSTPersonListWindow::EvLButtonDblClk(uint modKeys, TPoint& point)// Arguments   : les modKeys et le point double-clické
// Description : Fonction de réponse au double-click
// Returns     : Rien
// -----------------------------------------------------------------------------
void
NSTPersonListWindow::EvLButtonDblClk(uint /* modKeys */, NS_CLASSLIB::TPoint& point)
{
	TLwHitTestInfo info(point) ;

  HitTest(info) ;
  if (info.GetFlags() & LVHT_ONITEM)  	pDlg->CmOk() ;
}


// -----------------------------------------------------------------------------// Function    : NSTPersonListWindow::IndexItemSelect()// Arguments   : Aucun
// Description : Retourne l'index du premier item sélectionné
// Returns     : index si item est sélectionné, -1 sinon
// -----------------------------------------------------------------------------
int
NSTPersonListWindow::IndexItemSelect()
{
	int count = GetItemCount() ;
  int index = -1 ;

  for (int i = 0 ; i < count ; i++)  	if (GetItemState(i, LVIS_SELECTED))
    {
    	index = i ;
      break ;
    }

  return index ;}

// -----------------------------------------------------------------------------// Méthodes de NSTPersonListEdit// -----------------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NSTPersonListEdit, NSUtilEdit)
   EV_WM_CHAR,
END_RESPONSE_TABLE;

voidNSTPersonListEdit::EvChar(uint key, uint repeatCount, uint flags)
{
	NSUtilEdit::EvChar(key, repeatCount, flags) ;

	if ((key != VK_RETURN) && (key != VK_TAB))		pDlg->ScrollPerson() ;
}

// -----------------------------------------------------------------//
//  Méthodes de NSTPersonListScrollBar
//
// -----------------------------------------------------------------

NSTPersonListScrollBar::NSTPersonListScrollBar(NSTPersonListDialog* parent, int resourceId, TModule* module)				  :TScrollBar(parent, resourceId, module)
{
	pListeDlg = parent ;
}

voidNSTPersonListScrollBar::FixeRange(int max, int taille)
{
	// SetRange(1, max);
  // HWND hWnd = HWND();

  SCROLLINFO scInfo ;  scInfo.cbSize = (UINT) sizeof(scInfo) ;
  scInfo.fMask = SIF_ALL ;
  scInfo.nMin = 1 ;
  scInfo.nMax = max ;
  scInfo.nPage = (UINT) (taille) ;
  scInfo.nPos = 1 ;

  SetScrollInfo(&scInfo, TRUE) ;}

voidNSTPersonListScrollBar::SBLineDown()
{
	// Transmet à la boite de Dialogue
	TScrollBar::SBLineDown() ;

	pListeDlg->LineDown() ;}

voidNSTPersonListScrollBar::SBLineUp()
{
	// Transmet à la boite de Dialogue
	TScrollBar::SBLineUp() ;

	pListeDlg->LineUp() ;}

voidNSTPersonListScrollBar::SBPageDown()
{
	TScrollBar::SBPageDown() ;

	// for (int i = 0; i < PageMagnitude; i++)	//     if (!pListeDlg->LineDown())
	//   	    break;

	// provisoire...
	pListeDlg->LineDown() ;
}

voidNSTPersonListScrollBar::SBPageUp()
{
	TScrollBar::SBPageUp() ;

	// for (int i = 0; i < PageMagnitude; i++)	//     if (!pListeDlg->LineUp())
	//   	    break;

	// provisoire...
	pListeDlg->LineUp() ;
}

voidNSTPersonListScrollBar::PositionneCurseur()
{
    /*********************************************************
	//
	// Prise du nombre total de d'enregistrement et de la position actuelle
	//

	int NumEnCours;
	//
	// Positionnement du curseur
	//

	pListeDlg->pPersonEnCours->donneNumRecEnCours(&NumEnCours);
	SetPosition(NumEnCours);    **************************************************************/}

//--------------------------------------------------------------------------
//Class NSDoublonListDialog
//--------------------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSTDoublonListDialog, NSUtilDialog)
	// EV_COMMAND(IDOK, 	  CmOk),
	EV_COMMAND(IDC_ROLE_DIFF_ATTRIB, CmAttribuerRole),
  EV_COMMAND(IDC_ANNULER,  CmCancel),
  EV_COMMAND(IDC_CREER, 	 CmCreer),
  EV_LVN_GETDISPINFO(IDC_ROLE_EQUIV_LIST, LvnGetDispInfo1),
  EV_LVN_GETDISPINFO(IDC_ROLE_DIFF_LIST,	LvnGetDispInfo2),
END_RESPONSE_TABLE;

NSTDoublonListDialog::NSTDoublonListDialog(TWindow *pere, NSContexte *pCtx,
                                            NSPersonsAttributesArray *pList, string sRole, TModule* /* module */)
                    :	NSUtilDialog(pere, pCtx, "IDD_DOUBLON_LIST", pNSDLLModule)
{
	pGroupRoleEquiv = new TGroupBox(this, IDC_GROUP_ROLE_EQUIV) ;
  pGroupRoleDiff  = new TGroupBox(this, IDC_GROUP_ROLE_DIFF) ;
  pTexteRoleEquiv = new TStatic(this, IDC_ROLE_EQUIV_TEXT) ;
  pTexteRoleDiff  = new TStatic(this, IDC_ROLE_DIFF_TEXT) ;
  pListeRoleEquiv = new NSTDoublonEquivListWindow(this, IDC_ROLE_EQUIV_LIST) ;
  pListeRoleDiff  = new NSTDoublonDiffListWindow(this, IDC_ROLE_DIFF_LIST) ;
  pButtonCreer    = new TStatic(this, IDC_CREER) ;
  pButtonAnnuler  = new TStatic(this, IDC_ANNULER) ;
  pButtonAttrib   = new TStatic(this, IDC_ROLE_DIFF_ATTRIB) ;

  pRoleEquivArray = new NSPersonsAttributesArray() ;
  pRoleDiffArray  = new NSPersonsAttributesArray() ;

  sRolePerson = sRole ;
  sPersonSelect = "" ;
  bCreer = false ;

  // Initialisation des deux listes
  if (pList && (false == pList->empty()))
  	for (NSPersonsAttributeIter i = pList->begin() ; pList->end() != i ; i++)
  	{
  		if ((*i)->getAttributeValue(ROLE) == sRolePerson)
    		pRoleEquivArray->push_back(new NSBasicAttributeArray(*(*i))) ;
    	else
    		pRoleDiffArray->push_back(new NSBasicAttributeArray(*(*i))) ;
		}
}

NSTDoublonListDialog::~NSTDoublonListDialog()
{
  delete pGroupRoleEquiv ;
  delete pGroupRoleDiff ;
  delete pTexteRoleEquiv ;
  delete pTexteRoleDiff ;
  delete pListeRoleEquiv ;
  delete pListeRoleDiff ;
  delete pButtonCreer ;
  delete pButtonAnnuler ;
  delete pButtonAttrib ;
  delete pRoleEquivArray ;
  delete pRoleDiffArray ;
}

voidNSTDoublonListDialog::SetupWindow(){  NSUtilDialog::SetupWindow();  string sText;  sText = pContexte->getSuperviseur()->getText("doublonList", "captionDialog");  SetCaption(sText.c_str());  sText = pContexte->getSuperviseur()->getText("doublonList", "roleEquiv");  pTexteRoleEquiv->SetText(sText.c_str());  sText = pContexte->getSuperviseur()->getText("doublonList", "roleDiff");  pTexteRoleDiff->SetText(sText.c_str());  sText = pContexte->getSuperviseur()->getText("doublonList", "attribButton");  pButtonAttrib->SetText(sText.c_str());  sText = pContexte->getSuperviseur()->getText("doublonList", "createButton");  pButtonCreer->SetText(sText.c_str());  sText = pContexte->getSuperviseur()->getText("doublonList", "cancelButton");  pButtonAnnuler->SetText(sText.c_str());  InitListeRoleEquiv();  InitListeRoleDiff();  AfficheListeRoleEquiv();  AfficheListeRoleDiff();}void
NSTDoublonListDialog::InitListeRoleEquiv()
{
	string sFamillyName = pContexte->getSuperviseur()->getText("generalLanguage", "famillyName") ;
  string sGivenName   = pContexte->getSuperviseur()->getText("generalLanguage", "givenName") ;
  string sGender      = pContexte->getSuperviseur()->getText("generalLanguage", "gender") ;
  string sBirthDate   = pContexte->getSuperviseur()->getText("generalLanguage", "birthDate") ;

  pListeRoleEquiv->InsertColumn(0, TListWindColumn((char*)sFamillyName.c_str(), 100, TListWindColumn::Left, 0)) ;
  pListeRoleEquiv->InsertColumn(1, TListWindColumn((char*)sGivenName.c_str(),   100, TListWindColumn::Left, 1)) ;
  pListeRoleEquiv->InsertColumn(2, TListWindColumn((char*)sGender.c_str(),       50, TListWindColumn::Left, 2)) ;
  pListeRoleEquiv->InsertColumn(3, TListWindColumn((char*)sBirthDate.c_str(),    80, TListWindColumn::Left, 3)) ;
}

void
NSTDoublonListDialog::AfficheListeRoleEquiv()
{
	char buffer[255] ;

    // On vide la liste et on la remplit	pListeRoleEquiv->DeleteAllItems() ;

  if (pRoleEquivArray->empty())
		return ;

	for (NSPersonsAttributeReverseIter iterReverse = pRoleEquivArray->rbegin(); iterReverse != pRoleEquivArray->rend(); iterReverse++)	{
  	sprintf(buffer, "%s", ((*iterReverse)->getAttributeValue(LAST_NAME)).c_str()) ;
    TListWindItem Item(buffer, 0) ;
    pListeRoleEquiv->InsertItem(Item) ;
	}
}

void
NSTDoublonListDialog::LvnGetDispInfo1(TLwDispInfoNotify& dispInfo)
{
	const int   BufLen = 255 ;
	static char buffer[BufLen] ;
  buffer[0] = '\0' ;
  string      sDate ;

	TListWindItem& dispInfoItem = *(TListWindItem *)&dispInfo.item ;
	int index = dispInfoItem.GetIndex() ;  string sNaturalText = string("") ;  string sLang        = pContexte->getUserLanguage() ;
	// Affiche les informations en fonction de la colonne	switch (dispInfoItem.GetSubItem())
	{
		// prénom
    case 1 :
    	sprintf(buffer, "%s", (((*pRoleEquivArray)[index])->getAttributeValue(FIRST_NAME)).c_str()) ;
      dispInfoItem.SetText(buffer) ;
      break ;

    case 2 : 	// sexe    	if (((*pRoleEquivArray)[index])->getAttributeValue(SEX) == string("1"))      	sNaturalText = pContexte->getSuperviseur()->getText("generalLanguage", "male") ;      if (((*pRoleEquivArray)[index])->getAttributeValue(SEX) == string("2"))      	sNaturalText = pContexte->getSuperviseur()->getText("generalLanguage", "female") ;      dispInfoItem.SetText(sNaturalText.c_str()) ;
      break ;

		case 3 :	// date de naissance
      sDate = donne_date(((*pRoleEquivArray)[index])->getAttributeValue(BIRTHDATE), sLang) ;
      dispInfoItem.SetText(sDate.c_str()) ;
      break ;
	}
}

void
NSTDoublonListDialog::InitListeRoleDiff()
{
  string sFamillyName = pContexte->getSuperviseur()->getText("generalLanguage", "famillyName") ;
  string sGivenName   = pContexte->getSuperviseur()->getText("generalLanguage", "givenName") ;
  string sGender      = pContexte->getSuperviseur()->getText("generalLanguage", "gender") ;
  string sBirthDate   = pContexte->getSuperviseur()->getText("generalLanguage", "birthDate") ;

  pListeRoleDiff->InsertColumn(0, TListWindColumn((char*)sFamillyName.c_str(), 100, TListWindColumn::Left, 0)) ;
  pListeRoleDiff->InsertColumn(1, TListWindColumn((char*)sGivenName.c_str(),   100, TListWindColumn::Left, 1)) ;
  pListeRoleDiff->InsertColumn(2, TListWindColumn((char*)sGender.c_str(),       50, TListWindColumn::Left, 2)) ;
  pListeRoleDiff->InsertColumn(3, TListWindColumn((char*)sBirthDate.c_str(),    80, TListWindColumn::Left, 3)) ;
}

void
NSTDoublonListDialog::AfficheListeRoleDiff()
{
	char buffer[255] ;

	// On vide la liste et on la remplit	pListeRoleDiff->DeleteAllItems() ;

  if (pRoleDiffArray->empty())
		return ;

	for (NSPersonsAttributeReverseIter iterReverse = pRoleDiffArray->rbegin(); iterReverse != pRoleDiffArray->rend(); iterReverse++)	{
  	sprintf(buffer, "%s", ((*iterReverse)->getAttributeValue(LAST_NAME)).c_str()) ;
    TListWindItem Item(buffer, 0) ;
    pListeRoleDiff->InsertItem(Item) ;
	}
}

void
NSTDoublonListDialog::LvnGetDispInfo2(TLwDispInfoNotify& dispInfo)
{
	const int   BufLen = 255 ;
	static char buffer[BufLen] ;
  buffer[0] = '\0' ;
  string      sDate ;

	TListWindItem& dispInfoItem = *(TListWindItem *)&dispInfo.item ;
	int index = dispInfoItem.GetIndex() ;
  string sNaturalText = string("") ;
  string sLang        = pContexte->getUserLanguage() ;
	// Affiche les informations en fonction de la colonne	switch (dispInfoItem.GetSubItem())
	{
  	// prénom
    case 1 :
    	sprintf(buffer, "%s", (((*pRoleDiffArray)[index])->getAttributeValue(FIRST_NAME)).c_str()) ;
      dispInfoItem.SetText(buffer) ;
      break ;

    case 2: 	// sexe    	if (((*pRoleDiffArray)[index])->getAttributeValue(SEX) == string("1"))
      	sNaturalText = pContexte->getSuperviseur()->getText("generalLanguage", "male") ;      if (((*pRoleDiffArray)[index])->getAttributeValue(SEX) == string("2"))      	sNaturalText = pContexte->getSuperviseur()->getText("generalLanguage", "female") ;
      dispInfoItem.SetText(sNaturalText.c_str()) ;
      break ;

    case 3	:	// date de naissance
      string sDate = donne_date(((*pRoleDiffArray)[index])->getAttributeValue(BIRTHDATE), sLang) ;
      dispInfoItem.SetText(sDate.c_str()) ;
      break ;
	}
}

void
NSTDoublonListDialog::CmAttribuerRole()
{
	int index = pListeRoleDiff->IndexItemSelect() ;
	if (index == -1)
  {
  	string sErrorText = pContexte->getSuperviseur()->getText("generalLanguageForLists", "anElementMustBeSelected") ;
  	erreur(sErrorText.c_str(), warningError, 0, GetHandle()) ;
    return ;
	}

	sPersonSelect = ((*pRoleDiffArray)[index])->getAttributeValue(PIDS) ;
	sRolePersonSelect = ((*pRoleDiffArray)[index])->getAttributeValue(ROLE) ;
	CmOk() ;
}

void
NSTDoublonListDialog::CmCreer()
{
	bCreer = true ;
	CmOk() ;
}

bool
NSTDoublonListDialog::CanClose()
{
	return NSUtilDialog::CanClose() ;
}

void
NSTDoublonListDialog::CmOk()
{
	NSUtilDialog::CmOk() ;
}

void
NSTDoublonListDialog::CmCancel()
{
	NSUtilDialog::CmCancel() ;
}

//--------------------------------------------------------------------------
//Class NSDoublonEquivListWindow
//--------------------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSTDoublonEquivListWindow, TListWindow)
  EV_WM_LBUTTONDBLCLK,
  EV_WM_LBUTTONDOWN,
END_RESPONSE_TABLE ;

NSTDoublonEquivListWindow::NSTDoublonEquivListWindow(NSTDoublonListDialog* pere, int resId, TModule* module)
	: TListWindow(pere, resId, module)
{
	pDlg = pere ;
	Attr.Style |= LVS_REPORT | LVS_SHOWSELALWAYS ;
	Attr.ExStyle |= WS_EX_NOPARENTNOTIFY ;
}


// -----------------------------------------------------------------------------
// setupwindow
// -----------------------------------------------------------------------------
void
NSTDoublonEquivListWindow::SetupWindow()
{
	ListView_SetExtendedListViewStyle(this->HWindow, LVS_EX_FULLROWSELECT) ;
	TListWindow::SetupWindow() ;
}

void
NSTDoublonEquivListWindow::EvLButtonDblClk(uint /* modKeys */, NS_CLASSLIB::TPoint& point)
{
	TLwHitTestInfo info(point) ;

	HitTest(info) ;
	if (info.GetFlags() & LVHT_ONITEM)		pDlg->CmCancel() ;
}

int
NSTDoublonEquivListWindow::IndexItemSelect()
{
	int count = GetItemCount() ;
  int index = -1 ;

  for (int i = 0 ; i < count ; i++)  	if (GetItemState(i, LVIS_SELECTED))
    {
    	index = i ;
      break ;
    }

  return index ;
}

//--------------------------------------------------------------------------
//Class NSDoublonListDialog
//--------------------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSTSingleRoleDoublonListDialog, NSUtilDialog)
	// EV_COMMAND(IDOK, 	  CmOk),
  EV_COMMAND(IDC_ANNULER,  CmCancel),
  EV_COMMAND(IDC_CREER, 	 CmCreer),
  EV_LVN_GETDISPINFO(IDC_ROLE_EQUIV_LIST, LvnGetDispInfo1),
END_RESPONSE_TABLE;

NSTSingleRoleDoublonListDialog::NSTSingleRoleDoublonListDialog(TWindow *pere, NSContexte *pCtx,
                                            NSPersonsAttributesArray *pList, string sRole, TModule* /* module */)
                    :	NSUtilDialog(pere, pCtx, "IDD_DOUBLON_SINGLE_LIST", pNSDLLModule)
{
	pGroupRoleEquiv = new TGroupBox(this, IDC_GROUP_ROLE_EQUIV) ;
  pTexteRoleEquiv = new TStatic(this, IDC_ROLE_EQUIV_TEXT) ;
  pListeRoleEquiv = new NSTDoublonEquivRoleListWindow(this, IDC_ROLE_EQUIV_LIST) ;
  pButtonCreer    = new TStatic(this, IDC_CREER) ;
  pButtonAnnuler  = new TStatic(this, IDC_ANNULER) ;

  pRoleEquivArray = new NSPersonsAttributesArray() ;

  sRolePerson = sRole ;
  sPersonSelect = "" ;
  bCreer = false ;

  // Initialisation des deux listes
  if (pList && (!pList->empty()))
  	for (NSPersonsAttributeIter i = pList->begin(); i != pList->end(); i++)
  		if ((*i)->getAttributeValue(ROLE) == sRolePerson)
    		pRoleEquivArray->push_back(new NSBasicAttributeArray(*(*i))) ;
}

NSTSingleRoleDoublonListDialog::~NSTSingleRoleDoublonListDialog()
{
  delete pGroupRoleEquiv ;
  delete pTexteRoleEquiv ;
  delete pListeRoleEquiv ;
  delete pButtonCreer ;
  delete pButtonAnnuler ;
  delete pRoleEquivArray ;
}

voidNSTSingleRoleDoublonListDialog::SetupWindow(){  NSUtilDialog::SetupWindow() ;  string sText ;  sText = pContexte->getSuperviseur()->getText("doublonSingleRoleList", "captionDialog") ;  SetCaption(sText.c_str()) ;  sText = pContexte->getSuperviseur()->getText("doublonSingleRoleList", "potentialDoublons") ;  pGroupRoleEquiv->SetCaption(sText.c_str()) ;  sText = pContexte->getSuperviseur()->getText("doublonSingleRoleList", "roleEquiv") ;  pTexteRoleEquiv->SetText(sText.c_str()) ;  sText = pContexte->getSuperviseur()->getText("doublonSingleRoleList", "createButton") ;  pButtonCreer->SetText(sText.c_str()) ;  sText = pContexte->getSuperviseur()->getText("doublonSingleRoleList", "cancelButton") ;  pButtonAnnuler->SetText(sText.c_str()) ;  InitListeRoleEquiv() ;  AfficheListeRoleEquiv() ;}void
NSTSingleRoleDoublonListDialog::InitListeRoleEquiv()
{
	string sFamillyName = pContexte->getSuperviseur()->getText("generalLanguage", "famillyName") ;
  string sGivenName   = pContexte->getSuperviseur()->getText("generalLanguage", "givenName") ;
  string sGender      = pContexte->getSuperviseur()->getText("generalLanguage", "gender") ;
  string sBirthDate   = pContexte->getSuperviseur()->getText("generalLanguage", "birthDate") ;

  pListeRoleEquiv->InsertColumn(0, TListWindColumn((char*)sFamillyName.c_str(), 100, TListWindColumn::Left, 0)) ;
  pListeRoleEquiv->InsertColumn(1, TListWindColumn((char*)sGivenName.c_str(),   100, TListWindColumn::Left, 1)) ;
  pListeRoleEquiv->InsertColumn(2, TListWindColumn((char*)sGender.c_str(),       50, TListWindColumn::Left, 2)) ;
  pListeRoleEquiv->InsertColumn(3, TListWindColumn((char*)sBirthDate.c_str(),    80, TListWindColumn::Left, 3)) ;
}

void
NSTSingleRoleDoublonListDialog::AfficheListeRoleEquiv()
{
	char buffer[255] ;

    // On vide la liste et on la remplit	pListeRoleEquiv->DeleteAllItems() ;

  if (pRoleEquivArray->empty())
		return ;

	for (NSPersonsAttributeReverseIter iterReverse = pRoleEquivArray->rbegin(); iterReverse != pRoleEquivArray->rend(); iterReverse++)	{
  	sprintf(buffer, "%s", ((*iterReverse)->getAttributeValue(LAST_NAME)).c_str()) ;
    TListWindItem Item(buffer, 0) ;
    pListeRoleEquiv->InsertItem(Item) ;
	}
}

void
NSTSingleRoleDoublonListDialog::LvnGetDispInfo1(TLwDispInfoNotify& dispInfo)
{
	const int   BufLen = 255 ;
	static char buffer[BufLen] ;
  buffer[0] = '\0' ;
  string      sDate ;

	TListWindItem& dispInfoItem = *(TListWindItem *)&dispInfo.item ;
	int index = dispInfoItem.GetIndex() ;  string sNaturalText = string("") ;  string sLang        = pContexte->getUserLanguage() ;
	// Affiche les informations en fonction de la colonne	switch (dispInfoItem.GetSubItem())
	{
		// prénom
    case 1 :
    	sprintf(buffer, "%s", (((*pRoleEquivArray)[index])->getAttributeValue(FIRST_NAME)).c_str()) ;
      dispInfoItem.SetText(buffer) ;
      break ;

    case 2 : 	// sexe    	if (((*pRoleEquivArray)[index])->getAttributeValue(SEX) == string("1"))      	sNaturalText = pContexte->getSuperviseur()->getText("generalLanguage", "male") ;      if (((*pRoleEquivArray)[index])->getAttributeValue(SEX) == string("2"))      	sNaturalText = pContexte->getSuperviseur()->getText("generalLanguage", "female") ;
      dispInfoItem.SetText(sNaturalText.c_str()) ;
      break ;

		case 3 :	// date de naissance
      sDate = donne_date(((*pRoleEquivArray)[index])->getAttributeValue(BIRTHDATE), sLang) ;
      dispInfoItem.SetText(sDate.c_str()) ;
      break ;
	}
}

void
NSTSingleRoleDoublonListDialog::CmCreer()
{
	bCreer = true ;
	CmOk() ;
}

bool
NSTSingleRoleDoublonListDialog::CanClose()
{
	return NSUtilDialog::CanClose() ;
}

void
NSTSingleRoleDoublonListDialog::CmOk()
{
	NSUtilDialog::CmOk() ;
}

void
NSTSingleRoleDoublonListDialog::CmCancel()
{
	NSUtilDialog::CmCancel() ;
}

//--------------------------------------------------------------------------
//Class NSTDoublonEquivRoleListWindow
//--------------------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSTDoublonEquivRoleListWindow, TListWindow)
  EV_WM_LBUTTONDBLCLK,
  EV_WM_LBUTTONDOWN,
END_RESPONSE_TABLE ;

NSTDoublonEquivRoleListWindow::NSTDoublonEquivRoleListWindow(NSTSingleRoleDoublonListDialog* pere, int resId, TModule* module)
                              :TListWindow(pere, resId, module)
{
	pDlg = pere ;
	Attr.Style |= LVS_REPORT | LVS_SHOWSELALWAYS ;
	Attr.ExStyle |= WS_EX_NOPARENTNOTIFY ;
}


// -----------------------------------------------------------------------------
// setupwindow
// -----------------------------------------------------------------------------
void
NSTDoublonEquivRoleListWindow::SetupWindow()
{
	ListView_SetExtendedListViewStyle(this->HWindow, LVS_EX_FULLROWSELECT) ;
	TListWindow::SetupWindow() ;
}

void
NSTDoublonEquivRoleListWindow::EvLButtonDblClk(uint /* modKeys */, NS_CLASSLIB::TPoint& point)
{
	TLwHitTestInfo info(point) ;

	HitTest(info) ;
	if (info.GetFlags() & LVHT_ONITEM)		pDlg->CmCancel() ;
}

int
NSTDoublonEquivRoleListWindow::IndexItemSelect()
{
	int count = GetItemCount() ;
  int index = -1 ;

  for (int i = 0 ; i < count ; i++)  	if (GetItemState(i, LVIS_SELECTED))
    {
    	index = i ;
      break ;
    }

  return index ;
}

//--------------------------------------------------------------------------
//Class NSDoublonDiffListWindow
//--------------------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSTDoublonDiffListWindow, TListWindow)
  EV_WM_LBUTTONDBLCLK,
  EV_WM_LBUTTONDOWN,
END_RESPONSE_TABLE ;

NSTDoublonDiffListWindow::NSTDoublonDiffListWindow(NSTDoublonListDialog* pere, int resId, TModule* module)
	: TListWindow(pere, resId, module)
{
	pDlg = pere ;
	Attr.Style |= LVS_REPORT | LVS_SHOWSELALWAYS ;
	Attr.ExStyle |= WS_EX_NOPARENTNOTIFY ;
}


// -----------------------------------------------------------------------------
// setupwindow
// -----------------------------------------------------------------------------
void
NSTDoublonDiffListWindow::SetupWindow()
{
	ListView_SetExtendedListViewStyle(this->HWindow, LVS_EX_FULLROWSELECT) ;
	TListWindow::SetupWindow() ;
}

void
NSTDoublonDiffListWindow::EvLButtonDblClk(uint /* modKeys */, NS_CLASSLIB::TPoint& point)
{
	TLwHitTestInfo info(point) ;

	HitTest(info) ;
	if (info.GetFlags() & LVHT_ONITEM)		pDlg->CmAttribuerRole() ;
}

int
NSTDoublonDiffListWindow::IndexItemSelect()
{
	int count = GetItemCount() ;
  int index = -1 ;

  for (int i = 0 ; i < count ; i++)  	if (GetItemState(i, LVIS_SELECTED))
    {
    	index = i ;
      break ;
    }

  return index ;
}

//--------------------------------------------------------------------------
//Class NSHistorizedListWindow
//--------------------------------------------------------------------------
NSHistoryManagement::NSHistoryManagement()
{
	_sLibelle   = string("") ;
	_sStartDate = string("") ;
	_sEndDate   = string("") ;
  _iTreePos   = -1 ;
}

NSHistoryManagement::NSHistoryManagement(const NSHistoryManagement& rv)
{
  _sLibelle   = rv._sLibelle ;
	_sStartDate = rv._sStartDate ;
	_sEndDate   = rv._sEndDate ;
  _iTreePos   = rv._iTreePos ;
}

NSHistoryManagement::~NSHistoryManagement()
{
}

NSHistoryManagement&
NSHistoryManagement::operator=(const NSHistoryManagement& src)
{
  if (this == &src)
		return *this ;

  _sLibelle   = src._sLibelle ;
	_sStartDate = src._sStartDate ;
	_sEndDate   = src._sEndDate ;
  _iTreePos   = src._iTreePos ;

  return *this ;
}

//---------------------------------------------------------------------------
//      class NSHistoryManagementArray
//---------------------------------------------------------------------------

NSHistoryManagementArray::NSHistoryManagementArray(const NSHistoryManagementArray& rv)
                         :NSHistoryManArray()
{
try
{
	if (false == rv.empty())
		for (NSHistoryManagementConstIter i = rv.begin() ; rv.end() != i ; i++)
			push_back(*i) ;
}
catch (...)
{
	erreur("Exception NSHistoryManagementArray ctor.", standardError, 0) ;
}
}

void
NSHistoryManagementArray::vider()
{
	if (empty())
		return ;

	for (NSHistoryManagementIter i = begin() ; end() != i ; )
	{
		delete *i ;
		erase(i) ;
	}
}

NSHistoryManagementArray::~NSHistoryManagementArray()
{
	vider() ;
}

// --------------------------------------------------------------------------
//
// Class NSHistorizedListWindow
//
// --------------------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSHistorizedListWindow, NSSkinableListWindow)
    EV_WM_SETFOCUS,
    EV_WM_KILLFOCUS,
    EV_WM_RBUTTONDOWN,
  	EV_WM_LBUTTONDBLCLK,  //double click
    NS_LVN_GETDISPINFO(LvnGetDispInfo),
    //EV_LVN_COLUMNCLICK(IDC_LISTHIST,LVNColumnclick),
    NS_LW_COLUMNCLICK(LVNColumnclick),
    EV_WM_KEYDOWN,
    EV_COMMAND(IDC_NEWENREG, CmCreerEnreg),
    EV_COMMAND(IDC_MODENREG, CmModifEnreg),
    EV_COMMAND(IDC_DELENREG, CmSupprEnreg),
END_RESPONSE_TABLE;

NSHistorizedListWindow::NSHistorizedListWindow(TWindow* parent,
                                    NSContexte* pCtx, int id,
                                    int x, int y, int w, int h, TModule* module)
                        :NSSkinableListWindow(parent, pCtx, id, x, y, w, h, module)
{
	pVectData     = 0 ;
  pControle     = 0 ;
  bSetupWindow  = false ;
  bFirstUse     = true ;
  iSortedColumn = -1 ;

  Attr.Style |= LVS_REPORT | LVS_SHOWSELALWAYS ;
  Attr.ExStyle |= WS_EX_NOPARENTNOTIFY ;
}

NSHistorizedListWindow::NSHistorizedListWindow(TWindow* parent, NSContexte* pCtx, int resourceId,
                           TModule* module)
                        :NSSkinableListWindow(parent, pCtx, resourceId, module)
{
	pVectData     = 0 ;
  pControle     = 0 ;
  bSetupWindow  = false ;
  bFirstUse     = true ;
  iSortedColumn = -1 ;

  Attr.Style |= LVS_REPORT | LVS_SHOWSELALWAYS ;
  Attr.ExStyle |= WS_EX_NOPARENTNOTIFY ;
}

NSHistorizedListWindow::~NSHistorizedListWindow()
{
	if (pControle)
  	delete pControle ;
}

void
NSHistorizedListWindow::SetupWindow()
{
	ListView_SetExtendedListViewStyle(this->HWindow, LVS_EX_FULLROWSELECT) ;

  SetupColumns() ;
  //On recharge les données à partir du pTransfert
  if (pControle && pControle->getTransfert())
		pVectData = pControle->getTransfert()->getPatPatho() ;
  initCurentListe() ;
  AfficheListe() ;

  NSSkinableListWindow::SetupWindow() ;

  skinSwitchOff("historizedListOff") ;
}

void
NSHistorizedListWindow::EvSetFocus(HWND hWndLostFocus)
{
	skinSwitchOn("historizedListOn") ;
	TListWindow::EvSetFocus(hWndLostFocus) ;
}

void
NSHistorizedListWindow::EvKillFocus(HWND hWndGetFocus)
{
	skinSwitchOff("historizedListOff") ;
	TListWindow::EvKillFocus(hWndGetFocus) ;
}

void
NSHistorizedListWindow::EvRButtonDown(uint /* modkeys */, NS_CLASSLIB::TPoint& point)
{
    // création d'un menu popup
    NS_CLASSLIB::TPoint lp = point;

    TPopupMenu *menu = new TPopupMenu();

    //   menu->AppendMenu(MF_STRING, IDC_NEWADR, "Créer adresse");
     //   menu->AppendMenu(MF_GRAYED, 0, "Modifier adresse");
  	//	menu->AppendMenu(MF_GRAYED, 0, "Supprimer adresse");
   // }
  //  else
   // {
        menu->AppendMenu(MF_STRING, IDC_NEWENREG, "Créer nouvel enregistrement");
        menu->AppendMenu(MF_STRING, IDC_MODENREG, "Modifier enregistrement");
  	   	menu->AppendMenu(MF_STRING, IDC_DELENREG, "Supprimer enregistrement");
  //  }

    ClientToScreen(lp);
  	menu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, lp, 0, HWindow);
  	delete menu;
}

void
NSHistorizedListWindow::EvLButtonDblClk(uint /* modKeys */, NS_CLASSLIB::TPoint& point)
{
	TLwHitTestInfo info(point) ;

	HitTest(info) ;
	if (info.GetFlags() & LVHT_ONITEM)  	CmModifEnreg() ;	else		CmCreerEnreg() ;}

void
NSHistorizedListWindow::EvKeyDown(uint key, uint repeatCount, uint flags)
{
    if      (key == VK_DELETE)
        CmSupprEnreg() ;
    else if (key == VK_INSERT)
    {
        if (GetKeyState(VK_SHIFT) < 0)
            CmModifEnreg() ;
        else
            CmCreerEnreg() ;
    }
    else
    	TListWindow::EvKeyDown(key, repeatCount, flags) ;
}

void
NSHistorizedListWindow::SetupColumns()
{
    NS_CLASSLIB:: TRect  rectList ;
    GetWindowRect(rectList);
    int iDimLib = rectList.Width() - 140;
    InsertColumn(0, TListWindColumn("Libelle", iDimLib));
    InsertColumn(1, TListWindColumn("Debut", 68));
    InsertColumn(2, TListWindColumn("Fin", 68));

}

int
NSHistorizedListWindow::IndexItemSelect()
{
    int count = GetItemCount();
    int index = -1;

    for (int i = 0; i < count; i++)    {
   	    if (GetItemState(i, LVIS_SELECTED))        {
      	    index = i;
            break;
        }
    }

    return index;}


bool drugSortByEndDataInf(NSHistoryManagement* pDrug1, NSHistoryManagement* pDrug2)
{
	return (pDrug1->getEndDate() < pDrug2->getEndDate());
}

bool drugSortByEndDataSup(NSHistoryManagement* pDrug1, NSHistoryManagement* pDrug2)
{
	return (pDrug1->getEndDate() > pDrug2->getEndDate());
}

bool drugSortByLibelleInf(NSHistoryManagement* pDrug1, NSHistoryManagement* pDrug2)
{
	return (pDrug1->getLibelle() < pDrug2->getLibelle());
}

bool drugSortByLibelleSup(NSHistoryManagement* pDrug1, NSHistoryManagement* pDrug2)
{
	return (pDrug1->getLibelle() > pDrug2->getLibelle());
}

bool drugSortByStartDataInf(NSHistoryManagement* pDrug1, NSHistoryManagement* pDrug2)
{
	return (pDrug1->getStartDate() < pDrug2->getStartDate());
}

bool drugSortByStartDataSup(NSHistoryManagement* pDrug1, NSHistoryManagement* pDrug2)
{
	return (pDrug1->getStartDate() > pDrug2->getStartDate());
}


void
NSHistorizedListWindow::initCurentListe()
{
	aHistory.vider() ;
  int iPos = 0 ;
  if ((NULL == pVectData) || (pVectData->empty()))
  	return ;

	for (FatheredPatPathoIterVect itPPt = pVectData->begin() ; itPPt != pVectData->end(); itPPt++)
  {  	NSHistoryManagement* pHisPatPatho = TrouveObjectData((*itPPt)->getPatPatho()) ;    if (NULL != pHisPatPatho)    {    	pHisPatPatho->setPosition(iPos) ;      if (pHisPatPatho->getStartDate() == "")      	pHisPatPatho->setStartDate("00000000") ;   // la date de start la plus anciene est considerée celle vide      if(pHisPatPatho->getEndDate() == "")      	pHisPatPatho->setEndDate("99999999") ;   // la date de end vide = date en cours      aHistory.push_back(pHisPatPatho) ;    }    iPos++ ;
	}

	if (aHistory.empty())
  	return ;

	if (bFirstUse)
	{
  	bFirstUse = false ;
    sort(aHistory.begin(), aHistory.end(), drugSortByEndDataSup) ;
    iSortedColumn = 1 ;
    bNaturallySorted = true ;
  }
  else
  {
  	switch (iSortedColumn)
    {
    	case 0 :
      	if (bNaturallySorted)
        	sort(aHistory.begin(), aHistory.end(), drugSortByLibelleSup) ;
        else
        	sort(aHistory.begin(), aHistory.end(), drugSortByLibelleInf) ;
        break ;
      case 1 :
      	if (bNaturallySorted)
        	sort(aHistory.begin(), aHistory.end(), drugSortByStartDataSup) ;
        else
        	sort(aHistory.begin(), aHistory.end(), drugSortByStartDataInf) ;
        break ;
      case 2 :
      	if (bNaturallySorted)
        	sort(aHistory.begin(), aHistory.end(), drugSortByEndDataSup) ;
        else
        	sort(aHistory.begin(), aHistory.end(), drugSortByEndDataInf) ;
        break ;
    }
  }
}

NSHistoryManagement*
NSHistorizedListWindow::TrouveObjectData(NSPatPathoArray* pPatPathoArray)
{
try
{
	if (((NSPatPathoArray*) NULL == pPatPathoArray) || (pPatPathoArray->empty()))
  	return (NSHistoryManagement*) 0 ;

	NSHistoryManagement* pHisPatPatho = new NSHistoryManagement() ;

	NSPatPathoArray      PatPathoClone(*pPatPathoArray) ;

	PatPathoIter iter = PatPathoClone.begin() ;
  string sLib ;
  string sElemLex ;
  string sSens ;

	while (iter != PatPathoClone.end())
	{
  	if ((*iter)->getColonne() == 0)
    {
    	sElemLex = string((*iter)->getLexique()) ;
      pContexte->getDico()->donneCodeSens(&sElemLex, &sSens) ;

      if ((sSens == "KOUVR") || (sSens == "KFERM") )
      {
      	PatPathoIter iterPrec = iter ;

        iter++ ;
        int iLigneBase = (*iter)->getLigne() ;
        string sUnite  = "" ;
				string sFormat = "";
				string sValeur = "";
				string sTemp   = "";

				while ((iter != pPatPathoArray->end()) && ((*iter)->getLigne() == iLigneBase))
        {
          string sLexique = (*iter)->getLexique() ;
        	if ((string("") != sLexique) && (sLexique[0] == '£'))
          {
            sTemp   = (*iter)->getLexique() ;
            pContexte->getDico()->donneCodeSens(&sTemp, &sFormat) ;
            sValeur = (*iter)->getComplement() ;
            sTemp   = (*iter)->getUnit() ;
            pContexte->getDico()->donneCodeSens(&sTemp, &sUnite) ;
            break ;
          }
          iter++ ;
        }

        // sFormat est du type £D0;03
        if ((sFormat != "") && ((sFormat[1] == 'D') || (sFormat[1] == 'T')) && (sValeur != ""))
        {
        	if ((sUnite == "2DA01") || (sUnite == "2DA02"))
          {
          	if (sSens == "KOUVR")
            	pHisPatPatho->setStartDate(sValeur) ;
            else
            	if (sSens == "KFERM")
              	pHisPatPatho->setEndDate(sValeur) ;
          }
        }
        //si le noeud n'a pas de fils on l'efface
        PatPathoClone.SupprimerItem(iterPrec) ;
        iter = iterPrec ;

      }  //if ((sSens == "KOUVR") || (sSens == "KFERM") )
      else
      	iter++ ;
    } //((*iter)->pDonnees->getColonne() == 0)
    else
    	iter++ ;
  }

  if (PatPathoClone.empty())
  {
  	pHisPatPatho->setLibelle("?") ;
    return pHisPatPatho ;
  }

  string sLang = pContexte->getUserLanguage() ;

  GlobalDkd Dcode(pContexte, sLang, "", &PatPathoClone) ;
  Dcode.decodeNoeud("") ;
  sLib = Dcode.getDcodeur() ;

  iter = PatPathoClone.begin() ;
  int iColBase = (*iter)->getColonne() ;
  sElemLex = string((*iter)->getLexique()) ;
  pContexte->getDico()->donneCodeSens(&sElemLex, &sSens) ;

  if (sSens == "LBARZ")
  {
  	iter++ ;
    string sAld = "" ;

    if (iter != PatPathoClone.end())
    {
    	// on cherche ici un edit lexique (ayant comme fils un code lexique)
      sElemLex = (*iter)->getLexique() ;
      if (string(sElemLex, 0, 3) == string("£C;"))
      {
      	iter++ ;
        while ((iter != PatPathoClone.end()) && ((*iter)->getColonne() > iColBase + 1))
        {
        	// on cherche ici un code lexique pour un libelle
          string sCode = (*iter)->getLexique() ;
          pContexte->getDico()->donneLibelle(sLang, &sCode, &sAld) ;

          iter++ ;
        }
      }
    }

    if ((sAld != "") && (sLib != ""))
    	sLib += string(" (") + sAld + string(")") ;
  }

	pHisPatPatho->setLibelle(sLib) ;
  // pHisPatPatho->pPatPathoArray = pPatPathoArray;

	return pHisPatPatho ;
}
catch (...)
{
	erreur("Exception NSHistorizedListWindow::TrouveObjectData.", standardError, 0) ;
  return NULL ;
}
}

void
NSHistorizedListWindow::AfficheListe()
{

	DeleteAllItems();    /*pHistory.vider();

    NSHistoryManagement* pHisPatPatho =  new NSHistoryManagement();
    for (uint i = 0; i < pVectData->size(); i++)
    {        pHisPatPatho = TrouveObjectData((*pVectData)[i]);         //pHisPatPatho = TrouveObjectData((pVectData)[i]);        if(pHisPatPatho)             pHistory.push_back(pHisPatPatho);    }    */

    if (aHistory.empty())
        return ;

    NSHistoryManagementArray ::reverse_iterator itReverse ;
    itReverse = aHistory.rbegin() ;

    while (itReverse != aHistory.rend())
    {
        string sLibelle = (*itReverse)->getLibelle();
        if (sLibelle != "")
        {
            //string sLibelle;
           // pContexte->getDico()->donneLibelle(pContexte->getUserLanguage(), &sCodeSens, &sLibelle) ;

            TListWindItem Item(sLibelle.c_str() , 0) ;
            InsertItem(Item);
        }
        itReverse++ ;
    }
}

void
NSHistorizedListWindow::LvnGetDispInfo(TLwDispInfoNotify& dispInfo)
{
	string sDateAff ;
  string sDate ;
  TListWindItem& dispInfoItem = *(TListWindItem*)&dispInfo.item ;
  int index = dispInfoItem.GetIndex() ;

	string sLang = pContexte->getUserLanguage() ;

	// Affiche les informations en fonction de la colonne	switch (dispInfoItem.GetSubItem())
	{
  	case 1: // date début

    	sDate = (aHistory[index])->getStartDate() ;
      if (string("00000000") == sDate)
      	sDateAff = string("") ;
      else
      	if (string("") != sDate)
        	sDateAff = donne_date(sDate, sLang) ;
        else
        	sDateAff = string("") ;

      dispInfoItem.SetText(sDateAff.c_str()) ;
      break ;

    case 2: // date fin

    	sDate = (aHistory[index])->getEndDate() ;
      if (string("99999999") == sDate)
      	sDateAff = string("") ;
      else
      	if (string("") != sDate)
        	sDateAff = donne_date(sDate, sLang) ;
        else
        	sDateAff = string("") ;

      dispInfoItem.SetText(sDateAff.c_str()) ;
      break ;
  }
}

void
NSHistorizedListWindow::LVNColumnclick(TLwNotify& lwn)
{
	switch ( lwn.iSubItem )
	{
  	case 0 :
    	sortByLibelle();
      break;
    case 1 :
    	sortByStartingDate();
      break;
    case 2 :
    	sortByEndingDate();
      break;
	}
}

void
NSHistorizedListWindow::sortByLibelle()
{
    if (iSortedColumn == 0)
    {
        if (bNaturallySorted)
            bNaturallySorted = false ;
        else
            bNaturallySorted = true ;
    }
    else
    {
        iSortedColumn = 0 ;
        bNaturallySorted = true ;
    }

    if (aHistory.empty())
        return ;

    if (bNaturallySorted)
        sort(aHistory.begin(), aHistory.end(), drugSortByLibelleSup) ;
    else
        sort(aHistory.begin(), aHistory.end(), drugSortByLibelleInf) ;

    AfficheListe() ;
}

void
NSHistorizedListWindow::sortByStartingDate()
{
    if (iSortedColumn == 1)
    {
        if (bNaturallySorted)
            bNaturallySorted = false ;
        else
            bNaturallySorted = true ;
    }
    else
    {
        iSortedColumn = 1 ;
        bNaturallySorted = true ;
    }

    if (aHistory.empty())
        return ;

    if (bNaturallySorted)
        sort(aHistory.begin(), aHistory.end(), drugSortByStartDataSup) ;
    else
        sort(aHistory.begin(), aHistory.end(), drugSortByStartDataInf) ;

    AfficheListe() ;
}

void
NSHistorizedListWindow::sortByEndingDate()
{
    if (iSortedColumn == 2)
    {
        if (bNaturallySorted)
            bNaturallySorted = false ;
        else
            bNaturallySorted = true ;
    }
    else
    {
        iSortedColumn = 2 ;
        bNaturallySorted = true ;
    }

    if (aHistory.empty())
        return ;

    if (bNaturallySorted)
        sort(aHistory.begin(), aHistory.end(), drugSortByEndDataSup) ;
    else
        sort(aHistory.begin(), aHistory.end(), drugSortByEndDataInf) ;

 //   initDataListe();
    AfficheListe() ;
}

void
NSHistorizedListWindow::activeControle(int activation, Message* /* pMessage */)
{
	// on ne gère pas le cas BF_CHECKED avant SetupWindow() pour éviter que les nouveaux fils
	// n'activent la liste en cours de création.
	// le cas BF_UNCHECKED provient en principe d'un père du controle de liste
	// on le conserve pour permettre à la liste de se vider si on la désactive.
	if (!pVectData || !bSetupWindow)
		return ;

	if (activation == BF_CHECKED)
	{
  	// On recharge les données à partir du pTransfert
    *pVectData = *(pControle->getTransfert()->getPatPatho()) ;
    bSetupWindow = false ;
	}
	if (activation == BF_UNCHECKED)
  	// Le controle de liste est désactivé : on vide l'array des données
		pVectData->vider() ;

	initCurentListe() ;
	AfficheListe() ;
}

void
NSHistorizedListWindow::CmCreerEnreg()
{
	if (!pVectData)
		return ;

	// on crée ici un nouveau "fils" au pBBFilsItem lié au controle de liste
	// cela crée en fait un nouveau BBItem dans le VecteurFils de pBBFilsItem
	// puis active le BBItem qui lance l'archetype d'édition d'adresse à l'aide
	// d'un fil guide.
	if (pControle->getTransfert())
	{
  	pControle->getTransfert()->getFilsItem()->creerNouveauFils() ;

    // on récupère le vecteur de transfert, auquel on vient d'ajouter un fils (une patpatho)
    // si l'utilisateur est sorti par CmCancel(), cette dernière patpatho est vide
    // on doit donc faire l'inverse de creerNouveauFils() : vider la patpatho du transfert
    // et détruire le dernier BBItem du pBBFilsItem père.
    NSVectFatheredPatPathoArray* pVectTransfert    = pControle->getTransfert()->getPatPatho() ;
    NSVectFatheredPatPathoArray* pTmpVectTransfert = pControle->getTransfert()->getTmpPatho() ;

    if (false == pVectTransfert->empty())
    {
    	FatheredPatPathoIterVect iterVect = pVectTransfert->end() ;
      iterVect-- ;
      if ((*iterVect)->getPatPatho()->empty())
      {
      	// on détruit la dernière patpatho ainsi que la dernière patpatho temporaire
        delete (*iterVect) ;
        pVectTransfert->erase(iterVect) ;

        FatheredPatPathoIterVect iterTmpVect = pTmpVectTransfert->end() ;
        iterTmpVect-- ;
        delete (*iterTmpVect) ;
        pTmpVectTransfert->erase(iterTmpVect) ;
      }

      // on vide le vecteur des fils (on delete celui que l'on vient de créer)
      // car ce vecteur doit être vide pour permettre la modification
      pControle->getTransfert()->getFilsItem()->tuerFils() ;

      // On recharge les données à partir du pTransfert
      *pVectData = *pVectTransfert ;
      // On rafraichit la liste
      initCurentListe() ;
      AfficheListe() ;
    }
  }
}

void
NSHistorizedListWindow::CmModifEnreg()
{
	if (!pVectData)
		return ;

	int index = IndexItemSelect() ;

	if (index == -1)	{
  	string sText = pContexte->getSuperviseur()->getText("generalLanguageForLists", "anElementMustBeSelected") ;
		erreur(sText.c_str(), warningError, 0) ;
		return ;
	}
	int iSelElem = aHistory[index]->getPosition() ;

	// on envoie ici un ctrlNotification à l'elt index de pVectData
	// ce qui lance l'infernal processus de modification de l'enregistrement correspondante
	if (pControle->getTransfert())
	{
  	pControle->getTransfert()->ctrlNotification(BF_UNCHECKED, BF_CHECKED, 0, iSelElem) ;

    // On recharge les données à partir du pTransfert
    *pVectData = *(pControle->getTransfert()->getPatPatho()) ;
    // On rafraichit la liste
    initCurentListe() ;
    AfficheListe() ;
	}
}


void
NSHistorizedListWindow::CmSupprEnreg()
{
	if (!pVectData)
		return ;

	int index = IndexItemSelect();
	if (index == -1)
	{
  	string sText = pContexte->getSuperviseur()->getText("generalLanguageForLists", "anElementMustBeSelected") ;
		erreur(sText.c_str(), warningError, 0) ;
		return ;
	}
  int iSelElem = aHistory[index]->getPosition() ;
	// on doit supprimer la patpatho n°index du vecteur de transfert
	NSVectFatheredPatPathoArray* pVectTransfert = pControle->getTransfert()->getPatPatho() ;

	int k = 0 ;
	for (FatheredPatPathoIterVect i = pVectTransfert->begin(); i != pVectTransfert->end(); i++)
	{
  	if (k == iSelElem)
    {
    	delete (*i) ;
      pVectTransfert->erase(i) ;
      break ;
    }

    k++ ;
	}

	// On recharge les données à partir du pTransfert
	*pVectData = *(pControle->getTransfert()->getPatPatho()) ;
	// On rafraichit la liste
	initCurentListe() ;
	AfficheListe() ;
}

bool
NSHistorizedListWindow::canWeClose()
{
	return true ;
}


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// Class NSHistoryValueManagement
// -----------------------------------------------------------------------------
NSHistoryValueManagement::NSHistoryValueManagement()
{
	Reset() ;

  lObjectCount++ ;
}


NSHistoryValueManagement::~NSHistoryValueManagement()
{
  lObjectCount-- ;
}

NSHistoryValueManagement::NSHistoryValueManagement(const NSHistoryValueManagement& rv)
{
	sLabel    = rv.sLabel ;
	sDate     = rv.sDate ;
  sDateFin  = rv.sDateFin ;

	sValue    = rv.sValue ;
	sUnit     = rv.sUnit ;
  sNormale  = rv.sNormale ;
  sNormInf  = rv.sNormInf ;
  sNormSup  = rv.sNormSup ;

  sValue2   = rv.sValue2 ;
	sUnit2    = rv.sUnit2 ;
  sNormale2 = rv.sNormale2 ;
  sNormInf2 = rv.sNormInf2 ;
  sNormSup2 = rv.sNormSup2 ;

	sNode     = rv.sNode ;
	iTreePos  = rv.iTreePos ;

  lObjectCount++ ;
}

void
NSHistoryValueManagement::initFromNum(gereNum* pNum, NSContexte * /*pCtx*/)
{
  if ((gereNum*) NULL == pNum)
    return ;

  // Main values
  //
	sValue = pNum->getNum() ;

  string sTemp = pNum->getUnite() ;
  NSDico::donneCodeSens(&sTemp, &sUnit) ;

  if (pNum->getDate())
  {
  	string sDateVal = pNum->getDate()->getDate() ;
    string sUnite = pNum->getDate()->getUnit() ;
    if ((sUnite == "2DA01") || (sUnite == "2DA02"))
    	sDate = sDateVal ;
  }

  gereNum* pNormale = pNum->getNorm() ;
  if (pNormale)
  {
  	sTemp = pNormale->getUnite() ;
    string sNormUnitSens ;
  	NSDico::donneCodeSens(&sTemp, &sNormUnitSens) ;
    if (sNormUnitSens == sUnit)
    	sNormale = pNormale->getNum() ;
  }

  gereNum* pNormInf = pNum->getNormInf() ;
  if (pNormInf)
  {
  	sTemp = pNormInf->getUnite() ;
    string sNormUnitSens ;
  	NSDico::donneCodeSens(&sTemp, &sNormUnitSens) ;
    if (sNormUnitSens == sUnit)
    	sNormInf = pNormInf->getNum() ;
  }

  gereNum* pNormSup = pNum->getNormInf() ;
  if (pNormSup)
  {
  	sTemp = pNormSup->getUnite() ;
    string sNormUnitSens ;
  	NSDico::donneCodeSens(&sTemp, &sNormUnitSens) ;
    if (sNormUnitSens == sUnit)
    	sNormSup = pNormSup->getNum() ;
  }

  // Secondary values
  //
	sValue2 = pNum->getNum(1) ;
  if (sValue2 == string(""))
  	return ;

  sTemp = pNum->getUnite(1) ;
  if (sTemp == string(""))
  {
  	sValue2 = string("") ;
  	return ;
  }
  NSDico::donneCodeSens(&sTemp, &sUnit2) ;

  pNormale = pNum->getNorm(1) ;
  if (pNormale)
  {
  	sTemp = pNormale->getUnite() ;
    string sNormUnitSens ;
  	NSDico::donneCodeSens(&sTemp, &sNormUnitSens) ;
    if (sNormUnitSens == sUnit)
    	sNormale = pNormale->getNum() ;
  }

  pNormInf = pNum->getNormInf(1) ;
  if (pNormInf)
  {
  	sTemp = pNormInf->getUnite() ;
    string sNormUnitSens ;
  	NSDico::donneCodeSens(&sTemp, &sNormUnitSens) ;
    if (sNormUnitSens == sUnit)
    	sNormInf = pNormInf->getNum() ;
  }

  pNormSup = pNum->getNormInf(1) ;
  if (pNormSup)
  {
  	sTemp = pNormSup->getUnite() ;
    string sNormUnitSens ;
  	NSDico::donneCodeSens(&sTemp, &sNormUnitSens) ;
    if (sNormUnitSens == sUnit)
    	sNormSup = pNormSup->getNum() ;
  }
}

void
NSHistoryValueManagement::initFromDate(gereDate* pDate, NSContexte * /*pCtx*/)
{
  if ((gereDate*) NULL == pDate)
    return ;

  // Main values
  //
	sValue = pDate->getDate() ;

  string sTemp = pDate->getUnit() ;
  NSDico::donneCodeSens(&sTemp, &sUnit) ;
}

void
NSHistoryValueManagement::initFromTime(gereHeure* pTime, NSContexte* /* pCtx */)
{
  if (NULL == pTime)
    return ;

  // Main values
  //
	sValue = pTime->getHeure() ;
}

void
NSHistoryValueManagement::initFromCode(gereCode* pHour, NSContexte* /* pCtx */)
{
  if (NULL == pHour)
    return ;

  // Main values
  //
	sValue = pHour->getCode() ;
}

NSHistoryValueManagement&
NSHistoryValueManagement::operator=(const NSHistoryValueManagement& src)
{
	if (this == &src)
		return *this ;

	sLabel    = src.sLabel ;
  sDate     = src.sDate ;
  sDateFin  = src.sDateFin ;

  sValue    = src.sValue ;
  sUnit     = src.sUnit ;
  sNormale  = src.sNormale ;
  sNormInf  = src.sNormInf ;
  sNormSup  = src.sNormSup ;

  sValue2   = src.sValue2 ;
	sUnit2    = src.sUnit2 ;
  sNormale2 = src.sNormale2 ;
  sNormInf2 = src.sNormInf2 ;
  sNormSup2 = src.sNormSup2 ;

	sNode     = src.sNode ;
	iTreePos  = src.iTreePos ;

	return *this ;
}

// -----------------------------------------------------------------------------
// class NSHistoryValManagementArray
// -----------------------------------------------------------------------------

NSHistoryValManagementArray::NSHistoryValManagementArray(NSContexte* pCtx, historyType iType)
                            :NSHistoryValManArray(), NSRoot(pCtx)
{
  _iType = iType ;
}

NSHistoryValManagementArray::NSHistoryValManagementArray(NSHistoryValManagementArray& rv)
                            :NSHistoryValManArray(), NSRoot(rv.pContexte)
{
try
{
	if (!(rv.empty()))
		for (NSHistoryValManagementIter i = rv.begin() ; i != rv.end() ; i++)
			push_back(*i) ;
}
catch (...)
{
	erreur("Exception NSHistoryValManagementIter ctor.", standardError, 0) ;
}
}

void
NSHistoryValManagementArray::vider()
{
	if (empty())
		return ;

	for (NSHistoryValManagementIter i = begin() ; i != end() ; )
	{
		delete *i ;
		erase(i) ;
	}
}

NSHistoryValManagementArray::~NSHistoryValManagementArray()
{
	vider() ;
}

bool
NSHistoryValManagementArray::ChargeHistoryValueItem(string sItem)
{
try
{
	if (string("") == sItem)
		return false ;

  sItem = getRegularPath(sItem, cheminSeparationMARK, intranodeSeparationMARK) ;

	// on recherche à partir du code sens et non du code lexique
	// if sItem is a Meaning Code and not a Lexique Code
	string  sItemSens ;
	pContexte->getDico()->donneCodeSens(&sItem, &sItemSens) ;

	// on vérifie que l'historique exsite et qu'il contient des documents
	// does the patient datas' history exist and does it contain documents
  NSHISTODocument *pDocHis = pContexte->getPatient()->getDocHis() ;
	if ((NULL == pDocHis) || (pDocHis->getVectDocument()->empty()))
		return false ;	string sLang = pContexte->getUserLanguage() ;	// est-ce que le code lexique passé en argument est valide	// is "lexique" code item passed in argument valid	// if (strlen(sItem.c_str()) != BASE_LEXI_LEN)	//    return false ;	// initialisation des variables	// initialization of variables	NSPatPathoArray          PPT(pContexte->getSuperviseur()) ;	NSHistoryValueManagement Value ;	string  sLabel ;	string  sNode ;	string  sValue ;	string  sDate ;	string  sUnit ;	string  sLocalis ;	// on vide la liste courante	// we clear the current list	vider() ;	// on parcourt les documents de l'historique	// parsing documents in patient datas' history	DocumentIter DocIt = pDocHis->getVectDocument()->begin() ;	for ( ; pDocHis->getVectDocument()->end() != DocIt ; DocIt++)	{    NSPatPathoArray DocPpt(pContexte->getSuperviseur()) ;    if (*DocIt)      (*DocIt)->initFromPatPatho(&DocPpt) ;		// traitement de la patpatho du document		// document's patpatho processing		if (false == DocPpt.empty())		{			// on vérifie au préalable s'il s'agit du document "Index de santé"			PatPathoIter pptIt = DocPpt.begin() ;			string sLexRoot    = (*pptIt)->getLexique() ;			string sSensRoot ;			bool bIsHealthIndex = false ;			// Est-ce que le document est l'Index de Santé			// Does the document is the health index			pContexte->getDico()->donneCodeSens(&sLexRoot, &sSensRoot) ;			if (sSensRoot == "ZPOMR")				bIsHealthIndex = true ;			// on cherche maintenant tous les sItem du document			// searching item in document			if (strlen(sItem.c_str()) == BASE_LEXI_LEN)				pptIt = DocPpt.ChercherItem(sItemSens) ;			else				pptIt = DocPpt.ChaineDansPatpatho(sItemSens, &sLocalis, "/", &pptIt) ;			while ((NULL != pptIt) && (DocPpt.end() != pptIt))			{				// s'il s'agit de l'index de santé, on vérifiera que la valeur (sItem)				// recherchée appartient bien au chapître "traitement"				// if document is health index, we check if the searching item is in				// Treatment chapter				if (bIsHealthIndex)				{					string sCheminItem = DocPpt.donneCheminItem(pptIt) ;					string sCheminSens ;					pContexte->getDico()->donneCodeSens(&sCheminItem, &sCheminSens) ;					// does item found is in treatment chapter of health index					// on vérifie que l'item trouvé appartient au chapître "traitement" (N00001)					size_t pos = sCheminSens.find("ZPOMR/N0000") ;					if (pos != 0)					{						// cas où l'amorce n'est pas trouvée au début du chemin de l'item :						// on passe à l'item suivant et on réitère						if (strlen(sItem.c_str()) == BASE_LEXI_LEN)							pptIt = DocPpt.ChercherItem(sItemSens, true, pptIt) ;						else							pptIt = DocPpt.ChaineDansPatpatho(sItemSens, &sLocalis, "/", &pptIt) ;						continue ;					}				}				PPT.vider() ;				Value.Reset() ;				// récupération du titre du document				// Document's Title        string sAffiche = (*DocIt)->getDocName() ;        strip(sAffiche, stripBoth, ' ') ;

        if (string("Synthèse") != sAffiche)
        {
          if (string("") != (*DocIt)->getDateDoc())
            sAffiche += string(" du ") + donne_date((*DocIt)->getDateDoc(), sLang) ;
        }

				// We put document's title in sLabel
				// On met le Titre du document dans sLabel				Value.setLabel(sAffiche) ;				Value.setDate((*DocIt)->getDateDoc()) ;				Value.setNode((*pptIt)->getNode()) ;				// Extraction de la sous-patpatho et parsing de la valeur				// Extraction of sub-patpatho and pasing its value				DocPpt.ExtrairePatPatho(pptIt, &PPT) ;        AddNewValue(&PPT, &Value) ;/*				string sLang = pContexte->getUserLanguage() ;				GlobalDkd Dcode(pContexte, sLang, "", &PPT) ;				Dcode.setPPtArray(&PPT) ;        Dcode.initialiseIterateurs() ;				gereNum Num(pContexte, sLang) ;				Dcode.donneDimension(-1, &Num) ;
				if (Dcode.iBon())
				{
        	Value.initFromNum(&Num, pContexte) ;
        	push_back(new NSHistoryValueManagement(Value)) ;
				}
*/
				// on itère la recherche suivante dans le même document
				// next corresponding record in the document				if (strlen(sItem.c_str()) == BASE_LEXI_LEN)					pptIt = DocPpt.ChercherItem(sItemSens, true, pptIt) ;				else					pptIt = DocPpt.ChaineDansPatpatho(sItemSens, &sLocalis, "/", &pptIt) ;			}		}		// Si la patpatho du document n'est pas accessible, on cherche dans le    // chapitre "données de suivi" du meta		// If the document's patpatho is not available, we are looking for in    // the "follow up data" chapter of the meta		else		{      NSPatPathoArray PptMeta(pContexte->getSuperviseur()) ;      (*DocIt)->initFromMeta(&PptMeta) ;      if (false == PptMeta.empty())      {        // on vérifie au préalable qu'il existe un chapitre "données de suivi"        PatPathoIter pptIt = PptMeta.ChercherItem("9SUIV") ;        if ((PptMeta.end() != pptIt) && (NULL != pptIt))        {          // on cherche maintenant tous les sItem du document          // searching item in document          if (strlen(sItem.c_str()) == BASE_LEXI_LEN)            pptIt = PptMeta.ChercherItem(sItemSens, false, pptIt) ;          else            pptIt = PptMeta.ChaineDansPatpatho(sItemSens, &sLocalis, "/", &pptIt) ;          while ((NULL != pptIt) && (PptMeta.end() != pptIt))          {            PPT.vider() ;            Value.Reset() ;            // récupération du titre du document            // Document's Title            string sAffiche = (*DocIt)->getDocName() ;            strip(sAffiche, stripBoth, ' ') ;

            if (string("Synthèse") != sAffiche)
            {
              if (string("") != (*DocIt)->getDateDoc())
                sAffiche += string(" du ") + donne_date((*DocIt)->getDateDoc(), sLang) ;
            }

            // We put document's title in sLabel
            // On met le Titre du document dans sLabel            Value.setLabel(sAffiche) ;            Value.setDate((*DocIt)->getDateDoc()) ;            Value.setNode((*pptIt)->getNode()) ;            // Extraction de la sous-patpatho et parsing de la valeur            // Extraction of sub-patpatho and pasing its value            PptMeta.ExtrairePatPatho(pptIt, &PPT) ;            AddNewValue(&PPT, &Value) ;  /*            string sLang = pContexte->getUserLanguage() ;            GlobalDkd Dcode(pContexte, sLang, "", &PPT) ;            Dcode.setPPtArray(&PPT) ;            Dcode.initialiseIterateurs() ;            switch (_iType)            {              case histTypeNum :              {                gereNum Num(pContexte, sLang) ;                Dcode.donneDimension(-1, &Num) ;
                if (Dcode.iBon())
                {
                  Value.initFromNum(&Num, pContexte) ;
                  push_back(new NSHistoryValueManagement(Value)) ;
                }
                break ;
              }
              case histTypeDate :
              {                gereDate Date(pContexte, sLang) ;                Dcode.donneDate(-1, &Date) ;
                if (Dcode.iBon())
                {
                  Value.initFromDate(&Date, pContexte) ;
                  push_back(new NSHistoryValueManagement(Value)) ;
                }
                break ;
              }
              case histTypeTime :
              {                gereHeure Time(pContexte, sLang) ;                Dcode.donneHeure(-1, &Time) ;
                if (Dcode.iBon())
                {
                  Value.initFromTime(&Time, pContexte) ;
                  push_back(new NSHistoryValueManagement(Value)) ;
                }
                break ;
              }
              case histTypeCode :
              {                gereCode Code(pContexte, sLang) ;                Dcode.donneCode(-1, &Code) ;
                if (Dcode.iBon())
                {
                  Value.initFromCode(&Code, pContexte) ;
                  push_back(new NSHistoryValueManagement(Value)) ;
                }
                break ;
              }
            }
  */

            // on itère la recherche suivante dans le même document            // next corresponding record in the document            if (strlen(sItem.c_str()) == BASE_LEXI_LEN)              pptIt = PptMeta.ChercherItem(sItemSens, true, pptIt) ;            else              pptIt = PptMeta.ChaineDansPatpatho(sItemSens, &sLocalis, "/", &pptIt) ;          }        }      }		}	}	if (false == empty())		return true ;	return false ;}
catch (...)
{
	erreur("Exception NSHistoryValManagementArray::ChargeHistoryValueItem.", standardError, 0) ;
	return false ;
}
}

void
NSHistoryValManagementArray::AddNewValue(NSPatPathoArray *pPt, NSHistoryValueManagement *pValue)
{
try
{
  if ((NULL == pPt) || (true == pPt->empty()) || (NULL == pValue))
    return ;

  string sLang = pContexte->getUserLanguage() ;
  GlobalDkd Dcode(pContexte, sLang, "", pPt) ;  Dcode.setPPtArray(pPt) ;  Dcode.initialiseIterateurs() ;  switch (_iType)  {    case histTypeNum :    {      gereNum Num(pContexte->getSuperviseur(), sLang) ;      Dcode.donneDimension(-1, &Num) ;
      if (Dcode.iBon())
      {
        pValue->initFromNum(&Num, pContexte) ;
        push_back(new NSHistoryValueManagement(*pValue)) ;
      }
      break ;
    }
    case histTypeDate :
    {      gereDate Date(pContexte->getSuperviseur(), sLang) ;      Dcode.donneDate(-1, &Date) ;
      if (Dcode.iBon())
      {
        pValue->initFromDate(&Date, pContexte) ;
        push_back(new NSHistoryValueManagement(*pValue)) ;
      }
      break ;
    }
    case histTypeTime :
    {      gereHeure Time(pContexte->getSuperviseur(), sLang) ;      Dcode.donneHeure(-1, &Time) ;
      if (Dcode.iBon())
      {
        pValue->initFromTime(&Time, pContexte) ;
        push_back(new NSHistoryValueManagement(*pValue)) ;
      }
      break ;
    }
    case histTypeCode :
    {      gereCode Code(pContexte, sLang) ;      Dcode.donneCode(-1, &Code) ;
      if (Dcode.iBon())
      {
        pValue->initFromCode(&Code, pContexte) ;
        push_back(new NSHistoryValueManagement(*pValue)) ;
      }
      break ;
    }
  }
}
catch (...)
{
	erreur("Exception NSHistoryValManagementArray::AddNewValue.", standardError, 0) ;
}
}

bool
NSHistoryValManagementArray::ChargeCaptureValueItem(string sItem)
{
try
{
	if (sItem == "")
		return false ;

	NSEpisodus* pEpisodus = pContexte->getEpisodus() ;
  if (pEpisodus->CaptureArray.empty())
		return false ;

	// on recherche à partir du code sens et non du code lexique
	// if sItem is a Meaning Code and not a Lexique Code
	string  sItemSens ;
	pContexte->getDico()->donneCodeSens(&sItem, &sItemSens) ;

  ClasseStringVector ItemElements ;
  DecomposeChaine(&sItemSens, &ItemElements, "/");
  if (ItemElements.empty())
		return false ;

  NSHistoryValueManagement HistoValue ;

	NVLdVTemps tpsNow ;
  tpsNow.takeTime() ;

	for (CaptureIter captIter = pEpisodus->CaptureArray.begin() ; captIter != pEpisodus->CaptureArray.end() ; captIter++)
	{
  	size_t posInCapt = 0 ;
    iterClassString itItemElts = ItemElements.begin() ;
  	for ( ; (posInCapt != NPOS) && (itItemElts != ItemElements.end()) ; itItemElts++)
    	posInCapt = (*captIter)->getChemin().find((*itItemElts)->getItem(), posInCapt) ;

    if (posInCapt != NPOS)
		{
    	NSHistoryValueManagement  *pValue = new NSHistoryValueManagement() ;
    	// sauvegarde des attributs de la valeur historisée
      // saving attributes of historized value      pValue->setLabel("CAPTURE") ;      pValue->setValue((*captIter)->getLibelle()) ;      if ((*captIter)->getUnit() != string(""))			{        string sUnitSens = "" ;        string sUnit = (*captIter)->getUnit() ;      	pContexte->getDico()->donneCodeSens(&sUnit, &sUnitSens) ;      	pValue->setUnit(sUnitSens) ;			}      if ((*captIter)->getDate() == string(""))      	pValue->setDate(tpsNow.donneDateHeure()) ;      else      	pValue->setDate((*captIter)->getDate()) ;      pValue->setNode("") ;      push_back(pValue) ;
		}
	}

  return true ;
}
catch (...)
{
	erreur("Exception NSHistoryValManagementArray::ChargeCaptureValueItem.", standardError, 0) ;
	return false ;
}
}

void
NSHistoryValManagementArray::ChargeValueFromPatPatho(const NSPatPathoArray* pPatPathoArray, PatPathoConstIter iterVal, NVLdVTemps* pDefaultDate){
try
{
	if (NULL == pPatPathoArray)
		return ;

	// Extraction de la sous-patpatho et parsing de la valeur
	string          sElemLex, sSens, sDateVal = "" ;	string          sDate = "", sValue = "", sUnit = "" ;	NSPatPathoArray PPT(pContexte->getSuperviseur()) ;	pPatPathoArray->ExtrairePatPatho(iterVal, &PPT) ;	if (PPT.empty())		return ;	PatPathoIter iter = PPT.begin() ;	int iColBase = (*iter)->getColonne() ;

	while ((iter != PPT.end()) && ((*iter)->getColonne() >= iColBase))
	{
		sElemLex = (*iter)->getLexique() ;
		pContexte->getDico()->donneCodeSens(&sElemLex, &sSens) ;

		// Chapitre "DATE" / DATE chapter
		if (sSens == string("KDARE"))
		{			iter++ ;			int iLigneBase = (*iter)->getLigne() ;
			string sUnite  = "" ;
			string sFormat = "" ;
			string sTemp   = "" ;

			while ((iter != PPT.end()) && ((*iter)->getLigne() == iLigneBase))
			{
        string sLexique = (*iter)->getLexique() ;
				if ((string("") != sLexique) && (sLexique[0] == '£'))
        {
        	sTemp = (*iter)->getLexique() ;
          pContexte->getDico()->donneCodeSens(&sTemp, &sFormat) ;
          sDateVal = (*iter)->getComplement() ;
          sTemp = (*iter)->getUnit() ;
          pContexte->getDico()->donneCodeSens(&sTemp, &sUnite) ;
          iter++ ;
          break ;
        }
				iter++;
			}

      // sFormat est du type £D0;10
      if ((sFormat != "") && ((sFormat[1] == 'D') || (sFormat[1] == 'T')) && (sDateVal != ""))
      {
      	if ((sUnite == "2DA01") || (sUnite == "2DA02"))
        	sDate = sDateVal ;
      }
		}		else if (string(sElemLex, 0, 2) == "£N")		{    	int iLigneBase = (*iter)->getLigne() ;      string sTemp = "" ;
      string sFormat = "" ;

      while ((iter != PPT.end()) && ((*iter)->getLigne() == iLigneBase))
      {
        string sLexique = (*iter)->getLexique() ;
				if ((string("") != sLexique) && (sLexique[0] == '£'))
        {
        	sTemp  = (*iter)->getLexique() ;
          pContexte->getDico()->donneCodeSens(&sTemp, &sFormat) ;
          sValue = (*iter)->getComplement() ;
          sTemp  = (*iter)->getUnit() ;
          pContexte->getDico()->donneCodeSens(&sTemp, &sUnit) ;
          iter++ ;
          break ;
        }
				iter++ ;
			}		}    else    	iter++ ;	} // fin du bloc while de parsing  if ((string("") == sDate) && (NULL != pDefaultDate))    sDate = pDefaultDate->donneDateHeure() ;	NSHistoryValueManagement* pValue = new NSHistoryValueManagement() ;	// ici on n'est pas dans le cas d'un document (la patpatho est une "answer")	// => sLabel et sNode sont vides.	pValue->setLabel("") ;	pValue->setValue(sValue) ;	pValue->setUnit(sUnit) ;	pValue->setDate(sDate) ;	pValue->setNode("") ;	push_back(pValue) ;
}
catch (...)
{
	erreur("Exception NSHistoryValManagementArray::ChargeValueFromPatPatho.", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------
//
// Class NSHistorizedListWindow
//
// -----------------------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSHistorizedValListWindow, TListWindow)
  EV_WM_SETFOCUS,
  EV_WM_KILLFOCUS,
  EV_WM_RBUTTONDOWN,
  EV_WM_LBUTTONDBLCLK,  //double click
  NS_LVN_GETDISPINFO(LvnGetDispInfo),
  //EV_LVN_COLUMNCLICK(IDC_LISTHIST,LVNColumnclick),
  NS_LW_COLUMNCLICK(LVNColumnclick),
  EV_WM_KEYDOWN,
  EV_WM_CHAR,
  EV_COMMAND(IDC_NEWENREG, CmCreerEnreg),
  EV_COMMAND(IDC_MODENREG, CmModifEnreg),
  EV_COMMAND(IDC_DELENREG, CmSupprEnreg),
  EV_COMMAND(IDC_HISTORY,  CmHistory),
END_RESPONSE_TABLE ;

NSHistorizedValListWindow::NSHistorizedValListWindow(TWindow *parent, NSContexte *pCtx, int id, int x, int y, int w, int h,
                                                        bool bModeHis, TModule *module)
                        :NSSkinableListWindow(parent, pCtx, id, x, y, w, h, module),
                         _aHistory(pCtx)
{
	_pVectData      = (NSVectFatheredPatPathoArray*) 0 ;
	_pControle      = (NSControle*) 0 ;
	_bSetupWindow   = false ;
	_bFirstUse      = true ;
	_bModeHistory   = bModeHis ;
  _iSortedColumn  = -1 ;

  _pDialogHistory = dynamic_cast<NSValListHistoryDialog *>(parent) ;

	Attr.Style      |= LVS_REPORT | LVS_SHOWSELALWAYS ;
	Attr.ExStyle    &= ~WS_EX_NOPARENTNOTIFY ;
}

NSHistorizedValListWindow::NSHistorizedValListWindow(TWindow *parent, NSContexte *pCtx, int resourceId,
                                                        bool bModeHis, TModule *module)
                        :NSSkinableListWindow(parent, pCtx, resourceId, module),
                         _aHistory(pCtx)
{
  _pVectData      = (NSVectFatheredPatPathoArray*) 0 ;
	_pControle      = (NSControle*) 0 ;
  _bSetupWindow   = false ;
  _bFirstUse      = true ;
  _bModeHistory   = bModeHis ;
  _iSortedColumn  = -1 ;

  _pDialogHistory = dynamic_cast<NSValListHistoryDialog *>(parent) ;

  Attr.Style      |= LVS_REPORT | LVS_SHOWSELALWAYS ;
  Attr.ExStyle    &= ~WS_EX_NOPARENTNOTIFY ;
}

NSHistorizedValListWindow::~NSHistorizedValListWindow()
{
	if (_pControle)
		delete _pControle ;
}

void
NSHistorizedValListWindow::EvSetFocus(HWND hWndLostFocus)
{
	skinSwitchOn("historizedValuesListOn") ;
	TListWindow::EvSetFocus(hWndLostFocus) ;

	selectFirstItem() ;
}

void
NSHistorizedValListWindow::selectFirstItem()
{
	int count = GetItemCount() ;

    /*******************************
    for (int i = 0 ; i < count ; i++)
        if (GetItemState(i, LVIS_SELECTED))
            return ;

    SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED, LVIF_STATE) ;
    ******************************/

	if (count > 0)
  	SetSel(0, true) ;
}

void
NSHistorizedValListWindow::EvKillFocus(HWND hWndGetFocus)
{
	skinSwitchOff("historizedValuesListOff") ;
	TListWindow::EvKillFocus(hWndGetFocus) ;
}

void
NSHistorizedValListWindow::EvRButtonDown(uint /* modkeys */, NS_CLASSLIB::TPoint& point)
{
	if (false == _bModeHistory)
	{
  	// création d'un menu popup
    NS_CLASSLIB::TPoint lp = point ;

    TPopupMenu *menu = new TPopupMenu() ;

    menu->AppendMenu(MF_STRING, IDC_NEWENREG, "Créer nouvelle valeur") ;

    if (false == _aHistory.empty())
    {
    	menu->AppendMenu(MF_STRING, IDC_MODENREG, "Modifier valeur") ;
      menu->AppendMenu(MF_STRING, IDC_DELENREG, "Supprimer valeur") ;
    }
    else
    {
    	menu->AppendMenu(MF_GRAYED, 0, "Modifier valeur") ;
      menu->AppendMenu(MF_GRAYED, 0, "Supprimer valeur") ;
    }

    if (false == _pDialogHistory->_bEnCours)
    	menu->AppendMenu(MF_STRING, IDC_HISTORY,  "Historique des valeurs") ;
    else
    	menu->AppendMenu(MF_GRAYED, 0,            "Historique des valeurs") ;

    ClientToScreen(lp) ;
    menu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, lp, 0, HWindow) ;
    delete menu ;
	}
}

void
NSHistorizedValListWindow::EvLButtonDblClk(uint /* modKeys */, NS_CLASSLIB::TPoint& point)
{
	if (_bModeHistory)
		return ;

	TLwHitTestInfo info(point) ;

	HitTest(info) ;
	if (info.GetFlags() & LVHT_ONITEM)  	CmModifEnreg() ;	else		CmCreerEnreg() ;}
void
NSHistorizedValListWindow::EvKeyDown(uint key, uint repeatCount, uint flags)
{
	if (_bModeHistory)
	{
    if (_pDialogHistory)
    {
      if      (VK_ADD == key)
        _pDialogHistory->CmMoreSpecific() ;
      else if (VK_SUBTRACT == key)
        _pDialogHistory->CmMoreElusive() ;
      else
        TListWindow::EvKeyDown(key, repeatCount, flags) ;
    }
    else
		  TListWindow::EvKeyDown(key, repeatCount, flags) ;
    return ;
	}

	if      (key == VK_DELETE)
  	CmSupprEnreg() ;
  else if (key == VK_INSERT)
	{
  	if (GetKeyState(VK_SHIFT) < 0)
    	CmModifEnreg() ;
    else
    	CmCreerEnreg() ;
  }
  else
  	TListWindow::EvKeyDown(key, repeatCount, flags) ;
}

void
NSHistorizedValListWindow::EvChar(uint key, uint repeatCount, uint flags)
{
  if ((_bModeHistory) && (_pDialogHistory))
  {
    if      ('+' == key)
    {
      _pDialogHistory->CmMoreSpecific() ;
      return ;
    }
    else if ('-' == key)
    {
      _pDialogHistory->CmMoreElusive() ;
      return ;
    }
  }
  TListWindow::EvChar(key, repeatCount, flags) ;
}

void
NSHistorizedValListWindow::SetupWindow()
{
try
{
	ListView_SetExtendedListViewStyle(this->HWindow, LVS_EX_FULLROWSELECT) ;

  TListWindow::SetupWindow() ;
  SetupColumns() ;
  if (false == _bModeHistory)
  	_pDialogHistory = new NSValListHistoryDialog(this, pContexte, _sItemValue, "") ;

  if (_bModeHistory)
  {
  	_aHistory.ChargeHistoryValueItem(_sItemValue) ;
    _aHistory.ChargeCaptureValueItem(_sItemValue) ;
    _bFirstUse = false ;
    if (false == _aHistory.empty())
    	sort(_aHistory.begin(), _aHistory.end(), sortByDateSup) ;
    _iSortedColumn    = 1 ;
    _bNaturallySorted = true ;
  }
  else
  {
  	// On recharge les données à partir du pTransfert
    _pVectData = _pControle->getTransfert()->getPatPatho() ;
    initCurentListe() ;
  }

  AfficheListe() ;
  selectFirstItem() ;

  skinSwitchOff("historizedValuesListOff") ;
}
catch (...)
{
	erreur("Exception NSHistorizedValListWindow::SetupWindow.", standardError, 0) ;
}
}

void
NSHistorizedValListWindow::CmReset()
{
	if (false == _bModeHistory)
		return ;

	_aHistory.vider() ;

	// SetupWindow() ;
  if (_bModeHistory)
  {
  	_aHistory.ChargeHistoryValueItem(_sItemValue) ;
    _aHistory.ChargeCaptureValueItem(_sItemValue) ;
    _bFirstUse = false ;
    if (false == _aHistory.empty())
    	sort(_aHistory.begin(), _aHistory.end(), sortByDateSup) ;
    _iSortedColumn    = 1 ;
    _bNaturallySorted = true ;
  }
  else
  {
  	// On recharge les données à partir du pTransfert
    _pVectData = _pControle->getTransfert()->getPatPatho() ;
    initCurentListe() ;
  }

  AfficheListe() ;
  selectFirstItem() ;
}

void
NSHistorizedValListWindow::SetupColumns()
{
	// NS_CLASSLIB::TRect  rectList ;
	// GetWindowRect(rectList) ;
	// int iDimLib = rectList.Width() - 140 ;
	InsertColumn(0, TListWindColumn("Valeur", 190)) ;
	InsertColumn(1, TListWindColumn("Date", 80)) ;
	if (_bModeHistory)
  	InsertColumn(2, TListWindColumn("Document", 260)) ;
}

int
NSHistorizedValListWindow::IndexItemSelect()
{
	int count = GetItemCount() ;
	int index = -1 ;

	for (int i = 0 ; i < count ; i++)	{
  	if (GetItemState(i, LVIS_SELECTED))    {
    	index = i ;
      break ;
    }
  }

	return index ;}

NSHistoryValueManagement*
NSHistorizedValListWindow::getSelectedValue()
{
	int iSel = IndexItemSelect() ;
	if (-1 == iSel)
  	return (NSHistoryValueManagement*) 0 ;

	if (_aHistory.empty())
		return (NSHistoryValueManagement*) 0 ;

	NSHistoryValManagementIter it = _aHistory.begin() ;
	for (int i = 0 ; (_aHistory.end() != it) && (i < iSel) ; i++, it++) ;

	if (_aHistory.end() == it)
  	return (NSHistoryValueManagement*) 0 ;
	else
		return *it ;
}

void
NSHistorizedValListWindow::initCurentListe()
{
	_aHistory.vider() ;

	int iPos = 0 ;
	if ((NULL == _pVectData) || (_pVectData->empty()))
		return ;

	for (FatheredPatPathoIterVect itPPt = _pVectData->begin() ; _pVectData->end() != itPPt ; itPPt++)
	{  	NSHistoryValueManagement *pHisPatPatho = TrouveObjectData((*itPPt)->getPatPatho()) ;    //pHisPatPatho = TrouveObjectData((pVectData)[i]);    if (pHisPatPatho)    {    	pHisPatPatho->setPosition(iPos) ;      if (pHisPatPatho->getDate() == string(""))      	pHisPatPatho->setDate("00000000") ;      if (pHisPatPatho->getValue() == string(""))      	pHisPatPatho->setValue("") ;      _aHistory.push_back(pHisPatPatho) ;    }    iPos++ ;
  }

	if (_aHistory.empty())
		return ;

	if (_bFirstUse)
	{
  	_bFirstUse = false ;

    sort(_aHistory.begin(), _aHistory.end(), sortByDateSup) ;
    _iSortedColumn     = 1 ;
    _bNaturallySorted  = true ;
	}
	else
	{
  	switch (_iSortedColumn)
    {
    	case 0 :
      	if (_bNaturallySorted)
        	sort(_aHistory.begin(), _aHistory.end(), sortByLabelSup) ;
        else
        	sort(_aHistory.begin(), _aHistory.end(), sortByLabelInf) ;
        break ;
			case 1 :
      	if (_bNaturallySorted)
        	sort(_aHistory.begin(), _aHistory.end(), sortByDateSup) ;
        else
        	sort(_aHistory.begin(), _aHistory.end(), sortByDateInf) ;
        break ;
			case 2 :
      	if (_bNaturallySorted)
        	sort(_aHistory.begin(), _aHistory.end(), sortByValueSup) ;
        else
        	sort(_aHistory.begin(), _aHistory.end(), sortByValueInf) ;
        break ;
		}
	}
}

NSHistoryValueManagement*
NSHistorizedValListWindow::TrouveObjectData(NSPatPathoArray *pPPT)
{
try
{
	if ((NULL == pPPT) || (pPPT->empty()))
		return (NSHistoryValueManagement*) 0 ;

	NSHistoryValueManagement *pValue = new NSHistoryValueManagement() ;

	// Extraction de la sous-patpatho et parsing de la valeur
  //
	string sUnit  = string("") ;  string sDate  = string("") ;  string sValue = string("") ;	PatPathoIter iter = pPPT->begin() ;	int iColBase = (*iter)->getColonne() ;

	while ((pPPT->end() != iter) && ((*iter)->getColonne() >= iColBase))
	{
  	string sElemLex = (*iter)->getLexique() ;
    string sSens    = string("") ;
    pContexte->getDico()->donneCodeSens(&sElemLex, &sSens) ;

    // Chapitre "DATE" / DATE chapter
    if (string("KDARE") == sSens)
    {      string sDateVal = string("") ;    	iter++ ;      int iLigneBase = (*iter)->getLigne() ;
      string sUnite  = string("") ;
      string sFormat = string("") ;
      string sTemp   = string("") ;

      while ((pPPT->end() != iter) && ((*iter)->getLigne() == iLigneBase))
      {
        string sLexique = (*iter)->getLexique() ;
      	if ((string("") != sLexique) && (sLexique[0] == '£'))
        {
        	sTemp   = (*iter)->getLexique() ;
          pContexte->getDico()->donneCodeSens(&sTemp, &sFormat) ;
          sDateVal = (*iter)->getComplement() ;
          sTemp   = (*iter)->getUnit() ;
          pContexte->getDico()->donneCodeSens(&sTemp, &sUnite) ;
          iter++ ;
          break ;
        }
        iter++ ;
      }

      // sFormat est du type £D0;10
      if ((sFormat != "") && ((sFormat[1] == 'D') || (sFormat[1] == 'T')) && (sDateVal != ""))
      {
      	if ((sUnite == "2DA01") || (sUnite == "2DA02"))
        	sDate = sDateVal ;
      }
    }    else if (string(sElemLex, 0, 2) == "£N")    {    	int iLigneBase = (*iter)->getLigne() ;      string sTemp   = string("") ;
      string sFormat = string("") ;

      while ((pPPT->end() != iter) && ((*iter)->getLigne() == iLigneBase))
      {
        string sLexique = (*iter)->getLexique() ;
      	if ((string("") != sLexique) && (sLexique[0] == '£'))
        {
        	sTemp  = (*iter)->getLexique() ;
          pContexte->getDico()->donneCodeSens(&sTemp, &sFormat) ;
          sValue = (*iter)->getComplement() ;
          sTemp  = (*iter)->getUnit() ;
          pContexte->getDico()->donneCodeSens(&sTemp, &sUnit) ;
          iter++ ;
          break ;
        }
        iter++ ;
      }    }    else    	iter++ ;	}	pValue->setValue(sValue) ;  pValue->setUnit(sUnit) ;  pValue->setDate(sDate) ;
	return pValue ;
}
catch (...)
{
	erreur("Exception NSHistorizedValListWindow::TrouveObjectData.", standardError, 0) ;
	return (NSHistoryValueManagement*) 0 ;
}
}

void
NSHistorizedValListWindow::AfficheListe()
{
	DeleteAllItems() ;

	if (_aHistory.empty())
		return ;

	NSHistoryValManagementArray::reverse_iterator itReverse ;
	itReverse = _aHistory.rbegin() ;

	while (_aHistory.rend() != itReverse)
	{
  	string sLabel = (*itReverse)->getValue() ;

    if (string("") != sLabel)
    {
    	string sLang = pContexte->getUserLanguage() ;
      string sUnit = (*itReverse)->getUnit() + string("1") ;
      string sLibUnit ;
      pContexte->getDico()->donneLibelle(sLang, &sUnit, &sLibUnit) ;
      sLabel += string(" ") + sLibUnit ;

      TListWindItem Item(sLabel.c_str() , 0) ;
      InsertItem(Item) ;
    }

    itReverse++ ;
	}
}

void
NSHistorizedValListWindow::LvnGetDispInfo(TLwDispInfoNotify& dispInfo)
{
  string sDateDisplay ;
  char   szValueDisplay[256] ;
  string sDisplay ;
  TListWindItem& dispInfoItem = *(TListWindItem*)&dispInfo.item ;
  int index = dispInfoItem.GetIndex() ;

	string sLang = pContexte->getUserLanguage() ;

  // Affiche les informations en fonction de la colonne  switch (dispInfoItem.GetSubItem())
  {
    case 1: // date
            sDisplay = (_aHistory[index])->getDate() ;

            if (string("") == sDisplay)
              sDateDisplay = string("") ;
            else
            {
              sDisplay = string(sDisplay, 0, 8) ;

              if (string("00000000") == sDisplay)
                sDateDisplay = string("") ;
              else
                sDateDisplay = donne_date(sDisplay, sLang) ;
            }

            dispInfoItem.SetText(sDateDisplay.c_str()) ;
            break ;

    case 2: // libelle document (en modeHistory)
            sDisplay = (_aHistory[index])->getLabel() ;

            if (sDisplay == "")
              szValueDisplay[0] = '\0' ;
            else
                sprintf(szValueDisplay, "%s", sDisplay.c_str()) ;

            dispInfoItem.SetText(szValueDisplay, strlen(szValueDisplay)) ;
            break ;
  }
}

void
NSHistorizedValListWindow::LVNColumnclick(TLwNotify& lwn)
{
  switch (lwn.iSubItem)
  {
    case 0 : sortByLabel() ; break ;
    case 1 : sortByDate() ;  break ;
    case 2 : sortByValue() ; break ;
  }
}

void
NSHistorizedValListWindow::sortByLabel()
{
  if (0 == _iSortedColumn)
  {
    if (_bNaturallySorted)
      _bNaturallySorted = false ;
    else
      _bNaturallySorted = true ;
  }
  else
  {
    _iSortedColumn = 0 ;
    _bNaturallySorted = true ;
  }

  if (_aHistory.empty())
    return ;

  if (_bNaturallySorted)
    sort(_aHistory.begin(), _aHistory.end(), sortByLabelSup) ;
  else
    sort(_aHistory.begin(), _aHistory.end(), sortByLabelInf) ;

  AfficheListe() ;
}

void
NSHistorizedValListWindow::sortByDate()
{
  if (1 == _iSortedColumn)
  {
    if (_bNaturallySorted)
      _bNaturallySorted = false ;
    else
      _bNaturallySorted = true ;
  }
  else
  {
    _iSortedColumn = 1 ;
    _bNaturallySorted = true ;
  }

  if (_aHistory.empty())
    return ;

  if (_bNaturallySorted)
    sort(_aHistory.begin(), _aHistory.end(), sortByDateSup) ;
  else
    sort(_aHistory.begin(), _aHistory.end(), sortByDateInf) ;

  AfficheListe() ;
}

void
NSHistorizedValListWindow::sortByValue()
{
  if (2 == _iSortedColumn)
  {
    if (_bNaturallySorted)
      _bNaturallySorted = false ;
    else
      _bNaturallySorted = true ;
  }
  else
  {
    _iSortedColumn = 2 ;
    _bNaturallySorted = true ;
  }

  if (_aHistory.empty())
    return ;

  if (_bNaturallySorted)
    sort(_aHistory.begin(), _aHistory.end(), sortByValueSup) ;
  else
    sort(_aHistory.begin(), _aHistory.end(), sortByValueInf) ;

//   initDataListe();
  AfficheListe() ;
}

void
NSHistorizedValListWindow::activeControle(int activation, Message* /* pMessage */)
{
  // on ne gère pas le cas BF_CHECKED avant SetupWindow() pour éviter que les nouveaux fils
  // n'activent la liste en cours de création.
  // le cas BF_UNCHECKED provient en principe d'un père du controle de liste
  // on le conserve pour permettre à la liste de se vider si on la désactive.
  //
  if ((NULL == _pVectData) || (false == _bSetupWindow))
    return ;

  if (BF_CHECKED == activation)
  {
    // On recharge les données à partir du pTransfert
    *_pVectData = *(_pControle->getTransfert()->getPatPatho()) ;
    _bSetupWindow = false ;
  }
  if (BF_UNCHECKED == activation)
    // Le controle de liste est désactivé : on vide l'array des données
    _pVectData->vider() ;

  initCurentListe() ;
  AfficheListe() ;
}

void
NSHistorizedValListWindow::CmCreerEnreg()
{
	if (NULL == _pVectData)
		return ;

  // on crée ici un nouveau "fils" au pBBFilsItem lié au controle de liste
  // cela crée en fait un nouveau BBItem dans le VecteurFils de pBBFilsItem
  // puis active le BBItem qui lance l'archetype d'édition d'adresse à l'aide
  // d'un fil guide.
  if (_pControle->getTransfert())
  {
    _pControle->getTransfert()->getFilsItem()->creerNouveauFils() ;

    // on récupère le vecteur de transfert, auquel on vient d'ajouter un fils (une patpatho)
    // si l'utilisateur est sorti par CmCancel(), cette dernière patpatho est vide
    // on doit donc faire l'inverse de creerNouveauFils() : vider la patpatho du transfert
    // et détruire le dernier BBItem du pBBFilsItem père.
    NSVectFatheredPatPathoArray *pVectTransfert    = _pControle->getTransfert()->getPatPatho() ;
    NSVectFatheredPatPathoArray *pTmpVectTransfert = _pControle->getTransfert()->getTmpPatho() ;

    if (false == pVectTransfert->empty())
    {
      FatheredPatPathoIterVect iterVect = pVectTransfert->end() ;
      iterVect-- ;
      if ((*iterVect)->getPatPatho()->empty())
      {
        // on détruit la dernière patpatho ainsi que la dernière patpatho temporaire
        delete (*iterVect) ;
        pVectTransfert->erase(iterVect) ;

        FatheredPatPathoIterVect iterTmpVect = pTmpVectTransfert->end() ;
        iterTmpVect-- ;
        delete (*iterTmpVect) ;
        pTmpVectTransfert->erase(iterTmpVect) ;
      }

      // on vide le vecteur des fils (on delete celui que l'on vient de créer)
      // car ce vecteur doit être vide pour permettre la modification
      _pControle->getTransfert()->getFilsItem()->tuerFils() ;

      // On recharge les données à partir du pTransfert
      *_pVectData = *pVectTransfert ;
      // On rafraichit la liste
      initCurentListe() ;
      AfficheListe() ;
    }
  }
}

void
NSHistorizedValListWindow::CmModifEnreg()
{
	if (NULL == _pVectData)
		return ;

  int index = IndexItemSelect() ;

  if (-1 == index)  {
  	string sText = pContexte->getSuperviseur()->getText("generalLanguageForLists", "anElementMustBeSelected") ;
		erreur(sText.c_str(), warningError, 0) ;
    return ;
  }

  int iSelElem = _aHistory[index]->getPosition() ;

  // on envoie ici un ctrlNotification à l'elt index de pVectData
  // ce qui lance l'infernal processus de modification de l'enregistrement correspondante
  if (_pControle->getTransfert())
  {
    _pControle->getTransfert()->ctrlNotification(BF_UNCHECKED, BF_CHECKED, 0, iSelElem) ;

    // On recharge les données à partir du pTransfert
    *_pVectData = *(_pControle->getTransfert()->getPatPatho()) ;
    // On rafraichit la liste
    initCurentListe() ;
    AfficheListe() ;
  }
}

void
NSHistorizedValListWindow::CmSupprEnreg()
{
  int index = IndexItemSelect() ;
  int k = 0 ;

  if (-1 == index)  {
    erreur("Vous devez sélectionner une enregistrement à supprimer.", standardError, 0) ;
    return ;
  }

  int iSelElem = _aHistory[index]->getPosition() ;
  // on doit supprimer la patpatho n°index du vecteur de transfert
  NSVectFatheredPatPathoArray *pVectTransfert = _pControle->getTransfert()->getPatPatho() ;

  for (FatheredPatPathoIterVect i = pVectTransfert->begin() ; pVectTransfert->end() != i ; i++)
  {
    if (k == iSelElem)
    {
      delete (*i) ;
      pVectTransfert->erase(i) ;
      break ;
    }

    k++ ;
  }

  // On recharge les données à partir du pTransfert
  *_pVectData = *(_pControle->getTransfert()->getPatPatho()) ;
  // On rafraichit la liste
  initCurentListe() ;
  AfficheListe() ;
}

void
NSHistorizedValListWindow::CmHistory()
{
	if (false == _pDialogHistory->_bEnCours)
		_pDialogHistory->Create() ;
}

bool
NSHistorizedValListWindow::canWeClose()
{
  return true ;
}

bool
sortByDateSup(NSHistoryValueManagement *pHVal1, NSHistoryValueManagement *pHVal2)
{
  return (pHVal1->getDate() > pHVal2->getDate()) ;
}

bool
sortByDateInf(NSHistoryValueManagement *pHVal1, NSHistoryValueManagement *pHVal2)
{
  return (pHVal1->getDate() < pHVal2->getDate()) ;
}

bool
sortByLabelSup(NSHistoryValueManagement *pHVal1, NSHistoryValueManagement *pHVal2)
{
  return (pHVal1->getLabel() > pHVal2->getLabel()) ;
}

bool
sortByLabelInf(NSHistoryValueManagement *pHVal1, NSHistoryValueManagement *pHVal2)
{
  return (pHVal1->getLabel() < pHVal2->getLabel()) ;
}

bool
sortByValueSup(NSHistoryValueManagement *pHVal1, NSHistoryValueManagement *pHVal2)
{
  return (pHVal1->getValue() > pHVal2->getValue()) ;
}

bool
sortByValueInf(NSHistoryValueManagement *pHVal1, NSHistoryValueManagement *pHVal2)
{
  return (pHVal1->getValue() < pHVal2->getValue()) ;
}

// -----------------------------------------------------------------
//
//  Méthodes de NSValListHistoryDialog
//
// -----------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSValListHistoryDialog, NSUtilDialog)    EV_COMMAND(IDOK, CmOk),
    EV_COMMAND(IDCANCEL, CmCancel),
    EV_WM_SETFOCUS,
    EV_WM_PARENTNOTIFY,
    EV_WM_CHAR,
    EV_BN_CLICKED(IDC_LISTVALLESS, CmMoreElusive),
    EV_BN_CLICKED(IDC_LISTVALMORE, CmMoreSpecific),
END_RESPONSE_TABLE;

NSValListHistoryDialog::NSValListHistoryDialog(TWindow* pere, NSContexte* pCtx,                                string sItemVal, string sPath,                                string sWantedUnit, string* pInsValue)                       :NSUtilDialog(pere, pCtx, "IDD_VALHISTORY", pNSDLLModule),
                        _ValList(this, pCtx, IDC_LISTVALHISTORY, true, pNSDLLModule)
{
try
{
  _sItemValue  = sItemVal ;
  _sItemPath   = sPath ;

  _sUnit       = sWantedUnit ;
  if (string("") != _sUnit)
    pContexte->getDico()->donneCodeSens(&_sUnit, &_sUnitSens) ;
  else
    _sUnitSens = string("") ;

  _bEnCours    = false ;
  _psNewValue  = pInsValue ;
  if (_psNewValue)
    *_psNewValue = string("") ;

  _sCurrentItem = _sItemValue ;

  if (string("") != _sItemPath)
  {
    // On cherche la première localisation anatomique
    size_t pos      = 0 ;
    size_t iPathLen = strlen(_sItemPath.c_str()) ;
    while ((pos < iPathLen) && (NPOS != pos) && ('A' != _sItemPath[pos]))
    {
      pos = _sItemPath.find(string(1, cheminSeparationMARK), pos + 1) ;
      if (NPOS != pos)
        pos++ ;
    }
    if (pos < iPathLen)
      _sCurrentItem = string(_sItemPath, pos, iPathLen - pos) + string(1, cheminSeparationMARK) + _sItemValue ;
  }

  _ValList.setItemValue(_sCurrentItem) ;

  _pEditPath = new TEdit(this, IDC_LISTVALPATH) ;
  _pBtnMinus = new TButton(this, IDC_LISTVALLESS) ;
  _pBtnPlus  = new TButton(this, IDC_LISTVALMORE) ;
}
catch (...)
{
	erreur("Exception NSValListHistoryDialog ctor.", standardError, 0) ;
}
}

NSValListHistoryDialog::~NSValListHistoryDialog(){
  delete _pEditPath ;
  delete _pBtnMinus ;
  delete _pBtnPlus ;
}

voidNSValListHistoryDialog::SetupWindow()
{
	NSUtilDialog::SetupWindow() ;
	_bEnCours = true ;

	string sLibelle = string("") ;
	string sLang    = pContexte->getUserLanguage() ;

	if (strlen(_sItemValue.c_str()) == BASE_LEXI_LEN)
	{
		pContexte->getDico()->donneLibelle(sLang, &_sItemValue, &sLibelle) ;
		if (string("") != sLibelle)
			sLibelle[0] = pseumaj(sLibelle[0]) ;
	}
	else
		sLibelle = string("Historique des valeurs dans le dossier patient") ;

	SetCaption(sLibelle.c_str()) ;

	updatePathEdit() ;

	// SetControlFocus(pValList->HWindow) ;
  PostMessage(WM_NEXTDLGCTL, (WPARAM) _ValList.HWindow, TRUE) ;
}

void
NSValListHistoryDialog::EvSetFocus(HWND hWndLostFocus)
{
	NSUtilDialog::EvSetFocus(hWndLostFocus) ;
	_ValList.SetFocus() ;
}

void
NSValListHistoryDialog::EvParentNotify(uint event, uint /* childHandleOrX */, uint childIDOrY)
{
	if ((childIDOrY == IDC_LISTVALHISTORY) && (WM_CREATE == event))
  	_ValList.selectFirstItem() ;
}

void
NSValListHistoryDialog::EvChar(uint key, uint repeatCount, uint flags)
{
  if      ('+' == key)
    CmMoreSpecific() ;
  else if ('-' == key)
    CmMoreElusive() ;
  else
    EvChar(key, repeatCount, flags) ;
}

void
NSValListHistoryDialog::updatePathEdit()
{
  if (NULL == _pEditPath)
    return ;

  string sLibelle = string("") ;
  string sLang    = pContexte->getUserLanguage() ;

  string sPathLib = "" ;
  if (string("") != _sCurrentItem)
  {
    size_t pos      = 0 ;
    size_t prevpos  = 0 ;
    size_t iPathLen = strlen(_sCurrentItem.c_str()) ;
    string sThisItem ;
    while ((pos < iPathLen) && (NPOS != pos))
    {
      pos = _sCurrentItem.find(string(1, cheminSeparationMARK), prevpos) ;
      if (pos != NPOS)
      {
        sThisItem = string(_sCurrentItem, prevpos, pos - prevpos) ;
        prevpos = pos + 1 ;
      }
      else
        sThisItem = string(_sCurrentItem, prevpos, iPathLen - prevpos) ;

      pContexte->getDico()->donneLibelle(sLang, &sThisItem, &sLibelle) ;
      if (sPathLib != "")
        sPathLib += string(" / ") ;
      sPathLib += sLibelle ;
    }
  }
  _pEditPath->SetText(sPathLib.c_str()) ;
}

void
NSValListHistoryDialog::CmMoreSpecific()
{
  string sTotalPath ;

  if (string("") != _sItemPath)
    sTotalPath = _sItemPath + string(1, cheminSeparationMARK) + _sItemValue ;
  else
    sTotalPath = _sItemValue ;

  size_t iPathLen = strlen(sTotalPath.c_str()) ;
  size_t iItemLen = strlen(_sCurrentItem.c_str()) ;

  if (iPathLen == iItemLen)
    return ;

  size_t iCurrent = iPathLen - iItemLen ;
  iCurrent-- ;
  if (cheminSeparationMARK != sTotalPath[iCurrent])
    return ;
  if (iCurrent > 0)
    iCurrent-- ;

  while ((iCurrent > 0) && (cheminSeparationMARK != sTotalPath[iCurrent]))
    iCurrent-- ;

  if (0 == iCurrent)
  {
    if (cheminSeparationMARK == sTotalPath[0])
      _sCurrentItem = string(sTotalPath, 1, iPathLen - 1) ;
    else
      _sCurrentItem = sTotalPath ;
  }
  else
    _sCurrentItem = string(sTotalPath, iCurrent + 1, iPathLen - iCurrent - 1) ;

  _ValList.setItemValue(_sCurrentItem) ;
  _ValList.CmReset() ;

  updatePathEdit() ;
}

void
NSValListHistoryDialog::CmMoreElusive()
{
  if (string("") == _sCurrentItem)
    return ;

  size_t pos = _sCurrentItem.find(string(1, cheminSeparationMARK)) ;
  if (NPOS == pos)
    return ;

  size_t iPathLen = strlen(_sCurrentItem.c_str()) ;
  _sCurrentItem = string(_sCurrentItem, pos+1, iPathLen-pos-1) ;

  _ValList.setItemValue(_sCurrentItem) ;
  _ValList.CmReset() ;

  updatePathEdit() ;
}

voidNSValListHistoryDialog::CmOk()
{
  if (_psNewValue)
  {
    NSHistoryValueManagement* pValue = _ValList.getSelectedValue() ;
    if (pValue)
    {
      string sValue   = pValue->getValue() ;
      string sValUnit = pValue->getUnit() ;

      if (_sUnitSens == sValUnit)
        *_psNewValue = sValue ;
    }
  }

  _bEnCours = false ;
  NSUtilDialog::CmOk() ;
}

voidNSValListHistoryDialog::CmCancel()
{
  _bEnCours = false ;
  NSUtilDialog::CmCancel() ;
}

// -----------------------------------------------------------------
//
//  Méthodes de ChoixDansListeDialog
//
// -----------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(ChoixDansListeDialog, NSUtilDialog)	EV_CHILD_NOTIFY_AND_CODE(IDC_LISTBOX, LBN_SELCHANGE, CmSelectTemplate),
	EV_CHILD_NOTIFY_AND_CODE(IDC_LISTBOX, LBN_DBLCLK, CmTemplateDblClk),
END_RESPONSE_TABLE;

ChoixDansListeDialog::ChoixDansListeDialog(TWindow* pere, NSContexte* pCtx, VecteurString* pData, string caption)                    :NSUtilDialog(pere, pCtx, "IDD_HEADER_MODEL", pNSDLLModule)
{
try
{
  pListBox = new OWL::TListBox(this, IDC_LISTBOX) ;
  pVecteurString = pData ;
  sCaption = caption ;

  itemChoisi = -1 ;

  // fichiers d'aide  // sHindex = "hi_doc.htm";
  // sHcorps = "h_compos.htm";
}
catch (...)
{
	erreur("Exception ChoixDansListeDialog ctor.", standardError, 0) ;
}
}

ChoixDansListeDialog::~ChoixDansListeDialog(){
	delete pListBox ;
}

voidChoixDansListeDialog::SetupWindow()
{
try
{
  NSUtilDialog::SetupWindow() ;

  if (string("") != sCaption)
    SetCaption(sCaption.c_str()) ;

  //	// Remplissage de ListBox avec les libelles des fichiers modele
	//
  if ((pVecteurString) && (false == pVecteurString->empty()))
  {
    for (EquiItemIter iter = pVecteurString->begin() ; iter != pVecteurString->end() ; iter++)
      pListBox->AddString((*(*iter)).c_str()) ;    pListBox->SetSelIndex(0) ;    itemChoisi = 0 ;  }  pListBox->SetFocus() ;}catch (...)
{
    erreur("Exception ChoixDansListeDialog::SetupWindow.", standardError, 0) ;
}
}

void ChoixDansListeDialog::CmTemplateDblClk(WPARAM /* Cmd */){
	itemChoisi = pListBox->GetSelIndex() ;
	CmOk() ;
}

void ChoixDansListeDialog::CmSelectTemplate(WPARAM /* Cmd */){
	//
	// Récupération de l'indice du template sélectionné
	//

	itemChoisi = pListBox->GetSelIndex() ;}

void ChoixDansListeDialog::CmCancel(){
	itemChoisi = -1 ;
	NSUtilDialog::CmCancel() ;
}

void ChoixDansListeDialog::CmOk()
{
	NSUtilDialog::CmOk() ;
}

//***********************************************************************//
//							Classe NSEnregTreeDocDlg
//***********************************************************************//

DEFINE_RESPONSE_TABLE1(NSEnregTreeDocDlg, NSUtilDialog)END_RESPONSE_TABLE;

NSEnregTreeDocDlg::NSEnregTreeDocDlg(TWindow* pere, NSContexte* pCtx)
                    :NSUtilDialog(pere, pCtx, "IDD_ENREG_TREEDOC", pNSDLLModule)
{
    pTexte = new TStatic(this, IDC_ENREGDOC_TEXT);
}

NSEnregTreeDocDlg::~NSEnregTreeDocDlg(){    delete pTexte;}voidNSEnregTreeDocDlg::SetupWindow(){    NSUtilDialog::SetupWindow();    // centrage du dialogue à l'écran    NS_CLASSLIB::TRect rectDlg = GetWindowRect();//coordonnées par % à l'écran    NS_CLASSLIB::TRect rectMain = Parent->GetWindowRect();    int     X,Y,W,H;
    //
    // ScreenToClient converts the screen coordinates
    // of a specified point on the screen to client coordinates.
    //    NS_CLASSLIB::TPoint point(rectDlg.X(), rectDlg.Y());
    Parent->ScreenToClient(point);
    X = point.X();
    Y = point.Y();
    W = rectDlg.Width();
    H = rectDlg.Height();    X = (rectMain.Width())/2 - W/2;    Y = (rectMain.Height())/2 - H/2;    //    // fixer la nouvelle position
    //
    SetWindowPos(0, X, Y, W, H, SWP_NOZORDER);}void
NSEnregTreeDocDlg::SetText(string sCodeDocRoot)
{
  // Le code root doit être un code lexique
  string sLibelle ;
  string sLangue = pContexte->getUserLanguage() ;
  pContexte->getDico()->donneLibelle(sLangue, &sCodeDocRoot, &sLibelle) ;
  pTexte->SetText(sLibelle.c_str()) ;
}

// fin de nslistWind.cpp
///////////////////////////////////////////////////

