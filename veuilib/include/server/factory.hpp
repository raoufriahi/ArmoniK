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
#include "config/confdb.hpp"
#include "server/camera.hpp"
#include "vdb.hpp"
#include "vhdfsdb.hpp"
#include "vplay.hpp"
#include "config/sysdb.hpp"
#include "server/hdddevice.hpp"
#include <QThread>
#include <qdebug.h>
#include "Poco/Path.h"
#include "Poco/File.h"
#include "config/vidconf.pb.h"
#include "XSDK/TimeUtils.h"
#include "config/videnv.hpp"

using namespace XSDK;
class Factory;

typedef enum
{
    FACTORY_CAMERA_ADD = 1,
    FACTORY_CAMERA_DEL,
    FACTORY_CAMERA_ONLINE,
    FACTORY_CAMERA_OFFLINE,
    FACTORY_CAMERA_REC_ON,
    FACTORY_CAMERA_REC_OFF,
    FACTORY_CAMERA_HDFS_REC_ON,
    FACTORY_CAMERA_HDFS_REC_OFF,
    FACTORY_CAMERA_LAST
} FactoryCameraChangeType;


class UB_LIBRARY_API FactoryCameraChangeData
{
public:
	FactoryCameraChangeType type;
	string id;
	//VidCamera cCam;
};

typedef BOOL (*FactoryCameraChangeNotify)(void* pParam, 
		FactoryCameraChangeData data);

typedef BOOL (*FactoryUserChangeNotify)(void* pParam, 
		string strUser, string strPasswd);

typedef std::list<LPCamera> CameraList;
typedef std::list<CameraParam> CameraParamList;
typedef std::map<string, LPCamera> CameraMap;
typedef std::map<string, CameraParam> CameraParamMap;
typedef std::map<string, bool> CameraOnlineMap;
typedef std::map<string, bool> CameraRecMap;
typedef std::map<void *, FactoryCameraChangeNotify> CameraChangeNofityMap;
typedef std::map<void *, FactoryUserChangeNotify> UserChangeNofityMap;


class UB_LIBRARY_API FactoryHddTask
{

public:
	FactoryHddTask(Factory &pFactory);
	~FactoryHddTask();
	void run();
private:
	Factory &m_Factory;
	std::thread workerThread; 
};

/* Fatory is Qthread for callback in Qt GUI */
class UB_LIBRARY_API Factory: public QThread
{
    Q_OBJECT
public:
    Factory(VidEnv &pEnv);
    ~Factory();

	/* Init function */
	BOOL Init();
	s32 InitAddCamera(CameraParam & pParam, string strCamId);
	ConfDB &GetConfDB(){return m_Conf;};
	
	BOOL RegCameraChangeNotify(void * pData, FactoryCameraChangeNotify callback);
	BOOL UnRegCameraChangeNotify(void * pData);
	BOOL CallCameraChange(FactoryCameraChangeData data);
	static BOOL RecChangeHandler(string strId, bool bRec, void * pParam);
	BOOL RecChangeHandler1(string strId, bool bRec);

	BOOL RegUserChangeNotify(void * pData, FactoryUserChangeNotify callback);
	BOOL UnRegUserChangeNotify(void * pData);
	BOOL CallUserChange(string strUser, string strPasswd);
	
	BOOL GetLicense(string &strLicense, string &strHostId, 
							int &ch, string &type, string &startTime, string &expireTime);
	BOOL SetLicense(string &strLicense);
	BOOL InitLicense();

	BOOL GetExportPath(string &strPath);
	BOOL SetExportPath(string &strPath);

	BOOL GetEventDBConf(VidEventDBConf &pConf);

	bool AuthUser(string &strUser, string &strPasswd);
	bool GetAdminPasswd(string &strPasswd);
	bool SetAdminPasswd(string strPasswd);

	BOOL GetCameraOnlineMap(CameraOnlineMap &pMap);
	BOOL GetCameraRecMap(CameraRecMap &pMap);
	bool GetCameraList(VidCameraList & pCameraList);

       /* Camera function */
	string AddCamera(CameraParam & pParam);
	BOOL GetCameraRtspUrl(string & strUrl, string strCamId);
	BOOL DelCamera(string strCamId);
	BOOL GetCamera(string strId, VidCamera & pCam);
	BOOL PtzAction(string strCamId, FPtzAction action, float speed);
	BOOL UpdateRecSched(string strCamId, VidCamera &pCam);
	BOOL FireAlarm(string strCamId, s64 nStartTime);

	/* Disk function */
	BOOL AddHdd(string strHdd, string  strPath, s64 nSize);
	BOOL DelHdd(string  strHdd);
	BOOL HddUpdateSize(string strHdd, s64 nSize);
	BOOL GetDiskMap(VDBDiskMap &pMap);
	BOOL GetDiskStatus(VDBDiskStatus &pStatus);
	BOOL UpdateDiskStatusMap(VDBDiskStatus &pStatus);

	/* Search function */
	BOOL SearchItems(string strCamId, u32 startTime, u32 endTime, u32 recordType, 
					RecordItemMap &map);
	BOOL SearchHasItems(string strCamId, u32 startTime, u32 endTime, 
					u32 recordType);

	VDB & GetVdb();
	VidEnv & GetEnv(){return m_env;}


	BOOL GetStreamInfo(string strCamId, VideoStreamInfo &pInfo);
	BOOL GetCamStreamList(string strCamId, VidStreamList &pList);
	/* Data */
	BOOL RegDataCallback(string strCamId, CameraDataCallbackFunctionPtr pCallback, void * pParam);
	BOOL UnRegDataCallback(string strCamId, void * pParam);
	BOOL GetInfoFrame(string strCamId, InfoFrame &pFrame);
	BOOL GetiFrame(string strCamId, VideoFrame& frame);
	BOOL RegSubDataCallback(string strCamId, CameraDataCallbackFunctionPtr pCallback, void * pParam);
	BOOL UnRegSubDataCallback(string strCamId, void * pParam);
	BOOL GetSubInfoFrame(string strCamId, InfoFrame &pFrame);

	BOOL GetCameraOnline(string strCamId, BOOL &bStatus);
	BOOL SetSystemPath(string strPath);

	void run();

private:
	CameraMap m_CameraMap;
	CameraOnlineMap m_CameraOnlineMap;
	CameraRecMap m_CameraRecMap;

	XMutex m_cMutex;

	CameraChangeNofityMap m_CameraChange;
	UserChangeNofityMap m_UserChange;

	VDB *m_pVdb;
	VHdfsDB *m_pVHdfsdb;
	FactoryHddTask *m_HddTask;

	ConfDB m_Conf;
	//SysDB m_SysPath;
	VidEnv &m_env;
};

typedef Factory* LPFactory;

