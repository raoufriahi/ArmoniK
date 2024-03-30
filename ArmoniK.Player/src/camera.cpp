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


#include "camera.h"


CameraParam::CameraParam()
{
	Poco::UUIDGenerator uuidCreator;
	
	string strId  = uuidCreator.createRandom().toString();
	m_Conf.set_ntype(VID_ONVIF_S);
	m_Conf.set_strid(strId);
	m_Conf.set_strname("Camera");

	m_Conf.set_strip("192.168.0.1");
	m_Conf.set_strport("80");
	m_Conf.set_struser("admin");
	m_Conf.set_strpasswd("admin");

	m_Conf.set_strrtspurl("rtsp://192.168.0.1:554/Streaming");

	string filePath = "camera.mp4";

	m_Conf.set_strfile(filePath.c_str());

	m_Conf.set_stronvifaddress("/onvif/device_service");

	m_Conf.set_bprofiletoken(false);
	m_Conf.set_bhwaccel(false);

	m_Conf.set_strprofiletoken1("quality_h264");
	m_Conf.set_strprofiletoken1("second_h264");
	m_Conf.set_bhdfsrecord(false);
	string *pSched = m_Conf.add_crecsched();
	*pSched = REC_SCHED_ALL_DAY;
	
	m_bOnvifUrlGetted = false;
	m_bHasSubStream = false;

	m_Conf.set_nconnecttype(VID_CONNECT_TCP);

	m_Online = false;
	m_OnlineUrl = false;

	string IP = m_Conf.strip();
	m_strUrl = "rtsp://" + IP + ":" + "554" + "/Streaming";
	m_strUrlSubStream = "rtsp://" + IP + ":" + "554" + "/Streaming";

}

bool CameraParam::UpdateDefaultUrl()
{
    // Use std::ostringstream to construct URLs more efficiently
    std::ostringstream streamUrl, subStreamUrl;

    // Get the IP address as a string
    std::string IP = m_Conf.strip();

    // Construct the main URL
    streamUrl << "rtsp://" << IP << ":554/Streaming";
    m_strUrl = streamUrl.str();

    // Construct the substream URL
    subStreamUrl << "rtsp://" << IP << ":554/Streaming";
    m_strUrlSubStream = subStreamUrl.str();

    return true;
}



CameraParam::CameraParam(VidCamera &pData)
{
   // Copy configuration from VidCamera
    m_Conf = pData;

    // Initialize flags and variables
    m_bOnvifUrlGetted = false;
    m_bHasSubStream = false;
    m_Online = false;
    m_OnlineUrl = false;
}

std::string Replace(std::string &str, const char *string_to_replace, const char *new_string)
{
  // Find the first string to replace
  int index = str.find(string_to_replace);
  // while there is one
  while(index != std::string::npos)
  {
    // Replace it
    str.replace(index, strlen(string_to_replace), new_string);
    // Find the next one
    index = str.find(string_to_replace, index + strlen(new_string));
  }
  return str;
}

bool CameraParam::CheckOnline()
{
    if (m_Conf.ntype()== VID_FILE 
		|| m_Conf.ntype() == VID_RTSP 
		|| m_Conf.ntype() == VID_MJPEG )
    {
    	return true;
    }
    string IP = m_Conf.strip();
    string Port = m_Conf.strport();
    string User = m_Conf.struser();
    string Password = m_Conf.strpasswd();
    string OnvifAddress = m_Conf.stronvifaddress();

    string OnvifCameraService = "http://" + IP + ":" + Port + OnvifAddress;
	string url = "rtsp://" + IP + ":" + "554" + "/";
     VDC_DEBUG( "%s  TryCheckDevice Begin \n", __FUNCTION__);
     if (TryCheckDevice(IP, Port) == false)
     {
       VDC_DEBUG( "%s TryCheckDevice False \n", __FUNCTION__);    
	return false;
     }

     if (VVidOnvifC::CheckOnline(User, Password, OnvifCameraService) == true)
     {
         return true;
     }

    return false;
}

bool CameraParam::UpdateUrlOnvif()
{
	bool bGotUrl = false;
	string IP = m_Conf.strip();
	string Port = m_Conf.strport();
	string User = m_Conf.struser();
	string Password = m_Conf.strpasswd();
	string OnvifAddress = m_Conf.stronvifaddress();

	string OnvifCameraService = "http://" + IP + ":" + Port + OnvifAddress;
	string url = "rtsp://" + IP + ":" + "554" + "/";
	string urlSubStream = "rtsp://" + IP + ":" + "554" + "/";
	VVidOnvifProfileMap profileMap;

	if (VVidOnvifC::GetProfiles(User, Password, OnvifCameraService, profileMap) == true)
	{
		VDC_DEBUG( "%s new Got profiles size %d \n",__FUNCTION__, profileMap.size());
	}else
	{
		VDC_DEBUG( "%s %s Get Stream url failed \n",__FUNCTION__, 
					OnvifCameraService.c_str());
		return false;
	}
	
	if (profileMap.size() > 0)
	{
		string strToken;
	    	VDC_DEBUG( "%s m_toKenPro size %d \n",__FUNCTION__, profileMap.size());
		if (m_Conf.bprofiletoken() == true)
		{
			strToken = m_Conf.strprofiletoken1().c_str();
			//Find which token is in the OnvifProfileToken, and then use the token
		}else
		{
			strToken = profileMap.begin()->first;
			m_Conf.set_strprofiletoken1(strToken.c_str());
		}

		VVidOnvifProfileMap::iterator it;
		it = profileMap.find(strToken);
		if (it != profileMap.end())
		{
			if ((*it).second.bGotUrl == true)
			{
				m_strUrl = (*it).second.strRtspUrl;
				if (m_strUrl.size() > 0)
				{
					bGotUrl = true;
				}
			}
		}
	}

	if (profileMap.size() > 1)
	{
		string strToken;
	    	VDC_DEBUG( "%s m_toKenPro size %d \n",__FUNCTION__, profileMap.size());
		if (m_Conf.bprofiletoken() == true)
		{
			strToken = m_Conf.strprofiletoken2().c_str();
			//Find which token is in the OnvifProfileToken, and then use the token
		}else
		{
			VVidOnvifProfileMap::iterator it = profileMap.begin();
			it ++;
			strToken = (*it).first;
			m_Conf.set_strprofiletoken2(strToken.c_str());
		}

		VVidOnvifProfileMap::iterator it;
		it = profileMap.find(strToken);
		if (it != profileMap.end())
		{
			if ((*it).second.bGotUrl == true)
			{
				m_strUrlSubStream = (*it).second.strRtspUrl;
				if (m_strUrlSubStream.size() > 0)
				{
					bGotUrl = true;
					m_bHasSubStream = true;
				}
			}
		}
	}

	/* Cache the stream profile list */
	VVidOnvifProfileMap::iterator it = profileMap.begin();
	for ( int i=0; i!=profileMap.size(); ++i, ++ it)
	{
		VidStream *pNewStream = m_cStreamList.add_cvidstream();
		
		pNewStream->set_strname((*it).second.strNameDisplay);
		pNewStream->set_strtoken((*it).first);
	}

	/* rtsp://admin:12345@192.168.1.1:554/Streaming/Channels/1\
	?transportmode=unicast&profile=Profile_1 */
	//string urlWithUser = "rtsp://" + User + ":" + Password + "@";
	//Replace(strUrl, "rtsp://", urlWithUser.c_str());

	if (bGotUrl == true)
	{
		m_bOnvifUrlGetted = true;
	}
	
	return bGotUrl;
}

bool CameraParam::UpdateUrl()
{
#if 0	
    //TODO RTSP ONVIF call onvif sdk to get a Stream URL
    if (m_Conf.ntype()== VID_FILE )
    {
        m_strUrl = m_Conf.strfile();
	 m_bHasSubStream = false;
    }

    if (m_Conf.ntype()== VID_RTSP
	|| m_Conf.ntype()== VID_MJPEG)
    {
	Poco::URI rtspUrl(m_Conf.strrtspurl());
	string strRtsp;
	if (rtspUrl.empty() != true)
	{
		strRtsp = rtspUrl.getScheme() + "://" + rtspUrl.getHost() + ":" + std::to_string(rtspUrl.getPort()) + rtspUrl.getPathAndQuery();
	}

	m_strUrl = strRtsp;
	m_bHasSubStream = false;

    }

    if (m_Conf.ntype()== VID_ONVIF_S)
    {
        return UpdateUrlOnvif();
    }
    VDC_DEBUG( "%s url %s\n",__FUNCTION__, m_strUrl.c_str());
#endif
    return true;
}

CameraParam::CameraParam(const CameraParam &pParam)
{
	m_Conf = pParam.m_Conf;

	m_bOnvifUrlGetted = pParam.m_bOnvifUrlGetted;
	m_bHasSubStream = pParam.m_bHasSubStream;
	m_Online = pParam.m_Online;
	m_OnlineUrl = pParam.m_OnlineUrl;
}


CameraParam::~CameraParam()
{
}

Camera::Camera(ConfDB &pConfDB, VDB &pVdb, VHdfsDB &pVHdfsdb, 
	const CameraParam &pParam, RecChangeFunctionPtr pCallback, void *pCallbackParam)
:m_bStarted(false), m_param(pParam),
m_pVdb(pVdb), 
m_ptzInited(false), 
m_bGotInfoData(false), m_nDataRef(0), m_bGotInfoSubData(false),
m_nSubDataRef(0), 
m_pvPlay(new VPlay), m_pvPlaySubStream(new VPlay), 
m_vPlay(*m_pvPlay), m_vPlaySubStream(*m_pvPlaySubStream), 
m_pConfDB(pConfDB),
m_cRecordWrapper(pVdb, pParam.m_Conf.strid(), m_cSchedMap, pCallback, pCallbackParam)
{	
	m_param = pParam;
	m_param.UpdateDefaultUrl();
	UpdateRecSched(m_param.m_Conf);

	memset(&m_iFrameCache, 0, sizeof(m_iFrameCache));

	return;
}

Camera::~Camera()
{
	/* stop all the data  */
	Lock();
	SubLock();

	CameraDelCallbackMap::iterator it = m_DelMap.begin();

	for(; it!=m_DelMap.end(); ++it)
	{
	    void *pParam = (*it).first;
	    CameraDelCallbackFunctionPtr pFunc = (*it).second;
	    if (pFunc)
	    {
	        pFunc(pParam);
	    }
	}
	SubUnLock();
	UnLock();

	m_vPlay.StopGetData();
	m_vPlaySubStream.StopGetData();

	delete m_pvPlay;
	delete m_pvPlaySubStream;

	if (m_iFrameCache.dataBuf != NULL)
	{
		free(m_iFrameCache.dataBuf);
		m_iFrameCache.dataBuf = NULL;
	}
}

bool Camera::UpdateRecSched(VidCamera &pCam)
{
	m_cSchedMap.clear();

	string strSched = pCam.strsched();

	if (RecordSchedWeek::CheckStringForSched(strSched) == false)
	{
		return false;
	}
		
	RecordSchedWeek sched(strSched);

	m_cSchedMap["default"] = sched;
	
	m_cRecordWrapper.UpdateSchedMap(m_cSchedMap);

	return true;
	
}


bool Camera::GetCameraParam(CameraParam &pParam)
{
	pParam = m_param;
	return true;
}

CameraStatus Camera::CheckCamera(string strUrl, string strUrlSubStream, 
		bool bHasSubStream, bool bOnline, 
		bool bOnlineUrl, VidStreamList cStreamlist)
{
    if (bOnline == true)
    {
        /* Camera from offline to online */
        if (m_param.m_OnlineUrl == false)
        {
        	bool HWAccel = false;
		if (m_param.m_Conf.bhwaccel()== true)
		{
			HWAccel = true;
		}
		if (bOnlineUrl == false)
		{
		    return  DEV_NO_CHANGE;
		}
		m_param.m_strUrl = strUrl;
		m_param.m_strUrlSubStream = strUrlSubStream;
		m_param.m_bHasSubStream = bHasSubStream;
		if (m_param.m_Conf.ntype()== VID_FILE)
		{
			m_vPlay.Init(m_param.m_strUrl, false, "fake", "fake", HWAccel);
		}else if (m_param.m_Conf.bmotionjpeg()== true
		  || m_param.m_Conf.ntype()== VID_MJPEG)
		{
			/* Motion JPEG */
			m_vPlay.Init(m_param.m_strUrl, true, m_param.m_Conf.struser(),
				m_param.m_Conf.strpasswd(), HWAccel);
		}
		else
		{
			m_vPlay.Init(true, m_param.m_strUrl, m_param.m_Conf.struser(),
				m_param.m_Conf.strpasswd(), HWAccel, 
				(VSCConnectType)(m_param.m_Conf.nconnecttype()));
			VDC_DEBUG( "%s url %s\n",__FUNCTION__, m_param.m_strUrl.c_str());
			if (m_param.m_bHasSubStream == true)
			{
				m_vPlaySubStream.Init(true, m_param.m_strUrlSubStream, m_param.m_Conf.struser(),
					m_param.m_Conf.strpasswd(), HWAccel,
					(VSCConnectType)(m_param.m_Conf.nconnecttype()));
				VDC_DEBUG( "%s url %s\n",__FUNCTION__, m_param.m_strUrlSubStream.c_str());
			}
		}
		VDC_DEBUG( "%s url %s\n",__FUNCTION__, m_param.m_strUrl.c_str());
		
		m_param.m_OnlineUrl = true;
		UpdatePTZConf();
		m_param.m_cStreamList = cStreamlist;
		/* Always start data */
		StartData();
        }
        if (m_param.m_Online == false)
        {
            m_param.m_Online = true;
            return DEV_OFF2ON;
        }
    }else
    {
        if (m_param.m_Online == true)
        {
            m_param.m_Online = false;
            return DEV_ON2OFF;
        }
    }

    return DEV_NO_CHANGE;
    
}

 bool Camera::UpdatePTZConf()
{
	if (m_param.m_Conf.ntype()!= VID_ONVIF_S)
	{
		return true;
	}

	string IP = m_param.m_Conf.strip();
	string Port = m_param.m_Conf.strport();
	string User = m_param.m_Conf.struser();
	string Password = m_param.m_Conf.strpasswd();
	string OnvifAddress = m_param.m_Conf.stronvifaddress();

	string OnvifCameraService = "http://" + IP + ":" + Port + OnvifAddress;

	m_ptz.Init(User, Password, OnvifCameraService);

	m_ptzInited = true;
	return true;
}

 bool Camera::PtzAction(FPtzAction action, float speed)
{
	if (m_param.m_Online == false || m_ptzInited == false) {
		VDC_DEBUG( "%s PTZ Camera is Offline\n",__FUNCTION__);
		return true;
	}
	m_ptz.PtzAction((VVidOnvifCPtzAct)action, speed);

	return true;
}

bool Camera::FireAlarm(long nStartTime)
{	
	return m_cRecordWrapper.FireAlarm(nStartTime);
}

bool Camera::GetInfoFrame(InfoFrame &pFrame)
{
	Lock();
	if (m_bGotInfoData == true) {
		memcpy(&pFrame, &m_infoData, sizeof(InfoFrame));
		UnLock();
		return true;
	}
	UnLock();
	return false;
}
bool Camera::GetSubInfoFrame(InfoFrame &pFrame)
{
	Lock();
	if (m_bGotInfoSubData == true) {
		memcpy(&pFrame, &m_infoSubData, sizeof(InfoFrame));
		UnLock();
		return true;
	}
	UnLock();
	return false;
}

 bool Camera::RegDataCallback(CameraDataCallbackFunctionPtr pCallback, void * pParam)
{
	Lock();
	m_DataMap[pParam] = pCallback;
	UnLock();
	StartData();
	return true;
}

bool Camera::UnRegDataCallback(void * pParam)
{
	Lock(); // Acquire lock for thread safety
	m_DataMap.erase(pParam);  // Remove callback associated with pParam from the map
	UnLock(); // Release lock
	StopData(); // Stop data streaming if there are no remaining registered callbacks
	return true;
}

bool Camera::RegSubDataCallback(CameraDataCallbackFunctionPtr pCallback, void * pParam)
{
	Lock(); // Acquire lock for thread safety
    m_SubDataMap[pParam] = pCallback; // Register the callback associated with pParam
    UnLock(); // Release lock
    // Start data streaming based on the presence of a substream
    if (m_param.m_bHasSubStream == false) {
        StartData();
    } else {
        StartSubData();
    }
    return true;
}

bool Camera::UnRegSubDataCallback(void * pParam)
{
	Lock();
	m_SubDataMap.erase(pParam);
	UnLock();
	if (m_param.m_bHasSubStream == false) {
		StopData();
	}else
	{
		StopSubData();
	}
	return true;
}

bool Camera::RegDelCallback(CameraDelCallbackFunctionPtr pCallback, void * pParam)
{
	Lock();
	m_DelMap[pParam] = pCallback;
	UnLock();
	return true;
}
bool Camera::UnRegDelCallback(void * pParam)
{
	Lock();
	m_DelMap.erase(pParam);
	UnLock();
	return true;
}

bool Camera::StartData()
{
	Lock();
	if (m_param.m_OnlineUrl == true) {
		if (m_nDataRef == 0) {
			m_vPlay.StartGetData(this, (VPlayDataHandler)Camera::DataHandler);
		}
		m_nDataRef ++;
		printf("%s m_nDataRef %d\n", __FUNCTION__, m_nDataRef);
	}
	UnLock();
	return true;
}
bool Camera::StopData()
{
	Lock();
	m_nDataRef --;
	printf("%s m_nDataRef %d\n", __FUNCTION__, m_nDataRef);
	if (m_nDataRef <= 0) {
		m_nDataRef = 0;
		m_vPlay.StopGetData();
	}

	UnLock();
	return true;
}

 bool Camera::StartSubData()
{
	Lock();
	if (m_param.m_OnlineUrl == true) {
		if (m_nSubDataRef == 0) {
			m_vPlaySubStream.StartGetData(this, (VPlayDataHandler)Camera::SubDataHandler);
		}
		m_nSubDataRef ++;
 	}
	UnLock();
	return true;
}
 bool Camera::StopSubData()
{
	Lock();
	m_nSubDataRef --;
	if (m_nSubDataRef <= 0) {
		m_nSubDataRef = 0;
		m_vPlaySubStream.StopGetData();
	}

	UnLock();
	return true;
}

bool Camera::DataHandler(void* pData, VideoFrame& frame)
{
    LPCamera pThread = (LPCamera)pData;
    if (pThread) {
        return pThread->DataHandler1(frame);
    }
	return false;
}


/* the frame buffer is alloc here, need free by the application */
bool Camera::GetiFrame(VideoFrame& frame)
{
	Lock();
	if (m_iFrameCache.dataBuf == NULL) {
		UnLock();
		return false;
	}

	frame.dataBuf = (u8 *)malloc(m_iFrameCache.bufLen);
	frame.dataLen = m_iFrameCache.dataLen;

	frame.streamType = m_iFrameCache.streamType;
	frame.frameType = m_iFrameCache.frameType;
	frame.dataLen = m_iFrameCache.dataLen;
	memcpy(frame.dataBuf, m_iFrameCache.dataBuf, m_iFrameCache.dataLen);
	frame.secs = m_iFrameCache.secs;
	frame.msecs = m_iFrameCache.msecs;
	
	UnLock();

	return true;
}

bool Camera::DataHandler1(VideoFrame& frame)
{
	//VDC_DEBUG( "%s  %d\n",__FUNCTION__, frame.dataLen);
	Lock();
	/* Frist cache the info frame */
	if (frame.streamType == VIDEO_STREAM_INFO) {
		memcpy(&m_infoData, frame.dataBuf, sizeof(InfoFrame));
		m_bGotInfoData = true;
	}

	/* Cache all the I frame */
	if (frame.frameType == VIDEO_FRM_I) {
		if (m_iFrameCache.bufLen < frame.dataLen) {
			if (m_iFrameCache.dataBuf) {
				free(m_iFrameCache.dataBuf);
				m_iFrameCache.dataBuf = NULL;
			}
			m_iFrameCache.dataBuf = (u8 *)malloc(frame.dataLen);
			m_iFrameCache.bufLen = frame.dataLen;
		}
		m_iFrameCache.streamType = frame.streamType;
		m_iFrameCache.frameType = frame.frameType;
		m_iFrameCache.dataLen = frame.dataLen;
		memcpy(m_iFrameCache.dataBuf, frame.dataBuf, frame.dataLen);
		m_iFrameCache.secs = frame.secs;
		m_iFrameCache.msecs = frame.msecs;
	}
	
	/* 1. Send to network client */
	CameraDataCallbackMap::iterator it = m_DataMap.begin();

	for(; it!=m_DataMap.end(); ++it) {
	    void *pParam = (*it).first;
	    CameraDataCallbackFunctionPtr pFunc = (*it).second;
	    if (pFunc) {
	        pFunc(frame, pParam);
	    }
	}

	m_cRecordWrapper.PushAFrame(frame);

	/* Call the Sub DataHandler if there has no sub stream */
	if (m_param.m_bHasSubStream == false) {
		SubDataHandler1(frame);
	}
	UnLock();
	return true;
}

bool Camera::SubDataHandler(void* pData, VideoFrame& frame)
{
    LPCamera pThread = (LPCamera)pData;

    if (pThread){
        return pThread->SubDataHandler1(frame);
    }
	return false;
}

bool Camera::SubDataHandler1(VideoFrame& frame)
{
	SubLock();
	/* Frist cache the info frame */
	if (frame.streamType == VIDEO_STREAM_INFO) {
		memcpy(&m_infoSubData, frame.dataBuf, sizeof(InfoFrame));
		m_bGotInfoSubData = true;
	}
	
	/* 1. Send to client */
	CameraDataCallbackMap::iterator it = m_SubDataMap.begin();

	for(; it!=m_SubDataMap.end(); ++it)
	{
	    void *pParam = (*it).first;
	    CameraDataCallbackFunctionPtr pFunc = (*it).second;
	    if (pFunc) {
	        pFunc(frame, pParam);
	    }
	}

	SubUnLock();
	return true;
}

bool Camera::GetCameraOnline()
{
  	return m_param.m_Online;
}

bool Camera::GetStreamInfo(VideoStreamInfo &pInfo)
{
    m_vPlay.GetStreamInfo(pInfo);

    return true;
}


bool Camera::GetCamStreamList(VidStreamList &pList)
{
	pList =  m_param.m_cStreamList;
	return true;
}
		


bool Camera::AttachPlayer(HWND hWnd, int w, int h)
{
    m_vPlay.AttachWidget(hWnd, w, h);

    return true;
}

bool Camera::UpdateWidget(HWND hWnd, int w, int h)
{
    m_vPlay.UpdateWidget(hWnd, w, h);

    return true;
}

bool Camera::DetachPlayer(HWND hWnd)
{
    m_vPlay.DetachWidget(hWnd);
    
    return true;
}

bool Camera::ShowAlarm(HWND hWnd)
{
	m_vPlay.ShowAlarm(hWnd);
	return true;
}
