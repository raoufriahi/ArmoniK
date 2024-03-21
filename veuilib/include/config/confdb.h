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

#include "utility.hpp"
#include "leveldb/db.h"
#include "config/conf.hpp"
#include "debug.hpp"

#include "config/vidconf.pb.h"

#include "XSDK/XMutex.h"
#include "XSDK/XGuard.h"
#include "simplecrypt.hpp"
#include "recordwrapper.hpp"

using namespace UtilityLib;
using namespace XSDK;


class VE_LIBRARY_API ConfDB
{
public:
	ConfDB()
	{

	}
	~ConfDB()
	{

	}

	s32 Open(astring  pPath);
public:
	bool CameraRecordTemplSet(astring strCameraId, astring strTempl);
	bool FindCamera(astring strCameraId);
	bool DeleteCamera(astring strCameraId);
	bool AddCamera(VidCamera &pAddCam);
	bool GetCameraConf(astring strCameraId, VidCamera &pCam);
public:

	BOOL GetHdfsRecordConf(VidHDFSConf &pData);
	BOOL UpdateHdfsRecordConf(VidHDFSConf &pData);

	BOOL GetCameraListConf(VidCameraList &pData);
	BOOL UpdateCameraListConf(VidCameraList &pData);

	BOOL GetLicense(astring &strLicense);
	BOOL SetLicense(astring &strLicense);



	BOOL GetCmnParam(astring &strKey, astring &strParam);
	BOOL SetCmnParam(astring &strKey, astring &strParam);

	bool GetStorServerConf(VidStorServerConf &pData);
	bool SetStorServerConf(VidStorServerConf &pData);
public:
	bool GetRecSched(astring strId, RecordSchedWeek &pSched);

private:
    XMutex m_cMutex;

private:
    leveldb::DB* m_pDb;
    leveldb::Options m_Options;
};

typedef ConfDB* LPConfDB;

