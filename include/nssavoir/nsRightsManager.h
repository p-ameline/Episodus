// -----------------------------------------------------------------------------
// NSRightsManager.H
// -----------------------------------------------------------------------------
// Manager des droits des utilisateurs
// -----------------------------------------------------------------------------
// $Revision: 1.4 $
// $Author: pameline $
// $Date: 2015/02/07 14:37:22 $
// -----------------------------------------------------------------------------
// FLP - juin 2004
// CH  - mai 2004
// -----------------------------------------------------------------------------

#ifndef __NSRIGHTSMANAGER_H__
# define __NSRIGHTSMANAGER_H__

#include <owl/groupbox.h>
#include <owl/button.h>

#include "partage\ns_vector.h"

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsglobalLus.h"
#else
  #include "partage/nsglobal.h"
#endif

#include "nssavoir\nsRightsDefs.h"
#include "nssavoir\nsHealthTeam.h"
#include "nsbb\nsutidlg.h"
#include "nsbb\NSTip.h"

#if defined(_DANSLEXIDLL)
	#define _CLASSELEXI __export
#else
	#define _CLASSELEXI __import
#endif

#define MAXRIGHTSZORDER 10

#define ROSACE_TIMER    11

// Case de pétale
//
class NSRightPosition : public NSRoot
{
  public:

    string					sPositionTitle ;

    int						iZOrder ;

    int 					iStartAngle ;
    int 					iDeltaAngle ;
    int 					iRadius ;
    NS_CLASSLIB::TPoint		ptCenter ;

    NSRightPosition(NSContexte *pCtx, DistanceType distance, SpecialityType speciality, int theColor, string sTitle = "") ;
    ~NSRightPosition() ;
    NSRightPosition(const NSRightPosition& rv) ;

    void    		draw(TDC* pDc, bool erase, NS_CLASSLIB::TRect& RectAPeindre, OWL::TBrush** pBrushes, NSRightPosition* pSelectedPos) ;

    bool			isBoxOnPoint(int iPointRadius, int iPointDegreAngle) ;

    void          	setDistance(DistanceType distVar)      	{ dist = distVar ; }
    DistanceType  	getDistance()                           { return dist ; }

    void           	setSpeciality(SpecialityType specVar)	{ spec = specVar ; }
    SpecialityType	getSpeciality()                         { return spec ; }

    //void          	setFonction(FonctionType fonc)          { fonction = fonc ; }
    //FonctionType   	getFonction()                           { return fonction ; }

    //void         	setAccess(AccessType accesVar)          { acces = accesVar ; }
    //AccessType   	getAccess()                             { return acces ; }

    //void         	setQuarter(QuarterType quartVar)        { quarter = quartVar ; }
    //QuarterType   	getQuarter()                            { return quarter ; }

    void         	setColor(int colVar)                    { color = colVar ; }
    int           	getColor()                              { return color ; }

    void			setNextColor() ;

    NSRightPosition& operator=(const NSRightPosition& src);

    bool operator !=(const NSRightPosition& src);
    bool operator ==(const NSRightPosition& src);


  protected:

    DistanceType			dist ;
    SpecialityType			spec ;
    // La couleur donne les droits d'acces: none = 0, red = 1, orange = 2, green = 3
    int           			color ;

    //QuarterType 			quarter ;
    //FonctionType  		fonction ;   // code lexique LFONC1
    //AccessType    		acces ;
} ;

typedef vector<NSRightPosition*>      NSRightVector ;
typedef NSRightVector::iterator 	    NSRightsIter ;
typedef NSRightVector::const_iterator NSRightsConstIter ;

// PETAL
//
class NSPetal : public NSRightVector, public NSRoot
{
	protected:

		int					iAngle ;
		PetalType		iType ;
		PetalLevel	iLevel ;

	public:

    string sPetalTitle ;
    string sReferenceSpeciality ;

    NSPetal(NSContexte *pCtx) : NSRightVector(), NSRoot(pCtx) {}
    NSPetal(NSContexte *pCtx, int iCtAngle, PetalType iCtType, PetalLevel iCtLevel, int iRights[], std::string petal_title) ;
    NSPetal(const NSPetal& rv) ;
    ~NSPetal() ;

    void 		 resetPetal(string sRights) ;
    string	 getRightsString() ;

    void		 vider() ;
    NSPetal& operator=(const NSPetal& src) ;

    void		 prepareRights(NS_CLASSLIB::TPoint* pCenter, int iRadius, int iDeltaAngle) ;
    void     draw(TDC* pDc, bool erase, NS_CLASSLIB::TRect& RectAPeindre, int iZo, OWL::TBrush** pBrushes, NSRightPosition* pSelectedPos) ;

    bool		isBoxOnPoint(NSRightPosition** pClickedRight, int iPointRadius, int iPointDegreAngle, int iZo) ;
    NSRightPosition* getRP(DistanceType	distType, SpecialityType specType) ;

    int			   getAngle() 	{ return iAngle ; }
    void		   setAngle(int iNewAngle) { iAngle = iNewAngle ; }
    int			   getTrigoAngle() ;

    PetalType	 getType()	{ return iType ; }
    void		   setType(PetalType iNewType) { iType = iNewType ; }

    PetalLevel getLevel()	{ return iLevel ; }
    void		   setLevel(PetalLevel iNewLevel) { iLevel = iNewLevel ; }
} ;

bool infPetal(NSPetal *s, NSPetal *b) ;

typedef vector<NSPetal*>		          NSPetalVector ;
typedef NSPetalVector::iterator	      NSPetalsIter ;
typedef NSPetalVector::const_iterator	NSPetalsConstIter ;

// ROSACE
//
class _CLASSELEXI NSRosace : public NSPetalVector, public NSRoot{	public:

    bool 	bNeedPainting ;

    string	sReferenceSpeciality ;
    string 	sReferenceSpecialityLib ;
    string 	sReferenceJob ;
    string 	sReferenceJobLib ;

    // Constructeur - Destructeur
    NSRosace(NSContexte *pCtx, string sRights, NSHealthTeamMandate* pMdt) ; // Old
    NSRosace(NSContexte *pCtx, NSHealthTeamMandate* pMdt) ;	// New
    NSRosace(const NSRosace& rv) ;
    ~NSRosace() ;

    void		vider() ;
    NSRosace&	operator=(const NSRosace& src) ;

    void    	prepare(NS_CLASSLIB::TRect* containerRect) ;
    void    	draw(TDC* pDc, bool erase, NS_CLASSLIB::TRect& RectAPeindre) ;

    string		buildRosaceString() ;
    void   		resetRosace(string sRights) ;
    bool		colorsFromChar(char cPetalColors, int iRights[]) ;

    void  		translateRights(char *sCodRight, NSPetal *pRight) ;
    void   		decodePetal(char sDecodingChar, int iPosPetal, NSPetal *pPetalRight) ;
    void   		decodePetalFamily(char sDecodingChar, NSPetal *pPetalRight) ;
    void  		setProperty(FonctionType fonction, SpecialityType spec, AccessType acces, DistanceType dist, int color, NSRightPosition *pRightPos) ;

    int   		getRightsForUser(DistanceType dist, AccessType acces, QuarterType quarter, string sFonction, string sSpeciality, char *sCodeRights) ;
    char  		*codifRights(NSPetal *pRight) ;
    int   		getRightsPetal(string sUserSpec, string sCreatorSpec, DistanceType dist, char sCodePetal) ;

    int   		getVersion()    	{ return iVersion ; }

    bool		isProfNonMedecin()	{ return bProfNonMedecin ; }

    string		getRightsString() ;
    bool		setRightsString(string sNewRights) ;

    void		calculateParameters(NS_CLASSLIB::TRect* pContainerRect) ;
    /* void		calculateParametersForPoint(NS_CLASSLIB::TPoint* pPoint, int* piRadius, int* iDegreAngle) ; */

    NSRightPosition* getRightFromPoint(NS_CLASSLIB::TPoint* pPoint) ;
    NSRightPosition* getRightFromPoint(int iRadius, int iDegreAngle) ;

    void    EvLButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point, NS_CLASSLIB::TRect* pRosaceRect) ;
    void    EvLButtonUp(uint modKeys, NS_CLASSLIB::TPoint& point, NS_CLASSLIB::TRect* pRosaceRect) ;

    // Renseigne si un document est synchronisable
    // Renvoie vrai si le document est synchronisable
    // Faux sinon
    inline
    bool    IsSynchronisable();

    // Renseigne si un document est une note perso
    // Renvoie vrai si le document est une note perso
    // Faux sinon
    inline
    bool    IsPersonnalNote();

    // Indiquer si le document est une note perso
    inline
    void    setPersonnalNote(bool pn);

    // Indiquer si le document est synchronalisable
    inline
    void		setSynchronalisable(bool s);

 	protected:

    // Variables utilisées uniquement pour les documents
    bool 				_synchro; 				// indique si le document est synchronisable
    bool				_personnal_note; 	// indique si le document est une note personnelle

    bool				bReadOnly ;
    NSHealthTeamMandate* pMandate ;

    int  				iVersion ;

    // l'utilisateur est à gauche ou à droite
    bool 				bProfNonMedecin ;

    // retient si on est dans le cas d'une rosace élargie et dans quelle quartier se trouve la pétale
    int 				iIsExtended ;

    string				sRightsAsString ;
    bool				bValidRightsString ;

    int 			   	iRadius ;
    NS_CLASSLIB::TPoint	ptCenter ;

    NSRightPosition* 	pSelectedRP ;

    bool	createPetals() ;
    bool   	updateRightsString() ;
} ;// Renseigne si un document est synchronisable// Renvoie vrai si le document est synchronisable
// Faux sinoninline bool NSRosace::IsSynchronisable() { return _synchro; }
// Renseigne si un document est une note perso
// Renvoie vrai si le document est une note perso
// Faux sinon
inline bool NSRosace::IsPersonnalNote()  { return _personnal_note; }

// Indiquer si le document est une note perso
inline void NSRosace::setPersonnalNote(bool pn) { _personnal_note = pn; }

// Indiquer si le document est synchronalisable
inline void NSRosace::setSynchronalisable(bool s) {_synchro = s; }
class _CLASSELEXI NSRosaceGroupBox : public OWL::TButton, public NSRoot
{
  public:

    NSRosaceGroupBox(NSContexte *pCtx, TWindow* parent, int resId, NSRosace* pRose)  ;
    ~NSRosaceGroupBox() ;

    void	SetupWindow() ;

    string getRigthsString() ;

    void 	EvPaint() ;
    void 	Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectAPeindre) ;
    void 	EvLButtonDblClk(UINT modKeys, NS_CLASSLIB::TPoint& point) ;
    void  EvLButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point) ;
    void  EvLButtonUp(uint modKeys, NS_CLASSLIB::TPoint& point) ;
    void 	EvMouseMove(uint modKeys, NS_CLASSLIB::TPoint& point) ;
    void 	ButtonDblClk() ;
    //  void EvTimer(uint timerId) ;

    NSRosace* getRosace() { return _pRosace ; }

  protected:

    NSRosace* 		      _pRosace ;
  	NSTitleTip*         _pToolTip ;      // toolTip
  	int                 _time ;
    NSRightPosition*    _mouse_on ;
   	NS_CLASSLIB::TPoint _pos_tip ;

  DECLARE_RESPONSE_TABLE(NSRosaceGroupBox) ;
};

class _CLASSELEXI RightsDialog : public NSUtilDialog
{
  public :

    NSRosace* 			  _pRosace ;
    NSRosaceGroupBox*	_pRightsInterface ;
    string* 	    	  _pRightsString ;

    RightsDialog(TWindow* pere, NSContexte* pCtx, string* pRightsStr) ;    ~RightsDialog() ;

    void CmOk() ;
    void CmCancel() ;
    void SetupWindow() ;

  DECLARE_RESPONSE_TABLE(RightsDialog) ;
};

#endif // __NSRIGHTSMANGER_H__

