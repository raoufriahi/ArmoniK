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
#ifndef __VDB_HPP__
#define __VDB_HPP__
#include "utility.hpp"
#include "debug.hpp"
#include "utility/videotype.hpp"
#include "recordsession.hpp"
#include "pbsession.hpp"
#include "indexdb.hpp"
#include "veventdb.hpp"

using namespace UtilityLib;

class VE_LIBRARY_API VDB
{
public:
	typedef std::map<s32, RecordSession*> _MapSession;
public:

	VDB(astring & strPath);
	~VDB();
	
public:
	IndexDB  &GetIndexDB(){return m_IndexDB;}
	//VEventDB &GetEventDB(){return m_EventDB;}

public:
	BOOL SetRecordingMode(RecordingMode mode);

	void Lock();
	void UnLock();

	BOOL AddHdd(astring &strHdd, astring & strPath, s64 nSize);
	BOOL HddUpdateSize(astring &strHdd, s64 nSize);
	BOOL DelHdd(astring &strHdd);

	/* Disk map API */
	BOOL GetDiskMap(VDBDiskMap &pMap);
	BOOL GetDiskStatus(VDBDiskStatus &pStatus);
	BOOL UpdateDiskStatusMap(VDBDiskStatus &pMap);

	
public:
	/* Start and Stop Record */
	RecordSession * StartRecord(astring deviceId, u32 startTime, u32 recordType);
	BOOL FinishRecord(RecordSession * pRs);
		
public:
	/* video search function, if startTime or endTime is 0, it mean all */
	BOOL SearchItems(astring deviceId, u32 startTime, u32 endTime, u32 recordType, 
					RecordItemMap & pMap);
	BOOL SearchHasItems(astring deviceId, u32 startTime, u32 endTime, u32 recordType);
	BOOL SearchAItem(astring deviceId, u32 Time, VdbRecordItem &pItem);
	BOOL SearchAItemNear(astring deviceId, u32 Time, VdbRecordItem &pItem);
    	BOOL SearchNextItem(astring deviceId, s64 LastId, VdbRecordItem &pItem);
	BOOL RequestAMFRead(VdbRecordItem &pItem, astring & strPath);
	BOOL FinishedAMFRead(VdbRecordItem &pItem, astring & strPath);
public:
	/* The Task will merge type with real items, and pre record is based on the file */
	BOOL AddSchedItem(astring deviceId, s64 startTime, s64 endTime, RecordingType recordType);
	static void Run(void * pParam);
	void Run1();
	
private:
	IndexDB m_IndexDB;

	RecordingMode m_Mode;
	fast_mutex m_Lock;

	tthread::thread *m_pThread;
	bool m_bExit;
	
};
#endif /* __VDB_HPP__ */
