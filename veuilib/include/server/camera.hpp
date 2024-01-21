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


/**
 * \brief The CameraParam class represents parameters for a camera.
 */
class UB_LIBRARY_API CameraParam
{
public:
    /**
     * \brief Default constructor for the CameraParam class.
     */
    CameraParam();

    /**
     * \brief Copy constructor for the CameraParam class.
     *
     * \param pParam The CameraParam object to be copied.
     */
    CameraParam(const CameraParam &pParam);

    /**
     * \brief Constructor for the CameraParam class with a VidCamera object.
     *
     * \param pData The VidCamera object used to initialize the CameraParam.
     */
    CameraParam(VidCamera &pData);

    /**
     * \brief Destructor for the CameraParam class.
     */
    ~CameraParam();

    /**
     * \brief Assignment operator for the CameraParam class.
     *
     * \param pParam The CameraParam object to be assigned.
     * \return A reference to the modified CameraParam object.
     */
    CameraParam &operator=(const CameraParam &pParam);

    /**
     * \brief Updates the URL of the camera.
     *
     * \return TRUE if the update is successful, FALSE otherwise.
     */
    BOOL UpdateUrl();

    /**
     * \brief Updates the URL using ONVIF protocol.
     *
     * \return TRUE if the update is successful, FALSE otherwise.
     */
    BOOL UpdateUrlOnvif();

    /**
     * \brief Checks the online status of the camera.
     *
     * \return TRUE if the camera is online, FALSE otherwise.
     */
    BOOL CheckOnline();

    /**
     * \brief Updates the default URL of the camera.
     *
     * \return TRUE if the update is successful, FALSE otherwise.
     */
    BOOL UpdateDefaultUrl();

    VidCamera m_Conf;            ///< Configuration settings for the camera.
    BOOL m_bOnvifUrlGetted;      ///< Flag indicating if the ONVIF URL has been obtained.
    astring m_strUrl;            ///< URL of the camera.
    astring m_strUrlSubStream;   ///< URL of the camera's substream.
    BOOL m_bHasSubStream;        ///< Flag indicating if the camera has a substream.

    BOOL m_Online;               ///< Flag indicating if the camera is online.
    BOOL m_OnlineUrl;            ///< Flag indicating if the camera's URL is online.

    BOOL m_wipOnline;            ///< Backend status: Flag indicating if the camera is online.
    BOOL m_wipOnlineUrl;         ///< Backend status: Flag indicating if the camera's URL is online.
    VidStreamList m_cStreamList; ///< List of video streams associated with the camera.
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

class UB_LIBRARY_API Camera
{
public:
    /**
     * \brief Constructor for the Camera class.
     *
     * \param pConfDB Reference to the ConfDB object.
     * \param pVdb Reference to the VDB object.
     * \param pVHdfsdb Reference to the VHdfsDB object.
     * \param pParam The CameraParam object representing camera parameters.
     * \param pCallback Callback function for record change events.
     * \param pCallbackParam Parameter for the record change callback.
     */
    Camera(ConfDB &pConfDB, VDB &pVdb, VHdfsDB &pVHdfsdb, const CameraParam &pParam,
           RecChangeFunctionPtr pCallback, void *pCallbackParam);

    /**
     * \brief Destructor for the Camera class.
     */
	~Camera();

    /**
     * \brief Retrieves the camera parameters.
     *
     * \param pParam Reference to the CameraParam object to store the parameters.
     * \return TRUE if successful, FALSE otherwise.
     */
    BOOL GetCameraParam(CameraParam &pParam);
	
	/**
     * \brief Starts data streaming from the camera.
     *
     * \return TRUE if successful, FALSE otherwise.
     */
	BOOL StartData();
	
	/**
     * \brief Stops data streaming from the camera.
     *
     * \return TRUE if successful, FALSE otherwise.
     */
	BOOL StopData();
	
	/**
     * \brief Starts substream data streaming from the camera.
     *
     * \return TRUE if successful, FALSE otherwise.
     */
	BOOL StartSubData();

	/**
     * \brief Stops substream data streaming from the camera.
     *
     * \return TRUE if successful, FALSE otherwise.
     */
	BOOL StopSubData();

	/**
     * \brief Updates the recording schedule for the camera.
     *
     * \param pCam The VidCamera object containing updated schedule information.
     * \return TRUE if successful, FALSE otherwise.
     */
	bool UpdateRecSched(VidCamera &pCam);
	

	/**
     * \brief Checks the status of the camera.
     *
     * \param strUrl The URL of the camera.
     * \param strUrlSubStream The URL of the camera's substream.
     * \param bHasSubStream Flag indicating if the camera has a substream.
     * \param bOnline Flag indicating if the camera is online.
     * \param bOnlineUrl Flag indicating if the camera's URL is online.
     * \param cStreamlist List of video streams associated with the camera.
     * \return The CameraStatus indicating the status of the camera.
     */
	CameraStatus CheckCamera(astring strUrl, astring strUrlSubStream, 
		BOOL bHasSubStream, BOOL bOnline, BOOL bOnlineUrl, 
		VidStreamList cStreamlist);

	/* Data  */
	
	/**
     * \brief Handles data for the camera.
     *
     * \param pData Pointer to data associated with the camera.
     * \param frame Reference to the VideoFrame object containing video frame information.
     * \return TRUE if the data handling is successful, FALSE otherwise.
     * 
     * \note This is a static member function.
     */
	static BOOL DataHandler(void* pData, VideoFrame& frame);

	/**
     * \brief Handles substream data for the camera.
     *
     * \param pData Pointer to data associated with the camera.
     * \param frame Reference to the VideoFrame object containing substream video frame information.
     * \return TRUE if the substream data handling is successful, FALSE otherwise.
     * 
     * \note This is a static member function.
     */
	static BOOL SubDataHandler(void* pData, VideoFrame& frame);

    /**
     * \brief Handles data for the camera.
     *
     * \param frame Reference to the VideoFrame object containing video frame information.
     * \return TRUE if the data handling is successful, FALSE otherwise.
     */
	BOOL DataHandler1(VideoFrame& frame);
	
	/**
     * \brief Handles substream data for the camera.
     *
     * \param frame Reference to the VideoFrame object containing substream video frame information.
     * \return TRUE if the substream data handling is successful, FALSE otherwise.
     */
	BOOL SubDataHandler1(VideoFrame& frame);

   /**
    * \brief Acquires the main lock for thread synchronization.
    *
    * This function locks the main mutex to synchronize access to shared resources.
    */
	void Lock(){m_Lock.lock();}
	
	/**
     * \brief Releases the main lock after thread synchronization.
     *
     * This function unlocks the main mutex, allowing other threads to access shared resources.
     */
	void UnLock(){m_Lock.unlock();}

	/**
     * \brief Acquires the substream lock for thread synchronization.
     *
     * This function locks the substream mutex to synchronize access to substream-related resources.
     */
	void SubLock(){m_SubLock.lock();}

	/**
     * \brief Releases the substream lock after thread synchronization.
     *
     * This function unlocks the substream mutex, allowing other threads to access substream-related resources.
     */
	void SubUnLock(){m_SubLock.unlock();}

	/* Data  callback*/
	/**
     * \brief Registers a callback function for handling main data events.
     *
     * This function allows the registration of a callback function that will be invoked
     * when main data events occur.
     *
     * \param pCallback Pointer to the CameraDataCallbackFunction callback function.
     * \param pParam Pointer to the user-defined parameter to be passed to the callback function.
     * \return TRUE if the callback registration is successful, FALSE otherwise.
     */
	BOOL RegDataCallback(CameraDataCallbackFunctionPtr pCallback, void * pParam);

	/**
     * \brief Unregisters a callback function for handling main data events.
     *
     * This function allows the unregistration of a callback function previously registered
     * for main data events.
     *
     * \param pParam Pointer to the user-defined parameter associated with the callback function.
     * \return TRUE if the callback unregistration is successful, FALSE otherwise.
     */
	BOOL UnRegDataCallback(void * pParam);

	/**
     * \brief Registers a callback function for handling substream data events.
     *
     * \param pCallback Pointer to the CameraDataCallbackFunction callback function.
     * \param pParam Pointer to the user-defined parameter to be passed to the callback function.
     * \return TRUE if the callback registration is successful, FALSE otherwise.
     */
	BOOL RegSubDataCallback(CameraDataCallbackFunctionPtr pCallback, void * pParam);

	/**
     * \brief Unregisters a callback function for handling substream data events.
     *
     * \param pParam Pointer to the user-defined parameter associated with the callback function.
     * \return TRUE if the callback unregistration is successful, FALSE otherwise.
     */
	BOOL UnRegSubDataCallback(void * pParam);
	
	/**
     * \brief Registers a callback function for handling camera deletion events.
     *
     * \param pCallback Pointer to the CameraDelCallbackFunction callback function.
     * \param pParam Pointer to the user-defined parameter to be passed to the callback function.
     * \return TRUE if the callback registration is successful, FALSE otherwise.
     */
	BOOL RegDelCallback(CameraDelCallbackFunctionPtr pCallback, void * pParam);

    /**
     * \brief Unregisters a callback function for handling camera deletion events.
     *
     * \param pParam Pointer to the user-defined parameter associated with the callback function.
     * \return TRUE if the callback unregistration is successful, FALSE otherwise.
     */
	BOOL UnRegDelCallback(void * pParam);
	
	/**
     * \brief Retrieves information about the camera's main video frame.
     *
     * \param pFrame Reference to the InfoFrame object to store the main video frame information.
     * \return TRUE if the information frame is successfully retrieved, FALSE otherwise.
     */
	BOOL GetInfoFrame(InfoFrame &pFrame);

	/**
     * \brief Retrieves the current video frame from the camera.
     *
     * \param frame Reference to the VideoFrame object to store the video frame.
     * \return TRUE if the video frame is successfully retrieved, FALSE otherwise.
     */
	BOOL GetiFrame(VideoFrame& frame);

	/**
     * \brief Retrieves information about the camera's substream video frame.
     *
     * \param pFrame Reference to the InfoFrame object to store the substream video frame information.
     * \return TRUE if the substream information frame is successfully retrieved, FALSE otherwise.
     */
	BOOL GetSubInfoFrame(InfoFrame &pFrame);

	/**
     * \brief Checks if the camera is online.
     *
     * \return TRUE if the camera is online, FALSE otherwise.
     */
	BOOL GetCameraOnline();
	
	/**
     * \brief Attaches the camera player to a specified window.
     *
     * \param hWnd Handle to the window to which the camera player should be attached.
     * \param w Width of the window.
     * \param h Height of the window.
     * \return TRUE if the player is successfully attached to the window, FALSE otherwise.
     */
	BOOL AttachPlayer(HWND hWnd, int w, int h);

	/**
     * \brief Updates the dimensions of the camera player widget.
     *
     * \param hWnd Handle to the window containing the camera player.
     * \param w New width of the player widget.
     * \param h New height of the player widget.
     * \return TRUE if the player widget is successfully updated, FALSE otherwise.
     */
	BOOL UpdateWidget(HWND hWnd, int w, int h);

	/**
     * \brief Detaches the camera player from a specified window.
     *
     * \param hWnd Handle to the window from which the camera player should be detached.
     * \return TRUE if the player is successfully detached from the window, FALSE otherwise.
     */
	BOOL DetachPlayer(HWND hWnd);

	/**
     * \brief Retrieves information about the video stream.
     *
     * \param pInfo Reference to the VideoStreamInfo object to store the stream information.
     * \return TRUE if the stream information is successfully retrieved, FALSE otherwise.
     */
	BOOL GetStreamInfo(VideoStreamInfo &pInfo);

	/**
     * \brief Retrieves the list of video streams associated with the camera.
     *
     * \param pList Reference to the VidStreamList object to store the list of video streams.
     * \return TRUE if the stream list is successfully retrieved, FALSE otherwise.
     */
	BOOL GetCamStreamList(VidStreamList &pList);

    /**
     * \brief Displays an alarm on the specified window.
     *
     * \param hWnd Handle to the window where the alarm should be displayed.
     * \return TRUE if the alarm is successfully shown, FALSE otherwise.
     */
	BOOL ShowAlarm(HWND hWnd);

	/**
     * \brief Performs a PTZ (Pan-Tilt-Zoom) action on the camera.
     *
     * \param action The PTZ action to be performed.
     * \param speed The speed at which the PTZ action should be executed.
     * \return TRUE if the PTZ action is successfully executed, FALSE otherwise.
     */
	BOOL PtzAction(FPtzAction action, float speed);

	/**
     * \brief Updates the PTZ (Pan-Tilt-Zoom) configuration for the camera.
     *
     * \return TRUE if the PTZ configuration is successfully updated, FALSE otherwise.
     */
	BOOL UpdatePTZConf();

	/**
     * \brief Triggers an alarm starting from the specified time.
     *
     * This function is used to initiate an alarm event that occurred at a specific time.
     *
     * \param nStartTime The start time of the alarm event.
     * \return TRUE if the alarm is successfully triggered, FALSE otherwise.
     */
	BOOL FireAlarm(long nStartTime);

private:
	VPlay *m_pvPlay;
	VPlay *m_pvPlaySubStream;
	VPlay &m_vPlay;
	VPlay &m_vPlaySubStream;

	CameraDataCallbackMap m_DataMap;
	CameraDataCallbackMap m_SubDataMap;
    CameraParam m_param;
	CameraDelCallbackMap m_DelMap;

	VideoFrame m_iFrameCache;

	fast_mutex m_Lock;
	fast_mutex m_SubLock;

	VDB &m_pVdb;
	RecordSession *m_pRecord;

	RecordSchedWeekMap m_cSchedMap;
	RecordWrapper m_cRecordWrapper;
	
	VVidOnvifCPtz m_ptz;

	BOOL m_ptzInited;
    BOOL m_bGotInfoData;
	BOOL m_bGotInfoSubData;
    BOOL m_bStarted;
	BOOL m_bSubStarted;
	
	InfoFrame m_infoData;
	InfoFrame m_infoSubData;

	int m_nDataRef;
	int m_nSubDataRef;

	ConfDB &m_pConfDB;
};


typedef CameraParam* LPCameraParam; ///< Represents a pointer to a CameraParam object.
typedef Camera* LPCamera;           ///< Represents a pointer to a Camera object.