#ifndef __NSRICHEDIT_H#define __NSRICHEDIT_H

#include <owl/richedit.h>

class NSRichEdit ;
class NSHtml ;

#include "nsbb\nsexport.h"

#define MAXCARS 100000      // Limite du texte pour RichEdit

//
// class NSCharFormat2
// ~~~~~ ~~~~~~~~~~~~~
// TCharFormat encapsulates the CHARFORMAT2 structure which contains
// information about character formatting in a rich edit control.
//
class _NSBBCLASSE NSCharFormat2 : public CHARFORMAT2
{
  public:

    NSCharFormat2(ulong mask = 0);
    NSCharFormat2(const NSRichEdit&, bool selection = true,
                ulong mask = CFM_ALL);

    void    ToggleEffectsBit(ulong flag);
    void    ToggleMaskBit(ulong flag);

    void    EnableBold(bool = true);
    void    EnableItalic(bool = true);
    void    EnableUnderline(bool = true);
    void    EnableStrikeOut(bool = true);
    void    EnableProtected(bool = true);

    void    SetHeight(long);
    void    SetOffset(long);
    void    SetTextColor(const NS_CLASSLIB::TColor& = NS_CLASSLIB::TColor::None);
    void    SetCharSet(uint8);
    void    SetPitchAndFamily(uint8);
    void    SetFaceName(const char far*);

    NS_CLASSLIB::TColor  GetTextColor() const;

    void    SetFontInfo(LOGFONT& lf) const;
    void    GetFontInfo(const LOGFONT& lf);
};

//
// class NSParaFormat2
// ~~~~~ ~~~~~~~~~~~
// TParaFormat encapsulates the PARAFORMAT2 structure which contains
// information about paragraph formatting attributes of a rich edit control.
//
class _NSBBCLASSE NSParaFormat2 : public PARAFORMAT2
{
  public:

    NSParaFormat2(ulong mask = 0);
    NSParaFormat2(const NSRichEdit&, ulong mask = CFM_ALL) ;

    void  ToggleMaskBit(ulong flag);

    void  SetNumbering(uint16);
    void  SetStartIndent(long, bool relative = false);
    void  SetRightIndent(long);
    void  SetOffset(long);
    void  SetAlignment(uint16);
    void  SetTabCount(short, long*);
};

# define DIVTAB 568

typedef enum {N = 0, B, I, U, BI, BU, IU, BIU} TextStyle ;

class PoliceParam
{
  public:

    PoliceParam() ;
    PoliceParam(const PoliceParam& rv) ;
    ~PoliceParam() ;

    // operateurs
    PoliceParam& operator=(const PoliceParam& src) ;
    int          operator==(const PoliceParam& o) ;

    int    getSize() const          { return _iSize ; }
    void   setSize(const int iSize) { _iSize = iSize ; }

    string getFace() const             { return _sFace ; }
    void   setFace(const string sFace) { _sFace = sFace ; }

  protected:

    int     _iSize ;
    string  _sFace ;
} ;

/*

// Use it to make sure the proper dll is loaded (RICHED20.DLL and not RICHED32.DLL)
//
// class TRichEditModule
// ~~~~~ ~~~~~~~~~~~~~~~
// Wrapper object which loads the DLL providing the implementation of
// the RichEdit control
//
class NSRichEditModule : public TModule {
  public:
    NSRichEditModule() ;
};

//
// class TRichEditDll
// ~~~~~~~~~~~~~~~~~~
// TRichEditDll is a simple object which takes advantages of OWL's
// TDllLoader to ensure that only one copy of an object [theoretically
// representing a DLL] is created.
// TRichEditDll provides the 'IsAvailable' method which returns true if the
// DLL is available and loaded. TRichEditDll is used internally by OWL's
// TRichEdit class to ensure that the DLL is available and loaded. You may
// consider using TRichEditDll if your application creates rich edit controls
// without instantiating a TRichEdit object. (For example, if the control
// is part of a dialog resource). The following snippet illustrates:
//
//    if (!TRichEditDll::IsAvailable()) {
//      Error("Unable to load Rich Edit DLL");
//    }
//
typedef TDllLoader<NSRichEditModule> NSRichEditDll;

#if defined(BI_IMPEXP_TEMPLATES)
# if defined(_OWLDLL) || defined(BI_APP_DLL)
  //
  // Export template of TDllLoader<TRichEditModule> when building ObjectWindows
  // DLL and provide import declaration of DLL instance for users of the class.
  //
  template class _OWLCLASS TDllLoader<NSRichEditModule>;
# endif
#endif

*/

class _NSBBCLASSE NSRichEdit : public OWL::TRichEdit // public OWL::TEditFile
{
  public:

    NSRichEdit(TWindow*        parent,
               int             id,
               const char far* text,
               int x, int y, int w, int h,
               const char far* fileName = 0,
               TModule*        module = 0) ;
    NSRichEdit(TWindow*   parent,
               int        resourceId,
               TModule*   module = 0) ;

    void    ReplaceText(const size_t iStart, const size_t iLen, const string sNewText) ;

    void    ConvertToHtml(NSHtml *pTexte, NSContexte *pContexte) ;
    void    ReplaceTag(char far *text, const string &sTag, const string &sValue) ;
    NSHtml* CreerNewPara(NSHtml *pere, NSParaFormat2 far pf) ;
    NSHtml* CreerPara(NSHtml *pere, int indent, int align) ;
    void    CreerFils(NSHtml *pere, const PoliceParam pp, TextStyle style, const string &sText) ;

    static TextStyle TypeStyle(const NSCharFormat2 far cf) ;
    static void      TypePolice(const NSCharFormat2 far cf, PoliceParam& pp) ;

    ulong GetParaFormat(NSParaFormat2 far& pf) const ;
    bool  SetParaFormat(const NSParaFormat2 far& pf) ;

    ulong GetCharFormat(NSCharFormat2 far&, bool selection = false) const;
    bool  SetCharFormat(const NSCharFormat2 far&, uint flags= SCF_SELECTION);

    NS_CLASSLIB::TColor  SetBkgndColor(const NS_CLASSLIB::TColor& = NS_CLASSLIB::TColor::None);

    // Enumeration of flags returned when querrying a RichEdit about
    // a particular character attribute for a chunk of selected text.
    //
    enum TFmtStatus {
      No,               // The whole selection has the attribute
      Yes,              // The attribute is absent from the selection
      Partly            // Part of the selection has the attribute
    };

    // More specialized character formatting or querrying methods
    //
    uint    HasCharAttribute(ulong mask, ulong effects);
    bool    ToggleCharAttribute(ulong mask, ulong effects);
    bool    ChangeCharPointSize(int pointSizeDelta);

    // Selection and Hit Testing
    //
    bool    HasSelection() const;
    void    GetSelection(uint& startPos, uint& endPos) const;
    void    GetSelRange(OWL::TCharRange far&) const;
    bool    SetSelection(uint startPos, uint endPos);
    int     SetSelRange(const OWL::TCharRange far&);

    void    HideSelection(bool hide, bool changeStyle);
    ulong   GetSelectionType() const;

    // Text Operations
    //
    int     GetTextRange(OWL::TTextRange far&) const;
    int     GetTextRange(const OWL::TCharRange far&, char far* buffer) const;
    virtual void GetSubText(char far* str, uint startPos, uint endPos) const;
    int     GetSelectedText(char far* buffer) const;

    void    LimitText(long max);
    int     FindText(uint flags, const OWL::TFindText far&);
    int     FindText(uint flags, const OWL::TCharRange far&, const char far* text);
    int     Search(uint startPos, const char far* text,
                   bool caseSensitive=false, bool wholeWord=false,
                   bool up=false);

    // Word and Line Breaks
    //
    int     FindWordBreak(uint code, long start);
    int     GetLineFromPos(uint charPos) const;       // EM_EXLINEFROMCHAR

    // Lines and Scrolling
    //

    // Editing Operations
    //
    bool    CanPaste(uint format) const;
    void    Paste();  // {HandleMessage(WM_PASTE);}
    void    PasteSpecial(uint format);

    // Streams
    //
    ulong   StreamIn(uint format, OWL::TEditStream far&);
    ulong   StreamOut(uint format, OWL::TEditStream far&);

    // Printing
    //
    bool    DisplayBand(NS_CLASSLIB::TRect far&);
    int     FormatRange(const OWL::TFormatRange far& range, bool render = true);
    int     FormatRange();
    bool    SetTargetDevice(HDC, int lineWidth);

    // Bottomless Rich Edit Controls
    //
    void    RequestResize();

    // OLE Interfaces
    //
    bool    GetOleInterface(IRichEditOle far* &) const;
    bool    SetOleInterface(IRichEditOleCallback far*);

    // Querry/Set control's data format
    //
    uint    GetFormat() const;
    void    SetFormat(uint fmt);
    bool    IsRTF() const;

    // Misc
    //
    ulong   GetEventMask() const;
    ulong   SetEventMask(ulong msk);

    // Override to use RichEdit streaming capabilities
    //
    uint    Transfer(void* buffer, TTransferDirection direction);

    // Override TEditFile's I/O
    //
    bool    Read(const char far* fileName=0);
    bool    Write(const char far* fileName=0);

    bool    ReadFromStream(istream&, uint format = SF_RTF);
    bool    WriteToStream(ostream&, uint format = SF_RTF);

  protected:

    // Data format of control
    //
    uint    Format;

    // Command response functions
    //
    void    CmEditCut();       // CM_EDITCUT
    void    CmEditCopy();      // CM_EDITCOPY
    void    CmEditPaste();     // CM_EDITPASTE
    void    CmEditDelete();    // CM_EDITDELETE
    void    CmEditClear();     // CM_EDITCLEAR
    void    CmEditUndo();      // CM_EDITUNDO

    // Command enabler functions
    //
    void    CeHasSelect(TCommandEnabler& commandHandler);
    void    CeEditPaste(TCommandEnabler& commandHandler);
    void    CeEditClear(TCommandEnabler& commandHandler);
    void    CeEditUndo(TCommandEnabler& commandHandler);

    // Child id notification handled at the child
    //
    void    ENErrSpace();  // EN_ERRSPACE

    // Override TWindow virtual member functions
    //
    char far* GetClassName();
    void      SetupWindow();

    // Event handlers to override TEdit's handlers
    //
    void         EvChar(uint key, uint repeatCount, uint flags);
    void         EvKeyDown(uint key, uint repeatCount, uint flags);
    uint         EvGetDlgCode(MSG far*);
    void         EvSetFocus(HWND hWndLostFocus);
    void         EvKillFocus(HWND hWndGetFocus);
    void         EvChildInvalid(HWND);

  private:

    // Hidden to prevent accidental copying or assignment
    //
    NSRichEdit(const NSRichEdit&);
    NSRichEdit& operator =(const NSRichEdit&);

    // The following are TEdit methods which encapsulates messages/API
    // which are *NOT* supported by the RICHEDIT class.
    //
    void    FormatLines(bool addEOL);
    char far* LockBuffer(uint newSize = 0);
    void    UnlockBuffer(const char far*, bool = false);
    void    SetTabStops(int numTabs, const int far*);
    HLOCAL  GetHandle() const;
    void    SetHandle(HLOCAL localMem);
    uint    GetPasswordChar() const;
    void    SetPasswordChar(uint ch);
    void    SetRectNP(const NS_CLASSLIB::TRect& frmtRect);

    // Validators are not applicable to RichEdit where the text
    // is not plain ASCII...
    //
    bool    IsValid(bool reportErr = false);
    void    SetValidator(TValidator* validator);
    void    ValidatorError();

  DECLARE_RESPONSE_TABLE(NSRichEdit);
};

class _NSBBCLASSE ConvertRTFDialog : public TDialog
{
  public:

    NSRichEdit *_pRichEdit ;
    string      _sFileName ;

    ConvertRTFDialog(TWindow *pere, string sFileName) ;
    ~ConvertRTFDialog() ;

    void SetupWindow() ;

    NSRichEdit* getRichEdit() { return _pRichEdit ; }
    string      getFileName() { return _sFileName ; }
} ;

//----------------------------------------------------------------------------
// Inline implementation
//

//
// Handler of the CM_EDITCUT command - Invokes the 'Cut' method.
//
inline void NSRichEdit::CmEditCut()
{
  Cut();
}

//
// Handler of the CM_EDITCOPY command - invokes the'Copy' method
//
inline void NSRichEdit::CmEditCopy()
{
  Copy();
}

//
// Handler of the CM_EDITPASTE command - invokes the 'Paste' method
//
inline void NSRichEdit::CmEditPaste()
{
  Paste();
}

//
// Handler of the CM_EDITDELETE command - invokes the 'DeleteSelection'
// method
//
inline void NSRichEdit::CmEditDelete()
{
  DeleteSelection();
}

//
// Handler of the CM_EDITCLEAR command - invokes the 'Clear' method
//
inline void NSRichEdit::CmEditClear()
{
  Clear();
}

//
// Handler of the CM_EDITUNDO command - invokes the 'Undo' method
//
inline void NSRichEdit::CmEditUndo()
{
  Undo();
}

//
//
//
inline uint    
NSRichEdit::GetFormat() const {
  return Format;
}

//
//
//
inline void    
NSRichEdit::SetFormat(uint fmt) {
  Format = fmt;
}

//
//
//
inline bool
NSRichEdit::IsRTF() const {
  return Format == SF_RTF;
}

//
// Constructor of a 'TCharFormat' structure initialized with the specified
// mask. NOTE: This constructor is typically used for constructing an
// object which will be filled with the appropriate character format
// information by a rich edit control. The 'mask' specifies which field
// should be filled.
//
inline NSCharFormat2::NSCharFormat2(ulong mask)
{
  cbSize = sizeof(CHARFORMAT);
  dwMask = mask;
}

//
// Toggle the effect bits specified in the 'flag' parameter
//
inline void NSCharFormat2::ToggleEffectsBit(ulong flag)
{
  dwEffects ^= flag;
}

//
// Toggle the mask bits specified in the 'flag' parameter
//
inline void NSCharFormat2::ToggleMaskBit(ulong flag)
{
  dwMask ^= flag;
}

#endif

