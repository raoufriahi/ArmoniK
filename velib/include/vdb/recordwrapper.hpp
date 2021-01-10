/*
 * Copyright (c) 2017-2018 Heimdall
 *
 * The computer program contained herein contains proprietary
 * information which is the property of Heimdall.
 * The program may be used and/or copied only with the written
 * permission of Heimdall or in accordance with the
 * terms and conditions stipulated in the agreement/contract under
 * which the programs have been supplied.
 */
#ifndef __RECORD_WRAPPER_HPP__
#define __RECORD_WRAPPER_HPP__
#include "utility.hpp"
#include "mediafile.hpp"

#define VDB_DAYS_IN_WEEK 7
#define VDB_HOURS_IN_DAY 24

#define VDB_MON 0
#define VDB_TUE 1
#define VDB_WEN 2
#define VDB_THR 3
#define VDB_FRI 4
#define VDB_SAT 5
#define VDB_SUN 6



class RecordSchedDay
{
public:
	RecordSchedDay()
	{
		for (int i = 0; i < VDB_HOURS_IN_DAY; i++)
		{
			type[i] = R_OFF;
		}
	}
	RecordSchedDay(RecordingType allType)
	{
		for (int i = 0; i < VDB_HOURS_IN_DAY; i ++)
		{
			type[i] = allType;
		}
	}
	RecordSchedDay(const RecordSchedDay &pDay)
	{
		for (int i = 0; i < VDB_HOURS_IN_DAY; i ++)
		{
			type[i] = pDay.type[i];
		}
	}
	RecordSchedDay  operator +  (const RecordSchedDay& pDay) const
	{
		RecordSchedDay dayType;
		for (int i = 0; i < VDB_HOURS_IN_DAY; i ++)
		{
			dayType.type[i] = pDay.type[i] | type[i];
		}

		return dayType;
	}
public:
	u8 type[VDB_HOURS_IN_DAY];
};

class RecordSchedWeek
{
public:
	RecordSchedWeek()
	{
		RecordSchedDay dayType;
		for (int i = 0; i < VDB_DAYS_IN_WEEK; i ++)
		{
			week[i] = dayType;
		}
	}
	RecordSchedWeek(RecordSchedDay allType)
	{
		for (int i = 0; i < VDB_DAYS_IN_WEEK; i ++)
		{
			week[i] = allType;
		}
	}
	RecordSchedWeek(const RecordSchedWeek &pWeek)
	{
		for (int i = 0; i < VDB_DAYS_IN_WEEK; i ++)
		{
			week[i] = pWeek.week[i];
		}
	}
	RecordSchedWeek(std::string &pWeek)
	{
		for (int i = 0; i < VDB_DAYS_IN_WEEK; i ++)
		{
			for (int j = 0; j < VDB_HOURS_IN_DAY; j ++)
			{
				week[i].type[j] = pWeek[i * VDB_HOURS_IN_DAY + j];
			}
			
		}
	}
	std::string ToString()
	{
		std::string sched;
		for (int i = 0; i < VDB_DAYS_IN_WEEK; i ++)
		{
			for (int j = 0; j < VDB_HOURS_IN_DAY; j ++)
			{
				char c = week[i].type[j];
				sched.push_back(c);
			}
			
		}

		return sched;
	}
	RecordSchedWeek  operator +  (const RecordSchedWeek& pWeek) const
	{
		RecordSchedWeek weekType;
		for (int i = 0; i < VDB_DAYS_IN_WEEK; i ++)
		{
			weekType.week[i] = pWeek.week[i] + week[i];
		}
		return weekType;
	}
public:
	static RecordSchedWeek CreateOff()
	{
		RecordSchedDay day(R_OFF);
		
		RecordSchedWeek week(day);
		
		return week;
	}
	static RecordSchedWeek CreateWorkDay()
	{
		RecordSchedDay day(R_SCHED);
		RecordSchedDay dayOff(R_OFF);
		
		RecordSchedWeek week(day);
		week.week[VDB_SAT] = dayOff;
		week.week[VDB_SUN] = dayOff;
		
		return week;
	}
	static RecordSchedWeek CreateAllDay()
	{
		RecordSchedDay day(R_SCHED);
		
		RecordSchedWeek week(day);
		return week;
	}
	static bool CheckStringForSched(std::string &pSched)
	{
		if (pSched.length() != VDB_DAYS_IN_WEEK * VDB_HOURS_IN_DAY)
		{
			return false;
		}else
		{
			return true;
		}
	}
public:
	RecordSchedDay week[VDB_DAYS_IN_WEEK];
};

class VDB;
class RecordWrapperImpl;
typedef std::map<astring, RecordSchedWeek> RecordSchedWeekMap;
typedef void ( * RecChangeFunctionPtr)(astring strId, bool bRec, void * pParam);

class VE_LIBRARY_API RecordWrapper
{
public:
    RecordWrapper(VDB &pVdb, astring deviceId, 
				RecordSchedWeekMap cSchedMap, RecChangeFunctionPtr pCallback, 
				void *pParam);
    ~RecordWrapper();
public:
	MFStatus PushAFrame(VideoFrame &frame);
	bool UpdateSchedMap(RecordSchedWeekMap cSchedMap);
	bool FireAlarm(s64 nStartTime);
	bool ManualRecStart();
	bool ManualRecStop();
	bool SetPrePostRecordTime(s32 nPreTime, s32 nPostTime);

private:
	RecordWrapperImpl *m_pImpl;
};

#endif /* __RECORD_WRAPPER_HPP__ */
