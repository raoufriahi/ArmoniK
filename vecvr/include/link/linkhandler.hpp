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
class LinkHandler: public CamSearchNotify
{
public:

	LinkHandler(Factory &pFactory, VEventServer &pEvent);
	~LinkHandler();

	static bool CallChange(void* pParam, FactoryCameraChangeData data);
	
	static void EventHandler(VEventData data, void* pParam);
	static void SearchEventHandler(VEventData data, void* pParam);	
    
	virtual bool NewCam(string strIP, string strPort, string strModel, string strONVIFAddr);


	bool CallChange1(FactoryCameraChangeData data);
	bool NotifyCamAdd(FactoryCameraChangeData data);
	bool NotifyCamDel(FactoryCameraChangeData data);
	bool NotifyCamOnline(FactoryCameraChangeData data);
	bool NotifyCamOffline(FactoryCameraChangeData data);
	bool NotifyCamRecOn(FactoryCameraChangeData data);
	bool NotifyCamRecOff(FactoryCameraChangeData data);
	
	
	/* Event */
	void EventHandler1(VEventData data);
	void SearchEventHandler1(VEventData data);

	bool ProcessMsg(std::string &strMsg, CivetServer *pServer, mg_connection *pConnection);
	bool ProcessLoginReq(Link::LinkCmd &req, CivetServer *pServer, mg_connection *pConnection);
	bool ProcessRegNotifyReq(Link::LinkCmd &req, CivetServer *pServer, mg_connection *pConnection);

	/* Camera */
	bool ProcessCamListReq(Link::LinkCmd &req, CivetServer *pServer, mg_connection *pConnection);
	bool ProcessCamReq(Link::LinkCmd &req, CivetServer *pServer, mg_connection *pConnection);
	bool ProcessAddCamReq(Link::LinkCmd &req, CivetServer *pServer, mg_connection *pConnection);
	bool ProcessDelCamReq(Link::LinkCmd &req, CivetServer *pServer, mg_connection *pConnection);
	bool ProcessSetCamSchedReq(Link::LinkCmd &req, CivetServer *pServer, mg_connection *pConnection);
	bool ProcessGetStreamListReq(Link::LinkCmd &req, CivetServer *pServer, mg_connection *pConnection);

	/* Disk */
	bool ProcessDiskListReq(Link::LinkCmd &req, CivetServer *pServer, mg_connection *pConnection);
	bool ProcessSysDiskListReq(Link::LinkCmd &req, CivetServer *pServer, mg_connection *pConnection);
	bool ProcessAddDiskReq(Link::LinkCmd &req, CivetServer *pServer, mg_connection *pConnection);
	bool ProcessDelDiskReq(Link::LinkCmd &req, CivetServer *pServer, mg_connection *pConnection);
	bool ProcessUpdateDiskLimitReq(Link::LinkCmd &req, CivetServer *pServer, mg_connection *pConnection);

	/* License & Version */
	bool ProcessGetLicReq(Link::LinkCmd &req, CivetServer *pServer, mg_connection *pConnection);
	bool ProcessGetVerReq(Link::LinkCmd &req, CivetServer *pServer, mg_connection *pConnection);
	bool ProcessConfLicReq(Link::LinkCmd &req, CivetServer *pServer, mg_connection *pConnection);

	/* User */
	bool ProcessAddUserReq(Link::LinkCmd &req, CivetServer *pServer, mg_connection *pConnection);

	/* Search */
	bool ProcessHasRecordReq(Link::LinkCmd &req, CivetServer *pServer, mg_connection *pConnection);
	bool ProcessSearchRecordReq(Link::LinkCmd &req, CivetServer *pServer, mg_connection *pConnection);

	/* PTZ command */
	bool ProcessPtzCmdReq(Link::LinkCmd &req, CivetServer *pServer, mg_connection *pConnection);

	/* Cam Search & Event */
	bool ProcessCamSearchStartReq(Link::LinkCmd &req, CivetServer *pServer, mg_connection *pConnection);
	bool ProcessCamSearchStopReq(Link::LinkCmd &req, CivetServer *pServer, mg_connection *pConnection);
	bool ProcessRegEventReq(Link::LinkCmd &req, CivetServer *pServer, mg_connection *pConnection);
	bool ProcessUnRegEventReq(Link::LinkCmd &req, CivetServer *pServer, mg_connection *pConnection);
	bool ProcessHandleEventReq(Link::LinkCmd &req, CivetServer *pServer, mg_connection *pConnection);
	bool ProcessEventSearchReq(Link::LinkCmd &req, CivetServer *pServer, mg_connection *pConnection);
	bool SendRespMsg(Link::LinkCmd &resp, CivetServer *pServer, mg_connection *pConnection);					  
private:

    bool m_bLogin;
	bool m_bRegNotify;
	bool m_bRealEvent;
	bool m_bSearchEvent;
	
	Factory &m_pFactory;
	string m_seesionId;
	VEventServer &m_pEvent;
	CivetServer *m_pServer;
	mg_connection *m_pConnection;
	OnvifDisClientMgr *m_pCamSearch;
};


typedef std::map<void *, LinkHandler *> LinkHandlerMap;

