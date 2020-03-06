// -----------------------------------------------------------------------------
// nscomboutil.h
// -----------------------------------------------------------------------------
// $Revision: 1.4 $
// $Author: pameline $
// $Date: 2017/04/12 20:17:36 $
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// FLP  - juin 2004
// -----------------------------------------------------------------------------

#ifndef __NS_COMBO_UTIL_H__
# define __NS_COMBO_UTIL_H__

# include <vector>
# include <string>
# include <owl\combobox.h>
# include "nsbb\nsutidlg.h"
// # include "nsbb\nspatpat.h"
# include "nautilus\nssuper.h"
# include "nsbb\NTFunctor.h"
# include "nsepisod\eptables.h"

/*
** NSComboBox
** Classe gerant des codes lexiques et affichant
** automatiquement leurs libélés dans le combo
*/
class _NSBBCLASSE NSComboBox : public OWL::TComboBox, NSRoot
{
  public:

    // Constructeur par vecteur
    // TWindow *parent                : fenetre parent
    // NSContexte *pCtx               : contexte
    // VecteurString *pVecteurString  : vecteur contenant les codes lexiques
    NSComboBox(TWindow *parent, int resId, NSContexte *pCtx, const VecteurString *pVecteurString = (VecteurString*) NULL, bool bAddNoChoice = false) ;
    NSComboBox(TWindow *parent, int Id, NSContexte *pCtx, int x, int y, int w, int h, uint32 style, const VecteurString *pVecteurString = (VecteurString*) NULL, bool bAddNoChoice = false) ;

    // Constructeur par vecteur sémantique VectString (défini dans divfct.h)
    // TWindow *parent                : fenetre parent
    // NSContexte *pCtx               : contexte
    // VecteurString *pVecteurString  : vecteur contenant les codes lexiques
    NSComboBox(TWindow *parent, int resId, NSContexte *pCtx, const VectString *pVectString = (VectString*) NULL, bool bAddNoChoice = false) ;
    NSComboBox(TWindow *parent, int Id, NSContexte *pCtx, int x, int y, int w, int h, uint32 style, const VectString *pVectString = (VectString*) NULL, bool bAddNoChoice = false) ;

    // Constructeur par tableau
    // TWindow *parent                : fenetre parent
    // NSContexte *pCtx               : contexte
    // char *input[]                  : tableau contenant les codes lexiques
    NSComboBox(TWindow *parent, int resId, NSContexte *pCtx, const char *input[], int iInputSize, bool bAddNoChoice = false) ;               // Constructeur
    NSComboBox(TWindow *parent, int Id, NSContexte *pCtx, int x, int y, int w, int h, uint32 style, const char *input[], int iInputSize, bool bAddNoChoice = false) ;
    ~NSComboBox() ;                                                                                       // Destructeur

    void   SetupWindow() ;

    string getSelCode() ;
    void   setCode(const string sValue) ;

    void   EvKeyUp(uint key, uint repeatCount, uint flags) ;
    void   EvKeyDown(uint key, uint repeatCount, uint flags) ;
    uint   EvGetDlgCode(MSG far* msg);

    bool   isVisible() ;
    bool   isEmpty() { return _aLexiqCodes.empty() ; }
    void   AddLexiqueCode(const char *temp);         // Ajoute un code au lexique
    void   AddLexiqueCode(const std::string& temp);  // Ajoute un code au lexique

    void   SetLostFocusResponse(Functor* temp) {_LostFocusResponse = temp ; } // Ajoute une reponse a la perte de focus
    void   EvKillFocus(HWND hWndGetFocus); // Fonction gerant la perte de focus

    // Init or reload
    //
    void initLexiqCodes(const VecteurString *pVecteurString) ;
    void initLexiqCodes(const VectString *pVectString) ;
    void initLexiqCodes(const char *input[], int iInputSize) ;

  protected:

    Functor       *_LostFocusResponse ;
    VecteurString _aLexiqCodes ;  // code contenant les termes lexiques
    string        _sCode ;         //
    bool          _bWinStd ;       //
    TWindow       *_pDlgParent ;   // Parent de la combo
    bool          _bAddNoChoice ;

    void initCommonData() ;


  DECLARE_RESPONSE_TABLE(NSComboBox) ;
} ;

/*
** NSComboBoxClassif
** Classe gerant des codes de classification et affichant
** automatiquement leurs libélés dans le combo
*/
class _NSBBCLASSE NSComboBoxClassif : public OWL::TComboBox, NSRoot
{
  public:

    NSComboBoxClassif(TWindow *parent, int resId, NSContexte *pCtx, string sClassif) ;  // Constructeur
    ~NSComboBoxClassif() ;                                                            // Destructeur

    void   SetupWindow() ;

    string getSelCode() ;
    void   setCode(string sValue) ;

    void   EvKeyUp(uint key, uint repeatCount, uint flags) ;
    void   EvKeyDown(uint key, uint repeatCount, uint flags) ;
    uint   EvGetDlgCode(MSG far* msg) ;

    bool   isVisible() ;
    void   SetLostFocusResponse(Functor* temp) {LostFocusResponse = temp; } // Ajoute une reponse a la perte de focus
    void   EvKillFocus(HWND hWndGetFocus) ; // Fonction gerant la perte de focus

    void   searchList(string sClassif) ;

  protected:

    string                 sClassification ; // Classification en cours
    NSEpiClassifInfoVector aClassifList ;

    Functor                *LostFocusResponse ;
    string                 sCode ;         //
    bool                   bWinStd ;       //
    TWindow                *pDlgParent ;   // Parent de la combo

  DECLARE_RESPONSE_TABLE(NSComboBoxClassif) ;
} ;

template <class typeBB>
class NTComboItem
{
  protected :

    std::string _comboLabel;          // libélé du combo
    typeBB*     _associatedElement;   // Element associé

  public :

    NTComboItem(std::string label, typeBB *associatedElem)
    {
      _comboLabel        = label ;
      _associatedElement = associatedElem ;
    }

    NTComboItem(char* label, typeBB *associatedElem)
    {
      _comboLabel        = std::string(label) ;
      _associatedElement = associatedElem ;
    }

    ~NTComboItem()
    {
      if (_associatedElement)
        delete(_associatedElement) ;
    }

    std::string getLabel()   { return _comboLabel ; }
    typeBB*     getElement() { return _associatedElement ; }
};

/*
**  Classe servant seulement a pouvoir declarer une table de reponse
*/
class EventCombo : public OWL::TComboBox
{
  protected:

		Functor* _LostFocus ;

  public:

		EventCombo(TWindow *parent, int resId) : OWL::TComboBox(parent , resId) { _LostFocus = (Functor*) 0 ; }
    EventCombo(TWindow *parent, int Id, int x, int y, int w, int h, uint32 style, uint textLimit) : OWL::TComboBox(parent, Id, x, y, w, h, style, textLimit) { _LostFocus = (Functor*) 0 ; }
    virtual ~EventCombo() ;

    virtual void SetLostFocus(Functor* func) { _LostFocus = func ; }
    void         EvKillFocus(HWND hWndGetFocus) ;

	DECLARE_RESPONSE_TABLE(EventCombo) ;
};

template <class typeBB>
class NTComboBox :  public EventCombo
{
  std::vector< NTComboItem<typeBB>* > _data ;

  public:

    // Make certain that LVS_NOSORTHEADER is in place, or the automatic sort will screw up operator[] and getSelectedElement functions
    //
    NTComboBox(TWindow *parent, int resId) : EventCombo(parent, resId) { Attr.Style &= ~CBS_SORT ; }
    NTComboBox(TWindow *parent, int Id, int x, int y, int w, int h, uint32 style, uint textLimit) : EventCombo(parent, Id, x, y, w, h, style, textLimit) { Attr.Style &= ~CBS_SORT ; }

    ~NTComboBox()
    {
    	if (_data.empty())
      	return ;

      std::vector< NTComboItem<typeBB>* >::iterator it = _data.begin() ;
      for ( ; _data.end() != it ; )
      {
      	if (*it)
      		delete *it ;
        _data.erase(it) ;
      }
    }

    size_t getSize() { return _data.size() ; }

    void AddElement(NTComboItem<typeBB>* news)
    {
      _data.push_back(news) ;
    }

    void AddElement(std::string& element, typeBB* temp)
    {
      _data.push_back(new NTComboItem<typeBB>(element,temp)) ;
    }

    NTComboItem<typeBB>* getSelectedElement()
    {
      int iSelIndex = GetSelIndex() ;
      if ((iSelIndex < 0) || (iSelIndex >= _data.size()))
        return (NTComboItem<typeBB>*) 0 ;
    	return _data[GetSelIndex()] ;
    }

    NTComboItem<typeBB>* operator[] (size_t index)
    {
      if (index < _data.size())
        return _data[index] ;
      return (NTComboItem<typeBB>*) 0 ;
    }

    void PrintCombo()
    {
      Clear();
      for (size_t i = 0 ; i < _data.size() ; i++)
        AddString(_data[i]->getLabel().c_str()) ;
    }
};

#endif // # define __NS_COMBO_UTIL_H__

