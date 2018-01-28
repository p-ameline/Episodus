#include <owl\olemdifr.h>
#include <owl\applicat.h>
#include <owl\dialog.h>
#include <owl\edit.h>
#include <owl\listwind.h>
#include <owl\gauge.h>
#include <bde.hpp>
#include <stdio.h>
#include <string.h>

#include "partage\nsdivfct.h"
#include "nautilus\nssuper.h"
#include "nsoutil\nsoutil.rh"
#include "nsoutil\nsoutil.h"
#include "nsoutil\nsoutdlg.h"
//#include "nsoutil\nssmedit.h"
#include "nsoutil\v2_3dlg.h"
#include "nsoutil\v2_3pat.h"
#include "nsoutil\importpat.h"
#include "nsoutil\nssp.h"
#include "nsoutil\nsvpatbd.h"
#include "nsoutil\nsfilgui.h"
#include "nsoutil\nsrepair.h"
#include "nautilus\nscqdoc.h"

#include "nscompta\nsfse16.h"
#include "nsbb\nsednum.h"
#include "nsbb\nspatbd.h"
#include "nsbb\nspatpat.h"
#include "nsbb\nsmanager.h"
#include "nsbb\nstrewi.h"
#include "nsbb\nscomboutil.h"

#include "nsdn\nsintrad.h"

#include "nautilus\nsepicap.h"

bool
templateSortByLibelleInf(NSTemplateInfo *pTpl1, NSTemplateInfo *pTpl2)
{
	return (pTpl1->getLibelle() < pTpl2->getLibelle()) ;
}

bool
templateSortByLibelleSup(NSTemplateInfo *pTpl1, NSTemplateInfo *pTpl2)
{
	return (pTpl1->getLibelle() > pTpl2->getLibelle()) ;
}

bool
templateSortByCodeInf(NSTemplateInfo *pTpl1, NSTemplateInfo *pTpl2)
{
	return (pTpl1->getCode() < pTpl2->getCode()) ;
}

bool
templateSortByCodeSup(NSTemplateInfo *pTpl1, NSTemplateInfo *pTpl2)
{
	return (pTpl1->getCode() > pTpl2->getCode()) ;
}

bool
templateSortByTypeInf(NSTemplateInfo *pTpl1, NSTemplateInfo *pTpl2)
{
	return (pTpl1->getType() < pTpl2->getType()) ;
}

bool
templateSortByTypeSup(NSTemplateInfo *pTpl1, NSTemplateInfo *pTpl2)
{
	return (pTpl1->getType() > pTpl2->getType()) ;
}

bool
templateSortByUserInf(NSTemplateInfo *pTpl1, NSTemplateInfo *pTpl2)
{
	return (pTpl1->getUser() < pTpl2->getUser()) ;
}

bool
templateSortByUserSup(NSTemplateInfo *pTpl1, NSTemplateInfo *pTpl2)
{
	return (pTpl1->getUser() > pTpl2->getUser()) ;
}

bool
templateSortByRootInf(NSTemplateInfo *pTpl1, NSTemplateInfo *pTpl2)
{
	return (pTpl1->getRoot() < pTpl2->getRoot()) ;
}

bool
templateSortByRootSup(NSTemplateInfo *pTpl1, NSTemplateInfo *pTpl2)
{
	return (pTpl1->getRoot() > pTpl2->getRoot()) ;
}

bool
templateSortByCompoInf(NSTemplateInfo *pTpl1, NSTemplateInfo *pTpl2)
{
	return (pTpl1->getCompo() < pTpl2->getCompo()) ;
}

bool
templateSortByCompoSup(NSTemplateInfo *pTpl1, NSTemplateInfo *pTpl2)
{
	return (pTpl1->getCompo() > pTpl2->getCompo()) ;
}

bool
templateSortByDefaultInf(NSTemplateInfo *pTpl1, NSTemplateInfo *pTpl2)
{
	return (pTpl1->getDefaut() < pTpl2->getDefaut()) ;
}

bool
templateSortByDefaultSup(NSTemplateInfo *pTpl1, NSTemplateInfo *pTpl2)
{
	return (pTpl1->getDefaut() > pTpl2->getDefaut()) ;
}

bool
templateSortByHeaderInf(NSTemplateInfo *pTpl1, NSTemplateInfo *pTpl2)
{
	return (pTpl1->getEnTete() < pTpl2->getEnTete()) ;
}

bool
templateSortByHeaderSup(NSTemplateInfo *pTpl1, NSTemplateInfo *pTpl2)
{
	return (pTpl1->getEnTete() > pTpl2->getEnTete()) ;
}

bool
templateSortByFileInf(NSTemplateInfo *pTpl1, NSTemplateInfo *pTpl2)
{
	return (pTpl1->getFichier() < pTpl2->getFichier()) ;
}

bool
templateSortByFileSup(NSTemplateInfo *pTpl1, NSTemplateInfo *pTpl2)
{
	return (pTpl1->getFichier() > pTpl2->getFichier()) ;
}

// -----------------------------------------------------------------
//
//  Méthodes de NsOutilDialog
//
// -----------------------------------------------------------------
// -----------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NsOutilDialog, TDialog)
	EV_COMMAND(IDOK, CmOk),
  EV_CHILD_NOTIFY_AND_CODE(IDC_TV, LBN_DBLCLK, CmFunctionDblClk),
END_RESPONSE_TABLE;

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NsOutilDialog::NsOutilDialog(TWindow* pere, NSContexte* pCtx, string* psModuleName)
	            :NSUtilDialog(pere, pCtx, "OUTILS_NTIERS", pNSResModule)
{
	psSelectedModuleName = psModuleName ;
  if (NULL != psSelectedModuleName)
		*psSelectedModuleName = string("") ;

  pFunctionsBox    = new OWL::TListBox(this, IDC_TV) ;
  initFunctionsList() ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NsOutilDialog::~NsOutilDialog()
{
	delete pFunctionsBox ;
}

//---------------------------------------------------------------------------
//  Fonction :   NsOutilDialog::SetupWindow()
//
//  Description: Initialise la boite de dialogue
//---------------------------------------------------------------------------
void
NsOutilDialog::SetupWindow()
{
	NSUtilDialog::SetupWindow() ;

  if (aFunctions.empty())
    return ;

  map<string, string>::iterator iter ;

  for (iter = aFunctions.begin() ; iter != aFunctions.end() ; iter++)
    pFunctionsBox->AddString(((*iter).first).c_str());
}

void
NsOutilDialog::CmOk()
{
	char szSelectedString[1024] ;
	int iSelected = pFunctionsBox->GetSelString(szSelectedString, 1024) ;

  if (iSelected < 2)
  	return ;

	if (NULL != psSelectedModuleName)
		*psSelectedModuleName = aFunctions[string(szSelectedString)] ;

	CloseWindow(IDOK) ;
}

void
NsOutilDialog::initFunctionsList()
{
  // Looking for nsmt_*.dll files in current directory
  char szMask[1024] ;  strcpy(szMask, "nsmt_*.dll") ;  WIN32_FIND_DATA FileData ;
  HANDLE hSearch = FindFirstFile(szMask, &FileData) ;
  if (hSearch == INVALID_HANDLE_VALUE)    return ;  // pSuperContext->GetMainWindow()->SetCursor(0, IDC_WAIT) ;

  void (FAR *pAdresseFct) (string far *, string far *) ;

  pContexte->GetMainWindow()->SetCursor(0, IDC_WAIT) ;

  bool bFinish = false ;
  while (!bFinish)
  {    DWORD dwAttr = FileData.dwFileAttributes;
    if (!(dwAttr & FILE_ATTRIBUTE_DIRECTORY))
    {      string sFileName = string(FileData.cFileName) ;      string sToolLabel  ;      string sToolDescription  ;try{      TModule* pDCModule = new TModule(sFileName.c_str(), TRUE) ;
    	if (NULL == pDCModule)
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
    		if (pAdresseFct != NULL)
        {
    			((*pAdresseFct)((string far *) &sToolLabel, (string far *) &sToolDescription)) ;

          if (sToolLabel != string(""))
          	aFunctions [sToolLabel] = sFileName ;
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
NsOutilDialog::CmFunctionDblClk(WPARAM /* Cmd */)
{
	CmOk() ;

/*
	char szSelectedString[1024] ;
	int iSelected = pFunctionsBox->GetSelString(szSelectedString, 1024) ;

  if (iSelected < 2)
  	return ;

	string sModuleName = aFunctions[string(szSelectedString)] ;

  if (sModuleName == string(""))
  	return ;

  TModule* pDCModule = new TModule(sModuleName.c_str(), TRUE) ;

  if (!pDCModule)
  {
  	string sErrorMsg = string("Impossible to open DLL ") +  sModuleName ;
    erreur(sErrorMsg.c_str(), standardError, -1, pContexte->GetMainWindow()->GetHandle()) ;
    pContexte->getSuperviseur()->afficheStatusMessage("") ;
    return ;
  }

  void (FAR *pAdresseFct) (TWindow far *, NSContexte far *) ;

  // Récupération du pointeur sur la fonction // Getting function's pointer
  // (FARPROC) pAdresseFct = pDCModule->GetProcAddress(MAKEINTRESOURCE(2)) ;
  (FARPROC) pAdresseFct = pDCModule->GetProcAddress("@nsmToolsAction$qp11OWL@TWindowp10NSContexte") ;
  if (pAdresseFct == NULL)
  {
  	delete pDCModule ;
    return ;
  }

  ((*pAdresseFct)((TWindow far *) this, (NSContexte far *) pContexte)) ;

  delete pDCModule ;
*/
}

//***************************************************************************
//
//  		  Méthodes de NSVarTag//
//***************************************************************************
NSVarTag::NSVarTag(string sName, string sValue)
{
	name  = sName ;
	value = sValue ;
}

NSVarTag::~NSVarTag()
{
}

NSVarTag::NSVarTag(const NSVarTag& rv)
{
	name  = rv.name ;
	value = rv.value ;
}

NSVarTag&
NSVarTag::operator=(const NSVarTag& src)
{
	if (this == &src)
		return *this ;

	name  = src.name;
	value = src.value;

	return *this ;
}

// Classe NSVarTagArray
////////////////////////////////////////////////////

//---------------------------------------------------------------------------
//  Constructeur copie//---------------------------------------------------------------------------
NSVarTagArray::NSVarTagArray(const NSVarTagArray& rv) : NSVarTagVector(){	if (false == rv.empty())		for (NSVarTagConstIter i = rv.begin() ; rv.end() != i ; i++)    	push_back(new NSVarTag(**i)) ;}
NSVarTagArray&
NSVarTagArray::operator=(const NSVarTagArray& src)
{	if (this == &src)		return *this ;  vider() ;	if (false == src.empty())		for (NSVarTagConstIter i = src.begin() ; src.end() != i ; i++)    	push_back(new NSVarTag(**i)) ;	return *this ;}

//---------------------------------------------------------------------------//  Destructeur//---------------------------------------------------------------------------
voidNSVarTagArray::vider(){  if (false == empty())	  for (NSVarTagIter i = begin() ; end() != i ; )    {      delete *i ;      erase(i) ;    }}
NSVarTagArray::~NSVarTagArray(){	vider() ;}

//***************************************************************************
//
//  		  Méthodes de NSEnTeteEditWindow//
//***************************************************************************
DEFINE_RESPONSE_TABLE1(NSEnTeteEditWindow, TListWindow)   EV_WM_LBUTTONDBLCLK,
END_RESPONSE_TABLE;

// constructeur
NSEnTeteEditWindow::NSEnTeteEditWindow(NSEnTeteEditDialog* pere, int resId) : TListWindow(pere, resId)
{
	pDlg = pere ;
  Attr.Style |= LVS_REPORT | LVS_SHOWSELALWAYS ;
  Attr.ExStyle |= WS_EX_NOPARENTNOTIFY ;
}

//---------------------------------------------------------------------------//  Function: NSEnTeteEditWindow::EvLButtonDblClk(uint modKeys, TPoint& point)
//
//  Arguments:	  les modKeys et le point double-clické
//
//  Description: Fonction de réponse au double-click
//
//  Returns:     Rien
//---------------------------------------------------------------------------

voidNSEnTeteEditWindow::EvLButtonDblClk(uint /* modKeys */, NS_CLASSLIB::TPoint& point)
{
	TLwHitTestInfo info(point) ;

	HitTest(info) ;
	if (info.GetFlags() & LVHT_ONITEM)  	pDlg->CmEditVarTag() ;
}

// fonction SetupWindow
/////////////////////////////////////////////////////
void
NSEnTeteEditWindow::SetupWindow()
{
	ListView_SetExtendedListViewStyle(this->HWindow, LVS_EX_FULLROWSELECT) ;
	TListWindow::SetupWindow() ;
}

//---------------------------------------------------------------------------//  Function: NSEnTeteEditWindow::IndexItemSelect()
//
//  Arguments:	  Aucun
//
//  Description: Retourne l'index du premier item sélectionné
//
//  Returns:     index si item est sélectionné, -1 sinon
//---------------------------------------------------------------------------

intNSEnTeteEditWindow::IndexItemSelect()
{
    int count = GetItemCount();
    int index = -1;

    for (int i = 0; i < count; i++)   	    if (GetItemState(i, LVIS_SELECTED))
        {
      	    index = i;
            break;
        }

    return index;}

//***************************************************************************
//
//  Méthodes de NSEnTeteEditDialog
//
//***************************************************************************

DEFINE_RESPONSE_TABLE1(NSEnTeteEditDialog, NSUtilDialog)
    EV_LV_DISPINFO_NOTIFY(IDC_ENTETE_LW, LVN_GETDISPINFO, DispInfoListeVar),
    EV_COMMAND(IDOK, CmOk),
    EV_COMMAND(IDCANCEL, CmCancel),
    EV_COMMAND(IDHELP, CmHelp),
END_RESPONSE_TABLE;

NSEnTeteEditDialog::NSEnTeteEditDialog(TWindow* pere, NSContexte* pCtx, string filename)
                :NSUtilDialog(pere, pCtx, "IDD_ENTETE_EDIT", pNSResModule)
{
    pVarArray = new NSVarTagArray;
    nbVar = 0;
    sFileName = filename;
    pListeVar = new NSEnTeteEditWindow(this, IDC_ENTETE_LW);

    bParseOK = ParseModele();

    sHindex = "" ;
    sHcorps = "Edition_d_en_tete.html" ;
}

NSEnTeteEditDialog::~NSEnTeteEditDialog()
{
    delete pListeVar;
    delete pVarArray;
}

void
NSEnTeteEditDialog::SetupWindow()
{
    NSUtilDialog::SetupWindow();
    InitListeVar();
    AfficheListeVar();

    pContexte->setAideIndex(sHindex);
    pContexte->setAideCorps(sHcorps);
}

bool
NSEnTeteEditDialog::ParseModele()
{
    ifstream html_file;
	string  tmp_html_string;
    string  commentaire, name, tag, value;
    size_t  pos1, pos2;
    size_t  deb_tag, fin_tag, deb_val, fin_val;

	html_file.open(sFileName.c_str());

    if (!html_file)
    {
        string sErreur = string("Impossible d'ouvrir le fichier html (") +
                         sFileName + string(")");
        erreur(sErreur.c_str(), standardError, 0);
        return false;
    }

	while (getline (html_file, tmp_html_string))
		html_string += tmp_html_string + '\n';
	html_file.close ();

    pos1 = html_string.find("<!--");

    while (pos1 != NPOS)
    {
        pos2 = html_string.find("-->", pos1 + 4);
        if (pos2 == NPOS)
        {
            erreur("Commentaire non fermé dans le modèle d'en-tete.", standardError, 0);
            return false;
        }

        commentaire = string(html_string, pos1 + 4, pos2 - pos1 - 4);
        deb_tag = commentaire.find("$");
        if (deb_tag != NPOS)
        {
            fin_tag = commentaire.find("$", deb_tag + 1);
            if (fin_tag == NPOS)
            {
                erreur("Tag non fermé dans le modèle d'en-tete.", standardError, 0);
                return false;
            }

            name = string(commentaire, deb_tag + 1, fin_tag - deb_tag - 1);

            // recherche du commentaire suivant, contenant la fin du tag name
            // (on récupère au passage la valeur de la variable)
            pos1 = html_string.find("<!--", pos2 + 3);
            if (pos1 == NPOS)
            {
                erreur("Variable non refermée dans le modèle d'en-tete.", standardError, 0);
                return false;
            }

            deb_val = pos2 + 3;
            fin_val = pos1;

            value = string(html_string, deb_val, fin_val - deb_val);
            tag = string("$/") + name + string("$");

            pos2 = html_string.find("-->", pos1 + 4);
            if (pos2 == NPOS)
            {
                erreur("Commentaire non fermé dans le modèle d'en-tete.", standardError, 0);
                return false;
            }

            commentaire = string(html_string, pos1 + 4, pos2 - pos1 - 4);
            if (commentaire.find(tag) == NPOS)
            {
                erreur("Tag de fin de variable incorrect dans le modèle d'en-tete.", standardError, 0);
                return false;
            }

            // Ici la variable est correcte
            pVarArray->push_back(new NSVarTag(name, texteCourant(value)));
            nbVar++;
        }

        pos1 = html_string.find("<!--", pos2 + 3);
    }

    return true;
}

void
NSEnTeteEditDialog::InitListeVar()
{
    TListWindColumn colName("Nom de la variable", 150, TListWindColumn::Left, 0);
  	pListeVar->InsertColumn(0, colName);

    TListWindColumn colValue("Valeur dans l'en-tete", 350, TListWindColumn::Left, 1);  	pListeVar->InsertColumn(1, colValue);
}

void
NSEnTeteEditDialog::AfficheListeVar()
{
    pListeVar->DeleteAllItems();

	for (int i = nbVar - 1; i >= 0; i--)    {
        TListWindItem Item((((*pVarArray)[i])->name).c_str(), 0);
        pListeVar->InsertItem(Item);
    }
}

void
NSEnTeteEditDialog::DispInfoListeVar(TLwDispInfoNotify& dispInfo)
{
    const int 	    BufLen = 255;
    static char     buffer[BufLen];
    TListWindItem&  dispInfoItem = *(TListWindItem*)&dispInfo.item;
    int 			index;

    index = dispInfoItem.GetIndex();
    // Affiche les informations en fonction de la colonne    switch (dispInfoItem.GetSubItem())
    {
   	    case 1: // groupe
         strcpy(buffer, (((*pVarArray)[index])->value).c_str());
         dispInfoItem.SetText(buffer);
         break;
    }
}

void
NSEnTeteEditDialog::CmEditVarTag()
{
    int     varChoisie = pListeVar->IndexItemSelect();
    size_t  pos, deb_val, fin_val;

    if (varChoisie == -1)
    {
        erreur("Vous devez sélectionner une variable à éditer.", standardError, 0);
        return;
    }

    NSVarTagEditDialog* pDlg = new NSVarTagEditDialog(this, pContexte, (*pVarArray)[varChoisie]);
    int idRet = pDlg->Execute();

    if (idRet == IDOK)
    {
        string name = pDlg->pVarTag->name;
        string value = pDlg->pVarTag->value;
        string tagname = string("$") + name + string("$");

        pos = html_string.find(tagname);
        if (pos == NPOS)
        {
            erreur("Impossible de remplacer la variable dans le modèle.", standardError, 0);
            return;
        }

        deb_val = html_string.find("-->", pos + strlen(tagname.c_str()));
        if (deb_val == NPOS)
        {
            erreur("Impossible de remplacer la variable dans le modèle.", standardError, 0);
            return;
        }

        deb_val += 3;
        fin_val = html_string.find("<!--", deb_val);
        if (fin_val == NPOS)
        {
            erreur("Impossible de remplacer la variable dans le modèle.", standardError, 0);
            return;
        }

        // remplacement de la variable
        html_string.replace(deb_val, fin_val - deb_val, texteHtml(value));

        AfficheListeVar();
    }

    delete pDlg;
}

void
NSEnTeteEditDialog::CmOk()
{
	ofstream outFile ;

	if (bParseOK)
	{
		string fichier = pContexte->PathName("NTPL") + string("entete_") +
                        pContexte->getUtilisateur()->getNss() + string(".htm") ;

    outFile.open(fichier.c_str()) ;    if (!outFile)
    {
    	erreur("Erreur d'écriture du fichier d'en-tete.", standardError, 0) ;      return ;    }
    for (size_t i = 0; i < strlen(html_string.c_str()); i++)    	outFile.put(html_string[i]) ;

    outFile.close() ;  }	NSUtilDialog::CmOk() ;
}

void
NSEnTeteEditDialog::CmCancel()
{
    NSUtilDialog::CmCancel();
}

void
NSEnTeteEditDialog::CmHelp()
{
    // Les valeurs de sHindex et sHcorps sont déjà fixées dans le contexte (SetupWindow)
    pContexte->NavigationAideEnLigne();
}

//***************************************************************************
//
//  Méthodes de NSVarTagEditDialog
//
//***************************************************************************

DEFINE_RESPONSE_TABLE1(NSVarTagEditDialog, NSUtilDialog)
    EV_COMMAND(IDOK, CmOk),
    EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE;

NSVarTagEditDialog::NSVarTagEditDialog(TWindow* pere, NSContexte* pCtx, NSVarTag* pVar)
                :NSUtilDialog(pere, pCtx, "IDD_VARTAG_EDIT", pNSResModule)
{
    pVarTag = pVar;
    pEditName = new TEdit(this, IDC_VARTAG_EDITNAME);
    pEditValue = new TEdit(this, IDC_VARTAG_EDITVALUE);
}

NSVarTagEditDialog::~NSVarTagEditDialog()
{
    delete pEditName;
    delete pEditValue;
}

void
NSVarTagEditDialog::SetupWindow()
{
    NSUtilDialog::SetupWindow();
    pEditName->SetText((pVarTag->name).c_str());
    pEditValue->SetText((pVarTag->value).c_str());
    pEditValue->SetFocus();
}

void
NSVarTagEditDialog::CmOk()
{
    char text[256];

    pEditName->GetText(text, 255);
    if (string(text) != pVarTag->name)
    {
        erreur("Vous ne pouvez pas modifier le nom de la variable.", standardError, 0);
        return;
    }

    pEditValue->GetText(text, 255);
    if (string(text) != pVarTag->value)
        pVarTag->value = string(text);

    NSUtilDialog::CmOk();
}

void
NSVarTagEditDialog::CmCancel()
{
	NSUtilDialog::CmCancel() ;
}

//***************************************************************************
//
//  Méthodes de NSEditTemplateDialog
//
//***************************************************************************

DEFINE_RESPONSE_TABLE1(NSEditTemplateDialog, NSUtilDialog)
	EV_COMMAND(IDC_CREATTPL_ENTETE, CmEnTete),
	EV_COMMAND(IDC_CREATTPL_FICHIER, CmFichier),
	EV_COMMAND(IDOK, CmOk),
	EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE;

NSEditTemplateDialog::NSEditTemplateDialog(TWindow* pere, NSContexte* pCtx, NSTemplateInfo* pTpl)
                     :NSUtilDialog(pere, pCtx, "IDD_CREAT_TPL", pNSResModule)
{
	pTplInfo = pTpl ;

  // Récupération des types document par le réseau sémantique
  //
  string sConceptSens = string("0CLDO") ;
  VectString CodeTypes ;
  pContexte->getSuperviseur()->getFilGuide()->TousLesVrais(sConceptSens, "ES", &CodeTypes, "ENVERS") ;
  if (false == CodeTypes.empty())
  	for (IterString i = CodeTypes.begin() ; CodeTypes.end() != i ; i++)
  		pContexte->getDico()->donneCodeComplet(*(*i)) ;

  // on crée maintenant une patpatho pour sauvegarder la template
  pPatPatho = new NSPatPathoArray(pContexte->getSuperviseur(), graphObject);

  // Initialisation des controles
  pTypeDoc = new NSComboBox(this, IDC_CREATTPL_COMBO, pContexte, &CodeTypes) ;
  pUtil    = new TGroupBox(this, IDC_CREATTPL_UTIL) ;
  pUtil1   = new TRadioButton(this, IDC_CREATTPL_UTIL1, pUtil) ;
  pUtil2   = new TRadioButton(this, IDC_CREATTPL_UTIL2, pUtil) ;
  pConsole = new NSUtilEdit(pContexte, this, IDC_CREATTPL_CONSOLE, 15) ;
  pRoot    = new NSUtilLexique(pContexte, this, IDC_CREATTPL_ROOT, pContexte->getDico()) ;
  pLibelle = new NSUtilEdit(pContexte, this, IDC_CREATTPL_LIBELLE, 255) ;
  pEnTete  = new TButton(this, IDC_CREATTPL_ENTETE) ;
  pFichier = new TButton(this, IDC_CREATTPL_FICHIER) ;
  pTypeTpl = new TGroupBox(this, IDC_CREATTPL_TYPE) ;
  pCompo   = new TRadioButton(this, IDC_CREATTPL_COMPO, pTypeTpl) ;
	pDefaut  = new TRadioButton(this, IDC_CREATTPL_DEFAUT, pTypeTpl) ;

  sLibelle     = string("") ;
  sEnTete      = string("") ;
	sFichier     = string("") ;
	sTypeDoc     = string("") ;
	sOper        = string("") ;
	sCompo       = string("") ;
	sDefaut      = string("") ;
	sNoConsole   = string("") ;
	sLexiqueRoot = string("") ;
}

NSEditTemplateDialog::~NSEditTemplateDialog()
{
  delete pTypeDoc ;
  delete pUtil ;
  delete pUtil1 ;
  delete pUtil2 ;
  delete pConsole ;
  delete pRoot ;
  delete pLibelle ;
  delete pEnTete ;
  delete pFichier ;
  delete pTypeTpl ;
  delete pCompo ;
  delete pDefaut ;
}

void
NSEditTemplateDialog::SetupWindow()
{
	NSUtilDialog::SetupWindow() ;

  if (pTplInfo)
	{
  	sLibelle     = pTplInfo->getLibelle() ;
		sEnTete      = pTplInfo->getEnTete() ;
  	sFichier     = pTplInfo->getFichier() ;
    sLexiqueRoot = pTplInfo->getRoot() ;
    sTypeDoc     = pTplInfo->getType() ;
		sOper        = pTplInfo->getUser() ;
		sCompo       = pTplInfo->getCompo() ;
		sDefaut      = pTplInfo->getDefaut() ;
		sNoConsole   = pTplInfo->getConsole() ;
  }

  setHeaderInfo() ;
	setFileInfo() ;
  setDocInfo() ;
	setuserInfo() ;
	setCompoInfo() ;
	setDefaultInfo() ;
	setRootInfo() ;
	setConsoleInfo() ;
	setLibelleInfo() ;
}

void
NSEditTemplateDialog::setHeaderInfo()
{
	if (sEnTete != string(""))
  	pEnTete->SetCaption(sEnTete.c_str()) ;
}

void
NSEditTemplateDialog::setFileInfo()
{
	if (sFichier != string(""))
  	pFichier->SetCaption(sFichier.c_str()) ;
}

void
NSEditTemplateDialog::setDocInfo()
{
	if (sTypeDoc != string(""))
		pTypeDoc->setCode(sTypeDoc) ;
}

void
NSEditTemplateDialog::setuserInfo()
{
}

void
NSEditTemplateDialog::setCompoInfo()
{
	if (sCompo == string("WCEA01"))
		pCompo->SetCheck(BF_CHECKED) ;
	else
  	pCompo->SetCheck(BF_UNCHECKED) ;
}

void
NSEditTemplateDialog::setDefaultInfo()
{
	if (sDefaut == string("WCEA01"))
		pDefaut->SetCheck(BF_CHECKED) ;
  else
  	pDefaut->SetCheck(BF_UNCHECKED) ;
}

void
NSEditTemplateDialog::setRootInfo()
{
	if (sLexiqueRoot != string(""))
		pRoot->setLabel(sLexiqueRoot) ;
}

void
NSEditTemplateDialog::setConsoleInfo()
{
  if (string("") != sNoConsole)
    pConsole->SetText(sNoConsole.c_str()) ;
}

void
NSEditTemplateDialog::setLibelleInfo()
{
	pLibelle->SetText(sLibelle.c_str()) ;
}

string
NSEditTemplateDialog::TrouveNomFichier()
{
	char path[1024] ;
	// on choisi d'abord le répertoire par défaut (NTPL)
	strcpy(path,(pContexte->PathName("NTPL")).c_str()) ;

	TFileOpenDialog::TData initData(OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,
																	"Tous fichiers (*.*)|*.*|",
																	0,
																	path,
																	"*") ;

	int iUserChoice = TFileOpenDialog(pContexte->GetMainWindow(), initData).Execute() ;

	pContexte->reallocateBaseDirectory() ;

	if (IDOK != iUserChoice)
		return string("") ;

	string sFileName = string(initData.FileName) ;

	// Récupération du path
	size_t pos1 = sFileName.find_last_of("\\");
	if (NPOS == pos1)
	{
  	/* on doit normalement récupérer au moins C:\
    // => on génère une erreur s'il n'y a pas de '\\'  */
    erreur("Aucun PathName ne peut etre associé à ce fichier.", standardError, 0) ;
    return string("") ;
	}

	string sNomSeul = string(sFileName, pos1+1, strlen(sFileName.c_str())-pos1-1) ;
	return sNomSeul ;
}

void
NSEditTemplateDialog::CmEnTete()
{
  string sFileName = TrouveNomFichier() ;
  if (string("") != sFileName)
  {
    sEnTete = sFileName ;
    setHeaderInfo() ;
  }
}

void
NSEditTemplateDialog::CmFichier()
{
	string sFileName = TrouveNomFichier() ;
	if (string("") != sFileName)
	{
    sFichier = sFileName ;
    setFileInfo() ;
	}
}

void
NSEditTemplateDialog::CmOk()
{
	// contrôles
	if (sEnTete == "")
	{
  	erreur("Vous n'avez pas spécifié le fichier d'en-tete.", standardError, 0) ;
    return ;
	}

	if (sFichier == "")
	{
  	erreur("Vous n'avez pas spécifié le fichier modèle.", standardError, 0) ;
    return ;
  }

	sTypeDoc = pTypeDoc->getSelCode() ;
	bool bOper = false ;

	if (pUtil2->GetCheck() == BF_CHECKED)
		bOper = true ;

	if (bOper)
  	sOper = pContexte->getUtilisateur()->getNss() ;
  else
  	sOper = "9VOID1" ;

	sLexiqueRoot = pRoot->getCode() ;

  // numéro de console
	string sConsole = pConsole->GetText() ;

  if (strlen(sConsole.c_str()) > 0)
  	sNoConsole = sConsole ;
  else
  	sNoConsole = "9VOID1" ;

  // Compo
  if (pCompo->GetCheck() == BF_CHECKED)
  	sCompo = string("WCEA01") ;
	else
  	sCompo = string("WCE001") ;

  // Defaut
  if (pDefaut->GetCheck() == BF_CHECKED)
  	sDefaut = string("WCEA01") ;
  else
  	sDefaut = string("WCE001") ;

  sLibelle = pLibelle->GetText() ;

  fillPatPatho() ;
  fillTplInfo() ;

	NSUtilDialog::CmOk() ;
}

void
NSEditTemplateDialog::CmCancel()
{
	NSUtilDialog::CmCancel() ;
}

void
NSEditTemplateDialog::fillPatPatho()
{
	if (NULL == pPatPatho)
		return ;

	pPatPatho->vider() ;

	// noeud racine
  pPatPatho->ajoutePatho("0OTPL1", 0) ;

	// type document
  pPatPatho->ajoutePatho("0TYPE1", 1) ;
  pPatPatho->ajoutePatho(sTypeDoc, 2) ;

	// operateur
	if ((string("") != sOper) && (sOper != string("9VOID1")))
	{
    pPatPatho->ajoutePatho("DOPER1", 1) ;
    Message Msg ;
    Msg.SetComplement(sOper) ;
    pPatPatho->ajoutePatho("£SPID1", &Msg, 2) ;
  }

	// numéro de console
  if ((string("") != sNoConsole) && (sNoConsole != string("9VOID1")))
  {
  	pPatPatho->ajoutePatho("LNUCO1", 1) ;
    Message Msg ;
    Msg.SetComplement(sNoConsole) ;
    pPatPatho->ajoutePatho("£C;020", &Msg, 2) ;
  }

  // lexique root
  if (string("") != sLexiqueRoot)
  {
  	pPatPatho->ajoutePatho("0TYPC1", 1) ;
    pPatPatho->ajoutePatho(sLexiqueRoot, 2) ;
  }

  // flag composition
  if (sCompo == string("WCEA01"))
  	pPatPatho->ajoutePatho("0COMD1", 1) ;

  // flag defaut
  if (sDefaut == string("WCEA01"))
  	pPatPatho->ajoutePatho("0DEFA1", 1) ;

  // Fichier Template
  pPatPatho->ajoutePatho("0TPL01", 1) ;
  Message MsgTpl ;
  MsgTpl.SetTexteLibre(sFichier) ;
  pPatPatho->ajoutePatho("£?????", &MsgTpl, 2) ;

  // Fichier En-tete
  pPatPatho->ajoutePatho("0ENTE1", 1) ;
  Message MsgHead ;
  MsgHead.SetTexteLibre(sEnTete) ;
  pPatPatho->ajoutePatho("£?????", &MsgHead, 2) ;

  // Libelle
  pPatPatho->ajoutePatho("0INTI1", 1) ;
  Message MsgLib ;
  MsgLib.SetTexteLibre(sLibelle) ;
  pPatPatho->ajoutePatho("£?????", &MsgLib, 2) ;
}

void
NSEditTemplateDialog::fillTplInfo()
{
	if (NULL == pTplInfo)
		return ;

	pTplInfo->setType(sTypeDoc) ;
	pTplInfo->setUser(sOper) ;
  pTplInfo->setConsole(sNoConsole) ;
	pTplInfo->setRoot(sLexiqueRoot) ;
	pTplInfo->setCompo(sCompo) ;
	pTplInfo->setDefaut(sDefaut) ;
	pTplInfo->setEnTete(sEnTete) ;
	pTplInfo->setFichier(sFichier) ;
	pTplInfo->setLibelle(sLibelle) ;
}

/************************************************************************/
/*                                                                      */
/*                      CLASSE NSTEMPLATEMANAGER                        */
/*                                                                      */
/************************************************************************/

NSTemplateManager::NSTemplateManager(NSContexte* pCtx) : NSRoot(pCtx)
{
	pTplArray = new NSTemplateArray() ;
}

NSTemplateManager::NSTemplateManager(const NSTemplateManager& rv)
                  :NSRoot(rv.pContexte)
{
	pTplArray = new NSTemplateArray(*(rv.pTplArray)) ;
}

NSTemplateManager::~NSTemplateManager()
{
	delete pTplArray ;
}

NSTemplateManager&
NSTemplateManager::operator=(const NSTemplateManager& src)
{
	if (this == &src)
		return *this ;

	*pTplArray = *(src.pTplArray) ;

	return *this ;
}

bool
NSTemplateManager::LoadTemplateList(string sTypeDoc)
{
	NSDataGraph Graph(pContexte->getSuperviseur(), graphObject) ;
	NSPatPathoArray PatPathoArray(pContexte->getSuperviseur(), graphObject) ;

	NSPersonsAttributesArray ObjList ;
	NSBasicAttributeArray    AttrList ;

  AttrList.push_back(new NSBasicAttribute("_0OTPL_0TYPE",  sTypeDoc)) ;

  bool res = pContexte->getSuperviseur()->getPilot()->invokeService(NautilusPilot::SERV_SEARCH_OBJECT.c_str(),
                                  &Graph, &ObjList, &AttrList) ;

  if (false == res)
  {
    string sErrorText = pContexte->getSuperviseur()->getText("NTIERS", "templateNotFound") ;
    erreur(sErrorText.c_str(), standardError, 0, 0) ;
    return false ;
  }

  for (NSDataTreeIter iterTree = Graph.getTrees()->begin(); iterTree != Graph.getTrees()->end(); iterTree++)
  {
    (*iterTree)->getRawPatPatho(&PatPathoArray) ;

    initInfo(&PatPathoArray) ;
  }

  return true ;
}

void
NSTemplateManager::initInfo(NSPatPathoArray* pPPT)
{
  if (((NSPatPathoArray*) NULL == pPPT) || (pPPT->empty()))
    return ;

	PatPathoIter iter    = pPPT->begin() ;
	PatPathoIter iterEnd = pPPT->end() ;

	while (iter != iterEnd)
  {
  	string sSens = (*iter)->getLexiqueSens() ;

    if (sSens == "0OTPL")
    {
      NSTemplateInfo* pTplInfo = new NSTemplateInfo() ;

      pTplInfo->setCode((*iter)->getDoc()) ;

      int iColTpl = (*iter)->getColonne() ;

      pTplInfo->setCompo("WCE001") ;
      pTplInfo->setDefaut("WCE001") ;
      iter++ ;

      // on charge les données de l'archetype
      while ((iter != iterEnd) && ((*iter)->getColonne() > iColTpl))
      {
        sSens = (*iter)->getLexiqueSens() ;

        // type de document
        //
        if (string("0TYPE") == sSens)
        {
          string sTypeDocum = string("") ;
          iter++ ;

          while ((iter != iterEnd) && ((*iter)->getColonne() > iColTpl+1))
          {
            string sElemLex = (*iter)->getLexique() ;

            if (string(sElemLex, 0, 3) == string("£C;"))
              sTypeDocum = (*iter)->getComplement() ;
            else
            	sTypeDocum = sElemLex ;

            iter++ ;
          }

          if (sTypeDocum != "")
          	pTplInfo->setType(sTypeDocum) ;
        }
        // Opérateur - Operator
        //
        else if (string("DOPER") == sSens)
        {
          string sUtil = string("") ;
          iter++ ;

          while ((iter != iterEnd) && ((*iter)->getColonne() > iColTpl+1))
          {
            string sElemLex = (*iter)->getLexique() ;

            if (sElemLex == string("£SPID1"))
              sUtil = (*iter)->getComplement() ;

            iter++ ;
          }

          if (sUtil != "")
          	pTplInfo->setUser(sUtil) ;
        }
        // Numéro de console
        //
        else if (string("LNUCO") == sSens)
        {
          string sConsole = string("") ;
          iter++ ;

          while ((iter != iterEnd) && ((*iter)->getColonne() > iColTpl+1))
          {
            string sElemLex = (*iter)->getLexique() ;

            if (string(sElemLex, 0, 3) == string("£C;"))
              sConsole = (*iter)->getComplement() ;

            iter++ ;
          }

          if (string("") != sConsole)
          	pTplInfo->setConsole(sConsole) ;
        }
        // type de contenu (code lexique)
        //
        else if (string("0TYPC") == sSens)
        {
          string sRootLex = "" ;
          iter++ ;

          while ((iter != iterEnd) && ((*iter)->getColonne() > iColTpl+1))
          {
            // on cherche ici un code lexique
            sRootLex = (*iter)->getLexique() ;
            iter++ ;
          }

          if (sRootLex != "")
          	pTplInfo->setRoot(sRootLex) ;
        }
        // en-tete
        //
        else if (string("0ENTE") == sSens)
        {
          string sTexte = "" ;
          iter++ ;

          while ((iter != iterEnd) && ((*iter)->getColonne() > iColTpl+1))
          {
            // on cherche ici un texte libre
            string sElemLex = (*iter)->getLexique() ;
            if (sElemLex == string("£?????"))
              sTexte = (*iter)->getTexteLibre() ;

            iter++ ;
          }

          if (sTexte != "")
          	pTplInfo->setEnTete(sTexte) ;
        }
        // fichier template
        //
        else if (string("0TPL0") == sSens)
        {
          string sTexte = "" ;
          iter++ ;

          while ((iter != iterEnd) && ((*iter)->getColonne() > iColTpl+1))
          {
            // on cherche ici un texte libre
            string sElemLex = (*iter)->getLexique() ;
            if (sElemLex == string("£?????"))
              sTexte = (*iter)->getTexteLibre() ;

            iter++ ;
          }

          if (sTexte != "")
          	pTplInfo->setFichier(sTexte) ;
        }
        // libellé
        //
        else if (string("0INTI") == sSens)
        {
          string sTexte = "" ;
          iter++ ;

          while ((iter != iterEnd) && ((*iter)->getColonne() > iColTpl+1))
          {
            // on cherche ici un texte libre
            string sElemLex = (*iter)->getLexique() ;
            if (sElemLex == string("£?????"))
              sTexte = (*iter)->getTexteLibre() ;

            iter++ ;
          }

          if (sTexte != "")
          	pTplInfo->setLibelle(sTexte) ;
        }
        // template de composition
        else if (string("0COMD") == sSens)
        {
        	pTplInfo->setCompo("WCEA01") ;
          iter++ ;
        }
        // template par défaut
        else if (string("0DEFA") == sSens)
        {
        	pTplInfo->setDefaut("WCEA01") ;
          iter++ ;
        }
        else
          iter++ ;
      }

      pTplArray->push_back(pTplInfo) ;
    }
    else
    {
      erreur("Le noeud du modele est mal positionné dans ::LoadTemplateList().", standardError, 0) ;
      return ;
    }
  }
}

void
NSTemplateManager::LoadAllTemplates()
{
	pTplArray->vider() ;

  LoadTemplatesByType("WCEA01") ;
  LoadTemplatesByType("WCE001") ;
}

void
NSTemplateManager::LoadTemplatesByType(string sDefaultAttribute)
{
	VecteurString FoundObjects ;

	// on récupère la liste des matériels : actifs et non-actifs
	NSPersonsAttributesArray ObjList ;
	NSBasicAttributeArray    AttrList ;
	AttrList.push_back(new NSBasicAttribute("_0OTPL_0DEFA", sDefaultAttribute)) ;
	AttrList.push_back(new NSBasicAttribute(RESEARCH, EQUAL_RESEARCH)) ;

	bool res = pContexte->getSuperviseur()->getPilot()->objectList(NautilusPilot::SERV_OBJECT_LIST.c_str(), &ObjList, &AttrList) ;
	if (!res)
	{
		string sErrorText = pContexte->getSuperviseur()->getText("NTIERS", "templateNotFound") ;
		erreur(sErrorText.c_str(), standardError, 0, 0) ;
		return ;
	}

	if (false == ObjList.empty())
		for (NSPersonsAttributeIter k = ObjList.begin() ; ObjList.end() != k ; k++)
		{
			string sOIDS = (*k)->getAttributeValue(OIDS) ;
			FoundObjects.AddString(sOIDS) ;
		}

	for (EquiItemIter i = FoundObjects.begin() ; FoundObjects.end() != i ; i++)
		initTemplateFromOids(**i) ;
}

void
NSTemplateManager::getTemplateTreeFromOids(string sTplOids, NSPatPathoArray* pPPT)
{
  if ((string("") == sTplOids) || ((NSPatPathoArray*) NULL == pPPT))
    return ;

  // ------------ Appel du pilote
  //
  NSDataGraph Graph(pContexte->getSuperviseur(), graphObject) ;
  NSBasicAttributeArray AttrArray ;
	AttrArray.push_back(new NSBasicAttribute(OBJECT, sTplOids)) ;
  bool res = pContexte->getSuperviseur()->getPilot()->invokeService(NautilusPilot::SERV_SEARCH_OBJECT_FROM_ID.c_str(),
                                  &Graph,  &AttrArray) ;
  if (false == res)
  {
		string sErrorText = pContexte->getSuperviseur()->getText("NTIERS", "templateNotFound") + string(" ") + sTplOids ;
    erreur(sErrorText.c_str(), standardError, 0) ;
    return ;
  }

  if (Graph.getTrees()->empty())
		return ;

	NSDataTreeIter iterTree = Graph.getTrees()->begin() ;
	(*iterTree)->getRawPatPatho(pPPT) ;
}

void
NSTemplateManager::initTemplateFromOids(string sTplOids)
{
  NSPatPathoArray PPT(pContexte->getSuperviseur(), graphObject) ;

  getTemplateTreeFromOids(sTplOids, &PPT) ;

	initInfo(&PPT) ;
}

// -------------------------- NSTemplateWindow --------------------------
//

const int ID_TemplateList = 0x110 ;

#define CM_TPL_NEW    100
#define CM_TPL_EDIT   101#define CM_TPL_DELETE 102

DEFINE_RESPONSE_TABLE1(NSTemplateWindow, TWindow)
	EV_WM_SIZE,
	EV_LVN_GETDISPINFO(ID_TemplateList, DispInfoListe),
  EV_LVN_COLUMNCLICK(ID_TemplateList, LVNColumnclick),
  EV_COMMAND(CM_TPL_NEW,    CmNouveau),
  EV_COMMAND(CM_TPL_EDIT,   CmModifier),
	EV_COMMAND(CM_TPL_DELETE, CmSupprimer),
END_RESPONSE_TABLE ;

NSTemplateWindow::NSTemplateWindow(NSContexte* pCtx, TWindow* parent, const char far* title)
                 :TWindow(parent, title), NSRoot(pCtx)
{
	pTemplates = new NSTemplateManager(pContexte) ;
  pTemplates->LoadAllTemplates() ;

	pListeWindow = new NSTemplateListWindow(this, ID_TemplateList, 0, 0, 0, 0) ;
}

NSTemplateWindow::~NSTemplateWindow()
{
	delete pTemplates ;
}

// Appel de la fonction de remplissage de la vue
void
NSTemplateWindow::SetupWindow()
{
  TWindow::SetupWindow() ;

  string sCaption = pContexte->getSuperviseur()->getText("templateManagement", "templateManagement") ;
	Parent->SetCaption(sCaption.c_str()) ;

  SetupColumns() ;
  AfficheListe() ;
}

// Initialisation des colonnes de la ListWindow
void
NSTemplateWindow::SetupColumns()
{
	string sTplType   = pContexte->getSuperviseur()->getText("templateManagement", "type") ;
  string sTplUser   = pContexte->getSuperviseur()->getText("templateManagement", "user") ;
  string sTplCnsle  = pContexte->getSuperviseur()->getText("templateManagement", "console") ;
  string sTplDoc    = pContexte->getSuperviseur()->getText("templateManagement", "documentType") ;
  string sTplHeader = pContexte->getSuperviseur()->getText("templateManagement", "headerFile") ;
  string sTplTpl    = pContexte->getSuperviseur()->getText("templateManagement", "template") ;
  string sTplTitle  = pContexte->getSuperviseur()->getText("templateManagement", "title") ;
  string sTplCompo  = pContexte->getSuperviseur()->getText("templateManagement", "composition") ;
  string sTplDefau  = pContexte->getSuperviseur()->getText("templateManagement", "default") ;

  pListeWindow->InsertColumn(0, TListWindColumn((char*)sTplTitle.c_str(), 150, TListWindColumn::Left, 0)) ;
  pListeWindow->InsertColumn(1, TListWindColumn((char*)sTplType.c_str(),   80, TListWindColumn::Left, 1)) ;
  pListeWindow->InsertColumn(2, TListWindColumn((char*)sTplUser.c_str(),   80, TListWindColumn::Left, 2)) ;
  pListeWindow->InsertColumn(3, TListWindColumn((char*)sTplCnsle.c_str(),  80, TListWindColumn::Left, 3)) ;
  pListeWindow->InsertColumn(4, TListWindColumn((char*)sTplDoc.c_str(),    80, TListWindColumn::Left, 4)) ;
  pListeWindow->InsertColumn(5, TListWindColumn((char*)sTplCompo.c_str(),  50, TListWindColumn::Left, 5)) ;
  pListeWindow->InsertColumn(6, TListWindColumn((char*)sTplDefau.c_str(),  50, TListWindColumn::Left, 6)) ;
  pListeWindow->InsertColumn(7, TListWindColumn((char*)sTplHeader.c_str(),150, TListWindColumn::Left, 7)) ;
  pListeWindow->InsertColumn(8, TListWindColumn((char*)sTplTpl.c_str(),   150, TListWindColumn::Left, 8)) ;
}

// Affichage des éléments de la listevoid
NSTemplateWindow::AfficheListe()
{
  pListeWindow->DeleteAllItems() ;

	NSTemplateArray* pTplArray = pTemplates->pTplArray ;

  if (pTplArray->empty())
    return ;

  // Attention : insert insère au dessus ; il faut inscrire les derniers en premier
  NSTemplateIter itDg = pTplArray->end() ;
  do
  {
    itDg-- ;
    TListWindItem Item(((*itDg)->getLibelle()).c_str(), 0) ;
    pListeWindow->InsertItem(Item) ;
	}
  while (itDg != pTplArray->begin()) ;
}

void
NSTemplateWindow::DispInfoListe(TLwDispInfoNotify& dispInfo)
{
  const int       BufLen = 255 ;
  static char     buffer[BufLen] ;
  TListWindItem   &dispInfoItem = *(TListWindItem *)&dispInfo.item ;
  buffer[0] = '\0' ;

  int index = dispInfoItem.GetIndex() ;

	string sLang = "" ;
	if (pContexte->getUtilisateur())
		sLang = pContexte->getUtilisateur()->donneLang() ;

  string sRawData = string("") ;
  string sLibelle = string("") ;

  // Affiche les informations en fonction de la colonne  switch (dispInfoItem.GetSubItem())
  {
    case 1  : // tpl type
    	sRawData = ((*(pTemplates->pTplArray))[index])->getType() ;
    	if (sRawData != string(""))
      {
    		pContexte->getDico()->donneLibelle(sLang, &sRawData, &sLibelle) ;
      	strcpy(buffer, sLibelle.c_str()) ;
      }
      dispInfoItem.SetText(buffer) ;
      break ;

    case 2  : // user    {      string sUser = ((*(pTemplates->pTplArray))[index])->getUser() ;      if ((string("") != sUser) && (string("9VOID") != string(sUser, 0, 5)))
      {
        NSPersonInfo *pPersonInfo = pContexte->getPersonArray()->getPerson(pContexte, sUser, pidsUtilisat) ;
        if (NULL != pPersonInfo)
        {
  	      string sMemberName = pPersonInfo->getCivilite() ;
          strcpy(buffer, sMemberName.c_str()) ;
          dispInfoItem.SetText(buffer) ;
        }
      }
      break ;
    }

    case 3  : // console
    {      string sConsole = ((*(pTemplates->pTplArray))[index])->getConsole() ;      if ((string("") != sConsole) && (string("9VOID") != string(sConsole, 0, 5)))
      {
        strcpy(buffer, sConsole.c_str()) ;
        dispInfoItem.SetText(buffer) ;
      }
      break ;
    }

    case 4  : // doc type
    	sRawData = ((*(pTemplates->pTplArray))[index])->getRoot() ;
    	if (sRawData != string(""))
      {
    		pContexte->getDico()->donneLibelle(sLang, &sRawData, &sLibelle) ;
      	strcpy(buffer, sLibelle.c_str()) ;
      }
      dispInfoItem.SetText(buffer) ;
      break ;

    case 5  : // compo
      sRawData = ((*(pTemplates->pTplArray))[index])->getCompo() ;
      if (sRawData != string(""))
      {
    		pContexte->getDico()->donneLibelle(sLang, &sRawData, &sLibelle) ;
      	strcpy(buffer, sLibelle.c_str()) ;
      }
      dispInfoItem.SetText(buffer) ;
      break ;

    case 6  : // default
    	sRawData = ((*(pTemplates->pTplArray))[index])->getDefaut() ;
      if (sRawData != string(""))
      {
    		pContexte->getDico()->donneLibelle(sLang, &sRawData, &sLibelle) ;
      	strcpy(buffer, sLibelle.c_str()) ;
      }
      dispInfoItem.SetText(buffer) ;
      break ;

    case 7  : // header
    	strcpy(buffer, ((*(pTemplates->pTplArray))[index])->getEnTete().c_str()) ;
      dispInfoItem.SetText(buffer) ;
      break ;

  	case 8  : // template
    	strcpy(buffer, ((*(pTemplates->pTplArray))[index])->getFichier().c_str()) ;
      dispInfoItem.SetText(buffer) ;
      break ;
  }
}

void
NSTemplateWindow::sortAgain()
{
	switch (iSortedColumn)
  {
  	case 0 : sortByLibelle() ; break ;
    case 1 : sortByType() ;    break ;
    case 2 : sortByUser() ;    break ;
    case 3 : sortByRoot() ;    break ;
    case 4 : sortByCompo() ;   break ;
    case 5 : sortByDefault() ; break ;
    case 6 : sortByHeader() ;  break ;
    case 7 : sortByTpl() ;     break ;
	}
}

void
NSTemplateWindow::LVNColumnclick(TLwNotify& lwn)
{
	changeSortOrder(lwn.iSubItem) ;

  switch ( lwn.iSubItem )
  {
    case 0  : sortByLibelle() ; break ;
    case 1  : sortByType() ;    break ;
    case 2  : sortByUser() ;    break ;
    case 3  : sortByRoot() ;    break ;
    case 4  : sortByCompo() ;   break ;
    case 5  : sortByDefault() ; break ;
    case 6  : sortByHeader() ;  break ;
    case 7  : sortByTpl() ;     break ;
  }
}

void
NSTemplateWindow::changeSortOrder(int iOrder)
{
	if (iSortedColumn == iOrder)
  {
    if (bNaturallySorted)
      bNaturallySorted = false ;
    else
      bNaturallySorted = true ;
  }
  else
  {
    iSortedColumn = iOrder ;
    bNaturallySorted = true ;
  }
}

void
NSTemplateWindow::sortByUser()
{
  if (pTemplates->pTplArray->empty())
    return ;

  if (bNaturallySorted)
    sort(pTemplates->pTplArray->begin(), pTemplates->pTplArray->end(), templateSortByUserInf) ;
  else
    sort(pTemplates->pTplArray->begin(), pTemplates->pTplArray->end(), templateSortByUserSup) ;

  AfficheListe() ;
}

void
NSTemplateWindow::sortByLibelle()
{
  if (pTemplates->pTplArray->empty())
    return ;

  if (bNaturallySorted)
    sort(pTemplates->pTplArray->begin(), pTemplates->pTplArray->end(), templateSortByLibelleInf) ;
  else
    sort(pTemplates->pTplArray->begin(), pTemplates->pTplArray->end(), templateSortByLibelleSup) ;

  AfficheListe() ;
}

void
NSTemplateWindow::sortByType()
{
  if (pTemplates->pTplArray->empty())
    return ;

  if (bNaturallySorted)
    sort(pTemplates->pTplArray->begin(), pTemplates->pTplArray->end(), templateSortByTypeInf) ;
  else
    sort(pTemplates->pTplArray->begin(), pTemplates->pTplArray->end(), templateSortByTypeSup) ;

  AfficheListe() ;
}

void
NSTemplateWindow::sortByRoot()
{
  if (pTemplates->pTplArray->empty())
    return ;

  if (bNaturallySorted)
    sort(pTemplates->pTplArray->begin(), pTemplates->pTplArray->end(), templateSortByRootInf) ;
  else
    sort(pTemplates->pTplArray->begin(), pTemplates->pTplArray->end(), templateSortByRootSup) ;

  AfficheListe() ;
}

void
NSTemplateWindow::sortByHeader()
{
  if (pTemplates->pTplArray->empty())
    return ;

  if (bNaturallySorted)
    sort(pTemplates->pTplArray->begin(), pTemplates->pTplArray->end(), templateSortByHeaderInf) ;
  else
    sort(pTemplates->pTplArray->begin(), pTemplates->pTplArray->end(), templateSortByHeaderSup) ;

  AfficheListe() ;
}

void
NSTemplateWindow::sortByTpl()
{
  if (pTemplates->pTplArray->empty())
    return ;

  if (bNaturallySorted)
    sort(pTemplates->pTplArray->begin(), pTemplates->pTplArray->end(), templateSortByFileInf) ;
  else
    sort(pTemplates->pTplArray->begin(), pTemplates->pTplArray->end(), templateSortByFileSup) ;

  AfficheListe() ;
}

void
NSTemplateWindow::sortByCompo()
{
  if (pTemplates->pTplArray->empty())
    return ;

  if (bNaturallySorted)
    sort(pTemplates->pTplArray->begin(), pTemplates->pTplArray->end(), templateSortByCompoInf) ;
  else
    sort(pTemplates->pTplArray->begin(), pTemplates->pTplArray->end(), templateSortByCompoSup) ;

  AfficheListe() ;
}

void
NSTemplateWindow::sortByDefault()
{
  if (pTemplates->pTplArray->empty())
    return ;

  if (bNaturallySorted)
    sort(pTemplates->pTplArray->begin(), pTemplates->pTplArray->end(), templateSortByDefaultInf) ;
  else
    sort(pTemplates->pTplArray->begin(), pTemplates->pTplArray->end(), templateSortByDefaultSup) ;

  AfficheListe() ;
}

// fonction Click droit : menu contextuel
void
NSTemplateWindow::EvRButtonDown(uint /* modkeys */, NS_CLASSLIB::TPoint& point)
{
	// création d'un menu popup
	NS_CLASSLIB::TPoint lp = point;

	TPopupMenu *menu = new TPopupMenu();

	NSSuper* pSuper = pContexte->getSuperviseur() ;

	string sNew  = pSuper->getText("generalLanguageForLists", "add") ;
	string sRemo = pSuper->getText("generalLanguageForLists", "remove") ;
	string sModi = pSuper->getText("generalLanguageForLists", "modify") ;

	menu->AppendMenu(MF_STRING, CM_TPL_NEW,    sNew.c_str()) ;
  menu->AppendMenu(MF_STRING, CM_TPL_EDIT,   sModi.c_str()) ;
	menu->AppendMenu(MF_SEPARATOR, 0, 0);
	menu->AppendMenu(MF_STRING, CM_TPL_DELETE, sRemo.c_str()) ;

	ClientToScreen(lp);
	menu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, lp, 0, HWindow);
	delete menu;
}

void
NSTemplateWindow::EvRButtonDownOut(uint /* modkeys */, NS_CLASSLIB::TPoint& point)
{
	// création d'un menu popup
  NS_CLASSLIB::TPoint lp = point ;

  TPopupMenu *menu = new TPopupMenu() ;

	NSSuper* pSuper = pContexte->getSuperviseur() ;

	string sNew  = pSuper->getText("generalLanguageForLists", "add") ;

	menu->AppendMenu(MF_STRING, CM_TPL_NEW,    sNew.c_str()) ;

  ClientToScreen(lp) ;
  menu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, lp, 0, HWindow) ;
  delete menu ;
}

void
NSTemplateWindow::CmNouveau()
{
try
{
	NSEditTemplateDialog* pDlg = new NSEditTemplateDialog(pContexte->GetMainWindow(), pContexte);
	if (pDlg->Execute() != IDOK)
  {
  	delete pDlg ;
    return ;
	}

	NSObjectGraphManager GraphObject(pContexte->getSuperviseur()) ;
	string sRootId = GraphObject.setTree(pDlg->pPatPatho, "") ;
	GraphObject.setRootObject(sRootId) ;

	// Appel du pilote
	NSDataGraph* pGraph = GraphObject.getDataGraph() ;
	//pObectsList la liste d'objects qui correspondent a ces criteres
	NSPersonsAttributesArray ObjectsList ;

  // Elaboration des traits
  string user = "_000000" ;
  if (pContexte->getUtilisateur()!= NULL)
    user = pContexte->getUtilisateurID() ;

  string sTraitType = string("_0OTPL") + string("_0TYPE") ;
  string sTraitOper = string("_0OTPL") + string("_DOPER") ;
  string sTraitCons = string("_0OTPL") + string("_LNUCO") ;
  string sTraitRoot = string("_0OTPL") + string("_0TYPC") ;
  string sTraitComp = string("_0OTPL") + string("_0COMD") ;
  string sTraitDefa = string("_0OTPL") + string("_0DEFA") ;

  NSBasicAttributeArray AttrList ;
  AttrList.push_back(new NSBasicAttribute("user",     user)) ;
  AttrList.push_back(new NSBasicAttribute(sTraitType, pDlg->sTypeDoc)) ;
  AttrList.push_back(new NSBasicAttribute(sTraitOper, pDlg->sOper)) ;
  AttrList.push_back(new NSBasicAttribute(sTraitCons, pDlg->sNoConsole)) ;
  AttrList.push_back(new NSBasicAttribute(sTraitRoot, pDlg->sLexiqueRoot)) ;
  AttrList.push_back(new NSBasicAttribute(sTraitComp, pDlg->sCompo)) ;
  AttrList.push_back(new NSBasicAttribute(sTraitDefa, pDlg->sDefaut)) ;

  pContexte->getSuperviseur()->getPilot()->createPersonOrObject(NautilusPilot::SERV_CREATE_OBJECT.c_str(),
                      pGraph, &ObjectsList, &AttrList, OBJECT_TYPE, false);

  if (NULL == pGraph)
  {
    string sErrorText = pContexte->getSuperviseur()->getText("templateManagement", "templateCreationError") ;
    erreur(sErrorText.c_str(), standardError, 0) ;
    delete pDlg ;
    return ;
  }

  string sErrorText = pContexte->getSuperviseur()->getText("templateManagement", "templateSuccessfullyCreated") ;
  ::MessageBox(0, sErrorText.c_str(), "", MB_OK) ;
	
  delete pDlg ;

  if (pGraph->getTrees()->empty())
		return ;

	NSPatPathoArray PPT(pContexte->getSuperviseur(), graphObject) ;

	NSDataTreeIter iterTree = pGraph->getTrees()->begin() ;
	(*iterTree)->getRawPatPatho(&PPT) ;

	pTemplates->initInfo(&PPT) ;

  AfficheListe() ;
}
catch(TWindow::TXWindow& e)
{
	string sErr = string("Exception NSTemplateWindow::CmNouveau() : ") + e.why() ;
	erreur(sErr.c_str(), standardError, 0) ;
}
catch (...)
{
	erreur("Exception NSTemplateWindow::CmNouveau().", standardError, 0) ;
}
}

void
NSTemplateWindow::CmModifier()
{
try
{
  // Get selected Template
  //
  int iSelectedTemplate = pListeWindow->IndexItemSelect() ;
  if (iSelectedTemplate < 0)
  {
    string sErrorText = pContexte->getSuperviseur()->getText("generalLanguageForLists", "anElementMustBeSelected") ;
    erreur(sErrorText.c_str(), warningError, -1, GetHandle()) ;
    return ;
  }

  NSTemplateInfo* pSelectedTpl = (*(pTemplates->pTplArray))[iSelectedTemplate] ;

  // Execute the editiong dialog box
  //
  NSEditTemplateDialog* pDlg = new NSEditTemplateDialog(pContexte->GetMainWindow(), pContexte, pSelectedTpl) ;
	if (pDlg->Execute() != IDOK)
  {
  	delete pDlg ;
    return ;
	}

  // We must get a Graph with the previous tree inside in order to update it
  //
  string sObjectId = pSelectedTpl->getCode() ;

  NSObjectGraphManager GraphObject(pContexte->getSuperviseur()) ;
  NSDataGraph* pGraph = GraphObject.getDataGraph() ;

  NSBasicAttributeArray AttrArray ;
	AttrArray.push_back(new NSBasicAttribute(OBJECT, sObjectId)) ;
  bool res = pContexte->getSuperviseur()->getPilot()->invokeService(NautilusPilot::SERV_SEARCH_OBJECT_FROM_ID.c_str(), pGraph, &AttrArray) ;
  if (false == res)
  {
		string sErrorText = pContexte->getSuperviseur()->getText("NTIERS", "templateNotFound") + string(" ") + sObjectId ;
    erreur(sErrorText.c_str(), standardError, 0) ;
    delete pDlg ;
    return ;
  }

  if (pGraph->getTrees()->empty())
  {
    delete pDlg ;
		return ;
  }

  // Get the tree to be updated
  //
  NSPatPathoArray PPT(pContexte->getSuperviseur(), graphObject) ;
	NSDataTreeIter iterTree = pGraph->getTrees()->begin() ;
	(*iterTree)->getRawPatPatho(&PPT) ;

  // Update it
  //
  updateTemplateTree(pSelectedTpl, &PPT) ;

	// string sRootId = GraphObject.setTree(pDlg->pPatPatho, "") ;
  GraphObject.getDataGraph()->updateNodesTL(&PPT, sObjectId) ;
  string sRootId = GraphObject.setTree(&PPT, "") ;
	GraphObject.setRootObject(sRootId) ;

	// ObjectsList la liste d'objects qui correspondent a ces criteres
	NSPersonsAttributesArray ObjectsList ;

  // Elaboration des traits
  string user = "_000000" ;
  if (pContexte->getUtilisateur() != NULL)
    user = pContexte->getUtilisateurID() ;

  string sTraitType = string("_0OTPL") + string("_0TYPE") ;
  string sTraitOper = string("_0OTPL") + string("_DOPER") ;
  string sTraitCons = string("_0OTPL") + string("_LNUCO") ;
  string sTraitRoot = string("_0OTPL") + string("_0TYPC") ;
  string sTraitComp = string("_0OTPL") + string("_0COMD") ;
  string sTraitDefa = string("_0OTPL") + string("_0DEFA") ;

  NSBasicAttributeArray AttrList ;
  AttrList.push_back(new NSBasicAttribute("object",   pSelectedTpl->getCode())) ;
  AttrList.push_back(new NSBasicAttribute("operator", user)) ;
  AttrList.push_back(new NSBasicAttribute(sTraitType, pDlg->sTypeDoc)) ;
  AttrList.push_back(new NSBasicAttribute(sTraitOper, pDlg->sOper)) ;
  AttrList.push_back(new NSBasicAttribute(sTraitCons, pDlg->sNoConsole)) ;
  AttrList.push_back(new NSBasicAttribute(sTraitRoot, pDlg->sLexiqueRoot)) ;
  AttrList.push_back(new NSBasicAttribute(sTraitComp, pDlg->sCompo)) ;
  AttrList.push_back(new NSBasicAttribute(sTraitDefa, pDlg->sDefaut)) ;

  res = pContexte->getSuperviseur()->getPilot()->modifyPersonOrObject(NautilusPilot::SERV_MODIFY_OBJECT.c_str(),
                                          pGraph, &ObjectsList, &AttrList) ;
  if (!res)
	{
    string sErrorText = pContexte->getSuperviseur()->getText("templateManagement", "templateModificationError") ;
    erreur(sErrorText.c_str(), standardError, 0) ;
    delete pDlg ;
    return ;
	}

  if (!pGraph)
  {
    string sErrorText = pContexte->getSuperviseur()->getText("templateManagement", "templateModificationError") ;
    erreur(sErrorText.c_str(), standardError, 0) ;
    delete pDlg ;
    return ;
  }

  string sErrorText = pContexte->getSuperviseur()->getText("templateManagement", "templateSuccessfullyModified") ;
  ::MessageBox(0, sErrorText.c_str(), "", MB_OK) ;

  delete pDlg ;

  pTemplates->LoadAllTemplates() ;
  AfficheListe() ;
}
catch(TWindow::TXWindow& e)
{
	string sErr = string("Exception NSTemplateWindow::CmModifier() : ") + e.why() ;
	erreur(sErr.c_str(), standardError, 0) ;
}
catch (...)
{
	erreur("Exception NSTemplateWindow::CmModifier().", standardError, 0) ;
}
}

void
NSTemplateWindow::updateTemplateTree(NSTemplateInfo* pTplInfo, NSPatPathoArray* pPPT)
{
  if ((NULL == pTplInfo) || (NULL == pPPT))
    return ;

  string sObjectId = pTplInfo->getCode() ;
  if (string("") == sObjectId)
    return ;

  // pTemplates->getTemplateTreeFromOids(sObjectId, pPPT) ;

  // Updating the patpatho

  PatPathoIter iter ;

  // Document type
  //
  string sTypePath = string("0OTPL1/0TYPE1/") + pTplInfo->getType() ;
  if (false == pPPT->CheminDansPatpatho(sTypePath))
    pPPT->AjouterChemin("0OTPL1/0TYPE1", pTplInfo->getType(), true) ;

  // Operator
  //
  if ((string("") != pTplInfo->getUser()) && (string("9VOID1") != pTplInfo->getUser()))
  {
    if (pPPT->CheminDansPatpatho("0OTPL1/DOPER1/£SPID1", string(1, cheminSeparationMARK), &iter))
      (*iter)->setComplement(pTplInfo->getUser()) ;
    else
    {
      Message Msg ;
      Msg.SetLexique("£SPID1") ;
      Msg.SetComplement(pTplInfo->getUser()) ;
      pPPT->AjouterChemin("0OTPL1/DOPER1", &Msg) ;
    }
  }
  else
    if (pPPT->CheminDansPatpatho("0OTPL1/DOPER1", string(1, cheminSeparationMARK), &iter))
      pPPT->SupprimerItem(iter) ;

  // Document's root
  //
  if ((string("") != pTplInfo->getRoot()) && (string("9VOID1") != pTplInfo->getRoot()))
  {
    sTypePath = string("0OTPL1/0TYPC1/") + pTplInfo->getRoot() ;
    if (false == pPPT->CheminDansPatpatho(sTypePath))
      pPPT->AjouterChemin("0OTPL1/0TYPC1", pTplInfo->getRoot(), true) ;
  }
  else
    if (pPPT->CheminDansPatpatho("0OTPL1/0TYPC1", string(1, cheminSeparationMARK), &iter))
      pPPT->SupprimerItem(iter) ;

  // Console
  //
  updateTreeCode(pPPT, string("0OTPL1/LNUCO1"), pTplInfo->getConsole()) ;

  // Composition flag
  //
  if (string("WCEA01") == pTplInfo->getCompo())
  {
    if (false == pPPT->CheminDansPatpatho("0OTPL1/0COMD1"))
      pPPT->AjouterChemin("0OTPL1", "0COMD1", true) ;
  }
  else
    if (pPPT->CheminDansPatpatho("0OTPL1/0COMD1", string(1, cheminSeparationMARK), &iter))
      pPPT->SupprimerItem(iter) ;

  // Default flag
  //
  if (string("WCEA01") == pTplInfo->getDefaut())
  {
    if (false == pPPT->CheminDansPatpatho("0OTPL1/0DEFA1"))
      pPPT->AjouterChemin("0OTPL1", "0DEFA1", true) ;
  }
  else
    if (pPPT->CheminDansPatpatho("0OTPL1/0DEFA1", string(1, cheminSeparationMARK), &iter))
      pPPT->SupprimerItem(iter) ;

  // Template file
  //
  updateTreeText(pPPT, string("0OTPL1/0TPL01"), pTplInfo->getFichier()) ;

  // Header file
  //
  updateTreeText(pPPT, string("0OTPL1/0ENTE1"), pTplInfo->getEnTete()) ;

  // Label
  //
  updateTreeText(pPPT, string("0OTPL1/0INTI1"), pTplInfo->getLibelle()) ;
}

void
NSTemplateWindow::updateTreeText(NSPatPathoArray* pPPT, string sPath, string sTxt)
{
  if ((NULL == pPPT) || (string("") == sPath))
    return ;

  PatPathoIter iter ;

  if (string("") == sTxt)
  {
    if (pPPT->CheminDansPatpatho(sPath, string(1, cheminSeparationMARK), &iter))
      pPPT->SupprimerItem(iter) ;
    return ;
  }

  string sLocalPath = sPath + string(1, cheminSeparationMARK) + string("£?????") ;
  if (pPPT->CheminDansPatpatho(sLocalPath, string(1, cheminSeparationMARK), &iter))
    (*iter)->setTexteLibre(sTxt) ;
  else
  {
    Message Msg ;
    Msg.SetLexique("£?????") ;
    Msg.SetTexteLibre(sTxt) ;
    pPPT->AjouterChemin(sPath, &Msg) ;
  }
}

void
NSTemplateWindow::updateTreeCode(NSPatPathoArray* pPPT, string sPath, string sCode)
{
  if ((NULL == pPPT) || (string("") == sPath))
    return ;

  PatPathoIter iter ;

  if (string("") == sCode)
  {
    if (pPPT->CheminDansPatpatho(sPath, string(1, cheminSeparationMARK), &iter))
      pPPT->SupprimerItem(iter) ;
    return ;
  }

  string sLocalPath = sPath + string(1, cheminSeparationMARK) + string("£C;") ;
  if (pPPT->CheminDansPatpatho(sLocalPath, string(1, cheminSeparationMARK), &iter))
    (*iter)->setComplement(sCode) ;
  else
  {
    Message Msg ;
    Msg.SetLexique("£C;020") ;
    Msg.SetComplement(sCode) ;
    pPPT->AjouterChemin(sPath, &Msg) ;
  }
}

void
NSTemplateWindow::CmSupprimer()
{
}

void
NSTemplateWindow::focusFct()
{
}

// fonction permettant de prendre toute la taille de TWindow par la TListWindow
void
NSTemplateWindow::EvSize(uint sizeType, ClassLib::TSize& size)
{
  TWindow::EvSize(sizeType, size) ;
  pListeWindow->MoveWindow(GetClientRect(), true) ;
}

DEFINE_RESPONSE_TABLE1(NSTemplateListWindow, TListWindow)
  EV_WM_LBUTTONDBLCLK,
  EV_WM_RBUTTONDOWN,
  EV_WM_KEYDOWN,
  EV_WM_KILLFOCUS,
  EV_WM_SETFOCUS,
  EV_WM_LBUTTONUP,
END_RESPONSE_TABLE ;

NSTemplateListWindow::NSTemplateListWindow(NSTemplateWindow *parent, int id, int x, int y, int w, int h, TModule *module)
                     :TListWindow((TWindow *) parent, id, x, y, w, h, module)
{
  pView         = parent ;
  iRes          = id ;
  Attr.Style    |= LVS_REPORT | LVS_SHOWSELALWAYS ;
//  Attr.ExStyle  |= WS_EX_NOPARENTNOTIFY ;
}

void
NSTemplateListWindow::SetupWindow()
{
  ListView_SetExtendedListViewStyle(this->HWindow, LVS_EX_FULLROWSELECT) ;
  TListWindow::SetupWindow() ;
}


// -----------------------------------------------------------------------------
// Fonction de réponse au double-click
// -----------------------------------------------------------------------------
void
NSTemplateListWindow::EvLButtonDblClk(uint /* modKeys */, NS_CLASSLIB::TPoint& point)
{
	TLwHitTestInfo info(point) ;

  HitTest(info) ;
/*
  if (info.GetFlags() & LVHT_ONITEM)  	pView->CmModifier() ;
  else
  	pView->CmNouveau() ;
*/
}

void
NSTemplateListWindow::EvRButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point)
{
	TLwHitTestInfo info(point) ;

  int indexItem = HitTest(info) ;
  if (info.GetFlags() & LVHT_ONITEM)
  {  	SetItemState(indexItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED) ;    Invalidate() ;    pView->EvRButtonDown(modkeys, point) ;
  }
  else
  	pView->EvRButtonDownOut(modkeys, point) ;
}

void
NSTemplateListWindow::EvKeyDown(uint key, uint repeatCount, uint flags)
{
/*
	if      (key == VK_DELETE)
  	pView->CmSupprimer() ;
  else if (key == VK_INSERT)
  {
  	if (GetKeyState(VK_SHIFT) < 0)
    	pView->CmModifier() ;
    else
    	pView->CmNouveau() ;
  }
  else
*/
  	TListWindow::EvKeyDown(key, repeatCount, flags) ;
}


void
NSTemplateListWindow::EvLButtonUp(uint /* modKeys */, NS_CLASSLIB::TPoint& /* pt */)
{
	NSContexte  *pContexte  = pView->pContexte ;
  NSSuper     *pSuper     = pContexte->getSuperviseur() ;

  if (pSuper->bDragAndDrop)
  {
  	pSuper->DragDrop->DragLeave( *this ) ;
    pSuper->DragDrop->EndDrag() ;
    delete pSuper->DragDrop ;
    pSuper->DragDrop = 0 ;
    pSuper->bDragAndDrop = false ;
  }
}

// -----------------------------------------------------------------------------
// Retourne l'index du premier item sélectionné
// -----------------------------------------------------------------------------
int
NSTemplateListWindow::IndexItemSelect()
{
	int count = GetItemCount() ;
  int index = -1 ;

	for (int i = 0 ; i < count ; i++)  	if (GetItemState(i, LVIS_SELECTED))
    {
    	index = i ;
      break ;
    }

	return index ;}


void
NSTemplateListWindow::EvSetFocus(HWND /* hWndLostFocus */)
{
	SetBkColor(0x00fff0f0) ; // 0x00bbggrr
  SetTextBkColor(0x00fff0f0) ;
  Invalidate();

  pView->focusFct() ;

  int count = GetItemCount() ;

  for (int i = 0 ; i < count ; i++)
  	if (GetItemState(i, LVIS_SELECTED))
    	return ;

	SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED) ;
}

void
NSTemplateListWindow::EvKillFocus(HWND /* hWndGetFocus */)
{
	SetBkColor(0x00ffffff) ; // 0x00bbggrr
  SetTextBkColor(0x00ffffff) ;
  Invalidate() ;
}

//***********************************************************************//
//							Classe NSEnregProgressDlg
//***********************************************************************//
DEFINE_RESPONSE_TABLE1(NSProgressDlg, NSUtilDialog)
  EV_COMMAND(PROGRESS_STOP_BTN, stopPressed),
END_RESPONSE_TABLE;

NSProgressDlg::NSProgressDlg(TWindow* parent, NSContexte* pCtx,
															TModule* /* module */)
              :NSUtilDialog(parent, pCtx, "PROGRESS_DLG", pNSResModule)
{
  _pProgressBar = new TGauge(this, "%d%%", IDC_PROGRESS_BAR, 18, 43, 201, 21) ;
  _pProgressBar->SetRange(0, 100) ;
  _pProgressBar->SetNativeUse(nuNever) ;
  _pProgressBar->SetColor(NS_CLASSLIB::TColor(0, 0, 128)) ;   // Blue

  _pText = new OWL::TStatic(this, IDC_PROGRESS_TXT) ;

  _pStopButton = new OWL::TButton(this, PROGRESS_STOP_BTN) ;

  _bKeepLoading = true ;

  _sCaptionText = string("") ;
  _sTaskText    = string("") ;
}

NSProgressDlg::~NSProgressDlg(){
	delete _pProgressBar ;
	delete _pText ;
  delete _pStopButton ;
}

voidNSProgressDlg::SetupWindow()
{
	NSUtilDialog::SetupWindow() ;

  resetGauge() ;
  SetCaption(_sCaptionText.c_str()) ;
  _pText->SetText(_sTaskText.c_str()) ;

  string sHalt = pContexte->getSuperviseur()->getText("mailBoxProgress", "halt") ;
  _pStopButton->SetCaption(sHalt.c_str()) ;
}

void
NSProgressDlg::updateTaskText(string sT)
{
  _sTaskText = sT ;
  _pText->SetText(_sTaskText.c_str()) ;
}

void
NSProgressDlg::stopPressed()
{
  _bKeepLoading = false ;
}

// fin de nsoutdlg.cpp ................................
///////////////////////////////////////////////////////

