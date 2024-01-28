/*
 * TinyTimer.cpp
 *
 *  Created on: 2004-6-15
 *      Author: walter
 */



#include "TinyTimer.h"
#include <sys/time.h>

namespace wfan {

/*const uint32_t ONE_MSEC_IN_USECS = 1000L;
const uint32_t ONE_SECOND_IN_MSECS = 1000L;
const uint32_t ONE_SECOND_IN_USECS = 1000000L;
const uint64_t ONE_SECOND_IN_NSECS = 1000000000L;*/



TimeUnit CTinyTimer::ms_default_time_unit = MILLI_SECOND;

CTinyTimer::CTinyTimer()
{
	Reset();
}

CTinyTimer::~CTinyTimer()
{

}

void CTinyTimer::Reset()
{
	m_initTime = GetTimeNow();
	m_startTime = 0;
	m_endTime = 0;
	m_deadline = 0;
}


void CTinyTimer::Begin(int64_t aTime)
{
	if(aTime == 0)
		m_startTime = aTime;
	else
		m_startTime = GetTimeNow();

	m_endTime = 0;
}

void CTinyTimer::End(int64_t aTime)
{
	if(aTime == 0)
		m_endTime = aTime;
	else
		m_endTime = GetTimeNow();

}

void CTinyTimer::SetDeadline(int64_t aTime)
{
	m_deadline = aTime;
}



int64_t CTinyTimer::GetDuration(TimeUnit timeUnit)
{
	int64_t duration = 0;
	if(m_endTime > 0) {
		duration =  m_endTime - m_startTime;
	} else {
		duration = GetTimeNow() - m_startTime;
	}
	if (timeUnit == SECOND) {
		return duration / 1000;
	} else if (timeUnit == MINUTE) {
		return duration / 60000;
	} else if (timeUnit == HOUR) {
		return duration / 3600000;
	}
	return duration;
}

int64_t CTinyTimer::GetTimeNow(TimeUnit timeUnit)
 {
	struct timeval t;
	int64_t s;

	// get the time of day
	gettimeofday(&t, 0);

	// calculate the number of milliseconds represented by the seconds
	s = t.tv_sec;
	s *= 1000;

	// calculate the milliseconds now
	s += (t.tv_usec / 1000);

	// return the value
	if(timeUnit == SECOND) {
	   return s/ 1000;
	}
	else if(timeUnit == MINUTE) {
		   return s/60000;
	}
	else if(timeUnit == HOUR) {
		   return s/3600000;
	}

	return s;

}

int64_t CTinyTimer::GetTimeLeft(TimeUnit timeUnit)
{
	if(m_endTime > 0) {
		return 0;
	}

	if(m_deadline > 0) {
		return m_deadline - GetTimeNow();
	}

	return 0;
}

int64_t CTinyTimer::GetTimeCost(TimeUnit timeUnit)
{
	return GetTimeNow() - m_initTime;
}


TimeUnit CTinyTimer::GetDefaultTimeUnit() {
	return ms_default_time_unit;
}

}//--end of namespace
