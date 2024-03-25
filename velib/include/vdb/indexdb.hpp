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

#include "utility.hpp"
#include "Poco/Data/Session.h"
#include "utility/videotype.hpp"

/* The string is the device id of disk
    For example, on Unix filesystems (including OS X), this returns the
    devpath like \c /dev/sda0 for local storages. On Windows, it returns the UNC
    path starting with \c \\\\?\\ for local storages (in other words, the volume GUID).
*/

typedef std::map<string, VdbDiskItem> VDBDiskMap;
typedef std::map<string, VdbDiskStatus> VDBDiskStatus;

class VE_LIBRARY_API IndexDB
{
public:
	IndexDB(string & strPath);
	~IndexDB();
	
public:
	BOOL Init();
	BOOL UpdateDiskMap();
	BOOL CreateTablesIfNecessary();
	BOOL CreateBlocksIfNecessary(string & strPath, int nSize);
	BOOL CreateABlock(string & strHdd, string &strBlockPath);
	BOOL DeInit();
	
public:
	//The size is in M bytes
	/* Disk control API*/
	BOOL AddHdd(string &strHdd, string & strPath, long nSize);
	BOOL HddUpdateSize(string &strHdd, long nSize);
	BOOL DelHdd(string &strHdd);

	/* Disk map API */
	BOOL GetDiskMap(VDBDiskMap &pMap);
	BOOL GetDiskStatus(VDBDiskStatus &pStatus);
	BOOL UpdateDiskStatusMap(VDBDiskStatus &pMap);

	/* Internal Disk function */
	BOOL InitDiskMap();
	BOOL  SelectADisk(string &strHdd);
	
	BOOL RequestABlockFile(string & strPath);

       /* Get the old record */
	BOOL GetAOldRecord(string & strPath);
	BOOL GetRecordFilePath(VdbRecordItem &pItem, string & strPath);
	BOOL RecordRLock(string & strPath);
	BOOL RecordRUnLock(string & strPath);
	BOOL RecordWLock(string & strPath);
	BOOL RecordWUnLock(string & strPath);
	BOOL ClearAllBlockLock();
	BOOL ClearAllBlockWriting();
	BOOL CorrectAllTheRecord();
	
	
	/* The return is the ID of the record */
	long AddRecord(string deviceId, unsigned int recordType, unsigned int startTime, string & strPathBlock);
	BOOL UpdateRecordEndtime(long recordId, unsigned int endTime);
	
	
	/* Search Interface */
	BOOL SearchAItem(string deviceId, unsigned int Time, VdbRecordItem &pItem);
	BOOL SearchAItemNear(string deviceId, unsigned int Time, VdbRecordItem &pItem);
	BOOL SearchNextItem(string deviceId, long LastId, VdbRecordItem &pItem);
	BOOL SearchItems(string deviceId, unsigned int startTime, unsigned int endTime, unsigned int recordType, 
				RecordItemMap & pMap);
	BOOL SearchHasItems(string deviceId, unsigned int startTime, unsigned int endTime, unsigned int recordType);

public:
	/* The Task will merge type with real items, and pre record is based on the file */
	BOOL AddSchedItem(string deviceId, unsigned int startTime, unsigned int endTime, RecordingType type);
	bool SetPrePostRecordTime(int nPreTime, int nPostTime);
	/* The return vaule is sleep time of the thread */
	unsigned int MergeSingleStep();
	bool ProcessOneRecord(VdbRecordItem &pItem);
	bool RemoveOldScheds();
private:
	string m_strIndexPath;
	Poco::Data::Session *m_DB;
	VDBDiskMap m_diskMap;
	VDBDiskStatus m_diskStatus;
	bool m_diskStatusInit;
};

