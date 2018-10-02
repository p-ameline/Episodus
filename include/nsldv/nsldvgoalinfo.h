#ifndef  __NSLDVGOALINFO_H__
# define __NSLDVGOALINFO_H__

# include <vector.h>
# include <string.h>

# include "nsldv\nsldvuti.h"
# include "nsldv/nsldvexport.h"

// -----------------------------------------------------------------------------
// Jalon d'objectif
// -----------------------------------------------------------------------------

class	_NSLDVCLASSE NSLdvGoalInfo
{
  protected:

    static long lObjectCount ;

  public:

    string          sGoalReference ;

    enum            TYPESJALONS { jalonNotype = 0, jalonDebut, jalonFin } ; // (dur�e ?)
    TYPESJALONS     iTypeJalon ;

    enum            TYPESJALONEVENT { jalonNoEvent = 0, jalonOuverture, jalonOuvreCycle, jalonCycle, jalonIntermediaire, jalonFermeture, jalonNow } ; // (dur�e ?)
    TYPESJALONEVENT iTypeJalonEvent ;

    NVLdVTemps      tpsInfo ;
    NVLdVTemps      tpsClosed ;
    string          sEventNode ;

    enum            JALONSLEVELS { None = 0, AVrouge = 1, AVjaune, AVvert, Bleu, APvert, APjaune, AProuge } ;
    JALONSLEVELS    iTimeLevel ;
    JALONSLEVELS    iValueLevel ;
    JALONSLEVELS    iLevel ;        // Synthesis

    string	        sCode ;
    double          dValue ;
    string	        sValue ;
    string	        sUnit ;
    string	        sFormat ;

    NSLdvGoalInfo() ;
    NSLdvGoalInfo(const NSLdvGoalInfo& rv) ;
    ~NSLdvGoalInfo() ;

    NSLdvGoalInfo& 	operator=(const NSLdvGoalInfo& src) ;

    string          getCode()   { return sCode ; }
    string          getValue()	{ return sValue ; }
    string          getUnit()   { return sUnit ; }
    string          getFormat() { return sFormat ; }

    JALONSLEVELS    getPreviousColor(JALONSLEVELS iColorLevel) ;
    JALONSLEVELS    getNextColor(JALONSLEVELS iColorLevel) ;

    void            computeLevel() ;

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }
} ;

bool infGoalInfo(NSLdvGoalInfo* s, NSLdvGoalInfo* b);

typedef	vector<NSLdvGoalInfo *>		        GoalInfoVector ;
typedef	GoalInfoVector::iterator	        GoalInfoIter ;
typedef	GoalInfoVector::const_iterator	  GoalInfoConstIter ;
typedef	GoalInfoVector::reverse_iterator  GoalInfoRIter ;

class _NSLDVCLASSE GoalInfoArray : public GoalInfoVector
{
  public:

    GoalInfoArray() ;
    GoalInfoArray(const GoalInfoArray& rv) ;
    ~GoalInfoArray() ;

    GoalInfoArray&  operator=(const GoalInfoArray& src) ;

    void		        vider() ;

    NSLdvGoalInfo   *getClosingJalon(NSLdvGoalInfo* pJalon) ;
    GoalInfoIter    getClosingJalon(GoalInfoIter iJalon) ;
} ;

class _NSLDVCLASSE NSDelayZone
{
  protected:

    static long lObjectCount ;

	public:

    double dLowDelay ;
    string sLowDelayUnit ;
    bool   bLowIncluded ;

    double dHighDelay ;
    string sHighDelayUnit ;
    bool   bHighIncluded ;

    NSLdvGoalInfo::JALONSLEVELS iLevel ;

    NSDelayZone(NSLdvGoalInfo::JALONSLEVELS iColour, double dLow, string sLow, bool bLow, double dHigh, string sHigh, bool bHigh) ;
    NSDelayZone(const NSDelayZone& rv) ;
    ~NSDelayZone() ;

    bool isInside(double dValue, string sUnit) ;

    NSDelayZone& operator=(const NSDelayZone& src) ;

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }
} ;

typedef vector<NSDelayZone*> NSDelayZoneVector ;
typedef NSDelayZoneVector::iterator       NSDelayZoneIter ;
typedef NSDelayZoneVector::const_iterator NSDelayZoneConstIter ;

class _NSLDVCLASSE NSDelayZoneArray : public NSDelayZoneVector
{
  public :

    // Constructeurs
    NSDelayZoneArray() ;
    NSDelayZoneArray(const NSDelayZoneArray& rv) ;
    // Destructeur
    virtual ~NSDelayZoneArray() ;

    void vider() ;

    NSLdvGoalInfo::JALONSLEVELS getColor(double dValue, string sUnit) ;
    void setLowValues(NSLdvGoalInfo::JALONSLEVELS iColorZone, double dLow, string sLow, bool bLow) ;
    void setHighValues(NSLdvGoalInfo::JALONSLEVELS iColorZone, double dHigh, string sHigh, bool bHigh) ;
    NSDelayZone* getZone(NSLdvGoalInfo::JALONSLEVELS iColorZone) ;

    NSDelayZoneArray& operator=(const NSDelayZoneArray& src) ;
} ;

class _NSLDVCLASSE NSDateZone
{
  protected:

    static long lObjectCount ;

	public :

    string sLowDate ;
    bool   bLowIncluded ;

    string sHighDate ;
    bool   bHighIncluded ;

    NSLdvGoalInfo::JALONSLEVELS iLevel ;

    NSDateZone(NSLdvGoalInfo::JALONSLEVELS iColour, string sLow, bool bLow, string sHigh, bool bHigh) ;
    NSDateZone(const NSDateZone& rv) ;
    ~NSDateZone() ;

    bool isInside(string sDate) ;

    NVLdVTemps getLowDate() ;
    NVLdVTemps getHighDate() ;

    NSDateZone& operator=(const NSDateZone& src) ;

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }
} ;

typedef vector<NSDateZone*> NSDateZoneVector ;
typedef NSDateZoneVector::iterator       NSDateZoneIter ;
typedef NSDateZoneVector::const_iterator NSDateZoneConstIter ;

class _NSLDVCLASSE NSDateZoneArray : public NSDateZoneVector
{
	public :

    // Constructeurs
    NSDateZoneArray() ;
    NSDateZoneArray(const NSDateZoneArray& rv) ;
    // Destructeur
    virtual ~NSDateZoneArray() ;

    void vider() ;

    NSLdvGoalInfo::JALONSLEVELS getColor(string sDate) ;
    void setLowValues(NSLdvGoalInfo::JALONSLEVELS iColorZone, string sLow, bool bLow) ;
    void setHighValues(NSLdvGoalInfo::JALONSLEVELS iColorZone, string sHigh, bool bHigh) ;
    NSDateZone* getZone(NSLdvGoalInfo::JALONSLEVELS iColorZone) ;

    NSDateZoneArray& operator=(const NSDateZoneArray& src) ;
};

class _NSLDVCLASSE NSValueZone
{
  protected:

    static long lObjectCount ;

  public :

    bool   bLowExist ;
    double dLowValue ;
    string sLowValueUnit ;
    bool   bLowIncluded ;

    bool   bHighExist ;
    double dHighValue ;
    string sHighValueUnit ;
    bool   bHighIncluded ;

    NSLdvGoalInfo::JALONSLEVELS iLevel ;

    NSValueZone(NSLdvGoalInfo::JALONSLEVELS iColour, bool bIsLow, double dLow, string sLow, bool bLow, bool bIsHigh, double dHigh, string sHigh, bool bHigh) ;
    NSValueZone(const NSValueZone& rv) ;
    ~NSValueZone() ;

    bool isInside(double dValue, string sUnit) ;

    NSValueZone& operator=(const NSValueZone& src) ;

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }
} ;

typedef vector<NSValueZone*> NSValueZoneVector ;
typedef NSValueZoneVector::iterator       NSValueZoneIter ;
typedef NSValueZoneVector::const_iterator NSValueZoneConstIter ;

class _NSLDVCLASSE NSValueZoneArray : public NSValueZoneVector
{
  public :

    // Constructeurs
    NSValueZoneArray() ;
    NSValueZoneArray(const NSValueZoneArray& rv) ;
    // Destructeur
    virtual ~NSValueZoneArray() ;

    void vider() ;

    NSLdvGoalInfo::JALONSLEVELS getColor(double dValue, string sUnit) ;
    void setLowValues(NSLdvGoalInfo::JALONSLEVELS iColorZone, double dLow, string sLow, bool bLow) ;
    void setHighValues(NSLdvGoalInfo::JALONSLEVELS iColorZone, double dHigh, string sHigh, bool bHigh) ;
    NSValueZone* getZone(NSLdvGoalInfo::JALONSLEVELS iColorZone) ;

    NSValueZoneArray& operator=(const NSValueZoneArray& src) ;
};

#endif // __NSLDVGOALINFO_H__

