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
#include <google/protobuf/util/json_util.h>
#include "utility/type.hpp"
#include "config/linkproto.pb.h"
#include "config/linksystem.pb.h"
#include "config/vidconf.pb.h"
#include "config/restsystem.pb.h"
#include "XSDK/XMD5.h"
#include "server/eventserver.hpp"
#include "onvifclidis.hpp"
#include "CivetServer.h"
#include "link/linkhandler.hpp"

/**
 * \brief Constructor for the LinkHandler class.
 *
 * Initializes a new instance of the LinkHandler class with the provided Factory
 * and VEventServer objects. Sets various member variables to their initial values,
 * including generating a random UUID for the session ID using UUIDGenerator.
 *
 * \param pFactory Reference to the Factory object used for initialization.
 * \param pEvent Reference to the VEventServer object for event handling.
 */
LinkHandler::LinkHandler(Factory &pFactory, VEventServer &pEvent)
    : m_pFactory(pFactory),          /**< Reference to the Factory object. */
      m_bLogin(false),               /**< Flag indicating login status. */
      m_bRegNotify(false),           /**< Flag indicating registration notification status. */
      m_server(NULL),                /**< Pointer to the server object (initially set to NULL). */
      m_conn(NULL),                  /**< Pointer to the connection object (initially set to NULL). */
      m_bRealEvent(false),           /**< Flag indicating real event status. */
      m_bSearchEvent(false),         /**< Flag indicating search event status. */
      m_pEvent(pEvent),              /**< Reference to the VEventServer object. */
      m_pCamSearch(NULL)             /**< Pointer to the camera search object (initially set to NULL). */
{
    // Generate a random UUID for the session ID
    UUIDGenerator uuidCreator;
    m_seesionId = uuidCreator.createRandom().toString();
}

/**
 * \brief Destructor for the LinkHandler class.
 *
 * The destructor handles the cleanup tasks when an instance of the LinkHandler class
 * is being destroyed. It checks the status of registration flags and unregisters
 * from corresponding event notifications provided by Factory and VEventServer.
 */
LinkHandler::~LinkHandler()
{
    // Unregister from camera change notifications if registered
    if (m_bRegNotify == true) {
        m_pFactory.UnRegCameraChangeNotify((void *)this);
    }

    // Unregister from search event notifications if registered
    if (m_bSearchEvent == true) {
        m_pEvent.UnRegSearchEventNotify((void *)this);
    }

    // Unregister from real event notifications if registered
    if (m_bRealEvent == true) {
        m_pEvent.UnRegEventNotify((void *)this);
    }
}

/**
 * \brief Initiates a call to the CallChange1 method of a LinkHandler instance.
 *
 * This method is typically used as a callback function for handling camera change events.
 * It retrieves the LinkHandler instance from the provided void pointer and calls the
 * CallChange1 method, passing the FactoryCameraChangeData as a parameter.
 *
 * \param pParam A void pointer to the LinkHandler instance.
 * \param data The FactoryCameraChangeData containing information about the camera change event.
 * \return Returns the boolean result of the CallChange1 method if the LinkHandler instance is valid;
 *         otherwise, false is returned.
 */
bool LinkHandler::CallChange(void* pParam, FactoryCameraChangeData data)
{
    // Save the current value of errno (error number) in case it is needed later
    int dummy = errno;
    // Cast the void pointer to a LinkHandler instance
    LinkHandler * pObject = (LinkHandler *)pParam;
    // Check if the LinkHandler instance is valid
    if (pObject) {
        // Call the CallChange1 method of the LinkHandler instance with the provided data
        return pObject->CallChange1(data);
    }
    // Return false if the LinkHandler instance is not valid
    return false;
}

/**
 * \brief Handles camera change events based on the provided FactoryCameraChangeData.
 *
 * This method is called when a camera change event occurs, and it processes the event
 * based on the event type specified in the FactoryCameraChangeData. If the LinkHandler
 * instance is not registered or if the necessary objects are not initialized, it returns true.
 *
 * \param data The FactoryCameraChangeData containing information about the camera change event.
 * \return Returns true if the LinkHandler instance is not registered or if the necessary objects
 *         are not initialized; otherwise, returns the boolean result of the specific camera change
 *         notification method (e.g., NotifyCamAdd, NotifyCamDel) based on the event type.
 */
bool LinkHandler::CallChange1(FactoryCameraChangeData data)
{
    // Check if the server, connection, and registration flags are valid
    if (m_server == NULL || m_conn == NULL || m_bRegNotify == false) {
        return true;
    }

    // Process the camera change event based on the event type
    switch (data.type) {
        case FACTORY_CAMERA_ADD:
            return NotifyCamAdd(data);
            break;
        case FACTORY_CAMERA_DEL:
            return NotifyCamDel(data);
            break;
        case FACTORY_CAMERA_ONLINE:
            return NotifyCamOnline(data);
            break;
        case FACTORY_CAMERA_OFFLINE:
            return NotifyCamOffline(data);
            break;
        case FACTORY_CAMERA_REC_ON:
            return NotifyCamRecOn(data);
            break;
        case FACTORY_CAMERA_REC_OFF:
            return NotifyCamRecOff(data);
            break;
        default:
            break;
    }

    // Return false if the event type is not recognized (should not reach this point)
    return false;
}

/**
 * \brief Notifies the addition of a camera and sends the corresponding response message.
 *
 * This method is called when a camera addition event is received. It creates a LinkCmd response
 * of type LINK_CMD_CAM_ADD_NOTIFY and constructs a LinkCamAddNotify message containing
 * information about the added camera. The response message is then sent using the SendRespMsg method.
 *
 * \param data The FactoryCameraChangeData containing information about the added camera.
 * \return Returns true if the camera addition notification is successfully sent; otherwise, false.
 */
bool LinkHandler::NotifyCamAdd(FactoryCameraChangeData data)
{
    // Create a LinkCmd response of type LINK_CMD_CAM_ADD_NOTIFY
    Link::LinkCmd cmdResp;
    cmdResp.set_type(Link::LINK_CMD_CAM_ADD_NOTIFY);

    // Create a LinkCamAddNotify message
    LinkCamAddNotify * resp = new LinkCamAddNotify;

    // Create a VidCamera object and retrieve camera configuration from the Factory's ConfDB
    VidCamera *pCam = new VidCamera;
    if (m_pFactory.GetConfDB().GetCameraConf(data.id, *pCam) == false) {
        delete resp; // Cleanup allocated resources before returning
        delete pCam;
        return false;
    }

    // Set the VidCamera object in the LinkCamAddNotify message
    resp->set_allocated_ccam(pCam);

    // Set the LinkCamAddNotify message in the LinkCmd response
    cmdResp.set_allocated_camaddnotify(resp);

    // Send the response message using the SendRespMsg method
    SendRespMsg(cmdResp, m_server, m_conn);

    // Return true to indicate successful notification
    return true;
}

/**
 * \brief Notifies the deletion of a camera and sends the corresponding response message.
 *
 * This method is called when a camera deletion event is received. It creates a LinkCmd response
 * of type LINK_CMD_CAM_DEL_NOTIFY and constructs a LinkCamIdNotify message containing
 * the identifier of the deleted camera. The response message is then sent using the SendRespMsg method.
 *
 * \param data The FactoryCameraChangeData containing information about the deleted camera.
 * \return Returns true if the camera deletion notification is successfully sent; otherwise, false.
 */
bool LinkHandler::NotifyCamDel(FactoryCameraChangeData data)
{
    // Create a LinkCmd response of type LINK_CMD_CAM_DEL_NOTIFY
    Link::LinkCmd cmdResp;
    cmdResp.set_type(Link::LINK_CMD_CAM_DEL_NOTIFY);

    // Create a LinkCamIdNotify message and set the camera identifier
    LinkCamIdNotify *resp = new LinkCamIdNotify;
    resp->set_strid(data.id);

    // Set the LinkCamIdNotify message in the LinkCmd response
    cmdResp.set_allocated_camidnotify(resp);

    // Send the response message using the SendRespMsg method
    SendRespMsg(cmdResp, m_server, m_conn);

    // Return true to indicate successful notification
    return true;
}

/**
 * \brief Notifies the online status of a camera and sends the corresponding response message.
 *
 * This method is called when a camera online event is received. It creates a LinkCmd response
 * of type LINK_CMD_CAM_ONLINE_NOTIFY and constructs a LinkCamIdNotify message containing
 * the identifier of the camera that came online. The response message is then sent using the
 * SendRespMsg method.
 *
 * \param data The FactoryCameraChangeData containing information about the camera that came online.
 * \return Returns true if the camera online notification is successfully sent; otherwise, false.
 */
bool LinkHandler::NotifyCamOnline(FactoryCameraChangeData data)
{
    // Create a LinkCmd response of type LINK_CMD_CAM_ONLINE_NOTIFY
    Link::LinkCmd cmdResp;
    cmdResp.set_type(Link::LINK_CMD_CAM_ONLINE_NOTIFY);

    // Create a LinkCamIdNotify message and set the camera identifier
    LinkCamIdNotify *resp = new LinkCamIdNotify;
    resp->set_strid(data.id);

    // Set the LinkCamIdNotify message in the LinkCmd response
    cmdResp.set_allocated_camidnotify(resp);

    // Send the response message using the SendRespMsg method
    SendRespMsg(cmdResp, m_server, m_conn);

    // Return true to indicate successful notification
    return true;
}

/**
 * \brief Notifies the offline status of a camera and sends the corresponding response message.
 *
 * This method is called when a camera offline event is received. It creates a LinkCmd response
 * of type LINK_CMD_CAM_OFFLINE_NOTIFY and constructs a LinkCamIdNotify message containing
 * the identifier of the camera that went offline. The response message is then sent using the
 * SendRespMsg method.
 *
 * \param data The FactoryCameraChangeData containing information about the camera that went offline.
 * \return Returns true if the camera offline notification is successfully sent; otherwise, false.
 */
bool LinkHandler::NotifyCamOffline(FactoryCameraChangeData data)
{
    // Create a LinkCmd response of type LINK_CMD_CAM_OFFLINE_NOTIFY
    Link::LinkCmd cmdResp;
    cmdResp.set_type(Link::LINK_CMD_CAM_OFFLINE_NOTIFY);

    // Create a LinkCamIdNotify message and set the camera identifier
    LinkCamIdNotify *resp = new LinkCamIdNotify;
    resp->set_strid(data.id);

    // Set the LinkCamIdNotify message in the LinkCmd response
    cmdResp.set_allocated_camidnotify(resp);

    // Send the response message using the SendRespMsg method
    SendRespMsg(cmdResp, m_server, m_conn);

    // Return true to indicate successful notification
    return true;
}

/**
 * \brief Notifies the recording activation of a camera and sends the corresponding response message.
 *
 * This method is called when a camera recording activation event is received. It creates a LinkCmd response
 * of type LINK_CMD_CAM_REC_ON_NOTIFY and constructs a LinkCamIdNotify message containing
 * the identifier of the camera for which recording is activated. The response message is then sent using the
 * SendRespMsg method.
 *
 * \param data The FactoryCameraChangeData containing information about the camera with activated recording.
 * \return Returns true if the camera recording activation notification is successfully sent; otherwise, false.
 */
bool LinkHandler::NotifyCamRecOn(FactoryCameraChangeData data)
{
    // Create a LinkCmd response of type LINK_CMD_CAM_REC_ON_NOTIFY
    Link::LinkCmd cmdResp;
    cmdResp.set_type(Link::LINK_CMD_CAM_REC_ON_NOTIFY);

    // Create a LinkCamIdNotify message and set the camera identifier
    LinkCamIdNotify *resp = new LinkCamIdNotify;
    resp->set_strid(data.id);

    // Set the LinkCamIdNotify message in the LinkCmd response
    cmdResp.set_allocated_camidnotify(resp);

    // Send the response message using the SendRespMsg method
    SendRespMsg(cmdResp, m_server, m_conn);

    // Return true to indicate successful notification
    return true;
}

/**
 * \brief Notifies the recording deactivation of a camera and sends the corresponding response message.
 *
 * This method is called when a camera recording deactivation event is received. It creates a LinkCmd response
 * of type LINK_CMD_CAM_REC_OFF_NOTIFY and constructs a LinkCamIdNotify message containing
 * the identifier of the camera for which recording is deactivated. The response message is then sent using the
 * SendRespMsg method.
 *
 * \param data The FactoryCameraChangeData containing information about the camera with deactivated recording.
 * \return Returns true if the camera recording deactivation notification is successfully sent; otherwise, false.
 */
bool LinkHandler::NotifyCamRecOff(FactoryCameraChangeData data)
{
    // Create a LinkCmd response of type LINK_CMD_CAM_REC_OFF_NOTIFY
    Link::LinkCmd cmdResp;
    cmdResp.set_type(Link::LINK_CMD_CAM_REC_OFF_NOTIFY);

    // Create a LinkCamIdNotify message and set the camera identifier
    LinkCamIdNotify *resp = new LinkCamIdNotify;
    resp->set_strid(data.id);

    // Set the LinkCamIdNotify message in the LinkCmd response
    cmdResp.set_allocated_camidnotify(resp);

    // Send the response message using the SendRespMsg method
    SendRespMsg(cmdResp, m_server, m_conn);

    // Return true to indicate successful notification
    return true;
}


/**
 * \brief Notifies the discovery of a new camera and sends the corresponding response message.
 *
 * This method is called when a new camera is discovered. It creates a LinkCmd response
 * of type LINK_CMD_CAM_SEARCHED_NOTIFY and constructs a LinkCamSearchedNotify message containing
 * information about the newly discovered camera, such as its IP address, port, model, and ONVIF address.
 * The response message is then sent using the SendRespMsg method.
 *
 * \param strIP The IP address of the newly discovered camera.
 * \param strPort The port number of the newly discovered camera.
 * \param strModel The model information of the newly discovered camera.
 * \param strONVIFAddr The ONVIF address of the newly discovered camera.
 * \return Returns true if the camera discovery notification is successfully sent; otherwise, false.
 */
bool LinkHandler::NewCam(string strIP, string strPort, string strModel, string strONVIFAddr)
{
    // Create a LinkCmd response of type LINK_CMD_CAM_SEARCHED_NOTIFY
    Link::LinkCmd cmdResp;
    cmdResp.set_type(Link::LINK_CMD_CAM_SEARCHED_NOTIFY);

    // Create a LinkCamSearchedNotify message and set the camera information
    LinkCamSearchedNotify *resp = new LinkCamSearchedNotify;
    resp->set_strip(strIP);
    resp->set_strport(strPort);
    resp->set_strmodel(strModel);
    resp->set_stronvifaddress(strONVIFAddr);

    // Set the LinkCamSearchedNotify message in the LinkCmd response
    cmdResp.set_allocated_camsearchednotify(resp);

    // Send the response message using the SendRespMsg method
    SendRespMsg(cmdResp, m_server, m_conn);

    // Return true to indicate successful notification
    return true;
}

/**
 * \brief Event handler for the LinkHandler class.
 *
 * This function handles a specific event and sends a response message.
 *
 * \param data The event data containing information about the event.
 */
void LinkHandler::EventHandler1(VEventData data)
{
    // Create a response command object
    Link::LinkCmd cmdResp;

    // Set the type of the response command to event notification
    cmdResp.set_type(Link::LINK_CMD_EVENT_NOTIFY);

    // Create objects for the event notification and event data
    LinkEventNotify* resp = new LinkEventNotify;
    VidEvent* pEvent = new VidEvent;

    // Set properties of the event data
    pEvent->set_strid(data.strId);
    pEvent->set_strdevice(data.strDevice);
    pEvent->set_strdevicename(data.strDeviceName);
    pEvent->set_ntime(data.nTime);
    pEvent->set_strtime(data.strEvttime);
    pEvent->set_strtype(data.strType);
    pEvent->set_bsearched(false);
    pEvent->set_bhandled(data.bHandled);

    // Allocate the event data to the response object
    resp->set_allocated_cevent(pEvent);

    // Allocate the response object to the response command
    cmdResp.set_allocated_evnetnotify(resp);

    // Send the response message
    SendRespMsg(cmdResp, m_server, m_conn);
}

/**
 * \brief Event handler for the LinkHandler class with additional parameter.
 *
 * This function is a wrapper for the EventHandler1 method, allowing it to be used in
 * contexts where a function pointer or callback with a specific signature is required.
 * It is designed to be used with the EventHandler1 method of the LinkHandler class.
 *
 * \param data The event data containing information about the event.
 * \param pParam A pointer to the LinkHandler object that will handle the event.
 *               It is expected to be a pointer to the same object calling this method.
 *               Used to forward the event handling to the appropriate instance.
 */
void LinkHandler::EventHandler(VEventData data, void* pParam)
{
    // Cast the void pointer to a LinkHandler pointer
    LinkHandler* pObj = static_cast<LinkHandler*>(pParam);

    // Call the EventHandler1 method of the LinkHandler object
    pObj->EventHandler1(data);
}

/**
 * \brief Search event handler for the LinkHandler class.
 *
 * This function handles a specific search event and sends a response message.
 *
 * \param data The event data containing information about the search event.
 */
void LinkHandler::SearchEventHandler1(VEventData data)
{
    // Create a response command object
    Link::LinkCmd cmdResp;

    // Set the type of the response command to event notification
    cmdResp.set_type(Link::LINK_CMD_EVENT_NOTIFY);

    // Create objects for the event notification and event data
    LinkEventNotify* resp = new LinkEventNotify;
    VidEvent* pEvent = new VidEvent;

    // Set properties of the event data
    pEvent->set_strid(data.strId);
    pEvent->set_strdevice(data.strDevice);
    pEvent->set_strdevicename(data.strDeviceName);
    pEvent->set_ntime(data.nTime);
    pEvent->set_strtime(data.strEvttime);
    pEvent->set_strtype(data.strType);
    pEvent->set_bsearched(true);  // Indicates that this is a search event
    pEvent->set_bhandled(data.bHandled);

    // Allocate the event data to the response object
    resp->set_allocated_cevent(pEvent);

    // Allocate the response object to the response command
    cmdResp.set_allocated_evnetnotify(resp);

    // Send the response message
    SendRespMsg(cmdResp, m_server, m_conn);
}

/**
 * \brief Search event handler for the LinkHandler class with additional parameter.
 *
 * This function is a wrapper for the SearchEventHandler1 method, allowing it to be used in
 * contexts where a function pointer or callback with a specific signature is required.
 * It is designed to be used with the SearchEventHandler1 method of the LinkHandler class.
 *
 * \param data The event data containing information about the search event.
 * \param pParam A pointer to the LinkHandler object that will handle the search event.
 *               It is expected to be a pointer to the same object calling this method.
 *               Used to forward the search event handling to the appropriate instance.
 */
void LinkHandler::SearchEventHandler(VEventData data, void* pParam)
{
    // Cast the void pointer to a LinkHandler pointer
    LinkHandler* pObj = static_cast<LinkHandler*>(pParam);

    // Call the SearchEventHandler1 method of the LinkHandler object
    pObj->SearchEventHandler1(data);
}



bool LinkHandler::ProcessLoginReq(Link::LinkCmd &req, CivetServer *server, mg_connection *conn)
{
	long long p = (long long)conn;
	Link::LinkCmd cmdResp;
	if (!req.has_loginreq()) {
		return false;
	}
	
	
	const LinkLoginReq& pLogin =  req.loginreq();

	cmdResp.set_type(Link::LINK_CMD_LOGIN_RESP);
	LinkLoginResp * resp = new LinkLoginResp;

	astring realPasswd = "admin";

	SimpleCrypt crypt;
	// here has error QString strDePasswd = m_pFactory.GetAdminPasswd(realPasswd);
	bool bRet = m_pFactory.GetAdminPasswd(realPasswd);

	//realPasswd = crypt.decryptToString(strDePasswd).toStdString();
	realPasswd = "admin";

	/* calc the md5 and compare */
	std::string pass = m_seesionId + realPasswd;

	XMD5 md5Check;
	md5Check.Update((const uint8_t*)(pass.c_str()), pass.length());

	md5Check.Finalize();
	std::string md5Output = md5Check.GetAsString().c_str();
	

	if (md5Output == pLogin.strpasswd())
	{
		resp->set_bret(true);
		m_bLogin = true;
		resp->set_bretnonce(false);
	}else
	{
		resp->set_strnonce(m_seesionId);
		resp->set_bretnonce(true);
	}

	cmdResp.set_allocated_loginresp(resp);

	SendRespMsg(cmdResp, server, conn);

	return true;
}

bool LinkHandler::ProcessRegNotifyReq(Link::LinkCmd &req, CivetServer *server, mg_connection *conn)
{
	long long p = (long long)conn;
	Link::LinkCmd cmdResp;
	bool nRet = false;
	if (!req.has_regnotifyreq()) {
		return false;
	}

	const LinkRegNotifyReq& pReq =  req.regnotifyreq();
	m_bRegNotify = true;
	m_server = server;
	m_conn = conn;
	m_pFactory.RegCameraChangeNotify(this,
		(FactoryCameraChangeNotify)(LinkHandler::CallChange));


	cmdResp.set_type(Link::LINK_CMD_REG_NOTIFY_RESP);
	LinkRegNotifyResp * resp = new LinkRegNotifyResp;

	resp->set_bsuccess(nRet);

	cmdResp.set_allocated_regnotifyresp(resp);

	SendRespMsg(cmdResp, server, conn);

	return true;
}
	                        

bool LinkHandler::ProcessCamListReq(Link::LinkCmd &req, CivetServer *server, mg_connection *conn)
{
	long long p = (long long)conn;
	Link::LinkCmd cmdResp;
	if (!req.has_camlistreq()) {
		return false;
	}

	CameraOnlineMap pCameraOnlineMap;
	CameraRecMap pCameraRecMap;

	m_pFactory.GetCameraOnlineMap(pCameraOnlineMap);
	m_pFactory.GetCameraRecMap(pCameraRecMap);
	
	const LinkListCamReq& pList =  req.camlistreq();

	cmdResp.set_type(Link::LINK_CMD_CAM_LIST_RESP);
	LinkListCamResp * resp = new LinkListCamResp;
	VidCameraList * clist = new VidCameraList;

	m_pFactory.GetCameraList(*clist);

	for (s32 i = 0; i < clist->cvidcamera_size(); i ++) {
		VidCamera &cam = (VidCamera &)(clist->cvidcamera(i));
		/* remove the password */
		//cam.set_strpasswd("******");
		cam.set_bonline(pCameraOnlineMap[cam.strid()]);
		cam.set_brec(pCameraRecMap[cam.strid()]);	
	}
	resp->set_allocated_clist(clist);

	cmdResp.set_allocated_camlistresp(resp);

	SendRespMsg(cmdResp, server, conn);

	return true;
}

bool LinkHandler::ProcessCamReq(Link::LinkCmd &req, CivetServer *server, mg_connection *conn)
{
	long long p = (long long)conn;
	Link::LinkCmd cmdResp;
	if (!req.has_camreq()) {
		return false;
	}
	
	const LinkCamReq& pCamReq =  req.camreq();

	cmdResp.set_type(Link::LINK_CMD_CAM_RESP);
	LinkCamResp * resp = new LinkCamResp;
	VidCamera * pCam = new VidCamera;
	m_pFactory.GetCamera(pCamReq.strid(), *pCam);

	resp->set_allocated_ccam(pCam);

	cmdResp.set_allocated_camresp(resp);

	SendRespMsg(cmdResp, server, conn);

	return true;
}

bool LinkHandler::ProcessAddCamReq(Link::LinkCmd &req, CivetServer *server, mg_connection *conn)
{
	long long p = (long long)conn;
	Link::LinkCmd cmdResp;
	bool nRet = false;
	if (!req.has_addcamreq()) {
		return false;
	}
	
	const LinkAddCamReq& pCamReq =  req.addcamreq();
	CameraParam addParam((VidCamera &)(pCamReq.ccam()));
	nRet = true;
	m_pFactory.AddCamera(addParam);

	cmdResp.set_type(Link::LINK_CMD_ADD_CAM_RESP);
	LinkAddCamResp * resp = new LinkAddCamResp;

	resp->set_bsuccess(nRet);

	cmdResp.set_allocated_addcamresp(resp);

	SendRespMsg(cmdResp, server, conn);

	return true;
}
bool LinkHandler::ProcessDelCamReq(Link::LinkCmd &req, CivetServer *server, mg_connection *conn)
{
	long long p = (long long)conn;
	Link::LinkCmd cmdResp;
	bool bRet = false;
	if (!req.has_delcamreq()) {
		return false;
	}
	
	const LinkDelCamReq& pCamReq =  req.delcamreq();

	bRet = m_pFactory.DelCamera(pCamReq.strid());

	cmdResp.set_type(Link::LINK_CMD_DEL_CAM_RESP);
	LinkDelCamResp * resp = new LinkDelCamResp;

	resp->set_bsuccess(bRet);

	cmdResp.set_allocated_delcamresp(resp);

	SendRespMsg(cmdResp, server, conn);

	return true;
}
bool LinkHandler::ProcessSetCamSchedReq(Link::LinkCmd &req, CivetServer *server, mg_connection *conn)
{
	long long p = (long long)conn;
	Link::LinkCmd cmdResp;
	bool nRet = false;
	if (!req.has_setcamschedreq()) {
		return false;
	}
	
	const LinkSetCamSchedReq& pCamReq =  req.setcamschedreq();
	const VidCamera & pCam = pCamReq.ccam();

	nRet = m_pFactory.UpdateRecSched(pCam.strid(), (VidCamera &)pCam);

	cmdResp.set_type(Link::LINK_CMD_SET_CAM_SCHED_RESP);
	LinkSetCamSchedResp * resp = new LinkSetCamSchedResp;

	resp->set_bsuccess(nRet);

	cmdResp.set_allocated_setcamschedresp(resp);

	SendRespMsg(cmdResp, server, conn);

	return true;
}

bool LinkHandler::ProcessGetStreamListReq(Link::LinkCmd &req, CivetServer *server, mg_connection *conn)
{
	long long p = (long long)conn;
	Link::LinkCmd cmdResp;
	bool nRet = false;
	if (!req.has_getstreamlistreq()) {
		return false;
	}
	
	const LinkGetStreamListReq& pReq =  req.getstreamlistreq();

	VidStreamList *pList = new VidStreamList;
	nRet = m_pFactory.GetCamStreamList(pReq.strid(), *pList);

	cmdResp.set_type(Link::LINK_CMD_GET_STREAM_LIST_RESP);
	LinkGetStreamListResp * resp = new LinkGetStreamListResp;

	resp->set_allocated_clist(pList);

	cmdResp.set_allocated_getstreamlistresp(resp);

	SendRespMsg(cmdResp, server, conn);

	return true;
}

bool LinkHandler::ProcessDiskListReq(Link::LinkCmd &req, CivetServer *server, mg_connection *conn)
{
	long long p = (long long)conn;
	Link::LinkCmd cmdResp;
	if (!req.has_disklistreq()) {
		return false;
	}

	cmdResp.set_type(Link::LINK_CMD_DISK_LIST_RESP);
	LinkVidDiskListResp * resp = new LinkVidDiskListResp;


	/* Get disk list from factory */
	VDBDiskMap diskMap;
	m_pFactory.GetDiskMap(diskMap);
	VidDiskList *diskList = new VidDiskList;

	VDBDiskMap::iterator it = diskMap.begin(); 
	for(; it!=diskMap.end(); ++it) {
		VidDisk *pDisk = diskList->add_cviddisk();
		pDisk->set_strid((*it).second.hdd);
		pDisk->set_strpath((*it).second.path);
		pDisk->set_ntotalsize(0);
		pDisk->set_nfreesize(0);
		pDisk->set_nstorused((*it).second.used);
		pDisk->set_nstorlimit((*it).second.limit);
	}

	resp->set_allocated_cdisk(diskList);

	cmdResp.set_allocated_disklistresp(resp);

	SendRespMsg(cmdResp, server, conn);

	return true;
}
bool LinkHandler::ProcessSysDiskListReq(Link::LinkCmd &req, CivetServer *server, mg_connection *conn)
{
	long long p = (long long)conn;
	Link::LinkCmd cmdResp;
	if (!req.has_sysdisklistreq()) {
		return false;
	}

	cmdResp.set_type(Link::LINK_CMD_SYS_DISK_LIST_RESP);
	LinkSysVidDiskListResp * resp = new LinkSysVidDiskListResp;

	VidDiskList *diskList = new VidDiskList;
	/* Get Disk list from system */
	QList<QStorageInfo> hdd = QStorageInfo::mountedVolumes();
	QListIterator<QStorageInfo> it(hdd);
	while(it.hasNext()) {
		QStorageInfo  disk = it.next();
		string strHdd = disk.rootPath().toStdString();
		
		R_LOG(logRINFO,"%s strHdd %s \n",__FUNCTION__, strHdd.c_str());
		s64 totalSize = disk.bytesTotal() / (1024 * 1024);
		s64 leftSize = disk.bytesFree()/ (1024 * 1024);
		
		R_LOG(logRINFO,"%s Total %lld M Left %lld M \n",__FUNCTION__, totalSize, leftSize);  
		QString strQtHdd = disk.rootPath();
		HddDriveType diskType = HddGetDriveType(strQtHdd);
		
		R_LOG(logRINFO,"%s Type %d \n",__FUNCTION__,  diskType);
		
		if (totalSize/1024 < 4 || leftSize/1024 < 2) /* In G */ {
			continue;
		}
		
		if (diskType == HddInternalDrive 
			|| diskType == HddRemovableDrive || diskType == HddRemoteDrive) {
			VidDisk *pDisk = diskList->add_cviddisk();
			pDisk->set_strid(disk.device().toStdString());
			pDisk->set_strpath(disk.rootPath().toStdString());
			pDisk->set_ntotalsize(totalSize);
			pDisk->set_nfreesize(leftSize);
			pDisk->set_nstorused(0);
			pDisk->set_nstorlimit(0);
		}
	}
	resp->set_allocated_cdisk(diskList);

	cmdResp.set_allocated_sysdisklistresp(resp);

	SendRespMsg(cmdResp, server, conn);

	return true;	
}


bool LinkHandler::ProcessAddDiskReq(Link::LinkCmd &req, CivetServer *server, mg_connection *conn)
{
	long long p = (long long)conn;
	Link::LinkCmd cmdResp;
	if (!req.has_adddiskreq()) {
		return false;
	}

	const VidDisk& pDisk =  req.adddiskreq().cdisk();

	bool bRet = m_pFactory.AddHdd(pDisk.strid(), pDisk.strpath(), pDisk.nstorlimit());
	
	cmdResp.set_type(Link::LINK_CMD_ADD_DISK_RESP);
	LinkAddDiskResp * resp = new LinkAddDiskResp;
	resp->set_bsuccess(bRet);

	cmdResp.set_allocated_adddiskresp(resp);

	SendRespMsg(cmdResp, server, conn);
	return true;
}

bool LinkHandler::ProcessDelDiskReq(Link::LinkCmd &req, CivetServer *server, mg_connection *conn)
{
	long long p = (long long)conn;
	Link::LinkCmd cmdResp;
	if (!req.has_deldiskreq()) {
		return false;
	}

	const LinkDelDiskReq& pReq =  req.deldiskreq();
	bool bRet = m_pFactory.DelHdd(pReq.strid());
	cmdResp.set_type(Link::LINK_CMD_DEL_DISK_RESP);
	LinkDelDiskResp * resp = new LinkDelDiskResp;
	resp->set_bsuccess(bRet);

	cmdResp.set_allocated_deldiskresp(resp);

	SendRespMsg(cmdResp, server, conn);

	return true;
}

bool LinkHandler::ProcessUpdateDiskLimitReq(Link::LinkCmd &req, CivetServer *server, mg_connection *conn)
{
	long long p = (long long)conn;
	Link::LinkCmd cmdResp;
	if (!req.has_disklimitreq()) {
		return false;
	}

	const LinkUpdateDiskLimitReq& pReq =  req.disklimitreq();

	bool bRet = m_pFactory.HddUpdateSize(pReq.strid(), pReq.nlimit());
	
	cmdResp.set_type(Link::LINK_CMD_UPDATE_DISK_LIMIT_RESP);
	LinkUpdateDiskLimitResp * resp = new LinkUpdateDiskLimitResp;
	resp->set_bsuccess(bRet);

	cmdResp.set_allocated_disklimitresp(resp);

	SendRespMsg(cmdResp, server, conn);

	return true;
}

/* License & Version */
bool LinkHandler::ProcessGetLicReq(Link::LinkCmd &req, CivetServer *server, mg_connection *conn)
{
	long long p = (long long)conn;
	Link::LinkCmd cmdResp;
	if (!req.has_licreq()) {
		return false;
	}

	const LinkGetLicReq& pReq =  req.licreq();
	
	cmdResp.set_type(Link::LINK_CMD_GET_LIC_RESP);
	LinkGetLicResp * resp = new LinkGetLicResp;
	std::string strLic;
	std::string strHostId;
	int nCh;
	std::string strType;
	std::string strStartTime;
	std::string strExpireTime;	
	m_pFactory.GetLicense(strLic, strHostId, nCh, strType,
					strStartTime, strExpireTime);
	resp->set_strlic(strLic);
	resp->set_strhostid(strHostId);
	resp->set_nch(nCh);
	resp->set_strtype(strType);
	resp->set_strstarttime(strStartTime);
	resp->set_strexpiretime(strExpireTime);

	cmdResp.set_allocated_licresp(resp);

	SendRespMsg(cmdResp, server, conn);

	return true;
}
bool LinkHandler::ProcessGetVerReq(Link::LinkCmd &req, CivetServer *server, mg_connection *conn)
{
	long long p = (long long)conn;
	Link::LinkCmd cmdResp;
	if (!req.has_verreq()) {
		return false;
	}

	const LinkGetVerReq& pReq =  req.verreq();

	cmdResp.set_type(Link::LINK_CMD_GET_VER_RESP);
	LinkGetVerResp * resp = new LinkGetVerResp;

	resp->set_strver(VE_VERSION);
	resp->set_strinfo(VE_INFO);

	cmdResp.set_allocated_verresp(resp);

	SendRespMsg(cmdResp, server, conn);

	return true;
}

bool LinkHandler::ProcessConfLicReq(Link::LinkCmd &req, CivetServer *server, mg_connection *conn)
{
	long long p = (long long)conn;
	Link::LinkCmd cmdResp;
	if (!req.has_conflicreq()) {
		return false;
	}

	const LinkConfLicReq& pReq =  req.conflicreq();

	astring strLic = pReq.strlic();
	m_pFactory.SetLicense(strLic);

	cmdResp.set_type(Link::LINK_CMD_CONF_LIC_RESP);
	LinkConfLicResp * resp = new LinkConfLicResp;

	resp->set_bsuccess(true);

	cmdResp.set_allocated_conflicresp(resp);

	SendRespMsg(cmdResp, server, conn);

	return true;	
}

bool LinkHandler::ProcessAddUserReq(Link::LinkCmd &req, CivetServer *server,
                        mg_connection *conn)
{
	long long p = (long long)conn;
	Link::LinkCmd cmdResp;
	bool nRet = false;
	if (!req.has_adduserreq()) {
		return false;
	}

	const LinkAddUserReq& pReq =  req.adduserreq();

	/* Change passwd of admin */
	if (pReq.struser() == "admin") {
		nRet = m_pFactory.SetAdminPasswd(pReq.strpasswd());
	}
	else/* normal user */
	{
	}

	cmdResp.set_type(Link::LINK_CMD_ADD_USER_RESP);
	LinkAddUserResp * resp = new LinkAddUserResp;

	resp->set_bsuccess(nRet);

	cmdResp.set_allocated_adduserresp(resp);

	SendRespMsg(cmdResp, server, conn);

	return true;	
}

/* Search */
bool LinkHandler::ProcessHasRecordReq(Link::LinkCmd &req, CivetServer *server, mg_connection *conn)
{
	long long p = (long long)conn;
	Link::LinkCmd cmdResp;
	bool nRet = false;
	if (!req.has_hasrecreq()) {
		return false;
	}

	const LinkHasRecordReq& pReq =  req.hasrecreq();
	const LinkHasRecordList &pSearchList = pReq.clist();

	LinkHasRecordList *pNewList = new LinkHasRecordList;
	*pNewList = pSearchList;

	for (s32 i = 0; i < pNewList->chasrec_size(); i ++) {
		LinkHasRecordItem &item = (LinkHasRecordItem &)(pNewList->chasrec(i));
		bool bRet = m_pFactory.SearchHasItems(pReq.strid(), item.nstart(),
							item.nend(), item.ntype());
		item.set_bhas(bRet);
	}

	cmdResp.set_type(Link::LINK_CMD_HAS_RECORD_RESP);
	LinkHasRecordResp * resp = new LinkHasRecordResp;
	resp->set_allocated_clist(pNewList);

	cmdResp.set_allocated_hasrecresp(resp);

	SendRespMsg(cmdResp, server, conn);

	return true;	
}

bool LinkHandler::ProcessSearchRecordReq(Link::LinkCmd &req, CivetServer *server, mg_connection *conn)
{
	long long p = (long long)conn;
	Link::LinkCmd cmdResp;
	bool nRet = false;
	if (!req.has_searchrecreq()) {
		return false;
	}

	const LinkSearchRecordReq& pReq =  req.searchrecreq();
	RecordItemMap recMap;

	m_pFactory.SearchItems(pReq.strid(), pReq.nstart(), pReq.nend(), 
									pReq.ntype(), recMap);

	cmdResp.set_type(Link::LINK_CMD_SEARCH_RECORD_RESP);
	LinkSearchRecordResp * resp = new LinkSearchRecordResp;

	LinkRecordList *pList = new LinkRecordList;

	resp->set_strid(pReq.strid());
	RecordItemMap::iterator it = recMap.begin(); 
	
	for(; it!=recMap.end(); ++it) {	
		LinkRecordItem *pItem = pList->add_clist();

		pItem->set_nid((*it).second.id);
		pItem->set_nstart((*it).second.start);
		pItem->set_nend((*it).second.end);
		pItem->set_ntype((*it).second.type);
	}
	resp->set_allocated_clist(pList);
	cmdResp.set_allocated_searchrecresp(resp);
	SendRespMsg(cmdResp, server, conn);
	return true;	
}

bool LinkHandler::ProcessPtzCmdReq(Link::LinkCmd &req, CivetServer *server, mg_connection *conn)
{
	long long p = (long long)conn;
	Link::LinkCmd cmdResp;
	if (!req.has_ptzcmd()) {
		return false;
	}
	
	const LinkPtzCmd& pReq =  req.ptzcmd();
	m_pFactory.PtzAction(pReq.strid(), (FPtzAction)(pReq.naction()), 
					(float)pReq.nparam());
	return true;
}

bool LinkHandler::ProcessCamSearchStartReq(Link::LinkCmd &req, CivetServer *server, mg_connection *conn)
{
	long long p = (long long)conn;
	Link::LinkCmd cmdResp;
	if (!req.has_camsearchstartreq()) {
		return false;
	}
	m_server = server;
	m_conn = conn;
	
	const LinkCamSearchStartReq& pReq =  req.camsearchstartreq();

	if (m_pCamSearch) {
		delete m_pCamSearch;
		m_pCamSearch = NULL;
	}

	m_pCamSearch = new OnvifDisClientMgr(*this);
	m_pCamSearch->Start();

	cmdResp.set_type(Link::LINK_CMD_CAM_SEARCH_START_RESP);
	LinkCamSearchStartResp * resp = new LinkCamSearchStartResp;

	resp->set_bsuccess(true);
	cmdResp.set_allocated_camsearchstartresp(resp);
	SendRespMsg(cmdResp, server, conn);
	return true;	
}
bool LinkHandler::ProcessCamSearchStopReq(Link::LinkCmd &req, CivetServer *server, mg_connection *conn)
{
	long long p = (long long)conn;
	Link::LinkCmd cmdResp;
	if (!req.has_camsearchstopreq()) {
		return false;
	}
	const LinkCamSearchStopReq& pReq =  req.camsearchstopreq();
	if (m_pCamSearch) {
		delete m_pCamSearch;
		m_pCamSearch = NULL;
	}

	cmdResp.set_type(Link::LINK_CMD_CAM_SEARCH_STOP_RESP);
	LinkCamSearchStopResp * resp = new LinkCamSearchStopResp;

	resp->set_bsuccess(true);
	cmdResp.set_allocated_camsearchstopresp(resp);
	SendRespMsg(cmdResp, server, conn);

	return true;
}

bool LinkHandler::ProcessRegEventReq(Link::LinkCmd &req, CivetServer *server, mg_connection *conn)
{
	long long p = (long long)conn;
	Link::LinkCmd cmdResp;
	if (!req.has_regeventreq()) {
		return false;
	}
	m_server = server;
	m_conn = conn;
	
	const LinkRegEventReq& pReq =  req.regeventreq();

	m_pEvent.RegEventNotify(this, LinkHandler::EventHandler);
	m_bRealEvent = true;

	cmdResp.set_type(Link::LINK_CMD_REG_EVENT_RESP);
	LinkRegEventResp * resp = new LinkRegEventResp;

	resp->set_bsuccess(true);
	cmdResp.set_allocated_regeventresp(resp);
	SendRespMsg(cmdResp, server, conn);

	return true;
}
bool LinkHandler::ProcessUnRegEventReq(Link::LinkCmd &req, CivetServer *server, mg_connection *conn)
{
	long long p = (long long)conn;
	Link::LinkCmd cmdResp;
	if (!req.has_unregeventreq()) {
		return false;
	}

	const LinkUnRegEventReq& pReq =  req.unregeventreq();
	if (m_bRealEvent) {
		m_pEvent.UnRegEventNotify(this);
		m_bRealEvent = false;
	}

	cmdResp.set_type(Link::LINK_CMD_UNREG_EVENT_RESP);
	LinkUnRegEventResp * resp = new LinkUnRegEventResp;

	resp->set_bsuccess(true);
	cmdResp.set_allocated_unregeventresp(resp);
	SendRespMsg(cmdResp, server, conn);
	
	return true;
}
bool LinkHandler::ProcessHandleEventReq(Link::LinkCmd &req, CivetServer *server, mg_connection *conn)
{
	long long p = (long long)conn;
	Link::LinkCmd cmdResp;
	if (!req.has_handleeventreq()) {
		return false;
	}

	const LinkHandleEventReq& pReq =  req.handleeventreq();
	bool bRet = m_pEvent.HandleEvent(pReq.strid());
	cmdResp.set_type(Link::LINK_CMD_HANDLE_EVENT_RESP);
	LinkHandleEventResp * resp = new LinkHandleEventResp;
	resp->set_bsuccess(bRet);

	cmdResp.set_allocated_handleeventresp(resp);

	SendRespMsg(cmdResp, server, conn);
	return true;
}

/**
 * \brief Process an incoming event search request and send a response over a WebSocket connection.
 *
 * This function is responsible for processing an incoming event search request,
 * generating a response, and sending it over a WebSocket connection.
 *
 * \param req The LinkCmd request message containing the event search parameters.
 * \param server A pointer to the CivetServer instance.
 * \param conn A pointer to the mg_connection representing the WebSocket connection.
 * \return True if the event search request was successfully processed, false otherwise.
 */
bool LinkHandler::ProcessEventSearchReq(Link::LinkCmd &req, CivetServer *server, mg_connection *conn)
{
	long long p = (long long)conn;
	Link::LinkCmd cmdResp;
	if (!req.has_eventsearchreq()) {
		return false;
	}
	m_server = server;
	m_conn = conn;
	
	const LinkEventSearchReq& pReq =  req.eventsearchreq();

	m_pEvent.RegSearchEventNotify(this, (FunctionEventNotify)(LinkHandler::SearchEventHandler));
	m_pEvent.SearchEvent(pReq.strid(), pReq.nstart(), pReq.nend(), this);
	m_bSearchEvent = true;

	cmdResp.set_type(Link::LINK_CMD_EVENT_SEARCH_RESP);
	LinkEventSearchResp * resp = new LinkEventSearchResp;

	resp->set_bsuccess(true);
	cmdResp.set_allocated_eventsearchresp(resp);
	SendRespMsg(cmdResp, server, conn);

	return true;
}
	
/**
 * \brief Process an incoming message received over a WebSocket connection.
 *
 * This function is responsible for processing an incoming message received over a WebSocket connection.
 * It may involve parsing the message, handling specific commands, and sending an appropriate response.
 *
 * \param strMsg The incoming message as a string.
 * \param server A pointer to the CivetServer instance.
 * \param conn A pointer to the mg_connection representing the WebSocket connection.
 * \return True if the message was successfully processed, false otherwise.
 */
bool LinkHandler::ProcessMsg(std::string &strMsg, CivetServer *server, mg_connection *conn)
{
	Link::LinkCmd cmd;
	::google::protobuf::util::Status status = 
		::google::protobuf::util::JsonStringToMessage(strMsg, &cmd);
	if (!status.ok()) {
		return false;
	}

	switch (cmd.type()) {
		case Link::LINK_CMD_LOGIN_REQ: {
			return ProcessLoginReq(cmd, server, conn);
			break;
		}
		case Link::LINK_CMD_REG_NOTIFY_REQ: {
			return ProcessRegNotifyReq(cmd, server, conn);
			break;
		}
		case Link::LINK_CMD_CAM_LIST_REQ: {
			return ProcessCamListReq(cmd, server, conn);
			break;
		}
		case Link::LINK_CMD_CAM_REQ: {
			return ProcessCamReq(cmd, server, conn);
			break;
		}
		case Link::LINK_CMD_ADD_CAM_REQ: {
			return ProcessAddCamReq(cmd, server, conn);
			break;
		}
		case Link::LINK_CMD_DEL_CAM_REQ: {
			return ProcessDelCamReq(cmd, server, conn);
			break;
		}
		case Link::LINK_CMD_SET_CAM_SCHED_REQ: {
			return ProcessSetCamSchedReq(cmd, server, conn);
			break;
		}
		case Link::LINK_CMD_GET_STREAM_LIST_REQ: {
			return ProcessGetStreamListReq(cmd, server, conn);
			break;
		}
		case Link::LINK_CMD_DISK_LIST_REQ: {
			return ProcessDiskListReq(cmd, server, conn);
			break;
		}
		case Link::LINK_CMD_SYS_DISK_LIST_REQ: {
			return ProcessSysDiskListReq(cmd, server, conn);
			break;
		}
		case Link::LINK_CMD_ADD_DISK_REQ: {
			return ProcessAddDiskReq(cmd, server, conn);
			break;
		}
		case Link::LINK_CMD_DEL_DISK_REQ: {
			return ProcessDelDiskReq(cmd, server, conn);
			break;
		}
		case Link::LINK_CMD_UPDATE_DISK_LIMIT_REQ: {
			return ProcessUpdateDiskLimitReq(cmd, server, conn);
			break;
		}
		case Link::LINK_CMD_GET_LIC_REQ: {
			return ProcessGetLicReq(cmd, server, conn);
			break;
		}
		case Link::LINK_CMD_GET_VER_REQ: {
			return ProcessGetVerReq(cmd, server, conn);
			break;
		}
		case Link::LINK_CMD_CONF_LIC_REQ: {
			return ProcessConfLicReq(cmd, server, conn);
			break;
		}
		case Link::LINK_CMD_ADD_USER_REQ: {
			return ProcessAddUserReq(cmd, server, conn);
			break;
		}
		case Link::LINK_CMD_HAS_RECORD_REQ: {
			return ProcessHasRecordReq(cmd, server, conn);
			break;
		}
		case Link::LINK_CMD_SEARCH_RECORD_REQ: {
			return ProcessSearchRecordReq(cmd, server, conn);
			break;
		}
		case Link::LINK_CMD_PTZ_CMD: {
			return ProcessPtzCmdReq(cmd, server, conn);
			break;
		}
		case Link::LINK_CMD_REG_EVENT_REQ: {
			return ProcessRegEventReq(cmd, server, conn);
			break;
		}
		case Link::LINK_CMD_CAM_SEARCH_START_REQ: {
			return ProcessCamSearchStartReq(cmd, server, conn);
			break;
		}
		case Link::LINK_CMD_CAM_SEARCH_STOP_REQ: {
			return ProcessCamSearchStopReq(cmd, server, conn);
			break;
		}
		case Link::LINK_CMD_UNREG_EVENT_REQ: {
			return ProcessUnRegEventReq(cmd, server, conn);
			break;
		}
		case Link::LINK_CMD_HANDLE_EVENT_REQ: {
			return ProcessHandleEventReq(cmd, server, conn);
			break;
		}
		case Link::LINK_CMD_EVENT_SEARCH_REQ: {
			return ProcessEventSearchReq(cmd, server, conn);
			break;
		}
		default:
		   	return false;
	};		
}

/**
 * \brief Send a response message over a WebSocket connection.
 *
 * This function converts a protocol buffer message to a JSON string and sends it over a WebSocket connection.
 *
 * \param resp The LinkCmd response message to be sent.
 * \param server A pointer to the CivetServer instance.
 * \param conn A pointer to the mg_connection representing the WebSocket connection.
 * \return True if the message was successfully sent, false otherwise.
 */
bool LinkHandler::SendRespMsg(Link::LinkCmd &resp, CivetServer *server, mg_connection *conn)
{
    // Convert the protocol buffer message to a JSON string
    std::string strMsg;
    ::google::protobuf::util::Status status =
        ::google::protobuf::util::MessageToJsonString(resp, &strMsg);

    // Check if conversion was successful
    if (!status.ok()) {
        return false;
    }

    // Send the JSON string over the WebSocket connection
    int ret = mg_websocket_write(conn,
                                 WEBSOCKET_OPCODE_TEXT, strMsg.c_str(), strMsg.length());

    // Check if the message was sent successfully
    if (ret == strMsg.length()) {
        return true;
    }
    // Return false if the message was not sent successfully
    return false;
}


