#ifndef _OWLDLL
# include "BB1BB.h"
# include "BB1Focus.h"   // for BB1BB::RateKSAR(...)
# include "BB1KSAR.h"    // for BB1BB::RateKSAR()
#else
# include "ns_ob1\BB1BB.h"
# include "ns_ob1\BB1Focus.h"   // for BB1BB::RateKSAR(...)
# include "ns_ob1\BB1KSAR.h"    // for BB1BB::RateKSAR()
#endif


// -----------------------------------------------------------------------------
// Convert a proposed rating to a number between 0 and 100. This doesn't have to
// be a member function since all it needs is its Rating formal parameter.
// -----------------------------------------------------------------------------

Rating NormalizeRating(const Rating rating)
{
  if (rating < 0.0)
    return Rating(0.0) ;
  else
    if (rating > 100.0)
      return Rating(100.0) ;
    else
      return rating ;
}


#ifndef __TCONTROLER__
// -----------------------------------------------------------------------------
// Compute a priority for the KSAR, and assign it.
// -----------------------------------------------------------------------------

Errcode
BB1BB::RateKSAR(BB1KSAR& KSAR) const
{
#ifdef _DEBUG_
  _trout << "-- DEBUG:: BB1BB::GoNCyclesPhaseCheck() const\n\n" ;
#endif

  double sr = 0.0 ; // sum of ratings
  double sw = 0.0 ; // sum of weights
  // RatingRecord &rrset = KSAR.RatingRecord();
  // for (Pix p = rrset.first(); p != 0; rrset.next(p)) delete rrset(p);
  // rrset.clear();
  if (false == foci->empty())
    for (FocusCIter f = foci->begin() ; foci->end() != f ; f++)
    {
      const BB1Focus *const focus = *f ;
      if (focus->OperativeP())
      {
				const CriterionT	criterion	= focus->Criterion() ;
				const WeightT		weight		= focus->Weight() ;
				const Rating		rating		= (*criterion)(KSAR) ;
				const Rating		nrating		= NormalizeRating(rating) ;
				if (rating >= 0.0)
				{
					sr += nrating * weight ;
					sw += weight ;
				}
      	// Otherwise, irrelevant Focus/Criterion
      }
    }
  return (KSAR.Rate((sw == 0.0) ? PriorityT(0.0) : PriorityT(sr/sw))) ;
}
#endif

