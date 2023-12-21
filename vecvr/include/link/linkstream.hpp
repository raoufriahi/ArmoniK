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

#include "XSDK/XMD5.h"
#include "pbwrapper.hpp"
class LinkPlaybackInterface
{
public:
	virtual bool NewFrame(VideoFrame& frame){return false;}
};

class LinkServerPlayback
{
public:
	LinkServerPlayback(LinkPlaybackInterface &pPbInf, 
		Factory &pFactory, string strId, u32 nPlaytime);
	~LinkServerPlayback();

	void DataHandler1(VideoFrame& frame){
		XGuard guard(m_cMutex);
		if (m_bQuit == false){
			m_pPbInf.NewFrame(frame);
		}
	}

	static void DataHandler(VideoFrame& frame, void * pParam){
	    LinkServerPlayback *pObj = static_cast<LinkServerPlayback *> (pParam);
	    return pObj->DataHandler1(frame);
	}
public:
	void run1();
	static void run(void* pData);
	bool SeekToTime(u32 seekTime);
	bool StartPlay();
	bool PausePlay();
	bool QuitPlay();/* End of play */

private:
	Factory &m_pFactory;
	LinkPlaybackInterface &m_pPbInf;
	string m_strId;
	u32 m_nPlaytime;
	PlaybackWrapper m_pbWrapper;
	XMutex m_cMutex;
	bool m_bQuit;
	std::thread * m_pThread;
};

class LinkStream : public LinkPlaybackInterface
{
public:
	LinkStream(Factory &pFactory);
	~LinkStream();

	bool ProcessMsg(std::string &strMsg, CivetServer *pServer, mg_connection *pConnection);
	bool ProcessLoginReq(Link::LinkCmd &req, CivetServer *pServer, mg_connection *pConnection);

	/* Live view command  */
	bool ProcessStartLiveReq(Link::LinkCmd &req, CivetServer *pServer, mg_connection *pConnection);
	bool ProcessStopLiveReq(Link::LinkCmd &req, CivetServer *pServer, mg_connection *pConnection);

	/* Playback command */
	bool ProcessPlayBackReq(Link::LinkCmd &req, CivetServer *pServer, mg_connection *pConnection);
	bool ProcessPlayPauseReq(Link::LinkCmd &req, CivetServer *pServer, mg_connection *pConnection);
	bool ProcessPlayResumeReq(Link::LinkCmd &req, CivetServer *pServer, mg_connection *pConnection);
	bool ProcessPlaySeekReq(Link::LinkCmd &req, CivetServer *pServer, mg_connection *pConnection);
	bool ProcessPlayStopReq(Link::LinkCmd &req, CivetServer *pServer, mg_connection *pConnection);
	bool SendRespMsg(Link::LinkCmd &resp, CivetServer *pServer, mg_connection *pConnection);
    bool StopAll();

	void DataHandler1(VideoFrame& frame);

	static void DataHandler(VideoFrame& frame, void * pParam);
	virtual bool NewFrame(VideoFrame& frame);
			  
private:
	Factory &m_pFactory;
	string m_seesionId;
	bool m_bLogin;
	CivetServer *m_server;
	mg_connection *m_pConnection;
	bool m_bStreaming;
	string m_strLiveviewId;
	unsigned int m_nStream;
	bool m_bPlayback;
	u8 * m_pSendBuf;
	s32 m_nSendBufSize;
	LinkServerPlayback *m_pPlayback;
};


typedef std::map<void *, LinkStream *> LinkStreamMap;

