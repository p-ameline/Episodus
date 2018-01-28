
#include "nautilus\nssuper.h"
#include "nsoutil\nsoutil.rh"
#include "nsoutil\nsoutil.h"

#include "nsbb\nsutidlg.h"
#include "nsoutil\nsexport.h"
#include "nsoutil\nsInterfaces.h"

// -----------------------------------------------------------------
//
//  Méthodes de NsOutilDialog
//
// -----------------------------------------------------------------
// -----------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NsInterfacesDialog, NSUtilDialog)
	EV_COMMAND(IDOK, CmOk),
  EV_CHILD_NOTIFY_AND_CODE(IDC_TV, LBN_DBLCLK, CmInterfaceDblClk),
END_RESPONSE_TABLE;

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NsInterfacesDialog::NsInterfacesDialog(TWindow* pere, NSContexte* pCtx, string* psModuleName, map<string, string> *paInterfaces)
	                 :NSUtilDialog(pere, pCtx, "OUTILS_INTERFACES", pNSResModule)
{
  _paInterfaces = paInterfaces ;

  if (NULL == _paInterfaces)
  {
    _paInterfaces = new map<string, string> ;
    _bMustDeleteMap = true ;
  }
  else
    _bMustDeleteMap = false ;

	psSelectedModuleName = psModuleName ;
  if (NULL != psSelectedModuleName)
		*psSelectedModuleName = string("") ;

  pInterfacesBox = new OWL::TListBox(this, IDC_TV) ;
  initInterfacesList() ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NsInterfacesDialog::~NsInterfacesDialog()
{
	delete pInterfacesBox ;

  if (_bMustDeleteMap)
    delete _paInterfaces ;
}

//---------------------------------------------------------------------------
//  Fonction :   NsOutilDialog::SetupWindow()
//
//  Description: Initialise la boite de dialogue
//---------------------------------------------------------------------------
void
NsInterfacesDialog::SetupWindow()
{
	NSUtilDialog::SetupWindow() ;

  if (_paInterfaces->empty())
    return ;

  map<string, string>::iterator iter ;

  for (iter = _paInterfaces->begin() ; _paInterfaces->end() != iter ; iter++)
    pInterfacesBox->AddString(((*iter).first).c_str()) ;
}

void
NsInterfacesDialog::CmOk()
{
	char szSelectedString[1024] ;
	int iSelected = pInterfacesBox->GetSelString(szSelectedString, 1024) ;

  if (iSelected < 2)
  	return ;

	if (NULL != psSelectedModuleName)
		*psSelectedModuleName = (*_paInterfaces)[string(szSelectedString)] ;

	CloseWindow(IDOK) ;
}

void
NsInterfacesDialog::initInterfacesList()
{
  if (false == _paInterfaces->empty())
    return ;

  // Looking for nsmt_*.dll files in current directory
  char szMask[1024] ;  strcpy(szMask, "nsmi_*.dll") ;  WIN32_FIND_DATA FileData ;
  HANDLE hSearch = FindFirstFile(szMask, &FileData) ;
  if (hSearch == INVALID_HANDLE_VALUE)    return ;  // pSuperContext->GetMainWindow()->SetCursor(0, IDC_WAIT) ;

  void (FAR *pAdresseFct) (string far *, string far *) ;

  pContexte->GetMainWindow()->SetCursor(0, IDC_WAIT) ;

  bool bFinish = false ;
  while (!bFinish)
  {    DWORD dwAttr = FileData.dwFileAttributes;
    if (!(dwAttr & FILE_ATTRIBUTE_DIRECTORY))
    {      string sFileName = string(FileData.cFileName) ;      string sToolLabel  ;      string sToolDescription  ;try{      TModule* pDCModule = new TModule(sFileName.c_str(), TRUE) ;
    	if (!pDCModule)
    	{
      	string sErrorMsg = string("Impossible to open DLL ") +  sFileName ;
      	erreur(sErrorMsg.c_str(), standardError, -1, pContexte->GetMainWindow()->GetHandle()) ;
        pContexte->getSuperviseur()->afficheStatusMessage("") ;
    	}
     	else
      {
    		// Récupération du pointeur sur la fonction // Getting function's pointer
    		// (FARPROC) pAdresseFct = pDCModule->GetProcAddress(MAKEINTRESOURCE(1)) ;
        (FARPROC) pAdresseFct = pDCModule->GetProcAddress("@nsmtoolsInformation$qp60std@%basic_string$c19std@%char_traits$c%17std@%allocator$c%%t1") ;
    		if (NULL != pAdresseFct)
        {
    			((*pAdresseFct)((string far *) &sToolLabel, (string far *) &sToolDescription)) ;

          if (string("") != sToolLabel)
          	(*_paInterfaces)[sToolLabel] = sFileName ;
        }
        delete pDCModule ;
      }
}
catch (...)
{
  string sErrorText = pContexte->getSuperviseur()->getText("modulesManagement", "cannotLoadDll") ;
  sErrorText += string(" (") + sFileName + string(")") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1) ;
  erreur(sErrorText.c_str(), standardError, -1, pContexte->GetMainWindow()->GetHandle()) ;
}
    }

    pContexte->getSuperviseur()->getApplication()->PumpWaitingMessages() ;

    if (!FindNextFile(hSearch, &FileData))
    {      if (GetLastError() != ERROR_NO_MORE_FILES)      {        string sStatusMsg = pContexte->getSuperviseur()->getText("fileErrors", "cantGetNextFile") ;        erreur(sStatusMsg.c_str(), standardError, 0) ;        return ;      }      bFinish = true ;    }
  }
  pContexte->GetMainWindow()->SetCursor(0, IDC_ARROW) ;
}

void
NsInterfacesDialog::CmInterfaceDblClk(WPARAM /* Cmd */)
{
	CmOk() ;
}

// fin de nsInterfaces.cpp ............................
///////////////////////////////////////////////////////

