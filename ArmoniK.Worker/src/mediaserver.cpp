/*
 * Copyright (c) 2017-2023 Viva Technology
 *
 * The computer program contained herein contains proprietary
 * information which is the property of Viva Technology.
 * The program may be used and/or copied only with the written
 * permission of Viva Technology or in accordance with the
 * terms and conditions stipulated in the agreement/contract under
 * which the programs have been supplied.
 */
#include "Rtsp/UDPServer.h"
#include "Rtsp/RtspSession.h"
#include "Rtmp/RtmpSession.h"
#include "Http/HttpSession.h"

#ifdef ENABLE_OPENSSL
#include "Util/SSLBox.h"
#include "Http/HttpsSession.h"
#endif//ENABLE_OPENSSL

#include "Util/logger.h"
#include "Util/onceToken.h"
#include "Util/File.h"
#include "Network/TcpServer.h"
#include "Poller/EventPoller.h"
#include "Thread/WorkThreadPool.h"
#include "Device/PlayerProxy.h"
#include "Shell/ShellSession.h"
#include "Common/config.h"
#include <map>

#include "mediaserver.hpp"
#include "h264_parser.h"
using namespace std;
using namespace ZL::Util;
using namespace ZL::Http;
using namespace ZL::Rtsp;
using namespace ZL::Rtmp;
using namespace ZL::Shell;
using namespace ZL::Thread;
using namespace ZL::Network;
using namespace ZL::DEV;
using namespace media;


/**
 * \class VEMediaChannel
 * \brief Represents a media channel for video encoding.
 *
 * This class extends the functionality of the DevChannel class
 * to specifically handle media-related operations, such as video encoding.
 */
class VEMediaChannel : public DevChannel
{
public:
	VEMediaChannel(Factory &pFactory, std::string strId, int nStreamId);
	~VEMediaChannel();
	
public:
	static bool InitMediaServer();
	static void MediaServerThread();

public:
	void DataHandler1(VideoFrame& frame);
	static void DataHandler(VideoFrame& frame, void * pParam);	
		
private:
	std::string m_strId;
	int m_nStreamId;
	SmoothTicker m_Time;
	bool m_bFirstFrame;

	Factory &m_pFactory;
	H264Parser m_H264Parser;
	
    static TcpServer<RtspSession> *  m_rtspSrv;
    static TcpServer<RtmpSession> *  m_rtmpSrv;
    static TcpServer<HttpSession> *  m_httpSrv;
	static TcpServer<ShellSession> * m_shellSrv;
	static std::thread * m_pThread;
};

TcpServer<RtspSession>* VEMediaChannel::m_rtspSrv   = NULL;
TcpServer<RtmpSession>* VEMediaChannel::m_rtmpSrv   = NULL;
TcpServer<HttpSession>* VEMediaChannel::m_httpSrv   = NULL;
TcpServer<ShellSession>* VEMediaChannel::m_shellSrv = NULL;
std::thread * VEMediaChannel::m_pThread             = NULL;

/**
 * \brief Initializes the media server.
 *
 * This function initializes the media server by configuring various components
 * such as the logger, loading the INI configuration, starting TCP servers for
 * different protocols, and creating a separate thread to handle the media server
 * operations.
 *
 * \return True if the media server is successfully initialized, false otherwise.
 */
bool VEMediaChannel::InitMediaServer()
{
	Logger::Instance().add(std::make_shared<ConsoleChannel>("stdout", LTrace));
	Config::loaIniConfig();
	
	// Create TCP servers for different protocols
    m_rtspSrv  = new TcpServer<RtspSession>();
    m_rtmpSrv  = new TcpServer<RtmpSession>();
    m_httpSrv  = new TcpServer<HttpSession>();
	m_shellSrv = new TcpServer<ShellSession>();
	
	// Start TCP servers with specified ports from INI configuration
	m_rtspSrv->start(mINI::Instance()[Config::Rtsp::kPort]);
	m_rtmpSrv->start(mINI::Instance()[Config::Rtmp::kPort]);
	m_httpSrv->start(mINI::Instance()[Config::Http::kPort]);
	
	//TODO add HTTPS support
	
	// Create a separate thread to handle media server operations
	m_pThread = new std::thread(VEMediaChannel::MediaServerThread);
	
	return true;
	
}

/**
 * \brief Executes the media server thread.
 *
 * This function runs the media server thread, which is responsible for handling various media server operations.
 * It starts the event poller loop and performs necessary cleanup before returning.
 *
 * \note This function assumes that the necessary server objects (RTSP, RTMP, HTTP, and shell) have been initialized
 *       and are accessible through the member variables of the `VEMediaChannel` class.
 *
 * \return None.
 */
void VEMediaChannel::MediaServerThread()
{
	
	/* Loop for media server */
	EventPoller::Instance().runLoop();

	delete m_rtspSrv;
	delete m_rtmpSrv;
	delete m_httpSrv;
	delete m_shellSrv;

	//TODO HTTPS support

	UDPServer::Destory();
	WorkThreadPool::Destory();
	EventPoller::Destory();
	Logger::Destory();	
	return;
}	

/**
 * \brief Constructor for the VEMediaChannel class.
 *
 * This constructor initializes a VEMediaChannel instance with the provided parameters.
 *
 * \param pFactory The Factory object associated with the media channel.
 * \param strId The identifier string for the media channel.
 * \param nStreamId The stream ID for the media channel.
 */
VEMediaChannel::VEMediaChannel(Factory &pFactory, std::string strId, int nStreamId)
:m_pFactory(pFactory), m_strId(strId), m_nStreamId(nStreamId), m_bFirstFrame(true),
DevChannel("live", strId.c_str())
{	
	printf("Device ID %s  Stream %d\n", strId.c_str(), m_nStreamId);
    switch(m_nStreamId)
    {
    case 1:
        m_pFactory.RegDataCallback(m_strId,
        (CameraDataCallbackFunctionPtr)VEMediaChannel::DataHandler,
            (void *)this);
        break;
    case 2:
        m_pFactory.RegSubDataCallback(m_strId,
        (CameraDataCallbackFunctionPtr)VEMediaChannel::DataHandler,
            (void *)this);
        break;
    default:
        m_pFactory.RegDataCallback(m_strId,
        (CameraDataCallbackFunctionPtr)VEMediaChannel::DataHandler,
            (void *)this);
    }
}

/**
 * \brief Destructor for the VEMediaChannel class.
 *
 * This destructor is responsible for unregistering the data callback based on the stream ID.
 * It is automatically called when an instance of the VEMediaChannel class is destroyed.
 */
VEMediaChannel::~VEMediaChannel()
{
    switch(m_nStreamId)
    {
    case 1:
        m_pFactory.UnRegDataCallback(m_strId, (void *)this);
        break;
    case 2:
        m_pFactory.UnRegSubDataCallback(m_strId, (void *)this);
        break;
    default:
        m_pFactory.UnRegDataCallback(m_strId, (void *)this);
    }
}

/**
 * \brief DataHandler1 function processes the video frame data.
 *
 * This function is responsible for processing the video frame data received by the media channel.
 * It performs the necessary operations on the provided VideoFrame object.
 *
 * \param frame The video frame to be processed.
 *
 * \see VEMediaChannel::DataHandler
 */
void VEMediaChannel::DataHandler1(VideoFrame& frame)
{
    //CodecType current;
    bool bIsKeyFrame   = false;
    u8  *dataBuf       = NULL;
    u32 dataSize       = 0;

    /* Process the video frame for media server */
    if(frame.streamType!=VIDEO_STREAM_VIDEO)
        return;
	switch (frame.frameType)
	{
		case VIDEO_FRM_I:{
			dataBuf = frame.dataBuf + sizeof(InfoFrameI);
			dataSize = frame.dataLen - sizeof(InfoFrameI);
            InfoFrameP *pP = (InfoFrameP *)frame.dataBuf;
            /* Current only support H264 */
            if (pP->video != CODEC_H264){
				return;
			}
			bIsKeyFrame = true;
			break;
		}
		case VIDEO_FRM_P:
		{
			dataBuf = frame.dataBuf + sizeof(InfoFrameP);
			dataSize = frame.dataLen - sizeof(InfoFrameP);
            InfoFrameP *pP = (InfoFrameP *)frame.dataBuf;
            /* Current only support H264 */
            if (pP->video != CODEC_H264){
                return;
            }

			break;
		}
		default:
		   	return;
	};
	if (m_bFirstFrame == true)
        m_Time.resetTime();
	
	m_H264Parser.Reset();
	m_H264Parser.SetStream(dataBuf, dataSize);
	int sps_id = 0;

	H264NALU nalu;
	std::vector<H264NALU> nalus;
	int pps_id;

    if (m_bFirstFrame == true && bIsKeyFrame == false)
	{
        std::cout << "Have not get I frame skip" << std::endl;
        return;
	}

	while (m_H264Parser.AdvanceToNextNALU(&nalu) != H264Parser::kEOStream) {
		switch (nalu.nal_unit_type) {
		case H264NALU::Type::kSPS:
            m_H264Parser.ParseSPS(&sps_id);
            break;
		case H264NALU::Type::kPPS:
            m_H264Parser.ParsePPS(&pps_id);
            break;
		case H264NALU::Type::kSEIMessage:
        case H264NALU::Type::kSliceDataA:
		case H264NALU::Type::kSliceDataB:
		case H264NALU::Type::kSliceDataC:
		case H264NALU::Type::kIDRSlice: 
			break;
		default:
            break;
        }
        nalus.push_back(nalu);
	}

	if (m_bFirstFrame && bIsKeyFrame == true)
	{
		const H264SPS* pSPS = m_H264Parser.GetSPS(sps_id);
        VideoInfo info;
		info.iWidth  = (pSPS->pic_width_in_mbs_minus1 + 1) * 16;
		info.iHeight = (pSPS->pic_height_in_map_units_minus1 + 1) * 16;
		info.iFrameRate = 25;

		initVideo(info);
		m_bFirstFrame = false;
	}
	uint32_t uiStamp = m_Time.elapsedTime();
	/* loop to put frame */
	for (auto nalu : nalus) 
	{	
        inputH264((char *)(nalu.data - 4), nalu.size + 4, uiStamp);
	}
	return;
}

/**
 * \brief DataHandler function processes the video frame data.
 *
 * This function is responsible for handling video frame data received by the media channel.
 * It delegates the processing to the DataHandler1 member function of the associated VEMediaChannel object.
 *
 * \param frame The video frame to be processed.
 * \param pParam A pointer to the VEMediaChannel object associated with the data handler.
 *
 * \return The result of calling the DataHandler1 member function.
 *
 * \see VEMediaChannel::DataHandler1
 */
void VEMediaChannel::DataHandler(VideoFrame& frame, void * pParam)
{
    VEMediaChannel *pObject = static_cast<VEMediaChannel *> (pParam);
    
    return pObject->DataHandler1(frame);
}

/**
 * \brief Constructs a VEMediaServer object.
 *
 * This constructor initializes a VEMediaServer object with the provided Factory instance.
 * It also calls the InitDevices() method to initialize the devices used by the media server.
 *
 * \param pFactory The Factory instance to be used by the media server.
 */
VEMediaServer::VEMediaServer(Factory &pFactory)
:m_pFactory(pFactory)
{
	InitDevices();
}

/**
 * \brief Destructor for the VEMediaServer class.
 *
 * This destructor is responsible for cleaning up any resources allocated by the VEMediaServer object.
 * It ensures that all active media connections are closed and any associated memory is released.
 * 
 * \note This destructor should be called when the VEMediaServer object is no longer needed to avoid resource leaks.
 */
VEMediaServer::~VEMediaServer()
{
    // Clean up active media connections
    // ...

    // Release any other allocated resources
    // ...
}

/**
 * \brief Callback function for device change events in the VEMediaServer.
 *
 * This function is invoked when a device change event occurs in the VEMediaServer.
 *
 * \param pData Pointer to additional data associated with the device change event.
 * \param change The FactoryCameraChangeData object containing information about the device change.
 * \return Boolean value indicating the success of the callback function.
 *     - true: The callback function executed successfully.
 *     - false: An error occurred while executing the callback function.
 *
 */
bool VEMediaServer::DeviceChangeCallbackFunc(void* pData, 
								FactoryCameraChangeData change)
{
	if (pData){
		VEMediaServer * pthread = (VEMediaServer *)pData;
		pthread->DeviceChangeCallbackFunc1(change);
	}
	return true;
}

/**
 * \brief Callback function for device change events.
 *
 * This function is called when a device change event occurs in the media server.
 *
 * \param change The FactoryCameraChangeData object containing information about the device change.
 * \return True if the device change was handled successfully, false otherwise.
 */
bool VEMediaServer::DeviceChangeCallbackFunc1(FactoryCameraChangeData change)
{
    VidCamera  pParam;
    VDC_DEBUG( "Media Server Device Change Callback %s type %d Begin\n", change.id.c_str(), change.type);
	
    switch(change.type)
    {
    case FACTORY_CAMERA_OFFLINE:
        DeleteOneDevice(change.id);
        break;
    case FACTORY_CAMERA_ONLINE:
        if (m_pFactory.GetConfDB().GetCameraConf(change.id, pParam) == true && pParam.bservermotion() == true){
            InitOneDevice(pParam);
        }
        break;
    }
	VDC_DEBUG( "Media Server Change Callback %s type %d End \n", change.id.c_str(), change.type);
	return true;
}

/**
 * \brief Initializes all devices for the VEMediaServer.
 *
 * This function initializes all devices for the VEMediaServer. It performs the necessary setup and configuration
 * to prepare the devices for operation.
 *
 * \return `true` if the device initialization was successful for all devices, `false` otherwise.
 *
 */
bool VEMediaServer::InitDevices()
{	
	VidCameraList pCameraList;
	//TODO Check if online
	m_pFactory.GetCameraList(pCameraList);
	for (s32 i = 0; i < pCameraList.cvidcamera_size(); i ++){
        InitOneDevice(pCameraList.cvidcamera(i));
	}
	return true;
}


/**
 * \brief Deletes a single device from the VEMediaServer.
 *
 * This function deletes a single device from the VEMediaServer based on the provided device ID.
 *
 * \param strId The ID of the device to be deleted.
 * \return `true` if the device deletion was successful, `false` otherwise.
 *
 */
bool VEMediaServer::DeleteOneDevice(astring strId)
{
	VEMediaChannelMap::iterator it = m_ChannelMap.begin();
	for(; it!=m_ChannelMap.end(); ++it){
		if ((*it).first == strId){
			delete (*it).second;
			m_ChannelMap.erase(it);
			break;
		}
	}
	return true;
}

/**
 * \brief Initializes a single device for the VEMediaServer.
 *
 * This function initializes a single device for the VEMediaServer based on the provided video camera parameters.
 *
 * \param pParam The video camera parameters for the device initialization.
 * \return `true` if the device initialization was successful, `false` otherwise.
 *
 */
bool VEMediaServer::InitOneDevice(VidCamera  pParam)
{
    m_ChannelMap[pParam.strid()] = new VEMediaChannel(m_pFactory, pParam.strid(), 0);
	return true;
}

/**
 * \brief Initializes the media server.
 *
 * This function initializes the media server by performing necessary setup
 * tasks and configurations.
 *
 * \return True if the media server was successfully initialized, false otherwise.
 *
 * \note This function should be called before any other media server operations
 *       are performed.
 */
bool VEMediaServer::InitMediaServer()
{
	return VEMediaChannel::InitMediaServer();
}


