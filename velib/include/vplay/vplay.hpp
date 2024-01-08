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

typedef BOOL (*VPlayDataHandler)(void* pData, VideoFrame& packet);
typedef BOOL (*VPlayPBTimeCallback)(void* pData, int time);

typedef BOOL (*VPlayRawFrameHandler)(void* pData, RawFrame& frame);

typedef enum __VPlayCmd
{
	VPLAY_CMD_PLAY = 1,
	VPLAY_CMD_PAUSE,
	VPLAY_CMD_SPEED,
	VPLAY_CMD_MOVE,
	VPLAY_CMD_LAST
}VPlayCmd;

typedef enum __RenderType
{
	RENDER_TYPE_SDL = 1,
	RENDER_TYPE_D3D,
	RENDER_TYPE_OPENGL,
	RENDER_TYPE_DDRAW,
	RENDER_TYPE_VAAPI,
	RENDER_TYPE_LAST
}RenderType;

typedef struct __VPlayCmdParam
{
	int speed;
	int moveto;
}VPlayCmdParam;

class VPlayData;
class VE_LIBRARY_API VPlay
{
public:

	VPlay();
	~VPlay();

	static BOOL SetLicense(string &strLicense);
	static BOOL GetLicenseInfo(string &strHostId, int &ch, string &type, string &startTime, string &expireTime);
	static size_t EncodeJPEG(RawFrame& pRaw, int dst_w, int dst_h, unsigned char* output, size_t outputSize);

	BOOL Init(BOOL bRealStream, string strUrl, string strUser, string strPass, BOOL bHWAccel = FALSE, VSCConnectType connectType = VSC_CONNECT_TCP);
	BOOL Init(string strFile, bool bMJPEG, string strUser, string strPass, BOOL bHWAccel = FALSE);

	BOOL AttachWidget(HWND hWnd, int w, int h, RenderType render = RENDER_TYPE_SDL);
	BOOL UpdateWidget(HWND hWnd, int w, int h);
	BOOL DetachWidget(HWND hWnd);
	BOOL SetPbTimeCallback(HWND hWnd, void * pData, VPlayPBTimeCallback callback);
	BOOL Control(VPlayCmd cmd, VPlayCmdParam param);
	BOOL EnableMot(HWND hWnd, bool enable, string strConf);

	BOOL StartGetData(void * pData, VPlayDataHandler callback);
	BOOL StopGetData();
	BOOL StartGetRawFrame(void * pData, VPlayRawFrameHandler callback);
	BOOL StopGetRawFrame();
	BOOL PutRawData(VideoFrame& packet);
	BOOL ShowAlarm(HWND hWnd);
	BOOL GetStreamInfo(VideoStreamInfo &pInfo);
    
	VPlayData *GetVPlayData() { return m_data;}
private:
    VPlayData * m_data;
};

