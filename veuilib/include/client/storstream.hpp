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
#ifndef __VSC_STOR_STREAM_H__
#define __VSC_STOR_STREAM_H__
#include "config/confdb.hpp"
#include "server/camera.hpp"
#include "vdb.hpp"
#include "vhdfsdb.hpp"
#include "vplay.hpp"
#include "config/sysdb.hpp"
#include "server/hdddevice.hpp"
#include <QThread>
#include <qdebug.h>
#include "Poco/Path.h"
#include "Poco/File.h"
#include "config/vidconf.pb.h"
#include "server/camera.hpp"

#include "cppkit/ck_string.h"
#include "cppkit/os/ck_sprintf.h"
#include "client/storwsclient.hpp"

using namespace cppkit;
using namespace std;

class VE_LIBRARY_API StorStream: public QObject, public StorWebSocketClient
{
	Q_OBJECT
public:
	StorStream(VidStor &stor, astring strId, unsigned int nStream, 
		bool bPlayback = false, u32 nPlaytime = 0, bool bHWAccel = false);
	~StorStream();
public:
	virtual bool ProcessRecvMsg(char *data, size_t data_len);
	virtual bool IsKeep();
	virtual bool ProcessOnline();
	virtual bool ProcessLogined();
	virtual bool NeedReconnect();
public:
#ifdef WIN32
	bool AttachWidget(HWND hWnd, int w, int h, RenderType render = RENDER_TYPE_D3D);
#else
	bool AttachWidget(HWND hWnd, int w, int h, RenderType render = RENDER_TYPE_SDL);
#endif
	bool EnableMot(HWND hWnd, bool bEnable, astring strConf);
	bool UpdateWidget(HWND hWnd, int w, int h);
	bool DetachWidget(HWND hWnd);
	bool GetStreamInfo(VideoStreamInfo &pInfo);

	bool RegDataCallback(CameraDataCallbackFunctionPtr pCallback, void * pParam);
	bool UnRegDataCallback();

public:
	bool PausePlayback();
	bool ResumePlayback();
	bool SeekPlayback(u32 nPlaytime);

signals:
	void SignalPlayTime(unsigned int nTime);

public:
	bool StartStorStream();
	bool StopStorStream();
	
private:
	XMutex m_cMutex;
	
private:
	VPlay m_play;
	astring m_strId;
	unsigned int m_nStream;
	VidStor m_stor;
	unsigned int m_nLastTime;
	
	CameraDataCallbackFunctionPtr m_pCallback; 
	void * m_pParam;

private:
	bool m_bPlayback;
	u32 m_nPlaytime;
	bool m_bPbPause;
	s64 m_nRecvSize;
	u32 m_nNoFrameCnt;
	s64 m_nRecvSizeLast;
};

typedef StorStream* LPStorStream;

#endif
