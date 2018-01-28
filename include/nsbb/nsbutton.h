//-----------------------------------------------------------------
// NSButn dérivé de TButton
// Quand on appuie sur un  bouton, il faut pouvoir :
//   - lancer une fonction
//   - lancer une boîte de dialogue
//   - désélectionner ou sélectionner d'autres boutons 
//-----------------------------------------------------------------
_CLASSDEF(NSButn);
class NSButn : public TButton {
  // Constructeurs
  static PNSButton getButton(PNSWindow pwin, int id);

  // Destructeurs

  // Réponses au messages
  virtual void DefNotificationProc(RTMESSAGE Msg);

  // Transfert
  virtual WORD Transfer(int iTransferIndice, WORD TransferFlag);
};
//-----------------------------------------------------------------
// NSODBtutn dérivé de NSButton et de NSOwnerDrawItem
// Owner-draw bouton
//-----------------------------------------------------------------
_CLASSDEF(NSODButn);
class NSODButn : public NSButn, public NSOwnerDrawItem {

};