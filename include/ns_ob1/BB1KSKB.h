/*
**
** BB1KSKB.h
**
*/

#ifndef _BB1KSKB_h
# define _BB1KSKB_h 1

# ifdef __GNUC__
#  pragma interface
# endif

# include "BB1types.h"

class BB1BB ;
class BB1Event ;

#ifndef __TCONTROLER__

bool		UpdatePrescriptionTC(const BB1Event& event) ;
bool		UpdatePrescriptionPC(const BB1BB& bb, BB1KSAR& ksar) ;
bool		UpdatePrescriptionOC(const BB1BB& bb, BB1KSAR& ksar) ;
Errcode	UpdatePrescriptionAC(BB1BB& bb, BB1KSAR& ksar) ;

bool		TerminatePrescriptionTC(const BB1Event& event) ;
bool		TerminatePrescriptionPC(const BB1BB& bb, BB1KSAR& ksar) ;
bool		TerminatePrescriptionOC(const BB1BB& bb, BB1KSAR& ksar) ;
Errcode	TerminatePrescriptionAC(BB1BB& bb, BB1KSAR& ksar) ;

void		loadBB1KSKB(BB1BB& bb);
#endif

#endif // _BB1KSKB_h
