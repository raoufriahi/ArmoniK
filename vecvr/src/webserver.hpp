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
    /**
     * \brief Constructor for the VEWebServer class.
     *
     * Initializes a VEWebServer object with the provided parameters and sets up
     * various RESTful API and WebSocket handlers.
     *
     * \param cpp_options The options for the CivetServer instance.
     * \param pFactory A reference to the Factory object.
     * \param pEvent A reference to the VEventServer object.
     */
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

    /**
     * \brief Adds WebSocket handlers to the server.
     *
     * This function adds WebSocket handlers to the server for handling WebSocket
     * connections. It sets up handlers for the LINK_PROTO_WS_PATH and
     * LINK_PROTO_WS_STREAM_PATH paths. Additionally, a test handler is added for
     * the "/vwsapi" path.
     */
	void VEAddWebSocket(){
		server.addWebSocketHandler(LINK_PROTO_WS_PATH, h_wslink);
		server.addWebSocketHandler(LINK_PROTO_WS_STREAM_PATH, h_wslinkStream);
	    //Below is only for test
		server.addWebSocketHandler("/vwsapi", h_vwsapi);
	}

	/**
     * \brief Registers handlers for various system-related endpoints.
     *
     * This function adds multiple handler functions to the server for various system-related endpoints.
     * Each handler function is associated with a specific endpoint and will be called when a request is made to that endpoint.
     * The registered endpoints and their corresponding handler functions are as follows:
     *   - VAPI_LOGIN_SYSTEM:       h_System
     *   - VAPI_KEEPALIVE_SYSTEM:   h_System
     *   - VAPI_LOGOUT_SYSTEM:      h_System
     *   - VAPI_GET_SYSTEM_INFO:    h_System
     *   - VAPI_GET_CODEC_INFO:     h_System
     *   - VAPI_GET_RUN_INFO:       h_System
     *   - VAPI_GET_DEVICE_SUMMARY: h_System
     *   - VAPI_GET_VOLUMES_SYSTEM: h_System
     *
     * \see h_System
     */
	void VEWebAPISystemV1(){
        server.addHandler(VAPI_LOGIN_SYSTEM, h_System);
        server.addHandler(VAPI_KEEPALIVE_SYSTEM, h_System);
        server.addHandler(VAPI_LOGOUT_SYSTEM, h_System);
        server.addHandler(VAPI_GET_SYSTEM_INFO, h_System);
        server.addHandler(VAPI_GET_CODEC_INFO, h_System);
        server.addHandler(VAPI_GET_RUN_INFO, h_System);
        server.addHandler(VAPI_GET_DEVICE_SUMMARY, h_System);
        server.addHandler(VAPI_GET_VOLUMES_SYSTEM, h_System);
	}
	
	/**
     * \brief Registers a handler for the "/vapi/GetCamList" endpoint.
     *
     * This function adds a handler function, `h_GetCamList`, to the server for the "/vapi/GetCamList" endpoint.
     * The handler function will be called when a request is made to the specified endpoint.
     *
     * \see h_GetCamList
     */
	void VEWebAPIGetCamList(){
        server.addHandler("/vapi/GetCamList", h_GetCamList);
	}

	/**
     * \brief Registers a handler for the "/vapi/GetStreamUrl" endpoint.
     *
     * This function adds a handler function, `h_GetStreamUrl`, to the server for the "/vapi/GetStreamUrl" endpoint.
     * The handler function will be called when a request is made to the specified endpoint.
     *
     * \see h_GetStreamUrl
     */
	void VEWebAPIGetStream(){
        server.addHandler("/vapi/GetStreamUrl", h_GetStreamUrl);
	}

	/**
     * \brief Registers a handler function for the "/vapi/GetImage" endpoint.
     *
     * This function adds a handler function to the server for the "/vapi/GetImage" endpoint.
     *
     * \note The handler function should be implemented separately.
     *
     * \see h_GetImage
     */
	void VEWebAPIGetImage(){
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
