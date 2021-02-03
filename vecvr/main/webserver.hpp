/** <!--
 *
 *  Copyright (C) 2017 veyesys support@veyesys.com
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  If you would like this software to be made available to you under an 
 *  alternate commercial license please email support@veyesys.com 
 *  for more information.
 *
 * -->
 */
#ifndef __VE_WEB_SERVER_H__
#define __VE_WEB_SERVER_H__

#include "CivetServer.h"
#include "vapi/vapiimage.hpp"
#include "vapi/vapisystem.hpp"
#include "vapi/vapicamlist.hpp"
#include "vapi/vwsapi.hpp"
#include "link/wslink.hpp"

#ifdef WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#define DOCUMENT_ROOT "."
#define PORT "8080"

class VEWebServer 
{
public:
	VEWebServer(std::vector<std::string> cpp_options, Factory &pFactory, 
			VEventServer &pEvent)
		:pServer(new CivetServer(cpp_options)), server(*pServer), m_pFactory(pFactory), 
		h_GetCamList(pFactory), h_GetImage(pFactory), h_vwsapi(pFactory), 
		h_wslink(pFactory, pEvent),h_System(pFactory),
		h_wslinkStream(pFactory), h_GetStreamUrl(pFactory)
	{
		/* restful api */
		VEWebAPIGetImage();
		VEWebAPIGetStream();
		VEWebAPIGetCamList();
		VEWebAPISystemV1();
		/* websocket API */
		VEAddWebSocket();
		server.addAuthHandler(LINK_PROTO_WS_PATH, h_wslinkAuth);
		server.addAuthHandler(LINK_PROTO_WS_STREAM_PATH, h_wslinkStreamAuth);		
		
	}

	~VEWebServer(){}
private:

	void VEAddWebSocket(){
		server.addWebSocketHandler(LINK_PROTO_WS_PATH, h_wslink);
		server.addWebSocketHandler(LINK_PROTO_WS_STREAM_PATH, h_wslinkStream);
	    //Below is only for test
		server.addWebSocketHandler("/vwsapi", h_vwsapi);
	}
	void 
	VEWebAPISystemV1(){
            server.addHandler(VAPI_LOGIN_SYSTEM, h_System);
            server.addHandler(VAPI_KEEPALIVE_SYSTEM, h_System);
            server.addHandler(VAPI_LOGOUT_SYSTEM, h_System);
            server.addHandler(VAPI_GET_SYSTEM_INFO, h_System);
            server.addHandler(VAPI_GET_CODEC_INFO, h_System);
            server.addHandler(VAPI_GET_RUN_INFO, h_System);
            server.addHandler(VAPI_GET_DEVICE_SUMMARY, h_System);
            server.addHandler(VAPI_GET_VOLUMES_SYSTEM, h_System);
	}
	void
	VEWebAPIGetCamList(){
            server.addHandler("/vapi/GetCamList", h_GetCamList);
	}
	void
	VEWebAPIGetStream(){
           server.addHandler("/vapi/GetStreamUrl", h_GetStreamUrl);
	}
	void
	VEWebAPIGetImage(){
           server.addHandler("/vapi/GetImage", h_GetImage);
	}
private:
	
	WebAPIGetCamListHandler h_GetCamList;
	WebAPIGetImageHandler h_GetImage;
	WebAPIGetStreamUrlHandler h_GetStreamUrl;
	WebAPISystemHandler h_System;
	
	VwsAPI h_vwsapi;
	WSLink h_wslink;
	WSLinkStream h_wslinkStream;
	WSLinkAuth h_wslinkAuth;
	WSLinkAuth h_wslinkStreamAuth;
	CivetServer *pServer;
	CivetServer &server;
	Factory &m_pFactory;
};

#endif /* __VE_WEB_SERVER_H__ */
