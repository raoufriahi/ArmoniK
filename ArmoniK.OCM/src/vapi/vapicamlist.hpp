/*
 * Copyright (c) 2017-2023 UbVideo
 *
 * The computer program contained herein contains proprietary
 * information which is the property of UbVideo.
 * The program may be used and/or copied only with the written
 * permission of UbVideo or in accordance with the
 * terms and conditions stipulated in the agreement/contract under
 * which the programs have been supplied.
 */
#pragma once
#include "onvifclidis.hpp"

/* /vapi/GetCamList */
class WebAPIGetCamListHandler : public CivetHandler
{
public:
	WebAPIGetCamListHandler(Factory &pFactory) :m_pFactory(pFactory) {

	}

   	bool handleGet(CivetServer *pServer, mg_connection *pConnection) {
		return handleAll("GET", pServer, pConnection);
	}

	bool handlePost(CivetServer *pServer, mg_connection *pConnection) {
		return handleAll("POST", pServer, pConnection);
	}

private:
	bool handleAll(const char *method, CivetServer *pServer, mg_connection *pConnection) {
		CameraOnlineMap pCameraOnlineMap;
		CameraRecMap pCameraRecMap;

		m_pFactory.GetCameraOnlineMap(pCameraOnlineMap);
		m_pFactory.GetCameraRecMap(pCameraRecMap);

		VidCameraList pCameraList;
		m_pFactory.GetCameraList(pCameraList);

		for (s32 i = 0; i < pCameraList.cvidcamera_size(); i ++) {
			VidCamera &cam = (VidCamera &)(pCameraList.cvidcamera(i));
			cam.set_strpasswd("******");
		}
		std::string strMsg;
		::google::protobuf::util::Status status = 
			::google::protobuf::util::MessageToJsonString(pCameraList, &strMsg);
        s32 nJsonLen = strMsg.length();
		if (nJsonLen <= 0) {
			return true;
		}

		mg_printf(pConnection,
		          "HTTP/1.1 200 OK\r\nContent-Type: "
		          "application/json\r\n"
				  "Content-Length: %d\r\n\r\n", nJsonLen);
		mg_printf(pConnection,"%s", strMsg.c_str());
		return true;
	}

private:

	Factory &m_pFactory;
};


class WebAPIGetStreamUrlHandler : public CivetHandler
{
public:
	WebAPIGetStreamUrlHandler(Factory &pFactory)
		:m_pFactory(pFactory)
	{

	}
private:
	bool
	handleAll(const char *method,
	          CivetServer *pServer,
	          mg_connection *pConnection)
	{
		u8 * pBuf=NULL;
		int nLen= 0;
		std::string strCamera = "";
		if (CivetServer::getParam(pConnection, "Camera", strCamera)  == true)
		{
			VidStreamUrlList UrlList;
			std::vector<std::string> ipList = OnvifDisClient::GetInterfaces();
			if (ipList.size() <= 0)
			{
				printf("No network adaptor found !!!\n");
				return false;
			}
			std::vector<std::string>::iterator it;
			for (it = ipList.begin(); it != ipList.end(); ++it)
			{
				printf("Network %s\n", (*it).c_str());

				//RTSP Url
				{
					VidStreamUrl *pUrl = UrlList.add_curl();
					pUrl->set_strprotocol("RTSP");
					string rtspUrl = "rtsp://" + (*it) + ":10554/live/" + strCamera;
					pUrl->set_strurl(rtspUrl);
				}
				//RTMP Url
				{
					VidStreamUrl *pUrl = UrlList.add_curl();
					pUrl->set_strprotocol("RTMP");
					string rtmpUrl = "rtmp://" + (*it) + ":11935/live/" + strCamera;
					pUrl->set_strurl(rtmpUrl);
				}

				
				//HLS Url
				{
					VidStreamUrl *pUrl = UrlList.add_curl();
					pUrl->set_strprotocol("HLS");
					string hlsUrl = "http://" + (*it) + ":10080/live/" + strCamera + "/hls.m3u8";
					pUrl->set_strurl(hlsUrl);
				}
			}

			std::string strMsg;
			::google::protobuf::util::Status status = 
				::google::protobuf::util::MessageToJsonString(UrlList, &strMsg);

			s32 nJsonLen = strMsg.length();
			if (nJsonLen <= 0)
			{

				return true;
			}

			std::string s = "";
			mg_printf(pConnection,
			          "HTTP/1.1 200 OK\r\nContent-Type: "
			          "application/json\r\n"
					  "Content-Length: %d\r\n\r\n", nJsonLen);
			mg_printf(pConnection,"%s", strMsg.c_str());
			
			
			
		}else
		{
			mg_printf(pConnection,
			          "HTTP/1.1 200 OK\r\nContent-Type: "
			          "text/plain\r\nConnection: close\r\n\r\n");
			mg_printf(pConnection, "Can't Get Stream Url!\n");			
		}

		return true;
	}

  public:
	bool
	handleGet(CivetServer *pServer, mg_connection *pConnection)
	{
		return handleAll("GET", pServer, pConnection);
	}
	bool
	handlePost(CivetServer *pServer, mg_connection *pConnection)
	{
		return handleAll("POST", pServer, pConnection);
	}
private:
	Factory &m_pFactory;
};
