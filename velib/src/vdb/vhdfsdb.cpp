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
#include "vhdfsdb.hpp"
#include "hdfswrapper.hpp"
#include "hdfsrecsession.hpp"

class VHdfsDBData
{
public:
	VHdfsDBData(VHdfsDB &pDB)
	:m_vDB(pDB), m_pHdfsWrapper(NULL), m_nWrapperCnt(0){

	}
	~VHdfsDBData()
	{

	}

public:
	VHdfsDB &m_vDB;
	HdfsRecWrapper *m_pHdfsWrapper;
	int m_nWrapperCnt;
};


VHdfsDB::VHdfsDB(string &pNameNode, string &pPort, string &pUser) 
     :m_pNameNode(pNameNode), m_pPort(pPort), m_pUser(pUser), m_inerval(30)
{
	m_data = new VHdfsDBData(*this);
	ck_string ckStrNameNode = pNameNode;
	ck_string ckStrPort = pPort;
	ck_string ckStrUser = pUser;
	m_data->m_pHdfsWrapper = new HdfsRecWrapper(ckStrNameNode, ckStrPort, ckStrUser);
}

VHdfsDB::~VHdfsDB()
{

}

void VHdfsDB::Lock()
{
	m_Lock.lock();
}

void VHdfsDB::UnLock()
{
	m_Lock.unlock();
}

BOOL VHdfsDB::Config(string &pNameNode, string &pPort, string &pUser)
{
	return TRUE;
}

HdfsRecWrapper & VHdfsDB::GetHdfsRecWrapper()
{
	Lock();
	m_data->m_nWrapperCnt ++;
	UnLock();

	return *(m_data->m_pHdfsWrapper);
}

BOOL VHdfsDB::ReleaseHdfsRecWrapper()
{
	Lock();
	m_data->m_nWrapperCnt --;
	UnLock();
	return TRUE;
}

HdfsRecSession * VHdfsDB::StartRecord(int deviceId, string strName)
{
	Lock();
    //TODO
	UnLock();	
	return new HdfsRecSession(deviceId, strName, *this);
}

BOOL VHdfsDB::FinishRecord(HdfsRecSession *pSess)
{
   	Lock();
	//TODO
	UnLock();
	return TRUE;
}
