
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

#include "define.h"
#include "vdb/vdb.hpp"
#include "vdb/hdfsrecsession.hpp"
#include "vdb/recordwrapper.hpp"
#include "vplay.h"
#include "Poco/UUIDGenerator.h"
#include "vvidonvif/vvidonvifc.hpp"
#include "config/vidconf.pb.h"
#include "config/conf.hpp"
#include "confdb.h"
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
     * \return true if the update is successful, false otherwise.
     */
    bool UpdateUrl();

    /**
     * \brief Updates the URL using ONVIF protocol.
     *
     * \return true if the update is successful, false otherwise.
     */
    bool UpdateUrlOnvif();

    /**
     * \brief Checks the online status of the camera.
     *
     * \return true if the camera is online, false otherwise.
     */
    bool CheckOnline();

    /**
     * \brief Updates the default URL of the camera.
     *
     * \return true if the update is successful, false otherwise.
     */
    bool UpdateDefaultUrl();

    VidCamera m_Conf;            ///< Configuration settings for the camera.
    bool m_bOnvifUrlGetted;      ///< Flag indicating if the ONVIF URL has been obtained.
    astring m_strUrl;            ///< URL of the camera.
    astring m_strUrlSubStream;   ///< URL of the camera's substream.
    bool m_bHasSubStream;        ///< Flag indicating if the camera has a substream.

    bool m_Online;               ///< Flag indicating if the camera is online.
    bool m_OnlineUrl;            ///< Flag indicating if the camera's URL is online.

    bool m_wipOnline;            ///< Backend status: Flag indicating if the camera is online.
    bool m_wipOnlineUrl;         ///< Backend status: Flag indicating if the camera's URL is online.
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
     * \return true if successful, false otherwise.
     */
    bool GetCameraParam(CameraParam &pParam);
	
	/**
     * \brief Starts data streaming from the camera.
     *
     * \return true if successful, false otherwise.
     */
	bool StartData();
	
	/**
     * \brief Stops data streaming from the camera.
     *
     * \return true if successful, false otherwise.
     */
	bool StopData();
	
	/**
     * \brief Starts substream data streaming from the camera.
     *
     * \return true if successful, false otherwise.
     */
	bool StartSubData();

	/**
     * \brief Stops substream data streaming from the camera.
     *
     * \return true if successful, false otherwise.
     */
	bool StopSubData();

	/**
     * \brief Updates the recording schedule for the camera.
     *
     * \param pCam The VidCamera object containing updated schedule information.
     * \return true if successful, false otherwise.
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
		bool bHasSubStream, bool bOnline, bool bOnlineUrl, 
		VidStreamList cStreamlist);

	/* Data  */
	
	/**
     * \brief Handles data for the camera.
     *
     * \param pData Pointer to data associated with the camera.
     * \param frame Reference to the VideoFrame object containing video frame information.
     * \return true if the data handling is successful, false otherwise.
     * 
     * \note This is a static member function.
     */
	static bool DataHandler(void* pData, VideoFrame& frame);

	/**
     * \brief Handles substream data for the camera.
     *
     * \param pData Pointer to data associated with the camera.
     * \param frame Reference to the VideoFrame object containing substream video frame information.
     * \return true if the substream data handling is successful, false otherwise.
     * 
     * \note This is a static member function.
     */
	static bool SubDataHandler(void* pData, VideoFrame& frame);

    /**
     * \brief Handles data for the camera.
     *
     * \param frame Reference to the VideoFrame object containing video frame information.
     * \return true if the data handling is successful, false otherwise.
     */
	bool DataHandler1(VideoFrame& frame);
	
	/**
     * \brief Handles substream data for the camera.
     *
     * \param frame Reference to the VideoFrame object containing substream video frame information.
     * \return true if the substream data handling is successful, false otherwise.
     */
	bool SubDataHandler1(VideoFrame& frame);

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
     * \return true if the callback registration is successful, false otherwise.
     */
	bool RegDataCallback(CameraDataCallbackFunctionPtr pCallback, void * pParam);

	/**
     * \brief Unregisters a callback function for handling main data events.
     *
     * This function allows the unregistration of a callback function previously registered
     * for main data events.
     *
     * \param pParam Pointer to the user-defined parameter associated with the callback function.
     * \return true if the callback unregistration is successful, false otherwise.
     */
	bool UnRegDataCallback(void * pParam);

	/**
     * \brief Registers a callback function for handling substream data events.
     *
     * \param pCallback Pointer to the CameraDataCallbackFunction callback function.
     * \param pParam Pointer to the user-defined parameter to be passed to the callback function.
     * \return true if the callback registration is successful, false otherwise.
     */
	bool RegSubDataCallback(CameraDataCallbackFunctionPtr pCallback, void * pParam);

	/**
     * \brief Unregisters a callback function for handling substream data events.
     *
     * \param pParam Pointer to the user-defined parameter associated with the callback function.
     * \return true if the callback unregistration is successful, false otherwise.
     */
	bool UnRegSubDataCallback(void * pParam);
	
	/**
     * \brief Registers a callback function for handling camera deletion events.
     *
     * \param pCallback Pointer to the CameraDelCallbackFunction callback function.
     * \param pParam Pointer to the user-defined parameter to be passed to the callback function.
     * \return true if the callback registration is successful, false otherwise.
     */
	bool RegDelCallback(CameraDelCallbackFunctionPtr pCallback, void * pParam);

    /**
     * \brief Unregisters a callback function for handling camera deletion events.
     *
     * \param pParam Pointer to the user-defined parameter associated with the callback function.
     * \return true if the callback unregistration is successful, false otherwise.
     */
	bool UnRegDelCallback(void * pParam);
	
	/**
     * \brief Retrieves information about the camera's main video frame.
     *
     * \param pFrame Reference to the InfoFrame object to store the main video frame information.
     * \return true if the information frame is successfully retrieved, false otherwise.
     */
	bool GetInfoFrame(InfoFrame &pFrame);

	/**
     * \brief Retrieves the current video frame from the camera.
     *
     * \param frame Reference to the VideoFrame object to store the video frame.
     * \return true if the video frame is successfully retrieved, false otherwise.
     */
	bool GetiFrame(VideoFrame& frame);

	/**
     * \brief Retrieves information about the camera's substream video frame.
     *
     * \param pFrame Reference to the InfoFrame object to store the substream video frame information.
     * \return true if the substream information frame is successfully retrieved, false otherwise.
     */
	bool GetSubInfoFrame(InfoFrame &pFrame);

	/**
     * \brief Checks if the camera is online.
     *
     * \return true if the camera is online, false otherwise.
     */
	bool GetCameraOnline();
	
	/**
     * \brief Attaches the camera player to a specified window.
     *
     * \param hWnd Handle to the window to which the camera player should be attached.
     * \param w Width of the window.
     * \param h Height of the window.
     * \return true if the player is successfully attached to the window, false otherwise.
     */
	bool AttachPlayer(HWND hWnd, int w, int h);

	/**
     * \brief Updates the dimensions of the camera player widget.
     *
     * \param hWnd Handle to the window containing the camera player.
     * \param w New width of the player widget.
     * \param h New height of the player widget.
     * \return true if the player widget is successfully updated, false otherwise.
     */
	bool UpdateWidget(HWND hWnd, int w, int h);

	/**
     * \brief Detaches the camera player from a specified window.
     *
     * \param hWnd Handle to the window from which the camera player should be detached.
     * \return true if the player is successfully detached from the window, false otherwise.
     */
	bool DetachPlayer(HWND hWnd);

	/**
     * \brief Retrieves information about the video stream.
     *
     * \param pInfo Reference to the VideoStreamInfo object to store the stream information.
     * \return true if the stream information is successfully retrieved, false otherwise.
     */
	bool GetStreamInfo(VideoStreamInfo &pInfo);

	/**
     * \brief Retrieves the list of video streams associated with the camera.
     *
     * \param pList Reference to the VidStreamList object to store the list of video streams.
     * \return true if the stream list is successfully retrieved, false otherwise.
     */
	bool GetCamStreamList(VidStreamList &pList);

    /**
     * \brief Displays an alarm on the specified window.
     *
     * \param hWnd Handle to the window where the alarm should be displayed.
     * \return true if the alarm is successfully shown, false otherwise.
     */
	bool ShowAlarm(HWND hWnd);

	/**
     * \brief Performs a PTZ (Pan-Tilt-Zoom) action on the camera.
     *
     * \param action The PTZ action to be performed.
     * \param speed The speed at which the PTZ action should be executed.
     * \return true if the PTZ action is successfully executed, false otherwise.
     */
	bool PtzAction(FPtzAction action, float speed);

	/**
     * \brief Updates the PTZ (Pan-Tilt-Zoom) configuration for the camera.
     *
     * \return true if the PTZ configuration is successfully updated, false otherwise.
     */
	bool UpdatePTZConf();

	/**
     * \brief Triggers an alarm starting from the specified time.
     *
     * This function is used to initiate an alarm event that occurred at a specific time.
     *
     * \param nStartTime The start time of the alarm event.
     * \return true if the alarm is successfully triggered, false otherwise.
     */
	bool FireAlarm(long nStartTime);

private:
	VPlay *m_pvPlay;             ///< Pointer to the main video player object.
	VPlay *m_pvPlaySubStream;    ///< Pointer to the substream video player object.
	VPlay &m_vPlay;              ///< Reference to the main video player object.
	VPlay &m_vPlaySubStream;     ///< Reference to the substream video player object.

	CameraDataCallbackMap m_DataMap;    ///< Map for storing callbacks related to main data events
	CameraDataCallbackMap m_SubDataMap; ///< Map for storing callbacks related to substream data events.
     CameraParam m_param;                ///< Object representing camera parameters.
	CameraDelCallbackMap m_DelMap;      ///< Map for storing callbacks related to camera deletion events.
	VideoFrame m_iFrameCache;           ///< Cache for storing the main video frame.

	fast_mutex m_Lock;        ///< Mutex for thread synchronization of main data access.
	fast_mutex m_SubLock;     ///< Mutex for thread synchronization of substream data access.
	VDB &m_pVdb;              ///< Reference to the VDB (Video Database) object.
	RecordSession *m_pRecord; ///< Pointer to the record session object.

	RecordSchedWeekMap m_cSchedMap;  ///< Map for storing the recording schedule for each day of the week.
	RecordWrapper m_cRecordWrapper;  ///< Wrapper for handling recording-related functionality.
	VVidOnvifCPtz m_ptz;             ///< Object for handling PTZ (Pan-Tilt-Zoom) control using ONVIF protocol.     
     ConfDB &m_pConfDB;               ///< Reference to the configuration database object.

	bool m_ptzInited;        ///< indicating whether PTZ (Pan-Tilt-Zoom) control is initialized.
     bool m_bGotInfoData;     ///< Flag indicating whether information data is successfully obtained.
	bool m_bGotInfoSubData;  ///< Flag indicating whether substream information data is successfully obtained.
     bool m_bStarted;         ///< Flag indicating whether the camera data streaming is started.
	bool m_bSubStarted;      ///< Flag indicating whether the substream data streaming is started.
	
	InfoFrame m_infoData;     ///< Information frame for main video data.
	InfoFrame m_infoSubData;  ///< Information frame for substream video data.
	int m_nDataRef;           ///< Reference count for main video data.
	int m_nSubDataRef;        ///< Reference count for substream video data.
};


typedef CameraParam* LPCameraParam; ///< Represents a pointer to a CameraParam object.
typedef Camera* LPCamera;           ///< Represents a pointer to a Camera object.