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
#ifndef __V_TASK_MGR_HPP__
#define __V_TASK_MGR_HPP__
#include "utility.hpp"
#include "debug.hpp"
#include "videotype.hpp"
#include <QWidget>
#include "server/factory.hpp"
#include <QThread>
#include "Poco/UUIDGenerator.h"

using namespace UtilityLib;

class VTaskMgrImpl;

class VE_LIBRARY_API VTaskItem : public QThread
{
	Q_OBJECT
public:
	VTaskItem()
	{
		Poco::UUIDGenerator uuidCreator;
		m_strId  = uuidCreator.createRandom().toString();
		/* The task will be auto deleted */
	}
	~VTaskItem(){}
public:
	/* the result is 0 to 100 */
	virtual int GetPercentage(){return 0;}
	virtual bool HasOpenResult(){return FALSE;}
	virtual bool OpenResult(){return FALSE;}
	virtual bool StopTask(){return FALSE;}
	virtual bool IsEol(){return FALSE;}
	virtual std::string GetTaskName(){return "none";}
	std::string GetId()
	{
		return m_strId;
	}
private:
	std::string m_strId;
};

typedef std::map<std::string, VTaskItem *> VTaskItemList;

class VE_LIBRARY_API VTaskMgr
{
public:
	VTaskMgr();
	~VTaskMgr();
public:
	static bool GetTaskList(VTaskItemList &pList);
	static bool AddTask(VTaskItem *pTask);
	static bool DeleteTask(VTaskItem *pTask);
	static int GetTaskCnt();
	
public:
	VTaskMgrImpl *m_pImpl;
};

#endif /* __V_TASK_MGR_HPP__ */
