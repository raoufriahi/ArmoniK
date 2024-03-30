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
#ifndef VSC_EVENT_H
#define VSC_EVENT_H

#include "utility/utility.hpp"
#include "avplayer/include/factory.h"
#include "QEvent"
#include "onvifclient.hpp"
#include "vevent.hpp"

using namespace std;

class OnvifClientCam
{
public:
	OnvifClientCam()
	:onvifDevice(NULL), onvifEvent(NULL)
	{
		
	}
	~OnvifClientCam()
	{
		if (onvifDevice)
		{
			delete onvifDevice;
			onvifDevice = NULL;
		}
		if (onvifEvent)
		{
			delete onvifEvent;
			onvifEvent = NULL;
		}
	}
public:
	OnvifClientDevice *onvifDevice;
	OnvifClientEvent *onvifEvent;
	string ip;
	string onvifUrl;
	string eventNotify;
	string deviceName;
	string id;
	bool servermotion;
	time_t lastGotTime;
};

typedef std::map<string, OnvifClientCam*> OnvifMap;

class VOnvifEventThread : public QThread
{
    Q_OBJECT
public:
	VOnvifEventThread(Factory &pFactory, VEventServer &pEventServer)
	 :m_EventId(0), m_OnvifNotify(), 
	 m_pFactory(pFactory), m_pEventServer(pEventServer)
	{
	}
	~VOnvifEventThread()
	{
	}
public:
	void run();

	bool UpdateDeviceMap();
public:
	static bool OnvifEventCallbackFunc(void* pData, OnvifEvent& event);
	bool OnvifEventCallbackFunc1(OnvifEvent& event);

	static bool DeviceChangeCallbackFunc(void* pData, FactoryCameraChangeData change);
	bool DeviceChangeCallbackFunc1(FactoryCameraChangeData change);
public:
	bool InitDevices();
	bool RenewDevices();
	bool InitOneDevice(VidCamera  pParam);
	bool DeleteOneDevice(astring strId);
public:
	void Lock(){m_Lock.lock();}
	bool TryLock(){return m_Lock.try_lock();}
	void UnLock(){m_Lock.unlock();}

private:
	OnvifMap m_OnvifClientMap;
	VidCameraList m_DeviceMap;
	fast_mutex m_Lock;
	s64 m_EventId;
	OnvifClientEventNotify m_OnvifNotify;
	Factory &m_pFactory;
	VEventServer &m_pEventServer;
};

#endif // VSC_EVENT_H
