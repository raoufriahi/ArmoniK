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

bool VEMediaChannel::InitMediaServer()
{
	Logger::Instance().add(std::make_shared<ConsoleChannel>("stdout", LTrace));
	Config::loaIniConfig();
	
    m_rtspSrv  = new TcpServer<RtspSession>();
    m_rtmpSrv  = new TcpServer<RtmpSession>();
    m_httpSrv  = new TcpServer<HttpSession>();
	m_shellSrv = new TcpServer<ShellSession>();
	
	m_rtspSrv->start(mINI::Instance()[Config::Rtsp::kPort]);
	m_rtmpSrv->start(mINI::Instance()[Config::Rtmp::kPort]);
	m_httpSrv->start(mINI::Instance()[Config::Http::kPort]);
	
	//TODO add HTTPS support
	
	m_pThread = new std::thread(VEMediaChannel::MediaServerThread);
	
	return true;
	
}

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

void VEMediaChannel::DataHandler(VideoFrame& frame, void * pParam)
{
    VEMediaChannel *pObject = static_cast<VEMediaChannel *> (pParam);
    
    return pObject->DataHandler1(frame);
}



VEMediaServer::VEMediaServer(Factory &pFactory)
:m_pFactory(pFactory)
{
	InitDevices();
}
VEMediaServer::~VEMediaServer()
{
	
}

bool VEMediaServer::DeviceChangeCallbackFunc(void* pData, 
								FactoryCameraChangeData change)
{
	if (pData){
		VEMediaServer * pthread = (VEMediaServer *)pData;
		pthread->DeviceChangeCallbackFunc1(change);
	}
	return true;
}
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

bool VEMediaServer::InitOneDevice(VidCamera  pParam)
{
    m_ChannelMap[pParam.strid()] = new VEMediaChannel(m_pFactory, pParam.strid(), 0);
	return true;
}


bool VEMediaServer::InitMediaServer()
{
	return VEMediaChannel::InitMediaServer();
}


