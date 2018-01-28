//-----------------------------------------------------------------
//   NSFunction dérivée de TObject
//   Permet de définir un objet-fonction qui sera lancer par un
//   contrôle ou une boîte de dialogue. 
//-----------------------------------------------------------------
_CLASSDEF(NSFunction);
class NSFunction : public TObject {
  // Constructeurs
  // Destructeur
  // Fonction à implémenter
};
//-----------------------------------------------------------------
//  NSControl dérivé de TControl
//  Il faut que l'on soit capable de lancer indifférement soit une
//  boîte de dialogue ou une fonction, mais également modifier
//  l'état d'autres contrôles frères. 
//-----------------------------------------------------------------
__CLASSDEF(NSControl);
class NSControl : public TControl {
public :
  // Constructeurs
   NSControl(PTWindowsObject AParent, int AnId, LPSTR ATitle, int X, int
	    Y, int W, int H, PTModule ATModule = NULL)
   : TControl(AParent, AnId, ATitle, X, Y, W, H, ATModule){}
   NSControl(PTWindowsObject AParent, int ResourceId, PTModule ATModule = 
	    NULL)
   : TControl(AParent, ResourceId, ATModule) {}
  // Destructeur

  // Fonctions d'utilité générale
  virtual WORD Transfer(int iTransferIndice, WORD TransferFlag);
  virtual void DefNotificationProc(RTMESSAGE Msg);

  // Permet à NSDialog d'accéder à ses composants protected
  friend NSDialog;
};
