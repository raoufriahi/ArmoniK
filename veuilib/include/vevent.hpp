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
#ifndef __V_EVENT_HPP__
#define __V_EVENT_HPP__
#include "utility.hpp"
#include "debug.hpp"
#include "videotype.hpp"
#include <QWidget>
#include "server/factory.hpp"
#include "server/eventserver.hpp"
#include <QThread>

using namespace UtilityLib;

class VOnvifEventThread;
class VSmartMotionThread;

class VE_LIBRARY_API VEventMgr
{
public:
	VEventMgr(Factory &pFactory, VEventServer &pEventServer)
	:m_pFactory(pFactory), m_pEventServer(pEventServer), m_OnvifThread(NULL), 
	m_SmartMotionThread(NULL)
	{
	}
	~VEventMgr()
	{
		
	}
public:
	bool Init();
private:
	VOnvifEventThread *m_OnvifThread;
	VSmartMotionThread *m_SmartMotionThread;
	Factory &m_pFactory;
	VEventServer &m_pEventServer;
};


#endif /* __V_EVENT_HPP__ */
