// -----------------------------------------------------------------------------
// nsHealthTeamInterface.h
// -----------------------------------------------------------------------------
// Interface Visuelle de Gestion l'Equipe de Santé
// -----------------------------------------------------------------------------
// $Revision: 1.5 $
// $Author: pameline $
// $Date: 2013/11/25 16:07:09 $
// -----------------------------------------------------------------------------
// FLP - juin 2004
// -----------------------------------------------------------------------------

#ifndef __NSHEALTHTEAMINTERFACE_H__
# define __NSHEALTHTEAMINTERFACE_H__

# include <owl/dialog.h>
# include <owl/listbox.h>
# include <owl/listwind.h>
# include <owl/edit.h>

# include "nsbb\nsednum.h"
# include "nsbb\nscomboutil.h"

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsglobalLus.h"
#else
  #include "partage/nsglobal.h"
#endif
# include "nautilus\nssuper.h"

# include "nssavoir\nsHealthTeam.h"

# ifndef _DANSLEXIDLL
#  define _CLASSMODE __import
# else
#  define _CLASSMODE __export
# endif

class _CLASSMODE NSMemberMandatePair : public NSRoot
{
  public:

    // constructors/destructor
    NSMemberMandatePair(NSContexte *pCtx)	;
    NSMemberMandatePair(NSContexte *pCtx, NSHealthTeamMember *pMe, NSHealthTeamMandate *pMa) ;
    NSMemberMandatePair(const NSMemberMandatePair& rv) ;
    ~NSMemberMandatePair() { }

    NSMemberMandatePair operator=(const NSMemberMandatePair& src) ;

    string              getName()            { return _sName ; }
    string              getNameFromTraits()  { return _sNameFromTraits ; }
    string              getProfession()      { return _pMandate->getProfession() ; }
    string              getProfessionLabel() { return _pMandate->getProfessionLabel(pContexte) ; }
    string              getSpeciality()      { return _pMandate->getSpeciality() ; }
    string              getSpecialityLabel() { return _pMandate->getSpecialityLabel(pContexte) ; }
    int                 getStatus()          { return _pMandate->getStatus() ; }
    DistanceType        getDistance()        { return _pMandate->getDistance() ; }
    string              getDistanceTxt()     { return _pMandate->getDistanceTxt(pContexte) ; }
    int                 getAngle()           { return _pMandate->getAngle() ; }
    string              getStringAngle()     { return _pMandate->getStringAngle(pContexte) ; }
    string              getBeginDate()       { return _pMandate->getBeginDate() ; }
    string              getEndDate()         { return _pMandate->getEndDate() ; }
    NSHealthTeamMember  *getMember()         { return _pMember ; }
    NSHealthTeamMandate *getMandate()        { return _pMandate ; }

    void                setName(string name)           { _sName = name ; }
    void                setNameFromTraits(string name) { _sNameFromTraits = name ; }

  private:

    // variables
    string              _sName ;           // Member's name
    string              _sNameFromTraits ; // Member's name elabrated from traits
    NSHealthTeamMember  *_pMember ;
    NSHealthTeamMandate *_pMandate ;
} ;

typedef vector<NSMemberMandatePair *>               NSMemberMandatePairVector ;
typedef NSMemberMandatePairVector::iterator         NSMemberMandatePairIter ;
typedef NSMemberMandatePairVector::const_iterator   NSMemberMandatePairConstIter ;
typedef NSMemberMandatePairVector::reverse_iterator NSMemberMandatePairRIter ;

class _CLASSMODE NSMemberMandatePairArray : public NSMemberMandatePairVector
{
  public:

	  NSMemberMandatePairArray() ;
	  NSMemberMandatePairArray(const NSMemberMandatePairArray& src) ;
	  NSMemberMandatePairArray(NSHealthTeamMemberArray *pMembersArray, NSContexte* pctx) ;
	  ~NSMemberMandatePairArray() { vider() ; }

    void initFromHealthTeam(NSHealthTeamMemberArray *pMembersArray, NSContexte* pctx, bool bGetCivility) ;

	  NSMemberMandatePairArray&	operator=(const NSMemberMandatePairArray& src) ;

  protected:

    bool vider() ;
} ;


class NSListMeMaPairsWindow ;

class _CLASSMODE HealthTeamInterface : public NSUtilDialog
{
  friend class HealthTeamMemberInterface;

  public:

    // constructors/destructor
    HealthTeamInterface(TWindow *parent, NSContexte *pCtx) ;
    ~HealthTeamInterface() ;

    // control functions
    void SetupWindow() ;
    void SetupColumns() ;
    void DispInfoListe(TLwDispInfoNotify& dispInfo) ;
    void LVNColumnclick(TLwNotify& lwn) ;
    void sortByColumn(int iColumn) ;
    void displayList() ;

    void openDetailsMember(int rank) ;

    // functions
    void CmNewMember() ;
    void CmOk() ;

  private:

    // variables
    NSTeamManager								*pHTManager ;
    NSMemberMandatePairArray		*pMemberMandatePairs ;
    NSListMeMaPairsWindow				*pLWMemberMandatePairs ;
    OWL::TStatic								*pStaticPatient ;
    bool                  			bNaturallySorted ;
    int                   			iSortedColumn ;

  DECLARE_RESPONSE_TABLE(HealthTeamInterface) ;
} ;


class NSListMeMaPairsWindow : public TListWindow
{
  public:

    // constructors/destructor
    NSListMeMaPairsWindow(HealthTeamInterface *pParentWindow, int iRessourceId, TModule *module = 0) ;
    ~NSListMeMaPairsWindow() ;

    // control functions
    void												SetupWindow() ;
    void        								EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point) ;
    void        								EvRButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point) ;    void        								EvKeyDown(uint key, uint repeatCount, uint flags);    int         								IndexItemSelect() ;
    void        								EvSetFocus(HWND hWndLostFocus) ;

    // function
    void												CmNewMember() { pHTInterface->CmNewMember() ; }
   //	void												CmDelMember() 																		{ pHTInterface->CmDelMember() ; }
   //	void												CmModMember() 																		{ pHTInterface->CmModMember() ; }

  protected:

	  int													indexSel ;
	  HealthTeamInterface					*pHTInterface ;

  DECLARE_RESPONSE_TABLE(NSListMeMaPairsWindow) ;
} ;

// -----------------------------------------------------------------------------
// fonctions de tri sur des objets NSHealthTeamMember
// --
// functions that sort objects NSHealthTeamMember
// -----------------------------------------------------------------------------
bool										membermandatepairsSortByNameSup				(NSMemberMandatePair *pM1, NSMemberMandatePair *pM2) ;
bool										membermandatepairsSortByNameInf				(NSMemberMandatePair *pM1, NSMemberMandatePair *pM2) ;
bool										membermandatepairsSortByStatusSup   	(NSMemberMandatePair *pM1, NSMemberMandatePair *pM2) ;
bool										membermandatepairsSortByStatusInf			(NSMemberMandatePair *pM1, NSMemberMandatePair *pM2) ;
bool										membermandatepairsSortByAngleSup			(NSMemberMandatePair *pM1, NSMemberMandatePair *pM2) ;
bool										membermandatepairsSortByAngleInf			(NSMemberMandatePair *pM1, NSMemberMandatePair *pM2) ;
bool										membermandatepairsSortByDistanceSup		(NSMemberMandatePair *pM1, NSMemberMandatePair *pM2) ;
bool										membermandatepairsSortByDistanceInf		(NSMemberMandatePair *pM1, NSMemberMandatePair *pM2) ;
bool										membermandatepairsSortByProfessionSup	(NSMemberMandatePair *pM1, NSMemberMandatePair *pM2) ;
bool										membermandatepairsSortByProfessionInf	(NSMemberMandatePair *pM1, NSMemberMandatePair *pM2) ;
bool										membermandatepairsSortBySpecialitySup	(NSMemberMandatePair *pM1, NSMemberMandatePair *pM2) ;
bool										membermandatepairsSortBySpecialityInf	(NSMemberMandatePair *pM1, NSMemberMandatePair *pM2) ;
bool										membermandatepairsSortByBeginDateSup	(NSMemberMandatePair *pM1, NSMemberMandatePair *pM2) ;
bool										membermandatepairsSortByBeginDateInf	(NSMemberMandatePair *pM1, NSMemberMandatePair *pM2) ;
bool										membermandatepairsSortByEndDateSup		(NSMemberMandatePair *pM1, NSMemberMandatePair *pM2) ;
bool										membermandatepairsSortByEndDateInf		(NSMemberMandatePair *pM1, NSMemberMandatePair *pM2) ;

/*
class NSListMembersWindow	: public TListWindow
{
 public:
	// constructors/destructor
	NSListMembersWindow(HealthTeamInterface *parent, int ressourceId, TModule *module = 0) ;
	~NSListMembersWindow() ;

	// functions
	void                        SetupWindow() ;
	void												CmNewMember() ;
	void												CmDelMember() ;
	void												CmModMember() ;

  void        								EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point) ;
  void        								EvRButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point) ;  void        								EvKeyDown(uint key, uint repeatCount, uint flags);  int         								IndexItemSelect() ;
  void        								EvSetFocus(HWND hWndLostFocus) ;

 private:	int													indexSel ;
	HealthTeamInterface					*pHTInterface ;

 DECLARE_RESPONSE_TABLE(NSListMembersWindow) ;
} ;
*/

void				_CLASSMODE runHTDialog(NSContexte *pCtx) ;

#endif // __NSHEALTHTEAMINTERFACE_H__