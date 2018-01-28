// -----------------------------------------------------------------------------
// nsmt_trdrugs.h
// -----------------------------------------------------------------------------
// $Revision: 1.1 $
// $Author: pameline $
// $Date: 2015/02/07 14:50:29 $
// -----------------------------------------------------------------------------
// Turkish drug database mapping
// -----------------------------------------------------------------------------
// PA - June 2006
// -----------------------------------------------------------------------------

#ifndef __NSMXP_OBJECTS_H__
#define __NSMXP_OBJECTS_H__

#include "nsmxp_SOAP\mssoap30_proxy.h"

class _PWCLASS COM_TAI_LL_Services
{
	public:

		COM_TAI_LL_Services() {}

		bool getPoliclinicAdmissionData(TAutoString input) ;
} ;


class _PWCLASS COM_TAI_HL_Services : public TISoapClientProxy
{
	public:

		COM_TAI_HL_Services() : TISoapClientProxy() {}

		TAutoString getPoliclinicAdmissionData(TAutoString input) ;
    bool        connectToWebServices(string sWsdlFileNames) ;
} ;


#endif  // __NSMXP_OBJECTS_H__
