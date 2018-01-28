//---------------------------------------------------------------------------
#ifndef __NSATTVALTOOLS_H
# define __NSATTVALTOOLS_H

# include <vector.h>
# include <string>

# include "nssavoir\nsvarray.h"
# include "balise.h"
# include "nautilus\nssuper.h"
# include "nsbb\nspatpat.h"

# define LABEL_MENU_MENUBAR            "menubar"
# define LABEL_MENU_MENU               "menu"
# define LABEL_MENU_MENUPOPUP          "menupopup"
# define LABEL_MENU_MENUITEM           "menuitem"
# define LABEL_MENU_MENUSEPARATOR      "menuseparator"

# define ATTRIBUT_MENU_LABEL           "label"
# define ATTRIBUT_MENU_ID              "id"
# define ATTRIBUT_MENU_HELPID          "helpid"
# define ATTRIBUT_MENU_ACCEL_KEY       "accelkey"
# define ATTRIBUT_MENU_ACCEL_FLAG      "accelflag"

class _NSBBCLASSE CMenuBar : public Cbalise
{
 public:
  NSContexte        *pContexte ;

  CMenuBar(string attributs, string values, NSContexte *pCtx) ;
  ~CMenuBar() ;

  bool              parser() ;
  bool              verif() ;
  bool              compresser() ;
  bool              traiter() ;
} ;


class _NSBBCLASSE CMenu : public Cbalise
{
 public:
  NSContexte        *pContexte ;
  CMenuBar          *pMenuBar ;

  CMenu(string attributs, string values, NSContexte *pCtx, CMenuBar *pExp) ;
  ~CMenu() ;

  bool              parser() ;
  bool              verif() ;
  bool              compresser() ;
  bool              traiter() ;

  string            getID() ;
  string            getLabel() ;
} ;


class _NSBBCLASSE CMenuPopup : public Cbalise
{
 public:
  NSContexte        *pContexte ;
  CMenuBar          *pMenuBar ;

  CMenuPopup(string attributs, string values, NSContexte *pCtx, CMenuBar *pExp) ;
  ~CMenuPopup() ;

  bool              parser() ;
  bool              verif() ;
  bool              compresser() ;
  bool              traiter() ;
} ;


class _NSBBCLASSE CMenuItem : public Cbalise
{
 public:
  NSContexte        *pContexte ;
  CMenuBar          *pMenuBar ;

  CMenuItem(string attributs, string values, NSContexte *pCtx, CMenuBar *pExp) ;
  ~CMenuItem() ;

  bool   parser() ;
  bool   verif() ;
  bool   compresser() ;
  bool   traiter() ;

  string getID() ;
  string getHelpId() ;
  string getLabel() ;
  string getAccelKey() ;
  string getAccelFlag() ;

  WORD   getAccelKeyAsWord() ;
  BYTE   getAccelFlagAsByte() ;
} ;


class _NSBBCLASSE CMenuSeparator : public Cbalise
{
 public:
  NSContexte        *pContexte ;
  CMenuBar          *pMenuBar ;

  CMenuSeparator(string attributs, string values, NSContexte *pCtx, CMenuBar *pExp) ;
  ~CMenuSeparator() ;

  bool              parser() ;
  bool              verif() ;
  bool              compresser() ;
  bool              traiter() ;
} ;

class _NSBBCLASSE nsMenuParseur
{
 public:
  CMenuBar    *pMenuBar ;
  NSContexte  *pContexte ;
  bool        bParsingOk ;
  bool        bVerbose ;

  nsMenuParseur(NSContexte *pCtx, bool bVerb = true) ;
  ~nsMenuParseur();

  bool getMenu(string sMenuName, string sUserId, string sPersoDirectory) ;
  bool getMenuFromFile(string sFileName) ;
  bool getStringFromFile(string sFileName, string* pData) ;

  bool getMenuFromString(string* psXmlAsString) ;
  bool parse_message(string message, string tag, int *iParsingError) ;
} ;

class _NSBBCLASSE nsMenuIniter
{
 public:
 
  NSContexte  *pContexte ;
  bool        bVerbose ;

  nsMenuIniter(NSContexte *pCtx, bool bVerb = true) ;
  ~nsMenuIniter();

  bool initMenuDescr(OWL::TMenuDescr *pMenuDescr, string sMenuName) ;
  bool initMenu(CMenu *pMenu, OWL::TMenu *pOWLmenu, OWL::TMenuItemInfo* pItemInfo) ;
  bool initMenuPopup(CMenuPopup *pMenuPopup, OWL::TMenu *pOWLmenu) ;
  bool initMenuItem(CMenuItem *pMenuItem, OWL::TMenu *pOWLmenu) ;
  bool initMenuSeparator(CMenuSeparator *pMenuSepar, OWL::TMenu *pOWLmenu) ;

  void createAcceleratorTable() ;
  void destroyAcceleratorTable() ;

  HACCEL getAccelerator() { return hAccelerator ; }

 protected:

  HACCEL hAccelerator ;
  ACCEL  accel_table[20] ;
  size_t iAccelIndex ;
} ;

#endif // __NSATTVALTOOLS_H

