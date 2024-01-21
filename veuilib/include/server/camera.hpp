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

#include "vdb.hpp"
#include "hdfsrecsession.hpp"
#include "vplay.hpp"
#include "Poco/UUIDGenerator.h"
#include "vvidonvif/vvidonvifc.hpp"

/**
 * \brief Enumeration representing PTZ (Pan-Tilt-Zoom) actions.
 *
 * This enumeration defines various PTZ actions that can be performed, such as moving
 * the camera up, down, left, right, zooming in, zooming out, stopping PTZ movement, etc.
 */
typedef enum
{
    F_PTZ_UP       = 1,  /**< Move the PTZ camera upwards. */
    F_PTZ_DOWN     = 2,  /**< Move the PTZ camera downwards. */
    F_PTZ_LEFT     = 3,  /**< Move the PTZ camera to the left. */
    F_PTZ_RIGHT    = 4,  /**< Move the PTZ camera to the right. */
    F_PTZ_ZOOM_IN  = 5,  /**< Zoom in the PTZ camera. */
    F_PTZ_ZOOM_OUT = 6,  /**< Zoom out the PTZ camera. */
    F_PTZ_STOP     = 7,  /**< Stop PTZ movement. */
    F_PTZ_LAST     = 8   /**< Placeholder for the last PTZ action. */
} FPtzAction;

/**
 * \brief Enumeration representing camera status changes.
 *
 * This enumeration defines various camera status changes, such as transitioning from
 * off to on, on to off, no change in status, etc.
 */
typedef enum
{
    DEV_OFF2ON      = 1,  /**< Transition from off to on state. */
    DEV_ON2OFF      = 2,  /**< Transition from on to off state. */
    DEV_NO_CHANGE   = 3,  /**< No change in camera status. */
    DEV_LAST        = 4   /**< Placeholder for the last camera status change. */
} CameraStatus;



/**
 * \brief Callback function for camera video data.
 *
 * This function pointer type is used to define a callback for receiving compressed video data.
 *
 * \param frame     Reference to a VideoFrame object containing the video data.
 * \param pParam    A pointer to user-defined parameters associated with the callback.
 */
typedef void (*CameraDataCallbackFunctionPtr)(VideoFrame& frame, void* pParam);

/**
 * \brief Callback function for camera raw data.
 *
 * This function pointer type is used to define a callback for receiving raw video data.
 *
 * \param frame     Reference to a RawFrame object containing the raw video data.
 * \param pParam    A pointer to user-defined parameters associated with the callback.
 */
typedef void (*CameraRawCallbackFunctionPtr)(RawFrame& frame, void* pParam);

/**
 * \brief Callback function for camera video sequence data.
 *
 * This function pointer type is used to define a callback for receiving video sequence data.
 *
 * \param frame     Reference to a VideoSeqFrame object containing the video sequence data.
 * \param pParam    A pointer to user-defined parameters associated with the callback.
 */
typedef void (*CameraSeqCallbackFunctionPtr)(VideoSeqFrame& frame, void* pParam);

/**
 * \brief Callback function for camera deletion.
 *
 * This function pointer type is used to define a callback for handling camera deletion.
 *
 * \param pParam    A pointer to user-defined parameters associated with the callback.
 */
typedef void (*CameraDelCallbackFunctionPtr)(void* pParam);


/**
 * \brief Map for associating a pointer with a camera data callback function.
 *
 * This map is used to associate a void pointer with a callback function for handling camera data.
 * The void pointer serves as an identifier or key for the associated callback function.
 */
typedef std::map<void*, CameraDataCallbackFunctionPtr> CameraDataCallbackMap;

/**
 * \brief Map for associating a pointer with a camera raw data callback function.
 *
 * This map is used to associate a void pointer with a callback function for handling raw camera data.
 * The void pointer serves as an identifier or key for the associated callback function.
 */
typedef std::map<void*, CameraRawCallbackFunctionPtr> CameraRawCallbackMap;

/**
 * \brief Map for associating a pointer with a camera video sequence callback function.
 *
 * This map is used to associate a void pointer with a callback function for handling camera video sequence data.
 * The void pointer serves as an identifier or key for the associated callback function.
 */
typedef std::map<void*, CameraSeqCallbackFunctionPtr> CameraSeqCallbackMap;

/**
 * \brief Map for associating a pointer with a camera deletion callback function.
 *
 * This map is used to associate a void pointer with a callback function for handling camera deletion.
 * The void pointer serves as an identifier or key for the associated callback function.
 */
typedef std::map<void*, CameraDelCallbackFunctionPtr> CameraDelCallbackMap;


class VE_LIBRARY_API CameraParam
{
public:
	CameraParam();
	CameraParam(const CameraParam &pParam);
	CameraParam(VidCamera &pData);
	~CameraParam();
	CameraParam & operator=(const CameraParam &pParam)
	{
		m_Conf = pParam.m_Conf;
		if (pParam.m_strUrl.length() > 0)
		{
			m_strUrl = pParam.m_strUrl;
		}
		m_strUrlSubStream = pParam.m_strUrlSubStream;
		m_bHasSubStream = pParam.m_bHasSubStream;
		m_bOnvifUrlGetted = pParam.m_bOnvifUrlGetted;
		m_Online = pParam.m_Online;
		return *this;
	}

	BOOL UpdateUrl();
	BOOL UpdateUrlOnvif();
	BOOL CheckOnline();
	BOOL UpdateDefaultUrl();

	VidCamera m_Conf;
	BOOL m_bOnvifUrlGetted;
	astring m_strUrl;
	astring m_strUrlSubStream;
	BOOL m_bHasSubStream;

	BOOL m_Online;
	BOOL m_OnlineUrl;

	/* backend status  */
	BOOL m_wipOnline;
	BOOL m_wipOnlineUrl;
	VidStreamList m_cStreamList;
};

/* 
 * Add Channel number for NVR integration support, a camera is a device
 * ONVIFCamera -> Camera, 1 channel 
 * File               -> Camera, 1 channel 
 * RTSP             -> Camera, 1 channel
 * USB              -> Camera, 1 channel
 * MJPEG           -> Camera, 1 channel
 * LinkVision      -> Camera, register
 * Recorder        -> Camera, dynamic channel
*/

class VE_LIBRARY_API Camera
{
public:
	Camera(ConfDB &pConfDB, VDB &pVdb, 
					VHdfsDB &pVHdfsdb, const CameraParam &pParam, 
					RecChangeFunctionPtr pCallback, void *pCallbackParam);
	~Camera();

	/* Below api is for a new thread to do some network task whitch may be blocked */
	BOOL GetCameraParam(CameraParam &pParam);

	BOOL StartData();
	BOOL StopData();
	
	BOOL StartSubData();
	BOOL StopSubData();
	bool UpdateRecSched(VidCamera &pCam);
	
	CameraStatus CheckCamera(astring strUrl, astring strUrlSubStream, 
		BOOL bHasSubStream, BOOL bOnline, BOOL bOnlineUrl, 
		VidStreamList cStreamlist);

	/* Data  */
	static BOOL DataHandler(void* pData, VideoFrame& frame);
	BOOL DataHandler1(VideoFrame& frame);
	static BOOL SubDataHandler(void* pData, VideoFrame& frame);
	BOOL SubDataHandler1(VideoFrame& frame);

	void Lock(){m_Lock.lock();}
	void UnLock(){m_Lock.unlock();}
	void SubLock(){m_SubLock.lock();}
	void SubUnLock(){m_SubLock.unlock();}

	/* Data  callback*/
	BOOL RegDataCallback(CameraDataCallbackFunctionPtr pCallback, void * pParam);
	BOOL UnRegDataCallback(void * pParam);
	BOOL RegSubDataCallback(CameraDataCallbackFunctionPtr pCallback, void * pParam);
	BOOL UnRegSubDataCallback(void * pParam);
	
	BOOL RegDelCallback(CameraDelCallbackFunctionPtr pCallback, void * pParam);
	BOOL UnRegDelCallback(void * pParam);
	
	BOOL GetInfoFrame(InfoFrame &pFrame);
	BOOL GetiFrame(VideoFrame& frame);
	BOOL GetSubInfoFrame(InfoFrame &pFrame);
	
	BOOL GetCameraOnline();
	
	BOOL AttachPlayer(HWND hWnd, int w, int h);
	BOOL UpdateWidget(HWND hWnd, int w, int h);
	BOOL DetachPlayer(HWND hWnd);
	BOOL GetStreamInfo(VideoStreamInfo &pInfo);
	BOOL GetCamStreamList(VidStreamList &pList);

	BOOL ShowAlarm(HWND hWnd);
	BOOL PtzAction(FPtzAction action, float speed);
	BOOL UpdatePTZConf();
	BOOL FireAlarm(s64 nStartTime);

private:
	VPlay *m_pvPlay;
	VPlay *m_pvPlaySubStream;
	VPlay &m_vPlay;
	VPlay &m_vPlaySubStream;
	BOOL m_bStarted;
	BOOL m_bSubStarted;
	CameraDataCallbackMap m_DataMap;
	CameraDataCallbackMap m_SubDataMap;

	CameraDelCallbackMap m_DelMap;
	VideoFrame m_iFrameCache;

	CameraParam m_param;
	fast_mutex m_Lock;
	fast_mutex m_SubLock;

	VDB &m_pVdb;
	RecordSession *m_pRecord;
	RecordSchedWeekMap m_cSchedMap;
	RecordWrapper m_cRecordWrapper;
	
	VVidOnvifCPtz m_ptz;
	BOOL m_ptzInited;

	InfoFrame m_infoData;
	BOOL m_bGotInfoData;
	s32 m_nDataRef;
	InfoFrame m_infoSubData;
	BOOL m_bGotInfoSubData;
	s32 m_nSubDataRef;
	ConfDB &m_pConfDB;
};

typedef CameraParam* LPCameraParam;
typedef Camera* LPCamera;

