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
#ifndef __V_SMART_MOTION_EVENT_H
#define __V_SMART_MOTION_EVENT_H

#include <QWidget>
#include "utility.hpp"
#include "server/factory.hpp"
#include "QEvent"
#include <QThread>
#include "vevent.hpp"
#include "vsmotalgo.hpp"

/* Dynamics FPS need get from here, and it the dfps stream, may the stream3
   the stream is only for relay, because the the record system can only record the motion video
   The Dynamics stream can be useful for mobile client*/

class SmartMotionCam
{
public:
	SmartMotionCam(string strId, int nStreamId, astring strName, astring strMotReg,
		Factory &pFactory, VEventServer &pEventServer);
	~SmartMotionCam();
public:
	void DataHandler1(VideoFrame& frame);
	static void DataHandler(VideoFrame& frame, void * pParam);	
	void RawHandler1(RawFrame& frame);
	static void RawHandler(void* pData, RawFrame& frame);	
private:
	string m_strId;
	string m_strName;
	int m_nStreamId;
	fast_mutex m_Lock;
	Factory &m_pFactory;
	VPlay m_vPlay;
	VSMotAlgo m_MotAlgo;
	VEventServer &m_pEventServer;
	s64 m_nLastMotTime; 
	VVidMotReg m_motReg;

};

typedef std::map<string, SmartMotionCam*> SmartMotionCamMap;

class VSmartMotionThread : public QThread
{
    Q_OBJECT
public:
    VSmartMotionThread(Factory &pFactory, VEventServer &pEventServer)
	:m_pFactory(pFactory), m_pEventServer(pEventServer)
    {
    }
    ~VSmartMotionThread()
    {
    }
public:

	static bool DeviceChangeCallbackFunc(void* pData, FactoryCameraChangeData change);
	bool DeviceChangeCallbackFunc1(FactoryCameraChangeData change);
public:
	void run();
	bool InitDevices();
	bool InitOneDevice(VidCamera  pParam);
	bool DeleteOneDevice(astring strId);
	
public:
	void Lock(){m_Lock.lock();}
	bool TryLock(){return m_Lock.try_lock();}
	void UnLock(){m_Lock.unlock();}
	
private:
	fast_mutex m_Lock;
	Factory &m_pFactory;
	SmartMotionCamMap m_SmartMotionMap;
	VEventServer &m_pEventServer;
};

#endif /* __V_SMART_MOTION_EVENT_H */
