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
#ifndef __VSC_STOR_CLIENT_H_
#define __VSC_STOR_CLIENT_H_
#include "config/confdb.hpp"
#include "config/sysdb.hpp"
#include <QThread>
#include <qdebug.h>
#include "Poco/Path.h"
#include "Poco/File.h"
#include "config/vidconf.pb.h"
#include "client/storcmn.hpp"

#include "cppkit/ck_string.h"
#include "cppkit/os/ck_sprintf.h"
#include "client/storwsclient.hpp"

using namespace cppkit;
using namespace std;

typedef std::map<astring, bool> StorClientOnlineMap;
typedef std::map<astring, bool> StorClientRecMap;


class VE_LIBRARY_API StorClient : public StorWebSocketClient
{
public:
	StorClient(VidStor &stor, StorFactoryNotifyInterface &pNotify);
	~StorClient();
public:
	bool StorClientStart();
	
public:
	virtual bool ProcessRecvMsg(char *data, size_t data_len);
	virtual bool ProcessOffline();
	virtual bool ProcessOnline();
	bool ProcessCamListResp(Link::LinkCmd &cmd);
	virtual bool RegRealEvent();
	virtual bool UnRegRealEvent();
public:
	VidCameraList GetVidCameraList();
	bool GetCameraConf(astring strCameraId, VidCamera &pCam);
	void UpdateVidCameraList(const VidCameraList &list);
	StorClientOnlineMap GetVidCameraOnlineList();
	StorClientRecMap GetVidCameraRecList();
	astring GetVidCameraName(astring strCam);
	bool GetOnline();
public:
	bool SearchEvent(astring strId, s64 nStart, s64 nEnd);
	bool HandleEvent(astring strId);
public:
	bool PtzCmd(astring strId, u32 action, double param);
	bool DeleteCam(astring strId);
	bool AddCam(VidCamera &pParam);
	
private:
	XMutex m_cMutex;
	VidStor m_stor;
	bool m_Quit;
	/* The list is only for startup usage, if any change, it not update */
	VidCameraList m_cCamList;
	StorClientOnlineMap m_CamOnlineList;
	StorClientRecMap m_CamRecList;
	StorFactoryNotifyInterface &m_pNotify;
	Link::LinkCmd m_lastCmd;
};

typedef StorClient* LPStorClient;

#endif
