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
#include "debug.hpp"
#include "utility/videotype.hpp"

using namespace UtilityLib;

class HdfsRecSession;
class VHdfsDBData;
class HdfsRecWrapper;
class VE_LIBRARY_API VHdfsDB
{
public:
	typedef std::map<int, HdfsRecSession*> _MapSession;

	VHdfsDB(string &pNameNode, string &pPort, string &pUser);
	~VHdfsDB();

	void Lock();
	void UnLock();

	BOOL Config(string &pNameNode, string &pPort, string &pUser);

	/* Start and Stop Record */
	HdfsRecSession * StartRecord(int deviceId, string strName);
	BOOL FinishRecord(HdfsRecSession *pSess);

	HdfsRecWrapper & GetHdfsRecWrapper();
	BOOL ReleaseHdfsRecWrapper();
	int GetInerval() { return m_inerval; }
private:
    fast_mutex m_Lock;
	_MapSession m_MapSess;
    VHdfsDBData * m_data;
	string m_pNameNode;
	string m_pPort; 
	string m_pUser;
	int m_inerval; /*time in sec */
};
