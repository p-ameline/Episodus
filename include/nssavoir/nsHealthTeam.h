// -----------------------------------------------------------------------------
// nsHealthTeam.h
// -----------------------------------------------------------------------------
// Gestion l'Equipe de Sant�
// -----------------------------------------------------------------------------
// $Revision: 1.10 $
// $Author: pameline $
// $Date: 2015/02/07 14:36:59 $
// -----------------------------------------------------------------------------
// FLP - juin 2004
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Copyright Nautilus, 2004
// http://www.nautilus-info.com
// -----------------------------------------------------------------------------
// Ce logiciel est un programme informatique servant � g�rer et traiter les
// informations de sant� d'une personne.
//
// Ce logiciel est r�gi par la licence CeCILL soumise au droit fran�ais et
// respectant les principes de diffusion des logiciels libres. Vous pouvez
// utiliser, modifier et/ou redistribuer ce programme sous les conditions de la
// licence CeCILL telle que diffus�e par le CEA, le CNRS et l'INRIA sur le site
// "http://www.cecill.info".
//
// En contrepartie de l'accessibilit� au code source et des droits de copie, de
// modification et de redistribution accord�s par cette licence, il n'est offert
// aux utilisateurs qu'une garantie limit�e. Pour les m�mes raisons, seule une
// responsabilit� restreinte p�se sur l'auteur du programme, le titulaire des
// droits patrimoniaux et les conc�dants successifs.
//
// A cet �gard  l'attention de l'utilisateur est attir�e sur les risques
// associ�s au chargement, � l'utilisation, � la modification et/ou au
// d�veloppement et � la reproduction du logiciel par l'utilisateur �tant donn�
// sa sp�cificit� de logiciel libre, qui peut le rendre complexe � manipuler et
// qui le r�serve donc � des d�veloppeurs et des professionnels avertis
// poss�dant des connaissances informatiques approfondies. Les utilisateurs sont
// donc invit�s � charger et tester l'ad�quation du logiciel � leurs besoins
// dans des conditions permettant d'assurer la s�curit� de leurs syst�mes et ou
// de leurs donn�es et, plus g�n�ralement, � l'utiliser et l'exploiter dans les
// m�mes conditions de s�curit�.
//
// Le fait que vous puissiez acc�der � cet en-t�te signifie que vous avez pris
// connaissance de la licence CeCILL, et que vous en avez accept� les termes.
// -----------------------------------------------------------------------------
// This software is a computer program whose purpose is to manage and process
// a person's health data.
//
// This software is governed by the CeCILL  license under French law and abiding
// by the rules of distribution of free software. You can use, modify and/ or
// redistribute the software under the terms of the CeCILL license as circulated
// by CEA, CNRS and INRIA at the following URL "http://www.cecill.info".
//
// As a counterpart to the access to the source code and  rights to copy, modify
// and redistribute granted by the license, users are provided only with a
// limited warranty and the software's author, the holder of the economic
// rights, and the successive licensors have only limited liability.
//
// In this respect, the user's attention is drawn to the risks associated with
// loading, using, modifying and/or developing or reproducing the software by
// the user in light of its specific status of free software, that may mean that
// it is complicated to manipulate, and that also therefore means that it is
// reserved for developers and experienced professionals having in-depth
// computer knowledge. Users are therefore encouraged to load and test the
// software's suitability as regards their requirements in conditions enabling
// the security of their systems and/or data to be ensured and, more generally,
// to use and operate it in the same conditions as regards security.
//
// The fact that you are presently reading this means that you have had
// knowledge of the CeCILL license and that you accept its terms.
// -----------------------------------------------------------------------------

#ifndef __HEALTHTEAM_H__
# define __HEALTHTEAM_H__

#ifdef _ENTERPRISE_DLL
  class Person ;
#endif

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsglobalLus.h"
#else
  #include "partage/nsglobal.h"
#endif

# include "nsbb\nspatpat.h"
# include "partage\NTArray.h"
# include "nssavoir\nsvarray.h"
# include "nsldv\nsldvuti.h"
# include "nssavoir\nsRightsDefs.h"
# include "nssavoir\nsTeamDefs.h"

# ifndef _DANSLEXIDLL
#  define _CLASSMODE __import
# else
#  define _CLASSMODE __export
# endif


// -----------------------------------------------------------------------------
// class NSHealthTeamPosition
// -----------------------------------------------------------------------------
// classe qui repr�sente la position d'un membre dans l'Equipe de Sant�.
// --
// class that describes HealthTeam member position.
// -----------------------------------------------------------------------------
class NSHealthTeamPosition
{
  friend class NSHealthTeamMandate ;

  public:

    // constructors/destructor
    NSHealthTeamPosition() 																												{ distance = DTnotDef ; 		angle = 0 ; 				pptNode = NULL ;        sNodeID = string("") ;  }
    NSHealthTeamPosition(DistanceType dist, int ang, PatPathoIter pptIter = NULL)	{ distance = dist ; 				angle = ang ; 			pptNode = pptIter ;     sNodeID = string("") ;  }
    NSHealthTeamPosition(const NSHealthTeamPosition& src) 												{ distance = src.distance ; angle = src.angle ; pptNode = src.pptNode ; sNodeID = src.sNodeID ; }
    ~NSHealthTeamPosition() 																											{ }

    // operators
    NSHealthTeamPosition&       operator=(const NSHealthTeamPosition& src) 				{ distance = src.distance ; angle = src.angle ; pptNode = src.pptNode ; sNodeID = src.sNodeID ; return (*this) ; }

    // functions
    DistanceType								getDistance()																			{ return distance ; }
    int													getAngle()																				{ return angle ; }

    string											getSDistance() ;
    string											getSAngle() ;

  protected:

    void												setDistance(DistanceType dist)										{ distance 	= dist ; }
    bool												setDistance(int iDist) ;
    void												setAngle(int ang)																	{ angle  		= ang ; }

    // patpatho function
    bool												initFromPatho	(										NSPatPathoArray *pPPT, PatPathoIter *pptIter) ;
    bool												createTree		(										NSPatPathoArray *pPPT, int iCol) ;
    bool												modifyTree		(NSContexte *pCtx, 	NSPatPathoArray *pPPT, PatPathoIter *pptIter, int iCol) ;
    bool												modifyDistance(NSContexte *pCtx, 	NSPatPathoArray *pPPT, PatPathoIter *pptIter, int iCol) ;
    bool												modifyAngle		(NSContexte *pCtx, 	NSPatPathoArray *pPPT, PatPathoIter *pptIter, int iCol) ;

 private:

    // variables
    DistanceType								distance ;
    int													angle ;
    PatPathoIter								pptNode ;
    string                      sNodeID ;
} ;

class NSPersonInfo ;

// -----------------------------------------------------------------------------
// class NSHealthTeamMandate
// -----------------------------------------------------------------------------
// classe qui repr�sente un mandat d'un membre de l'Equipe de Sant�.
// --
// class that describes a HealthTeam member Mandate.
// -----------------------------------------------------------------------------
class _CLASSMODE NSHealthTeamMandate
{
  friend class NSHealthTeamMember ;
  friend class HealthTeamMandateInterface ;

  public:

    enum mandateType { user = 1, root = 2 } ;

    // constructrs/destructor
    NSHealthTeamMandate(mandateType typeUsed = user) ;
    NSHealthTeamMandate(DistanceType dist, int iAng, string sProf, string sSpec, string begin, string end, string durVal, string durUnit, mandateType typeUsed) ;
    NSHealthTeamMandate(const NSHealthTeamMandate& src) ;
    ~NSHealthTeamMandate() ;

    // operator
    NSHealthTeamMandate& operator=(const NSHealthTeamMandate& src) ;

    // functions
    mandateType  getType()       { return _typeOfMandate ; }
    string       getProfession() { return _sProfession ; }
    string       getProfessionLabel(NSContexte *pCtx) ;
    string			 getSpeciality() { return _sSpeciality ; }
    string       getSpecialityLabel(NSContexte *pCtx) ;
    DistanceType getDistance()   { return _position.getDistance() ; }
    string       getDistanceTxt(NSContexte *pCtx) ;
    int          getAngle()      { return _position.getAngle() ; }
    string       getStringAngle(NSContexte *pCtx) ;
    string       getBeginDate()  { return _mandateBeginDate ; }
    void         getBeginDate(NVLdVTemps *pTps) ;
    string       getEndDate()    { return _mandateEndDate ; }
    void         getEndDate(NVLdVTemps *pTps) ;

    int          getStatus() ;
    string       getSDistance()  { return _position.getSDistance() ; }

    bool         isValid() ;
    bool         isActive() ;

    void         setAsARecordKeeper() { setProfession(string("DMEDT1")) ; }

  #ifndef _ENTERPRISE_DLL
    void         autoInitMandate(NSPersonInfo *pPersonInfo) ;
  #else
    void         autoInitMandate(Person *pPerson) ;
  #endif

    static long  getNbInstance()  { return lObjectCount ; }
    static void  initNbInstance() { lObjectCount = 0 ; }

  protected:

    // functions
    void setType(mandateType mandType)  { _typeOfMandate    = mandType ; }
    void setProfession(string sProf)    { _sProfession      = sProf ; }
    void setSpeciality(string sSpec)    { _sSpeciality      = sSpec ; }
    void setDistance(DistanceType dist) { _position.setDistance(dist) ; }
    void setAngle(int ang)              { _position.setAngle(ang) ; }
    void setBeginDate(string sDate)     { _mandateBeginDate = sDate ; }
    void setEndDate(string sDate)       { _mandateEndDate   = sDate ; }
    void setDuration(string sDuration, string sUnit) ;

    // patpatho functions
    bool initFromPatho   (                  NSPatPathoArray *pPPT, PatPathoIter *pptIter) ;
    bool createTree      (                  NSPatPathoArray *pPPT, int iCol) ;
    bool modifyTree      (NSContexte *pCtx,	NSPatPathoArray *pPPT, int iCol) ;
    bool modifyProfession(NSContexte *pCtx,	NSPatPathoArray *pPPT) ;
    bool modifyProfession(NSContexte *pCtx,	NSPatPathoArray *pPPT, PatPathoIter *pptIter, int iCol) ;
    bool modifySpeciality(NSContexte *pCtx,	NSPatPathoArray *pPPT) ;
    bool modifySpeciality(NSContexte *pCtx,	NSPatPathoArray *pPPT, PatPathoIter *pptIter, int iCol) ;
    bool modifyBeginDate (NSContexte *pCtx,	NSPatPathoArray *pPPT) ;
    bool modifyBeginDate (NSContexte *pCtx,	NSPatPathoArray *pPPT, PatPathoIter *pptIter, int iCol) ;
    bool modifyEndDate   (NSContexte *pCtx,	NSPatPathoArray *pPPT) ;
    bool modifyEndDate   (NSContexte *pCtx,	NSPatPathoArray *pPPT, PatPathoIter *pptIter, int iCol) ;

  private:

    // variables
    string               _sProfession ;
    string               _sSpeciality ;
    NSHealthTeamPosition _position ;
    string               _mandateBeginDate ; // format AAAAMMJJhhmmss
    string               _mandateEndDate ;   // format AAAAMMJJhhmmss
    mandateType          _typeOfMandate ;
    PatPathoIter         _pptNode ;
    string               _sNodeID ;

    static long lObjectCount ;
} ;

typedef vector<NSHealthTeamMandate *>					NSHTMMandateVector ;
typedef NSHTMMandateVector::iterator					NSHTMMandateIter ;
typedef NSHTMMandateVector::const_iterator		NSHTMMandateConstIter ;
typedef NSHTMMandateVector::reverse_iterator	NSHTMMandateRIter ;
typedef NTArray<NSHealthTeamMandate>          NSHTMMandateArray ;

class NSMemberMandatePairArray ;

// -----------------------------------------------------------------------------
// class NSHealthTeamMember
// -----------------------------------------------------------------------------
// classe qui repr�sente un membre de l'Equipe de Sant�.
// --
// class that describes a HealthTeam member.
// -----------------------------------------------------------------------------
class _CLASSMODE NSHealthTeamMember
{
  friend class NSTeam ;
  friend class NSHealthTeam ;
  friend class NSMoralPerson ;
// friend class NSHealthTeamManager ;
  friend class HealthTeamMemberInterface ;
  friend class NSMemberMandatePairArray ;
  friend class HealthTeamInterface ;

  public:

    enum memberType { person = 1, team = 2} ;

    // constructor/destructor
    NSHealthTeamMember() ;
    // Used to import patient for a doctor which is not yet in the heath team
    NSHealthTeamMember(NSHealthTeamMember& src) ;
    NSHealthTeamMember(string id) ;
    NSHealthTeamMember(string id, memberType mType, NSHTMMandateArray *mandates) ;
    ~NSHealthTeamMember() ;

    // operator
    NSHealthTeamMember&					operator=(const NSHealthTeamMember& src) ;

    // functions
    string											getID()																						{ return sID ; }
    string											getName() 																				{ return "" ; }

    string											getProfession(NSHealthTeamMandate *pM)						{ return pM->getProfession() ; }
    string											getSpeciality(NSHealthTeamMandate *pM)						{ return pM->getSpeciality() ; }
    DistanceType								getDistance(NSHealthTeamMandate *pM)   						{ return pM->getDistance() ; }
    int         								getAngle(NSHealthTeamMandate *pM)	                { return pM->getAngle() ; }
    string											getMandateBeginDate(NSHealthTeamMandate *pM)			{ return pM->getBeginDate() ; }
    string											getMandateEndDate(NSHealthTeamMandate *pM)				{ return pM->getEndDate() ; }
    string											getSDistance(NSHealthTeamMandate *pM) 						{ return pM->getSDistance() ; }
    bool												isMandateValid(NSHealthTeamMandate *pM) 					{ return pM->isValid() ; }
    bool												isMandateActive(NSHealthTeamMandate *pM)					{ return pM->isActive() ; }

    bool												isValid() ;
    bool												isActive() ;
    memberType									getType()																					{ return type ; }
    bool												isPerson()																				{ if (type == person) return true ; return false ; }
    bool												isTeam()																					{ if (type == team) return true ; return false ; }

    void												getActiveMandates(NSHTMMandateArray *pActiveMandates) ;
    void												getHistoryMandates(NSHTMMandateArray *pHistoryMandates) ;
    void 												getTree(NSPatPathoArray *pPPT, int iCol) ;
    void												AddMandate(NSHealthTeamMandate *pM)								{ pMandates->push_back(pM) ; }

  protected:

    // fonction prot�g�e parce qu'elle ne peuvent pas �tre accessible par un utilisateur
    void												setID(string id)																	{ sID = id ; }

    void												setProfession(NSHealthTeamMandate *pM, string sProf)									{ pM->setProfession(sProf) ; }
    void												setSpeciality(NSHealthTeamMandate *pM, string sSpec)									{ pM->setSpeciality(sSpec) ; }
    void                				setDistance(NSHealthTeamMandate *pM, DistanceType distVar)      		 	{ pM->setDistance(distVar) ; }
    void                				setAngle(NSHealthTeamMandate *pM, int iAngle)        									{ pM->setAngle(iAngle) ; }
    void												setMandateBeginDate(NSHealthTeamMandate *pM, string begin)						{ pM->setBeginDate(begin) ; }
    void												setMandateEndDate(NSHealthTeamMandate *pM, string end)		 						{ pM->setEndDate(end) ; }
    void												setMandateDuration(NSHealthTeamMandate *pM, string val, string dUnit)	{ pM->setDuration(val, dUnit) ; }

    void												setType(memberType mType)													{ type = mType ; }
    bool												initFromPatho(									NSPatPathoArray *pPPT, PatPathoIter *pptIter) ;
    bool												createTree(											NSPatPathoArray *pPPT, int iCol) ;
    bool												modifyTree(NSContexte *pCtx,		NSPatPathoArray *pPPT, int iCol) ;

    bool												addMandate(											NSHealthTeamMandate *pM, NSPatPathoArray *pPPT) ;
    bool												modMandate(NSContexte *pCtx, 		NSHealthTeamMandate *pM, NSPatPathoArray *pPPT) ;
    bool												closeMandate(NSContexte *pCtx,	NSHealthTeamMandate *pM, NSPatPathoArray *pPPT) ;
    bool												closeAllMandates(NSContexte *pCtx, NSPatPathoArray *pPPT) ;

  private:

    // variables
    string											sID ;
    NSHTMMandateArray						*pMandates ;
    memberType									type ;
    PatPathoIter								pptNode ;
    string                      sNodeID ;
} ;

typedef vector<NSHealthTeamMember *>								NSHealthTeamMemberVector ;
typedef NSHealthTeamMemberVector::iterator					NSHealthTeamMemberIter ;
typedef NSHealthTeamMemberVector::const_iterator		NSHealthTeamMemberConstIter ;
typedef NSHealthTeamMemberVector::reverse_iterator	NSHealthTeamMemberRIter ;

class _CLASSMODE NSHealthTeamMemberArray : public NSHealthTeamMemberVector
{
// friend class NSHealthTeam ;
  friend class NSTeam ;

  public:

    // constructor/destructor
    NSHealthTeamMemberArray() ;
    NSHealthTeamMemberArray(NSHealthTeamMemberVector& src) ;
    ~NSHealthTeamMemberArray() ;

    // operator
    NSHealthTeamMemberArray& operator=(const NSHealthTeamMemberArray& src) ;

    // functions
    bool vider() ;
} ;

class _CLASSMODE NSTeam
{
  friend class NSHealthTeam ;
  friend class NSMoralPerson ;

  protected:

    enum teamType { healthTeam = 1, moralPerson = 2 } ;

    // constructors/destructor
    NSTeam(teamType aType) ;
    NSTeam(const NSTeam& src) ;
    ~NSTeam() ;

    // operator
    NSTeam&                     operator=(const NSTeam& src) ;

    teamType										getType()					{ return _type ; }
    VecteurString								*getPIDs() ;
    VecteurString								*gtActivePIDs() ;
    NSHealthTeamMemberArray			*getMembers()			{ return &_TeamMembers ; }
    NSHealthTeamMemberArray			*getActiveMembers()	;

    bool                        isMemberP(string personId) ;
    bool                        isActiveMember(string personId) ;
    NSHealthTeamMember					*getFirstMember(string personId) ;

    bool												createTree(NSPatPathoArray *pPPT, int iCol) ;

    void			addMember(NSHealthTeamMember *pMember, NSHealthTeamMember::memberType mType, NSPatPathoArray *pPPT) ;
    void			modMember(NSContexte *pCtx, NSHealthTeamMember *pMember, NSPatPathoArray *pPPT) ;
    void			delMember(NSHealthTeamMember *pMember, NSPatPathoArray *pPPT) ;
    void			closeMember(NSContexte *pCtx, NSHealthTeamMember *pMember, NSPatPathoArray *pPPT) ;

    void			addMandate(NSHealthTeamMember *pMe, NSHealthTeamMandate *pMa, NSPatPathoArray *pPPT) 										{ pMe->addMandate(pMa, pPPT) ; }
    void			modMandate(NSContexte *pCtx, NSHealthTeamMember *pMe, NSHealthTeamMandate *pMa, NSPatPathoArray *pPPT) 	{ pMe->modMandate(pCtx, pMa, pPPT) ; }
    void			closeMandate(NSContexte *pCtx,NSHealthTeamMember *pMe, NSHealthTeamMandate *pMa, NSPatPathoArray *pPPT)	{ pMe->closeMandate(pCtx, pMa, pPPT) ; }
    void			closeAllMandates(NSContexte *pCtx, NSHealthTeamMember *pMe, NSPatPathoArray *pPPT) ;
    void      closeAllMandates(NSContexte *pCtx, string sPersonId, NSPatPathoArray *pPPT) ;

  private:

    NSHealthTeamMemberArray _TeamMembers ;
    teamType								_type ;
} ;

// used in NSTeamListDialog
typedef vector<NSTeam *>								NSTeamVector ;
typedef NSTeamVector::iterator					NSTeamIter ;
typedef NSTeamVector::const_iterator		NSTeamConstIter ;
typedef NSTeamVector::reverse_iterator	NSTeamRIter ;
typedef NTArray<NSTeam>          				NSTeamArray ;

// -----------------------------------------------------------------------------
// objet g�rant une Equipe de Sant�
// object that represents an HealthTeam
// -----------------------------------------------------------------------------
class _CLASSMODE NSHealthTeam
{
  friend class NSHealthTeamManager ;

  public:

    // constructor/destructor
    NSHealthTeam(NSPatPathoArray *pPPT) ;
    NSHealthTeam(NSTeam *team) ;
    NSHealthTeam(const NSHealthTeam& src) ;
    ~NSHealthTeam() ;

    // operator
    NSHealthTeam&               operator=(const NSHealthTeam& src) ;

    // functions
    VecteurString								*getPIDS()																{ return (_Team.getPIDs()) ; }
    NSHealthTeamMemberArray			*getMembers() 														{ return (_Team.getMembers()) ; }
    NSHealthTeamMemberArray			*getActiveMembers()												{ return (_Team.getActiveMembers()) ; }
    NSHealthTeamMember					*getUserAsMember(NSContexte *pCtx) ;

    bool												initFromPatho() ;

    NSPatPathoArray							*getPatho()	{ return &_ppt ; }

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }

  protected:

    NSTeam *										getTeam() ;
    NSHealthTeamMember *        getFirstMember(string personId)						{ return (_Team.getFirstMember(personId)) ; }
    bool                        isMemberP(string personId)                { return (_Team.isMemberP(personId)) ; }

    void			addMember(NSHealthTeamMember *pMember, NSHealthTeamMember::memberType mType = NSHealthTeamMember::person)	{ _Team.addMember(pMember, mType, &_ppt) ; }
    void			modMember(NSContexte *pCtx, NSHealthTeamMember *pMember)		{ _Team.modMember(pCtx, pMember, &_ppt) ; }
    void			closeMember(NSContexte *pCtx, NSHealthTeamMember *pMember)	{ _Team.closeMember(pCtx, pMember, &_ppt) ; }

    void			addMandate(NSHealthTeamMember *pMe, NSHealthTeamMandate *pMa) 										{ _Team.addMandate(pMe, pMa, &_ppt) ; }
    void			modMandate(NSContexte *pCtx, NSHealthTeamMember *pMe, NSHealthTeamMandate *pMa)		{ _Team.modMandate(pCtx, pMe, pMa, &_ppt) ; }
    void			closeMandate(NSContexte *pCtx, NSHealthTeamMember *pMe, NSHealthTeamMandate *pMa)	{ _Team.closeMandate(pCtx, pMe, pMa, &_ppt) ; }
    void			closeAllMandates(NSContexte *pCtx, NSHealthTeamMember *pMe)												{ _Team.closeAllMandates(pCtx, pMe, &_ppt) ; }

  private:

    // variables
    NSTeam					_Team ;
    NSPatPathoArray _ppt ;
    PatPathoIter		_pptNode ;
    string          _sNodeID ;

    static long lObjectCount ;

    // fabTODO
    // pointeur sur le Patient - penser au cas o� le Patient n'est pas SuperAdministrateur (r > 0 ?)
} ;

class _CLASSMODE NSMoralPerson
{
  friend class NSMoralPersonManager ;

  public:

    NSMoralPerson(NSContexte *pCtx) ;
    NSMoralPerson(NSContexte *pCtx, string id, string label, string speciality) ;
    NSMoralPerson(NSContexte *pCtx, NSPatPathoArray *pPPT) ;
    NSMoralPerson(const NSMoralPerson& src) ;
    ~NSMoralPerson() ;

    // functions
    bool												initFromPatho() ;
    NSPatPathoArray *           getPatho()	                                      { return &_ppt ; }

    NSHealthTeamMemberArray			*getMembers() 														        { return (_Team.getMembers()) ; }
    NSHealthTeamMemberArray			*getActiveMembers()												        { return (_Team.getActiveMembers()) ; }
    NSHealthTeamMember					*getUserAsMember(NSContexte *pCtx) ;

    bool												createTree(/*NSPatPathoArray *pPPT, int iCol = 0*/) ;
    bool												setLabel(string label)			{ if (_Team.getType() != NSTeam::moralPerson) return false ; _sLabel = label ; return true ; }
    string											getLabel()									{ if (_Team.getType() == NSTeam::moralPerson) return _sLabel ; return string("") ; }
    string											getSpeciality()							{ if (_Team.getType() == NSTeam::moralPerson) return _sSpeciality ; return string("") ; }
    bool												setSpeciality(string sSpec)	{ if (_Team.getType() != NSTeam::moralPerson) return false ; _sSpeciality = sSpec ; return true ; }
    string											getID()											{ if (_Team.getType() == NSTeam::moralPerson) return _teamId ; return string("") ; }

  protected:

    NSTeam *										getTeam()										                      { return &_Team ; }
    bool                        isMemberP(string personId)                        { return (_Team.isMemberP(personId)) ; }
    bool                        isActiveMember(string personId)                   { return (_Team.isActiveMember(personId)) ; }

    void			addMember(NSHealthTeamMember *pMember, NSHealthTeamMember::memberType mType = NSHealthTeamMember::person)	{ _Team.addMember(pMember, mType, &_ppt) ; }
    void			modMember(NSContexte *pCtx, NSHealthTeamMember *pMember)		{ _Team.modMember(pCtx, pMember, &_ppt) ; }
    void			closeMember(NSContexte *pCtx, NSHealthTeamMember *pMember)	{ _Team.closeMember(pCtx, pMember, &_ppt) ; }

    void			addMandate(NSHealthTeamMember *pMe, NSHealthTeamMandate *pMa) 										{ _Team.addMandate(pMe, pMa, &_ppt) ; }
    void			modMandate(NSContexte *pCtx, NSHealthTeamMember *pMe, NSHealthTeamMandate *pMa)		{ _Team.modMandate(pCtx, pMe, pMa, &_ppt) ; }
    void			closeMandate(NSContexte *pCtx, NSHealthTeamMember *pMe, NSHealthTeamMandate *pMa)	{ _Team.closeMandate(pCtx, pMe, pMa, &_ppt) ; }
    void			closeAllMandates(NSContexte *pCtx, NSHealthTeamMember *pMe)												{ _Team.closeAllMandates(pCtx, pMe, &_ppt) ; }
    void      closeAllMandates(NSContexte *pCtx, string personId)                               { _Team.closeAllMandates(pCtx, personId, &_ppt) ; }

    // ajout Equipe Medicale Locale
    bool      addMember(NSContexte *pCtx, string sPersonId) ;
    bool      delMember(NSContexte *pCtx, string sPersonId) ;

  private:

    // variables
    NSTeam          _Team ;
    string					_teamId ;
    string					_sLabel ;
    string					_sSpeciality ;
    NSPatPathoArray _ppt ;
    PatPathoIter		_pptNode ;
    string          _sNodeID ;
} ;

class RosaceManager ;

// -----------------------------------------------------------------------------
// virtual class NSTeamManager
// -----------------------------------------------------------------------------
// classe virtuelle qui s'occupe de la gestion d'une Equipe
// --
// virtual class that manages a Team.
// -----------------------------------------------------------------------------
class _CLASSMODE NSTeamManager
{
  public:
    // operator
#ifndef _ENTERPRISE_DLL
    virtual NSTeamManager&					operator=(const RosaceManager& src) = 0 ;
#endif

    virtual NSPatPathoArray					*getPatho() = 0 ;

    // team functions
    virtual NSMoralPerson *					getMPTeam() = 0 ;
    virtual NSHealthTeam *					getHTeam() = 0 ;
    virtual bool                    isMemberP(string sPersonId) = 0 ;

    // HTmember functions
    virtual NSHealthTeamMemberArray	*getMembers() = 0 ;
    virtual void										addMember(NSHealthTeamMember *pMember, NSHealthTeamMember::memberType mType = NSHealthTeamMember::person) = 0 ;
    virtual void										modMember(NSContexte *pCtx, NSHealthTeamMember *pMe) = 0 ;

    // HTmandate functions
    virtual void										addMandate(NSHealthTeamMember *pMe, NSHealthTeamMandate *pMa) = 0 ;
    virtual void										modMandate(NSContexte *pCtx, NSHealthTeamMember *pMe, NSHealthTeamMandate *pMa) = 0 ;
    virtual void										closeMandate(NSContexte *pCtx, NSHealthTeamMember *pMe, NSHealthTeamMandate *pMa) = 0 ;
    virtual void										closeAllMandates(NSContexte *pCtx, NSHealthTeamMember *pMe) = 0 ;
} ;

// -----------------------------------------------------------------------------
// class NSHealthTeamManager
// -----------------------------------------------------------------------------
// classe qui s'occupe de la gestion de l'Equipe de Sant�.
// --
// class that manages the Patient's HealthTeam.
// -----------------------------------------------------------------------------
class _CLASSMODE NSHealthTeamManager : public NSTeamManager
{
	// fab TODO - d�clarer en friend la classe de l'interface qui acc�de � la
	// gestion de l'Equipe de Sant�
  friend class HealthTeamInterface ;
  friend class HealthTeamMemberInterface ;
  friend class RosaceDrawer ;
  friend class GraphicHealthTeam ;

  public:

    // constructors/destructor
    NSHealthTeamManager(NSHealthTeam *pHT = NULL) : HTeam(*pHT) {}
    NSHealthTeamManager(const NSHealthTeamManager& src) : HTeam(src.HTeam) {}
    ~NSHealthTeamManager() {}

    // operators
    NSHealthTeamManager&        operator=(const NSHealthTeamManager& src) { HTeam = src.HTeam ; return (*this) ; }
  #ifndef _ENTERPRISE_DLL
    NSHealthTeamManager&			 	operator=(const RosaceManager& src) ;
  #endif

    NSPatPathoArray							*getPatho()																				{ return  HTeam.getPatho() ; }

    // team functions
    NSMoralPerson *							getMPTeam()			                                  { return (NSMoralPerson*) 0 ; }
    NSHealthTeam *							getHTeam()																				{ return &HTeam ; }

    // HTmember functions
    NSHealthTeamMemberArray			*getMembers() 																																		{ return HTeam.getMembers() ; }
    void												addMember(NSHealthTeamMember *pMember, NSHealthTeamMember::memberType mType = NSHealthTeamMember::person) 			{ HTeam.addMember(pMember, mType) ; }
    void												modMember(NSContexte *pCtx, NSHealthTeamMember *pMember)													{	HTeam.modMember(pCtx, pMember) ; }

    // HTmandate functions
    void												addMandate(NSHealthTeamMember *pMe, NSHealthTeamMandate *pMa)											{ HTeam.addMandate(pMe, pMa) ; }
    void												modMandate(NSContexte *pCtx, NSHealthTeamMember *pMe, NSHealthTeamMandate *pMa)		{ HTeam.modMandate(pCtx, pMe, pMa) ; }
    void												closeMandate(NSContexte *pCtx, NSHealthTeamMember *pMe, NSHealthTeamMandate *pMa)	{ HTeam.closeMandate(pCtx, pMe, pMa) ; }
    void												closeAllMandates(NSContexte *pCtx, NSHealthTeamMember *pMe)												{ HTeam.closeAllMandates(pCtx, pMe) ; }

    bool                        isMemberP(string personId)                                                        { return (HTeam.isMemberP(personId)) ; }

  private:

    // variables
	  NSHealthTeam								HTeam ;
} ;

// -----------------------------------------------------------------------------
// class NSMoralPersonManager
// -----------------------------------------------------------------------------
// classe qui s'occupe de la gestion d'une Equipe (personne morale).
// --
// class that manages a Team (moral person).
// -----------------------------------------------------------------------------
class _CLASSMODE NSMoralPersonManager : public NSTeamManager
{
 friend class HealthTeamInterface ;
 friend class HealthTeamMemberInterface ;
 friend class RosaceDrawer ;
 friend class GraphicHealthTeam ;

 public:

	// constructors/destructor
	NSMoralPersonManager(NSMoralPerson *pMP = NULL)																{ pMPTeam = new NSMoralPerson(*pMP) ; }
	NSMoralPersonManager(const NSMoralPersonManager& src)																{ pMPTeam = new NSMoralPerson(*src.pMPTeam) ; }
	~NSMoralPersonManager() 																											{ delete pMPTeam ; }

	// operators
	NSMoralPersonManager&       operator=(const NSMoralPersonManager& src)							{ pMPTeam = new NSMoralPerson(*src.getMPTeam()) ; return (*this) ; }
  NSMoralPersonManager&			 	operator=(const RosaceManager& src) ;

  NSPatPathoArray *           getPatho() 																			  { return (pMPTeam->getPatho()) ; }

	// team functions
  NSMoralPerson *             getMPTeam()																				{ return pMPTeam ; }
  NSHealthTeam *							getHTeam()                                        { return NULL ; }

	NSHealthTeamMemberArray *   getMembers() 																																		  { return (pMPTeam->getMembers()) ; }

  bool                        isMemberP(string personId)                                                        { return (pMPTeam->isMemberP(personId)) ; }
  bool                        isActiveMember(string personId)                                                   { return (pMPTeam->isActiveMember(personId)) ; }

  // adding member/mandate functions
  void                        addMember(NSContexte *pCtx, string personId)                                      { pMPTeam->addMember(pCtx, personId) ; }
	void												addMember(NSHealthTeamMember *pMember, NSHealthTeamMember::memberType mType) 			{ pMPTeam->addMember(pMember, mType) ; }
	void												addMandate(NSHealthTeamMember *pMe, NSHealthTeamMandate *pMa)											{ pMPTeam->addMandate(pMe, pMa) ; }

  // modifying member/mandate functions
	void												modMember(NSContexte *pCtx, NSHealthTeamMember *pMember)													{	pMPTeam->modMember(pCtx, pMember) ; }
	void												modMandate(NSContexte *pCtx, NSHealthTeamMember *pMe, NSHealthTeamMandate *pMa)		{ pMPTeam->modMandate(pCtx, pMe, pMa) ; }

  // closing/deleting member/mandate functions
  void												closeMandate(NSContexte *pCtx, NSHealthTeamMember *pMe, NSHealthTeamMandate *pMa)	{ pMPTeam->closeMandate(pCtx, pMe, pMa) ; }
  void												closeAllMandates(NSContexte *pCtx, NSHealthTeamMember *pMe)												{ pMPTeam->closeAllMandates(pCtx, pMe) ; }
  void                        closeAllMandates(NSContexte *pCtx, string personId)                               { pMPTeam->closeAllMandates(pCtx, personId) ; }

 private:

	// variables
	NSMoralPerson *             pMPTeam ;
} ;

class _CLASSMODE NSTeamSupervisor : public NSRoot
{
 public:

	// constructors/destructor
	NSTeamSupervisor(NSContexte *pContexte) ;

  vector<string> getLocalTeams() ;
} ;

#endif // __HEALTHTEAM_H__

