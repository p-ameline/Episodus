#ifdef __GNUC__
#pragma implementation
#endif

#ifndef _OWLDLL
# include "BB1Focus.h"
#else
# include "ns_bbk\BB1Focus.h"
#endif

#ifndef __TCONTROLER__

BB1Focus::BB1Focus(	const string&			oname,
										BB1KB&						kb,
										BB1Class					*c,
										const AVPSet			*attSpec,
										const LinkSet			*linkSpec,
										const GoalT				g,
										int								OperativeP,
										int								StableP,
										WeightT						Weight,
										const CriterionT	crit,
										const string&			Description)
  : BB1Decision(oname, kb, c, attSpec, linkSpec, g, OperativeP, StableP, Weight, Description),
    criterion(crit)
{

#ifdef _DEBUG_
  cout << "-- DEBUG:: BB1Focus::BB1Focus("
       << "\"" << oname << "\", "
       << ", kb, *c, attSpec, linkSpec, g, "
       << "<" << OperativeP << ">, "
       << "<" << StableP << ">, Weight, crit, "
       << "\"" << Description << "\")\n\n" ;
#endif

};


BB1Focus::BB1Focus(const BB1Focus& src)
  : BB1Decision(src),
    criterion(src.criterion)
{
}

#endif
