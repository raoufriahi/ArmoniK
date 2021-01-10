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
#include "vtaskmgr.hpp"

VTaskMgr * g_VTaskMgr = NULL;

class VTaskMgrImpl 
{
public:
	VTaskMgrImpl();
	~VTaskMgrImpl();
public:
	bool GetTaskList(VTaskItemList &pList);
	bool AddTask(VTaskItem *pTask);
	bool DeleteTask(VTaskItem *pTask);
	int GetTaskCnt();
	void Lock(){m_Lock.lock();}
	void UnLock(){m_Lock.unlock();}
	
private:
	VTaskItemList m_list;
	fast_mutex m_Lock;
	int m_TaskCnt;
};

VTaskMgrImpl::VTaskMgrImpl()
:m_TaskCnt(0)
{

}

VTaskMgrImpl::~VTaskMgrImpl()
{

}

bool VTaskMgrImpl::GetTaskList(VTaskItemList &pList)
{
	Lock();
	
	pList = m_list;
	UnLock();
	return TRUE;
}


bool VTaskMgrImpl::AddTask(VTaskItem *pTask)
{
	if (pTask == NULL)
	{
		return FALSE;
	}
	
	Lock();
	m_TaskCnt ++;
	m_list[pTask->GetId()] = pTask;
	UnLock();
	return TRUE;
}

bool VTaskMgrImpl::DeleteTask(VTaskItem *pTask)
{

	if (pTask == NULL)
	{
		return FALSE;
	}
	
	Lock();

	m_TaskCnt --;
	if (m_TaskCnt <= 0)
	{
		m_TaskCnt = 0;
	}

	m_list.erase(pTask->GetId());

	pTask->StopTask();

	pTask->wait();

	UnLock();
	return TRUE;
}

int VTaskMgrImpl::GetTaskCnt()
{
	int cnt = 0;
	Lock();

	cnt = m_TaskCnt;

	UnLock();
	return cnt;
}

VTaskMgr::VTaskMgr()
:m_pImpl(NULL)
{
	m_pImpl = new VTaskMgrImpl();
	if (g_VTaskMgr == NULL)
	{
		g_VTaskMgr = this;
	}
}

VTaskMgr::~VTaskMgr()
{

}

bool VTaskMgr::GetTaskList(VTaskItemList &pList)
{
	if (g_VTaskMgr)
	{
		return g_VTaskMgr->m_pImpl->GetTaskList(pList);
	}
	return false;
}
bool VTaskMgr::AddTask(VTaskItem *pTask)
{
	if (g_VTaskMgr)
	{
		return g_VTaskMgr->m_pImpl->AddTask(pTask);
	}
	return false;
}
bool VTaskMgr::DeleteTask(VTaskItem *pTask)
{
	if (g_VTaskMgr)
	{
		return g_VTaskMgr->m_pImpl->DeleteTask(pTask);
	}
	return false;
}

int VTaskMgr::GetTaskCnt()
{
	if (g_VTaskMgr)
	{
		return g_VTaskMgr->m_pImpl->GetTaskCnt();
	}
	return false;
}

