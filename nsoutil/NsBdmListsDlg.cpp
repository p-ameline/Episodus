#include <owl\applicat.h>#include <owl\olemdifr.h>
#include <owl\dialog.h>
#include <owl\edit.h>
#include <owl\color.h>
#include <bde.hpp>
#include <stdio.h>
#include <string.h>

#include "nssavoir\nsBdmDriver.h"
#include "nautilus\nssuper.h"
#include "partage\nsdivfct.h"
#include "nsoutil\nsoutil.h"
#include "nsoutil\nsBdmListsDlg.h"

#include "nsoutil\nsBdmDlg.rh"

//*******************************************************************
//						classe List   ListeBoxLexique
//*******************************************************************
DEFINE_RESPONSE_TABLE1(ListeBoxBdmList, TListBox)
END_RESPONSE_TABLE;

ListeBoxBdmList::ListeBoxBdmList(ChoixBdmListDialog* parent, int ressource)
				        :TListBox(parent, ressource)
{
  _pChoixBdmListDialog = parent ;
}

ListeBoxBdmList::~ListeBoxBdmList(){}

void
ListeBoxBdmList::SetupWindow()
{
  TListBox::SetupWindow() ;
}

// -----------------------------------------------------------------
//                          NSEditBdmList
// -----------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NSEditBdmList, OWL::TEdit)
END_RESPONSE_TABLE ;

NSEditBdmList::NSEditBdmList(ChoixBdmListDialog* parent, int resourceId,
                             uint textLimit)
              :OWL::TEdit(parent, resourceId, textLimit)
{
  _pBdmListDialog = parent ;
}

NSEditBdmList::~NSEditBdmList()
{
}

void
NSEditBdmList::SetupWindow()
{
  OWL::TEdit::SetupWindow() ;
}

//*******************************************************************
// -----------------------------------------------------------------
//
//  Méthodes de ChoixBdmDialog
//
// -----------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(ChoixBdmListDialog, TDialog)
	EV_COMMAND(IDOK, CmOk),
	EV_COMMAND(IDCANCEL, CmCancel),
  EV_COMMAND(IDC_LISTSRCH_SEARCH_BTN, CmSearch),
	EV_CHILD_NOTIFY_AND_CODE(IDC_LISTSRCH_LIST, LBN_DBLCLK, CmPathoDblClk),
END_RESPONSE_TABLE ;

ChoixBdmListDialog::ChoixBdmListDialog(TWindow* parent, NSBdmDriver* pDriver,                                       NSBdmDriver::BAMTABLETYPE iTableType,                                       NSBdmEntry* pSelectedElement,                                       string	sSeed)                   :TDialog(parent, "LISTS_SEARCH_DLG", pNSResModule)
{
try
{
  _pSelectedElement = pSelectedElement ;

  _iTableType    = iTableType ;
  _pDriver       = pDriver ;
  _sSeed         = sSeed ;

  _pEdit         = new NSEditBdmList(this, IDC_LISTSRCH_EDIT) ;
	_pListeBox 	   = new ListeBoxBdmList(this, IDC_LISTSRCH_LIST) ;
  _pSearchButton = new OWL::TButton(this, IDC_LISTSRCH_SEARCH_BTN) ;
}
catch (...)
{
	erreur("Exception (ChoixBdmListDialog ctor)", standardError, 0) ;
}
}

//---------------------------------------------------------------------------
//  Destructor
//---------------------------------------------------------------------------
ChoixBdmListDialog::~ChoixBdmListDialog()
{
	delete _pEdit ;
  delete _pListeBox ;
  delete _pSearchButton ;
}

//---------------------------------------------------------------------------
//  Description :	Initialise la boite de dialogue
//---------------------------------------------------------------------------
void
ChoixBdmListDialog::SetupWindow()
{
	TDialog::SetupWindow() ;

  _pEdit->SetText(_sSeed.c_str()) ;

  SetupArray() ;
}

//---------------------------------------------------------------------------
//  Description :	Initialise l'Array de NSListPathoInfo
//---------------------------------------------------------------------------
void
ChoixBdmListDialog::SetupArray()
{
try
{
  //
  // Vidange de l'Array
  //
 	_aListeArray.vider() ;

  // Get the entry
  //
  string sText = GetText() ;
  if (string("") == sText)
    return ;

	//
	// Remplissage de l'Array avec les éléments du lexique qui correspondent
  // à l'ammorce
	//
  if ((NSBdmDriver*) NULL == _pDriver)
    return ;

  // Make certain that the driver is not busy and that the seed has not evolved
  // while it was (it is useless to query a deprecated seed)
  //
  string sCurrentSeed = sText ;
  while (_pDriver->isBusy())
    _pDriver->pContexte->getSuperviseur()->getApplication()->PumpWaitingMessages() ;

  if (GetText() != sCurrentSeed)
    return ;

  // Asks the driver to return the information that fits the seed
  //
  _pDriver->getListForSeed(&_aListeArray, &sCurrentSeed, _iTableType) ;

  // Sort the array on codes
  //
  if (false == _aListeArray.empty())
    sort(_aListeArray.begin(), _aListeArray.end(), BdmEntrySortByIdInf) ;

  // Fill the list
  //
  FillList() ;
}
catch (...)
{
	erreur("Exception (ChoixBdmListDialog::SetupArray)", standardError, 0) ;
}
}

//---------------------------------------------------------------------------//  Initialize the list
//---------------------------------------------------------------------------
void
ChoixBdmListDialog::FillList()
{
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
    string sLabel = (*i)->getCode() + string(" - ") + (*i)->getLabel() ;
    _pListeBox->AddString(sLabel.c_str()) ;
  }

  _pListeBox->Invalidate() ;
}

//---------------------------------------------------------------------------//  Arguments :	Cmd : WPARAM retourné par Windows
//  Description :	Sélectionne un élément de la liste
//---------------------------------------------------------------------------
void
ChoixBdmListDialog::CmPathoDblClk(WPARAM /* Cmd */)
{
  NSBdmEntry* pSelected = getSelectedElement() ;
  if ((NSBdmEntry*) NULL == pSelected)
    return ;

  CmOk() ;
}

NSBdmEntry*
ChoixBdmListDialog::getSelectedElement()
{
  if (((ListeBoxBdmList*) NULL == _pListeBox) || (_pListeBox->GetCount() <= 0))
    return (NSBdmEntry*) 0 ;

  if (_aListeArray.empty())
    return (NSBdmEntry*) 0 ;

  int iElementChoisi = _pListeBox->GetSelIndex() ; //indice de l'élément choisi

  if ((iElementChoisi >= 0) && (iElementChoisi < (int) _aListeArray.size()))
    return _aListeArray[iElementChoisi] ;

  return (NSBdmEntry*) 0 ;
}

void
ChoixBdmListDialog::CmSearch()
{
  SetupArray() ;
}

string
ChoixBdmListDialog::GetText()
{
  size_t iBuffLen = _pEdit->GetTextLen() ;
	char far* str = new char[iBuffLen + 1] ;

	_pEdit->GetText(str, iBuffLen + 1) ;
	string sContent = string(str) ;
	delete[] str ;

  return sContent ;
}

// Note : Lorsqu'on appuie sur Return lorsque la ListBox a le focus// le dialogue intercepte le Return et appelle CmOk
void
ChoixBdmListDialog::CmOk()
{
  NSBdmEntry* pSelected = getSelectedElement() ;
  if (pSelected)
    *_pSelectedElement = *pSelected ;

  TDialog::CmOk() ;
}

voidChoixBdmListDialog::CmCancel()
{
  TDialog::CmCancel() ;
}

