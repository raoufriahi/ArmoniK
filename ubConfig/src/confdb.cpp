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

#include <iostream>
#include <thread>
#include <mutex>
#include "confdb.h"
using namespace std;

#define VSC_OAPI_DEFAULT_PORT 9080

void VSCHdfsRecordDefault(VidHDFSConf &pData)
{
	pData.set_strnamenode("localhost");
	pData.set_strport("8020");
	pData.set_struser("admin");
	pData.set_strpasswd("admin");
	pData.set_nfileinterval(30);
}

void VSCStorServerConfDefault(VidStorServerConf &pData)
{
#if 0
	SimpleCrypt crypt;
	pData.set_noapiport(VSC_OAPI_DEFAULT_PORT);
	/* Default passwd is admin */
	//RRI QString strPass = "admin";
	//RRI pData.set_stradminpasswd(crypt.encryptToString(strPass).toStdString());
#endif
	return;
}

bool ConfDB::Open(string  pPath)
{
	m_Options.create_if_missing = true;
	leveldb::Status status = leveldb::DB::Open(m_Options, pPath, &m_pDb);
	if (false == status.ok()) {
	    VDC_DEBUG( "Unable to open/create test database %s\n", pPath.c_str());
	    return false;
	}

	string fakeKey = "fakeKey";
	string fakeValue = "fakeValue";
	SetCmnParam(fakeKey, fakeValue);
    return true;
}


bool ConfDB::FindCamera(string strCameraId)
{
	std::lock_guard<std::mutex> lock(m_cMutex);

	VidCameraList cameraList;
	GetCameraListConf(cameraList);

	for (int i = 0; i < cameraList.cvidcamera_size(); i ++) {
		const VidCamera &cam = cameraList.cvidcamera(i);
		if (cam.strid() == strCameraId) {
			return true;
		}
	}
	return false;
}

bool ConfDB::DeleteCamera(string strCameraId)
{
	std::lock_guard<std::mutex> lock(m_cMutex);

	VidCameraList cameraList;
	VidCameraList cameraListNew;
	GetCameraListConf(cameraList);
	for (int i = 0; i < cameraList.cvidcamera_size(); i ++) {
		const VidCamera &cam = cameraList.cvidcamera(i);
		if (cam.strid() != strCameraId) {
			VidCamera *pCam = cameraListNew.add_cvidcamera();
			*pCam = cam;
		}
	}
	UpdateCameraListConf(cameraListNew);
	return true;
}
	
bool ConfDB::AddCamera(VidCamera &pAddCam)
{
	std::lock_guard<std::mutex> lock(m_cMutex);
	
	VidCameraList cameraList;
	GetCameraListConf(cameraList);
	int cameraSize = cameraList.cvidcamera_size();

	VidCamera *pCam = cameraList.add_cvidcamera();
	*pCam = pAddCam;
	UpdateCameraListConf(cameraList);
	return true;
}

bool ConfDB::GetCameraConf(string strCameraId, VidCamera &pCam)
{
	std::lock_guard<std::mutex> lock(m_cMutex);
	
	VidCameraList cameraList;
	GetCameraListConf(cameraList);
	for (int i = 0; i < cameraList.cvidcamera_size(); i ++) {
		const VidCamera &cam = cameraList.cvidcamera(i);
		if (cam.strid() == strCameraId) {
			pCam = cam;
			return true;
		}
	}
	return false;
}

bool ConfDB::CameraRecordTemplSet(string strCameraId, string strTempl)
{
	std::lock_guard<std::mutex> lock(m_cMutex);

	return true;
}

bool ConfDB::GetLicense(string &strLicense)
{
	std::lock_guard<std::mutex> lock(m_cMutex);

	VSCConfLicenseKey sLicKey;
	
	leveldb::Slice key((char *)&sLicKey, sizeof(sLicKey));
	leveldb::Status Status = m_pDb->Get(leveldb::ReadOptions(), key, &strLicense);
	if (!Status.ok()) {
		strLicense = "";
		return false;
	}
	return true;
}
bool ConfDB::SetLicense(string &strLicense)
{
	std::lock_guard<std::mutex> lock(m_cMutex);
	
	VSCConfLicenseKey sLic;
	leveldb::WriteOptions writeOptions;

	leveldb::Slice licKey((char *)&sLic, sizeof(sLic));
	leveldb::Slice licValue(strLicense);
	m_pDb->Put(writeOptions, licKey, licValue);

	return true;
    
}

bool ConfDB::GetCmnParam(string &strKey, string &strParam)
{
	std::lock_guard<std::mutex> lock(m_cMutex);
	
	leveldb::Slice key(strKey);
	leveldb::Status Status = m_pDb->Get(leveldb::ReadOptions(), key, &strParam);
	if (!Status.ok()) {
		strParam = "";
		return false;
	}
	return true;
}

bool ConfDB::SetCmnParam(string &strKey, string &strParam)
{
	std::lock_guard<std::mutex> lock(m_cMutex);
	
	leveldb::WriteOptions writeOptions;

	leveldb::Slice licKey(strKey);
	leveldb::Slice licValue(strParam);
	m_pDb->Put(writeOptions, licKey, licValue);
	return true;
}



bool ConfDB::GetHdfsRecordConf(VidHDFSConf &pData)
{
	std::lock_guard<std::mutex> lock(m_cMutex);
	
	VSCConfHdfsRecordKey sKey;
    string strValue;

	leveldb::Slice key((char *)&sKey, sizeof(sKey));

	
	leveldb::Status Status = m_pDb->Get(leveldb::ReadOptions(), key, &strValue);
	if (!Status.ok()) {
		strValue = "fake";
	}

	if (pData.ParseFromString(strValue) == false) {
		VDC_DEBUG( "Hdfs Record Config is not init\n");
		VSCHdfsRecordDefault(pData);
		UpdateHdfsRecordConf(pData);
		return true;
	}
	return true;
}

/* HDFS record  */
bool ConfDB::UpdateHdfsRecordConf(VidHDFSConf &pData)
{
	std::lock_guard<std::mutex> lock(m_cMutex);
	
	VSCConfHdfsRecordKey sKey;
    string strOutput;

	leveldb::WriteOptions writeOptions;
	leveldb::Slice sysKey((char *)&sKey, sizeof(sKey));
	
	if (pData.SerializeToString(&strOutput) != true) {
		return false;
	}

	leveldb::Slice sysValue(strOutput);
	m_pDb->Put(writeOptions, sysKey, sysValue);

	return true;
}

bool ConfDB::GetCameraListConf(VidCameraList &pData)
{
	std::lock_guard<std::mutex> lock(m_cMutex);
	
	VSCConfCameraKey sKey;
    string strValue;

	leveldb::Slice key((char *)&sKey, sizeof(sKey));

	leveldb::Status Status = m_pDb->Get(leveldb::ReadOptions(), key, &strValue);
	if (!Status.ok()) {
		strValue = "fake";
	}

	if (pData.ParseFromString(strValue) == false) {
		VidCameraList listDefault;
		pData = listDefault;
		VDC_DEBUG( "Camera List Config is not init\n");
		return true;
	}
	return true;
}
bool ConfDB::UpdateCameraListConf(VidCameraList &pData)
{
	std::lock_guard<std::mutex> lock(m_cMutex);

	VSCConfCameraKey sKey;
	leveldb::WriteOptions writeOptions;
    string strOutput;

	leveldb::Slice sysKey((char *)&sKey, sizeof(sKey));
	if (pData.SerializeToString(&strOutput) != true) {
		return false;
	}

	leveldb::Slice sysValue(strOutput);
	m_pDb->Put(writeOptions, sysKey, sysValue);

	return true;
}

bool ConfDB::GetStorServerConf(VidStorServerConf &pData)
{
    std::lock_guard<std::mutex> lock(m_cMutex);

	VSCConfStorServerKey sKey;
	string strValue;

	leveldb::Slice key((char *)&sKey, sizeof(sKey));
	leveldb::Status Status = m_pDb->Get(leveldb::ReadOptions(), key, &strValue);
	if (!Status.ok()) {
		strValue = "fake";
	}

	if (pData.ParseFromString(strValue) == false) {
		VSCStorServerConfDefault(pData);
		VDC_DEBUG( "Stor Server Config is not init\n");
		return false;
	}
	return true;
}
bool ConfDB::SetStorServerConf(VidStorServerConf &pData)
{
    std::lock_guard<std::mutex> lock(m_cMutex);

	VSCConfStorServerKey sKey;
    string strOutput;

	leveldb::WriteOptions writeOptions;
	leveldb::Slice sysKey((char *)&sKey, sizeof(sKey));

	if (pData.SerializeToString(&strOutput) != true) {
		return false;
	}

	leveldb::Slice sysValue(strOutput);
	m_pDb->Put(writeOptions, sysKey, sysValue);

	return true;
}

bool ConfDB::GetRecSched(string strId, RecordSchedWeek &pSched)
{
	std::lock_guard<std::mutex> lock(m_cMutex);

	if (strId == REC_SCHED_OFF) {
		pSched = RecordSchedWeek::CreateOff();
		return true;
	}
	if (strId == REC_SCHED_ALL_DAY) {
		pSched = RecordSchedWeek::CreateAllDay();
		return true;
	}
	if (strId == REC_SCHED_WORK_DAY) {
		pSched = RecordSchedWeek::CreateWorkDay();
		return true;
	}
	return false;
}

#if 0
bool ConfDB::GetSystemConf(VSCConfData &pSys)
{
    VSCConfSystemKey sSysKey;

    leveldb::Slice key((char *)&sSysKey, sizeof(sSysKey));


    leveldb::Iterator* it = m_pDb->NewIterator(leveldb::ReadOptions());

    it->Seek(key);
    leveldb::Slice sysValue;

    if (it->Valid())
    {
        sysValue = it->value();
    }

    if (sysValue.size() != sizeof(VSCConfData))
    {
        VDC_DEBUG( "System Config is not init\n");
        delete it;
        memset(&pSys, 0, sizeof(VSCConfData));
        SysConfDataDefault(pSys);
        UpdateSysData(pSys);
        string strLicense = " ";
        SetLicense(strLicense);//set the default license
        /* Call get system again */
        return true;
    }

    memcpy(&pSys, sysValue.data(), sizeof(VSCConfData));
	if (pSys.data.conf.RTSPServerPort == 0)
	{
		pSys.data.conf.RTSPServerPort = VSC_RTSP_DEFAULT_PORT;
	}
	if (pSys.data.conf.OAPIPort == 0)
	{
		pSys.data.conf.OAPIPort = VSC_OAPI_DEFAULT_PORT;
	}
	if (pSys.data.conf.VHTTPServerPort == 0)
	{
		pSys.data.conf.VHTTPServerPort = VSC_VHTTPS_DEFAULT_PORT;
	}
	if (pSys.data.conf.VHLSServerPort == 0)
	{
		pSys.data.conf.VHLSServerPort = VSC_VHLSS_DEFAULT_PORT;
	}
	if (pSys.data.conf.VHTTPSSLServerPort == 0)
	{
		pSys.data.conf.VHTTPSSLServerPort = VSC_VHTTPS_SSL_DEFAULT_PORT;
	}

    // Check for any errors found during the scan
    assert(it->status().ok());
    delete it;

    return true;

}

bool ConfDB::GetVmsConf(VSCVmsData &pVms)
{
    VSCConfVmsKey sVmsKey;

    leveldb::Slice key((char *)&sVmsKey, sizeof(sVmsKey));


    leveldb::Iterator* it = m_pDb->NewIterator(leveldb::ReadOptions());

    it->Seek(key);
    leveldb::Slice sysValue;

    if (it->Valid())
    {
        sysValue = it->value();
    }

    if (sysValue.size() != sizeof(VSCVmsData))
    {
        VDC_DEBUG( "VMS Config is not init\n");
        delete it;
        memset(&pVms, 0, sizeof(VSCVmsData));
        VmsConfDataDefault(pVms);
        UpdateVmsData(pVms);
        /* Call get system again */
        return true;
    }

    memcpy(&pVms, sysValue.data(), sizeof(VSCVmsData));

    // Check for any errors found during the scan
    assert(it->status().ok());
    delete it;

    return true;

}

bool ConfDB::GetViewConf(VSCViewData &pView)
{
    VSCConfViewKey sViewKey;

    leveldb::Slice key((char *)&sViewKey, sizeof(sViewKey));


    leveldb::Iterator* it = m_pDb->NewIterator(leveldb::ReadOptions());

    it->Seek(key);
    leveldb::Slice sysValue;

    if (it->Valid())
    {
        sysValue = it->value();
    }

    if (sysValue.size() != sizeof(VSCViewData))
    {
        VDC_DEBUG( "View Config is not init\n");
        delete it;
        memset(&pView, 0, sizeof(VSCViewData));
        ViewConfDataDefault(pView);
        UpdateViewData(pView);
        /* Call get system again */
        return true;
    }

    memcpy(&pView, sysValue.data(), sizeof(VSCViewData));

    // Check for any errors found during the scan
    assert(it->status().ok());
    delete it;

    return true;

}


bool ConfDB::GetVGroupConf(VSCVGroupData &pVGroup)
{
    VSCConfVGroupKey sVGroupKey;

    leveldb::Slice key((char *)&sVGroupKey, sizeof(sVGroupKey));


    leveldb::Iterator* it = m_pDb->NewIterator(leveldb::ReadOptions());

    it->Seek(key);
    leveldb::Slice sysValue;

    if (it->Valid())
    {
        sysValue = it->value();
    }

    if (sysValue.size() != sizeof(VSCVGroupData))
    {
        VDC_DEBUG( "Group Config is not init\n");
        delete it;
		memset(&pVGroup, 0, sizeof(VSCVGroupData));
		VGroupConfDataDefault(pVGroup);
		UpdateVGroupData(pVGroup);
        /* Call get system again */
        return true;
    }

	memcpy(&pVGroup, sysValue.data(), sizeof(VSCVGroupData));

    // Check for any errors found during the scan
    assert(it->status().ok());
    delete it;

    return true;

}

bool ConfDB::GetUserConf(VSCUserData &pData)
{
    VSCConfUserKey sKey;

    leveldb::Slice key((char *)&sKey, sizeof(sKey));


    leveldb::Iterator* it = m_pDb->NewIterator(leveldb::ReadOptions());

    it->Seek(key);
    leveldb::Slice sysValue;

    if (it->Valid())
    {
        sysValue = it->value();
    }

    if (sysValue.size() != sizeof(VSCUserData))
    {
        VDC_DEBUG( "User Config is not init\n");
        delete it;
	memset(&pData, 0, sizeof(VSCUserData));
	VSCUserDataItemDefault(pData.data.conf);
	UpdateUserData(pData);
        /* Call get system again */
        return true;
    }

	memcpy(&pData, sysValue.data(), sizeof(VSCUserData));

    // Check for any errors found during the scan
    assert(it->status().ok());
    delete it;

    return true;

}

bool ConfDB::GetTourConf(VSCTourData &pData)
{
    VSCConfTourKey sKey;

    leveldb::Slice key((char *)&sKey, sizeof(sKey));


    leveldb::Iterator* it = m_pDb->NewIterator(leveldb::ReadOptions());

    it->Seek(key);
    leveldb::Slice sysValue;

    if (it->Valid())
    {
        sysValue = it->value();
    }

    if (sysValue.size() != sizeof(VSCTourData))
    {
        VDC_DEBUG( "Tour Config is not init\n");
        delete it;
	memset(&pData, 0, sizeof(VSCTourData));
	VSCTourDataDefault(pData.data.conf);
	UpdateTourData(pData);
        /* Call get system again */
        return true;
    }

	memcpy(&pData, sysValue.data(), sizeof(VSCTourData));

    // Check for any errors found during the scan
    assert(it->status().ok());
    delete it;

    return true;

}


bool ConfDB::GetEmapConf(VSCEmapData &pData)
{
    VSCConfEmapKey sKey;

    leveldb::Slice key((char *)&sKey, sizeof(sKey));


    leveldb::Iterator* it = m_pDb->NewIterator(leveldb::ReadOptions());

    it->Seek(key);
    leveldb::Slice sysValue;

    if (it->Valid())
    {
        sysValue = it->value();
    }

    if (sysValue.size() != sizeof(VSCEmapData))
    {
        VDC_DEBUG( "User Config is not init\n");
        delete it;
	memset(&pData, 0, sizeof(VSCEmapData));
	VSCEmapDataDefault(pData.data.conf);
	UpdateEmapData(pData);
        /* Call get system again */
        return true;
    }

	memcpy(&pData, sysValue.data(), sizeof(VSCEmapData));

    // Check for any errors found during the scan
    assert(it->status().ok());
    delete it;

    return true;

}


s32 ConfDB::UpdateSysData(VSCConfData &pSysData)
{
    VSCConfSystemKey sSysKey;

    leveldb::WriteOptions writeOptions;

    leveldb::Slice sysKey((char *)&sSysKey, sizeof(sSysKey));
    leveldb::Slice sysValue((char *)&pSysData, sizeof(VSCConfData));

    m_pDb->Put(writeOptions, sysKey, sysValue);

    return true;
}

s32 ConfDB::GetSysData(VSCConfData &pSysData)
{
    GetSystemConf(pSysData);

    return true;
}

bool ConfDB::UpdateDeviceData(u32 nId, VSCDeviceData &pData)
{
    VSCConfDeviceKey sChKey(nId);
    leveldb::WriteOptions writeOptions;

    leveldb::Slice chKey((char *)&sChKey, sizeof(sChKey));
    leveldb::Slice chValue((char *)&pData, sizeof(VSCDeviceData));
    m_pDb->Put(writeOptions, chKey, chValue);

    return true;
}

bool ConfDB::GetDeviceData(u32 nId, VSCDeviceData &pData)
{
    VSCConfDeviceKey sChKey(nId);

    leveldb::Slice key((char *)&sChKey, sizeof(sChKey));


    leveldb::Iterator* it = m_pDb->NewIterator(leveldb::ReadOptions());

    it->Seek(key);
    leveldb::Slice sysValue;

    if (it->Valid())
    {
        sysValue = it->value();
    }

    if (sysValue.size() != sizeof(VSCDeviceData))
    {
        VDC_DEBUG( "Device Can not find !!!\n");

        delete it;
        return false;
    }

    memcpy(&pData, sysValue.data(), sizeof(VSCDeviceData));

    // Check for any errors found during the scan
    assert(it->status().ok());
    delete it;

    return true;
}

bool ConfDB::UpdateVIPCData(u32 nId, VSCVIPCData &pData)
{
    VSCConfVIPCKey sChKey(nId);
    leveldb::WriteOptions writeOptions;

    leveldb::Slice chKey((char *)&sChKey, sizeof(sChKey));
    leveldb::Slice chValue((char *)&pData, sizeof(VSCDeviceData));
    m_pDb->Put(writeOptions, chKey, chValue);

    return true;
}

bool ConfDB::GetVIPCData(u32 nId, VSCVIPCData &pData)
{
    VSCConfVIPCKey sChKey(nId);

    leveldb::Slice key((char *)&sChKey, sizeof(sChKey));


    leveldb::Iterator* it = m_pDb->NewIterator(leveldb::ReadOptions());

    it->Seek(key);
    leveldb::Slice sysValue;

    if (it->Valid())
    {
        sysValue = it->value();
    }

    if (sysValue.size() != sizeof(VSCVIPCData))
    {
        VDC_DEBUG( "Device Can not find !!!\n");

        delete it;
        return false;
    }

    memcpy(&pData, sysValue.data(), sizeof(VSCVIPCData));

    // Check for any errors found during the scan
    assert(it->status().ok());
    delete it;

    return true;
}



s32 ConfDB::GetVmsData(VSCVmsData &pVmsData)
{
	GetVmsConf(pVmsData);
	
	return true;
}
s32 ConfDB::UpdateVmsData(VSCVmsData &pVmsData)
{
    VSCConfVmsKey sVmsKey;

    leveldb::WriteOptions writeOptions;

    leveldb::Slice sysKey((char *)&sVmsKey, sizeof(sVmsKey));
    leveldb::Slice sysValue((char *)&pVmsData, sizeof(VSCVmsData));

    m_pDb->Put(writeOptions, sysKey, sysValue);

    return true;
}

s32 ConfDB::GetViewData(VSCViewData &pViewData)
{
	GetViewConf(pViewData);
	
	return true;
}
s32 ConfDB::UpdateViewData(VSCViewData &pViewData)
{
    VSCConfViewKey sViewKey;

    leveldb::WriteOptions writeOptions;

    leveldb::Slice sysKey((char *)&sViewKey, sizeof(sViewKey));
    leveldb::Slice sysValue((char *)&pViewData, sizeof(VSCViewData));

    m_pDb->Put(writeOptions, sysKey, sysValue);

    return true;
}

/* Camera Group  */
s32 ConfDB::GetVGroupData(VSCVGroupData &pGroupData)
{
	GetVGroupConf(pGroupData);
	
	return true;
}
/* Camera Group  */
s32 ConfDB::UpdateVGroupData(VSCVGroupData &pVGroupData)
{
    VSCConfVGroupKey sVGroupKey;

    leveldb::WriteOptions writeOptions;

    leveldb::Slice sysKey((char *)&sVGroupKey, sizeof(sVGroupKey));
    leveldb::Slice sysValue((char *)&pVGroupData, sizeof(VSCVGroupData));

    m_pDb->Put(writeOptions, sysKey, sysValue);

    return true;
}

/* HDFS record  */
s32 ConfDB::GetHdfsRecordData(VSCHdfsRecordData &pData)
{
	GetHdfsRecordConf(pData);
	
	return true;
}

/* User  */
s32 ConfDB::GetUserData(VSCUserData &pData)
{
	GetUserConf(pData);
	
	return true;
}

s32 ConfDB::UpdateUserData(VSCUserData &pData)
{
    VSCConfUserKey sKey;

    leveldb::WriteOptions writeOptions;

    leveldb::Slice sysKey((char *)&sKey, sizeof(sKey));
    leveldb::Slice sysValue((char *)&pData, sizeof(VSCUserData));

    m_pDb->Put(writeOptions, sysKey, sysValue);

    return true;
}

/* Tour  */
s32 ConfDB::GetTourData(VSCTourData &pData)
{
	GetTourConf(pData);
	
	return true;
}

s32 ConfDB::UpdateTourData(VSCTourData &pData)
{
    VSCConfTourKey sKey;

    leveldb::WriteOptions writeOptions;

    leveldb::Slice sysKey((char *)&sKey, sizeof(sKey));
    leveldb::Slice sysValue((char *)&pData, sizeof(VSCTourData));

    m_pDb->Put(writeOptions, sysKey, sysValue);

    return true;
}


/* Emap */
s32 ConfDB::GetEmapData(VSCEmapData &pData)
{
	GetEmapConf(pData);
	
	return true;
}
s32 ConfDB::UpdateEmapData(VSCEmapData &pData)
{
    VSCConfEmapKey sKey;

    leveldb::WriteOptions writeOptions;

    leveldb::Slice sysKey((char *)&sKey, sizeof(sKey));
    leveldb::Slice sysValue((char *)&pData, sizeof(VSCEmapData));

    m_pDb->Put(writeOptions, sysKey, sysValue);

    return true;
}

/* Emap file Get & Set */
  bool ConfDB::GetEmapFile(string &strFile)
{
	bool ret = false;
	VSCConfEmapFileKey sMapKey;
	

	leveldb::Slice key((char *)&sMapKey, sizeof(sMapKey));


	leveldb::Iterator* it = m_pDb->NewIterator(leveldb::ReadOptions());

	it->Seek(key);
	leveldb::Slice sysValue;

	if (it->Valid())
	{
		sysValue = it->value();
		strFile = sysValue.ToString();
		ret = true;
	}
	
	// Check for any errors found during the scan
	assert(it->status().ok());
	delete it;

	return ret;

}

  bool ConfDB::SetEmapFile(string &strFile)
{
	VSCConfEmapFileKey sMapKey;
	leveldb::WriteOptions writeOptions;

	leveldb::Slice Key((char *)&sMapKey, sizeof(sMapKey));
	leveldb::Slice Value(strFile);
	m_pDb->Put(writeOptions, Key, Value);
	return true;
    
}

s32 ConfDB::AddDevice(VSCDeviceData &pData, u32 nId)
{
    if (nId > CONF_MAP_MAX)
    {
        return false;
    }
    VSCConfData SysData;
    GetSysData(SysData);

    SysData.data.conf.DeviceMap[nId] = nId;
    SysData.data.conf.DeviceNum ++;
    pData.data.conf.nId = nId;

    UpdateSysData(SysData);

    UpdateDeviceData(nId, pData);


    return true;
}

s32 ConfDB::DelDevice(u32 nId)
{
    if (nId > CONF_MAP_MAX)
    {
        return false;
    }

    VSCConfData SysData;
    GetSysData(SysData);

    SysData.data.conf.DeviceMap[nId] = CONF_MAP_INVALID_MIN;

    UpdateSysData(SysData);

    return true;
}

s32 ConfDB::AddVIPC(VSCVIPCData &pData, u32 nId)
{
    if (nId > CONF_MAP_MAX)
    {
        return false;
    }
    VSCConfData SysData;
    GetSysData(SysData);

    SysData.data.conf.VIPCMap[nId] = nId;
    SysData.data.conf.VIPCNum ++;
    pData.data.conf.nId = nId;

    UpdateSysData(SysData);

    UpdateVIPCData(nId, pData);


    return true;
}

s32 ConfDB::DelVIPC(u32 nId)
{
    if (nId > CONF_MAP_MAX)
    {
        return false;
    }

    VSCConfData SysData;
    GetSysData(SysData);

    SysData.data.conf.VIPCMap[nId] = CONF_MAP_INVALID_MIN;

    UpdateSysData(SysData);

    return true;
}

#endif
