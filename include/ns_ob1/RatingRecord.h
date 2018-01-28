/*
**
** RatingRecord.h
**
*/

#ifndef _RatingRecord_h
# define _RatingRecord_h 1

# include "BB1types.h" // for Rating and WeightT

class BB1Focus ;
class BB1KSAR ;

class RatingRecord
{
public:
  RatingRecord(const BB1Focus& focus, const BB1KSAR& KSAR, Rating rating)
    : m_Focus(focus), m_KSAR(ksar) , m_rating(rating)
  {} ;

  RatingRecord(const RatingRecord& src)
    : m_Focus(src.m_Focus),
      m_KSAR(src.m_KSAR),
      m_rating(src.m_rating)
  {} ;

  const BB1Focus&	Focus() const { return (m_Focus) ; }
  const BB1KSAR&	KSAR() const { return (m_KSAR) ; }
  Rating		Rating() const { return (m_rating) ; }
  WeightT		Weight() const { return (m_Focus->Weight()) ; } // syntactic sugar

private:
  const BB1Focus&	m_Focus ;
  const BB1KSAR&	m_KSAR ;
  const Rating		m_rating ;
} ;

#endif // _RatingRecord_h

