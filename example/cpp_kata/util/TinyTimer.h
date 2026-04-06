/*
 * TinyTimer.h
 *
 *  Created on: 2004-6-15
 *      Author: walter
 */

#ifndef __TINY_TIMER_H__
#define __TINY_TIMER_H__

#include <stdint.h>
#include <sys/time.h>

#ifndef NULL
#define NULL (0)
#endif


namespace wfan {

typedef enum {
	MICRO_SECOND,
	MILLI_SECOND,
	SECOND,
	MINUTE,
	HOUR,
	DAY,
	WEEK,
	MONTH,
	YEAR
} TimeUnit;

class CTinyTimer {
public:
	CTinyTimer();
	virtual ~CTinyTimer();

	void Reset();

	void Begin(int64_t beginTime = 0);
	void End(int64_t endTime = 0);
	void SetDeadline(int64_t deadline);

	static TimeUnit GetDefaultTimeUnit();

	int64_t GetDuration(TimeUnit timeUnit = MILLI_SECOND);
	int64_t GetTimeNow(TimeUnit timeUnit = MILLI_SECOND);
	int64_t GetTimeLeft(TimeUnit timeUnit = MILLI_SECOND);
	int64_t GetTimeCost(TimeUnit timeUnit = MILLI_SECOND);
private:
	int64_t m_initTime;
	int64_t m_startTime;
	int64_t m_endTime;
	int64_t m_deadline;
	static TimeUnit ms_default_time_unit;

};



class CTimeValue {
public:
    CTimeValue(timeval& aTv): m_lSec(aTv.tv_sec), m_lUsec(aTv.tv_usec)
    {

    }
    static CTimeValue GetTimeOfDay();

private:
	long m_lSec;
	long m_lUsec;

};



#ifndef CM_WIN32
inline CTimeValue CTimeValue::GetTimeOfDay()
{
	timeval tvCur;
	::gettimeofday(&tvCur, NULL);
	return CTimeValue(tvCur);
}
#endif // !CM_WIN32

class ITimerHandler
{
public:
	virtual void OnTimeout(const CTimeValue& aCurtime, void* pArg);
	virtual ~ITimerHandler() {}
};

class ITimerManager{
public:
	virtual int ScheduleTimer(ITimerHandler * pHandler, void* pArg, const CTimeValue& aCurtim, int aCount) = 0;
	virtual int CancelTimer(ITimerHandler * pHandler);

	virtual ~ITimerManager() { }
};
}
#endif /* __TINY_TIMER_H__ */
