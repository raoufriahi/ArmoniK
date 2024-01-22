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
#include "videotype.hpp"
#include <QWidget>
#include "server/factory.hpp"
#include <QThread>
#include "vonvifdismgr.hpp"

using namespace UtilityLib;

typedef std::map<int, QThread *> VIPCThreadMap;

class UB_LIBRARY_API VVIPCMgr
{
public:
	VVIPCMgr(Factory & pFactory, VONVIFDisMgr & pDisMgr);
	~VVIPCMgr();

	static bool DeviceChangeCallbackFunc(void* pData, 
						FactoryDeviceChangeData change);
	bool DeviceChangeCallbackFunc1(FactoryDeviceChangeData change);

	void Lock(){m_Lock.lock();}
	void UnLock(){m_Lock.unlock();}
private:
    Factory & m_pFactory;
	VONVIFDisMgr & m_pDisMgr;
    VIPCThreadMap m_ThreadMap;
	fast_mutex m_Lock;
};

