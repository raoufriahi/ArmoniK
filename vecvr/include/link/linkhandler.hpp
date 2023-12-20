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

#ifndef __LINK_HANDLER_H__
#define __LINK_HANDLER_H__
#include "server/eventserver.hpp"

class LinkHandler: public CamSearchNotify
{
public:
	LinkHandler(Factory &pFactory, VEventServer &pEvent);
	~LinkHandler();

	static bool CallChange(void* pParam, FactoryCameraChangeData data);

	bool CallChange1(FactoryCameraChangeData data);
	bool NotifyCamAdd(FactoryCameraChangeData data);
	bool NotifyCamDel(FactoryCameraChangeData data);
	bool NotifyCamOnline(FactoryCameraChangeData data);
	bool NotifyCamOffline(FactoryCameraChangeData data);
	bool NotifyCamRecOn(FactoryCameraChangeData data);
	bool NotifyCamRecOff(FactoryCameraChangeData data);
	virtual bool NewCam(string strIP, string strPort, string strModel, string strONVIFAddr);
	
	/* Event */
	void EventHandler1(VEventData data);
	void SearchEventHandler1(VEventData data);

	static void EventHandler(VEventData data, void* pParam);
	static void SearchEventHandler(VEventData data, void* pParam);	

	bool ProcessMsg(std::string &strMsg, CivetServer *server, mg_connection *conn);
	bool ProcessLoginReq(Link::LinkCmd &req, CivetServer *server, mg_connection *conn);
	bool ProcessRegNotifyReq(Link::LinkCmd &req, CivetServer *server, mg_connection *conn);

	/* Camera */
	bool ProcessCamListReq(Link::LinkCmd &req, CivetServer *server, mg_connection *conn);
	bool ProcessCamReq(Link::LinkCmd &req, CivetServer *server, mg_connection *conn);
	bool ProcessAddCamReq(Link::LinkCmd &req, CivetServer *server, mg_connection *conn);
	bool ProcessDelCamReq(Link::LinkCmd &req, CivetServer *server, mg_connection *conn);
	bool ProcessSetCamSchedReq(Link::LinkCmd &req, CivetServer *server, mg_connection *conn);
	bool ProcessGetStreamListReq(Link::LinkCmd &req, CivetServer *server, mg_connection *conn);

	/* Disk */
	bool ProcessDiskListReq(Link::LinkCmd &req, CivetServer *server, mg_connection *conn);
	bool ProcessSysDiskListReq(Link::LinkCmd &req, CivetServer *server, mg_connection *conn);
	bool ProcessAddDiskReq(Link::LinkCmd &req, CivetServer *server, mg_connection *conn);
	bool ProcessDelDiskReq(Link::LinkCmd &req, CivetServer *server, mg_connection *conn);
	bool ProcessUpdateDiskLimitReq(Link::LinkCmd &req, CivetServer *server, mg_connection *conn);

	/* License & Version */
	bool ProcessGetLicReq(Link::LinkCmd &req, CivetServer *server, mg_connection *conn);
	bool ProcessGetVerReq(Link::LinkCmd &req, CivetServer *server, mg_connection *conn);
	bool ProcessConfLicReq(Link::LinkCmd &req, CivetServer *server, mg_connection *conn);

	/* User */
	bool ProcessAddUserReq(Link::LinkCmd &req, CivetServer *server, mg_connection *conn);

	/* Search */
	bool ProcessHasRecordReq(Link::LinkCmd &req, CivetServer *server, mg_connection *conn);
	bool ProcessSearchRecordReq(Link::LinkCmd &req, CivetServer *server, mg_connection *conn);

	/* PTZ command */
	bool ProcessPtzCmdReq(Link::LinkCmd &req, CivetServer *server, mg_connection *conn);

	/* Cam Search & Event */
	bool ProcessCamSearchStartReq(Link::LinkCmd &req, CivetServer *server, mg_connection *conn);
	bool ProcessCamSearchStopReq(Link::LinkCmd &req, CivetServer *server, mg_connection *conn);
	bool ProcessRegEventReq(Link::LinkCmd &req, CivetServer *server, mg_connection *conn);
	bool ProcessUnRegEventReq(Link::LinkCmd &req, CivetServer *server, mg_connection *conn);
	bool ProcessHandleEventReq(Link::LinkCmd &req, CivetServer *server, mg_connection *conn);
	bool ProcessEventSearchReq(Link::LinkCmd &req, CivetServer *server, mg_connection *conn);
	bool SendRespMsg(Link::LinkCmd &resp, CivetServer *server, mg_connection *conn);					  
private:

    bool m_bLogin;
	bool m_bRegNotify;
	bool m_bRealEvent;
	bool m_bSearchEvent;
	
	Factory &m_pFactory;
	string m_seesionId;
	VEventServer &m_pEvent;
	CivetServer *m_server;
	mg_connection *m_conn;
	OnvifDisClientMgr *m_pCamSearch;
};


typedef std::map<void *, LinkHandler *> LinkHandlerMap;


#endif /* __LINK_HANDLER_H__ */
