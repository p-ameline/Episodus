#ifndef __NSAGENTTOOLS_H
#define __NSAGENTTOOLS_H

#include <owl/listwind.h>
#include <owl/dialog.h>
#include <owl/edit.h>

// macro pour appeler LvnGetDispInfo
#define NS_LV_DISPINFO_NOTIFY(notifyCode, method)\
  {notifyCode, UINT_MAX, (TAnyDispatcher)::v_LPARAM_Dispatch,\
  (TMyPMF)v_LV_DISPINFO_NOTIFY_Sig(&TMyClass::method)}

#define NS_LVN_GETDISPINFO(method) NS_LV_DISPINFO_NOTIFY(LVN_GETDISPINFO, method)


class NSAgentsListWindow;
//---------------------------------------------------------------
//         classe NSAddFunctions
//---------------------------------------------------------------
class  _CLASSESGBD NSAgentsList : public TDialog, public NSSuperRoot
{
  public :

    NSAgentsListWindow*	pList ;

   	NSAgentsList(TWindow* pere, NSSuper* pSuper) ;
   	~NSAgentsList() ;
    void CmOk() ;
    void CmCancel() ;    void SetupWindow() ;

  DECLARE_RESPONSE_TABLE(NSAgentsList) ;
};
//---------------------------------------------------------------//         classe NSAddFunctions
//---------------------------------------------------------------
class  _CLASSESGBD NSAgentError : public TDialog, public NSSuperRoot
{
  public:

    TStatic             *pAgentName ;
    TStatic             *pAgentError ;
    string              sAgentName ;
    string              sAgentError ;
    NSAgentsListWindow  *pPere ;

   	NSAgentError(string agent, string error, NSAgentsListWindow* pere, NSSuper* pSuper) ;
   	~NSAgentError() ;
    void CmOk() ;
    void CmStart() ;
    void CmCancel() ;    void SetupWindow() ;

  DECLARE_RESPONSE_TABLE(NSAgentError) ;};
//------------------------------------------------------------------------------// Classe NSFunctionsListWindow
//------------------------------------------------------------------------------
class  _CLASSESGBD NSAgentsListWindow : public TListWindow, public NSSuperRoot
{
  public :

    //list of all agents
    //NSPersonsAttributesArray    *pAgentsList ;
    //incorrect agent list
    NSAgentStatusArray *pAgentList ;

    //constructeurs  Destructeur
    NSAgentsListWindow(TWindow* parent, NSSuper* pSuper, TModule* module = 0) ;
    ~NSAgentsListWindow();

    // réponse aux événements
    void    EvSetFocus(HWND hWndLostFocus);
    void    EvRButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point);
    void    LvnGetDispInfo(TLwDispInfoNotify& dispInfo);
    void    EvLButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point);

    // méthodes
    void    SetupWindow() ;
    void    SetupColumns() ;
    int     IndexItemSelect() ;
    void    InitListe();
    void    AfficheListe() ;
    // entrées de menu
    void    CmErrorAgent() ;
    void    CmStartAgent() ;

  private:

    TWindow *pDialog ;

  DECLARE_RESPONSE_TABLE(NSAgentsListWindow) ;
};

#endif
