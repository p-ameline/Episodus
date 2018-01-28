/*
	CITIPProcessor.h

	Author:
	Description:	<describe the CITIPProcessor class here>
*/

#ifndef CITIPProcessor_H
#define CITIPProcessor_H

#include "cdstring.h"

#include "CICalendarComponent.h"
#include "CICalendarComponentRecur.h"
#include "CICalendarPeriod.h"

class CAddress;
class CAddressList;
class CDataAttachment;
class CIdentity;
class CMessage;

namespace iCal {

class CICalendar;
class CICalendarVEvent;
class CICalendarVToDo;

class CITIPProcessor
{
public:

	enum EAttendeeState
	{
		eNone,
		eAllAccepted,
		eSomeAccepted,
		eSomeDeclined,
		eIHaveAccepted,
		eIHaveNotAccepted
	};

	enum EDescriptionType
	{
		ePublish,
		eRequest,
		eReplyAccepted,
		eReplyDeclined
	};

	static void	SendCalendar(const CICalendar& cal, CDataAttachment*& data);
	static void	PublishEvents(const CICalendarComponentRecurs& vevents, const CIdentity* id);
	static void	PublishComponent(const CICalendarVToDo* vtodo, const cdstring& organiser, cdstring& desc, CDataAttachment*& attach);

	static void SendRequest(const CAddressList* addrs, const CICalendarComponent* comp, const CIdentity* id);
	static void SendReply(const CICalendarComponent* comp, const cdstring& status, const CICalendarProperty& attendee, const CIdentity* id, CMessage* msg, EDescriptionType type);

	static void ProcessAttachment(const char* data, CMessage* msg);

	static bool NeedsITIPRequest(const CICalendarComponent& comp);
	static void GetRequestDetails(const CICalendarComponent& comp, cdstrvect& https, CAddressList& emails, cdstring& id);
	static void ClearITIPRequest(CICalendarComponent& comp);

	static bool OrganiserIsMe(const CICalendarComponent& comp);
	static EAttendeeState GetAttendeeState(const CICalendarComponent& comp);
	static cdstring GetAttendeeDescriptor(const CICalendarProperty& attendee);
	static cdstring GetAttendeeRoleDescriptor(const CICalendarProperty& attendee);
	static cdstring GetAttendeeStatusDescriptor(const CICalendarProperty& attendee);

private:
	CITIPProcessor();
	~CITIPProcessor();

	static void SendiMIPRequest(const CAddressList* addrs, const CICalendar& cal, const CICalendarComponent* comp, const CIdentity* id);
	static void SendCalDAVRequest(const CAddressList* addrs, const CICalendar& cal, const CICalendarComponent* comp, const CIdentity* id);
	static void ReceivePublish(const CICalendar& cal);
	static void ReceiveRequest(const CICalendar& cal, CMessage* msg = NULL);
	static void ReceiveRefresh(const CICalendar& cal, CMessage* msg = NULL);
	static void ReceiveCancel(const CICalendar& cal, CMessage* msg = NULL);
	static void ReceiveAdd(const CICalendar& cal, CMessage* msg = NULL);
	static void ReceiveReply(const CICalendar& cal, CMessage* msg = NULL);
	static void ReceiveCounter(const CICalendar& cal, CMessage* msg = NULL);
	static void ReceiveDeclineCounter(const CICalendar& cal, CMessage* msg = NULL);
	
	enum EFoundComponent
	{
		eNotFound,
		eFoundExact,
		eFoundMaster
	};

	static EFoundComponent LookForComponentInCalendars(const CICalendarComponent* orig, CICalendarComponent*& found);
	
	static void GenerateMIMEData(const CICalendar* cal, const CICalendarComponent* comp, const cdstring& mime_method, cdstring& desc, CDataAttachment*& attach, EDescriptionType type);

	static void DescribeComponent(const CICalendarComponent* comp, cdstring& desc, EDescriptionType type);
	static void DescribeComponent(const CICalendarVEvent* vevent, cdstring& desc, EDescriptionType type);
	static void DescribeComponent(const CICalendarVToDo* vtodo, cdstring& desc, EDescriptionType type);
	
	// REQUEST processing
	enum ECompareComponents
	{
		eFirstNewer,
		eFirstOlder,
		eTheSame
	};
	static ECompareComponents Compare(const CICalendarVEvent* comp_old, const CICalendarVEvent* comp_new);

	static bool CheckForReschedule(const CICalendarVEvent* comp_old, const CICalendarVEvent* comp_new);
	static bool CheckForConflicts(const CICalendarVEvent* comp);
	static void DetermineITIPBusyPeriods(const CICalendarVEvent* comp, CICalendarPeriodList& busy);
	static bool AttendeeIdentity(const CICalendarProperty& attendee, CIdentity*& id);
	
	static bool GetOrganiserAddress(const CICalendarComponent* comp, CAddress& addr);
	static bool GetMatchingAttendee(CICalendarComponent* comp, const CICalendarProperty& attendee, CICalendarProperty*& found);
};

}	// namespace iCal

#endif	// CICalendar_H
