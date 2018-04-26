#include <owl\applicat.h>#include <owl\olemdifr.h>
#include <owl\dialog.h>
#include <owl\edit.h>
#include <owl\color.h>
#include <bde.hpp>
#include <stdio.h>
#include <string.h>

#include "nautilus\nssuper.h"
#include "partage\nsdivfct.h"
#include "nsoutil\nsPathorBdm.h"
#include "nsoutil\nsDicoBdm.h"

#include "nssavoir\nsPathor.rh"

//*******************************************************************
//						classe List   ListeBoxLexique
//*******************************************************************
DEFINE_RESPONSE_TABLE1(ListeBoxBdm, TListBox)
    EV_WM_KEYDOWN,
    EV_LBN_DBLCLK(IDC_PATHORLISTBOX, CmPathoDblClk),
END_RESPONSE_TABLE;

ListeBoxBdm::ListeBoxBdm(TWindow* parent, int ressource, ChoixBdmDialog* pChoixPatho, TModule* module)
				    :TListBox(parent,ressource, module)
{
  _pChoixBdmDialog = pChoixPatho ;
}

ListeBoxBdm::~ListeBoxBdm(){}

//--------------------------------------------------------------------------// touche interceptée par la boîte lexique
//--------------------------------------------------------------------------
void
ListeBoxBdm::EvKeyDown(uint key, uint repeatCount, uint flags)
{
  TWindow::EvKeyDown(key, repeatCount, flags) ;

  // Note : Dans le cas de NSUtilLexique, le Return est intercepté par EvKeyDown
  // Dans le cas d'un NSEditDico (consultation) le Return est intercepté par CmOk
  // NSUtilLexique n'utilise pas le même constructeur de NSEditDicoGlobal que NSEditDico
  if ((VK_INSERT == key) || (VK_RETURN == key))
  {
    _pChoixBdmDialog->_iElementChoisi = GetSelIndex() ; //indice de l'élément choisi
    if (_pChoixBdmDialog->_pEdit)
      _pChoixBdmDialog->_pEdit->ElementWasSelected() ;
    return ;
  }
}

voidListeBoxBdm::CmPathoDblClk()
{
  if (GetCount() <= 0)
    return ;

  _pChoixBdmDialog->_iElementChoisi = GetSelIndex() ; //indice de l'élément choisi

  if (_pChoixBdmDialog->_pEdit)
    _pChoixBdmDialog->_pEdit->ElementWasSelected() ;
}

//*******************************************************************
// -----------------------------------------------------------------
//
//  Méthodes de ChoixBdmDialog
//
// -----------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(ChoixBdmDialog, TDialog)
	EV_COMMAND(IDOK, CmOk),
	EV_COMMAND(IDCANCEL, CmCancel),
	EV_WM_KEYDOWN,
	EV_CHILD_NOTIFY_AND_CODE(IDC_PATHORLISTBOX, WM_KEYDOWN, CmSelectPatho),
	EV_CHILD_NOTIFY_AND_CODE(IDC_PATHORLISTBOX, LBN_DBLCLK, CmPathoDblClk),
	EV_WM_CLOSE,
	EV_WM_DESTROY,      // évenement reçu lorsqu'on fait Return
    //le lexique reçoit l'événement de se fermer, mais il se contente de
    // se cacher
END_RESPONSE_TABLE ;

ChoixBdmDialog::ChoixBdmDialog(TWindow* parent, NSBdmDriver* pDriver,                               NSEditBdm* pEdit,                               NSBdmDriver::BAMTABLETYPE iTableType,                               TModule* module)               :TDialog(parent, "BDM_DLG", module)
{
try
{
  _iTableType     = iTableType ;
	_bIsOpen        = false ;
	_bSetup         = false ;
	_pEdit          = pEdit ;
  _pDriver        = pDriver ;
	_pListeBox 	    = new ListeBoxBdm(this, IDC_PATHORLISTBOX, this, module) ;
	_iElementChoisi = -1 ;
	_sAmmorce       = string("") ;

	Create() ;
	ShowWindow(SW_HIDE) ;
}
catch (...)
{
	erreur("Exception (ChoixBdmDialog ctor)", standardError, 0) ;
	return ;
}
}

//---------------------------------------------------------------------------
//  ChoixBdmDialog::ChoixBdmDialog()
//  Destructor
//---------------------------------------------------------------------------
ChoixBdmDialog::~ChoixBdmDialog()
{
	delete _pListeBox ;
}

//---------------------------------------------------------------------------
//  Fonction : 	ChoixPathoDialog::SetupWindow()
//
//  Arguments :	Aucun
//
//  Description :	Initialise la boite de dialogue
//
//  Returns : 		Rien
//---------------------------------------------------------------------------
void
ChoixBdmDialog::SetupWindow()
{
	TDialog::SetupWindow() ;

	_bSetup = true ;

	SetupListe() ;
}

void
ChoixBdmDialog::setEdit(NSEditBdm* pE)
{
  _pEdit = pE ;

  if (_pEdit)
    _iTableType = _pEdit->getTableType() ;
  else
    _iTableType = NSBdmDriver::bamTableUndefined ;
}

//---------------------------------------------------------------------------
//					Recupération du code
//---------------------------------------------------------------------------
string
ChoixBdmDialog::GetCode()
{
	int index = _pListeBox->GetSelIndex() ;

	if ((index < 0) || (index >= (int) _aListeArray.size()))    return string("") ;  NSBdmEntry *pLPPt = _aListeArray[index] ;  if (pLPPt)    return pLPPt->getID() ;

  return string("") ;
}

//---------------------------------------------------------------------------
//  Description:  Fixe une nouvelle ammorce, met à jour la liste puis
//                (éventuellement) ouvre ou ferme la fenêtre.
//                Establish a new seed, update the list and (if needed) open
//                or close the window
//
//  Returns:     	Rien
//---------------------------------------------------------------------------
int
ChoixBdmDialog::DonneAmmorce(string* pAmmorce)
{
  if ((string*) NULL == pAmmorce)
    return 2 ;
  //
  // On met la liste à jour - uniquement si l'ammorce a changé
  // Updating the list - only if seed changed
  //
	if (*pAmmorce != _sAmmorce)
	{
		_sAmmorce = *pAmmorce ;
		SetupArray() ;
	}
  else
    return 0 ;

  //
  // Si la liste est vide, on ferme la fenêtre (si elle était ouverte !)
  // If the list is empty, we close the window (only if it was open !)
  //
  if (_aListeArray.empty() )
	{
    if (_bIsOpen)
            //cacherDialogue();
      return 2 ;
	}
  //
  // Si la liste n'est pas vide, on met à jour la fenêtre (ou on l'ouvre, ce
  // qui a pour effet de la mettre à jour)
  // ATTENTION : ne pas faire SetupListe avant que la boite de dialogue
  // 				n'ait fait son SetupWindow
  //
  // If the list is empty, we update the window (or open it, and it get
  // updated)
  // WARNING : don't call SetupListe before dialog box's SetupWindow has been
  //           called
  //
	else
	{
    if (false == _bIsOpen)
			//montrerDialogue();
      return 1 ;
    else
    {
      SetupListe() ;
      return 0 ;
    }
  }

  return 2 ;
}

#define MAX_NB_WORDS 50
//---------------------------------------------------------------------------
//  Description :	Initialise l'Array de NSListPathoInfo
//---------------------------------------------------------------------------
void
ChoixBdmDialog::SetupArray()
{
try
{
  //
  // Vidange de l'Array
  //
 	_aListeArray.vider() ;

	//
	// Remplissage de l'Array avec les éléments du lexique qui correspondent
  // à l'ammorce
	//
  if ((NSBdmDriver*) NULL == _pDriver)
    return ;

  // Make certain that the driver is not busy and that the seed has not evolved
  // while it was (it is useless to query a deprecated seed)
  //
  string sCurrentSeed = _sAmmorce ;
  while (_pDriver->isBusy())
    _pDriver->pContexte->getSuperviseur()->getApplication()->PumpWaitingMessages() ;

  if (_sAmmorce != sCurrentSeed)
    return ;

  // Asks the driver to return the information that fits the seed
  //
  _pDriver->getListForSeed(&_aListeArray, &_sAmmorce, _iTableType) ;
}
catch (...)
{
	erreur("Exception (ChoixPathoDialog::SetupArray)", standardError, 0) ;
}
}

//--------------------------------------------------------------------------// Touche interceptée par la boîte lexique
//--------------------------------------------------------------------------
void
ChoixBdmDialog::EvKeyDown(uint key, uint repeatCount, uint flags)
{
  TWindow::EvKeyDown(key, repeatCount, flags) ;
  if (VK_INSERT == key)
  {
    _iElementChoisi = _pListeBox->GetSelIndex() ; //indice de l'élément choisi
    if (_pEdit)
      _pEdit->ElementWasSelected() ;
  }
}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
void
ChoixBdmDialog::GetSelectedLabel(string* pChaineRecuperee)
{
  if ((string*) NULL == pChaineRecuperee)
    return ;

  *pChaineRecuperee = string("") ;

  if ((_iElementChoisi >= 0) && (_iElementChoisi < int(_aListeArray.size())))
    *pChaineRecuperee = (_aListeArray[_iElementChoisi])->getLabel() ;
}

//--------------------------------------------------------------------------// Sélectionner dans la liste le code lexique transmis en paramètre
//--------------------------------------------------------------------------
/*
void
ChoixBdmDialog::SelectById(string sID)
{
  int Element = 0 ;

  for (NSBdmEntryIter i = _aListeArray.begin() ; _aListeArray.end() != i ; i++)
  {
    if ((*i)->getLabel() == sID)
    {
      _pListeBox->SetSelIndex(Element) ;
      break ;
    }

    Element++ ;
  }
}
*/

//---------------------------------------------------------------------------//  Initialize the list
//---------------------------------------------------------------------------
void
ChoixBdmDialog::SetupListe()
{
  if ((false == _bIsOpen) || (false == _bSetup))
    return ;

  int listeCount = _pListeBox->GetCount() ;
  if (listeCount < 0)
  {
    erreur("Boite de liste défectueuse.", standardError, 0) ;
    return ;
  }
  if (listeCount > 0)
    _pListeBox->ClearList() ;

  if (_aListeArray.empty())
    return ;

  for (NSBdmEntryIter i = _aListeArray.begin() ; _aListeArray.end() != i ; i++)
  {
    string sLabel = (*i)->getLabel() ;
    _pListeBox->AddString(sLabel.c_str()) ;
  }
}

//---------------------------------------------------------------------------
// select the next element
//---------------------------------------------------------------------------
void
ChoixBdmDialog::NextElement()
{
  int nbElement = _pListeBox->GetCount() ;
  if (nbElement <= 0)
    return ;

  int Element = _pListeBox->GetSelIndex() ; //indice de l'élément choisi;
  Element++ ;
  if (Element < nbElement)
    _pListeBox->SetSelIndex(Element) ;
}

//---------------------------------------------------------------------------
// select the former element
//---------------------------------------------------------------------------
void
ChoixBdmDialog::FormerElement()
{
  int nbElement = _pListeBox->GetCount() ;
  if (nbElement <= 0)
    return ;

  int Element = _pListeBox->GetSelIndex() ; //indice de l'élément choisi;
  Element-- ;
  if (Element >= 0)
    _pListeBox->SetSelIndex(Element) ;
}

//---------------------------------------------------------------------------// inserer un élément du lexique suite à la touche Inserer
//---------------------------------------------------------------------------
void
ChoixBdmDialog::InsererElementLexique()
{
  if (false == hasSelectedElement())
    return ;

  if (_pEdit)
    _pEdit->ElementWasSelected() ;
}

//---------------------------------------------------------------------------//  Function :		ChoixPathoDialog::CmPathoDblClk(WPARAM Cmd)
//
//  Arguments :	Cmd : WPARAM retourné par Windows
//
//  Description :	Sélectionne un élément du lexique
//
//  Retour	: 		Rien
//---------------------------------------------------------------------------
void
ChoixBdmDialog::CmPathoDblClk(WPARAM /* Cmd */)
{
  if (false == hasSelectedElement())
    return ;

  if (_pEdit)
    _pEdit->ElementWasSelected() ;
}

bool
ChoixBdmDialog::hasSelectedElement()
{
  if (((ListeBoxBdm*) NULL == _pListeBox) || (_pListeBox->GetCount() <= 0))
    return false ;

  _iElementChoisi = _pListeBox->GetSelIndex() ; //indice de l'élément choisi

  return ((_iElementChoisi >= 0) && (_iElementChoisi < _pListeBox->GetCount())) ;
}

//---------------------------------------------------------------------------//  Function :		ChoixPathoDialog::CmSelectPatho(WPARAM Cmd)
//
//  Arguments :	Cmd : WPARAM retourné par Windows
//
//  Description :	Sélectionne un document de la chemise en cours
//
//  Retour	: 		Rien
//---------------------------------------------------------------------------
void
ChoixBdmDialog::CmSelectPatho(WPARAM Cmd)
{
  if (VK_INSERT == Cmd)
  {
    _iElementChoisi = _pListeBox->GetSelIndex() ; //indice de l'élément choisi
    if (_pEdit)
      _pEdit->ElementWasSelected() ;
  }
}

//---------------------------------------------------------------------------//  Shows the dialog
//---------------------------------------------------------------------------
void ChoixBdmDialog::showDialog()
{
  if (_bIsOpen)
    return ;

  char szThis[20] ;
  sprintf(szThis, "%p", HWindow) ;
  string sMsg = string("ChoixBdmDialog::showDialog for BdmDlg ") + string(szThis) ;
  _pDriver->pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;

  _bIsOpen = true ;
  SetupListe() ;
  ShowWindow(SW_SHOW) ;
}

//---------------------------------------------------------------------------
//  Hide the dialog
//---------------------------------------------------------------------------
void ChoixBdmDialog::hideDialog()
{
  if (false == _bIsOpen)
    return ;

  _bIsOpen  = false ;
  _sAmmorce = string("") ;
  _aListeArray.vider() ;

  int listeCount = _pListeBox->GetCount() ;  if (listeCount < 0)
    erreur("Boite de liste défectueuse.", standardError, 0) ;
  else if (listeCount > 0)
    _pListeBox->ClearList() ;

  ShowWindow(SW_HIDE) ;}

voidChoixBdmDialog::EvClose()
{
	hideDialog() ;
}

// Note : Lorsqu'on appuie sur Return lorsque la ListBox a le focus// le dialogue intercepte le Return et appelle CmOk
void
ChoixBdmDialog::CmOk()
{
  _iElementChoisi = _pListeBox->GetSelIndex() ; //indice de l'élément choisi
  if (_pEdit)
    _pEdit->ElementWasSelected() ;
}

voidChoixBdmDialog::CmCancel()
{
	hideDialog() ;
}

void
ChoixBdmDialog::EvDestroy()
{
	//cacherDialogue();
}

bool
ChoixBdmDialog::doesBelong(THandle hWndFocus)
{
  if ((hWndFocus == HWindow) || IsChild(hWndFocus))
    return true ;

  return false ;
}

