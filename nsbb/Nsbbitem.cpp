// -----------------------------------------------------------------------------// Nsbbitem.cpp
// -----------------------------------------------------------------------------
// Implémentation de toutes les classes nécessaires au fonctionnement de BIG-BROTHER
// -----------------------------------------------------------------------------
// $Revision: 1.25 $
// $Author: pameline $
// $Date: 2016/12/29 16:21:23 $
// -----------------------------------------------------------------------------
// PK  - 04/1993
// PA  - 02/1995
// HK  - 03/1997
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

#include <owl\olemdifr.h>
#include <owl\applicat.h>
#include <owl\checkbox.h>

#include "partage\nsdivfct.h"
#include "nautilus\nssuper.h"

#include "nsbb\nsbb_msg.h"

#include "partage\ns_vector.h"
#include "nsbb\nsbb.h"
#include "nsbb\nsbbsmal.h"
#include "nsbb\nsbbitem.h"
#include "nsbb\nsbbtran.h"
#include "nsbb\nsbouton.h"
#include "nsbb\nsarc.h"#include "nsbb\nsarcCtrls.h"#include "nsbb\nstabwindow.h"#include "nsbb\ns_multi.h"#include "nsbb\nsrcdlg.h"#include "nssavoir\nsgraphe.h"#include "nssavoir\nsguide.h"#include "parseur\balise.h"#include <owl\window.h>
#include "nsbb\nsattval.h"

long BBFilsItem::lObjectCount = 0 ;
long BBFilsArray::lObjectCount = 0 ;
long BBItem::lObjectCount = 0 ;

// -----------------------------------------------------------------------------
// Implémentation de VectorBBItem
// -----------------------------------------------------------------------------
VectorBBItem::VectorBBItem(const VectorBBItem& rv)
             :BBFilsVecteur()
{
try
{
  if (false == rv.empty())
    for (BBiterFilsConst it = rv.begin() ; rv.end() != it ; it++)
      push_back(new BBItem(**it)) ;
}
catch (...)
{
  erreur("Exception VectorBBItem copy ctor.", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------// Vidange de l'array// -----------------------------------------------------------------------------
void
VectorBBItem::vider()
{
  if (false == empty())
    for (BBiterFils it = begin() ; end() != it ; )
    {
      delete *it ;
      erase(it) ;
    }
}

VectorBBItem::~VectorBBItem()
{
	vider() ;
}

BBItem*
VectorBBItem::addBBItem(const BBItem& item)
{
  BBItem* pNewItem = new BBItem(item) ;
  push_back(pNewItem) ;
  return pNewItem ;
}

VectorBBItem&
VectorBBItem::operator=(const VectorBBItem& src)
{
  if (this == &src)
  	return *this ;

  vider() ;

	if (false == src.empty())
    for (BBiterFilsConst it = src.begin() ; src.end() != it ; it++)
      push_back(new BBItem(**it)) ;

	return (*this) ;
}

int
VectorBBItem::operator==(const VectorBBItem& o) const
{
  return 0 ;
}

// -----------------------------------------------------------------------------
// Implémentation de BBFilsExclu
// -----------------------------------------------------------------------------

BBFilsExclu::BBFilsExclu()
{
}

BBFilsExclu::BBFilsExclu(const BBFilsExclu& src)
{
	if (false == src._aExclusion.empty())
  {
    vector<string>::const_iterator stringIter = src._aExclusion.begin() ;
    for ( ; src._aExclusion.end() != stringIter ; stringIter++)
      addExclusion(*stringIter) ;
  }
}

BBFilsExclu&
BBFilsExclu::operator=(const BBFilsExclu& src)
{
	if (this == &src)
  	return *this ;

  vider() ;

  if (false == src._aExclusion.empty())
  {
    vector<string>::const_iterator stringIter = src._aExclusion.begin() ;
    for ( ; src._aExclusion.end() != stringIter ; stringIter++)
      addExclusion(*stringIter) ;
  }

	return (*this) ;
}

int
BBFilsExclu::operator==(const BBFilsExclu& o) const
{
  if (_aExclusion.empty())
  {
    if (o._aExclusion.empty())
      return 1 ;
    return 0 ;
  }

  if (o._aExclusion.empty())
    return 0 ;

	vector<string>::const_iterator stringIter = _aExclusion.begin() ;
  for ( ; _aExclusion.end() != stringIter ; stringIter++)
    if (false == o.isExclusion(*stringIter))
      return 0 ;

  return 1 ;
}

bool
BBFilsExclu::isExclusion(string sExclu) const
{
  if (_aExclusion.empty())
    return false ;

  vector<string>::const_iterator stringIter = _aExclusion.begin() ;
  for ( ; _aExclusion.end() != stringIter ; stringIter++)
    if (*stringIter == sExclu)
      return true ;

  return false ;
}

void
BBFilsExclu::addExclusion(string sExclu)
{
  // First check if it is not already there
  //
  if (isExclusion(sExclu))
    return ;

  _aExclusion.push_back(sExclu) ;
}

void
BBFilsExclu::removeExclusion(string sExclu)
{
  if (_aExclusion.empty())
    return ;

  vector<string>::iterator stringIter = _aExclusion.begin() ;
  for ( ; _aExclusion.end() != stringIter ; )
  {
    if (*stringIter == sExclu)
    {
      _aExclusion.erase(stringIter) ;
      return ;  // We assume here that unicity is garanteed by addExclusion
    }
    else
      stringIter++ ;
  }
}

void
BBFilsExclu::operateExclusion(char chOperator, string sExclu)
{
  switch(chOperator)
  {
    case '-' : addExclusion(sExclu) ;    break ;
    case '+' : removeExclusion(sExclu) ; break ;
  }
}

string
BBFilsExclu::getFirstExcluded() const
{
  if (_aExclusion.empty())
    return string("") ;

  return *(_aExclusion.begin()) ;
}

string
BBFilsExclu::getNextExcluded(string sExclus) const
{
  if (_aExclusion.empty())
    return string("") ;

  vector<string>::const_iterator stringIter = _aExclusion.begin() ;
  for ( ; _aExclusion.end() != stringIter ; stringIter++)
    if (*stringIter == sExclus)
      break ;

  if (_aExclusion.end() == stringIter)
    return string("") ;

  stringIter++ ;

  if (_aExclusion.end() == stringIter)
    return string("") ;

  return *stringIter ;
}

void
BBFilsExclu::vider()
{
  if (_aExclusion.empty())
    return ;

  vector<string>::iterator stringIter = _aExclusion.begin() ;
  for ( ; _aExclusion.end() != stringIter ; )
    _aExclusion.erase(stringIter) ;
}

// -----------------------------------------------------------------------------
// Implémentation de BBExcluArray
// -----------------------------------------------------------------------------
BBExcluArray::BBExcluArray(BBExcluArray& rv)
             :BBFilsExcluArray()
{
try
{
  if (false == rv.empty())
    for (BBExIter it = rv.begin() ; rv.end() != it ; it++)
      push_back(*it) ;
}
catch (...)
{
  erreur("Exception BBExcluArray copy ctor.", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------
// Vidange de l'array
// -----------------------------------------------------------------------------
void
BBExcluArray::vider()
{
  if (false == empty())
    for (BBExIter it = begin() ; end() != it ; )
      erase(it) ;
}

BBExcluArray::~BBExcluArray()
{
	vider() ;
}

// -----------------------------------------------------------------------------
//
// Implémentation de BBFilsItem
//
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Function     : inline BBFilsItem::BBFilsItem(string loc, string Avant, string Apres)
// Arguments    : loc  		      -> Localisation de l'Item au sein de l'arbre
//                Avant, Apres  -> Préfixe/Suffixe à ajouter au codon de l'Item
// Description  : Constructeur par défaut
// -----------------------------------------------------------------------------
inline
BBFilsItem::BBFilsItem(NSContexte* pCtx, BBItem* pPereItem, NSSmallBrother* pBig, string Avant, string Apres)
           :NSRoot(pCtx), pBigBoss(pBig), _pPere(pPereItem), _Transfert(this)
{
try
{
  _sNom             = string("") ;
  _sEtiquette       = string("") ;
  estActif          = false ;
  DialogueMultiple  = false ;
  FilsProlongeable  = false ;
  bCorriger         = false ;
  pNsMultiDialog    = (NsMultiDialog*) 0 ;
  pCitem            = (Citem*) 0 ;  lObjectCount++ ;}
catch (...)
{
  erreur("Exception constructeur BBFilsItem.", standardError, 0) ;
}
}

BBFilsItem::~BBFilsItem(){
  lObjectCount-- ;

  VectorFils.vider() ;

  // Note : on ne delete pas le pCitem
  // car est détruit par BBItem dans le delete du parseur  Exclusion.vider() ;
}

// -----------------------------------------------------------------------------// Constructeur copie// -----------------------------------------------------------------------------
BBFilsItem::BBFilsItem(const BBFilsItem& src)
           :NSRoot(src.pContexte), _Data(src._Data), _Transfert(src._Transfert)
{
try
{
  pBigBoss          = src.pBigBoss ;
  _pPere            = src._pPere ;
  FilsProlongeable  = src.FilsProlongeable ;
  pNsMultiDialog    = src.pNsMultiDialog ;
  pCitem            = src.pCitem ;  DialogueMultiple  = src.DialogueMultiple ;
  estActif          = src.estActif ;
  VectorFils        = src.VectorFils ;
  bCorriger         = src.bCorriger ;
	_sEtiquette 			= src._sEtiquette ;
	_sNom             = src._sNom ;

  _Transfert.setFilsItem(this) ;

  lObjectCount++ ;
}
catch (...)
{
  erreur("Exception constructeur copie BBFilsItem.", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------
// Opérateur d'affectation
// -----------------------------------------------------------------------------
inline BBFilsItem&
BBFilsItem::operator=(const BBFilsItem& src)
{
	if (this == &src)
  	return *this ;

try
{
  pBigBoss         = src.pBigBoss ;
  _pPere		       = src._pPere ;
  estActif         = src.estActif ;
  DialogueMultiple = src.DialogueMultiple ;
  bCorriger		     = src.bCorriger ;
  // Réallocation de pFils et pTransfert
  pNsMultiDialog   = src.pNsMultiDialog ;
  pCitem           = src.pCitem ;
  _Transfert       = src._Transfert ;
  _Data            = src._Data ;
	_sNom            = src._sNom ;
	_sEtiquette      = src._sEtiquette ;

	return (*this) ;
}
catch (...)
{
  erreur("Exception BBFilsItem::operator=.", standardError, 0) ;
  return (*this) ;
}
}

// -----------------------------------------------------------------------------// Opérateur de comparaison// -----------------------------------------------------------------------------
bool
BBFilsItem::operator==(const BBFilsItem& o) const
{
	if ((_sNom                  == o._sNom) 			      &&
      (_sEtiquette 	          == o._sEtiquette) 		  &&
      // (VectorFils 	          == o.VectorFils)		    &&
      (_pPere 			          == o._pPere) 			      &&
      (_Data		              == o._Data)     	 	    &&
      (DialogueMultiple       == o.DialogueMultiple)  &&
      (pNsMultiDialog	        == o.pNsMultiDialog)    &&
      (pCitem                 == o.pCitem)            &&      (estActif   	          == o.estActif)          &&
      (bCorriger		          == o.bCorriger)		      &&
      (_Transfert 	          == o._Transfert))
		return true ;

  return false ;
}

bool
BBFilsItem::operator!=(const BBFilsItem& o) const
{
  return false == (o == *this) ;
}

// -----------------------------------------------------------------------------
// Function     : void BBFilsItem::creerFils()
// Description  : Pour chaque fils "actif" (qui correspond à une étiquette),
//					      on cherche la fiche Paradox correspondante et, si elle
//					      intéresse la même fenêtre, on crée une BBItem
// -----------------------------------------------------------------------------
void
BBFilsItem::creerFils(int indexFils)
{
try
{
  NSVectFatheredPatPathoArray* pVect = getPatPatho() ;
  // NSVectFatheredPatPathoArray* pVectTmp = getTmpPatho() ;

  NSSuper* _pSuper = pContexte->getSuperviseur() ;

  if (pVect->empty())
    pVect->push_back(new NSFatheredPatPathoArray(_pSuper, (NSPatPathoInfo*) 0, new NSPatPathoArray(_pSuper))) ; // mettre une patpatho vide
  // if (pVectTmp->empty())
  //   pVectTmp->push_back(new NSFatheredPatPathoArray(_pSuper, (NSPatPathoInfo*) 0, new NSPatPathoArray(_pSuper))) ; // mettre une patpatho vide

  // ici deux cas :
  // soit indexFils == -1 et on crée un fils pour chaque entrée de pVect
  // soit indexFils >= 0 et on ne crée que le fils correspondant à l'entrée n°indexFils de pVect
  // Important : on passe au nouveau BBItem un pointeur sur une patpatho du pTransfert, donc :
  // - chaque fils sera ultérieurement créé et activé par la fonction BBItem::developper, et :
  // - pour chaque fils, le OkFermerDialogue remettra directement à jour ce pointeur dans le pTransfert initial
  //   (en fait, on indique au départ la patpatho à mettre à jour par le dialogue associé au BBItem)

  int index = 0 ;
  FatheredPatPathoIterVect iJ = pVect->begin() ;
  // FatheredPatPathoIterVect iJTmp = pVectTmp->begin() ;
  for ( ; pVect->end() != iJ ; iJ++, /*iJTmp++,*/ index++)
  {
    if ((indexFils < 0) || (index == indexFils))
    {
/*
      if (pVectTmp->end() == iJTmp)
      {
        pVectTmp->push_back(new NSFatheredPatPathoArray(_pSuper, (NSPatPathoInfo*) 0, new NSPatPathoArray(_pSuper))) ; // mettre une patpatho vide
        iJTmp = pVectTmp->end() ;
        iJTmp-- ;
      }
*/

      // créer un nouveau BBItem
      //
      BBItem NewItem(pContexte, this, pBigBoss, (*iJ)->getPatPatho() /*, (*iJTmp)->getPatPatho()*/) ;
      VectorFils.addBBItem(NewItem) ;
    }
  }

  if (false == VectorFils.empty())
    for (BBiterFils it = VectorFils.begin() ; VectorFils.end() != it ; it++)
      (*it)->setItemData(&_Data) ;
}
catch (...)
{
  erreur("Exception BBFilsItem::creerFils.", standardError, 0) ;
}
}

boolBBFilsItem::actifVide()
{
  return _Data.actifVide() ;
}

void
BBFilsItem::Desactive()
{
  _Transfert.Desactive() ;
  DesactiveFils() ;

  if (_pPere)
    _pPere->DesactivePereFictif(this) ;

  // si this est champ edit non actif, et s'il est fils unique de son père,
  // alors désactiver ce père s'il est non actif vide
  /*
  if ((!Actif()) && (pPere->aBBItemFils.size() == 1) && (!pPere->actifVide()))
    pPere->pBBFilsPere->pTransfert->Desactive() ;
  */
}

void
BBFilsItem::Active()
{
  _Transfert.Active() ;
  if (_pPere)
    _pPere->ActivePere(this) ;
}

// -----------------------------------------------------------------------------// Function: void BBFilsItem::CreerFilsManuel(...)// le BBfilsItem se crée en tant que BBitem
// -----------------------------------------------------------------------------
void
BBFilsItem::CreerFilsManuel(NSControle* pControle, NSControle* pControlePetitFrere/*, BBItemData* pDonnees*/)
{
try
{
  if (false == VectorFils.empty())
  {
    BBiterFils itFirstSon = VectorFils.begin() ;
    (*itFirstSon)->CreerFilsManuel(pControle, pControlePetitFrere) ;
    return ;
  }

  // Il n'y a pas de BBItem fils
  //
  NSVectFatheredPatPathoArray* pVect    = getPatPatho() ;
  NSVectFatheredPatPathoArray* pVectTmp = getTmpPatho() ;

  NSSuper* _pSuper = pContexte->getSuperviseur() ;

  if (pVect && pVect->empty())
    pVect->push_back(new NSFatheredPatPathoArray(_pSuper, 0, new NSPatPathoArray(_pSuper))) ; // mettre une patpatho vide
  if (pVectTmp && pVectTmp->empty())
    pVectTmp->push_back(new NSFatheredPatPathoArray(_pSuper, 0, new NSPatPathoArray(_pSuper))) ; // mettre une patpatho vide

  BBItem bbItem(pContexte, this, pBigBoss, (*(pVect->begin()))->getPatPatho() /*, (*(pVectTmp->begin()))->getPatPatho()*/) ;
  bbItem._sLocalisation = getLocalisation() ;
  bbItem._iProfondeur   = _pPere->_iProfondeur + 1 ;

  BBItem* pItem = VectorFils.addBBItem(bbItem) ; // créer un nouveau BBItem
  pItem->CreerFilsManuel(pControle, pControlePetitFrere) ;
}
catch (...)
{
  erreur("Exception BBFilsItem::CreerFilsManuel.", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void
BBFilsItem::DestructionManuelle(Message* pMessage)
{
  if (false == VectorFils.empty())
    for (BBiterFils i = VectorFils.begin() ; VectorFils.end() != i ; i++)
      (*i)->DestructionManuelle(pMessage) ;

  tuerFils() ;

  _Transfert.activeControle(BF_DELETE, pMessage) ;
}

// -----------------------------------------------------------------------------// vider VectorFils// -----------------------------------------------------------------------------
void
BBFilsItem::tuerFils()
{
  VectorFils.vider() ;
}

// -----------------------------------------------------------------------------
// lancer les boîtes de dialogue multiples
// -----------------------------------------------------------------------------
void
BBFilsItem::lancerMultiDialogue()
{
try
{
  if (pNsMultiDialog)
    return ;
	pNsMultiDialog = (NsMultiDialog*) 0 ;  if (_Transfert.getControl() && (_Transfert.getControl()->getGestionMultiple()))    return ;  if (VectorFils.empty())  	return ;	BBItem* PremierBBitem = *(VectorFils.begin()) ;
  if (false == PremierBBitem->uniciteLesion())
  {
  	pNsMultiDialog = new NsMultiDialog(PremierBBitem->donneFenetre(), "MULTI", this, pNSDLLModule) ;
    DialogueMultiple = true ;
  }
}
catch (...)
{
  erreur("Exception BBFilsItem::lancerMultiDialogue.", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------
// Retourne vrai si le BBFilsItem ouvre un dialogue (ou archétype)
// -----------------------------------------------------------------------------
bool
BBFilsItem::ouvreDialog(){  return ((_Data.ouvreDlg()) || (_Data.ouvreArchetype())) ;}boolBBFilsItem::isActif(){  return (1 == _Transfert.getActif()) ;}boolBBFilsItem::isAvailable(){  if ((false == isActif()) || (false == _Data.uniciteLesion()))    return true ;  return true ;}NSVectFatheredPatPathoArray*BBFilsItem::getPatPatho(){  return _Transfert.getPatPatho() ;}NSVectFatheredPatPathoArray*BBFilsItem::getTmpPatho(){  return _Transfert.getTmpPatho() ;}Message*BBFilsItem::getItemTransfertMsg(){  return _Transfert.getTransfertMessage() ;}
// -----------------------------------------------------------------------------// -----------------------------------------------------------------------------voidBBFilsItem::creerNouveauFils()
{
try
{
  NSSuper* _pSuper = pContexte->getSuperviseur() ;

  NSPatPathoArray* pPath = new NSPatPathoArray(_pSuper) ; //créer une patpatho vide
  getPatPatho()->push_back(new NSFatheredPatPathoArray(_pSuper, 0, pPath)) ;
  NSPatPathoArray* pPathTmp = new NSPatPathoArray(_pSuper) ; //créer une patpatho vide
  getTmpPatho()->push_back(new NSFatheredPatPathoArray(_pSuper, 0, pPathTmp)) ;

  BBItem NewFils(pContexte, this, pBigBoss, pPath /*, pPathTmp*/) ; //créer un nouveau BBItem
  NewFils._iProfondeur = _pPere->_iProfondeur + 1 ;
  BBItem* pNewItem = VectorFils.addBBItem(NewFils) ;

  pNewItem->setItemData(&_Data) ;

  _pPere->donneGenetique(pNewItem, _sEtiquette) ;

  // On demande au BBItem de se créer (et de créer ses fils qui correspondent à la même boîte de dialogue)
  int i = pNewItem->creer() ;
  if (0 != i)
    return ;

  // On demande au BBItem de s'activer
  /* int iRetActiv = */ pNewItem->activer() ;

  if (pNewItem->_iProfondeur < pBigBoss->getSeuilSauve())
    pBigBoss->sauvegarde() ;
}
catch (...)
{
  erreur("Exception BBFilsItem::creerNouveauFils.", standardError, 0) ;
}
}

bool
BBFilsItem::estPlusZero()
{
  return (_Data.getLevelShift() == string("+00+00")) ;
}
// -----------------------------------------------------------------------------// Fonction appelée par le BBItem père qui prévient que la boite de dialogue// demande l'autorisation de se fermer.
// Demande à la structure de transfert de se mettre à jour.
// Retransmet la demande aux éventuels BBItem de la branche qui concernent
// la même boite de dialogue.
// -----------------------------------------------------------------------------
// Arguments  : bDetacheControle : autorise à détacher le controle
// Returns    : true si la boite de dialogue a l'autorisation de se fermer, false sinon
// -----------------------------------------------------------------------------
bool
BBFilsItem::okFermerDialogue(bool rapatrier, bool bDetacheControle)
{
try
{
	// Demande à l'élément de transfert de se mettre à jour, uniquement si
	// l'élément n'est pas prolongeable
  if (false == FilsProlongeable)
	{
    if (0 == _Transfert.Transfer(tdGetData))
      return false ;

    // Utilisé pour le transfert de document type consultation pour la mise à jour de la base des textes libres
    if (0 == _Transfert.TransferFinal(tdGetData))
      return false ;
	}

	// Lance la fonction récursivement sur l'éventuel BBItem fils
  if (false == VectorFils.empty())
  {
    for (BBiterFils it = VectorFils.begin() ; VectorFils.end() != it ; it++)
    {
      bool bFerme = (*it)->okFermerDialogue(rapatrier, bDetacheControle) ;
      if (false == PatPtahovide())
        Active() ;
      if (false == bFerme)
      {
        if (bDetacheControle)
          _Transfert.detacheControle() ;
        return bFerme ;
      }
    }
    // Ajout 23/04/2004 PA
    if (PatPtahovide() && (false == actifVide()))
      Desactive() ;
  }

	// Détache l'élément de transfert du contrôle Windows associé
  // Il ne faut pas le faire avant l'appel à okFermerDialogue des fils,
  // sinon l'élément est considéré comme un pere fictif (et il risque
  // d'être désactivé s'il est dans le même dialogue que ses fils et
  // que ses fils sont vides - même s'il est lui même actif vide)
  // --
  // We disconnect the transfert element from the GUI control
  // We must do it after that okFermerDialogue has been called on its sons
  // elsewhere this element will be considered as a fake father (pere fictif)
  // and there is a risk it can be switched off if it is in the same dialog
  // box as its sons and the sons are off - even if this element is "on even
  // if empty" (actif vide)
  //
  if (bDetacheControle)
    _Transfert.detacheControle() ;

	return true ;
}
catch (...)
{
  erreur("Exception BBFilsItem::okFermerDialogue.", standardError, 0) ;
  return false ;
}
}

void
BBFilsItem::detacheControle()
{
  // Lance la fonction récursivement sur l'éventuel BBItem fils
  if (false == VectorFils.empty())
    for (BBiterFils it = VectorFils.begin() ; VectorFils.end() != it ; it++)
      (*it)->detacheControle() ;

  _Transfert.detacheControle() ;
}

void
BBFilsItem::rapatrieTmpPpt(NSPatPathoArray *pTempPatPatho, size_t iLevel)
{
try
{
	// Demande à l'élément de transfert de se mettre à jour, uniquement si
	// l'élément n'est pas prolongeable
  if (false == FilsProlongeable)
	{
    if (0 == _Transfert.TempTransfer())
      return ;
	}

	// Lance la fonction récursivement sur l'éventuel BBItem fils
  if (false == VectorFils.empty())
    for (BBiterFils it = VectorFils.begin() ; VectorFils.end() != it ; it++)
      (*it)->rapatrieTmpPpt(pTempPatPatho, iLevel, false) ;
}
catch (...)
{
  erreur("Exception BBFilsItem::rapatrieTmpPpt.", standardError, 0) ;
  return ;
}
}

// -----------------------------------------------------------------------------// Function     : int BBFilsItem::Initialiser(string sCodon, int iActif)// Description  : Initialise les données du niveau, initialise l'éventuel BBItem qui y est rattaché.
// Arguments    : sCodon  -> Codon à ce niveau
//        			  iActif  -> Mode Actif/Non actif à ce niveau
// Returns      : 0 si tout a bien fonctionné, 1 si on a eu un problème
// -----------------------------------------------------------------------------
int
BBFilsItem::Initialiser(string /* sCodon */, CTRL_ACTIVITY iActif)
{
try
{
	// Initialisation des variables locales
	_Transfert.setActif(iActif) ;

	// Initialisation de l'éventuel BBItem rattaché
  if (VectorFils.empty())
		return 0 ;

	for (BBiterFils it = VectorFils.begin() ; VectorFils.end() != it ; it++)
    return (*it)->Initialiser() ;

  return 0 ;
}
catch (...)
{
  erreur("Exception BBFilsItem::Initialiser.", standardError, 0) ;
  return 0 ;
}
}

// -----------------------------------------------------------------------------// Transférer la valeur sVal et activer le contrôle piloté par this// -----------------------------------------------------------------------------
void
BBFilsItem::ActiveControle(string sVal)
{
	if ((Message*) NULL == getItemTransfertMsg())
		return ;

  if (string("£CL") == string(_sEtiquette, 0, 3))
  	getItemTransfertMsg()->SetTexteLibre(sVal) ;
  else
  	getItemTransfertMsg()->SetComplement(sVal) ;

  // pTransfert->pTmpTransfertMessage->SetComplement(sVal) ;
  _Transfert.activeControle(BF_CHECKED, getItemTransfertMsg()) ;
  _Transfert.Transfer(tdGetData) ;

  if (_Transfert.getFilsItem())
    _Transfert.getFilsItem()->Active() ;
}

// -----------------------------------------------------------------------------
//activer le bouton piloté par this
// -----------------------------------------------------------------------------
void
BBFilsItem::ActiveBouton(string /* sEtiquette */)
{
  if ((NSControle*) NULL == _Transfert.getControl())
    return ;

  NSButton* pNSButton = static_cast<NSButton *>(_Transfert.getControl()->getControle()) ;
  if (pNSButton)
    pNSButton->BNClicked() ;
}

// -----------------------------------------------------------------------------
// Transférer la valeur sVal et activer le contrôle piloté par this
// -----------------------------------------------------------------------------
void
BBFilsItem::DesactiveControle()
{
  if (getItemTransfertMsg())
  {
    getItemTransfertMsg()->SetComplement("") ;
    // if (pTransfert->pTmpTransfertMessage)
    //    pTransfert->pTmpTransfertMessage->SetComplement("") ;
    _Transfert.activeControle(BF_UNCHECKED, getItemTransfertMsg()) ;
  }
  _Transfert.Transfer(tdGetData) ;

  if (_Transfert.getFilsItem())
    _Transfert.getFilsItem()->Desactive() ;
}

// -----------------------------------------------------------------------------// voir si le vecteur des patpatho est vide ou non dans les cas des dialogues uniques// et les dialogues multiples
// -----------------------------------------------------------------------------
bool
BBFilsItem::PatPtahovide()
{
  NSVectFatheredPatPathoArray* pPpt = getPatPatho() ;
  if ((NSVectFatheredPatPathoArray*) NULL == pPpt)
    return true ;

  if (DialogueMultiple)
    return (pPpt->MultiEstVide()) ;

  return pPpt->estVide() ;
}

// -----------------------------------------------------------------------------
// voir si le vecteur des patpatho est vide ou non dans les cas des dialogues uniques
// et les dialogues multiples
// -----------------------------------------------------------------------------
bool
BBFilsItem::TmpPptvide()
{
  NSVectFatheredPatPathoArray* pPpt = getTmpPatho() ;
  if ((NSVectFatheredPatPathoArray*) NULL == pPpt)
    return true ;

  if (DialogueMultiple)
    return (pPpt->MultiEstVide()) ;

  return pPpt->estVide() ;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void
BBFilsItem::DesactiveFils()
{
  if (VectorFils.empty())
    return ;

  // Premier cas : il ne s'agit pas d'un Archetype
  if ((Citem*) NULL == pCitem)
  {
    // Modif RS 03/11/03 : on regarde à quelle fenetre est lié
    // le BBItem père et on vérifie si les BBItem fils appartiennent
    // à la même fenêtre.
    TWindow* pWindowPere = (TWindow *) _pPere->getDialog() ;
    if ((TWindow*) NULL == pWindowPere)
      pWindowPere = _pPere->donneFenetre() ;

    if (((TWindow*) NULL == pWindowPere) || (!pWindowPere->IsWindow()))
      return ;

    BBiterFils iter ;

    /*
    // ancienne méthode
    for (iter = VectorFils.begin() ; iter != VectorFils.end() ; iter++)
      if ( strcmp(pPere->szNomDlg, (*iter)->szNomDlg) == 0 )
        (*iter)->DesactiveFils() ;
    */

    for (iter = VectorFils.begin() ; VectorFils.end() != iter ; iter++)
    {
      // Si le BBItem est ouvreur de boite de dialogue,
      // on ne désactive pas ses fils.
      // Sinon, on vérifie à chaque niveau si les fils sont dans
      // la même fenêtre que leur pBBFIlsItem père.
      if ((*iter)->lienDialogue())
      {
        TWindow* pWindowFils = (TWindow *) (*iter)->getDialog() ;
        if ((TWindow*) NULL == pWindowFils)
          pWindowFils = (*iter)->donneFenetre() ;

        if ((pWindowFils) && (pWindowFils == pWindowPere))
          (*iter)->DesactiveFils() ;
      }
    }
  }
  else
  {
    // Dans un Archétype
    BBiterFils iter ;
    for (iter = VectorFils.begin() ; VectorFils.end() != iter ; iter++)
    {
      // Pour les Archetypes, lienDialogue signifie une ouverture de fenêtre
      // if (!((*iter)->lienDialogue()))
      if (_pPere->dansMemeDialogue(*iter))
        (*iter)->DesactiveFils() ;
    }
  }
}

// -----------------------------------------------------------------------------// Efface toutes les données pour permettre un redispatche// -----------------------------------------------------------------------------
void
BBFilsItem::RemetAZero()
{
  if (VectorFils.empty())
    return ;

  for (BBiterFils iter = VectorFils.begin() ; VectorFils.end() != iter ; iter++)
    if (_pPere->dansMemeDialogue(*iter))
      (*iter)->RemetAZero() ;
}

void
BBFilsItem::Redeveloppe()
{
  if (pNsMultiDialog)
    pNsMultiDialog->CmCancel() ;
  else
  {
    if (false == VectorFils.empty())
    {
      BBItem* pItem = *(VectorFils.begin()) ;
      if (pItem)
      {
        NSDialog* pDlg = pItem->getDialog() ;
        if (pDlg)
          pDlg->Destroy() ;
      }
    }
  }

  _pPere->developper(this) ;
}

void
BBFilsItem::Redispatche()
{
  if (VectorFils.empty())
    return ;

  for (BBiterFils iter = VectorFils.begin() ; VectorFils.end() != iter ; iter++)
    if (_pPere->dansMemeDialogue(*iter))
      (*iter)->Redispatche() ;
}

// -----------------------------------------------------------------------------
// mettre à jour la patptho en cours
// pere : itérateur sur l élément patpatho (dans PatpathoActuelle)
// correspondant au BBItem pere de ce BBFilsItem
// -----------------------------------------------------------------------------
void
BBFilsItem::RemplirPatpatho(NSPatPathoArray* /*pPatpathoActuelle*/, PatPathoIter /*pere*/)
{
}

// -----------------------------------------------------------------------------
// In order that miscellanous Fils Guides don't get stuck to dialog controls
// that should be leaves, we have to destroy all BBItems that don't open a
// dialog or have sons connected to controls.
// If the control is a TreeView, it can do what it wants.
// -----------------------------------------------------------------------------
void
BBFilsItem::cutBadDialogBranches(NSDialog* pDlg)
{
	if ((NSDialog*) NULL == pDlg)
		return ;

	// Only leaves must cut bad branches. If we are not on an Item that is
  // connected to a control, we just iterate
  //
	if ((NSControle*) NULL == _Transfert.getControl())
  {
  	if (false == VectorFils.empty())
  		for (BBiterFils it = VectorFils.begin() ; VectorFils.end() != it ; it++)
    		(*it)->cutBadDialogBranches(pDlg) ;

    return ;
  }

  //
  //
  NSDialog* pCrlDlg = _Transfert.getControl()->getNSDialog() ;
  if (pCrlDlg != pDlg)
		return ;

  // If we are there, we are supposed to be connected to a control
  //
  // TreeWindows can do what they want
  //
	WNDTYPE iCtrlType = _Transfert.getControl()->getType() ;
  if (isTreeWindow == iCtrlType)
		return ;

	// If it is a leaf and it is "expandable", check that the Fil Guide is a
  // "Windows opening" one
  //
  if (VectorFils.empty())
  {
  	if (FilsProlongeable)
    {
    	// Sometimes, the Fil Guide that is connected to the control doesn't
      // open a dialog because its sons do it. In order not to cut this
      // good branch, we have no other way than to test if its Dialog Name is
      // __IDEM ; meaning that is has been built to get connected to a dialog
      //
    	if ((false == _Data.ouvreDlg()) &&
                (string("__CROS") != _Data.getDialogName()))
      {
      	FilsProlongeable = false ;
        _Data.metAZero() ;
      }
    }
    return ;
  }

  // Other controls must kill their sons if they have not themselves a parent
  // connected to the same dialog
  //
  for (BBiterFils it = VectorFils.begin() ; VectorFils.end() != it ; )
  {
  	if (false == (*it)->isBadDialogBranch(pDlg))
    {
    	(*it)->cutBadDialogBranches(pDlg) ;
      it++ ;
    }
    else
    {
    	Message Msg ;
    	(*it)->DestructionManuelle(&Msg) ;
      VectorFils.erase(it) ;
    }
  }
}

bool
BBFilsItem::isBadDialogBranch(NSDialog* pDlg)
{
	if ((NSDialog*) NULL == pDlg)
		return false ;

	// No sons : means it is either connected to the dialog, or a bad branch
	//
	if (true == VectorFils.empty())
	{
  	if ((NSControle*) NULL == _Transfert.getControl())
			return true ;
    else
    	return false ;
  }

  // If there are sons, just check that a least one is not a bad branch
  //
  for (BBiterFils it = VectorFils.begin() ; VectorFils.end() != it ; it++)
  	if (false == (*it)->isBadDialogBranch(pDlg))
    	return false ;

	return true ;
}

BBItem*
BBFilsItem::getFirstItemSon()
{
  if (VectorFils.empty())
    return (BBItem*) 0 ;

  return *(VectorFils.begin()) ;
}

BBiterFils
BBFilsItem::getFirstItemSonIter()
{
  if (VectorFils.empty())
    return (BBiterFils) 0 ;

  return VectorFils.begin() ;
}

// -----------------------------------------------------------------------------
// instancier la patpatho active d'un BBFilsItem qui correspondant au pPPTEnCours
// du BBitem (égal à ce BBFilsItem)
// pere : itérateur sur l élément patpatho (dans PatpathoActuelle)
// correspondant au BBItem pere de ce BBFilsItem
// -----------------------------------------------------------------------------
NSPatPathoArray*
BBFilsItem::getPatpathoActive(NSPatPathoArray* pPatpathoActuelle, PatPathoIter pere)
{
try
{
	if (((NSPatPathoArray*) NULL == pPatpathoActuelle) ||
               (pPatpathoActuelle->empty()) || ((PatPathoIter) NULL == pere))
    return (NSPatPathoArray*) 0 ;

  int LignePere 	= (*pere)->getLigne() ;
  int ColonnePere = (*pere)->getColonne() ;

  PatPathoIter fils = pere ;

  if (pPatpathoActuelle->end() == fils)
    return (NSPatPathoArray*) 0 ;
  fils++ ;

  bool PatPathoNonVide = false ; // savoir s'il y a des données à mettre dans la la patpatho du fils ou non
  bool bContinuer = true ;
  Message message("") ;

  NSSuper* _pSuper = pContexte->getSuperviseur() ;

  NSPatPathoArray* pPatpathoFils = new NSPatPathoArray(_pSuper) ;

  while ((bContinuer) && (pPatpathoActuelle->end() != fils))
	{
    int LigneFils 	= (*fils)->getLigne() ;
    int ColonneFils = (*fils)->getColonne() ;

		if ((ColonneFils > ColonnePere) && (LigneFils > LignePere))
    {
      message.SetNoeud(     (*fils)->getNodeID()) ;
      message.SetLexique(   (*fils)->getLexique()) ;
      message.SetUnit(      (*fils)->getUnit()) ;
      message.SetCertitude( (*fils)->getCertitude()) ;
      message.SetInteret(   (*fils)->getInteret()) ;
      message.SetComplement((*fils)->getComplement()) ;
      message.SetPluriel(   (*fils)->getPluriel()) ;
      message.SetVisible(   (*fils)->getVisible()) ;
      message.SetType(      (*fils)->getType()) ;
      message.SetTexteLibre((*fils)->getTexteLibre()) ;
      message.SetNodeRight( (*fils)->getNodeRight()) ;

      NSPatPathoInfo* pPatPatho = new NSPatPathoInfo() ;
      pPatPatho->setNodeID(    message.GetNoeud()) ;
      pPatPatho->setLexique(   message.GetLexique()) ;
      pPatPatho->setUnit(      message.GetUnit()) ;
      pPatPatho->setComplement(message.GetComplement()) ;
      pPatPatho->setCertitude( message.GetCertitude()) ;
      pPatPatho->setInteret(   message.GetInteret()) ;
      pPatPatho->setPluriel(   message.GetPluriel()) ;
      pPatPatho->setVisible(   message.GetVisible()) ;
      pPatPatho->setNodeRight( message.GetNodeRight()) ;
      pPatPatho->setTexteLibre(message.GetTexteLibre()) ;
      pPatPatho->setColonne(ColonneFils - (ColonnePere + 1)) ;
      pPatPatho->setLigne(LigneFils - (LignePere + 1)) ;

      pPatpathoFils->push_back(pPatPatho) ;
      PatPathoNonVide = true ;
    }
    else
      bContinuer = false ;
    fils++ ;
  }

  if (PatPathoNonVide)
  {
    (getPatPatho())->push_back(new NSFatheredPatPathoArray(_pSuper, 0, new NSPatPathoArray(*pPatpathoFils))) ;
    return pPatpathoFils ;
  }

  delete pPatpathoFils ;
  return (NSPatPathoArray*) 0 ;
}
catch (...)
{
  erreur("Exception BBFilsItem::getPatpathoActive.", standardError, 0) ;
  return NULL ;
}
}

string
BBFilsItem::getLocalisation()
{
	string sEtiq ;
  NSDico::donneCodeSens(&_sEtiquette, &sEtiq) ;

  string sLoc = string("") ;
  if (_pPere && (string("") != _pPere->_sLocalisation))
    sLoc += _pPere->_sLocalisation + string(1, cheminSeparationMARK) ;

  sLoc += sEtiq ;

  return sLoc ;
}

// -----------------------------------------------------------------------------
//
// Implémentation de BBFilsArray
//
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Constructeur copie
// -----------------------------------------------------------------------------
BBFilsArray::BBFilsArray(const BBFilsArray& rv)
            :BBFilsItemArray()
{
try
{
  gestionVecteur = rv.gestionVecteur ;

  if (false == rv.empty())
    for (BBiterConst it = rv.begin() ; rv.end() != it ; it++)
      push_back(new BBFilsItem(**it)) ;
}
catch (...)
{
  erreur("Exception BBFilsArray ctor.", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------// Destructeur// -----------------------------------------------------------------------------
BBFilsArray::~BBFilsArray()
{
	vider() ;
}

BBFilsArray&
BBFilsArray::operator=(const BBFilsArray& src)
{
  if (this == &src)
  	return *this ;

  vider() ;

	if (false == src.empty())
    for (BBiterConst it = src.begin() ; src.end() != it ; it++)
      push_back(new BBFilsItem(**it)) ;

	return (*this) ;
}


// -----------------------------------------------------------------------------// Renvoie un pointeur sur le indice ième élément de l'array// Renvoie 0 si l'élément n'existe pas
// -----------------------------------------------------------------------------
BBFilsItem*
BBFilsArray::donneNieme(size_t indice)
{
  if (gestionVecteur)
  {
    if (indice < size())
      return (*this)[indice] ;
  }
  else
  {
    if (empty())
      return (BBFilsItem*) 0 ;

    BBiter iter = begin() ;
    for (size_t i = 0 ; (end() != iter) && (i < indice) ; iter++, i++)
      ;    if (end() != iter)
      return *iter ;
  }

  return (BBFilsItem*) 0 ;
}

// -----------------------------------------------------------------------------
// Renvoie un pointeur sur l'élément de l'array dont l'étiquette correspond
// Renvoie 0 si l'élément n'existe pas
// -----------------------------------------------------------------------------
BBFilsItem*
BBFilsArray::donneEtiqu(const string &sEtiquette)
{
  if (empty())
    return (BBFilsItem*) 0 ;

  BBiter Iter ;
  for (Iter = begin() ; (end() != Iter) && ((*Iter)->getItemLabel() != sEtiquette) ; Iter++)
    ;

  if (end() != Iter)
    return *Iter ;

  return (BBFilsItem*) 0 ;
}

// -----------------------------------------------------------------------------
// Vidange de l'array
// -----------------------------------------------------------------------------
void
BBFilsArray::vider()
{
  if (false == empty())
	  for (BBiter it = begin() ; end() != it ; )
    {
      delete *it ;
      erase(it) ;
    }
}

// -----------------------------------------------------------------------------
//
// Implémentation de BBItem
//
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Constructeur à partir d'un BBItem père
// -----------------------------------------------------------------------------
BBItem::BBItem(NSContexte* pCtx, BBFilsItem* pere, NSSmallBrother* pBig, NSPatPathoArray* pPPTEnCoursActif /*, NSPatPathoArray* pPPTEnCoursTMP */)
       :NSRoot(pCtx), _pBBFilsPere(pere), _pBigBoss(pBig), _KsInterface(-1)
{
try
{
  _sLocalisation 	        = string("") ;
  _pNSFonction	  	      = (NSBBFonction*) 0 ;
  _pNSDialog 	            = (NSDialog*) 0 ;
  _bModalDialog           = true ;    // boite modale par défaut
  _pParseur               = (nsarcParseur*) 0 ;  _bCreateParseur         = true ;  _pView                  = (NSBBMUEView*) 0 ;  _bCacher			          = false ;   // cacher la NSDialog
  _sNomDlg                = string("") ;
  _sIdArchetype           = string("") ;
  _bActif		              = false ;
  _ID				              = 0 ;
  _iFilsActif	            = 0 ;
  _pPPTEnCours 	          = pPPTEnCoursActif ;
  // pTempPPT             = pPPTEnCoursTMP ;
  _pPPTMerge              = (NSPatPathoArray*) 0 ;
  _bConclusion            = false ;
  _sPositionConclusion    = string("") ;
  _bConclusionAutomatique = false ;
  _iProfondeur            = 0 ;
  _iStatusDlg             = NSDLGSTATUS_NORMAL ;

#ifdef __OB1__
	// The ability to be initialised by BBK is inherited from father at construction
  //
	if (pere && pere->getItemFather())
		_KsInterface.setInitFromBbk(pere->getItemFather()->_KsInterface.getInitFromBbk()) ;
#endif

  lObjectCount++ ;
}
catch (...)
{
  erreur("Exception BBItem ctor 1.", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------
// Constructeur par défaut
// -----------------------------------------------------------------------------
BBItem::BBItem(NSContexte* pCtx, NSSmallBrother* pBig, bool Actif, int Id)
       :NSRoot(pCtx), _pBigBoss(pBig), _bActif(Actif), _ID(Id), _KsInterface(-1)
{
try
{
  _sLocalisation          = string("") ;
  _pBBFilsPere	          = (BBFilsItem*) 0 ;
  _bCacher			          = false ;                     // cacher la NSDialog
  _pNSDialog	            = (NSDialog*) 0 ;
  _bModalDialog           = true ;                      // boite modale par défaut
  _pParseur               = (nsarcParseur*) 0 ;  _bCreateParseur         = true ;  _pView                  = (NSBBMUEView*) 0 ;  _sNomDlg                = string("") ;
  _sIdArchetype           = string("") ;
  _pNSFonction	          = (NSBBFonction*) 0 ;
  _iFilsActif	            = 0 ;
  _pPPTEnCours            = _pBigBoss->getPatPatho() ;   //  NSPatPathoArray*
  // pTempPPT             = pBigBoss->getTmpPatho() ;
  _pPPTMerge              = (NSPatPathoArray*) 0 ;
  _bConclusion            = false ;
  _sPositionConclusion    = string("") ;
  _bConclusionAutomatique = false ;
  _iProfondeur            = 0 ;
  _iStatusDlg             = NSDLGSTATUS_NORMAL ;

  lObjectCount++ ;
}
catch (...)
{
  erreur("Exception BBItem ctor 2.", standardError, 0) ;
}
}

void
BBItem::InitFromBBItem(const BBItem& src)
{
try
{
  _pBigBoss 		          = src._pBigBoss ;

  _bActif                 = src._bActif ;
  _ID                     = src._ID ;

  _sLocalisation 	        = src._sLocalisation ;
  _bCacher			          = src._bCacher ; //cacher la NSDialog
  _Data   		            = src._Data ;
  _pBBFilsPere		        = src._pBBFilsPere ;
  _aBBItemFils 	          = src._aBBItemFils ;

  if (src._pParseur)
  {
    if (_bCreateParseur)
      _pParseur = new nsarcParseur(*(src._pParseur)) ;
    else
      _pParseur = src._pParseur ;
  }
  else
    _pParseur             = (nsarcParseur*) 0 ;

  _bCreateParseur         = src._bCreateParseur ;

  _pView                  = src._pView ;
  _pNSDialog		          = src._pNSDialog ;
  _bModalDialog           = src._bModalDialog ;
  _sNomDlg                = src._sNomDlg ;
  _sIdArchetype           = src._sIdArchetype ;

  _pNSFonction		        = src._pNSFonction ;
  _iFilsActif		          = src._iFilsActif ;

  _pPPTEnCours	  	      = src._pPPTEnCours ;
  // pTempPPT		          = src.pTempPPT ;
  _pPPTMerge              = src._pPPTMerge ;
  _bConclusion            = src._bConclusion ;
  _sPositionConclusion    = src._sPositionConclusion ;
  _bConclusionAutomatique = src._bConclusionAutomatique ;
  _iProfondeur            = src._iProfondeur ;
  _iStatusDlg             = src._iStatusDlg ;
}
catch (...)
{
  erreur("Exception BBItem::InitFromBBItem", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------
// Constructeur copie
// -----------------------------------------------------------------------------
BBItem::BBItem(const BBItem& src)
       :NSRoot(src.pContexte), _KsInterface(src._KsInterface)
{
  InitFromBBItem(src) ;

  lObjectCount++ ;
}

// -----------------------------------------------------------------------------// Destructeur// -----------------------------------------------------------------------------
BBItem::~BBItem()
{
  lObjectCount-- ;

  // _aBBItemFils.vider() ;

	if (_pNSFonction)
    delete _pNSFonction ;

  if (_bCreateParseur && _pParseur)
    delete _pParseur ;

  if (_pPPTMerge)
    delete _pPPTMerge ;
}
// -----------------------------------------------------------------------------
// Opérateur d'affectation
// -----------------------------------------------------------------------------
inline BBItem&
BBItem::operator=(const BBItem& src)
{
	if (this == &src)
  	return *this ;

try
{
  if (_bCreateParseur && _pParseur)
  {
  	delete _pParseur ;
    _pParseur = (nsarcParseur*) 0 ;
  }

  InitFromBBItem(src) ;

  _KsInterface = src._KsInterface ;

  return *this ;
}
catch (...)
{
  erreur("Exception BBItem::operator=", standardError, 0) ;
  return (*this) ;
}
}

bool
BBItem::operator==(const BBItem& src) const
{
  if (_sLocalisation == src._sLocalisation)
    return true ;

  return false ;
}

// -----------------------------------------------------------------------------
// ce BBItem crée son fils (qui correspond au NStreeNode qui vient d'être
// crée) et l'ajoute à la liste de ses fils
// -----------------------------------------------------------------------------
void
BBItem::CreerFilsManuel(NSControle* pControle, NSControle* pControlePetitFrere)
{
try
{

  if ((NSControle*) NULL == pControle)
    return ;

  // Création d'un BBFilsItem (qui correspond au NStreeNode qui vient d'être crée)
  //
  BBFilsItem* pFilsItem = new BBFilsItem(pContexte, this, _pBigBoss) ;

  pFilsItem->getItemTransfertData()->setControl(pControle) ;
  pControle->setTransfert(pFilsItem->getItemTransfertData()) ;
  pFilsItem->setItemLabel(pControle->getIdentite()) ;

  // Ajout de l'élément à l'Array de Fils
  if ((NSControle*) NULL == pControlePetitFrere)
    _aBBItemFils.push_back(pFilsItem) ;
  else
  {
  	NSTransferInfo* pTrInfo = pControlePetitFrere->getTransfert() ;
    if (pTrInfo)
    {
    	BBFilsItem* pFilsItemPetitFrere = pTrInfo->getFilsItem() ;
    	BBiter iter = find(_aBBItemFils.begin(), _aBBItemFils.end(), pFilsItemPetitFrere) ;
    	//inserer pFilsItem avant pFilsItemPetitFrere
    	_aBBItemFils.insert(iter, 1, pFilsItem) ;
    }
    else
    	_aBBItemFils.push_back(pFilsItem) ;
  }
}
catch (...)
{
  erreur("Exception BBItem::CreerFilsManuel.", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------
// créer un BBitem de code lexique sItem, fils de this
// -----------------------------------------------------------------------------
BBItem*
BBItem::CreerBBitem(NSSmallBrother* pBigBoss, string sItem, NSPatPathoArray* pPPT, string sSens /*, BBItem* pBBItemCree*/)
{
try
{
  // On cherche le FilsItem à prolonger
  BBFilsItem* pBBFils = TrouverFilsAyantBonneEtiquette(sItem, "") ;
  if ((BBFilsItem*) NULL == pBBFils)
    return (BBItem*) 0 ;

  NSSuper* _pSuper = pContexte->getSuperviseur() ;

  // On crée la patpatho
  NSPatPathoArray* pPath = new NSPatPathoArray(_pSuper) ;
  if (pPPT)
    *pPath = *pPPT ;
  pBBFils->getPatPatho()->push_back(new NSFatheredPatPathoArray(_pSuper, 0, pPath)) ;

  NSPatPathoArray* pPathTmp = new NSPatPathoArray(_pSuper) ;
  pBBFils->getTmpPatho()->push_back(new NSFatheredPatPathoArray(_pSuper, 0, pPathTmp)) ;

  // On crée le BBItem
  //
  BBItem NewBBItem(pContexte, pBBFils, pBigBoss, pPath /*, pPathTmp */) ;
  BBItem* pNewItem = pBBFils->VectorFils.addBBItem(NewBBItem) ; // Référence le BBItem chez son fils

  pNewItem->setItemData(pBBFils->getItemData()) ;
  pNewItem->_sNomDlg = pNewItem->getItemDialogName() ;
  string sCodeSens ;
  NSDico::donneCodeSens(&sItem, &sCodeSens) ;
  pNewItem->_sLocalisation = sSens + "/" + sCodeSens ;
  pNewItem->_iProfondeur   = _iProfondeur + 1 ;
  if (string("") != pNewItem->getItemDialogFile())
    if (false == pBigBoss->CreerNouveauModule(pNewItem->getItemDialogFile()))
      return (BBItem*) 0 ;

  pNewItem->creer(false) ;
  return pNewItem ;
}
catch (...)
{
  erreur("Exception BBItem::CreerBBitem.", standardError, 0) ;
  return 0 ;
}
}

// -----------------------------------------------------------------------------
// Function     : int BBItem::creer(string code)
// Arguments    : Consultation -> Si Consultation = true, on ne dispatche pas la
//                patpatho car les NSTreeWindow le font elles même
// Description  : Demande à l'objet de se mettre en place : il crée des objets
//				        BBFilsItem pour chaque étiquette, qu'il stocke dans aBBItemFils.
//					      A la fin, lance creerFils
// -----------------------------------------------------------------------------
int
BBItem::creer(bool Consultation)
{
try
{
  string sCodeSens ;

	// Si l'élément est lié à une fonction, on la crée
	if (lienFonction())
  {
    if (_pNSFonction)
      delete _pNSFonction ;

		_pNSFonction = new NSBBFonction(pContexte, _Data.getFctName(), this) ;
  }

  NSSuper* pSuper = pContexte->getSuperviseur() ;

  // Cas d'un bbitem lié à un archétype
  if (ouvreArchetype())
  {
    string sArchetype = _Data.getSonsList() ;

    // Get file supposing it is an Archetype
    //
    NSArcManager::TYPEARC iArcType = NSArcManager::archetype ;
    string sArchetypeFile = pSuper->getArcManager()->DonneFichierArchetypeParNom(iArcType, sArchetype) ;
    if (string("") == sArchetypeFile)
    {
      // Get file supposing it is a Decision tree
      //
      iArcType = NSArcManager::decisionTree ;
      sArchetypeFile = pSuper->getArcManager()->DonneFichierArchetypeParNom(iArcType, sArchetype) ;

      if (string("") == sArchetypeFile)
      {
        string sErrorMsg = pSuper->getText("archetypesManagement", "cannotFindThisArchetypeFile") ;
        sErrorMsg += string(" ") + sArchetype ;
        erreur(sErrorMsg.c_str(), standardError, 0) ;
        return 1 ;
      }
    }

    if (_pParseur)
    	delete _pParseur ;

    switch(iArcType)
    {
      case NSArcManager::archetype :
        return createFromArchetype(Consultation, sArchetypeFile) ;
      case NSArcManager::decisionTree :
        return createFromDecisionTree(Consultation, sArchetypeFile) ;
    }
  }

	// Si cet élément n'a pas de fils, on sort
	if ((string("") == _Data.getSonsList()) || (strspn(_Data.fils, " ") == strlen(_Data.fils)))
		return 0 ;

	// On passe en revue le champ Fils, en créant un BBFilsItem pour chaque étiquette  VecteurRechercheSelonCritere VecteurSelonCritere(GUIDE) ;
  for (int i = 0 ; ('\0' != _Data.fils[i]) && (' ' != _Data.fils[i]) ; i++)
  {
    // Création d'un BBFilsItem
    BBFilsItem* pFilsItem = new BBFilsItem(pContexte, this, _pBigBoss) ;

    // Prise de l'étiquette
    string sNewLabel = string("") ;
    for ( ; ('\0' != _Data.fils[i]) && (nodeSeparationMARK != _Data.fils[i]) ; i++)
      sNewLabel += _Data.fils[i] ;
    pFilsItem->setItemLabel(sNewLabel) ;

    // On fabrique le chemin indiqué par le futur élément

    // traiter le cas des textes libres à part
    if (false == pFilsItem->isFreeText())
    {
      string sFilsLabel = pFilsItem->getItemLabel() ;
      NSDico::donneCodeSens(&sFilsLabel, &sCodeSens) ;
      VecteurSelonCritere.AjouteEtiquette(sCodeSens) ;
    }

    // Ajout de l'élément à l'Array de Fils
    _aBBItemFils.push_back(pFilsItem) ;

    if (('\0' == _Data.fils[i]) || (' ' == _Data.fils[i]))
      break ;
  }

  pSuper->afficheStatusMessage("Recherche des fils guides") ;  pSuper->getFilGuide()->chercheChemin(&_sLocalisation, &VecteurSelonCritere, NSFilGuide::compReseau) ;

  bool trouve ;
  if (false == _aBBItemFils.empty())
	{
  	BBiter it = _aBBItemFils.begin() ;
  	for ( ; _aBBItemFils.end() != it ; it++)
  	{
    	// traiter le cas des textes libres à part
    	if ((*it)->isFreeText())
    	{
        BBItemData* pItemData = (*it)->getItemData() ;
        if (pItemData)
        {
      	  pItemData->metAZero() ;
      	  pItemData->setSemanticPath("~****/#####") ;
      	  pItemData->setSonsList("#TLI#1") ;
      	  pItemData->setDialogName("TEXTLIB") ;
      	  pItemData->setOpenDialog(true) ;
      	  pItemData->setLevelShift("+00+00") ;
      	  pItemData->setDialogFile("NSBB") ;
      	  pItemData->setEmptyActivation(false) ;
        }
        (*it)->FilsProlongeable = true ;
    	}
    	else
    	{
        string sFilsLabel = (*it)->getItemLabel() ;
      	NSDico::donneCodeSens(&sFilsLabel, &sCodeSens) ;
      	VecteurSelonCritere.SetData(sCodeSens, &trouve, (*it)->getItemData()) ;
      	if (trouve)
      	{
        	// si feuille

        	// cas particulier : étiquette = #C£££1 : pas de fils mais
        	// le considérer comme prolongeable
        	size_t pos = (*it)->getItemData()->getSonsList().find_first_not_of(string(" ")) ;
        	if (pos)
          	(*it)->FilsProlongeable = false ;
        	else
          	(*it)->FilsProlongeable = true ;
      	}
      	else
        	(*it)->FilsProlongeable = false ;
    	}
    }
  }

  // Don't do that before DispatcherPatPatho() because it can lead to
  // potential active item excluding the good ones
  // creerExclus() ;

  // M.A.J des patpatho
  //
  if (false == Consultation)
    if (_pPPTEnCours)
      DispatcherPatPatho() ;

  creerExclus() ;
  creerFils() ;
  return 0 ;
}
catch (...)
{
  erreur("Exception BBItem::creer", standardError, 0) ;
  return 1 ;
}
}
intBBItem::createFromArchetype(bool bConsultation, string sArchetypeFile){  _pParseur = new nsarcParseur(pContexte->getSuperviseur()) ;
  nsarcParseur *pArcParseur = (nsarcParseur*) _pParseur ;
  if (false == pArcParseur->open(sArchetypeFile))
    return 1 ;

  Carchetype* pArchetype = pArcParseur->getArchetype() ;
  if ((Carchetype*) NULL == pArchetype)
    return 1 ;

  // on renseigne les champs définissant le dialogue
  // (ces champs seront utilisés par BBItem::activer dans creerDialogue()
  //  qui ne sera appelée que pour le BBItem Root)
  if (pArchetype->getDialog())
  {
    string sNomDialog = pArchetype->getDialog()->getStringAttribute(ATTRIBUT_DIALOGUE_NOM) ;
    _sNomDlg = sNomDialog ;
    string sNomDll = pArchetype->getDialog()->getStringAttribute(ATTRIBUT_DIALOGUE_DLL) ;
    _Data.setDialogFile(sNomDll) ;
  }
  else
  {
    if (string("") != _sNomDlg)
      _sNomDlg = string("") ;
    _Data.setDialogFile(string("")) ;
  }

  _sIdArchetype = pArchetype->getName() ;

  // on développe tout l'archetype à partir du BBItem root
  creerArchetype(pArchetype->getRootItem(), bConsultation) ;

  return 0 ;
}intBBItem::createFromDecisionTree(bool /* bConsultation */, string /* sDecisionTreeFile */){/*  _pParseur = new nsGuidelineParseur(pContexte->getSuperviseur()) ;
  nsGuidelineParseur *pDecisionTreeParseur = (nsGuidelineParseur*) _pParseur ;
  if (false == pDecisionTreeParseur->open(sDecisionTreeFile))
    return 1 ;

  Cguideline* pDecisionTree = pDecisionTreeParseur->getGuideline() ;
  if (NULL == pDecisionTree)
    return 1 ;

  // on renseigne les champs définissant le dialogue
  // (ces champs seront utilisés par BBItem::activer dans creerDialogue()
  //  qui ne sera appelée que pour le BBItem Root)
  if (pDecisionTree->getDialog())
  {
    string sNomDialog = pDecisionTree->getDialog()->getStringAttribute(ATTRIBUT_DIALOGUE_NOM) ;
    _sNomDlg = sNomDialog ;
    string sNomDll = pDecisionTree->getDialog()->getStringAttribute(ATTRIBUT_DIALOGUE_DLL) ;
    pDonnees->setDialogFile(sNomDll) ;
  }
  else
  {
    if (string("") != _sNomDlg)
      _sNomDlg = string("") ;
    pDonnees->setDialogFile(string("")) ;
  }

  sIdArchetype = pDecisionTree->getName() ;

  // on développe tout l'archetype à partir du BBItem root
  creerArchetype(pDecisionTree->getRootItem(), bConsultation) ;
*/

  return 0 ;}// -----------------------------------------------------------------------------// Function     : int BBItem::creerArchetype(Citem* pItem, bool Consultation)// Arguments    : pItem -> objet item à développer//                Consultation -> Si Consultation = true, on ne dispatche
//                pas la patpatho car les NSTreeWindow le font elles même
// Description  : Demande à l'objet de se mettre en place : il crée des objets
//				        BBFilsItem pour chaque étiquette, qu'il stocke dans aBBItemFils.
//                (idem creer mais avec des Citem au lieu des fils guide)
//				        A la fin, lance creerFils
// -----------------------------------------------------------------------------
// ATTENTION    : a priori on ne devrait jamais avoir Consultation = true (car
//                les Archetypes ne sont probablement jamais liés à des
//                NSTreeWindow (?))
// -----------------------------------------------------------------------------
int
BBItem::creerArchetype(Citem* pItem, bool Consultation){try{	if ((Citem*) NULL == pItem)		return 0 ;  NSSuper* _pSuper = pContexte->getSuperviseur() ;	string sMsg = "creerArchetype() : Entrée" ;	_pSuper->trace(&sMsg, 1, NSSuper::trSubDetails) ;  Valeur_array* pValueArray = pItem->getArrayFils() ;	// Si l'élément Citem n'a pas de fils, on sort	if (((Valeur_array*) NULL == pValueArray) || (pValueArray->empty()))		return 0 ;  bool bChercherChemin = false ;	// Vecteur de recherche pour les fils guides suivant l'archetype	// (à partir des Citem feuilles de l'archetype)	VecteurRechercheSelonCritere VecteurSelonCritere(GUIDE) ;	// On récupère les fils de premier niveau	for (ValIter ival = pValueArray->begin() ; pValueArray->end() != ival ; ival++)	{		// Item		if (LABEL_ITEM == (*ival)->getLabel())		{			// création d'un BBFilsItem du BBItem			BBFilsItem* pFilsItem = new BBFilsItem(pContexte, this, _pBigBoss) ;			// Stockage du Citem dans le BBFilsItem      Cbalise* pObject = (*ival)->getObject() ;      if (pObject)			  pFilsItem->pCitem = dynamic_cast<Citem*>(pObject) ;			// Récupération de l'étiquette attachée au Citem			pFilsItem->setItemLabel(pFilsItem->pCitem->getStringAttribute(ATTRIBUT_ITEM_CODE)) ;			sMsg = "creerArchetype() : Nouveau fils " + pFilsItem->getItemLabel() ;			_pSuper->trace(&sMsg, 1, NSSuper::trSubDetails) ;/*			string sComplement = pFilsItem->pCitem->getStringAttribute(ATTRIBUT_ITEM_COMPLEMENT) ;			if (sComplement != )*/			string sDecal = pFilsItem->pCitem->getStringAttribute(ATTRIBUT_ITEM_DECAL) ;			if (string("") == sDecal) // si le decalage n'est pas précisé...				sDecal = string("+01+01") ;  // ... on prend la valeur par défaut			// on donne au nouveau fils son décalage			pFilsItem->getItemData()->setLevelShift(sDecal) ;			// Si c'est une feuille			if (pFilsItem->pCitem->getArrayFils()->empty())			{				// sauf dans le cas des textes libres				if (false == pFilsItem->isFreeText())				{
					if (pFilsItem->pCitem->getArchetype() == string(""))
          {
						// on ajoute au vecteur de recherche
            string sFilsLabel = pFilsItem->getItemLabel() ;
            string sCodeSens ;
						NSDico::donneCodeSens(&sFilsLabel, &sCodeSens) ;
						VecteurSelonCritere.AjouteEtiquette(sCodeSens) ;
						bChercherChemin = true ;
					}
				}
			}			// Ajout du BBFilsItem à l'array des fils			_aBBItemFils.push_back(pFilsItem) ;		}
		// Contrainte
		else if (LABEL_CONTRAINTE == (*ival)->getLabel())
		{      Cbalise* pObject = (*ival)->getObject() ;      if (pObject)      {			  Ccontrainte* pContrainte = dynamic_cast<Ccontrainte *>(pObject) ;			  if (pContrainte && (pContrainte->getStringAttribute(ATTRIBUT_CONTR_TYPE) == string(VAL_ATTR_CONTR_TYPE_EXCLUS)))			  {				  string sExclus = pContrainte->getStringAttribute(ATTRIBUT_CONTR_LISTE) ;				  _Data.setSonsRules(sExclus) ;			  }      }		}	}	if (bChercherChemin)	{		sMsg = "creerArchetype() : Recherche des fils guides suivant archetype" ;		_pSuper->trace(&sMsg, 1, NSSuper::trSubDetails) ;		_pSuper->afficheStatusMessage("Recherche des fils guides suivant archetype") ;		_pSuper->getFilGuide()->chercheChemin(&_sLocalisation, &VecteurSelonCritere, NSFilGuide::compReseau) ;
	}

	if (false == _aBBItemFils.empty())
	{
		// parcours de l'array des fils créés et détection des fils prolongeables
		// normalement chaque BBFilsItem de l'array est associé à un Citem
    //
		for (BBiter it = _aBBItemFils.begin() ; _aBBItemFils.end() != it ; it++)		{
			// cas des feuilles
			if ((*it)->pCitem->getArrayFils()->empty())
			{
				// traiter le cas des textes libres à part
				if ((*it)->isFreeText())
				{
          BBItemData* pFilsData = (*it)->getItemData() ;
          if (pFilsData)
          {
					  pFilsData->metAZero() ;
					  pFilsData->setSemanticPath("~****/#####") ;
					  pFilsData->setSonsList("#TLI#1") ;
					  pFilsData->setDialogName("TEXTLIB") ;
					  pFilsData->setOpenDialog(true) ;
					  pFilsData->setLevelShift("+00+00") ;
					  pFilsData->setDialogFile("NSBB") ;
					  pFilsData->setEmptyActivation(false) ;
          }

					(*it)->FilsProlongeable = true ;
				}
      	// cas des feuilles non texte-libre
				else
				{
					string sArchetype = (*it)->pCitem->getArchetype() ;

        	// s'il existe un Archetype, on le met en place
        	if (sArchetype != string(""))
        	{
            BBItemData* pFilsData = (*it)->getItemData() ;
            if (pFilsData)
            {
						  pFilsData->metAZero() ;
						  pFilsData->setSemanticPath((*it)->getItemLabel()) ;
						  pFilsData->setSonsList(sArchetype) ;
						  pFilsData->setOpenArchetype(true) ;

          	  string sDecal = (*it)->pCitem->getStringAttribute(ATTRIBUT_ITEM_DECAL) ;
						  if (string("") == sDecal) // si le decalage n'est pas précisé...							  sDecal = string("+01+01") ;  // ... on prend la valeur par défaut

						  pFilsData->setLevelShift(sDecal) ;
						  pFilsData->setEmptyActivation(false) ;
            }

						(*it)->FilsProlongeable = true ;
        	}
        	// on regarde s'il existe un fil guide
        	else
        	{
            string sItemLabel = (*it)->getItemLabel() ;
            string sCodeSens ;
						NSDico::donneCodeSens(&sItemLabel, &sCodeSens) ;

            bool bTrouve ;
						VecteurSelonCritere.SetData(sCodeSens, &bTrouve, (*it)->getItemData()) ;
						if (bTrouve)						{
							// si feuille

							// cas particulier : étiquette = #C£££1 : pas de fils mais
							// le considérer comme prolongeable
							size_t pos = (*it)->getItemData()->getSonsList().find_first_not_of(string(" ")) ;

							// Pour les Archétypes, on n'accepte comme prolongement							// que des fils guides qui ouvrent une boite de dialogue
							// (ou un Archétype)
							// --
							// Archetypes can only be extended by fils guides that
							// open a dialog or another Archetype
							if ((0 == pos) && (((*it)->getItemData()->ouvreDlg() || (*it)->getItemData()->ouvreArchetype())))
								(*it)->FilsProlongeable = true ;
							else
								(*it)->FilsProlongeable = false ;
						}
						else
							(*it)->FilsProlongeable = false ;
					}
				}
			}
			else // cas des non-feuilles => fils prolongeable
				(*it)->FilsProlongeable = true ;
		} // fin du for (; it != aBBItemFils.end(); it++)
	}

	sMsg = "creerArchetype() : MAJ des patpatho" ;
	_pSuper->trace(&sMsg, 1, NSSuper::trSubDetails) ;

	// M.A.J des patpatho (sauf mode Consultation)
	if (false == Consultation)
		if (_pPPTEnCours)
			DispatcherPatPatho() ;

	sMsg = "creerArchetype() : Appel de creerFilsArchetype" ;
	_pSuper->trace(&sMsg, 1, NSSuper::trSubDetails) ;

	creerExclus() ;
	creerFilsArchetype() ;

	return 0 ;
}catch (...){  erreur("Exception BBItem::creerArchetype.", standardError, 0) ;  return 1 ;}}boolBBItem::verifConstraintItem(string* psMessage){  if ((nsarcParseur*) NULL == _pParseur)    return true ;  nsarcParseur *pArcParseur = dynamic_cast<nsarcParseur*>(_pParseur) ;  if (pArcParseur)    return verifConstraintItemForArchetype(psMessage, pArcParseur) ;  return true ;}boolBBItem::verifConstraintItemForArchetype(string* psMessage, nsarcParseur *pArcParseur){  if (((nsarcParseur*) NULL == pArcParseur) ||      ((Carchetype*)   NULL == pArcParseur->getArchetype()))    return true ;  Carchetype* pArc = pArcParseur->getArchetype() ;  if (((Carchetype*) NULL == pArc) || pArc->vect_val.empty())    return true ;  NSSuper* _pSuper = pContexte->getSuperviseur() ;  NSPatPathoArray TempPPT(_pSuper) ;  rapatrieTmpPpt(&TempPPT, 0) ;  // on recherche dans l'archetype la balise items (qui se situe à la racine)  for (ValIter i = pArc->vect_val.begin() ; pArc->vect_val.end() != i ; i++)  {    Cbalise* pObject = (*i)->getObject() ;    if (pObject)    {      Citem *pItemRoot = dynamic_cast<Citem *>(pObject) ;      if (pItemRoot && (true == pItemRoot->bItemRoot))      {        // à priori on est dans la balise "items"        Valeur_array *pItemSons = pItemRoot->getArrayFils() ;        if (((Valeur_array*) NULL == pItemSons) || pItemSons->empty())          return false ;        for (ValIter itemIter = pItemSons->begin() ; itemIter != pItemSons->end() ; itemIter++)
        {
          Cbalise* pSonObject = (*itemIter)->getObject() ;
          if (pSonObject)          {
            Citem *pItem = dynamic_cast<Citem *>(pSonObject) ;
            if (pItem && (false == pItem->verifConstraintsWithPPatho(&TempPPT, psMessage)))
              return false ;
          }
        }
        return true ;
      }
    }
  }  return false ;/*  // si on a pas trouvé la balise "items" on ne peut pas continuer, on sort  if (itemsVal == NULL)  {    // erreur    return ;  }  for (PatPathoIter pptIter = pTempPPT->begin() ; pptIter != pTempPPT->end() ; pptIter++)  {    if ((*pptIter)->pDonnees->getColonne() == 0)    {      // on est à la racine de la patpatho      Cbalise *pBalise = itemsVal->pObject ;      for (Val_iter v = pBalise->vect_val.begin() ; v != pBalise->vect_val.end() ; v++)      {        Attr_iter a = NULL ;        for (a = pBalise->vect_attr.begin() ; a != pBalise->vect_attr.end() ; a++)          if ((*a)->sLabel == "code")            break ;        if ((*a)->sLabel == "code")        {          // ce qui est contenu dans (*a)->sValue doit être le chemin          // on construit le noeud à partir du code lexique et du complement          char  pcPath[128] ;          sprintf(pcPath, "%s/$%s", (*pptIter)->pDonnees->lexique, (*pptIter)->pDonnees->complement) ;          if (strcmp(pcPath, (*a)->sValue.c_str()))          {            // le noeud correspond à l'item dans l'archetype            // il faut maintenant vérifier ces fils            //if (!comparePPathoWithArc()          }        }        // il faut vérifier que le code correspond à la patpatho        // code archetype correspond à un iter de (*i)->pObject->vect_attr (sLabel="code") et sValue=$chemin        // $chemin doit correspondre à (*pptIter)->pDonnees->lexique + "/$" + (*pptIter)->pDonnees->complement        // si il correspond, on vérifie la contrainte et on lance la vérification        // sur ses fils        NSSuper *pSuper = pBigBoss->pContexte->getSuperviseur() ;      }    }  }*/}boolBBItem::verifConstraintItemForDecisionTree(string* /* psMessage */, nsGuidelineParseur* /* pDecisionTreeParseur */){/*  if ((NULL == pDecisionTreeParseur) || (NULL == pDecisionTreeParseur->getDecisionTree()))    return true ;  CdecisionTree* pDecisionTree = pDecisionTreeParseur->getDecisionTree() ;  if (pDecisionTree->vect_val.empty())    return true ;  NSSuper* _pSuper = pContexte->getSuperviseur() ;  NSPatPathoArray TempPPT(_pSuper) ;  rapatrieTmpPpt(&TempPPT, 0) ;  // on recherche dans l'archetype la balise items (qui se situe à la racine)  for (ValIter i = pDecisionTree->vect_val.begin() ; pDecisionTree->vect_val.end() != i ; i++)  {    Citem *pItemRoot = dynamic_cast<Citem *>((*i)->pObject) ;    if ((pItemRoot) && (true == pItemRoot->bItemRoot))    {      // à priori on est dans la balise "items"      Valeur_array *pItemSons = pItemRoot->getArrayFils() ;      if ((NULL == pItemSons) || pItemSons->empty())        return false ;      for (ValIter itemIter = pItemSons->begin() ; itemIter != pItemSons->end() ; itemIter++)
      {
        Citem *pItem = dynamic_cast<Citem *>((*itemIter)->pObject) ;
        if (pItem && (false == pItem->verifConstraintsWithPPatho(&TempPPT, psMessage)))
          return false ;
      }
      return true ;
    }
  }*/  return false ;}// -----------------------------------------------------------------------------// -----------------------------------------------------------------------------voidBBItem::MiseAjourPatPatho(NSPatPathoArray* pPatpathoFils, NSPatPathoInfo* pPatPatho, int ligneNonNulle)
{
try
{
  if (((NSPatPathoInfo*)  NULL == pPatPatho) ||
      ((NSPatPathoArray*) NULL == pPatpathoFils))
    return ;

  pPatPatho->setColonne(pPatPatho->getColonne() - 1) ;
  pPatPatho->setLigne(pPatPatho->getLigne() - ligneNonNulle + ORIGINE_PATH_PATHO) ;

  // un élément de plus
  pPatpathoFils->push_back(new NSPatPathoInfo(*pPatPatho)) ;
}
catch (...)
{
  erreur("Exception BBItem::MiseAjourPatPatho.", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------// -----------------------------------------------------------------------------voidBBItem::MiseAjourPatPathoZero(NSPatPathoArray *pPatpathoFils, NSPatPathoInfo *pPatPatho, int ligneNonNulle)
{
try
{
  if (((NSPatPathoInfo*) NULL == pPatPatho) || ((NSPatPathoArray*) NULL == pPatpathoFils))
    return ;

  pPatPatho->setColonne(pPatPatho->getColonne()) ;
  pPatPatho->setLigne(pPatPatho->getLigne() - ligneNonNulle + ORIGINE_PATH_PATHO ) ;

  // un élément de plus
  pPatpathoFils->push_back(new NSPatPathoInfo(*pPatPatho)) ;
}
catch (...)
{
  erreur("Exception BBItem::MiseAjourPatPathoZero.", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------
// A partir du complément sComplement reconstruire la string $0..sComplement
// exemple  sComplement = 1.5 on aura $001.5
// exemple  sComplement = 1 on aura $00001
// -----------------------------------------------------------------------------
string
BBItem::AjusteEtiquetteAvecDollar(string sComplement)
{
  if (BASE_LEXIQUE_LEN < strlen(sComplement.c_str()))
    return string("") ;

  string sDollar = string(1, '$') ;

  for (size_t i = 1 ; i < BASE_LEXIQUE_LEN - strlen(sComplement.c_str()) ; i++)
    sDollar = sDollar + string("0") ;
    
  return (sDollar + sComplement) ;
}

// -----------------------------------------------------------------------------
// retourner le BBFilsItem DISPONIBLE ayant sEtiquette comme étiquette.
// traiter le cas où l'étiquette du fils contient WCEA00.
// traiter le cas où l'étiquette du fils est du type 200001/£N0;01/$00001 : dans ce cas
// sComplement est forcément 1 : $00001
// si multidialogue alors ne pas tenir compte de Actif
// -----------------------------------------------------------------------------
BBFilsItem*
BBItem::TrouverFilsAyantBonneEtiquette(string sEtiquette, string sComplement)
{
  if (string("") == sEtiquette)
    return (BBFilsItem*) 0 ;

  if (_aBBItemFils.empty())
    return (BBFilsItem*) 0 ;

  string sEtiq ;
  NSDico::donneCodeSens(&sEtiquette, &sEtiq) ;

  for (BBiter Iter = _aBBItemFils.begin() ; _aBBItemFils.end() != Iter ; Iter++)
  {
    if ((*Iter)->isAvailable())
    {
      string sFilsLabel = (*Iter)->getItemLabel() ;
      string sEtiqIter ;
      NSDico::donneCodeSens(&sFilsLabel, &sEtiqIter) ;

      // égalité stricte
      if (sEtiqIter == sEtiq)
        return *Iter ;

      if (sEtiqIter.find(sEtiq) != string::npos)
      {
        // Cas particulier : l'étiquette du fils contient WCEA00
        size_t posWCEA0 = sEtiqIter.find(string("WCEA0")) ;
        if ((string::npos != posWCEA0) && (posWCEA0 < strlen(sEtiqIter.c_str())))
          return *Iter ;

        if (string("") != sComplement)
        {
          // On teste avec le complément "brut"
          if (sEtiqIter == (sEtiq + string(1, cheminSeparationMARK) + string(1, '$') + sComplement))
            return *Iter ;

          // Cas (*Iter)->sEtiquette = 200001/£N0;01/$00001
          // sEtiquette = 200001/£N0;01
          // AjusteEtiquetteAvecDollar(sComplement) = $00001
          if (sEtiqIter == (sEtiq + string(1, cheminSeparationMARK) + AjusteEtiquetteAvecDollar(sComplement)))
            return *Iter ;
        }
        else
        {
          if (false == (*Iter)->isActif())
          {
            // Cas particulier : l'étiquette du fils contient WCEA00
            posWCEA0 = sEtiqIter.find(string("WCEA0")) ;
            if ((string::npos != posWCEA0) && (posWCEA0 < strlen(sEtiqIter.c_str())))
              return *Iter ;

            // Cas (*Iter)->sEtiquette = 200001/£N0;01/$00001
            // sEtiquette = 200001/£N0;01
            // AjusteEtiquetteAvecDollar(sComplement) = $00001

            if (sEtiqIter == (sEtiq + string(1, cheminSeparationMARK) + AjusteEtiquetteAvecDollar(sComplement)))
              return *Iter ;
          }
        }
      }
    }
  }

  return (BBFilsItem*) 0 ;
}

BBFilsItem*
BBItem::GetSonWithLabel(string sLabel)
{
  if ((string("") == sLabel) || _aBBItemFils.empty())
    return (BBFilsItem*) 0 ;

  string sEtiq ;
  NSDico::donneCodeSens(&sLabel, &sEtiq) ;

  for (BBiter Iter = _aBBItemFils.begin() ; _aBBItemFils.end() != Iter ; Iter++)
  {
    string sFilsLabel = (*Iter)->getItemLabel() ;

    string sEtiqIter ;
    NSDico::donneCodeSens(&sFilsLabel, &sEtiqIter) ;

    if (sEtiqIter == sEtiq)
      return *Iter ;
  }

  return (BBFilsItem*) 0 ;
}


// -----------------------------------------------------------------------------
// M.A.J du patptho de tous les BBFilsItem à partir de celui de leur pére
// -----------------------------------------------------------------------------
void
BBItem::DispatcherPatPatho()
{
	if (((NSPatPathoArray*) NULL == _pPPTEnCours) || (_pPPTEnCours->empty()))
    return ;

  NSSuper* pSuper = pContexte->getSuperviseur() ;

  string sEtiquettePatpatho ;
  BBiter Iter ;

  int ligneNulle, ColonneNulle ;  bool patPathoFilsNonvide = false ;  // détecter si la patpatho est vide ou non
  int colonneNonNulle ;
  int ligneNonNulle ;
  string motcompose = string("") ;   //mot composé

try
{
  PatPathoIter iterColonneNulle = _pPPTEnCours->begin() ; // items dont la colonne est nulle
  PatPathoIter k 				        = _pPPTEnCours->begin() ; // iterateur temporaire
  PatPathoIter iterTemp 		    = _pPPTEnCours->begin() ; // iterateur temporaire

  Message currentMsg ;

  int pFilSuivant ;
  int decalageNonNul ;

  // PatPathoIter i = pPPTEnCours->begin() ;

  // On passe en revue toute la patpatho
  while (_pPPTEnCours->end() != iterColonneNulle)
  {
    currentMsg.Reset() ;
    decalageNonNul = 1 ;

    // items dont la colonne est non nulle
    PatPathoIter iterColonneNonNulle = _pPPTEnCours->begin() ;
    sEtiquettePatpatho = string("") ;
    if ((*iterColonneNulle)->getColonne() == ORIGINE_PATH_PATHO)
    {
      // Récupérer les données dans la patpatho
      sEtiquettePatpatho = (*iterColonneNulle)->getLexique() ; // mot unique
      currentMsg.SetNoeud((*iterColonneNulle)->getNodeID()) ;
      currentMsg.SetTreeID((*iterColonneNulle)->getDoc()) ;

      // Cas particulier : si sEtiquettePatpatho contient £C prendre les données
      // dans l'élément suivant de la patpatho qui, lui (controle fictif)
      // contient les vraies données à afficher.
      // Exemple en Echo : Chimiothérapie
      if ((sEtiquettePatpatho.find(string("£C;"))  != string::npos) ||
          (sEtiquettePatpatho.find(string("/£C;")) != string::npos) ||
          (sEtiquettePatpatho.find(string("£CX"))  != string::npos))
      {
        k = iterColonneNulle ;
        k++ ;
        if (_pPPTEnCours->end() != k)
        {
          currentMsg.getContent(*k) ;
          currentMsg.SetNoeud(currentMsg.GetNoeud() + string(1,cheminSeparationMARK) + (*k)->getNodeID()) ;
        }
      }
      else
        currentMsg.getContent(*iterColonneNulle) ;

      ligneNulle   = (*iterColonneNulle)->getLigne() ;
      ColonneNulle = (*iterColonneNulle)->getColonne() ;

      iterColonneNonNulle = iterColonneNulle ;
      iterColonneNonNulle++ ;
      if ((_pPPTEnCours->end() != iterColonneNonNulle) &&
          ((*iterColonneNonNulle)->getColonne() == ORIGINE_PATH_PATHO))
        iterColonneNonNulle = iterColonneNulle ;

      if (_pPPTEnCours->end() != iterColonneNonNulle)
      {
        colonneNonNulle = (*iterColonneNonNulle)->getColonne() ;
        ligneNonNulle   = (*iterColonneNonNulle)->getLigne() ;
      }

      // L'unité est inscrite en premier
      if (string("") != currentMsg.GetUnit())
        sEtiquettePatpatho = currentMsg.GetUnit() + string(1,cheminSeparationMARK) + sEtiquettePatpatho ;

      string sEtiquetteBrute = sEtiquettePatpatho ; // sans certitude et pluriel

      // L'étiquette pourrait être du type : sEtiquettePatpatho + sPluriel + sCertitude ;
      //
      if (string("") != currentMsg.GetPluriel())
        sEtiquettePatpatho += string(1, cheminSeparationMARK) + currentMsg.GetPluriel() ;
      if (string("") != currentMsg.GetCertitude())
        sEtiquettePatpatho += string(1, cheminSeparationMARK) + currentMsg.GetCertitude() ;

      motcompose = sEtiquettePatpatho ;

      // Trouver le fils ayant la bonne étiquette
      //
      BBFilsItem* pFilsTrouve = TrouverFilsAyantBonneEtiquette(sEtiquettePatpatho, currentMsg.GetComplement()) ;

      // Si on trouve un fils , on l'active et on lui donnee ses données (vides ou non )
      //
      if (pFilsTrouve)
      {
        Message* pFilsMessage = pFilsTrouve->getItemTransfertMsg() ;
        if (pFilsMessage)
          *pFilsMessage = currentMsg ;

        iterColonneNonNulle = DispatcherPatPathoForZeroSon(pFilsTrouve, &currentMsg, iterColonneNulle, iterColonneNonNulle, sEtiquettePatpatho, motcompose, ligneNulle, &decalageNonNul) ;

        pFilSuivant = 0 ;
      }
      // Si on n'a pas trouvé de fils, c'est certainement un +00+00
      // Attention, les textes libres amènent artificiellement un +00+00
      // supplémentaire, à cause du fil guide */##### -+00+00-> #TLI#1
      //
      else
      {
        // In case there is several possible sons and further inquiry needed
        //
        int iFoundSons = 0 ;

        for (Iter = _aBBItemFils.begin() ; _aBBItemFils.end() != Iter ; Iter++)
        {
          if ((*Iter)->estPlusZero())
          {
            if ((*Iter)->getItemData()->getSonsList().find(sEtiquetteBrute) != NPOS)
            {
              decalageNonNul = 0 ;

              (*Iter)->Active() ; //rendre ce fils actif

              Message* pFilsMessage = (*Iter)->getItemTransfertMsg() ;
              if (pFilsMessage)
                pFilsMessage->getContent(currentMsg) ;

              pFilSuivant = 1 ;
              NSPatPathoArray PatpathoFils(pSuper) ;
              patPathoFilsNonvide = false ;
              iterColonneNonNulle = iterColonneNulle ;
              ligneNonNulle = ligneNulle ;

              iterColonneNonNulle = GetSubPathoForSon(&PatpathoFils, &currentMsg, iterColonneNonNulle, motcompose, ligneNonNulle, false) ;
              pFilSuivant = 0 ;

              if ((_aBBItemFils.end() != Iter) && (false == PatpathoFils.empty()))
              {
                NSVectFatheredPatPathoArray* pPatpatho = (*Iter)->getPatPatho() ;
                if (false == pPatpatho->empty())
                {
                  NSPatPathoArray* ContenuPatpatho = (*(pPatpatho->begin()))->getPatPatho() ;

                  // Si ContenuPatpatho existe, mettre pPatpathoFils à la suite
                  if (false == ContenuPatpatho->empty())
                    _pBigBoss->ConcatenerPatpatho(&PatpathoFils, ContenuPatpatho, false) ;
                  // Sinon, mettre pPatpathoFils à la place
                  else
                    pPatpatho->push_back(new NSFatheredPatPathoArray(pSuper, new NSPatPathoInfo(**iterColonneNulle), new NSPatPathoArray(PatpathoFils))) ;
                }
                else
                  pPatpatho->push_back(new NSFatheredPatPathoArray(pSuper, new NSPatPathoInfo(**iterColonneNulle), new NSPatPathoArray(PatpathoFils))) ;
              }
              pFilsTrouve = *Iter ;

              iFoundSons++ ;
            }
          }
        }

        if (iFoundSons > 1)
        {
          pFilsTrouve = SelectProperPath() ;

          // Remove patpatho from impostors
          //
          if (pFilsTrouve)
          {
            for (Iter = _aBBItemFils.begin() ; _aBBItemFils.end() != Iter ; Iter++)
            {
              if (*Iter != pFilsTrouve)
              {
                (*Iter)->Desactive() ; //rendre ce fils actif

                NSVectFatheredPatPathoArray* pPatpatho = (*Iter)->getPatPatho() ;
                if (pPatpatho)
                  pPatpatho->vider() ;
              }
            }

            pFilsTrouve->Active() ;
          }
        }

        // Si on n'a toujours pas trouvé de fils, c'est que c'est une série de +00+00
        if ((BBFilsItem*) NULL == pFilsTrouve)
        {
          // bool bTrouveSuite = false ;
          for (Iter = _aBBItemFils.begin() ; _aBBItemFils.end() != Iter ; Iter++)
          {
          	bool bTrouveSuite = false ;

            if ((*Iter)->estPlusZero())
            {
              string sFilsLabel = (*Iter)->getItemLabel() ;
              string sSens      = string("") ;
              NSDico::donneCodeSens(&sFilsLabel, &sSens) ;
              string sLocalChem = _sLocalisation + string(1, cheminSeparationMARK) + sSens ;

              string sSons = (*Iter)->getItemData()->getSonsList() ;

              VecteurRechercheSelonCritere VecteurSelonCritere(GUIDE) ;
              for (int i = 0 ; ('\0' != sSons[i]) && (' ' != sSons[i]) ; i++)
              {
                string sEtiquet  = string("") ;
                string sCodeSens = string("") ;

                // Prise de l'étiquette
                for ( ; ('\0' != sSons[i]) && (nodeSeparationMARK != sSons[i]) ; i++)
                  sEtiquet += sSons[i] ;

                // traiter le cas des textes libres à part

                //if ((!(sEtiquet == string("#####1"))) && (!(sEtiquet == string("#####2"))))
                //{
                  NSDico::donneCodeSens(&sEtiquet, &sCodeSens) ;
                  VecteurSelonCritere.AjouteEtiquette(sCodeSens) ;
                //}
                // Attention à la double boucle qui fait sauter par dessus le '\0'
                if ('\0' == sSons[i])
                  break ;
              }
              if (false == VecteurSelonCritere.empty())
              {
                pSuper->afficheStatusMessage("Recherche des fils guides +00+00") ;
                pSuper->getFilGuide()->chercheChemin(&sLocalChem, &VecteurSelonCritere, NSFilGuide::compReseau) ;

                for (IterCritere iCrit = VecteurSelonCritere.begin() ; (VecteurSelonCritere.end() != iCrit) && !bTrouveSuite ; iCrit++)
                {
                  RechercheSelonCritereData* pRechItem = (RechercheSelonCritereData *)(*iCrit) ;
                  if (((*iCrit)->trouve) &&
                      (string("+00+00") == pRechItem->pDonnees->getLevelShift()) &&
                      (string::npos != string(pRechItem->pDonnees->fils).find(sEtiquetteBrute)))
                    bTrouveSuite = true ;
                }
              }
            }
            if (bTrouveSuite)
            {
              decalageNonNul = 0 ;
              (*Iter)->Active() ; //rendre ce fils actif
              Message* pFilsMessage = (*Iter)->getItemTransfertMsg() ;
              if (pFilsMessage)
                pFilsMessage->getContent(currentMsg) ;

              pFilSuivant = 1 ;
              NSPatPathoArray PatpathoFils(pSuper) ;
              patPathoFilsNonvide = false ;
              iterColonneNonNulle = iterColonneNulle ;
              ligneNonNulle = ligneNulle ;

              iterColonneNonNulle = GetSubPathoForSon(&PatpathoFils, &currentMsg, iterColonneNonNulle, motcompose, ligneNonNulle, false) ;
              pFilSuivant = 0 ;

              if ((_aBBItemFils.end() != Iter) && (false == PatpathoFils.empty()))              {
                NSVectFatheredPatPathoArray* pPatpatho = (*Iter)->getPatPatho() ;
                if (false == pPatpatho->empty())
                {
                  NSPatPathoArray* pContenuPatpatho = (*(pPatpatho->begin()))->getPatPatho() ;

                  // Si ContenuPatpatho existe, mettre pPatpathoFils à la suite
                  if (false == pContenuPatpatho->empty())
                    _pBigBoss->ConcatenerPatpatho(&PatpathoFils, pContenuPatpatho, false) ;
                  // Sinon, mettre pPatpathoFils à la place
                  else
                    pPatpatho->push_back(new NSFatheredPatPathoArray(pSuper, new NSPatPathoInfo(**iterColonneNulle), new NSPatPathoArray(PatpathoFils))) ;
                }
                else
                  pPatpatho->push_back(new NSFatheredPatPathoArray(pSuper, new NSPatPathoInfo(**iterColonneNulle), new NSPatPathoArray(PatpathoFils))) ;
              }
              pFilsTrouve = *Iter ;
            }
          }
        }
      }

      pFilSuivant = 1 ; // on passe au pfils suivant ou non
      patPathoFilsNonvide = false ;
      if ((_pPPTEnCours->end() != iterColonneNonNulle) && (decalageNonNul))
      {
        NSPatPathoArray PatpathoFils(pSuper) ;

        iterTemp = iterColonneNonNulle ;
        // Si mot composé
        if ((ligneNonNulle == ligneNulle) && (colonneNonNulle != ColonneNulle))
          iterTemp++ ;

        // Si la colonne du fils suivant est non nulle et  ce fils existe
        if ((_pPPTEnCours->end() != iterTemp) &&
            ((*iterTemp)->getColonne() > ORIGINE_PATH_PATHO))
        {
          // On passe à l'item suivant
          iterColonneNonNulle = iterTemp ;

          if (pFilSuivant)
          {
            iterColonneNonNulle = GetSubPathoForSon(&PatpathoFils, &currentMsg, iterColonneNonNulle, motcompose, ligneNonNulle, true) ;
            pFilSuivant = 0 ;
          }
        }
        // Mise à jour du patPatho du fils
        // if (pFilsTrouve && patPathoFilsNonvide)
        if (pFilsTrouve && (false == PatpathoFils.empty()))
          (pFilsTrouve->getPatPatho())->push_back(new NSFatheredPatPathoArray(pSuper, new NSPatPathoInfo(**iterColonneNulle), new NSPatPathoArray(PatpathoFils)));
      }
    }
    // Cas de mot composé : exemple : MAMPL/2M001/£N0;01
    for (Iter = _aBBItemFils.begin() ; (_aBBItemFils.end() != Iter) &&
                              ((*Iter)->getItemLabel() != motcompose) ; Iter++)
      ;
    if (_aBBItemFils.end() != Iter)
    {
      (*Iter)->Active() ; // rendre ce fils actif et lui attribuer ses données
      Message* pFilsMessage = (*Iter)->getItemTransfertMsg() ;
      if (pFilsMessage)
        *pFilsMessage = currentMsg ;
    }
    if (_pPPTEnCours->end() != iterColonneNonNulle)
      iterColonneNulle = iterColonneNonNulle ;

		// Elément suivant
		if (_pPPTEnCours->end() != iterColonneNulle)
      iterColonneNulle++ ;
	}

  // Now we have to check that self excluding sons are not selected
  //
  creerExclus() ;
  DispatcherPatPathoArbitrateExcluded() ;
}
catch (...)
{
  erreur("Exception BBItem::DispatcherPatPatho.", standardError, 0) ;
}
}

PatPathoIter
BBItem::DispatcherPatPathoForZeroSon(BBFilsItem* pFilsTrouve, Message* pMsg, PatPathoIter iterColonneNulle, PatPathoIter iterColNonNulle, const string sEtiquettePatpatho, string &sMotCompose, const int iLigneNulle, int *pDecalageNonNul)
{
  if ((BBFilsItem*) NULL == pFilsTrouve)
    return iterColNonNulle ;

  pFilsTrouve->Active() ; // rendre ce fils actif
  if (1 == (*iterColonneNulle)->_ID)    pFilsTrouve->bCorriger = true ;

  if (false == pFilsTrouve->estPlusZero())
    return iterColNonNulle ;

  if (pFilsTrouve->getItemData()->getSonsList().find(sEtiquettePatpatho) == string::npos)
    return iterColNonNulle ;

  if (pDecalageNonNul)
    *pDecalageNonNul = 0 ;

  NSSuper* pSuper = pContexte->getSuperviseur() ;

  NSPatPathoArray PatpathoFils(pSuper) ;

  PatPathoIter iterColonneNonNulle = GetSubPathoForSon(&PatpathoFils, pMsg, iterColonneNulle, sMotCompose, iLigneNulle, false) ;

  if (pFilsTrouve && (false == PatpathoFils.empty()))
  {
    NSPatPathoArray* pNewPpt = new NSPatPathoArray(PatpathoFils) ;
    (pFilsTrouve->getPatPatho())->push_back(new NSFatheredPatPathoArray(pSuper, new NSPatPathoInfo(**iterColonneNulle), pNewPpt)) ;
  }

  return iterColonneNonNulle ;
}

PatPathoIter
BBItem::GetSubPathoForSon(NSPatPathoArray* pPatpathoFils, Message* pMsg, PatPathoIter iterColNonNulle, string &sMotCompose, const int iLigneNulle, bool bDecalCol)
{
  if ((NSPatPathoArray*) NULL == pPatpathoFils)
    return iterColNonNulle ;

  PatPathoIter iterColonneNonNulle = iterColNonNulle ;

  int iLigneNonNulle = iLigneNulle ;

  bool bFilSuivant = true ;
  while ((_pPPTEnCours->end() != iterColonneNonNulle) && (bFilSuivant))
  {
    NSPatPathoInfo PatPatho(**iterColonneNonNulle) ;

    // Placer l'élément dont la colonne est nulle
    if (bDecalCol)
      MiseAjourPatPatho(pPatpathoFils, &PatPatho, iLigneNonNulle) ;
    else
      MiseAjourPatPathoZero(pPatpathoFils, &PatPatho, iLigneNonNulle) ;

    string sUnit = (*iterColonneNonNulle)->getUnit() ;
    if (string("") != sUnit)
      sMotCompose += string(1, cheminSeparationMARK) + sUnit ;

    // Former éventuellement le mot composé
    sMotCompose += string(1, cheminSeparationMARK) + (*iterColonneNonNulle)->getLexique() ;

    if (pMsg)
      pMsg->getContent(*iterColonneNonNulle) ;

    PatPathoIter k = iterColonneNonNulle ;
    k++ ;

    //test pour l'élément suivant
    if ((_pPPTEnCours->end() != k) && ((*k)->getColonne() > ORIGINE_PATH_PATHO))
      iterColonneNonNulle++ ;
    else
      bFilSuivant = false ;
  } //while

  return iterColonneNonNulle ;
}

BBFilsItem*
BBItem::SelectProperPath()
{
  if (_aBBItemFils.empty())
    return (BBFilsItem*) 0 ;

  int iNbFound = 0 ;

  BBFilsItem* pFoundSon = (BBFilsItem*) 0 ;

  for (BBiter Iter = _aBBItemFils.begin() ; _aBBItemFils.end() != Iter ; Iter++)
  {
    if ((*Iter)->isActif())
    {
      if ((*Iter)->VectorFils.empty())
        (*Iter)->creerFils() ;

      if (false == (*Iter)->VectorFils.empty())
      {
        for (BBiterFils it = (*Iter)->VectorFils.begin() ; (*Iter)->VectorFils.end() != it ; it++)
        {
          // Don't do that because it doesn't handle the +00+00 elements
          //
          // (*it)->_sLocalisation = (*Iter)->getLocalisation() ;

          // Better do that
          //
          donneGenetique(*it, (*Iter)->getItemLabel()) ;

          // But donneGenetique sets _sNomDlg and when it is set, then creer will
          // automatically create all the BBItems and BBFilsItem for the dialog,
          // though we just want to go manually to the point where we can separate
          // candidate paths
          //
          (*it)->_sNomDlg = string("") ;


          (*it)->creer() ;
          BBFilsItem* pFilsItem = (*it)->findSonToEdit() ;

          if (pFilsItem)
          {
            iNbFound++ ;
            pFoundSon = *Iter ;
          }
        }
      }
    }
  }

  if (0 == iNbFound)
    return (BBFilsItem*) 0 ;

  if (1 == iNbFound)
  {
    for (BBiter Iter = _aBBItemFils.begin() ; _aBBItemFils.end() != Iter ; Iter++)
    {
      if (*Iter != pFoundSon)
        (*Iter)->Desactive() ;
    }

    return pFoundSon ;
  }

  // Still there... means we still couldn't find the actual path... iterate
  //
  iNbFound = 0 ;

  for (BBiter Iter = _aBBItemFils.begin() ; _aBBItemFils.end() != Iter ; Iter++)
  {
    if ((*Iter)->isActif())
    {
      if (false == (*Iter)->VectorFils.empty())
      {
        for (BBiterFils it = (*Iter)->VectorFils.begin() ; (*Iter)->VectorFils.end() != it ; it++)
        {
          BBFilsItem* pFilsItem = (*it)->findSonToEdit() ;
          if (pFilsItem)
          {
            pFilsItem->creerFils() ;

            if (false == (*Iter)->VectorFils.empty())
            {
              for (BBiterFils it = (*Iter)->VectorFils.begin() ; (*Iter)->VectorFils.end() != it ; it++)
              {
                // See comment above for these two lines
                //
                donneGenetique(*it, (*Iter)->getItemLabel()) ;
                (*it)->_sNomDlg = string("") ;

                BBFilsItem* pFoundGrandSon = (*it)->SelectProperPath() ;
                if (pFoundGrandSon)
                {
                  pFoundSon = *Iter ;
                  iNbFound++ ;
                }
              }
            }
          }
        }
      }
    }
  }

  if (0 == iNbFound)
    return (BBFilsItem*) 0 ;

  if (1 == iNbFound)
  {
    for (BBiter Iter = _aBBItemFils.begin() ; _aBBItemFils.end() != Iter ; Iter++)
    {
      if (*Iter != pFoundSon)
        (*Iter)->Desactive() ;
    }

    return pFoundSon ;
  }

  return (BBFilsItem*) 0 ;
}

void
BBItem::DispatcherPatPathoArbitrateExcluded()
{
  if (_aBBItemFils.empty())
    return ;

  for (BBiter Iter = _aBBItemFils.begin() ; _aBBItemFils.end() != Iter ; Iter++)
  {
    BBiter IterBro = Iter ;
    IterBro++ ;
    while (((*Iter)->isActif()) && (_aBBItemFils.end() != IterBro))
    {
      if (((*IterBro)->isActif()) && (isExcluded(*Iter, *IterBro)))
        arbitrateExcluded(*Iter, *IterBro) ;
      IterBro++ ;
    }
  }
}

void
BBItem::arbitrateExcluded(BBFilsItem *pFilsIt1, BBFilsItem *pFilsIt2)
{
  if (((BBFilsItem*) NULL == pFilsIt1) || ((BBFilsItem*) NULL == pFilsIt2))
    return ;

  // So far, the arbitration is made on:
  // - the size of the patpatho
  // - if equal, the order (first remains)
  //
  NSVectFatheredPatPathoArray* pFatheredPptArray1 = pFilsIt1->getPatPatho() ;
  NSVectFatheredPatPathoArray* pFatheredPptArray2 = pFilsIt2->getPatPatho() ;

  // Is one of both arrays empty?
  //
  if (((NSVectFatheredPatPathoArray*) NULL == pFatheredPptArray2) || (pFatheredPptArray2->estVide()))
  {
    pFilsIt2->Desactive() ;
    return ;
  }
  if (((NSVectFatheredPatPathoArray*) NULL == pFatheredPptArray1) || (pFatheredPptArray1->estVide()))
  {
    pFilsIt1->Desactive() ;
    return ;
  }

  // Evaluate the size of collected information
  //
  FatheredPatPathoIterVect iter ;

  size_t iSize1 = 0 ;
	for (iter = pFatheredPptArray1->begin(); pFatheredPptArray1->end() != iter ; iter++)
    if ((*iter)->getPatPatho() && (false == (*iter)->getPatPatho()->empty()))
      iSize1 += (*iter)->getPatPatho()->size() ;

  size_t iSize2 = 0 ;
	for (iter = pFatheredPptArray2->begin(); pFatheredPptArray2->end() != iter ; iter++)
    if ((*iter)->getPatPatho() && (false == (*iter)->getPatPatho()->empty()))
      iSize2 += (*iter)->getPatPatho()->size() ;

  if (iSize1 > iSize2)
  {
    pFilsIt2->Desactive() ;
    return ;
  }
  if (iSize2 > iSize1)
  {
    pFilsIt1->Desactive() ;
    return ;
  }

  // No valid criterion remains... first one wins
  //
  pFilsIt2->Desactive() ;

  return ;
}

// -----------------------------------------------------------------------------
// Description  : Pour chaque fils "actif" (qui correspond à une étiquette), on
//                cherche la fiche Paradox correspondante et, si elle intéresse
//                la même fenêtre, on crée une BBItem
// Returns      : 0 si OK et 1 si le champ exclu est non traitable
// -----------------------------------------------------------------------------
int
BBItem::creerExclus()
{
try
{
  string sSonsRules = _Data.getSonsRules() ;

  // Si la liste d'exclusion ou la liste des fils sont vides, on sort
  //
  strip(sSonsRules, stripBoth) ;
	if (string("") == sSonsRules)
		return 0 ;

	if (_aBBItemFils.empty())
		return 0 ;

	int	i = 0 ;

  string exclueur, exclus ;

	// Cas particulier : exclusion complète
  //
  if (('(' == _Data.exclusion[0]) && (')' == _Data.exclusion[1]))
	{
    // Pour chacun des fils
    for (BBiter Iter = _aBBItemFils.begin() ; _aBBItemFils.end() != Iter ; Iter++)
    {
      // On ajoute à sa liste d'exclusion toutes les autres étiquettes
      for (BBiter Jter = _aBBItemFils.begin() ; _aBBItemFils.end() != Jter ; Jter++)
        if ((*Jter)->getItemLabel() != (*Iter)->getItemLabel())
          (*Iter)->Exclusion.addExclusion((*Jter)->getItemLabel()) ;
    }
		i = 2 ;
  }

	// Analyse du champ exclusion
	for ( ; '\0' != _Data.exclusion[i] ; i++)
	{
		// on cherche une parenthèse ouvrante
		for ( ; ('\0' != _Data.exclusion[i]) && ('(' != _Data.exclusion[i]) ; i++)
      ;
		if ('(' != _Data.exclusion[i])
			return 0 ;
		i++ ;

		// Les caractères après la parenthèse donnent l'étiquette de l'exclueur
    //
    string exclueur = string("") ;
    for ( ; ('\0' != _Data.exclusion[i]) && (')' != _Data.exclusion[i]) &&
                                            ('-' != _Data.exclusion[i]) &&
                                            ('+' != _Data.exclusion[i]) ; i++)
      exclueur += _Data.exclusion[i] ;

		// Recherche de son indice
    //
    BBiter excluIter = _aBBItemFils.begin() ;
    for ( ; (_aBBItemFils.end() != excluIter) &&
                       ((*excluIter)->getItemLabel() != exclueur) ; excluIter++)
      ;

		if (_aBBItemFils.end() == excluIter)
			return 1 ;

    char chOperator = _Data.exclusion[i] ;

		// L'exclueur est forcément suivi d'un '-' ou d'un '+'
		if (('-' != chOperator) && ('+' != chOperator))
			return 1 ;

		i++ ;

		// après le tiret, une parenthèse fermante signifie que l'exclueur "opère" tous les autres
    //
		if (')' == _Data.exclusion[i])
		{
      string sExcluLabel = (*excluIter)->getItemLabel() ;

			// On ajoute ou retranche l'exclueur à la liste des autres
			// et chacun des autres à la liste de l'exclueur
      for (BBiter Iter = _aBBItemFils.begin() ; _aBBItemFils.end() != Iter ; Iter++)
      {
        if ((*Iter)->getItemLabel() != sExcluLabel)
        {
          (*Iter)->Exclusion.operateExclusion(chOperator, sExcluLabel) ;
          (*excluIter)->Exclusion.operateExclusion(chOperator, (*Iter)->getItemLabel()) ;
        }
      }
    }
		else
		{
			// chaque exclus est séparé par une virgule jusqu'à la parenthèse fermante
			for ( ; ('\0' != _Data.exclusion[i]) && (')' != _Data.exclusion[i]) ; )
			{
        string exclus = string("") ;
        for ( ; ('\0' != _Data.exclusion[i]) && (')' != _Data.exclusion[i]) && (nodeSeparationMARK != _Data.exclusion[i]) ; i++)
          exclus += _Data.exclusion[i] ;

				// Recherche de son indice
        BBiter Iter = _aBBItemFils.begin() ;
				for ( ; (_aBBItemFils.end() != Iter) && ((*Iter)->getItemLabel() != exclus) ; Iter++)
          ;

				if (_aBBItemFils.end() == Iter)					return 1 ;

				// Ajout de l'exclu à la liste de l'exclueur et vice versa
        (*Iter)->Exclusion.operateExclusion(chOperator, (*excluIter)->getItemLabel()) ;
        (*excluIter)->Exclusion.operateExclusion(chOperator, (*Iter)->getItemLabel()) ;

        // On avance d'un cran
				if (nodeSeparationMARK == _Data.exclusion[i])
					i++ ;
			}
		}
    if ('\0' == _Data.exclusion[i])
      break ;
	}
  return 0 ;
}
catch (...)
{
  erreur("Exception BBItem::creerExclus.", standardError, 0) ;
  return 0 ;
}
}

// -----------------------------------------------------------------------------
// Function     : int BBItem::creerFils()
// Arguments    :	code -> codon d'initialisation
// Description  : Pour chaque fils "actif" (qui correspond à une étiquette), on
//                cherche la fiche Paradox correspondante et, si elle intéresse
//                la même fenêtre, on crée une BBItem
// -----------------------------------------------------------------------------
int
BBItem::creerFils()
{
try
{
  string  sFutureLocalisation = string("") ;
  string  sCodeSens ;

	// Si le fil guide n'est pas lié à une boite de dialogue, on sort.
  if ((false == lienDialogue()) || (_aBBItemFils.empty()))
    return 0 ;

	// On passe en revue tous les Fils déclarés de la BBItem,
	for (BBiter it = _aBBItemFils.begin() ; _aBBItemFils.end() != it ; it++)
	{
    if ((*it)->FilsProlongeable)
		{
      // On ne crée un BBItem à partir de la fiche Paradox que si l'élément
      // concerne la même boite de dialogue et que l'élément n'est pas une feuille.
      string sDialogName = (*it)->getItemData()->getDialogName() ;

      if ((*it)->getItemData()->isDialogNameIdem() ||
          ((_Data.getDialogName() == sDialogName) &&
                          (false == (*it)->getItemData()->isDialogNameAuto()))
         )
      {
        // Création d'un BBItem
        (*it)->creerFils() ;

        // Initialisation du BBItem
        // attribuer la patpthoarray correspondante à ce pFilsItem
        // si pFilsItem->sEtiquette trouvée
        string sEtiquette = (*it)->getItemLabel() ;
        if (false == (*it)->VectorFils.empty())
        {
          for (BBiterFils iter = (*it)->VectorFils.begin() ; (*it)->VectorFils.end() != iter ; iter++)
          {
            donneGenetique(*iter, sEtiquette) ;

            // On demande au BBItem de se créer
            (*iter)->creer() ;
          }
        }
      }
    }
  }
	return 0 ;
}
catch (...)
{
  erreur("Exception BBItem::creerFils.", standardError, 0) ;
  return 0 ;
}
}

// -----------------------------------------------------------------------------
// Function     : int BBItem::creerFils()
// Description  : Pour chaque fils, on cherche le Citem correspondant dans
//                l'archetype et, si il ne s'agit pas d'une feuille, on crée un
//                BBItem
// -----------------------------------------------------------------------------
int
BBItem::creerFilsArchetype(){try{  NSSuper* pSuper = pContexte->getSuperviseur() ;  string sMsg = "creerFilsArchetype() : Entrée" ;  pSuper->trace(&sMsg, 1, NSSuper::trSubDetails) ;  if (_aBBItemFils.empty())    return 0 ;  // On passe en revue tous les Fils déclarés de la BBItem,  for (BBiter it = _aBBItemFils.begin() ; _aBBItemFils.end() != it ; it++)
  {
    // si le Citem correspondant n'est pas une feuille
    if (((*it)->pCitem) && ((*it)->pCitem->getArrayFils()) && (false == (*it)->pCitem->getArrayFils()->empty()))
    {
      sMsg = "creerFilsArchetype() : Appel de creerFils" ;
      pSuper->trace(&sMsg, 1, NSSuper::trSubDetails) ;

      // Création d'un BBItem
      (*it)->creerFils() ;

      // Initialisation du BBItem
      //attribuer la patpathoarray correspondante à ce pFilsItem (donneGenetique)      //si pFilsItem->sEtiquette trouvée
      string sEtiquette = (*it)->getItemLabel() ;

      sMsg = "creerFilsArchetype() : Etiquette " + sEtiquette ;
      pSuper->trace(&sMsg, 1, NSSuper::trSubDetails) ;

      // Chaque BBFilsItem a un vecteur de BBItem mais en général, ce vecteur
      // contient un seul BBItem.
      if (false == (*it)->VectorFils.empty())
      {
        for (BBiterFils iter = (*it)->VectorFils.begin() ; iter != (*it)->VectorFils.end() ; iter++)        {
          sMsg = "creerFilsArchetype() : Avant donneGenetique()" ;
          pSuper->trace(&sMsg, 1, NSSuper::trSubDetails) ;

          donneGenetique(*iter, sEtiquette) ;

          sMsg = "creerFilsArchetype() : Après donneGenetique() " + (*iter)->_sLocalisation ;
          pSuper->trace(&sMsg, 1, NSSuper::trSubDetails) ;

          // On demande au BBItem de se créer par l'archetype en repartant du Citem du fils          (*iter)->creerArchetype((*it)->pCitem) ;
        }
      }
    }
  }

  sMsg = "creerFilsArchetype() : Sortie" ;
  pSuper->trace(&sMsg, 1, NSSuper::trSubDetails) ;

  return 1 ;
}catch (...){
  erreur("Exception BBItem::creerFilsArchetype.", standardError, 0) ;
  return 0 ;
}
}
// -----------------------------------------------------------------------------// mise à jour du patpatho dans les cas ://    - Si le BBItem est rattaché purement à une fonction
//		- Si le BBItem n'est pas rattaché à une boite de dialogue, il active sa
// 			première branche si il est en mode "automatique", il propose ses fils si
// 			il est en mode "manuel"
// -----------------------------------------------------------------------------
void
BBItem::MiseAjourPatpatho()
{
try
{
	if ((NSPatPathoArray*) NULL == _pPPTEnCours)
		return ;

/*
	NSPatPathoArray PPTMissingConcepts(pBigBoss->pContexte) ;
	if ((NULL != pParseur) && (false == pPPTEnCours->empty()))
  	PPTMissingConcepts = *pPPTEnCours ;
*/

	_pPPTEnCours->vider() ;

	if (_aBBItemFils.empty())
		return ;

	int colonne = 0 ;

	VecteurRechercheSelonCritere VecteurSelonCritere(GUIDE) ;
	bool rechercher = false ;

  NSSuper* pSuper = pContexte->getSuperviseur() ;

	for (BBiter it = _aBBItemFils.begin() ; _aBBItemFils.end() != it ; it++)
	{
		// On fabrique le chemin indiqué par le futur élément
		if (((BBItemData*) NULL == (*it)->getItemData()) ||
                       (string("") == (*it)->getItemData()->getSemanticPath()))
		{
      string sCodeSens ;
      string sFilsLabel = (*it)->getItemLabel() ;
			NSDico::donneCodeSens(&sFilsLabel, &sCodeSens) ;
			VecteurSelonCritere.AjouteEtiquette(sCodeSens) ;
			rechercher = true ;
		}
	}
	if (rechercher)
		pSuper->getFilGuide()->chercheChemin(&_sLocalisation, &VecteurSelonCritere, NSFilGuide::compReseau) ;

	bool trouve ;
	for (BBiter Iter = _aBBItemFils.begin() ; _aBBItemFils.end() != Iter ; Iter++)
	{
    string sCodeSens ;
    string sFilsLabel = (*Iter)->getItemLabel() ;
		NSDico::donneCodeSens(&sFilsLabel, &sCodeSens) ;

		if (rechercher && (((BBItemData*) NULL == (*Iter)->getItemData()) ||
                       (string("") == (*Iter)->getItemData()->getSemanticPath())))
			VecteurSelonCritere.SetData(sCodeSens, &trouve, (*Iter)->getItemData()) ;
		else
			trouve = true ;

		if (trouve)
		{
			if ((*Iter)->isActif())
			{
				colonne = 0 ;
				NSVectFatheredPatPathoArray* pPatpathoItem = (*Iter)->getPatPatho() ;

				if ((*Iter)->getItemData()->uniciteLesion()) //non multidialogue
				{
					if (false == (*Iter)->estPlusZero())
					{
						colonne = 1 ;
						AjouteEtiquette(*Iter) ;
					}
					if (pPatpathoItem && (false == pPatpathoItem->empty()))
					{
						for (FatheredPatPathoIterVect j = pPatpathoItem->begin() ; pPatpathoItem->end() != j ; j++)
						{
							_pPPTEnCours->ajouteVecteur((*j)->getPatPatho(), ORIGINE_PATH_PATHO + colonne) ;
							colonne = 1 ;
						}
					}
				}
				else
				{
          // si Iter est multidialogue, il faut dupliquer Iter autant de fois
          // qu'il a été crée en tant que multidialogue.
          // Exemple : this est PPAT
          //  Iter = PINQ9 (multidialogue)
          //    avec le patpatho suivantes : 	1) PIVG(0,0)
          //														      2) PIVD(0,0)
          //	on aura alors pour PPATT
          //     PINQ9(0,0)
          //        PIVG(1,1)
          //     PINQ9(2,0)
          //        PIVD(3,1)
					if (false == (*Iter)->estPlusZero())
					{
          	if (pPatpathoItem && (false == pPatpathoItem->empty()))
						{
							for (FatheredPatPathoIterVect j = pPatpathoItem->begin() ; pPatpathoItem->end() != j ; j++)
							{
								AjouteEtiquette(*Iter, string(""), (*j)->getFatherNode(), true) ;
								_pPPTEnCours->ajouteVecteur((*j)->getPatPatho(), ORIGINE_PATH_PATHO + 1) ;
							}
            }
					}
					else
					{
						colonne = 0 ;
						if (pPatpathoItem && (false == pPatpathoItem->empty()))
						{
							for (FatheredPatPathoIterVect j = pPatpathoItem->begin() ; pPatpathoItem->end() != j ; j++)
							{
								_pPPTEnCours->ajouteVecteur((*j)->getPatPatho(), ORIGINE_PATH_PATHO + colonne) ;
								colonne = 1 ;
							}
						}
					}
				}
			}
		}
	}

/*
	if (false == PPTMissingConcepts.empty())
	{
    // Killing all leaves that still exist in the new patpatho
    //
    // We suppose that this job has already been done by other archetypes
    // related BBItems, so that leaves that are controled by sub-levels have
    // already been managed
    //
    PatPathoIter it = PPTMissingConcepts.begin() ;
    for (; PPTMissingConcepts.end() != it; )
    {
    	// Is it a leaf?
      //
      PatPathoIter itFils = PPTMissingConcepts.ChercherPremierFils(it) ;
      if ((NULL == itFils) || (PPTMissingConcepts.end() == itFils))
      {
      	string sItemPath = PPTMissingConcepts.donneCheminItem(it) ;
        if (true == pPPTEnCours->CheminDansPatpatho(sItemPath))
        {
        	delete *it ;
          PPTMissingConcepts.erase(it) ;
        }
        else
        	it++ ;
      }
      else
      	it++ ;
    }
    //
    // Now that we have all the missing elements, we have to distinguish between
    // those nodes that were deleted (because they were available in the
    // archetype OR are "excluded" by other nodes available in the archetype)
    // and those nodes that are not handled by the archetype (we have to add
    // them back)
    //

  }
*/
}
catch (...)
{
	erreur("Exception BBItem::MiseAjourPatpatho.", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------
// lors de la correction d'un CR le BBitem développe la boîte qui contient la phrase à corriger
// -----------------------------------------------------------------------------
void
BBItem::Corriger()
{
  if (_aBBItemFils.empty())
    return ;

  // Trouver le fils ayant dans sa patpatho l'élément d'ID  = 1
  for (BBiter Iter = _aBBItemFils.begin() ; _aBBItemFils.end() != Iter ; Iter++)
  {
    if (false == (*Iter)->PatPtahovide())
    {
      NSVectFatheredPatPathoArray* pPatpathoItem = (*Iter)->getPatPatho() ;
      FatheredPatPathoIterVect     iterPath      = pPatpathoItem->begin() ;
      for ( ; pPatpathoItem->end() != iterPath ; iterPath++)
      {
        NSPatPathoArray* pPatPatho = (*iterPath)->getPatPatho() ;
        for (PatPathoIter it = pPatPatho->begin() ; pPatPatho->end() != it ; it++)
        {
          if (1 == (*it)->_ID)
          {
            int k = developper(*Iter) ;
            if (k == 3)
            {
              BBItem* pbbitem = *((*Iter)->VectorFils.begin()) ;
              //Iter en tant que BBItem
              if (pbbitem)
                pbbitem->activer() ;
            }
            MiseAjourPatpatho() ;
          }
        }
      }
    }
  }
}

// -----------------------------------------------------------------------------// Function     : int BBItem::activer()// Description  : Demande à la BBItem de se lancer : créer la boite de dialogue
//                ou lancer un processus
//						    Il serait tentant de gérer à ce niveau la génération/mise à
//						    jour/destruction des lésions lorsque le BBItem est créateur.
//						    La diversité de traitement des lésions (unique/multiples...)
//						    nous oblige à les traiter plus bas (bonne idée ?).
// Returns      : 1 si aucune branche ne possède cette étiquette valeur de
//                retour de BBItem::developper(int numFils) sinon
// -----------------------------------------------------------------------------
int
BBItem::activer()
{
try
{
  // en mode correction //on est pas au bout du chemin
  //
  if (_pBigBoss->isEditionMode())
  {
    if (activerInEditionMode())
      return 0 ;
	}

	// Si le BBItem est rattaché à une boite de dialogue, on la crée
  //
	if (lienDialogue())
  {
    if (_pBBFilsPere)
      _pBBFilsPere->lancerMultiDialogue() ;

    return creerDialogue() ;
	}

	// Si le BBItem est rattaché purement à une fonction, on l'exécute
  //
	if (lienFonction())
    return activerForFunction() ;

	// Si le BBItem n'est pas rattaché à une boite de dialogue, il active sa
  // première branche si il est en mode "automatique", il propose ses fils si il
  // est en mode "manuel"
  if (_aBBItemFils.empty())
    return 0 ;

  // Il est prudent de vérifier qu'au moins un fils est "développable", sinon on
  // entre dans une boucle sans fin
  //
  BBiter ItFils = _aBBItemFils.begin() ;
  for ( ; _aBBItemFils.end() != ItFils ; ItFils++)
    if ((*ItFils)->FilsProlongeable)
      break ;
  if (_aBBItemFils.end() == ItFils)
  {
    // On simule une sortie "par le bas"
    ActivateFromEnd() ;
    return 0 ;
  }

  BBCmdMessage BBMsg ;

	_iFilsActif = 1 ;
	bool bTourner    = true ;

  int iNext = 0 ;

  // Niveau sans fenêtre ni fonction, piloté par des signaux
  //
	while (bTourner)
	{
    if (_pBigBoss->pileVide())
    {
      iNext = developper(_aBBItemFils.donneNieme(_iFilsActif - 1)) ;
      MiseAjourPatpatho() ;
    }
    else
      iNext = _pBigBoss->depile(&BBMsg) ;

		switch (iNext)
		{
      case NSDLG_SUITE      : // passage à la suite

        _iFilsActif++ ;
        // S'il n'y a pas de suivant : on sort par le bas
        if (_aBBItemFils.donneNieme(_iFilsActif-1) == (BBFilsItem*) NULL)
        {
          ActivateFromEnd() ;
          return 0 ;
        }
        break ;

      case NSDLG_RETOUR     : // Retour en arrière

        _iFilsActif-- ;
        // Si on est au début : on sort par le haut
        if (0 == _iFilsActif)
          return -1 ;
        break ;

      case NSDLG_SORTIE_BAS : // Sortie par le bas

        ActivateFromEnd() ;
        return 0 ;

      case NSDLG_SORTIE_HAUT  : // Sortie par le haut

        return -1 ;

      case NSDLG_ALLER_A      :

        if (ActivateGoTo(&BBMsg))
          return 0 ;
        break ;

      default :

        if ((iNext >= NSDLGRETOUR_DIRECT) && (iNext < NSDLGRETOUR_FIN))
        {
          // Does this message targets a lower level node
          //
          if (iNext >= NSDLGRETOUR_DIRECT + NSDLGRETOUR_SEPARATOR)
          {
            iNext -= NSDLGRETOUR_SEPARATOR ;
            return iNext ;
          }

          _iFilsActif = iNext - NSDLGRETOUR_DIRECT + 1 ;
        }
        else if ((iNext > 0) && (_aBBItemFils.donneNieme(iNext-1) != (BBFilsItem*) NULL))
          _iFilsActif = iNext ;
    }
  }
  return 1 ;
}
catch (...)
{
  erreur("Exception BBItem::activer.", standardError, 0) ;
  return 1 ;
}
}

// Returns true if activated
//
bool
BBItem::activerInEditionMode()
{
  NSPatPathoArray PPT(pContexte->getSuperviseur()) ;

  // trouver le fils ayant dans sa patpatho l'élément d'ID  = 1
  //
  BBFilsItem* pFilsItem = findSonToEdit() ;

  if (pFilsItem)
  {
    // si décalge nul alors ne pas développer Iter, mais attribuer true à son bCorriger
    int k = developper(pFilsItem) ;
    if (3 == k)
    {
      if (pFilsItem->FilsProlongeable)
      {
        if (false == pFilsItem->VectorFils.empty())
        {
          BBItem* pbbitem = *(pFilsItem->VectorFils.begin()) ;
          // Iter en tant que BBItem
          if (pbbitem)
            pbbitem->activer() ;
        }
      }
    }
    MiseAjourPatpatho() ;
    return true ;
  }

  for (BBiter Iter = _aBBItemFils.begin() ; _aBBItemFils.end() != Iter ; Iter++)
  {
    if ((*Iter)->bCorriger)
    {
      _pBigBoss->setEditionMode(false) ;
      PPT.vider() ;
      if ((false == (*Iter)->getItemData()->uniciteLesion()) && _pPPTEnCours)
      {
        PatPathoIter it = _pPPTEnCours->begin() ;

        for ( ; (_pPPTEnCours->end() != it) && (1 != (*it)->_ID) ; it++)
          ;
        if (_pPPTEnCours->end() != it)
          _pPPTEnCours->ExtrairePatPatho(it, &PPT) ;
      }

      int m = developper(*Iter, &PPT) ;
      if (0 != m)
      {
        bool cherchePereOD = true ;
        BBItem* pere = (*Iter)->getItemFather() ;
        while (cherchePereOD)
        {
          if ((BBItem*) NULL == pere)
            cherchePereOD = false ;
          else
          {
            if (pere->ouvreDialog())
              cherchePereOD = false ;
            else
            {
              if (pere->_pBBFilsPere)
                pere = pere->_pBBFilsPere->getItemFather() ;
              else
                pere = (BBItem*) 0 ;
            }
          }
        }
        if (pere)
          pere->activer() ;
      }
      MiseAjourPatpatho() ;
      return true ;
    }
  }

  return false ;
}

// Find son whose patpatho contains the element with ID = 1
// trouver le fils ayant dans sa patpatho l'élément d'ID  = 1
//
BBFilsItem*
BBItem::findSonToEdit()
{
  if (_aBBItemFils.empty())
    return (BBFilsItem*) 0 ;

  for (BBiter Iter = _aBBItemFils.begin() ; _aBBItemFils.end() != Iter ; Iter++)
  {
    if (false == (*Iter)->PatPtahovide())
    {
      NSVectFatheredPatPathoArray* pPatpathoItem = (*Iter)->getPatPatho() ;
      if (pPatpathoItem && (false == pPatpathoItem->empty()))
      {
        FatheredPatPathoIterVect iterPath = pPatpathoItem->begin() ;
        for ( ; pPatpathoItem->end() != iterPath ; iterPath++)
        {
          NSPatPathoArray* pPatPatho = (*iterPath)->getPatPatho() ;
          if (pPatPatho && (false == pPatPatho->empty()))
            for (PatPathoIter it = pPatPatho->begin() ; pPatPatho->end() != it ; it++)
              if (1 == (*it)->_ID)
                return *Iter ;
        }
      }
    }
  }

  return (BBFilsItem*) 0 ;
}

int
BBItem::activerForFunction()
{
  // La fonction peut se comporter soit comme une boite de dialogue (et elle
  // retourne 0), soit comme un aiguillage, et elle choisi le premier fils actif
  //
  bool bTourner = true ;
  if (_pBigBoss->pileVide())
    bTourner = _pNSFonction->execute(NSFCT_ACTIVE, &_iFilsActif) ;

  BBCmdMessage BBMsg ;

  int i = 0 ;
  while (bTourner)
  {
    if (_pBigBoss->pileVide())
    {
      i = developper(_aBBItemFils.donneNieme(_iFilsActif - 1)) ;
      bTourner = _pNSFonction->execute(NSFCT_AIGUILLE, &i) ;

      MiseAjourPatpatho() ;
    }
    else
      i = _pBigBoss->depile(&BBMsg) ;

    if (bTourner)
    {
      switch (i)
      {
        case NSDLG_SUITE : // passage à la suite

          _iFilsActif++ ;
          // S'il n'y a pas de suivant : on sort par le bas
          if (_aBBItemFils.donneNieme(_iFilsActif - 1) == 0)
          {
            ActivateFromEnd() ;
            return 0 ;
          }
          break ;

        case NSDLG_RETOUR : // retour en arrière

          _iFilsActif-- ;
          // Si on est au début : on sort par le haut
          if (0 == _iFilsActif)
            return -1 ;
          break ;

        case NSDLG_SORTIE_BAS : // sortie par le bas

          ActivateFromEnd() ;
          return 0 ;

        case NSDLG_SORTIE_HAUT  : // sortie par le haut

          return -1 ;

        case NSDLG_ALLER_A : // Traitement du message pour extraire la localisation et l'étiquette à atteindre

          if (ActivateGoTo(&BBMsg))
            return 0 ;
          break ;

        default :

          if ((i >= NSDLGRETOUR_DIRECT) && (i < NSDLGRETOUR_FIN))
          {
            // iFilsActif = i - NSDLGRETOUR_DIRECT + 1 ;
            bTourner = false ;
          }
          else if ((i > 0) && (_aBBItemFils.donneNieme(i-1) != 0))
            _iFilsActif = i ;
      }
    }
    else
      ActivateFromEnd() ;
  }
  return i ;
}

void
BBItem::ActivateFromEnd()
{
  if (_pBBFilsPere)
  {
    if (_pBBFilsPere->PatPtahovide())
    {
      if (actifVide())
        _pBBFilsPere->Active() ;
      else
        _pBBFilsPere->Desactive() ;
    }
    else
      _pBBFilsPere->Active() ;
  }
}

bool
BBItem::ActivateGoTo(const BBCmdMessage* pBBMsg)
{
  if ((BBCmdMessage*) NULL == pBBMsg)
    return false ;

  string sMessage = pBBMsg->getMessage() ;

  size_t pos = sMessage.find(string(1, nodeSeparationMARK)) ;
  if ((string::npos == pos) || (pos >= strlen(sMessage.c_str())))
    return false ;

  string sLocalis = string(sMessage, 0, pos) ;
  string sEtiquet = string(sMessage, pos + 1, strlen(sMessage.c_str()) - pos - 1) ;

  BBiter ItFils = _aBBItemFils.begin() ;

  bool bCheminTrouve = false ;

  // 3 cas :
  // 1) sLocalisation == sLocalis on lance l'étiquette
  // 2) sLocalisation est au début de sLocalis et on lance l'étiquette qui va dans la bonne direction
  // 3) sinon, on descend d'un cran
  //
  if (_sLocalisation == sLocalis)
  {
    int iBonFils = 1 ;

    for ( ; (_aBBItemFils.end() != ItFils) && ((*ItFils)->getItemLabel() != sEtiquet) ; ItFils++)
      iBonFils++ ;

    if (_aBBItemFils.end() != ItFils)
    {
      _iFilsActif = iBonFils ;
      bCheminTrouve = true ;
    }
  }
  else if ((strlen(_sLocalisation.c_str()) < strlen(sLocalis.c_str())) && (string(sLocalis, 0, strlen(_sLocalisation.c_str())) == _sLocalisation))
  {
    int iBonFils = 1 ;

    // On cherche l'étiquette qui va dans la bonne direction
    //
    for ( ; (_aBBItemFils.end() != ItFils) && (false == bCheminTrouve) ; ItFils++)
    {
      iBonFils++ ;
      string sGag = _sLocalisation + (*ItFils)->getItemLabel() ;
      if ((strlen(sGag.c_str()) <= strlen(sLocalis.c_str())) && (string(sLocalis, 0, strlen(sGag.c_str())) == sGag))
        bCheminTrouve = true ;
    }
    if (_aBBItemFils.end() != ItFils)
    {
      _iFilsActif = iBonFils ;
      bCheminTrouve = true ;
      _pBigBoss->reEmpile((BBCmdMessage*) pBBMsg, false) ;
    }
  }

  // On rempile et on sort par le bas
  //
  if (false == bCheminTrouve)
  {
    _pBigBoss->reEmpile((BBCmdMessage*) pBBMsg, false) ;
    ActivateFromEnd() ;
    return true ;
  }

  return false ;
}

// -----------------------------------------------------------------------------// Function     : int BBItem::developper(string sEtiquette)// Arguments    : sEtiquette -> étiquette de la branche à développer
// Description  : Demande à l'arborescence de se prolonger (usuellement pour
//                ouvrir une nouvelle boite de dialogue)
// Returns      : 1 si aucune branche ne possède cette étiquette valeur de
//                retour de BBItem::developper(int numFils) sinon
// -----------------------------------------------------------------------------
int
BBItem::developper(string sEtiquette)
{
try
{
  if (_aBBItemFils.empty())
    return 1 ;

  // Recherche du BBFilsItem qui pilote cette branche
  //
  BBiter it = _aBBItemFils.begin() ;
  for ( ; (_aBBItemFils.end() != it) && ((*it)->getItemLabel() != sEtiquette) ; it++)
    ;

	// Si on a trouvé la branche, on lui demande de se développer
  //
	if (_aBBItemFils.end() != it)
		return developper(*it) ;

	return 1 ;
}
catch (...)
{
  erreur("Exception BBItem::developper 1.", standardError, 0) ;
  return 1 ;
}
}

// -----------------------------------------------------------------------------
// Function     : int BBItem::developper(BBFilsItem* pFilsIt)
// Arguments    : BBFilsItem* pFils -> pointeur sur le fils à développer
// Description  : Demande à l'arborescence de se prolonger (usuellement pour
//                ouvrir une nouvelle boite de dialogue).
//                Il faut alors : - créer un BBItem à partir du fils choisi
//                                - lancer BBItem.creer()
//                                - lancer BBItem.activer()
// Returns      : 0 si tout a bien fonctionné
//						    1 si on a eu un problème
//						    2 si la branche n'a pas de prolongation
//						    3 si la branche a déjà été prolongée
// -----------------------------------------------------------------------------
int
BBItem::developper(BBFilsItem* pFilsIt, NSPatPathoArray* pPPT, int indexFils)
{
try
{
  if (((BBFilsItem*) NULL == pFilsIt) || (false == pFilsIt->VectorFils.empty()))
    return 3 ;

  //for (BBiterFils iter = pFilsIt->VectorFils.begin() ; iter != pFilsIt->VectorFils.end() ; iter++)
  //	if ((*iter))
  //    return 3 ;


	// La branche est-elle prolongeable ?

	//if (!(pFilsIt->estProlongeable()))  //PENSER A RETOURNER 1 EN CAS DE PB FICHIER DANS ESTPRO...
  if (false == pFilsIt->FilsProlongeable)
    return 2 ;

	//si pFilsIt actif et vide, on le désactive
  if (pFilsIt->isActif() && (pFilsIt->getItemTransfertData()->getPatPatho()->empty()))
	{
    // Sauf si on est en mode correction
    if (false == pFilsIt->bCorriger)
    {
      pFilsIt->Desactive() ;
      return 0 ; // iRetourDlg =0
    }
  }

  string sEtiquette = pFilsIt->getItemLabel() ;

	//*(pFilsIt->pDonnees) = *(pBigBoss->pBBFiche->pDonnees) ;
  pFilsIt->creerFils(indexFils) ;

  int iRetActiv = 0 ;
  // int ok = 0 ;
  // multidialogue
  if ((false == pFilsIt->getItemData()->uniciteLesion()) && (pFilsIt->bCorriger) && pPPT)
  {
    // chercher parmi les multiples BBitem celui dont le pPPTEnCours est pPPT
    // sa boîte de dialogue sera affichée , alors que celles de ses frères sera cachées
    bool continuer = true ;
    BBiterFils iter = pFilsIt->VectorFils.begin() ;
    while ((pFilsIt->VectorFils.end() != iter) && (continuer))
    {      (*iter)->_iRetourDlg = 0 ;
      PatPathoIter it = (*iter)->_pPPTEnCours->begin() ;
      PatPathoIter jt = pPPT->begin() ;
      bool bEgalite = true ;
      bool bContinue = true ;
      while (((*iter)->_pPPTEnCours->end() != it) && (pPPT->end() != jt) && bContinue)
      {
        if (!(*((*it)->getDataTank()) == *((*jt)->getDataTank())))
        {
          bEgalite  = false ;
          bContinue = false ;
        }
        else
        {
          it++ ;
          jt++ ;
        }
      }

      if (false == bEgalite)
        (*iter)->_bCacher = true ; // créer la NSDialog et la cacher
      iter++ ;
    }
	}

  for (BBiterFils iter = pFilsIt->VectorFils.begin() ; pFilsIt->VectorFils.end() != iter ; iter++)
  {
    // Simplifie les écritures
    BBItem* pNewFils = *iter ;

    // Donne sa filiation au nouvel élément
    donneGenetique(pNewFils, sEtiquette) ;

    // On demande au BBItem de se créer (et de créer ses fils qui correspondent à la même boîte de dialogue)
    int i = pNewFils->creer() ;
    if (0 == i)
    {
      iRetActiv = pNewFils->activer() ;
      if ((pNewFils->_iProfondeur < _pBigBoss->getSeuilSauve()) && (pNewFils->_pPPTEnCours))
        _pBigBoss->sauvegarde() ;
    }
	}

  //crée la boite multidialogue
  if ((pFilsIt->pNsMultiDialog))
  {
    int iResult = pFilsIt->pNsMultiDialog->Execute() ;
    if (IDCANCEL != iResult)
    {
      if ((pFilsIt->PatPtahovide()))
      {
        if (pFilsIt->actifVide())
          pFilsIt->Active() ;
        else
          pFilsIt->Desactive() ;
      }
      else
        pFilsIt->Active() ;
    }
	}

  if ((NsMultiDialog*) NULL == pFilsIt->pNsMultiDialog)
    pFilsIt->tuerFils() ;
  else
  {
    delete pFilsIt->pNsMultiDialog ;
    pFilsIt->pNsMultiDialog = (NsMultiDialog*) 0 ;
  }

  // Demande à l'éventuelle boite de dialogue de répercuter les modifs
  if (_pNSDialog)
    _pNSDialog->rafraichitControles() ;

	return iRetActiv ;
}
catch(TWindow::TXWindow& e)
{
  string sErr = string("Exception BBItem::developper 2 : ") + e.why() ;
  erreur(sErr.c_str(), standardError, 0) ;
  return 1 ;
}
catch (...)
{
  erreur("Exception BBItem::developper 2.", standardError, 0) ;
  return 1 ;
}
}

// -----------------------------------------------------------------------------
// Function     : int BBItem::developper(BBFilsItem* pFilsIt)
//	              cas de la consultation
// Arguments    : BBFilsItem* pFils -> pointeur sur le fils à développer
// Description  : Demande à l'arborescence de se prolonger (usuellement pour
//                ouvrir une nouvelle boite de dialogue).
//                Il faut alors : - créer un BBItem à partir du fils choisi
//                                - lancer BBItem.creer()
//                                - lancer BBItem.activer()
// Returns      : 0 si tout a bien fonctionné
//						    1 si on a eu un problème
//						    2 si la branche n'a pas de prolongation
//						    3 si la branche a déjà été prolongée
// -----------------------------------------------------------------------------
int
BBItem::developperConsultation(BBFilsItem* pFilsIt)
{
try
{
  if ((BBFilsItem*) NULL == pFilsIt)
    return 1 ;

  if (false == pFilsIt->VectorFils.empty())
    // for (BBiterFils iter = pFilsIt->VectorFils.begin() ; pFilsIt->VectorFils.end() != iter ; iter++)
    //  if ((*iter))
        return 3 ;

	// La branche est-elle prolongeable ?
//  if (!(pFilsIt->FilsProlongeable))
//	  return 2 ;

  //si pFilsIt actif et vide, on le désactive
  if (pFilsIt->isActif() && (pFilsIt->getItemTransfertData()->getPatPatho()->empty()))
  {
    pFilsIt->Desactive() ;
    return 0 ;  // iRetourDlg =0
  }

  int iRes = 0 ;
  pFilsIt->creerFils() ;
  string sEtiquette = pFilsIt->getItemLabel() ;

  if (pFilsIt->VectorFils.empty())
    return 0 ;

  for (BBiterFils iter = pFilsIt->VectorFils.begin() ; pFilsIt->VectorFils.end() != iter ; iter++)
  {
    // Simplifie les écritures
    BBItem* pNewFils = *iter ;

    // Donne sa filiation au nouvel élément
    donneGenetique(pNewFils, sEtiquette) ;

    // On demande au BBItem de se créer (et de créer ses fils qui correspondent à la même boîte de dialogue)
    int iResult = pNewFils->creer() ;
    if (iResult == 1)
      return 1 ;

    // rustine 28/11/02 RS
    //if (sEtiquette == string("ZRIKC1"))    if (pNewFils->ouvreArchetype())      iRes = pNewFils->activer() ;  }

  return iRes ;}
catch (...)
{
  erreur("Exception BBItem::developperConsultation.", standardError, 0) ;
  return 3 ;
}
}

// -----------------------------------------------------------------------------
// Initialise le BBItem
// -----------------------------------------------------------------------------
void
BBItem::donneGenetique(BBItem* pNewFils, string sEtiquette)
{
  NSSuper* pSuper = pContexte->getSuperviseur() ;

  string sMsg = string("donneGenetique() : Entrée -> Etiquette = ") + sEtiquette ;
  pSuper->trace(&sMsg, 1, NSSuper::trDebug) ;

  // Initialisation conçue pour les Fils guides
  //
  string sSonDlg = pNewFils->getItemDialogName() ;
  if ((strlen(sSonDlg.c_str()) >= 6) && (string(sSonDlg, 0, 6) == string("__IDEM")))
  // if (strncmp(pNewFils->pDonnees->nomDialogue, "__IDEM", 6) == 0)
    pNewFils->_sNomDlg = _sNomDlg ;
  else
    pNewFils->_sNomDlg = sSonDlg ;

  // Dans le cas d'un Archétype, et faute de Fil guide, on reste dans la même fenêtre
  if ((string("") == pNewFils->_sNomDlg) && (string("") != _sIdArchetype))
    pNewFils->_sNomDlg = _sNomDlg ;

  // on place ici l'héritage de l'id d'archetype des BBItem pour le cas des
  // archetypes (les dialogues se passent la chaine vide) car donneGenetique est
  // appelée pour chaque nouveau BBItem fils dans la méthode BBItem::creerFilsArchetype()
  pNewFils->_sIdArchetype = _sIdArchetype ;

  // Calcul de la nouvelle localisation
  string sCodeSens ;
  NSDico::donneCodeSens(&sEtiquette, &sCodeSens) ;

  sMsg = "donneGenetique() : Avant récup de sEtiquettePere" ;
  pSuper->trace(&sMsg, 1, NSSuper::trDebug) ;

  string sEtiquettePere = string("") ;
  if (_pBBFilsPere)
  {
    string sFilsLabel = _pBBFilsPere->getItemLabel() ;
    NSDico::donneCodeSens(&sFilsLabel, &sEtiquettePere) ;
  }

  sMsg = string("donneGenetique() : sEtiquettePere = ") + sEtiquettePere ;
  pSuper->trace(&sMsg, 1, NSSuper::trDebug) ;
  sMsg = string("donneGenetique() : sLocalisation = ") + _sLocalisation ;
  pSuper->trace(&sMsg, 1, NSSuper::trDebug) ;

  if (estPlusZero())
  {
    if (strlen(_sLocalisation.c_str()) > strlen(sEtiquettePere.c_str()))
      pNewFils->_sLocalisation = string(_sLocalisation, 0, strlen(_sLocalisation.c_str()) - strlen(sEtiquettePere.c_str())) ;
    else
      pNewFils->_sLocalisation = string("") ;

    if ((string("") != pNewFils->_sLocalisation) && (string("") == sEtiquettePere))
      pNewFils->_sLocalisation += string(1,cheminSeparationMARK) ;

    pNewFils->_sLocalisation += sCodeSens ;
  }
  else
    pNewFils->_sLocalisation = _sLocalisation + string(1, cheminSeparationMARK) + sCodeSens ;

  sMsg = "donneGenetique() : Sortie" ;
  if (estPlusZero())
    sMsg += "+00+00" ;
  pSuper->trace(&sMsg, 1, NSSuper::trDebug) ;
}

// -----------------------------------------------------------------------------
// Function     : int BBItem::creerDialogue()
// Description  : Création de l'élément d'interface (boite de dialogue...) qui
//                correspond à ce BBItem. Si l'élément est générateur de lésion
//                unique, la gestion de la lésion se fait à ce niveau.
// Returns      : 0 si tout a bien fonctionné
//						    1 si on a eu un problème
//						    2 si ce BBItem n'est pas ouvreur de boîte de dialogue
//						    3 si une boite de dialogue est déjà ouverte
//						    4 si La DLL spécifique est introuvable
// -----------------------------------------------------------------------------
// En cas de lancement d'un NSDialog::Execute(), retourne 0 pour OK et -1 pour
// CANCEL (cf NSDialog)
// -----------------------------------------------------------------------------
int
BBItem::creerDialogue()
{
try
{
  string sLang = pContexte->getUserLanguage() ;

  NSSuper* pSuper = pContexte->getSuperviseur() ;
	string sMsg = "creerDialogue() : Entrée" ;
	pSuper->trace(&sMsg, 1, NSSuper::trSubDetails) ;

	// On teste si le BBItem est ouvreur et s'il possède un nom de dialogue
  //
	if (false == ouvreDialog())
  {
    sMsg = string("Not a dialog box or Archetype opening node, leaving") ;
    pSuper->trace(&sMsg, 1, NSSuper::trSubDetails) ;
		return 2 ;
  }

	if ((string("") == _sNomDlg) || (string::npos == _sNomDlg.find_first_not_of(string(" "))))
  {
    if (false == existArchetypeDialog(sLang))
    {
      sMsg = string("No dialog box and no Archetype, leaving") ;
	    pSuper->trace(&sMsg, 1, NSSuper::trSubDetails) ;
      return 2 ;
    }
  }
  else
  {
    sMsg = string("Dialog box to open : ") + _sNomDlg ;
	  pSuper->trace(&sMsg, 1, NSSuper::trSubDetails) ;
  }

	// On vérifie qu'un dialogue n'est pas déjà créé
	if (_pNSDialog)
  {
    sMsg = string("Dialog box already exists, leaving") ;
    pSuper->trace(&sMsg, 1, NSSuper::trSubDetails) ;
		return 3 ;
  }

	// On crée un objet NSDialog
  if (string("") != _Data.getDialogFile())
  {
    if (false == _pBigBoss->CreerNouveauModule(_Data.getDialogFile()))
    {
      sMsg = string("Cannot open dll ") + _Data.getDialogFile() + string(", leaving") ;
      pSuper->trace(&sMsg, 1, NSSuper::trSubDetails) ;
      return 4 ;
    }
  }

  NsMultiDialog* pMulti = (NsMultiDialog*) 0 ;

  if (_pBBFilsPere)
    pMulti = _pBBFilsPere->pNsMultiDialog ;

  TModule* pModule = (TModule*) 0 ;

  if (string("") != _Data.getDialogFile())
  {
    pModule = _pBigBoss->TrouverModule(_Data.getDialogFile()) ;
    if ((TModule*) NULL == pModule)
    {
      sMsg = string("Cannot find dll ") + _Data.getDialogFile() + string(", leaving") ;
      pSuper->trace(&sMsg, 1, NSSuper::trSubDetails) ;
      return 1 ;
    }
  }

  pSuper->afficheStatusMessage("Création du dialogue") ;

  bool bTabControl = false ;

  if ((false == ouvreArchetype()) || (string("__AUTO") != _sNomDlg))
  {
    if (string("__AUTO") == _sNomDlg)#ifdef __OB1__      _pNSDialog = new NSDialog(pContexte, donneFenetre(), this, pModule, pMulti) ;
#else
			_pNSDialog = new NSDialog(pContexte, donneFenetre(), this, pModule, pMulti, pBigBoss->IsInitFromBbk()) ;
#endif
    else if (string("") == _sNomDlg)
    {
      Cdialogbox* pDialogBox = getArchetypeDialog(sLang) ;
      // on sait déjà ici qu'on possède un dialogbox dans l'archetype
      if (pDialogBox->getTabControl() == NULL)
      {
        if (getArchetypeFunction() == string("Controled"))
#ifdef __OB1__
          _pNSDialog = new NSRCDialog(pContexte, donneFenetre(), this, pModule, pMulti) ;
#else
					_pNSDialog = new NSRCDialog(pContexte, donneFenetre(), this, pModule, pMulti, pBigBoss->IsInitFromBbk()) ;
#endif
        else
#ifdef __OB1__
          _pNSDialog = new NSDialog(pContexte, donneFenetre(), this, pModule, pMulti) ;
#else
					_pNSDialog = new NSDialog(pContexte, donneFenetre(), this, pModule, pMulti, pBigBoss->IsInitFromBbk()) ;
#endif
      }
      else
        bTabControl = true ;
    }
    else#ifdef __OB1__      _pNSDialog = new NSDialog(pContexte, donneFenetre(), _sNomDlg.c_str(), 0, this, pModule, pMulti) ;
#else
			_pNSDialog = new NSDialog(pContexte, donneFenetre(), _sNomDlg.c_str(), 0, this, pModule, pMulti, pBigBoss->IsInitFromBbk()) ;
#endif
  }
  // Boite de dialogue non modale : pour le dialogue de lésion multiples ou les  // Archetypes lorsqu'ils sont en BBItem lanceur (les Archétypes de sous-niveau  // sont modaux)  // if ((pMulti) || (ouvreArchetype() && (this == pBigBoss->pBBItem)))  if ((pMulti) || (false == _bModalDialog))
  {
    if (ouvreArchetype() && (string("__AUTO") == _sNomDlg))    {      nsarcParseur *pArcParseur = dynamic_cast<nsarcParseur*>(_pParseur) ;      if (pArcParseur)      {        nsarcDialog* pDlg = new nsarcDialog(donneFenetre(), pContexte, pArcParseur) ;        pDlg->Create() ;      }    }    else if (bTabControl)
    {
#ifdef __OB1__
      NSTabWindow* pDlg = new NSTabWindow(pContexte, donneFenetre(), this, pModule, pMulti) ;
#else
			NSTabWindow* pDlg = new NSTabWindow(pContexte, donneFenetre(), this, pModule, pMulti, pBigBoss->IsInitFromBbk()) ;
#endif
      pDlg->Create() ;
      // on référence pNSDialog pour servir + tard à la NSCQVue
      _pNSDialog = pDlg ;
    }
    else
      _pNSDialog->Create() ;
    //if (bCacher)    //	pNSDialog->ShowWindow(SW_HIDE) ;
  }
  else
  {
    //if (bCacher)
    //  pNSDialog->ShowWindow(SW_HIDE);

    pSuper->afficheStatusMessage("Ouverture du dialogue") ;

    if (bTabControl)
    {
#ifdef __OB1__
      NSTabWindow* pDlg = new NSTabWindow(pContexte, donneFenetre(), this, pModule, pMulti) ;
#else
			NSTabWindow* pDlg = new NSTabWindow(pContexte, donneFenetre(), this, pModule, pMulti, pBigBoss->IsInitFromBbk()) ;
#endif
      _pNSDialog = pDlg ;
      pDlg->Execute() ;
    }
    else
      _pNSDialog->Execute() ;

    if (_bCacher)
      _pNSDialog->ShowWindow(SW_HIDE) ;

    if (_pNSDialog)
    {
      if (_pBBFilsPere)
      {
        if (_pBBFilsPere->PatPtahovide())
        {
          if ((actifVide()) && (0 == _iRetourDlg))  // OK
            _pBBFilsPere->Active() ;
          else
            _pBBFilsPere->Desactive() ;
        }
        else
          _pBBFilsPere->Active() ;
      }

      // On fait le delete à la fin, de façon à ce que le BBItem sache encore
      // à quel dialogue il est attaché pendant les Active() ou Desactive()
      delete _pNSDialog ;
      _pNSDialog = (NSDialog*) 0 ;
    }
  }

	sMsg = "creerDialogue() : Sortie" ;
	pSuper->trace(&sMsg, 1, NSSuper::trSubDetails) ;

	return _iRetourDlg ;
}
catch(TWindow::TXWindow& e)
{
  string sErr = string("Exception BBItem::creerDialogue : ") ;
  if (string("") != _sNomDlg)
  	sErr += _sNomDlg + string(" ") ;
  sErr += e.why() ;
  erreur(sErr.c_str(), standardError, 0) ;
  return 0 ;
}
catch (...)
{
  erreur("Exception BBItem::creerDialogue.", standardError, 0) ;
  return 0 ;
}
}

// -----------------------------------------------------------------------------// Function     : int BBItem::gereRien()// Description  : Gestion de l'élément __RIEN
//						    Cet élément permet de créer/détruire des lésions pour
//                lorsqu'il n'y a pas de boite de dialogue associée.
// Returns      : 0 si tout a bien fonctionné
//						    1 si on a eu un problème
// -----------------------------------------------------------------------------
int
BBItem::gereRien()
{
	return 0 ;
}

// -----------------------------------------------------------------------------
// enleve les 0 de la string : 00023 devient 23 -- 00.23 devient 0.23
// -----------------------------------------------------------------------------
string
BBItem::EnleveZeo(string* pDollar)
{
  if ((string*) NULL == pDollar)
    return string("") ;

  if (string("") == *pDollar)
  {
    *pDollar = string("") ;
    return (*pDollar) ;
  }

  if (string("0") == *pDollar)
    return (*pDollar) ;

  bool bIsNegative = false ;

  // There is a minus sign
  //
  size_t iPos = pDollar->find("-") ;
  if (NPOS != iPos)
  {
    // If it isn't the first char, that's bad
    //
    if (iPos > 0)
    {
      *pDollar = string("") ;
      return (*pDollar) ;
    }

    // If the entry is limited to this minus char, that's bad too
    //
    if (strlen(pDollar->c_str()) == 1)
    {
      *pDollar = string("") ;
      return (*pDollar) ;
    }

    // If there are multiple minus char, that's also bad
    //
    size_t iPos = pDollar->find("-", 1) ;
    if (NPOS != iPos)
    {
      *pDollar = string("") ;
      return (*pDollar) ;
    }

    bIsNegative = true ;
    *pDollar = string(*pDollar, 1, strlen(pDollar->c_str()) - 1) ;
  }

  char cZero = (*pDollar)[0] ;
  while (('0' == cZero) && ('.' != (*pDollar)[1]))
  {
    (*pDollar) = string(*pDollar, 1, strlen(pDollar->c_str()) - 1) ;
    cZero = (*pDollar)[0] ;
  }

  // If only '0', return "0"
  //
  if (string("") == *pDollar)
  {
    *pDollar = string("0") ;
    return (*pDollar) ;
  }

  if (bIsNegative)
    *pDollar = string("-") + *pDollar ;

  if (string("-0") == *pDollar)
    *pDollar = string("0") ;

  return (*pDollar) ;
}

// -----------------------------------------------------------------------------
// le vecteur est vide on ajoute seulement l'étiquette
// ranger le patpatho de pFils dans pPPTEnCours
// traite les cas suivants :  1)	pFils : VDIAM1/MSYST1/2KG001/£N0;01
//											          mettre dans pPPTEnCours : VDIAM1 	MSYST1
//																							            2KG001 	£N0;01
//                            2)  pFils : 2KG001/£N0;01
//											          mettre dans pPPTEnCours : 2KG001 	£N0;01
//
//									          3)  pFils : VDIAM1
//                                mettre dans pPPTEnCours : VDIAM1
// -----------------------------------------------------------------------------
// Refabrication de la patpathoinfo du noeud fils à partir de ses données de transfert <br>
// Ajout de la patpathoInfo au pPPTEnCours <br>
// Rebuilding the patpathoInfo of the child node from it's transfert data <br>
// Adding the pathpatoInfo to pPPTEnCours
// -----------------------------------------------------------------------------
void
BBItem::AjouteEtiquette(BBFilsItem* pFils, string sArc, NSPatPathoInfo* pPreviousNode, bool bForceEmptyId)
{
try
{
	if (((BBFilsItem*) NULL == pFils) ||
                    ((NSTransferInfo*) NULL == pFils->getItemTransfertData()))
		return ;

  Message* pTranfertMsg = pFils->getItemTransfertMsg() ;
  if ((Message*) NULL == pTranfertMsg)
		return ;

	Message Msg(*pTranfertMsg) ;

  if (pPreviousNode)
  {
    Msg.SetTreeID(pPreviousNode->getDoc()) ;
    Msg.SetNoeud(pPreviousNode->getNodeID()) ;

    if (pPreviousNode->getTemporaryLinks())
      Msg.SetTemporaryLinks(pPreviousNode->getTemporaryLinks()) ;
  }
  else if (bForceEmptyId)
  {
    Msg.SetTreeID(string("")) ;
    Msg.SetNoeud(string("")) ;
  }

	// On met de côté les constituants du message
	string sChampEdit 	= Msg.GetComplement() ;
	string sCertitude  	= Msg.GetCertitude() ;
	string sPluriel    	= Msg.GetPluriel() ;
	string sVisible    	= Msg.GetVisible() ;
	string sStockNoeud 	= Msg.GetNoeud() ;
	string sTexteLibre  = Msg.GetTexteLibre() ;
	string sRights		  = Msg.GetNodeRight() ;

	// pTransfertMessage a un intêret vide alors initialser cet intêret avec "A"
	if (string("") == Msg.GetInteret())
  {
		Msg.SetInteret(string("A")) ;
    pTranfertMsg->SetInteret(string("A")) ;
  }

	// Initialisation de l'archetype
	if ((string("") != sArc) && (Msg.GetArchetype() != sArc))
  {
		Msg.SetArchetype(sArc) ;
    pTranfertMsg->SetArchetype(sArc) ;
  }

	string sLabel = pFils->getItemLabel() ;

	// texte libre £?????
	if (string("£?????") == sLabel)
	{
		if (_pPPTEnCours)
			_pPPTEnCours->ajoutePatho(sLabel, &Msg, ORIGINE_PATH_PATHO + 0) ;
		return ;
	}

	// Est-ce une étiquette multiple ?
	size_t posit = sLabel.find(string(1, cheminSeparationMARK)) ;

	// Un / positionné à la fin : on l'enlève
	if (strlen(sLabel.c_str()) - 1 == posit)
	{
		pFils->setItemLabel(string(sLabel, 0, strlen(sLabel.c_str()) - 1)) ;
		posit = string::npos ;
	}

	// Pas une étiquette multiple, traitement simple
	if (string::npos == posit)
	{
		sLabel = pFils->getItemLabel() ;
		if (_pPPTEnCours)
			_pPPTEnCours->ajoutePatho(sLabel, &Msg, ORIGINE_PATH_PATHO + 0) ;

		return ;
	}

	// Une étiquette multiple peut juste signifier un complément forcé ($...)
	if ('$' == (pFils->getItemLabel())[posit+1])
	{
		sLabel = string(pFils->getItemLabel(), 0, posit) ;
		size_t posit_debut = posit + 2 ;
		if ((posit_debut < strlen(pFils->getItemLabel().c_str())) && (_pPPTEnCours))
		{
			string sComplValue = string(pFils->getItemLabel(), posit_debut, strlen(pFils->getItemLabel().c_str()) - posit_debut) ;

      // Numerical value
      if (sLabel.find("£N") != string::npos)
      {
      	strip(sComplValue, stripLeft, '0') ;
        if (string("") == sComplValue)
        	sComplValue = string("0") ;
      }

      pTranfertMsg->SetComplement(sComplValue) ;
			Msg.SetComplement(sComplValue) ;
			_pPPTEnCours->ajoutePatho(sLabel, &Msg, ORIGINE_PATH_PATHO + 0) ;
		}
		return ;
	}

  string sEtiqRoot       = string("") ;
  string sEtiqFormat     = string("") ;
  string sEtiqUnite      = string("") ;
  string sEtiqComplement = string("") ;
  string sEtiqCertitude  = string("") ;
  string sEtiqPluriel    = string("") ;

  string sEtiqLabel      = pFils->getItemLabel() ;
  posit = sEtiqLabel.find(string(1, cheminSeparationMARK)) ;

  while (string::npos != posit)
  {
    string sElement = string(sEtiqLabel, 0, posit) ;
    sEtiqLabel      = string(sEtiqLabel, posit + 1, strlen(sEtiqLabel.c_str()) - posit - 1) ;

    if      ('2' == sElement[0])
      sEtiqUnite  = sElement ;
    else if ('£' == sElement[0])
      sEtiqFormat = sElement ;
    else if ('$' == sElement[0])
      sEtiqComplement = string(sElement, 1, strlen(sElement.c_str()) - 1) ;
    else if ('W' == sElement[0])
    {
      if      (string(sElement, 0, 3) == "WCE")
        sEtiqCertitude = sElement ;
      else if (string(sElement, 0, 3) == "WPL")
        sEtiqPluriel   = sElement ;
    }
    else
      sEtiqRoot = sElement ;

    posit = sEtiqLabel.find(string(1, cheminSeparationMARK)) ;
  }
  if      (sEtiqLabel[0] == '2')
    sEtiqUnite  = sEtiqLabel ;
  else if (sEtiqLabel[0] == '£')
    sEtiqFormat = sEtiqLabel ;
  else if (sEtiqLabel[0] == '$')
    sEtiqComplement = string(sEtiqLabel, 1, strlen(sEtiqLabel.c_str())-1) ;
  else if (sEtiqLabel[0] == 'W')
  {
    if      (string(sEtiqLabel, 0, 3) == string("WCE"))
      sEtiqCertitude = sEtiqLabel ;
    else if (string(sEtiqLabel, 0, 3) == string("WPL"))
      sEtiqPluriel   = sEtiqLabel ;
  }
  else
    sEtiqRoot = sEtiqLabel ;

  if (string("") == sChampEdit)
  {
    pTranfertMsg->SetComplement(sEtiqComplement) ;
    Msg.SetComplement(sEtiqComplement) ;
  }

  pTranfertMsg->SetUnit(sEtiqUnite) ;
  Msg.SetUnit(sEtiqUnite) ;

  if (sEtiqCertitude.find(string("WCEA0")) == string::npos) //ne pas mettre dans certitude WCEA0 : présent 100%
  {
    pTranfertMsg->SetCertitude(sEtiqCertitude) ;
    Msg.SetCertitude(sEtiqCertitude) ;
  }

  pTranfertMsg->SetPluriel(sEtiqPluriel) ;
  Msg.SetPluriel(sEtiqPluriel) ;

  if (string("") != sEtiqRoot)
    sEtiqFormat = sEtiqRoot ;

	// Verify that there is not useless '0' on left
	if (string(sEtiqFormat, 0, 2) == string("£N"))
	{
		sChampEdit = Msg.GetComplement() ;
    EnleveZeo(&sChampEdit) ;
    pTranfertMsg->SetComplement(sChampEdit) ;
    Msg.SetComplement(sChampEdit) ;
	}

	_pPPTEnCours->ajoutePatho(sEtiqFormat, &Msg, ORIGINE_PATH_PATHO + 0) ;
}
catch (...)
{
	erreur("Exception BBItem::AjouteEtiquette.", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------
// Refabrication de la patpathoinfo du noeud fils à partir de ses données de transfert
// Rebuilding the patpathoinfo of the child node from it's transfert data
// -----------------------------------------------------------------------------
void
BBItem::AjouteTmpEtiquette(NSPatPathoArray *pTempPatPatho, BBFilsItem* pFils, NSPatPathoInfo* pPreviousNode)
{
try
{
  if ((NSPatPathoArray*) NULL == pTempPatPatho)
    return ;

  if (((BBFilsItem*) NULL == pFils) || ((NSTransferInfo*) NULL == pFils->getItemTransfertData()))
    return ;

  Message* pTmpMsg = pFils->getItemTransfertData()->getTmpTransfertMessage() ;
  if ((Message*) NULL == pTmpMsg)
    return ;

  Message Msg(*pTmpMsg) ;
  if (pPreviousNode)
  {
    Msg.SetTreeID(pPreviousNode->getDoc()) ;
    Msg.SetNoeud(pPreviousNode->getNodeID()) ;
  }

  string sNoeud = string("") ;
  string sUnite = string("") ;

  // On met de côté les constituants du message
  string sChampEdit 	= Msg.GetComplement() ;
  string sCertitude   = Msg.GetCertitude() ;
  string sPluriel     = Msg.GetPluriel() ;
  string sVisible     = Msg.GetVisible() ;
  string sStockNoeud  = Msg.GetNoeud() ;
  string sTexteLibre  = Msg.GetTexteLibre() ;
  //
  // pTransfertMessage a un intêret vide alors initialser cet intêret avec "A"
  //
  if (pTmpMsg->GetInteret() == "")
  {
    pTmpMsg->SetInteret(string("A")) ;
    Msg.SetInteret(string("A")) ;
  }

  string sLabel = pFils->getItemLabel() ;

  //texte libre £?????
  if (string("£?????") == sLabel)
  {
    pTempPatPatho->ajoutePatho(sLabel, &Msg, ORIGINE_PATH_PATHO + 0) ;
    return ;
  }

  // Est-ce une étiquette multiple ?
  size_t posit = sLabel.find(string(1, cheminSeparationMARK)) ;
  // Un / positionné à la fin : on l'enlève
  if (strlen(sLabel.c_str()) - 1 == posit)
  {
    pFils->setItemLabel(string(sLabel, 0, strlen(sLabel.c_str()) - 1)) ;
    posit = string::npos ;
  }

  // Pas une étiquette multiple, traitement simple
  if (string::npos == posit)
  {
    sLabel = pFils->getItemLabel() ;
    pTempPatPatho->ajoutePatho(sLabel, &Msg, ORIGINE_PATH_PATHO + 0) ;
    return ;
  }

  // Une étiquette multiple peut juste signifier un complément forcé ($...)
  if ('$' == (pFils->getItemLabel())[posit + 1])
  {
    size_t posit_debut = posit + 2 ;
    if (posit_debut < strlen(pFils->getItemLabel().c_str()))
    {
      string sComplValue = string(pFils->getItemLabel(), posit_debut, strlen(pFils->getItemLabel().c_str()) - posit_debut) ;
      pTmpMsg->SetComplement(sComplValue) ;
      Msg.SetComplement(sComplValue) ;
      pTempPatPatho->ajoutePatho(sLabel, &Msg, ORIGINE_PATH_PATHO + 0) ;
    }
    return ;
  }

  string sEtiqRoot        = string("") ;
  string sEtiqFormat      = string("") ;
  string sEtiqUnite       = string("") ;
  string sEtiqComplement  = string("") ;
  string sEtiqCertitude   = string("") ;
  string sEtiqPluriel     = string("") ;

  string sEtiqLabel       = pFils->getItemLabel() ;
  posit = sEtiqLabel.find(string(1, cheminSeparationMARK)) ;

  while (string::npos != posit)
  {
    string sElement = string(sEtiqLabel, 0, posit) ;
    sEtiqLabel      = string(sEtiqLabel, posit + 1, strlen(sEtiqLabel.c_str()) - posit - 1) ;

    if      (sElement[0] == '2')
      sEtiqUnite  = sElement ;
    else if (sElement[0] == '£')
      sEtiqFormat = sElement ;
    else if (sElement[0] == '$')
      sEtiqComplement = string(sElement, 1, strlen(sElement.c_str()) - 1) ;
    else if (sElement[0] == 'W')
    {
      if      (string(sElement, 0, 3) == "WCE")
        sEtiqCertitude  = sElement ;
      else if (string(sElement, 0, 3) == "WPL")
        sEtiqPluriel    = sElement ;
    }
    else
      sEtiqRoot = sElement ;

    posit = sEtiqLabel.find(string(1, cheminSeparationMARK)) ;
  }

  if      ('2' == sEtiqLabel[0])
    sEtiqUnite  = sEtiqLabel ;
  else if ('£' == sEtiqLabel[0])
    sEtiqFormat = sEtiqLabel ;
  else if ('$' == sEtiqLabel[0])
    sEtiqComplement = string(sEtiqLabel, 1, strlen(sEtiqLabel.c_str()) - 1) ;
  else if ('W' == sEtiqLabel[0])
  {
    if      (string(sEtiqLabel, 0, 3) == string("WCE"))
      sEtiqCertitude = sEtiqLabel ;
    else if (string(sEtiqLabel, 0, 3) == string("WPL"))
      sEtiqPluriel   = sEtiqLabel ;
  }
  else
    sEtiqRoot = sEtiqLabel ;

  if (string("") == sChampEdit)
  {
    pTmpMsg->SetComplement(sEtiqComplement) ;
    Msg.SetComplement(sEtiqComplement) ;
  }

  pTmpMsg->SetUnit(sEtiqUnite) ;
  Msg.SetUnit(sEtiqUnite) ;

  if (sEtiqCertitude.find(string("WCEA0")) == string::npos) //ne pas mettre dans certitude WCEA0 : présent 100%
  {
    pTmpMsg->SetCertitude(sEtiqCertitude) ;
    Msg.SetCertitude(sEtiqCertitude) ;
  }

  pTmpMsg->SetPluriel(sEtiqPluriel) ;
  Msg.SetPluriel(sEtiqPluriel) ;

  if (string("") != sEtiqRoot)
    sEtiqFormat = sEtiqRoot ;

  pTempPatPatho->ajoutePatho(sEtiqFormat, &Msg, ORIGINE_PATH_PATHO + 0) ;
}
catch (...)
{
  erreur("Exception BBItem::AjouteEtiquette.", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------// Function     : bool BBItem::okFermerDialogue()// Description  : Fonction appelée par la boite de dialogue qui souhaite se
//                fermer et demande l'autorisation de le faire. Demande aux
//                BBFilsItem de se préparer à la fermeture.
// Returns      : true si la boite de dialogue à l'autorisation de se fermer
//						    false sinon
// -----------------------------------------------------------------------------
bool
BBItem::okFermerDialogue(bool rapatrier, bool bDetacheControle)
{
try
{
	bool retour = true ;
  string sNomXml = string("") ;

  // Si le BBItem est ouvreur d'archetype, il transmet le nom du fichier
  // à ses fils (racines de l'archetype) par AjouteEtiquette
  if (ouvreArchetype())
    sNomXml = _Data.getSonsList() ;

	// Si on demande un rapatriement : mise à zéro du codon
  if (rapatrier)
		_bMiseAJour = true ;
	else
		_bMiseAJour = false ;

  if (true == rapatrier)
  {
    if (_pPPTEnCours)
      _pPPTEnCours->vider() ;

    if (false == _aBBItemFils.empty())
    {
      //pour tous les fils
			for (BBiter it = _aBBItemFils.begin() ; _aBBItemFils.end() != it ; it++)
      {
        if ((*it)->okFermerDialogue(rapatrier, bDetacheControle))
        {
          if ((*it)->isActif())
          {
            // cas particulier : si sEtiquettePatpatho contient £C --> prendre les données vider le complément
            if (((*it)->getItemLabel().find(string("£C;")) != string::npos) ||
                ((*it)->getItemLabel().find(string("/£C;")) != string::npos))
              (*it)->getItemTransfertMsg()->SetComplement(string("")) ;

            NSVectFatheredPatPathoArray* pPatpathoItem = (*it)->getPatPatho() ;

            // si le vecteur est vide on ajoute seulement  l'étiquette
            // if (pPatpathoItem->empty())
            if ((*it)->PatPtahovide())
              AjouteEtiquette(*it, sNomXml) ;

            // sinon, pour chaque élément du vecteur , on ajoute l'étiquette et cet élément
            else if (_pPPTEnCours)
            {
              if ((*it)->estPlusZero())
              {
                if (false == pPatpathoItem->empty())
                  for (FatheredPatPathoIterVect j = pPatpathoItem->begin() ; pPatpathoItem->end() != j ; j++)
                    _pPPTEnCours->ajouteVecteur((*j)->getPatPatho(), ORIGINE_PATH_PATHO + 0) ;
              }
              else if (false == pPatpathoItem->empty())
              {
                for (FatheredPatPathoIterVect j = pPatpathoItem->begin() ; pPatpathoItem->end() != j ; j++)
                {
                  AjouteEtiquette(*it, sNomXml, (*j)->getFatherNode(), true) ;
                  _pPPTEnCours->ajouteVecteur((*j)->getPatPatho(), ORIGINE_PATH_PATHO + 1) ;
                }
              }
            }
          }
        }
      }
    }
  }

  // PA 16/05/09
  // if (NULL != pTempPPT)
  //  *pTempPPT = *pPPTEnCours ;

  mergePPTEnCours(bDetacheControle) ;
  return retour ;
}
catch (...)
{
  erreur("Exception BBItem::okFermerDialogue.", standardError, 0) ;
  return true ;
}
}

void
BBItem::detacheControle()
{
  if (false == _aBBItemFils.empty())
    for (BBiter it = _aBBItemFils.begin() ; _aBBItemFils.end() != it ; it++)
      (*it)->detacheControle() ;
}

void
BBItem::mergePPTEnCours(bool bKillMerge)
{
try
{
  if ((NSPatPathoArray*) NULL == _pPPTMerge)
    return ;

  if (_pPPTMerge->empty())
  {
    delete _pPPTMerge ;
    _pPPTMerge = (NSPatPathoArray*) 0 ;
    return ;
  }

  if (_pPPTEnCours->empty())
  {
    *_pPPTEnCours = *_pPPTMerge ;
    delete _pPPTMerge ;
    _pPPTMerge = (NSPatPathoArray*) 0 ;
    return ;
  }

  NSVectPatPathoArray Vect ;
  PatPathoIter iterRoot = _pPPTMerge->begin() ;
  _pPPTMerge->ExtraireVecteurPatPathoFreres(iterRoot, &Vect) ;

  int iColBase = (*iterRoot)->getColonne() ;

  PatPathoIterVect iterVect = Vect.begin() ;
  while (Vect.end() != iterVect)
  {
    // Le type de chaque frere est donné par le code lexique de sa racine
    //
    iterRoot = (*iterVect)->begin() ;

    string sElemLex = (*iterRoot)->getLexique() ;
    string sSens ;
    pContexte->getDico()->donneCodeSens(&sElemLex, &sSens) ;

    // on supprime tous les fils de code sSens dans pPPTEnCours
    PatPathoIter iterCourant = _pPPTEnCours->ChercherItem(sSens) ;
    PatPathoIter iterLast    = NULL ;

    while (((PatPathoIter) NULL != iterCourant) && (_pPPTEnCours->end() != iterCourant))
    {
      if ((*iterCourant)->getColonne() == iColBase)
      {
        _pPPTEnCours->SupprimerFils(iterCourant) ;
        _pPPTEnCours->SupprimerItem(iterCourant) ;

        iterLast = iterCourant ;

        if (((PatPathoIter) NULL == iterCourant) || (_pPPTEnCours->end() == iterCourant))
          break ;

        if ((*iterCourant)->getLexique() != sElemLex)
          iterCourant = _pPPTEnCours->ChercherItem(sSens, true, iterCourant) ;
      }
      else // cas item trouvé ailleurs qu'en colonne 0
        iterCourant = _pPPTEnCours->ChercherItem(sSens, true, iterCourant) ;
    }

    // on insère le nouveau fils de la patpatho de merge
    if (iterLast && (_pPPTEnCours->end() != iterLast))
      // s'il existait un fils : on le réécrit à son ancienne position
      // NB : les fils doivent en principe être uniques
      _pPPTEnCours->InserePatPatho(iterLast, *iterVect, 0, true) ;
    else
      // insertion en queue
      _pPPTEnCours->InserePatPatho(_pPPTEnCours->end(), *iterVect, 0, true) ;

    iterVect++ ;
  }

  // RAZ de pPPTMerge
  if (bKillMerge)
  {
    delete _pPPTMerge ;
    _pPPTMerge = (NSPatPathoArray*) 0 ;
  }
}
catch (...)
{
  erreur("Exception BBItem::mergePPTEnCours.", standardError, 0) ;
}
}

// Building a temporary patpatho through a recursive process
//
// bRootRecursivity = true at first recursivity level
// iLevel = 1 if local patpatho is son of ending node ; 0 means brother
//
void
BBItem::rapatrieTmpPpt(NSPatPathoArray *pTempPatPatho, size_t iLevel, bool bRootRecursivity)
{
try
{
  if ((NSPatPathoArray*) NULL == pTempPatPatho)
    return ;

  if (bRootRecursivity)
    pTempPatPatho->vider() ;

  if (_aBBItemFils.empty())
    return  ;

  NSSuper* pSuper = pContexte->getSuperviseur() ;

  //pour tous les fils
  for (BBiter it = _aBBItemFils.begin() ; _aBBItemFils.end() != it ; it++)
  {
    NSPatPathoArray SonTempPatPatho(pSuper) ;

    // PA 18/05/09 (*i)->rapatrieTmpPpt(pTempPatPatho) ;
    if ((*it)->isActif())
    {
      // cas particulier : si sEtiquettePatpatho contient £C --> prendre les données vider le complément
      if (((*it)->getItemLabel().find(string("£C;")) != string::npos) ||
          ((*it)->getItemLabel().find(string("/£C;")) != string::npos))
      {
        if (((*it)->getItemTransfertData()) && ((*it)->getItemTransfertData()->getTmpTransfertMessage()))
          (*it)->getItemTransfertData()->getTmpTransfertMessage()->SetComplement(string("")) ;
      }

      // This son is not "expanded"; just take stored information
      //
      if ((*it)->VectorFils.empty())
      {
        // pPatpathoItem = (*i)->getTmpPatho() ;
        //
        NSVectFatheredPatPathoArray* pPatpathoItem = (*it)->getPatPatho() ;

        //si le vecteur est vide on ajoute seulement  l'étiquette
        //if (pPatpathoItem->empty())
        // if ((*i)->TmpPptvide())
        if ((*it)->PatPtahovide())
          AjouteTmpEtiquette(&SonTempPatPatho, *it) ;

        // sinon, pour chaque élément du vecteur , on ajoute
        // l'étiquette et cet élément
        else
        {
          if ((*it)->estPlusZero())
          {
            if (false == pPatpathoItem->empty())
              for (FatheredPatPathoIterVect j = pPatpathoItem->begin() ; pPatpathoItem->end() != j ; j++)
                SonTempPatPatho.ajouteVecteur((*j)->getPatPatho(), ORIGINE_PATH_PATHO + 0) ;
          }
          else if (false == pPatpathoItem->empty())
          {
            for (FatheredPatPathoIterVect j = pPatpathoItem->begin() ; pPatpathoItem->end() != j ; j++)
            {
              AjouteTmpEtiquette(&SonTempPatPatho, *it) ;
              SonTempPatPatho.ajouteVecteur((*j)->getPatPatho(), ORIGINE_PATH_PATHO + 1) ;
            }
          }
        }
      }
      // This son is "expanded"; get fresh information
      //
      else
      {
        size_t iSubLevel = 0 ;
        if (false == (*it)->estPlusZero())
        {
          AjouteTmpEtiquette(&SonTempPatPatho, *it) ;
          iSubLevel++ ;
        }
        (*it)->rapatrieTmpPpt(&SonTempPatPatho, iSubLevel) ;
      }
    }
    // Not "activated" but in duty; get current state
    else if (false == (*it)->VectorFils.empty())
    {
      // First step: is there some data there?
      NSPatPathoArray LocalTempPatPatho(pSuper) ;
      (*it)->rapatrieTmpPpt(&LocalTempPatPatho, 0) ;

      if (false == LocalTempPatPatho.empty())
      {
        if (false == (*it)->estPlusZero())
          AjouteTmpEtiquette(&SonTempPatPatho, *it) ;

        if (false == SonTempPatPatho.empty())
          SonTempPatPatho.InserePatPathoFille(&(SonTempPatPatho.back()), &LocalTempPatPatho) ;
        else
          SonTempPatPatho = LocalTempPatPatho ;
      }
    }

    if (false == SonTempPatPatho.empty())
      pTempPatPatho->InserePatPatho(pTempPatPatho->end(), &SonTempPatPatho, iLevel) ;
  }
}
catch (...)
{
  erreur("Exception BBItem::rapatrieTmpPpt.", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------// fermeture d'une boîte de dialogue// -----------------------------------------------------------------------------
void
BBItem::CmOkFermer(NSDialog* pDialog)
{
  if ((NSDialog*) NULL == pDialog)
    return ;

  donneRetourDlg(0) ;
  pDialog->CmOk() ;
  delete pDialog ;
  pDialog = (NSDialog*) 0 ;

  if (_pBBFilsPere)
  {
    if (_pBBFilsPere->PatPtahovide())
    {
      if (actifVide())
        _pBBFilsPere->Active() ;
      else
        _pBBFilsPere->Desactive() ;
    }
    else
      _pBBFilsPere->Active() ;
  }
}

// -----------------------------------------------------------------------------// fermeture d'une boîte de dialogue// -----------------------------------------------------------------------------
void
BBItem::CmCancelFermer(NSDialog* pDialog)
{
  if ((NSDialog*) NULL == pDialog)
    return ;

	donneRetourDlg(-1) ;
	okFermerDialogue(false) ;
	pDialog->Destroy(IDCANCEL) ;
	pDialog = (NSDialog*) 0 ;
}

// -----------------------------------------------------------------------------// Function     : void BBItem::ctrlNotification(BBFilsItem* pFilsItem, string* pIdentite, int etatInitial, int etatSouhaite, Message* pMessage, int indexFils)// Description  : Réponse à la notification d'activation d'un contrôle
// Arguments    : sIdentite 		-> coordonnées du contrôle
//						    etatInitial		-> état du contrôle avant activation
//						    etatSouhaite  -> état final désiré
//						    message				-> informations complémentaires
//                indexFils     -> index du fils de pFilsItem à développer
// Returns      : Rien
// -----------------------------------------------------------------------------
void
BBItem::ctrlNotification(BBFilsItem* pFilsItem, string* /*pIdentite*/, int /*etatInitial*/, int etatSouhaite, Message* pMessage, int indexFils)
{
  if ((BBFilsItem*) NULL == pFilsItem)
    return ;

  // TRAITEMENT DES TREEVIEW

  if (BF_DELETE == etatSouhaite) //  TreeView 	{
    //pour chacun des BBItem fils
    pFilsItem->DestructionManuelle(pMessage) ;
    //enlever pFilsItem de le liste des fils de son pére
    if (false == _aBBItemFils.empty())
    {
      BBiter it = _aBBItemFils.begin() ;
      for ( ; (_aBBItemFils.end() != it) && (*it != pFilsItem) ; it++)
        ;

      if (_aBBItemFils.end() != it)
        _aBBItemFils.erase(it) ;
    }
    //si le pére(this) n'a plus de fils il faut le désactiver
    if (_aBBItemFils.empty())
      if (false == actifVide())
        _pBBFilsPere->Desactive() ;
    return ;
  }

	// Si le fils est terminal, on laisse le contrôle faire à sa guise
  if (false == pFilsItem->FilsProlongeable)
	{
    if (pFilsItem->getItemTransfertData())
    {
      // Le contrôle se met lui même à jour
      pFilsItem->getItemTransfertData()->activeControle(etatSouhaite, pMessage) ;

      // Puis met à jour sa structure de transfert
      pFilsItem->getItemTransfertData()->Transfer(tdGetData) ;
    }

		// Si le contrôle désirait se sélectionner, on désactive ses frères incompatibles
		if (pFilsItem->isActif())
			pFilsItem->Active() ;
    else
      pFilsItem->Desactive() ;

		// Cas particulier de l'élément créateur de lésion non ouvreur de dialogue
		// (par exemple champ Edit) - s'il était ouvreur de dialogue, la lésion
		// serait traitée par la fonction creerDialogue()
		return ;
	}

	// Si le fils n'est pas terminal, on analyse
	// Premier cas : le contrôle désire se sélectionner
  if (BF_CHECKED == etatSouhaite)
	{
		// On développe la branche ou éventuellement la sous-branche indiquée par indexFils (si indexFils >= 0)
		int k = developper(pFilsItem, 0, indexFils) ;

    // Si le contrôle est père d'un autre contrôle situé dans la même boite
    // de dialogue, developper retourne une erreur 3, il faut gérer ce
    // contrôle comme un contrôle terminal, puisque c'est à l'utilisateur
    // d'instancier (ou non) le fils.
    if ((3 == k) && (pFilsItem->getItemTransfertData()))
    {
      pFilsItem->getItemTransfertData()->activeControle(etatSouhaite, pMessage) ;
      pFilsItem->getItemTransfertData()->Transfer(tdGetData) ;
    }

		// Si le contrôle a réussi à se sélectionner, on détruit les incompatibles
		if (pFilsItem->isActif())
			pFilsItem->Active() ;
    else
      pFilsItem->Desactive() ;
  }

	// Deuxième cas : le contrôle désire se déselectionner
  // Si c'est un controle "ON/OFF" (radiobouton, checkbox), celà veut
  // simplement dire qu'il était déjà coché et que l'utilisateur a cliqué
  // dessus. On a alors deux cas dans pPatPatho :
  //				1) L'élément n'a pas de fils et on le laisse se déselectionner
  //				2) L'élément a des fils et il doit se développer
	if (BF_UNCHECKED == etatSouhaite)
	{
    if (pFilsItem->PatPtahovide())
    {
      pFilsItem->Desactive() ;
      if (pFilsItem->getItemTransfertData())
        pFilsItem->getItemTransfertData()->activeControle(etatSouhaite, pMessage) ;
    }
    else
    {
      NSDlgFonction* pFct = (NSDlgFonction*) 0 ;

      // In such situation, the control cannot guess if it will be activated
      // or unchecked, so it doesn't fire its functions and we must do it there
      //
      if (pFilsItem->getItemTransfertData() && pFilsItem->getItemTransfertData()->getControl())
        pFct = pFilsItem->getItemTransfertData()->getControl()->getFonction() ;

      if (pFct)
        pFct->execute(NSDLGFCT_EXECUTE) ;

			// On développe la branche
			int k = developper(pFilsItem) ;

      // Si le contrôle est père d'un autre contrôle situé dans la même boite
      // de dialogue, developper retourne une erreur 3, il faut gérer ce
      // contrôle comme un contrôle terminal, puisque c'est à l'utilisateur
      // d'instancier (ou non) le fils.
      if ((3 == k) && (pFilsItem->getItemTransfertData()))
      {
        pFilsItem->getItemTransfertData()->activeControle(etatSouhaite, pMessage) ;
        pFilsItem->getItemTransfertData()->Transfer(tdGetData) ;
      }

			// Si le contrôle a réussi à se sélectionner, on détruit les incompatibles
      if (pFilsItem->isActif())
				pFilsItem->Active() ;
      else
        pFilsItem->Desactive() ;

      if (pFct)
        pFct->execute(NSDLGFCT_POSTEXEC) ;
    }
	}

	// Transmission de son état au contrôle
  if (pFilsItem->getItemTransfertData())
  {
    if (pFilsItem->isActif())
      pFilsItem->getItemTransfertData()->activeControle(BF_CHECKED, pMessage) ;
    else
      pFilsItem->getItemTransfertData()->activeControle(BF_UNCHECKED, pMessage) ;
  }
}

// -----------------------------------------------------------------------------// le BBItem détruit tous ses fils// -----------------------------------------------------------------------------
void
BBItem::DestructionManuelle(Message* pMessage)
{
  if (false == _aBBItemFils.empty())
    for (BBiter it = _aBBItemFils.begin() ; _aBBItemFils.end() != it ; it++)
      (*it)->DestructionManuelle(pMessage) ;
}

// -----------------------------------------------------------------------------
// Function     : int BBItem::DetruireExclus(int iRef)
// Description  : Destruction des contrôles incompatibles avec le contrôle qui vient d'etre sélectionné
// Arguments    : pointeur sur BBFilsItem
// Returns      : Rien
// Comment      : pFilsIt->Exclusion = aBBItemExclu[iRef]
// -----------------------------------------------------------------------------
void
BBItem::DetruireExclus(BBFilsItem* pFilsIt)
{
  if ((BBFilsItem*) NULL == pFilsIt)
    return ;

  // traitement spécial des BBFilsItem contrôlant un EditLexique
  // ils doivent demander à leur père (de type £C0;020) créateur de desactiver ses frères
  if (pFilsIt->getItemFather() && pFilsIt->getItemFather()->_pBBFilsPere)
	{
    string sIdentite = pFilsIt->getItemFather()->_pBBFilsPere->getItemLabel() ;
    if ((sIdentite.find(string("£C;")) != string::npos) || (sIdentite.find(string("/£C;")) != string::npos) ||
        (sIdentite.find(string("£CX")) != string::npos))
      pFilsIt->getItemFather()->_pBBFilsPere->getItemFather()->DetruireExclus(pFilsIt->getItemFather()->_pBBFilsPere) ;
	}

  if (pFilsIt->Exclusion.isEmpty())
    return ;

  string sExclus = pFilsIt->Exclusion.getFirstExcluded() ;
	while (string("") != sExclus)
	{
    if (sExclus != pFilsIt->getItemLabel())
    {
      BBiter iter = _aBBItemFils.begin() ;
      for ( ; (_aBBItemFils.end() != iter) && ((*iter)->getItemLabel() != sExclus) ; iter++)
        ;

      if (_aBBItemFils.end() != iter)
      {
        if (string("") != (*iter)->getItemLabel())
        {
          (*iter)->DesactiveFils() ;
          (*iter)->Desactive() ;

          if ((*iter)->getItemTransfertData()->getControl())
            (*iter)->getItemTransfertData()->activeControle(BF_UNCHECKED, (*iter)->getItemTransfertMsg()) ;
        }
      }
    }
    sExclus = pFilsIt->Exclusion.getNextExcluded(sExclus) ;
	}
}

bool
BBItem::isExcluded(BBFilsItem *pFilsIt1, BBFilsItem *pFilsIt2)
{
  if (((BBFilsItem*) NULL == pFilsIt1) || ((BBFilsItem*) NULL == pFilsIt2))
    return false ;

  return pFilsIt1->Exclusion.isExclusion(pFilsIt2->getItemLabel()) ;
}
// -----------------------------------------------------------------------------// Désactive tous les fils, mais conserve les patpathos en l'état// -----------------------------------------------------------------------------
void
BBItem::DesactiveFils()
{
  if (_aBBItemFils.empty())
    return ;

  for (BBiter iter = _aBBItemFils.begin() ; _aBBItemFils.end() != iter ; iter++)
  {
    (*iter)->DesactiveFils() ;
    (*iter)->Desactive() ;

    if (((*iter)->getItemTransfertData()) && ((*iter)->getItemTransfertData()->getControl()))
      (*iter)->getItemTransfertData()->activeControle(BF_UNCHECKED, (*iter)->getItemTransfertMsg()) ;
  }
}

// -----------------------------------------------------------------------------
// Désactive tous les fils et vide les patpathos
// -----------------------------------------------------------------------------
void
BBItem::RemetAZero()
{
  if (_aBBItemFils.empty())
    return ;

  // if (pPPTEnCours)
  //	pPPTEnCours->vider();

  for (BBiter iter = _aBBItemFils.begin() ; _aBBItemFils.end() != iter ; iter++)
  {
    (*iter)->DesactiveFils() ;
    if ((*iter)->getPatPatho())
      (*iter)->getPatPatho()->vider() ;
    (*iter)->RemetAZero() ;
  }
}

// -----------------------------------------------------------------------------// Désactive tous les fils et vide les patpathos// -----------------------------------------------------------------------------
void
BBItem::Redeveloppe()
{
	if (_pNSDialog)
    _pNSDialog->Destroy() ;

  okFermerDialogue(false) ;

  activer() ;}
void
BBItem::Redispatche()
{
  DispatcherPatPatho() ;  for (BBiter iter = _aBBItemFils.begin() ; _aBBItemFils.end() != iter ; iter++)
    (*iter)->Redispatche() ;
}

void
BBItem::ActivePere(BBFilsItem* pFilsIt)
{
  if ((BBFilsItem*) NULL == pFilsIt)
    return ;

	DetruireExclus(pFilsIt) ;
  if (_pBBFilsPere)
	{
    // Attention : si on ne teste que dansMemeDialogue, il y a un bug
    // (retour vers préambule dans écho-coeur efface les données descriptives)
    if ((_sNomDlg == _pBBFilsPere->getItemFather()->_sNomDlg) ||
                            dansMemeDialogue(_pBBFilsPere->getItemFather()))
    {
      if (_pBBFilsPere->getItemTransfertData())
        _pBBFilsPere->getItemTransfertData()->Active() ;
      if (_pBBFilsPere->getItemFather())
        _pBBFilsPere->getItemFather()->ActivePere(_pBBFilsPere) ;

      if ((_pBBFilsPere->getItemTransfertData()) &&
          (_pBBFilsPere->getItemTransfertData()->getControl()))
        _pBBFilsPere->getItemTransfertData()->activeControle(BF_CHECKED, _pBBFilsPere->getItemTransfertMsg()) ;
    }
	}
}

voidBBItem::DesactivePereFictif(BBFilsItem* pFilsIt)
{
  if ((BBFilsItem*) NULL == pFilsIt)
    return ;

  // bool tourner = true ;
  if (pFilsIt->getItemTransfertData())
    pFilsIt->getItemTransfertData()->Desactive() ;

  if ((_pBBFilsPere) && (_pBBFilsPere->getItemFather()))
  {
    // Si on n'est pas en mode correction, on désactive récursivement
    // les pères qui ne sont pas liés à des éléments de dialogue

    // En mode correction, les boites de dialogue intermédiaires ne sont
    // pas ouvertes ; il faut limiter la réaction en chaine en vérifiant
    // qu'on est bien dans le cadre d'une
    //if ((!(pBigBoss->Correction)) || (dansDialogue()))
    if (dansDialogue())
    {
      if ((dansMemeDialogue(_pBBFilsPere->getItemFather())) &&
          ((NSControle*) NULL == _pBBFilsPere->getItemTransfertData()->getControl()))
        _pBBFilsPere->getItemFather()->DesactivePereFictif(_pBBFilsPere) ;
    }
  }
}

// -----------------------------------------------------------------------------// Function     : int BBItem::Initialiser()// Description  : Répartition du codon entre les BBFilsItem de façon récursive
// Returns      : 0 si tout a bien fonctionné, 1 si on a eu un problème
// -----------------------------------------------------------------------------
int
BBItem::Initialiser()
{
	// Si le tableau de fils est vide, on sort immédiatement
	if (_aBBItemFils.empty())
		return 1 ;

  for (BBiter iter = _aBBItemFils.begin() ; _aBBItemFils.end() != iter ; iter++)
    (*iter)->Initialiser() ;

  return 0 ;
}

// -----------------------------------------------------------------------------// Function     : int BBItem::InitialiserSeparateur()// Description  : Répartition du codon entre les BBFilsItem quand il y a des séparateurs
// Returns      : 0 si tout a bien fonctionné, 1 si on a eu un problème
// -----------------------------------------------------------------------------
int
BBItem::InitialiserSeparateur()
{
	return 0 ;
}

// -----------------------------------------------------------------------------// Function     : int BBItem::InitialiserNumerique()// Description  : Répartition du codon entre les BBFilsItem quand il y n'y a pas de séparateur
// Returns      : 0 si tout a bien fonctionné, 1 si on a eu un problème
// -----------------------------------------------------------------------------
int
BBItem::InitialiserNumerique()
{
  return 0 ;
}

// -----------------------------------------------------------------------------
// Function     : BBFilsItem* BBItem::ChercheFils(string sIdentite, string sCheminParcouru)
// Description  : Recherche le BBFilsItem qui, pour la fenêtre en cours, correspond à un groupe et une étiquette donnée
// Retours      : un pointeur sur le BBFilsItem si on l'a trouvé, 0 sinon
// -----------------------------------------------------------------------------
BBFilsItem*
BBItem::ChercheFils(string sIdentite, string sCheminParcouru)
{
  if (true == _aBBItemFils.empty())
    return (BBFilsItem*) 0 ;

  string sCodeSenssCheminParcouru ;
  NSDico::donneCodeSens(&sCheminParcouru, &sCodeSenssCheminParcouru) ;

  string sCodeSenssIdentite ;
  NSDico::donneCodeSens(&sIdentite, &sCodeSenssIdentite) ;

	// Si aucun fils ne correspond directement, on cherche récursivement dans les BBItems qui leurs sont attachés
  //
	for (BBiter iter = _aBBItemFils.begin() ; _aBBItemFils.end() != iter ; iter++)
	{
    string sFilsLabel = (*iter)->getItemLabel() ;
    string sIter ;
		NSDico::donneCodeSens(&sFilsLabel, &sIter) ;

    string sCheminPossible = string("") ;
		if (string("") == sCodeSenssCheminParcouru)
			sCheminPossible = sIter ;
		else
			sCheminPossible = sCodeSenssCheminParcouru + string(1, cheminSeparationMARK) + sIter ;

		if ((sCheminPossible == sCodeSenssIdentite) &&
        ((NSControle*) NULL == (*iter)->getItemTransfertData()->getControl()))
			return *iter ;

    if (sCheminPossible == string(sCodeSenssIdentite, 0, strlen(sCheminPossible.c_str())))
    {
      if (false == (*iter)->VectorFils.empty())
      {
        for (BBiterFils iterfils = (*iter)->VectorFils.begin() ; (*iter)->VectorFils.end() != iterfils ; iterfils++)
        {
          BBItem*	pBBIt = *iterfils ;
          if (pBBIt)
          {
            //pBBFilsIt = pBBIt->ChercheFils(pCodeSenssIdentite, sCheminPossible);
            BBFilsItem* pBBFilsIt = pBBIt->ChercheFils(sIdentite, sCheminPossible) ;
            if (pBBFilsIt)
              return pBBFilsIt ;
          }
        }
      }
    }
  }

	// Si on est encore là, c'est qu'on n'a rien trouvé
	return (BBFilsItem*) 0 ;
}

// -----------------------------------------------------------------------------// Function     : TWindow * BBItem::donneFenetre()// Description  : Recherche le pointeur sur la fenêtre mére de la boîte de
//                dialogue à ouvrir (première fenêtre rencontrée chez les ancêtres)
// Returns      : le pointeur
// -----------------------------------------------------------------------------
TWindow *
BBItem::donneFenetre()
{
	// Si on est à l'origine, on dépend directement de la fenêtre principale
	// (Attention dansDialogue() dépend de cette valeur)

  // if (sLocalisation.find(cheminSeparationMARK) == string::npos)
  if ((BBFilsItem*) NULL == _pBBFilsPere)
	{
		if (_pView && _pView->IsWindow())
    	return (TWindow*) _pView ;
    else
    	return pContexte->GetMainWindow() ;
	}

	// Si le père dépend d'une boite de dialogue, on renvoie son pointeur	if (_pBBFilsPere->getItemFather()->getDialog())
		return (TWindow*)_pBBFilsPere->getItemFather()->getDialog() ;

	// Sinon on recherche récursivement	return (_pBBFilsPere->getItemFather()->donneFenetre()) ;
}

bool
BBItem::dansDialogue()
{
	if (donneFenetre() == pContexte->GetMainWindow())
    return false ;
  else
    return true ;
}

// -----------------------------------------------------------------------------
// Ce BBItem et *pAutre sont-ils dans le même dialogue ?
// Rappel : si un BBItem est dans un Archétype, sIdArchetype est l'ID de l'Arch.
//          si l'Archetype est lié à un RC dans une dll, szNomDlg est non vide
//          si l'Archétype contient ses contrôles, szNomDlg est vide)
//          si le BBItem provient d'un Fil guide non lanceur d'Archétype,
//          alors sIdArchetype est vide, mais pas szNomDlg
// -----------------------------------------------------------------------------
// Are this BBItem and *pAutre in the same dialog
// -----------------------------------------------------------------------------
bool
BBItem::dansMemeDialogue(BBItem *pAutre)
{
  if ((BBItem*) NULL == pAutre)
    return false ;

  // 1. Cas des archetypes
  if (((string("") != _sIdArchetype) || (string("") != pAutre->_sIdArchetype)) &&
                              (_sIdArchetype == pAutre->_sIdArchetype))
    return true ;

  // 2. Cas des dialogues
  if (((string("") != _sNomDlg) || (string("") != pAutre->_sNomDlg)) && (pAutre->_sNomDlg == _sNomDlg))
    return true ;

  return false ;
}

bool
BBItem::estPlusZero()
{
  return (_Data.getLevelShift() == string("+00+00")) ;
}


// -----------------------------------------------------------------------------
// Function     : int BBItem::actifVide()
// Description  : Annonce si le BBItem est (ou non) activé même si son codon
//						    est vide (sous niveaux non obligatoires)
// Returns      : 1 si le BBItem est activé, 0 sinon
// -----------------------------------------------------------------------------
bool
BBItem::actifVide()
{
	return _Data.actifVide() ;
}

// -----------------------------------------------------------------------------// Function     : int BBItem::uniciteLesion()// Description  : Annonce si la lésion est (ou non) unique
// Returns      : true si la lésion est unique, false sinon
// -----------------------------------------------------------------------------
bool
BBItem::uniciteLesion()
{
	return _Data.uniciteLesion() ;
}

// -----------------------------------------------------------------------------
// Function     : int BBItem::lienDialogue()
// Description  : Annonce si le BBItem est (ou non) rattaché à une boite de dialogue
// Retours      : 1 si le BBItem est rattaché, 0 sinon
// -----------------------------------------------------------------------------
bool
BBItem::lienDialogue()
{
	if (string("__CROS") == _sNomDlg)
		return false ;

	if ((string("") != _sNomDlg) && (string::npos != _sNomDlg.find_first_not_of(string(" "))))
		return true ;

  string sLang = pContexte->getUserLanguage() ;

  if (existArchetypeDialog(sLang))
    return true ;	return false ;}

void
BBItem::setItemData(BBItemData* pIt)
{
  if ((BBItemData*) NULL == pIt)
    return ;

  _Data = *pIt ;
}

// -----------------------------------------------------------------------------// Function     : int BBItem::lienFonction()// Description  : Annonce si le BBItem est (ou non) rattaché à une fonction
// Returns      : 1 si le BBItem est rattaché, 0 sinon
// -----------------------------------------------------------------------------
bool
BBItem::lienFonction()
{
	if ((_Data.nomFonction[0] != '\0') &&
      (strspn(_Data.nomFonction, " ") != strlen(_Data.nomFonction)))
		return true ;

	return false ;}

// -----------------------------------------------------------------------------
// Function     : int BBItem::ouvreDialog()
// Description  : Annonce si le BBItem ouvre (ou non) un dialogue (dialogue éventuellement rattaché à un archétype)
// Retours      : 1 si le BBItem est rattaché, 0 sinon
// -----------------------------------------------------------------------------
bool
BBItem::ouvreDialog()
{
	return (_Data.ouvreDlg() || _Data.ouvreArchetype()) ;
}

// -----------------------------------------------------------------------------
// Function     : int BBItem::ouvreArchetype()
// Description  : Annonce si le BBItem ouvre (ou non) un archétype
// Returns      : 1 si le BBItem est ouvreur d'archetype, 0 sinon
// -----------------------------------------------------------------------------
bool
BBItem::ouvreArchetype()
{
	return _Data.ouvreArchetype() ;
}
stringBBItem::donneVraiChemin(){  if ((BBFilsItem*) NULL == _pBBFilsPere)    return string("") ;  string sChemin = string("") ;  bool bPlusZero = _pBBFilsPere->estPlusZero() ;  if (false == bPlusZero)    sChemin = _pBBFilsPere->getItemLabel() ;  BBItem* pBBcurrent = _pBBFilsPere->getItemFather() ;  while (pBBcurrent && pBBcurrent->_pBBFilsPere)  {    bPlusZero = pBBcurrent->_pBBFilsPere->estPlusZero() ;    if (false == bPlusZero)    {      if (string("") != sChemin)        sChemin = string(1, cheminSeparationMARK) + sChemin ;      sChemin = pBBcurrent->_pBBFilsPere->getItemLabel() + sChemin ;    }    pBBcurrent = pBBcurrent->_pBBFilsPere->getItemFather() ;  }  if (pBBcurrent && pBBcurrent->_pBigBoss)  {    if (string("") != sChemin)      sChemin = string(1, cheminSeparationMARK) + sChemin ;    sChemin = pBBcurrent->_pBigBoss->getLexiqueModule() + sChemin ;  }  return sChemin ;}voidBBItem::cutBadDialogBranches(NSDialog* pDlg){	if (((NSDialog*) NULL == pDlg) || (true == _aBBItemFils.empty()))		return ;	for (BBiter iter = _aBBItemFils.begin() ; _aBBItemFils.end() != iter ; iter++)  	(*iter)->cutBadDialogBranches(pDlg) ;}boolBBItem::isBadDialogBranch(NSDialog* pDlg){	if ((NSDialog*) NULL == pDlg)		return false ;	if (true == _aBBItemFils.empty())		return true ;	for (BBiter iter = _aBBItemFils.begin() ; _aBBItemFils.end() != iter ; iter++)  	if (false == (*iter)->isBadDialogBranch(pDlg))    	return false ;	return true ;}voidBBItem::setParseur(nsarcParseur *pArc){  _pParseur = pArc ;}Cdialogbox*BBItem::getArchetypeDialog(string sLang){  if ((nsarcParseur*) NULL == _pParseur)    return (Cdialogbox*) 0 ;  nsarcParseur *pArcParseur = dynamic_cast<nsarcParseur*>(_pParseur) ;  if (pArcParseur)
  {
    if ((Carchetype*) NULL == pArcParseur->getArchetype())
      return (Cdialogbox*) 0 ;

    return pArcParseur->getArchetype()->getDialogBox(sLang) ;
  }

/*
  nsGuidelineParseur *pDecisionTreeParseur = dynamic_cast<nsGuidelineParseur*>(_pParseur) ;
  if (pDecisionTreeParseur)  {    if (NULL == pDecisionTreeParseur->getDecisionTree())
      return NULL ;

    return pDecisionTreeParseur->getDecisionTree()->getDialogBox(sLang) ;
  }*/  return (Cdialogbox*) 0 ;}boolBBItem::existArchetypeDialog(string sLang){  return (getArchetypeDialog(sLang) != NULL) ;}stringBBItem::getArchetypeFunction(){  if ((nsarcParseur*) NULL == _pParseur)    return string("") ;  nsarcParseur *pArcParseur = dynamic_cast<nsarcParseur*>(_pParseur) ;  if (pArcParseur)
  {
    if ((Carchetype*) NULL == pArcParseur->getArchetype())
      return string("") ;

    return pArcParseur->getArchetype()->getFunction() ;
  }

/*
  nsDecisionTreeParseur *pDecisionTreeParseur = dynamic_cast<nsDecisionTreeParseur*>(_pParseur) ;
  if (pDecisionTreeParseur)  {    if (NULL == pDecisionTreeParseur->getDecisionTree())
      return string("") ;

    return pDecisionTreeParseur->getDecisionTree()->getFunction() ;
  }*/  return string("") ;}Creferences*BBItem::getArchetypeReference(){  if ((nsarcParseur*) NULL == _pParseur)    return (Creferences*) 0 ;  nsarcParseur *pArcParseur = dynamic_cast<nsarcParseur*>(_pParseur) ;  if (pArcParseur)
  {
    if ((Carchetype*) NULL == pArcParseur->getArchetype())
      return (Creferences*) 0 ;

    return pArcParseur->getArchetype()->getReference() ;
  }

/*
  nsDecisionTreeParseur *pDecisionTreeParseur = dynamic_cast<nsDecisionTreeParseur*>(_pParseur) ;
  if (pDecisionTreeParseur)  {    if (NULL == pDecisionTreeParseur->getDecisionTree())
      return NULL ;

    return pDecisionTreeParseur->getDecisionTree()->getReference() ;
  }*/  return (Creferences*) 0 ;}stringBBItem::getArchetypeName(){  if ((nsarcParseur*) NULL == _pParseur)    return string("") ;  nsarcParseur *pArcParseur = dynamic_cast<nsarcParseur*>(_pParseur) ;  if (pArcParseur)
  {
    if ((Carchetype*) NULL == pArcParseur->getArchetype())
      return string("") ;

    return pArcParseur->getArchetype()->getName() ;
  }

/*
  nsDecisionTreeParseur *pDecisionTreeParseur = dynamic_cast<nsDecisionTreeParseur*>(_pParseur) ;
  if (pDecisionTreeParseur)  {    if (NULL == pDecisionTreeParseur->getDecisionTree())
      return string("") ;

    return pDecisionTreeParseur->getDecisionTree()->getName() ;
  }*/  return string("") ;}Citem*BBItem::getArchetypeRootItem(){  if ((nsarcParseur*) NULL == _pParseur)    return (Citem*) 0 ;  nsarcParseur *pArcParseur = dynamic_cast<nsarcParseur*>(_pParseur) ;  if (pArcParseur)
  {
    if ((Carchetype*) NULL == pArcParseur->getArchetype())
      return (Citem*) 0 ;

    return pArcParseur->getArchetype()->getRootItem() ;
  }

/*
  nsDecisionTreeParseur *pDecisionTreeParseur = dynamic_cast<nsDecisionTreeParseur*>(_pParseur) ;
  if (pDecisionTreeParseur)  {    if (NULL == pDecisionTreeParseur->getDecisionTree())
      return NULL ;

    return pDecisionTreeParseur->getDecisionTree()->getRootItem() ;
  }*/  return (Citem*) 0 ;}string BBItem::getItemID(){  return _Data.getID() ;}
string BBItem::getItemGroupID()
{
  return _Data.getGroupID() ;
}

string BBItem::getItemSemanticPath()
{
  return _Data.getSemanticPath() ;
}
string BBItem::getItemValidityCase(){  return _Data.getValidityCase() ;}string BBItem::getItemLabel(){  return _Data.getLabel() ;}
string BBItem::getItemDialogFile()
{
  return _Data.getDialogFile() ;
}

string BBItem::getItemDialogName()
{
  return _Data.getDialogName() ;
}

string BBItem::getItemFctFile()
{
  return _Data.getFctFile() ;
}

string BBItem::getItemFctName()
{
  return _Data.getFctName() ;
}

string BBItem::getItemLevelShift()
{
  return _Data.getLevelShift() ;
}

string BBItem::getItemSonsList()
{
  return _Data.getSonsList() ;
}

string BBItem::getItemSonsRules()
{
  return _Data.getSonsRules() ;
}

string BBItem::getItemSortRules(){  return _Data.getSortRules() ;}void BBItem::setItemLabel(string sLabel){  _Data.setLabel(sLabel) ;}
void BBItem::setItemDialogFile(string sDlgF)
{
  _Data.setDialogFile(sDlgF) ;
}

void BBItem::setItemDialogName(string sDlgN)
{
  _Data.setDialogName(sDlgN) ;
}

void BBItem::setItemFctFile(string sFctF)
{
  _Data.setFctFile(sFctF) ;
}

void BBItem::setItemFctName(string sFctN)
{
  _Data.setFctName(sFctN) ;
}

void BBItem::setItemLevelShift(string sLevel)
{
  _Data.setLevelShift(sLevel) ;
}

void BBItem::setItemSonsList(string sSons)
{
  _Data.setSonsList(sSons) ;
}

void BBItem::setItemSonsRules(string sRules)
{
  _Data.setSonsRules(sRules) ;
}

void BBItem::setItemSortRules(string sSort){  _Data.setSortRules(sSort) ;}void BBItem::setItemAutomatic(bool bAuto){  _Data.setAutomatic(bAuto) ;}void BBItem::setItemEmptyActivation(bool bEA)
{
  _Data.setEmptyActivation(bEA) ;
}

void BBItem::setItemUnicity(bool bUniq)
{
  _Data.setUnicity(bUniq) ;
}

void BBItem::setItemOpenDialog(bool bOpen)
{
  _Data.setOpenDialog(bOpen) ;
}

void BBItem::setItemOpenArchetype(bool bOpen)
{
  _Data.setOpenArchetype(bOpen) ;
}
void BBItem::setItemNeeded(bool bNeed){  _Data.setNeeded(bNeed) ;}