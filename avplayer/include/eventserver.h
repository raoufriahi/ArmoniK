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
#include "avconfig/include/confdb.h"
#include "camera.h"
#include "vdb/vdb.hpp"
#include "vdb/vhdfsdb.hpp"
#include "vplay.h"
#include "avconfig/include/sysdb.h"
#include "factory.h"
#include "Poco/Path.h"
#include "Poco/File.h"
#include "config/vidconf.pb.h"
#include "XSDK/TimeUtils.h"
#include "XSDK/XBlockingQueue.h"
#include "Poco/UUIDGenerator.h"
#include "Poco/Data/RecordSet.h"
#include "Poco/Data/Column.h"
#include "Poco/Data/SQLite/Connector.h"
#include "Poco/Path.h"
#include "Poco/File.h"
#include "Poco/SharedPtr.h"
#include "Poco/DateTime.h"
#include "Poco/Data/SessionFactory.h"
#include "Poco/Data/SQLite/Connector.h"


using Poco::Data::Statement;
using Poco::Data::RecordSet;
using namespace Poco::Data;
using namespace Poco;
using namespace Poco::Data::Keywords;

using Poco::Data::Session;

class UB_LIBRARY_API VEventData
{
public:
	string strId;
	string strDevice;
	string strDeviceName;
	string strType;/* MOTION_START, MOTION_END, MOTION, SMART_MOTION */
	long nTime;
	string strEvttime;
	string strDesc;
	bool bHandled;
	string strComments;
	bool bMetaData;
	string strMetaData;/* the MetaData Depend the type */
};

typedef std::map<u64, VEventData> EventItemMap;

typedef void (*FunctionEventNotify)(VEventData data, void* pParam);
typedef std::map<void *, FunctionEventNotify> FunctionEventNotifyMap;

class UB_LIBRARY_API VEventServerDbTask: public std::thread 
{
public:
	VEventServerDbTask(Factory &pFactory);
	~VEventServerDbTask();

	void PushEvent(VEventData &pData);
	void UpdateDBSession(bool bIsFirst);
	/* Used by the Search task, TODO for the Release DataSession  */
	Poco::Data::Session *GetDataSession();
	void ReleaseDataSession();

	void run();
private:
	XSDK::XBlockingQueue<VEventData> m_Queue;
	Factory &m_Factory;
	Poco::Data::Session *m_pSqlSession;
	int m_nYear;
	int m_nMonth;
	XMutex m_cMutex;
	
};

//TODO VEventServerMetaDbTask for the Meta Data for the VA 

class UB_LIBRARY_API VEventServerCallbackTask: public std::thread
{
public:
	VEventServerCallbackTask(Factory &pFactory);
	~VEventServerCallbackTask();

	void PushEvent(VEventData &pData);
	/* Register the call back for the event */
	BOOL RegEventNotify(void * pData, FunctionEventNotify callback);
	BOOL UnRegEventNotify(void * pData);	

	void run();
private:
	XSDK::XBlockingQueue<VEventData> m_Queue;
	Factory &m_Factory;
	XMutex m_cMutex;

	FunctionEventNotifyMap m_NotifyMap;
};

typedef enum __VVidEventSearchCmdType
{
	VVID_EVENT_SEARCH_CMD = 1,
	VVID_EVENT_HANDLE_CMD,	
	VVID_EVENT_SEARCH_LAST
}VVidEventSearchCmdType;

typedef struct __VVidEventSearchCmd
{
	VVidEventSearchCmdType type;
	string strId;
	long nStart;
	long nEnd;
	void * pData;/* Who send this cmd */
}VVidEventSearchCmd;

class UB_LIBRARY_API VEventServerSearchTask: public std::thread
{
public:
	VEventServerSearchTask(Factory &pFactory, VEventServerDbTask &pDbTask);
	~VEventServerSearchTask();

	BOOL PushCmd(VVidEventSearchCmd &pCmd);
	/* Register the call back for the event */
	BOOL RegEventNotify(void * pData, FunctionEventNotify callback);
	BOOL UnRegEventNotify(void * pData);	
	BOOL ProcessSearchCmd(VVidEventSearchCmd &pCmd);
	BOOL ProcessHandleCmd(VVidEventSearchCmd &pCmd);
	void run();

private:
	XSDK::XBlockingQueue<VVidEventSearchCmd> m_Queue;
	Factory &m_Factory;
	XMutex m_cMutex;

	FunctionEventNotifyMap m_NotifyMap;
	VEventServerDbTask &m_pDbTask;
};


class UB_LIBRARY_API VEventServer: public std::thread
{
public:
	VEventServer(Factory &pFactory);
	~VEventServer();

	void PushEvent(VEventData &pData);

	/* Register the call back for the event */
	BOOL RegEventNotify(void * pData, FunctionEventNotify callback);
	BOOL UnRegEventNotify(void * pData);
	BOOL RegSearchEventNotify(void * pData, FunctionEventNotify callback);
	BOOL UnRegSearchEventNotify(void * pData);
	BOOL SearchEvent(string strId, long nStart, long nEnd, void *pData);
	BOOL HandleEvent(string strId);
	BOOL Init();

private:
	VEventServerDbTask m_DbTask;
	VEventServerCallbackTask m_CallbackTask;
	VEventServerSearchTask m_SearchTask;
	Factory &m_Factory;
};
