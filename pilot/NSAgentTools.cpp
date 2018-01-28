
//#include "partage\nsglobal.h"

#include <owl/listwind.h>
#include "partage\nsdivfct.h"
#include "pilot\agentList.rh"
#include "ns_sgbd\ns_sgbd.h"
#include "nautilus\nssuper.h"
#include "pilot\NautilusPilot.hpp"
#include "pilot\NSAgentTools.h"
//***********************************************************************//
//							Classe NSAgentsList
//***********************************************************************//
DEFINE_RESPONSE_TABLE1(NSAgentsList, TDialog)
	EV_COMMAND(IDOK,            CmOk),
    EV_COMMAND(IDCANCEL,	CmCancel),
END_RESPONSE_TABLE;

NSAgentsList::NSAgentsList(TWindow* parent, NSSuper* pSuper)
             :TDialog(parent, "IDD_AGENTS_WINDS", pNSResModule),
              NSSuperRoot(pSuper)
{
	pList = new NSAgentsListWindow(this, _pSuper, pNSResModule) ;
}

NSAgentsList::~NSAgentsList()
{
	delete pList ;
}

void
NSAgentsList::SetupWindow()
{
  TDialog::SetupWindow() ;
}

void
NSAgentsList::CmOk()
{
  int index = pList->IndexItemSelect() ;
  if (index != -1)
    pList->CmErrorAgent() ;

  TDialog::CmOk() ;
}

void
NSAgentsList::CmCancel()
{
	Destroy(IDCANCEL) ;
}

// --------------------------------------------------------------------------
//              Classe NSAgentsListWindow
// --------------------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSAgentsListWindow, TListWindow)
    EV_WM_SETFOCUS,
    EV_WM_RBUTTONDOWN,
    EV_WM_LBUTTONDOWN,
    NS_LVN_GETDISPINFO(LvnGetDispInfo),
    EV_COMMAND(IDC_ERROR, CmErrorAgent),
    EV_COMMAND(IDC_START, CmStartAgent),
END_RESPONSE_TABLE;

NSAgentsListWindow::NSAgentsListWindow(TWindow* parent, NSSuper* pSuper,
                           TModule* module)
             :TListWindow(parent, AGENT_LIST, module), NSSuperRoot(pSuper)
{
  //liste des agents
  //pAgentsList = new NSPersonsAttributesArray();
  pDialog = parent ;
  Attr.Style |= LVS_REPORT | LVS_SHOWSELALWAYS ;
  Attr.ExStyle |= WS_EX_NOPARENTNOTIFY ;

  InitListe() ;
}

NSAgentsListWindow::~NSAgentsListWindow()
{
    //delete  pAgentsList;
}

void
NSAgentsListWindow::InitListe()
{
  //appel pilot pour agents
  //pContexte->pPilot->resourceList(NautilusPilot::SERV_GET_AGENT_LIST.c_str(), pAgentsList) ;
  //pErrorAgentList = pContexte->pPilot->pIncorrectAgent;

  pAgentList = &(_pSuper->getPilot()->_AvailableAgentList) ;
}

void
NSAgentsListWindow::SetupWindow()
{
  SetupColumns() ;
  InitListe() ;
  AfficheListe() ;
  ListView_SetExtendedListViewStyle(this->HWindow, LVS_EX_FULLROWSELECT) ;
  TListWindow::SetupWindow() ;
}

void
NSAgentsListWindow::AfficheListe()
{
  DeleteAllItems();

  if (pAgentList->empty())
    return ;

  NSAgentStatusArray ::reverse_iterator itReverse ;
  itReverse = pAgentList->rbegin() ;

  while (itReverse != pAgentList->rend())
  {
    // string sAgentName = (*itReverse)->getAttributeValue("name");
    string sAgentName = (*itReverse)->getAgentName();
    if (string("") != sAgentName)
    {
      TListWindItem Item(sAgentName.c_str() , 0) ;
      InsertItem(Item) ;
    }
    itReverse++ ;
  }
}

void
NSAgentsListWindow::EvSetFocus(HWND hWndLostFocus)
{
}

void
NSAgentsListWindow::EvRButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point)
{
  TListWindow::EvRButtonDown(modkeys,point) ;
	TLwHitTestInfo info(point) ;

  TPopupMenu *menu = new TPopupMenu() ;

  menu->AppendMenu(MF_STRING, IDC_ERROR, "Voir erreur ") ;
  menu->AppendMenu(MF_STRING, IDC_START, "Demarer l'agent ") ;

  ClientToScreen(point) ;
  menu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, point, 0, HWindow) ;
  delete menu ;
}

void
NSAgentsListWindow::LvnGetDispInfo(TLwDispInfoNotify& dispInfo)
{
  TListWindItem& dispInfoItem = *(TListWindItem*)&dispInfo.item ;
  int index = dispInfoItem.GetIndex() ;

  switch (dispInfoItem.GetSubItem())
  {
    case 1:
      int i = 0 ;
      NSAgentStatusIter iterAgent= pAgentList->begin() ;
      for (iterAgent; ((iterAgent != pAgentList->end()) && (i < index)); iterAgent++)
        i++ ;
      if ((iterAgent != pAgentList->end()) && (i == index))
      {
        string sAgentName = (*iterAgent)->getAgentName() ;
        string sError = (*iterAgent)->getError() ;
        string sStatus = "operationel" ;
        if (sError != "")
          sStatus = "non operationnel" ;
        dispInfoItem.SetText(sStatus.c_str(), strlen(sStatus.c_str())) ;
      }
      break ;
  }
}

void
NSAgentsListWindow::CmErrorAgent()
{
  int index = IndexItemSelect();
  if (index == -1)
  {
    string sErrorText = _pSuper->getText("NTIERS", "NotSelectedAgent") ;
    erreur("sErrorText", standardError, 0) ;
    return ;
  }
  int i = 0 ;
  if ((NULL == pAgentList) || (pAgentList->empty()))
    return ;

  NSAgentStatusIter iterAgent= pAgentList->begin() ;
  for (iterAgent; ((iterAgent != pAgentList->end()) && (i<index)); iterAgent++)
    i++ ;
  if ((iterAgent != pAgentList->end()) && (i == index))
  {
    //  string sAgentName = (*iterAgent)->getAttributeValue("name");
    // string sError = pErrorAgentList->getAgentError(sAgentName);
    string sAgentName = (*iterAgent)->getAgentName() ;
    string sError = (*iterAgent)->getError() ;
    NSAgentError *errDlg = new NSAgentError(sAgentName, sError, this, _pSuper) ;
    errDlg->Execute() ;
    delete errDlg  ;
  }
}

void
NSAgentsListWindow::CmStartAgent()
{
  int index = IndexItemSelect() ;
  if (index == -1)
  {
    //erreur("Vous devez sélectionner un agent à demarer.",standardError,0);
    string sErrorText = _pSuper->getText("NTIERS", "NotSelectedAgent") ;
    erreur(sErrorText.c_str(), standardError, 0) ;
    return ;
  }
  
  int i = 0 ;
  NSAgentStatusIter iterAgent= pAgentList->begin() ;
  for(iterAgent; ((iterAgent != pAgentList->end()) && (i < index)); iterAgent++)
    i++ ;
  if ((pAgentList->end() != iterAgent) && (i == index))
  {
    //string sAgentName = (*iterAgent)->getAttributeValue("name");
    string sAgentName = (*iterAgent)->getAgentName() ;
    if (_pSuper->getPilot()->startAgent(sAgentName))
      (*iterAgent)->setActive() ;
    else
    {
      string sErrorText = _pSuper->getText("NTIERS", "AgentStartOff") ;
      erreur(sErrorText.c_str(), standardError, 0) ;
    }
  }
  AfficheListe() ;
}

void
NSAgentsListWindow::EvLButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point)
{
  TLwHitTestInfo info(point) ;
  int index = HitTest(info) ;
  if (index == -1)
    return ;
  TListWindow::EvLButtonDown(modkeys, point) ;
  SetItemState(index, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED) ;
}


void
NSAgentsListWindow::SetupColumns()
{
  NS_CLASSLIB::TRect rectList ;

  InsertColumn(0, TListWindColumn("Agent", 150)) ;
  InsertColumn(1, TListWindColumn("Status", 300)) ;
}

int
NSAgentsListWindow::IndexItemSelect()
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


//***********************************************************************//
//							Classe NSAgentError
//***********************************************************************//
DEFINE_RESPONSE_TABLE1(NSAgentError, TDialog)
  EV_COMMAND(IDC_START, CmStart),
	EV_COMMAND(IDOK,      CmOk),
  EV_COMMAND(IDCANCEL,  CmCancel),
END_RESPONSE_TABLE;

NSAgentError::NSAgentError(string agent, string error, NSAgentsListWindow* parent, NSSuper* pSuper)
        	   :TDialog((TWindow*)parent, "IDD_AGENTSTATUS", pNSResModule),
		          NSSuperRoot(pSuper)
{
	pAgentName  = new TStatic(this, IDC_AGENTNAME, 0, pNSResModule) ;
  pAgentError = new TStatic(this, IDC_AGENTSTATUS, 0, pNSResModule) ;
  sAgentName  = agent ;
  sAgentError = error ;
  pPere       = parent ;
}

NSAgentError::~NSAgentError()
{
	delete pAgentName ;
  delete pAgentError ;
}

void
NSAgentError::SetupWindow()
{
  TDialog::SetupWindow();
  pAgentName->SetText(sAgentName.c_str()) ;
  pAgentError->SetText(sAgentError.c_str()) ;
}

void
NSAgentError::CmStart()
{
  if ((!pPere->pAgentList) || (pPere->pAgentList->empty()))
    return ;

  NSAgentStatusIter iterAgent = pPere->pAgentList->begin() ;
  for (iterAgent; iterAgent != pPere->pAgentList->end(); iterAgent++)
  {
    //string agent =  (*iterAgent)->getAttributeValue("name");
    string agent = (*iterAgent)->getAgentName();
    if (agent == sAgentName)
    {
      if (_pSuper->getPilot()->startAgent( sAgentName))
      {
        pPere->pAgentList->selectAgent(sAgentName)->setActive();
        pAgentError->SetText("operationel");
      }
      else
      {
        string sErrorText = _pSuper->getText("NTIERS", "AgentStartOff") ;
        erreur(sErrorText.c_str(),standardError,0);
        pAgentError->SetText(pPere->pAgentList->getAgentError(sAgentName).c_str());
      }
      return;
    }
  }
}

void
NSAgentError::CmOk()
{   
  pPere->AfficheListe() ;
  TDialog::CmOk() ;
}

void
NSAgentError::CmCancel()
{
	Destroy(IDCANCEL) ;
}


