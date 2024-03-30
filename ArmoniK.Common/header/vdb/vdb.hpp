/*
 * Copyright (c) 2017-2024 UbVideo
 *
 * The computer program contained herein contains proprietary
 * information which is the property of UbVideo.
 * The program may be used and/or copied only with the written
 * permission of UbVideo or in accordance with the
 * terms and conditions stipulated in the agreement/contract under
 * which the programs have been supplied.
 */
#pragma once

#include "utility/debug.hpp"
#include "recordsession.hpp"
#include "indexdb.hpp"

class VE_LIBRARY_API VDB
{
public:
	typedef std::map<int, RecordSession*> _MapSession;

	VDB(string & strPath);
	~VDB();
	

	IndexDB  &GetIndexDB(){return m_IndexDB;}
	//VEventDB &GetEventDB(){return m_EventDB;}


	BOOL SetRecordingMode(RecordingMode mode);

	void Lock();
	void UnLock();

	BOOL AddHdd(string &strHdd, string & strPath, long nSize);
	BOOL HddUpdateSize(string &strHdd, long nSize);
	BOOL DelHdd(string &strHdd);

	/* Disk map API */
	BOOL GetDiskMap(VDBDiskMap &pMap);
	BOOL GetDiskStatus(VDBDiskStatus &pStatus);
	BOOL UpdateDiskStatusMap(VDBDiskStatus &pMap);

	

	/* Start and Stop Record */
	RecordSession * StartRecord(string deviceId, unsigned int startTime, unsigned int recordType);
	BOOL FinishRecord(RecordSession * pRs);
		

	/* video search function, if startTime or endTime is 0, it mean all */
	BOOL SearchItems(string deviceId, unsigned int startTime, unsigned int endTime, unsigned int recordType, 
					RecordItemMap & pMap);
	BOOL SearchHasItems(string deviceId, unsigned int startTime, unsigned int endTime, unsigned int recordType);
	BOOL SearchAItem(string deviceId, unsigned int Time, VdbRecordItem &pItem);
	BOOL SearchAItemNear(string deviceId, unsigned int Time, VdbRecordItem &pItem);
    	BOOL SearchNextItem(string deviceId, long LastId, VdbRecordItem &pItem);
	BOOL RequestAMFRead(VdbRecordItem &pItem, string & strPath);
	BOOL FinishedAMFRead(VdbRecordItem &pItem, string & strPath);

	/* The Task will merge type with real items, and pre record is based on the file */
	BOOL AddSchedItem(string deviceId, long startTime, long endTime, RecordingType recordType);
	static void Run(void * pParam);
	void Run1();
	
private:
	IndexDB m_IndexDB;

	RecordingMode m_Mode;
	fast_mutex m_Lock;

	tthread::thread *m_pThread;
	bool m_bExit;
	
};

