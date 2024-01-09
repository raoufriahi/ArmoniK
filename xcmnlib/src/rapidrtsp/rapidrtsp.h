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

#include "ros.h"
#include <thread>
#include <mutex>
#include <chrono>
#include "Poco/URI.h"
#include "Poco/String.h"
#include "h5slib.hpp"
using namespace std; 
#ifdef __cplusplus
extern "C"
{
#endif
#include <libavformat/avformat.h>

#ifdef __cplusplus
}
#endif

/*
* rapidrtsp result of call ...
*/
#define RapidRTSP_OK					(0)
#define RapidRTSP_FAIL					(-1)
#define RapidRTSP_INVALID_PARAM			(-2)
#define RapidRTSP_NOT_ENOUGH_MEM		(-3)


/*
* rtsp transport mode
*/
enum {
	RapidRTSP_TRANSPORT_AUTO = 0, // try rtp-over-rtsp first
	RapidRTSP_TRANSPORT_UDP,
	RapidRTSP_TRANSPORT_OVER_RTSP,
	RapidRTSP_TRANSPORT_OVER_HTTP,
	RapidRTSP_TRANSPORT_OVER_HTTPS,
};

typedef enum __RapidRTSPState {
	RapidRTSP_INIT = 0,
	RapidRTSP_CONNECTING,
	RapidRTSP_CONNECTED,
}RapidRTSPState;

#define RAPID_MAX_EXTRA_SIZE 1024 * 10
typedef struct rapidrtsp_media_info
{
	union
	{
		struct{
			int width, height, fps, bps;
			int extradataSize;
			unsigned char extradata[RAPID_MAX_EXTRA_SIZE];
		}video;		
		struct{
			int sampleRate, sampleSize, channel;
		}audio;
	};
	CodecType codec;
}CRapidRTSPMediaInfo;

typedef struct rapidRTSPAVInfo
{
	CRapidRTSPMediaInfo video;
	CRapidRTSPMediaInfo audio;
	/* TODO add more ... meta data */
}CRapidRTSPAVInfo;


typedef void (*fRapidRTSP_DATA_HANDLE)(void *pdata, unsigned int dataSize, 
	unsigned int sec, unsigned int msec,
	int dataType, VideoStreamType streamType, VideoFrameType frameType,
	unsigned int flag, CRapidRTSPAVInfo &pAVInfo, void *context);

class CRapidRTSP
{
public:
	CRapidRTSP(string streamUrl, int transport, string userName, string userPwd, bool bEnableAudio);
	virtual ~CRapidRTSP();
	void set_data_handle(fRapidRTSP_DATA_HANDLE handle, void *context);
	virtual int start(){return true;}

protected:
	string m_userName;
	string m_userPwd;
	string m_streamUrl;
	string m_streamFullUrl; /* the full url will include the user & password */
	bool m_bEnableAudio;
	fRapidRTSP_DATA_HANDLE m_dataHandle;
	void *m_dataContext;
	VSCConnectType m_connectType;
};
	
class CRapidRTSPFFMPEG :public CRapidRTSP
{
public:
	CRapidRTSPFFMPEG(string streamUrl, int transport, string userName, string userPwd, bool bEnableAudio);
	~CRapidRTSPFFMPEG();

	static int  CheckInterruptCallback(void *param);
	int CheckInterruptCallback1();

	static void  proc(void *param);
	void proc1();
	virtual int start();
private:
	
	std::thread *m_pThread;
	CRapidRTSPAVInfo m_AVinfo;
	AVFormatContext *m_pContext;
	RapidRTSPState m_nState;
	
	struct timeval m_currVidTime;
	long m_LastVidPts;
	long m_nStartConnectTime;
	unsigned int m_VidFlag; 
	bool m_bExit;
	double m_ConnectingStart;
	double m_nLastGetDataTime; /* Last receive time */
};

class CRapidRTSPLive555 :public CRapidRTSP , public H5SCallback
{
public:
	CRapidRTSPLive555(string streamUrl, int transport, string userName, string userPwd, bool bEnableAudio);
	~CRapidRTSPLive555();

	virtual int start();
	virtual bool   onH5SData(unsigned char* buffer, int size, unsigned long long secs, 
			unsigned long long msecs, H5SCodecType codec, H5SStreamType stream, 
			H5SFrameType frm);
	static bool WatchThread(void* pData);
	bool WatchThreadRun();
	bool CheckRTSPClient();
private:
	H5SLibRTSP *m_rtsp;
	CRapidRTSPAVInfo m_AVinfo;
	std::mutex  m_Mutex;
	std::mutex  m_MutexData;
	long m_lastRecv;
	long m_currRecv;
	bool m_bExit ;
	bool m_bStarted;
	std::thread * m_pWatchThread;

};

typedef struct CRapidRTSP stCRapidRTSP, *lpCRapidRTSP;
