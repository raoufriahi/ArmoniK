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

#include "leveldb/db.h"
#include "config/conf.hpp"
#include "utility/debug.hpp"
#include "config/vidconf.pb.h"
//RRI #include "simplecrypt.hpp"
#include "vdb/recordwrapper.hpp"


class VE_LIBRARY_API ConfDB
{
public:
	ConfDB()   { }
	~ConfDB()  { }

	bool  Open(string  pPath);

	bool CameraRecordTemplSet(string strCameraId, string strTempl);
	bool FindCamera(string strCameraId);
	bool DeleteCamera(string strCameraId);
	bool AddCamera(VidCamera &pAddCam);
	bool GetCameraConf(string strCameraId, VidCamera &pCam);

	bool GetHdfsRecordConf(VidHDFSConf &pData);
	bool UpdateHdfsRecordConf(VidHDFSConf &pData);

	bool GetCameraListConf(VidCameraList &pData);
	bool UpdateCameraListConf(VidCameraList &pData);

	bool GetLicense(string &strLicense);
	bool SetLicense(string &strLicense);

	bool GetCmnParam(string &strKey, string &strParam);
	bool SetCmnParam(string &strKey, string &strParam);

	bool GetStorServerConf(VidStorServerConf &pData);
	bool SetStorServerConf(VidStorServerConf &pData);

	bool GetRecSched(string strId, RecordSchedWeek &pSched);

private:
    std::mutex m_cMutex;
    leveldb::DB* m_pDb;
    leveldb::Options m_Options;
};

typedef ConfDB* LPConfDB;

