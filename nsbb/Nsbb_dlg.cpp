#include<owl\window.h>
#include "nsbb\nspatpat.h"

#include "nsbb\nschkbox.h"#include "nsbb\nsbouton.h"
#include "nsbb\nsradiob.h"
#include "nsbb\nsedit.h"
#include "nsbb\nscombo.h"
#include "nsbb\nsgroupe.h"
#include "nautilus\nssuper.h"
#include "nautilus\nscsvue.h"
#include "nsbb\nsedilex.h"
#include "partage\nsdivfct.h"

#include "nsbb\nsbb.h"
#include "nsbb\nsbbitem.h"
#include "nsbb\nsbbsmal.h"
#include "nsbb\nsdlg.h"
#include "nsbb\nsbb_dlg.h"
#include "nsbb\nsbbdivfct.h"
#include "nsbb\nstrnode.h"
#include "nsbb\nslistwind.h"
#include "nssavoir\nsguide.h"
#include "nsepisod\nsldvuti.h"
//#include "nautilus\nscsvue.h"

long NSControle::lObjectCount = 0 ;

//***************************************************************************
// Implémentation des méthodes NSControle
//***************************************************************************

//
// Constructeur par défaut
//
NSControle::NSControle(NSContexte* pCtx)
           :NSRoot(pCtx)
{
	_pNSDialog         = (NSDialog*)       0 ;
  _pMUEView          = (NSBBMUEView*)    0 ;
	_pTransfert        = (NSTransferInfo*) 0 ;
	_pNSCtrl           = (void*)           0 ;
	_sIdentite         = string("") ;
	_iType             = isUndefined ;
	_pNSDlgFct         = (NSDlgFonction*)  0 ;
	_bGestionMultiple  = false ;
  _bVisible          = true ;
  _bDisconnected     = false ;

	_sFilling          = string("") ;
	_sFillingFilter    = string("") ;
  _sFillingStarter   = string("") ;
	// sFillingEndPeriod = string("") ;
	_sPathControl      = string("") ;

  lObjectCount++ ;
}

////  Constructeur avec recherche de liaison
//
NSControle::NSControle(NSContexte* pCtx, BBItem* pBBItem, string sIdent, string sDlgFct)
           :NSRoot(pCtx)
{
try
{
	_sIdentite        = sIdent ;
	_iType            = isUndefined ;
	_pNSDialog        = (NSDialog*)       0 ;
  _pMUEView         = (NSBBMUEView*)    0 ;
	_pNSCtrl	        = (void*)           0 ;
	_pTransfert       = (NSTransferInfo*) 0 ;
	_bGestionMultiple = false ;
  _bVisible         = true ;
  _bDisconnected    = false ;

	_sFilling          = string("") ;
	_sFillingFilter    = string("") ;
  _sFillingStarter   = string("") ;
	// sFillingEndPeriod = string("") ;
	_sPathControl      = string("") ;

	lierTransfert(pBBItem) ;

	if ((string("") != sDlgFct) && pBBItem)
		_pNSDlgFct = new NSDlgFonction(pContexte, pBBItem->_pBigBoss, pBBItem,
                                                  sDlgFct, this, pNSResModule) ;
	else
		_pNSDlgFct = (NSDlgFonction*) 0 ;

  lObjectCount++ ;
}
catch (...)
{
	erreur("Exception NSControle ctor.", standardError, 0);
}
}

////  Destructeur
//
NSControle::~NSControle()
{
  lObjectCount-- ;

	//
	// Ne pas faire "delete pTransfert", car l'objet pointé par pTransfert est
	// créé et détruit par un objet de type BBFilsItem
	//
	// En théorie, le travail a déjà été réalisé par le BBFilsItem
	// lorsqu'il effectue okFermerDialogue()
	//
 	if (_pNSDlgFct)
  {
  	delete _pNSDlgFct ;
    _pNSDlgFct = (NSDlgFonction*) 0 ;
  }

  if (_pNSCtrl)
  {
    switch (_iType)
    {
    	case isCaseACocher :
      	(static_cast<NSCheckBox*>(_pNSCtrl))->pControle = 0 ;
        break ;

      case isRadioBtn	:
      	(static_cast<NSRadioButton*>(_pNSCtrl))->pControle = 0 ;
        break ;

      case isEdit	:
      	(static_cast<NSEdit*>(_pNSCtrl))->setControl((NSControle*) 0) ;
        break ;

      case isEditDate	:
      	(static_cast<NSEditDate*>(_pNSCtrl))->setControl((NSControle*) 0) ;
        break ;

      case isEditDateHeure	:
      	(static_cast<NSEditDateHeure*>(_pNSCtrl))->setControl((NSControle*) 0) ;
        break ;

      case isTreeNode	:      	(static_cast<NSTreeNode*>(_pNSCtrl))->_pControle = 0 ;
        break ;

      case isTreeWindow :
      	(static_cast<NSTreeWindow*>(_pNSCtrl))->setControl((NSControle*) 0) ;
        break ;

      case isAdrListWindow :
      	(static_cast<NSAdrListWindow*>(_pNSCtrl))->pControle = 0 ;
        break ;

      case isCorListWindow :
      	(static_cast<NSCorListWindow*>(_pNSCtrl))->pControle = 0 ;
        break ;

      case isNSCSVue :      	(static_cast<NSCsVue*>(_pNSCtrl))->setControl((NSControle*) 0) ;
        break ;

      case isEditLexique :
      	(static_cast<NSEditLexique*>(_pNSCtrl))->pControle = 0 ;
        break ;

      case isEditLexiqueDerive :      	(static_cast<NSEditLexiqueDerive*>(_pNSCtrl))->pControle = 0 ;
        break ;

      case isEditNoLexique :
      	(static_cast<NSEditNoLex*>(_pNSCtrl))->setControl((NSControle*) 0) ;
        break ;

      case isStatic :
      	(static_cast<NSStatic*>(_pNSCtrl))->pControle = 0 ;
        break ;

      case isComboClassif :
      	(static_cast<NSComboClassif*>(_pNSCtrl))->pControle = 0 ;
        break ;

      case isComboSemantique :
      	(static_cast<NSComboSemantique*>(_pNSCtrl))->pControle = 0 ;
        break ;

      case isHistoryListWindow :
      	(static_cast<NSHistorizedListWindow*>(_pNSCtrl))->pControle = 0 ;
        break ;

      case isHistoryValListWindow :
      	(static_cast<NSHistorizedValListWindow*>(_pNSCtrl))->_pControle = 0 ;
        break ;
    }
	}

  // Disconnecting from controler objects
  //
	if (_pTransfert)
	{
  	_pTransfert->setControl((NSControle*) 0) ;
    _pTransfert = (NSTransferInfo*) 0 ;
	}
}
//
// Can we close the mother dialog box ?
//
bool
NSControle::canWeClose()
{
	if ((void*) NULL == _pNSCtrl)
		return true ;

	switch (_iType)
	{
  	case isCaseACocher :
    	return (static_cast<NSCheckBox*>(_pNSCtrl))->canWeClose() ;

    case isRadioBtn	:
    	return (static_cast<NSRadioButton*>(_pNSCtrl))->canWeClose() ;

    case isEdit	:
    	return (static_cast<NSEdit*>(_pNSCtrl))->canWeClose() ;

    case isEditDate	:
    	return (static_cast<NSEditDate*>(_pNSCtrl))->canWeClose() ;

    case isEditDateHeure	:
    	return (static_cast<NSEditDateHeure*>(_pNSCtrl))->canWeClose() ;

    case isTreeNode	:
    	return (static_cast<NSTreeNode*>(_pNSCtrl))->canWeClose() ;

    case isTreeWindow :
    	return (static_cast<NSTreeWindow*>(_pNSCtrl))->canWeClose() ;

    case isAdrListWindow :
    	return (static_cast<NSAdrListWindow*>(_pNSCtrl))->canWeClose();

    case isCorListWindow :
    	return (static_cast<NSCorListWindow*>(_pNSCtrl))->canWeClose();

    case isNSCSVue :    	return (static_cast<NSCsVue*>(_pNSCtrl))->canWeClose() ;
    case isEditLexique :
    	return (static_cast<NSEditLexique*>(_pNSCtrl))->canWeClose() ;

    case isEditLexiqueDerive :    	return (static_cast<NSEditLexiqueDerive*>(_pNSCtrl))->canWeClose() ;
    case isEditNoLexique :
    	return (static_cast<NSEditNoLex*>(_pNSCtrl))->canWeClose() ;

    case isStatic :
    	return (static_cast<NSStatic*>(_pNSCtrl))->canWeClose() ;

    case isGroup :
    	return (static_cast<NSGroupBox*>(_pNSCtrl))->canWeClose() ;

    case isComboClassif :
    	return (static_cast<NSComboClassif*>(_pNSCtrl))->canWeClose() ;

    case isComboSemantique :
    	return (static_cast<NSComboSemantique*>(_pNSCtrl))->canWeClose() ;

    case isHistoryListWindow :
    	return (static_cast<NSHistorizedListWindow*>(_pNSCtrl))->canWeClose() ;

    case isHistoryValListWindow :
    	return (static_cast<NSHistorizedValListWindow*>(_pNSCtrl))->canWeClose() ;
	}

	return true ;
}

HWND
NSControle::getHWND()
{
	if ((void*) NULL == _pNSCtrl)
  	return 0 ;

  switch (_iType)
  {
    case isCaseACocher :
    	return (static_cast<NSCheckBox*>(_pNSCtrl))->HWindow ;

    case isRadioBtn	:
    	return (static_cast<NSRadioButton*>(_pNSCtrl))->HWindow ;

    case isEdit	:
    	return (static_cast<NSEdit*>(_pNSCtrl))->HWindow ;

    case isEditDate	:
    	return (static_cast<NSEditDate*>(_pNSCtrl))->HWindow ;

    case isEditDateHeure	:
    	return (static_cast<NSEditDateHeure*>(_pNSCtrl))->HWindow ;

    case isTreeNode	:
    	return 0 ;

    case isTreeWindow :
    	return (static_cast<NSTreeWindow*>(_pNSCtrl))->HWindow ;

    case isAdrListWindow :
    	return (static_cast<NSAdrListWindow*>(_pNSCtrl))->HWindow ;

    case isCorListWindow :
    	return (static_cast<NSCorListWindow*>(_pNSCtrl))->HWindow ;

    case isNSCSVue :    	return (static_cast<NSCsVue*>(_pNSCtrl))->HWindow ;
    case isEditLexique :
    	return (static_cast<NSEditLexique*>(_pNSCtrl))->HWindow ;

    case isEditLexiqueDerive :    	return 0 ;
    case isEditNoLexique :
    	return (static_cast<NSEditNoLex*>(_pNSCtrl))->HWindow ;

    case isStatic :
    	return (static_cast<NSStatic*>(_pNSCtrl))->HWindow ;

    case isGroup :
    	return (static_cast<NSGroupBox*>(_pNSCtrl))->HWindow ;

    case isComboClassif :
    	return (static_cast<NSComboClassif*>(_pNSCtrl))->HWindow ;

    case isComboSemantique :
    	return (static_cast<NSComboSemantique*>(_pNSCtrl))->HWindow ;

    case isHistoryListWindow :
    	return (static_cast<NSHistorizedListWindow*>(_pNSCtrl))->HWindow ;

  	case isHistoryValListWindow :
    	return (static_cast<NSHistorizedValListWindow*>(_pNSCtrl))->HWindow ;
  }

	return (HWND) 0 ;
}

//
// Constructeur copie
//
NSControle::NSControle(const NSControle& src)
           :NSRoot(src.pContexte)
{
try
{
	// Attention : l'objet pointé par pTransfert doit rester unique.
	//             Ne pas le dupliquer.
	_pTransfert = src._pTransfert ;
	_pNSDialog  = src._pNSDialog ;
  _pMUEView   = src._pMUEView ;
	_pNSCtrl	  = src._pNSCtrl ;
	_sIdentite  = src._sIdentite ;
	_iType      = src._iType ;
  _bVisible   = src._bVisible ;

	_bGestionMultiple = src._bGestionMultiple ;
  _bDisconnected    = src._bDisconnected ;

	if (src._pNSDlgFct)
		_pNSDlgFct = new NSDlgFonction(pContexte, src._pNSDlgFct->getBigBoss(),
                                      src._pNSDlgFct->getItem(),
                                      src._pNSDlgFct->getFonction(), this) ;
	else
		_pNSDlgFct = (NSDlgFonction*) 0 ;

	_sFilling          = src._sFilling ;
	_sFillingFilter    = src._sFillingFilter ;
  _sFillingStarter   = src._sFillingStarter ;
	// sFillingEndPeriod = src.sFillingEndPeriod ;
	_sPathControl      = src._sPathControl ;

  lObjectCount++ ;
}
catch (...)
{
	erreur("Exception NSControle copy ctor.", standardError, 0) ;
}
}

void
NSControle::activateParent()
{
	if (_pNSDialog)
		_pNSDialog->activateParent() ;
}

/*void
NSControle::SetFocus()
{
    activateParent();
}   */


// --------------------------------------------------------------------------// -------------------- Méthodes de NSControleVector ------------------------
// --------------------------------------------------------------------------

//
// Constructeur copie
//
NSControleVector::NSControleVector(const NSControleVector& rv)
                 :NSControleVectorCtrl()
{
try
{
	if (false == rv.empty())
		for (iterNSControleConst i = rv.begin() ; rv.end() != i ; i++)
			push_back(new NSControle((*i)->pContexte)) ;
}
catch (...)
{
	erreur("Exception NSControleVector copy ctor.", standardError, 0) ;
}
}

//
// Destructeur
//
void
NSControleVector::vider()
{
	if (empty())
  	return ;

	for (iterNSControle i = begin() ; end() != i ; )
	{
  	delete *i ;
    erase(i) ;
  }
}

NSControleVector::~NSControleVector(){
	vider() ;
}

//// Opérateur d'affectation
//
NSControleVector&
NSControleVector::operator=(const NSControleVector& src)
{
	if (this == &src)
  	return *this ;

try
{
	vider() ;

  if (false == src.empty())  	for (iterNSControleConst i = src.begin() ; src.end() != i ; i++)
    	push_back(new NSControle((*i)->pContexte)) ;
  return *this ;
}
catch (...)
{
	erreur("Exception NSControleVector (=).", standardError, 0) ;
	return *this ;
}
}

//// Demande au contrôle de se mettre dans l'état voulu
//
// Arguments:  activation : état souhaité
//             message	  : renseignements complémentaires
//
void
NSControle::activeControle(int activation, Message* pMessage)
{
	if ((void*) NULL == _pNSCtrl)
  	return ;

	switch (_iType)  {
  	case isCaseACocher :
    	(static_cast<NSCheckBox*>(_pNSCtrl))->
                                        activeControle(activation, pMessage ) ;
      break ;

    case isRadioBtn	:
    	(static_cast<NSRadioButton*>(_pNSCtrl))->
                                        activeControle(activation, pMessage ) ;
      break ;

    case isEdit	 	   :    case isEditDate  :    case isEditDateHeure :    	(static_cast<NSEdit*>(_pNSCtrl))->
                                        activeControle(activation, pMessage ) ;
      break ;

    case isTreeNode	   :
    	(static_cast<NSTreeNode*>(_pNSCtrl))->
                                        activeControle(activation, pMessage ) ;
      break ;

    case isTreeWindow  :
            (static_cast<NSTreeWindow*>(_pNSCtrl))->
                                        activeControle(activation, pMessage );
            break ;

        case isAdrListWindow  :
            (static_cast<NSAdrListWindow*>(_pNSCtrl))->
                                        activeControle(activation, pMessage );
            break ;

        case isCorListWindow  :
            (static_cast<NSCorListWindow*>(_pNSCtrl))->
                                        activeControle(activation, pMessage );
            break ;

        case isNSCSVue     :            (static_cast<NSCsVue*>(_pNSCtrl))->
                                        activeControle(activation, pMessage );
            break ;

        case isBtn		   :            (static_cast<NSButton*>(_pNSCtrl))->
                                        activeControle(activation, pMessage );
            break ;

        case isEditLexique :            (static_cast<NSEditLexique*>(_pNSCtrl))->
                                        activeControle(activation, pMessage );
            break ;

        case isEditLexiqueDerive :            (static_cast<NSEditLexiqueDerive*>(_pNSCtrl))->
                                        activeControle(activation, pMessage );
            break ;

        case isEditNoLexique :
            (static_cast<NSEditNoLex*>(_pNSCtrl))->
                                        activeControle(activation, pMessage );
            break ;

        case isComboClassif :
            (static_cast<NSComboClassif*>(_pNSCtrl))->
                                        activeControle(activation, pMessage );
            break ;

        case isComboSemantique :
            (static_cast<NSComboSemantique*>(_pNSCtrl))->
                                        activeControle(activation, pMessage );
            break ;

        case isHistoryListWindow :
            (static_cast<NSHistorizedListWindow*>(_pNSCtrl))->
                                        activeControle(activation, pMessage );
            break ;

        case isHistoryValListWindow :
            (static_cast<NSHistorizedValListWindow*>(_pNSCtrl))->
                                        activeControle(activation, pMessage );
            break ;
    }
}

////  Transfère les données du contrôle vers le TransfertInfo
//						et vice-versa suivant la direction
//
//  Arguments :	direction : tdGetData ou tdSetData
//
//  Returns :	1 si le transfert a fonctionné
//				0 sinon
uint
NSControle::Transfer(TTransferDirection direction)
{
	if ((void*) NULL == _pNSCtrl)
		return 0 ;

	switch (_iType)
	{
    case isCaseACocher :
        return (static_cast<NSCheckBox*>(_pNSCtrl))->
                            Transferer(direction,
                                       &(_pTransfert->_iActif),
                                       _pTransfert->getTransfertMessage()) ;

    case isRadioBtn	 :
        return (static_cast<NSRadioButton*>(_pNSCtrl))->
                            Transferer(direction,
                                       &(_pTransfert->_iActif),
                                       _pTransfert->getTransfertMessage()) ;

    case isTreeNode	 :
        return (static_cast<NSTreeNode*>(_pNSCtrl))->
                            Transferer(direction,
                                       &(_pTransfert->_iActif),
                                       _pTransfert->getTransfertMessage()) ;

    case isEdit	 	       :
    case isEditDate      :
    case isEditDateHeure :
        return (static_cast<NSEdit*>(_pNSCtrl))->
                            Transferer(direction,
                                       &(_pTransfert->_iActif),
                                       _pTransfert->getTransfertMessage()) ;

    case isEditLexique :
        return (static_cast<NSEditLexique*>(_pNSCtrl))->
                            Transferer(direction,
                                       &(_pTransfert->_iActif),
                                       _pTransfert->getTransfertMessage()) ;

    case isEditLexiqueDerive :
        return (static_cast<NSEditLexiqueDerive*>(_pNSCtrl))->
                            Transferer(direction,
                                       &(_pTransfert->_iActif),
                                       _pTransfert->getTransfertMessage()) ;

    case isEditNoLexique :
            return (static_cast<NSEditNoLex*>(_pNSCtrl))->
                                Transferer(direction,
                                           &(_pTransfert->_iActif),
                                           _pTransfert->getTransfertMessage()) ;

    case isComboClassif :
          return (static_cast<NSComboClassif*>(_pNSCtrl))->
                              Transferer(direction,
                                         &(_pTransfert->_iActif),
                                         _pTransfert->getTransfertMessage()) ;

    case isComboSemantique :
          return (static_cast<NSComboSemantique*>(_pNSCtrl))->
                              Transferer(direction,
                                         &(_pTransfert->_iActif),
                                         _pTransfert->getTransfertMessage()) ;

	}
	return 1 ;
}

uintNSControle::TransferFinal(TTransferDirection direction)
{
  if ((void*) NULL == _pNSCtrl)
    return 0 ;

  switch (_iType)
  {
    case isCaseACocher :
            return (static_cast<NSCheckBox*>(_pNSCtrl))->Transferer(direction,
                                            &(_pTransfert->_iActif),
                                            _pTransfert->getTransfertMessage()) ;

        case isRadioBtn	:
            return (static_cast<NSRadioButton*>(_pNSCtrl))->Transferer(direction,
                                            &(_pTransfert->_iActif),
                                            _pTransfert->getTransfertMessage()) ;

        case isTreeNode	:
            return (static_cast<NSTreeNode*>(_pNSCtrl))->TransfererFinal(direction,
                                            &(_pTransfert->_iActif),
                                            _pTransfert->getTransfertMessage()) ;

        case isEdit	:
            return (static_cast<NSEdit*>(_pNSCtrl))->Transferer(direction,
                                            &(_pTransfert->_iActif),
                                            _pTransfert->getTransfertMessage()) ;

        case isEditDate	:
            return (static_cast<NSEditDate*>(_pNSCtrl))->Transferer(direction,
                                            &(_pTransfert->_iActif),
                                            _pTransfert->getTransfertMessage()) ;

        case isEditDateHeure	:
            return (static_cast<NSEditDateHeure*>(_pNSCtrl))->Transferer(direction,
                                            &(_pTransfert->_iActif),
                                            _pTransfert->getTransfertMessage()) ;

        case isEditLexique :
            return (static_cast<NSEditLexique*>(_pNSCtrl))->Transferer(direction,
                                            &(_pTransfert->_iActif),
                                            _pTransfert->getTransfertMessage()) ;

        case isEditLexiqueDerive :
            return (static_cast<NSEditLexiqueDerive*>(_pNSCtrl))->Transferer(direction,
                                            &(_pTransfert->_iActif),
                                            _pTransfert->getTransfertMessage()) ;

        case isEditNoLexique :
            return (static_cast<NSEditNoLex*>(_pNSCtrl))->Transferer(direction,
                                            &(_pTransfert->_iActif),
                                            _pTransfert->getTransfertMessage()) ;

        case isComboClassif :
            return (static_cast<NSComboClassif*>(_pNSCtrl))->Transferer(direction,
                                            &(_pTransfert->_iActif),
                                            _pTransfert->getTransfertMessage()) ;

        case isComboSemantique :
            return (static_cast<NSComboSemantique*>(_pNSCtrl))->Transferer(direction,
                                            &(_pTransfert->_iActif),
                                            _pTransfert->getTransfertMessage()) ;

       /* case isHistoryListWindow :
            return (static_cast<NSHistorizedListWindow*>(_pNSCtrl))->Transferer(direction,
                                            &(_pTransfert->_iActif),
                                            _pTransfert->getTransfertMessage()) ;  */
	}
	return 1 ;
}

uint
NSControle::TempTransfer()
{
  if ((void*) NULL == _pNSCtrl)
    return 0 ;

  switch (_iType)
  {
    case isCaseACocher :
      return (static_cast<NSCheckBox*>(_pNSCtrl))->
                                TempTransferer(&(_pTransfert->_iTmpActif),
                                           _pTransfert->getTmpTransfertMessage()) ;

    case isRadioBtn	 :
      return (static_cast<NSRadioButton*>(_pNSCtrl))->
                                TempTransferer(&(_pTransfert->_iTmpActif),
                                           _pTransfert->getTmpTransfertMessage()) ;

    case isTreeNode	 :
      return (static_cast<NSTreeNode*>(_pNSCtrl))->
                                TempTransferer(&(_pTransfert->_iTmpActif),
                                           _pTransfert->getTmpTransfertMessage()) ;

    case isEdit	 	       :
    case isEditDate	 	   :
    case isEditDateHeure :
      return (static_cast<NSEdit*>(_pNSCtrl))->
                                TempTransferer(&(_pTransfert->_iTmpActif),
                                           _pTransfert->getTmpTransfertMessage()) ;

    case isEditLexique :
      return (static_cast<NSEditLexique*>(_pNSCtrl))->
                                TempTransferer(&(_pTransfert->_iTmpActif),
                                           _pTransfert->getTmpTransfertMessage()) ;

    case isEditLexiqueDerive :
      return (static_cast<NSEditLexiqueDerive*>(_pNSCtrl))->
                                TempTransferer(&(_pTransfert->_iTmpActif),
                                           _pTransfert->getTmpTransfertMessage()) ;

    case isEditNoLexique :
      return (static_cast<NSEditNoLex*>(_pNSCtrl))->
                                TempTransferer(&(_pTransfert->_iTmpActif),
                                           _pTransfert->getTmpTransfertMessage()) ;
  }
	return 1 ;
}
//// Surcharge de l'opérateur d'affectation
//
NSControle&
NSControle::operator=(const NSControle& src)
{
	if (this == &src)
		return *this ;

	// Attention : l'objets pointé par pTransfert doit rester unique.
	//             Ne pas le dupliquer.
	_pTransfert  = src._pTransfert ;
	_pNSDialog   = src._pNSDialog ;
  _pMUEView    = src._pMUEView ;
	_pNSCtrl	   = src._pNSCtrl ;
	_sIdentite   = src._sIdentite ;
	_iType       = src._iType ;
  _bVisible    = src._bVisible ;
	_bGestionMultiple = src._bGestionMultiple ;

	if (src._pNSDlgFct)
		_pNSDlgFct = new NSDlgFonction(*(src._pNSDlgFct)) ;
	else
		_pNSDlgFct = (NSDlgFonction*) 0 ;

	_sFilling          = src._sFilling ;
	_sFillingFilter    = src._sFillingFilter ;
  _sFillingStarter   = src._sFillingStarter ;
	// _sFillingEndPeriod = src.sFillingEndPeriod ;
  _searchStruct      = src._searchStruct ;
	_sPathControl      = src._sPathControl ;

	return *this ;
}

//
// Surcharge de l'opérateur ==
//
int
NSControle::operator == (const NSControle& o)
{
	if	(_pTransfert == o._pTransfert)
		return 1 ;
	else
		return 0 ;
}

//
// Activer physiquement le controle
//
void
NSControle::SetFocus()
{
	if ((void*) NULL == _pNSCtrl)
  	return ;

  switch (_iType)
  {
  	case isCaseACocher :

    	(static_cast<NSCheckBox*>(_pNSCtrl))->ActiveToi = false ;
      break ;

    case isRadioBtn	 :

    	(static_cast<NSRadioButton*>(_pNSCtrl))->ActiveToi = false ;
      break ;

    case isBtn :

    	(static_cast<NSButton*>(_pNSCtrl))->ActiveToi = false ;
      break ;
  }
  (static_cast<TControl*>(_pNSCtrl))->SetFocus() ;
  activateParent() ;
}

BBItem*
NSControle::getInterfaceElementItem()
{
  if (_pNSDialog)
    return _pNSDialog->_pBBItem ;

  if (_pMUEView)
    return _pMUEView->getItem() ;

  return (BBItem*) 0 ;
}

TWindow*
NSControle::getInterfacePointer()
{
  if (_pNSDialog)
    return (TWindow*) _pNSDialog ;

  if (_pMUEView)
    return (TWindow*) _pMUEView ;

  return (TWindow*) 0 ;
}

NSContexte*
NSControle::getInterfaceContext()
{
  if (_pNSDialog)
    return _pNSDialog->pContexte ;

  if (_pMUEView)
    return _pMUEView->pContexte ;

  return (NSContexte*) 0 ;
}

NSControleVector*
NSControle::getInterfaceControls()
{
  if (_pNSDialog)
    return &(_pNSDialog->_aNSCtrls) ;

  if (_pMUEView)
    return _pMUEView->getControls() ;

  return (NSControleVector*) 0 ;
}

//
// Etablit un pont entre le versant Windows (NSDialog) et le
// versant BigBrother (BBItem et BBFilsItem) par l'intermédiaire des
// objets NSTransferInfo selon le schéma ci-dessous :
//
//    BBItem -> BBFilsItem -> NSTransferInfo <- NSControle <- NSDialog
//
//  Au lancement de la fonction, l'objet NSTransferInfo a déjà été créé
//	sous forme neutre par l'objet BBFilsItem correspondant.
//	L'objet NSControle s'y référence et lui donne une structure de
//	transfert appropriée.
//
int
NSControle::lierTransfert(BBItem* pBBItem)
{
	if ((BBItem*) NULL == pBBItem)
  	return 0 ;

  BBFilsItem* pBBFilsIt = (BBFilsItem*) 0 ;
	//
	// Un élément d'Identité vide n'est, par définition, pas liable
  // Un texte libre non plus ???
	//
	if ((string("") == _sIdentite) || (string("#") == _sIdentite))
  	return 0 ;
	//
	// Demande au BBItem qui a ouvert la fenêtre de localiser le BBFilsItem
	// qui correspond à ce groupe et ce code
	//
	pBBFilsIt = pBBItem->ChercheFils(_sIdentite) ;
	//
	// Si le BBFilsItem a été localisé, on traite son NSTransferInfo
	//
	if (pBBFilsIt)
	{
		_pTransfert = pBBFilsIt->getItemTransfertData() ;
		_pTransfert->referenceControle(this) ;
		if (pBBFilsIt->isActif())
			activeControle(BF_CHECKED) ;
	}
	else if (_pNSDialog)
	{
		string errmess = "ATTENTION !!! Controle orphelin (groupe " ;
		errmess += " - identité " ;
		errmess += _sIdentite ;
		errmess += ")" ;
		erreur(errmess.c_str(), standardError, 0) ;
		_pTransfert = (NSTransferInfo*) 0 ;
	}
	return 0 ;
}

void
NSControle::initFromArray(NSPatPathoArray* pPatPathoArray)
{
  if (((NSPatPathoArray*) NULL == pPatPathoArray) || pPatPathoArray->empty())
  	return ;

  PatPathoIter iter = pPatPathoArray->begin() ;

  int iLigne = (*iter)->getLigne() ;
  string sEtiquettePatpatho = (*iter)->getLexique() ;
  string sNoeud             = (*iter)->getNodeID() ;
  string sComplement        = (*iter)->getComplement() ;
  string sInteret           = (*iter)->getInteret() ;
  string sPluriel           = (*iter)->getPluriel() ;
  string sVisible           = (*iter)->getVisible() ;
  string sCertitude         = (*iter)->getCertitude() ;
  string sTexteLibre        = (*iter)->getTexteLibre() ;

  iter++ ;

  if ((pPatPathoArray->end() != iter) && ((*iter)->getLigne() == iLigne))
  {
  	sEtiquettePatpatho += string(1, cheminSeparationMARK) + (*iter)->getLexique() ;
    sNoeud             += string(1, cheminSeparationMARK) + (*iter)->getNodeID() ;
    sComplement = (*iter)->getComplement() ;
    sInteret    = (*iter)->getInteret() ;
    sPluriel    = (*iter)->getPluriel() ;
    sVisible    = (*iter)->getVisible() ;
    sCertitude  = (*iter)->getCertitude() ;
    sTexteLibre = (*iter)->getTexteLibre();
  }

  if (_pTransfert)
  {
    Message* pMessage = _pTransfert->getTransfertMessage() ;

    if (pMessage)
    {
      pMessage->SetNoeud(sNoeud) ;
      pMessage->SetLexique(sEtiquettePatpatho) ;
      pMessage->SetComplement(sComplement) ;
      pMessage->SetPluriel(sPluriel) ;
      pMessage->SetVisible(sVisible) ;
      pMessage->SetCertitude(sCertitude) ;
      pMessage->SetInteret(sInteret) ;
      pMessage->SetTexteLibre(sTexteLibre) ;
    }

    if (_pTransfert->getFilsItem())
      _pTransfert->getFilsItem()->Active() ; //rendre ce fils actif  }
  prepareControle() ;
}

//
// Initialise le contrôle en fonction de l'état du BBFilsItem qui lui correspond
//
void NSControle::prepareControle()
{
	if (_pTransfert)
	{
		if (1 == _pTransfert->getActif())
			activeControle(BF_CHECKED, _pTransfert->getTransfertMessage()) ;
		else
			activeControle(BF_UNCHECKED, _pTransfert->getTransfertMessage()) ;
	}
	if (_pNSDlgFct)
		_pNSDlgFct->execute(NSDLGFCT_REINIT) ;
}

void
NSControle::executeKillFocusBehaviour()
{
	if ((void*) NULL == _pNSCtrl)
  	return ;

	switch (_iType)
	{
/*
		case isCaseACocher :
            return (static_cast<NSCheckBox*>(_pNSCtrl))->
                                Transferer(direction,
                                           &(pTransfert->iActif),
                                           (pTransfert->pTransfertMessage ));

        case isRadioBtn	 :
            return (static_cast<NSRadioButton*>(_pNSCtrl))->
                                Transferer(direction,
                                           &(pTransfert->iActif),
                                           (pTransfert->pTransfertMessage ));

        case isTreeNode	 :
            return (static_cast<NSTreeNode*>(_pNSCtrl))->
                                Transferer(direction,
                                           &(pTransfert->iActif),
                                           (pTransfert->pTransfertMessage));

        case isEdit	 	       :
        case isEditDate      :
        case isEditDateHeure :
            return (static_cast<NSEdit*>(_pNSCtrl))->
                                Transferer(direction,
                                           &(pTransfert->iActif),
                                           (pTransfert->pTransfertMessage ));

        case isEditLexique :
            return (static_cast<NSEditLexique*>(_pNSCtrl))->
                                Transferer(direction,
                                           &(pTransfert->iActif),
                                           (pTransfert->pTransfertMessage ));

        case isEditLexiqueDerive :
            return (static_cast<NSEditLexiqueDerive*>(_pNSCtrl))->
                                Transferer(direction,
                                           &(pTransfert->iActif),
                                           (pTransfert->pTransfertMessage ));
*/

		case isEditNoLexique :
    	(static_cast<NSEditNoLex*>(_pNSCtrl))->executeKillFocusBehaviour() ;

/*
      case isComboClassif :
            return (static_cast<NSComboClassif*>(_pNSCtrl))->
                                Transferer(direction,
                                           &(pTransfert->iActif),
                                           (pTransfert->pTransfertMessage ));

      case isComboSemantique :
            return (static_cast<NSComboSemantique*>(_pNSCtrl))->
                                Transferer(direction,
                                           &(pTransfert->iActif),
                                           (pTransfert->pTransfertMessage ));
*/
	}
	return ;
}

void
NSControle::setFilling(string sFil)
{
  _searchStruct.reinit() ;

  _sFillingFilter  = string("") ;
  _sFillingStarter = string("") ;
  _sPathControl    = string("") ;

	if (string("") == sFil)
		return ;

  _searchStruct.init(sFil) ;

	size_t pos ;
  size_t equ ;
  string sField ;
  bool progress = true ;

  // valeurs par défaut
  _sFillingFilter = string("/0QUES/") ;
  // sFillingEndPeriod = "" ;
  _sPathControl   = string("") ;

  do
  {
  	pos = sFil.find("|") ;

    if (string::npos != pos)
    {
    	sField = string(sFil, 0, pos) ;
      sFil   = string(sFil, pos+1, strlen(sFil.c_str()) - pos - 1) ;
    }
    else
    {
    	sField = sFil ;
      progress = false ;
    }

    equ = sField.find("=") ;
    if (string::npos == equ)
    	_sFilling = sField ;
    else
    {
      string sValue = string("") ;
      if (equ < strlen(sField.c_str()) - 1)
        sValue = string(sField, equ + 1, strlen(sField.c_str()) - equ - 1) ;
      strip(sValue, stripBoth) ;

    	if      ('F' == sField[0])
      	_sFillingFilter = string("/") + sValue + string("/") ;
      else if ('R' == sField[0])
      	_sFillingStarter = sValue + string("/") ;
     //  else if ('D' == sField[0])
     // 	sFillingEndPeriod = sValue ;
      else if ('C' == sField[0])
      	_sPathControl = sValue ;
    }
  }
  while(progress) ;
}

bool
NSControle::initOnSetupWindow()
{
	if (strstr(_sFilling.c_str(), "S") != NULL)
  	return true ;
	return false ;
}

bool
NSControle::initOnFocus()
{
	if (strstr(_sFilling.c_str(), "F") != NULL)
  	return true ;
  return false ;
}

bool
NSControle::initOnTimer()
{
	if (strstr(_sFilling.c_str(), "T") != NULL)
  	return true ;
  return false ;
}

bool
NSControle::isToFilling(string sAction)
{
	if ((string("SetupWindow") == sAction) && (strstr(_sFilling.c_str(), "S") != NULL ))
  	return true ;
  if ((string("SetFocus") == sAction)&& (strstr(_sFilling.c_str(), "F") != NULL ))
  	return true ;
  if ((string("onTimer") == sAction)&& (strstr(_sFilling.c_str(), "T") != NULL ))
  	return true ;

  return false ;
}

string
NSControle::getPath()
{
  if (((NSTransferInfo*) NULL == _pTransfert) || (NULL == _pTransfert->getFilsItem()))
  	return string("") ;

  return _pTransfert->getFilsItem()->getLocalisation() ;
}

string
NSControle::cutPath(string *psPath, string sCutElement, bool bInclude)
{
  if (((string*) NULL == psPath) || (string("") == *psPath))
    return string("") ;

  string sElementPath = *psPath ;

  if (string("") != sCutElement)
  {
  	size_t pos = psPath->find(sCutElement) ;    if (NPOS != pos)    {      size_t iCutLen = strlen(sCutElement.c_str()) ;      if (false == bInclude)			  sElementPath = string(*psPath, pos+iCutLen, strlen(psPath->c_str())-pos-iCutLen) ;
      else
        sElementPath = string(*psPath, pos, strlen(psPath->c_str())-pos) ;
    }
  }

  return sElementPath ;
}

bool
NSControle::getPathForBlackboard(string* psPathForBB)
{
	if ((string*) NULL == psPathForBB)
  	return false ;

  *psPathForBB = getPath() ;

  // We apply the filter before the starter
  //
  if (string("") != _sFillingFilter)
    *psPathForBB = cutPath(psPathForBB, _sFillingFilter, false) ;
  if (string("") != _sFillingStarter)
    *psPathForBB = cutPath(psPathForBB, _sFillingStarter, true) ;

  if (string("") == *psPathForBB)
    return false ;

/*
	*psPathForBB = string("") ;

  if ((NULL == pTransfert) || (NULL == pTransfert->pBBFilsItem))
  	return false ;
  BBFilsItem* pFilsItem = pTransfert->pBBFilsItem ;

  *psPathForBB = pFilsItem->getLocalisation() ;

	// on récupère le chemin du BBItem pere du BBFilsItem associé
  // au NSControle via son pTransfert  if (pFilsItem->getItemFather())  	*psPathForBB = pFilsItem->getItemFather()->sLocalisation ;  // on rajoute l'étiquette du fils  string sEtiquette = pFilsItem->getItemLabel() ;  if (sEtiquette != "")  {  	string sEtiq ;    NSSuper* pSuper = pContexte->getSuperviseur() ;  	pSuper->getDico()->donneCodeSens(&sEtiquette, &sEtiq) ;    if (*psPathForBB != string(""))
  		*psPathForBB += string(1, cheminSeparationMARK) ;
    *psPathForBB += sEtiq ;
  }

  // on enleve le chemin jusqu'à "0QUES1"
  if (sFillingFilter != "")
  {
  	size_t pos = psPathForBB->find(sFillingFilter) ;    if (pos != NPOS)			*psPathForBB = string(*psPathForBB, pos+7, strlen(psPathForBB->c_str())-pos-7) ;
  }
*/

  *psPathForBB = getRegularPath(*psPathForBB, cheminSeparationMARK, intranodeSeparationMARK) ;

  return true ;
}

/*
bool
NSControle::getEndValidityDate(string* psValidityEndingDate)
{
	if (NULL == psValidityEndingDate)
  	return false ;

  *psValidityEndingDate = string("") ;

  if (string("") == sFillingEndPeriod)
  	return true ;

  NVLdVTemps tpsObj ;
  tpsObj.takeTime() ;

  string sAjoutAns  = string(sFillingEndPeriod, 0, 2) ;
  string sAjoutMois = string(sFillingEndPeriod, 2, 2) ;
  string sAjoutJour = string(sFillingEndPeriod, 4, 2) ;

  if (sAjoutAns != "00")
  	tpsObj.ajouteAns((-1) * atoi(sAjoutAns.c_str()), false) ;
  if (sAjoutMois != "00")
  	tpsObj.ajouteMois((-1) * atoi(sAjoutMois.c_str()), false) ;
  if (sAjoutJour != "00")
  	tpsObj.ajouteJours((-1) * atoi(sAjoutJour.c_str())) ;

  *psValidityEndingDate = tpsObj.donneDateHeure() ;

  return true ;
}
*/

bool
NSControle::isEmpty()
{
	if ((void*) NULL == _pNSCtrl)
  	return false ;

	string sLang = pContexte->getUserLanguage() ;

  if (isEditDate == _iType)
  {
		NSEditDate* pDate = static_cast<NSEditDate*> (_pNSCtrl) ;
    if ((NSEditDate*) NULL == pDate)
      return false ;

		string sContenu = pDate->GetText() ;

    if (string("") == sContenu)
      return true ;

    string sMessage ;
    donne_date_inverse(sContenu, sMessage, sLang) ;
    if ((string("") == sMessage) || (string("00000000") == sMessage))
    	return true ;

    return false ;
	}

  if (isEditDateHeure == _iType)
  {
		NSEditDateHeure* pDateHeure = static_cast<NSEditDateHeure*> (_pNSCtrl) ;
    if ((NSEditDateHeure*) NULL == pDateHeure)
      return false ;

		string sContenu = pDateHeure->GetText() ;

    string sMessage ;
    donne_date_inverse(sContenu, sMessage, sLang) ;
    if ((string("") == sMessage) || (string("00000000") == sMessage))
    	return true ;

    return false ;
	}

  switch (_iType)
  {
    case isEdit	:
    	return (0 == (static_cast<NSEdit*>(_pNSCtrl))->GetTextLen()) ;

    case isEditLexique :
    	return (0 == (static_cast<NSEditLexique*>(_pNSCtrl))->GetTextLen()) ;

    //case isEditLexiqueDerive :
    //	return ((static_cast<NSEditLexiqueDerive*>(_pNSCtrl))->GetTextLen() == 0) ;

    case isEditNoLexique :
    	return (0 == (static_cast<NSEditNoLex*>(_pNSCtrl))->GetTextLen()) ;

    case isCaseACocher :
      return (static_cast<NSCheckBox*>(_pNSCtrl))->isEmpty() ;

    case isRadioBtn	:
      return (static_cast<NSRadioButton*>(_pNSCtrl))->isEmpty() ;
	}

  return false ;
}

bool
NSControle::hasBeenValidated()
{
	if ((void*) NULL == _pNSCtrl)
  	return false ;

  switch (_iType)
  {
    case isEdit	:
    case isEditDate	:
    case isEditDateHeure	:
    	return (static_cast<NSEdit*>(_pNSCtrl))->hasBeenValidated() ;
	}

  return true ;
}

string
NSControle::getHelpText()
{
  // Replace all <br> with
  //
  size_t posit1 = _sHelpText.find("<br>") ;
  while (NPOS != posit1)
  {
    _sHelpText.replace(posit1, 4, NEWLINE);
    posit1 = _sHelpText.find("<br>", posit1 + 1) ;
  }

  return _sHelpText ;
}

//***************************************************************************
// Implémentation des méthodes NSDlgFonction
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur par défaut
//---------------------------------------------------------------------------
NSDlgFonction::NSDlgFonction(NSContexte* pCtx, NSSmallBrother* pBig, BBItem* pbBItem, string sNomFonct, NSControle* pCtrl, TModule* /* pResModule */)
              :NSRoot(pCtx), _pBigBoss(pBig)
{
	_pControle    = pCtrl ;
  _sNomFonction = sNomFonct ;
  _pBBItem      = pbBItem ;
  _pAdresseFct  = NULL ;

  string sDialogFile = _pBBItem->getItemDialogFile() ;
  string sFichMaj    = pseumaj(sDialogFile) ;

  if ((string("NSBB") != sFichMaj) && (string("NSMBB") != sFichMaj))
  {
  	pNSResModule = pBig->TrouverModule(sDialogFile) ;
    if (pNSResModule)
    	(FARPROC) _pAdresseFct = pNSResModule->GetProcAddress(MAKEINTRESOURCE(3)) ;
  }
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSDlgFonction::NSDlgFonction(const NSDlgFonction& src)
              :NSRoot(src.pContexte)
{
	_pBigBoss     = src._pBigBoss ;
	_pControle    = src._pControle ;
	_sNomFonction = src._sNomFonction ;
	_pAdresseFct  = src._pAdresseFct ;
  _pNSResModule = src._pNSResModule ;
  _pBBItem      = src._pBBItem ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSDlgFonction::~NSDlgFonction()
{
}

//---------------------------------------------------------------------------
//  Fonction :		int NSDlgFonction::::execute(int iParam)
//
//  Paramètres :	iParam -> moment dans la vie du Ctrl où est lancé la fct
//									 iParam est de la forme NSDLGFCT_XXXX
//
//	 Retour :      0 si l'exécution interdit la communication avec BigBrother
//						1 sinon
//---------------------------------------------------------------------------
int
NSDlgFonction::execute(SITUATION_TYPE iParam, HWND hWndFocus, uint key)
{
	int j = 0 ;
	if (_pAdresseFct)
	{
		j = (*_pAdresseFct)((NSDlgFonction far *) this,
    															(NSSmallBrother far*) _pBigBoss, iParam) ;
		return j ;
	}

  string sDialogFile = _pBBItem->getItemDialogFile() ;
  string sFichMaj    = pseumaj(sDialogFile) ;

	if ((string("") != sFichMaj) && (string("NSBB") != sFichMaj) && (string("NSMBB") != sFichMaj))
		return j ;

	string sCurrentFonction ;
  string sRmnFct = _sNomFonction ;

	size_t iPos = sRmnFct.find("/") ;
	if (string::npos == iPos)
	{
		sCurrentFonction = _sNomFonction ;
    sRmnFct = string("") ;
	}
	else
  {
		sCurrentFonction = string(sRmnFct, 0, iPos) ;
    sRmnFct = string(sRmnFct, iPos + 1, strlen(sRmnFct.c_str()) - iPos - 1) ;
	}

  bool bLetDefaultProcessing = true ;

	while (string("") != sCurrentFonction)
	{
		if (sCurrentFonction == "NOW")
		{
			switch (iParam)
			{
				case NSDLGFCT_CREATION :
					FixerDateDuJour(this) ;
					// return 0 ;
          bLetDefaultProcessing = false ;
          break ;
			}
		}
		else if (sCurrentFonction == "BEGINNOW")
		{
			switch (iParam)
			{
				case NSDLGFCT_CREATION :
					FixerDateDuJour(this) ;
					// return 0 ;
          bLetDefaultProcessing = false ;
          break ;

				case NSDLGFCT_POSTEXEC :					AdaptLastingTime(this) ;
					// return 1 ;
          break ;
			}
		}
		else if (sCurrentFonction == "ENDING")
		{
			switch (iParam)
			{
				case NSDLGFCT_POSTEXEC :
					AdaptLastingTime(this) ;
					// return 1 ;
          break ;
			}
		}
		else if (sCurrentFonction == "LASTING_TIME")
		{
			switch (iParam)
			{
				case NSDLGFCT_CREATION :
					FixLastingTime(this) ;
					// return 0 ;
          bLetDefaultProcessing = false ;
          break ;

				case NSDLGFCT_POSTEXEC :					AdaptEnding(this) ;
					// return 1 ;
          break ;
			}
		}
    else if (sCurrentFonction == "LASTING_TIME_FORCE")
		{
			switch (iParam)
			{
				case NSDLGFCT_CREATION :
					FixLastingTime(this) ;
					// return 0 ;
          bLetDefaultProcessing = false ;
          break ;

				case NSDLGFCT_POSTEXEC :					AdaptEnding(this, true) ;
					// return 1 ;
          break ;
			}
		}
		else if (sCurrentFonction == "NOENDING")
		{
			switch (iParam)
			{
				case NSDLGFCT_CREATION :
					FixNoEnding(this) ;
					// return 0 ;
          bLetDefaultProcessing = false ;
          break ;

				case NSDLGFCT_POSTEXEC :					CancelEnding(this) ;
					// return 1 ;
          break ;
			}
		}
		else if (sCurrentFonction == "NUMSYNCHRO")
		{
			switch (iParam)
			{
				case NSDLGFCT_POSTEXEC :
					SynchroNumValues(this) ;
					// return 1 ;
          break ;
			}
		}
    else if (sCurrentFonction == "SETDATEMAX")
		{
			switch (iParam)
			{
				case NSDLGFCT_POSTEXEC :
					RemplirDateMax(this) ;
					// return 1 ;
                break ;
			}
		}
		else if (sCurrentFonction == "CAPTION")
		{
			switch (iParam)
			{
				case NSDLGFCT_CREATION :
					FixCaption(this) ;
					// return 0 ;
          bLetDefaultProcessing = false ;
          break ;
			}
		}
		else if (sCurrentFonction == "CREEPREO")
		{
			switch (iParam)
			{
      	case NSDLGFCT_EXECUTE :
				case NSDLGFCT_POSTEXEC :
					CreatePreoccup(this) ;
          break ;
			}
		}
    else if (sCurrentFonction == "ADDALLPS")
		{
			switch (iParam)
			{
				case NSDLGFCT_CREATION :
					// EditAddress(this);
          initFromCurrentHealthProblems(this) ;
          bLetDefaultProcessing = false ;
					// return 0 ;
          break ;

				case NSDLGFCT_EXECUTE :
          // bLetDefaultProcessing = false ;
					// return 0;
          break ;
			}
		}
    else if (sCurrentFonction == "ADDALLLINES")
		{
			switch (iParam)
			{
				case NSDLGFCT_CREATION :
					// EditAddress(this);
          initFromCurrentConcerns(this) ;
          bLetDefaultProcessing = false ;
					// return 0 ;
          break ;

				case NSDLGFCT_EXECUTE :
          // bLetDefaultProcessing = false ;
					// return 0;
          break ;
			}
		}
    else if (sCurrentFonction == "ADDALLSOCIAL")
		{
			switch (iParam)
			{
				case NSDLGFCT_CREATION :
					// EditAddress(this);
          initFromCurrentSocialProblems(this) ;
          bLetDefaultProcessing = false ;
					// return 0 ;
          break ;

				case NSDLGFCT_EXECUTE :
          // bLetDefaultProcessing = false ;
					// return 0;
          break ;
			}
		}
    else if (sCurrentFonction == "ADDALLRISKS")
		{
			switch (iParam)
			{
				case NSDLGFCT_CREATION :
					// EditAddress(this);
          initFromCurrentRisks(this) ;
          bLetDefaultProcessing = false ;
					// return 0 ;
          break ;

				case NSDLGFCT_EXECUTE :
          // bLetDefaultProcessing = false ;
					// return 0;
          break ;
			}
		}
		else if (sCurrentFonction == "EDITADR")
		{
			switch (iParam)
			{
				case NSDLGFCT_CREATION :
					// EditAddress(this);
					ModifyAddress(this) ;
          bLetDefaultProcessing = false ;
					// return 0 ;
          break ;

				case NSDLGFCT_EXECUTE :
					ModifyAddress(this) ;
          bLetDefaultProcessing = false ;
					// return 0;
          break ;
			}
		}
		else if (sCurrentFonction == "EDITADR2")
		{
			switch (iParam)
			{
				case NSDLGFCT_EXECUTE :
					EditAddressButton(this);
					// return 0;
          bLetDefaultProcessing = false ;
          break ;
			}
		}
		else if (sCurrentFonction == "CHOIXADR")
		{
			switch (iParam)
			{
				case NSDLGFCT_EXECUTE :
					ChooseAddress(this);
					// return 0;
          bLetDefaultProcessing = false ;
          break ;
			}
		}
		else if (sCurrentFonction == "CHOIXADRPAT")
		{
			switch (iParam)
			{
				case NSDLGFCT_EXECUTE :
					ChoosePatAddress(this);
					// return 0;
          bLetDefaultProcessing = false ;
          break ;
			}
		}
		else if (sCurrentFonction == "CHOIXADRCOR")
		{
			switch (iParam)
			{
				case NSDLGFCT_EXECUTE :
					ChooseCorAddress(this);
					// return 0;
          bLetDefaultProcessing = false ;
          break ;
			}
		}
		else if (sCurrentFonction == "EDITCOR")
		{
			switch (iParam)
			{
				case NSDLGFCT_CREATION :
					ShowCorresp(this);
					// return 0;
          bLetDefaultProcessing = false ;
          break ;

				case NSDLGFCT_EXECUTE :
					EditCorresp(this);
					// return 0;
          bLetDefaultProcessing = false ;
          break ;
			}
		}
		else if (sCurrentFonction == "EDITREFCOR")
		{
			switch (iParam)
			{
				case NSDLGFCT_CREATION :
					ShowRefCorresp(this);
					// return 0;
          bLetDefaultProcessing = false ;
          break ;

				case NSDLGFCT_EXECUTE :
					EditRefCorresp(this);
					// return 0;
          bLetDefaultProcessing = false ;
          break ;
			}
		}
    else if (sCurrentFonction == "ALL_LOW_CAPS")
    {
      switch (iParam)
      {
        case NSDLGFCT_POSTEXEC :
          ChangeToLowCaps(this);
          bLetDefaultProcessing = false ;
          break ;
      }
    }
    else if (sCurrentFonction == "PIDSADR_SITE")
    {
			switch (iParam)
			{
				case NSDLGFCT_POSTEXEC :
					PidsAdrSite(this);
					//return 0;
          bLetDefaultProcessing = false ;
          break ;
			}
		}
    else if (sCurrentFonction == "PIDSADR_VOIE")
    {
			switch (iParam)
			{
				case NSDLGFCT_POSTEXEC :
					if (isFrereApres(hWndFocus))
						PidsAdrVoie(this) ;
					//return 0;
          bLetDefaultProcessing = false ;
					break ;
        }
    }
    else if (sCurrentFonction == "PIDSADR_ZIP")
    {
    	switch (iParam)
      {
      	case NSDLGFCT_POSTEXEC :
        	if (isFrereApres(hWndFocus))
          	PidsAdrZip(this) ;
              //return 0;
          bLetDefaultProcessing = false ;
          break ;
      }
    }
    else if (sCurrentFonction == "GET_IPP")
    {
    	switch (iParam)
      {
      	case NSDLGFCT_CREATION :
        	GetIPP(this) ;
          bLetDefaultProcessing = false ;
          break ;
      }
    }
    else if (sCurrentFonction == "GET_LOCALIPP")
    {
    	switch (iParam)
      {
      	case NSDLGFCT_CREATION :
        	GetLocalIPP(this, false) ;
          bLetDefaultProcessing = false ;
          break ;
      }
    }
    else if (sCurrentFonction == "GET_MANUAL_LOCALIPP")
    {
    	switch (iParam)
      {
      	case NSDLGFCT_CREATION :
        	GetLocalIPP(this, true) ;
          bLetDefaultProcessing = false ;
          break ;
      }
    }
    else if (sCurrentFonction == "ZIP_CITYNAME")
    {
    	switch (iParam)
      {
      	case NSDLGFCT_POSTEXEC :
        	if (isFrereApres(hWndFocus))
          	PidsZipFillCity(this) ;
              //return 0;
          bLetDefaultProcessing = false ;
          break ;
      }
    }
    else if (sCurrentFonction == "SURF_ONMAP")
    {
    	switch (iParam)
      {
      	case NSDLGFCT_POSTEXEC :
        	SurfOnTheMap(this) ;
          bLetDefaultProcessing = false ;
          break ;
      }
    }
    else if (sCurrentFonction == "QR_CODE")
    {
    	switch (iParam)
      {
      	case NSDLGFCT_CTRL_KEY :
          if (('Q' == key) || ('q') == key)
          {
        	  ShowQrCode(this) ;
            bLetDefaultProcessing = false ;
          }
          break ;
      }
    }
    else if (sCurrentFonction == "CALCIMC")
    {
      switch (iParam)
      {
        case NSDLGFCT_CREATION :
          CalcIMC(this);
          //return 0;
          bLetDefaultProcessing = false ;
          break ;
        case NSDLGFCT_POSTEXEC :
          CalcIMC(this);
          //return 0;
          bLetDefaultProcessing = false ;
          break ;
      }
    }
    else if (sCurrentFonction == "CALCIPS")
    {
      switch (iParam)
      {
        case NSDLGFCT_CREATION :
          CalcIPS(this) ;
          //return 0;
          bLetDefaultProcessing = false ;
          break ;
        case NSDLGFCT_POSTEXEC :
          CalcIPS(this) ;
          //return 0;
          bLetDefaultProcessing = false ;
          break ;
      }
    }
    else if (sCurrentFonction == "WEIGHTGAIN")
    {
      switch (iParam)
      {
        case NSDLGFCT_CREATION :
          CalcWeigthGain(this);
          //return 0;
          bLetDefaultProcessing = false ;
          break ;
        case NSDLGFCT_POSTEXEC :
          CalcWeigthGain(this);
          //return 0;
          bLetDefaultProcessing = false ;
          break ;
      }
    }
    else if (sCurrentFonction == "CALCSC")
    {
      switch (iParam)
      {
        case NSDLGFCT_CREATION :
          CalcSC(this);
          bLetDefaultProcessing = false ;
          break ;
        case NSDLGFCT_POSTEXEC :
          CalcSC(this);
          bLetDefaultProcessing = false ;
          break ;
      }
    }
    else if (sCurrentFonction == "CALCVCHAL")
    {
      switch (iParam)
      {
        case NSDLGFCT_CREATION :
          CalcVCHAL(this);
          bLetDefaultProcessing = false ;
          break ;
        case NSDLGFCT_POSTEXEC :
          CalcVCHAL(this);
          bLetDefaultProcessing = false ;
          break ;
      }
    }
    else if (sCurrentFonction == "CALCVDATA")
    {
      switch (iParam)
      {
        case NSDLGFCT_CREATION :
          CalcVDATA(this);
          bLetDefaultProcessing = false ;
          break ;
        case NSDLGFCT_POSTEXEC :
          CalcVDATA(this);
          bLetDefaultProcessing = false ;
          break ;
      }
    }
    else if (sCurrentFonction == "FRIEDGPL")
    {
      switch (iParam)
      {
        case NSDLGFCT_CREATION :
          CalcFriedwaldGpl(this);
          bLetDefaultProcessing = false ;
          break ;
        case NSDLGFCT_POSTEXEC :
          CalcFriedwaldGpl(this);
          bLetDefaultProcessing = false ;
          break ;
      }
    }
    else if (sCurrentFonction == "CALCVM8AL")
    {
      switch (iParam)
      {
        case NSDLGFCT_CREATION :
          CalcMicroAlb24(this);
          bLetDefaultProcessing = false ;
          break ;
        case NSDLGFCT_POSTEXEC :
          CalcMicroAlb24(this);
          bLetDefaultProcessing = false ;
          break ;
      }
    }
    else if (sCurrentFonction == "CALCVCSEL")
    {
      switch (iParam)
      {
        case NSDLGFCT_CREATION :
          CalcConsoSel(this);
          bLetDefaultProcessing = false ;
          break ;
        case NSDLGFCT_POSTEXEC :
          CalcConsoSel(this);
          bLetDefaultProcessing = false ;
          break ;
      }
    }
    else if (sCurrentFonction == "CALCCOCKROFTMGPL")
    {
      switch (iParam)
      {
        case NSDLGFCT_CREATION :
          CalcCockroftMGPL(this);
          bLetDefaultProcessing = false ;
          break ;
        case NSDLGFCT_POSTEXEC :
          CalcCockroftMGPL(this);
          bLetDefaultProcessing = false ;
          break ;
      }
    }
    else if (sCurrentFonction == "CALCCOCKROFTMMPL")
    {
      switch (iParam)
      {
        case NSDLGFCT_CREATION :
          CalcCockroftMMPL(this);
          bLetDefaultProcessing = false ;
          break ;
        case NSDLGFCT_POSTEXEC :
          CalcCockroftMMPL(this);
          bLetDefaultProcessing = false ;
          break ;
      }
    }
    else if (sCurrentFonction == "CALCMDRDMGPL")
    {
      switch (iParam)
      {
        case NSDLGFCT_CREATION :
          CalcMdrdMGPL(this);
          bLetDefaultProcessing = false ;
          break ;
        case NSDLGFCT_POSTEXEC :
          CalcMdrdMGPL(this);
          bLetDefaultProcessing = false ;
          break ;
      }
    }
    else if (sCurrentFonction == "CALCMDRDMMPL")
    {
      switch (iParam)
      {
        case NSDLGFCT_CREATION :
          CalcMdrdMMPL(this);
          bLetDefaultProcessing = false ;
          break ;
        case NSDLGFCT_POSTEXEC :
          CalcMdrdMMPL(this);
          bLetDefaultProcessing = false ;
          break ;
      }
    }
    else if (sCurrentFonction == "CALCBILILIBRE")
    {
      switch (iParam)
      {
        case NSDLGFCT_CREATION :
          CalcBilirubineLibre(this) ;
          bLetDefaultProcessing = false ;
          break ;
        case NSDLGFCT_POSTEXEC :
          CalcBilirubineLibre(this) ;
          bLetDefaultProcessing = false ;
          break ;
      }
    }
    else if (sCurrentFonction == "CLASSIFY")
    {
    	switch (iParam)
      {
      	case NSDLGFCT_POSTEXEC :
        	Classifying(this) ;
          bLetDefaultProcessing = false ;
          break ;
      }
    }
    else if (sCurrentFonction == "MATERIAL")
    {
    	switch (iParam)
      {
      	case NSDLGFCT_CREATION :
          InitTitleOfMaterial(this) ;
          bLetDefaultProcessing = false ;
          break ;
        case NSDLGFCT_EXECUTE :
					SelectMaterial(this) ;
          bLetDefaultProcessing = false ;
          break ;
      }
    }
    else if (sCurrentFonction == "PERSON")
    {
    	switch (iParam)
      {
      	case NSDLGFCT_CREATION :
          InitTitleOfPerson(this) ;
          bLetDefaultProcessing = false ;
          break ;
        case NSDLGFCT_EXECUTE :
					SelectPerson(this) ;
          bLetDefaultProcessing = false ;
          break ;
      }
    }
    else if (sCurrentFonction == "CROSSOVER")
    {
    	switch (iParam)
      {
      	case NSDLGFCT_CREATION :
          GetPrescriptionCrossover(this) ;
          bLetDefaultProcessing = false ;
          break ;
        case NSDLGFCT_POSTEXEC :
					GetPrescriptionCrossover(this) ;
          bLetDefaultProcessing = false ;
          break ;
      }
    }
    else if (sCurrentFonction == "SETCROSSOVER")
    {
    	switch (iParam)
      {
        case NSDLGFCT_POSTEXEC :
					SetPrescriptionCrossover(this) ;
          bLetDefaultProcessing = false ;
          break ;
      }
    }
    else if (sCurrentFonction == "SETDDR")
    {
    	switch (iParam)
      {
        case NSDLGFCT_POSTEXEC :
					SetGestationDDR(this) ;
          bLetDefaultProcessing = false ;
          break ;
      }
    }
    else if (sCurrentFonction == "SETGESTDDR")
    {
    	switch (iParam)
      {
        case NSDLGFCT_POSTEXEC :
					SetGestationDatesDDR(this) ;
          bLetDefaultProcessing = false ;
          break ;
      }
    }
    else if (sCurrentFonction == "SETGESTECHO")
    {
    	switch (iParam)
      {
        case NSDLGFCT_POSTEXEC :
					SetGestationDatesEcho(this) ;
          bLetDefaultProcessing = false ;
          break ;
      }
    }
    else if (sCurrentFonction == "SETGESTDATES")
    {
    	switch (iParam)
      {
        case NSDLGFCT_POSTEXEC :
					SetGestationDates(this) ;
          bLetDefaultProcessing = false ;
          break ;
      }
    }
    else if (sCurrentFonction == "SURG_HISTORY")
    {
    	switch (iParam)
      {
      	case NSDLGFCT_CREATION :
          CheckSurgicalHistory(this) ;
          bLetDefaultProcessing = false ;
          break ;
        case NSDLGFCT_POSTEXEC :
					RecordInSynthesis(this, string("ZSYNT/QANTP/QANTC")) ;
          bLetDefaultProcessing = false ;
          break ;
      }
    }

		//
		// Retourner 1 pour laisser le noyau effectuer le traitement par défaut
		//
		// return 1 ;

		if (sRmnFct != "")
		{
			iPos = sRmnFct.find("/") ;
			if (iPos == string::npos)
			{
				sCurrentFonction = sRmnFct ;
				sRmnFct = "" ;
			}
			else
			{
				sCurrentFonction = string(sRmnFct, 0, iPos) ;
				sRmnFct = string(sRmnFct, iPos + 1, strlen(sRmnFct.c_str()) - iPos - 1) ;
			}
		}
		else
			sCurrentFonction = "" ;
	}

	if (bLetDefaultProcessing)
		return 1 ;
	else
		return 0 ;
}

//---------------------------------------------------------------------------
//  Function:		NSDlgFonction& NSDlgFonction::operator = (NSDlgFonction src)
//
//  Description:	Surcharge de l'opérateur d'affectation
//---------------------------------------------------------------------------
NSDlgFonction&
NSDlgFonction::operator=(const NSDlgFonction& src)
{
  if (this == &src)
    return *this ;

	_pBigBoss     = src._pBigBoss ;
	_pControle    = src._pControle ;
	_sNomFonction = src._sNomFonction ;
	_pAdresseFct  = src._pAdresseFct ;
  _pBBItem      = src._pBBItem ;

  return *this ;
}

//---------------------------------------------------------------------------
//  Function:		int NSControle::operator == (const NSControle& o)
//
//  Description:	Surcharge de l'opérateur ==
//---------------------------------------------------------------------------
int
NSDlgFonction::operator==(const NSDlgFonction& x)
{
	if (_pControle == x._pControle)
		return 1 ;
	else
		return 0 ;
}

//--------------------------------------------------------------------
//rafraichir la boîte de dialogue contrôlée par pControle
//--------------------------------------------------------------------
void
NSDlgFonction::rafraichitControles()
{
  if ((NSControle*) NULL == _pControle)
    return ;

  if      (_pControle->getNSDialog())
    _pControle->getNSDialog()->rafraichitControles() ;
  else if (_pControle->getMUEView())
    _pControle->getMUEView()->rafraichitControles() ;
}

bool
NSDlgFonction::isFrere(HWND hWndGetFocus)
{
  bool bIsFrere = false ;

  if ((HWND) 0 != hWndGetFocus)
  {
    NSControleVector *pVectNSCtrl = _pControle->getInterfaceControls() ;
    if (pVectNSCtrl && (false == pVectNSCtrl->empty()))
    {
      for (iterNSControle i = pVectNSCtrl->begin() ; pVectNSCtrl->end() != i ; i++)
      {
        if ((*i)->getHWND() == hWndGetFocus)
        {
          bIsFrere = true ;
          break ;
        }
      }
    }
  }

  return bIsFrere ;
}

bool
NSDlgFonction::isFrereAvant(HWND hWndGetFocus)
{
  bool bIsFrereAvant = false ;

  if ((HWND) 0 != hWndGetFocus)
  {
    NSControleVector *pVectNSCtrl = _pControle->getInterfaceControls() ;
    if (pVectNSCtrl && (false == pVectNSCtrl->empty()))
    {
      for (iterNSControle i = pVectNSCtrl->begin(); i != pVectNSCtrl->end(); i++)
      {
        // Si on retrouve le controle en cours avant son frere
        // le bIsFrereAvant reste à false
        if (_pControle->getHWND() == (*i)->getHWND())
          break ;

        if ((*i)->getHWND() == hWndGetFocus)
        {
          bIsFrereAvant = true ;
          break ;
        }
      }
    }
  }

  return bIsFrereAvant ;
}

bool
NSDlgFonction::isFrereApres(HWND hWndGetFocus)
{
  bool bIsFrereApres = false ;
  bool bIsControle   = false ;

  if ((HWND) 0 != hWndGetFocus)
  {
    NSControleVector *pVectNSCtrl = _pControle->getInterfaceControls() ;
    if (pVectNSCtrl && (false == pVectNSCtrl->empty()))
    {
      for (iterNSControle i = pVectNSCtrl->begin(); i != pVectNSCtrl->end(); i++)
      {
        // Si on retrouve le controle en cours avant son frere
        if (_pControle->getHWND() == (*i)->getHWND())
        {
          bIsControle = true ;
          continue ;   // le controle en cours n'est pas supposé avoir le focus
        }

        if ((*i)->getHWND() == hWndGetFocus)
        {
          if (bIsControle)
            bIsFrereApres = true ;
          break ;
        }
      }
    }
  }

  return bIsFrereApres ;
}

bool
NSDlgFonction::containFonction(string sFctName)
{
  if ((string("") == _sNomFonction) || (string("") == sFctName))
    return false ;

  ClasseStringVector aVect ;
  DecomposeChaine(&_sNomFonction, &aVect, string("/")) ;

  if (aVect.empty())
    return false ;

  for (iterClassString it = aVect.begin() ; aVect.end() != it ; it++)
    if ((*it)->getItem() == sFctName)
      return true ;

  return false ;
}

