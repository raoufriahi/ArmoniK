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
#include "velibdemo.h"
#include "vplay/vplay.hpp"
#include "vdb/pbsession.hpp"
#include "Poco/URI.h"

//#define DEMO_PLAYBACK

static void DumpHexData(unsigned char *pNuf, int nLen)
{
    int i = 0;
    for (i = 0; i < nLen; i ++)
    {
		printf(" 0x%x", pNuf[i]);
    }
	printf("\n");
}

velibdemo::velibdemo(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
#ifndef DEMO_PLAYBACK
	Poco::URI rtspUrl("rtsp://admin:12345@192.168.1.1:554/PSIA/streaming/channels/202");
	astring strRtsp;
	if (rtspUrl.empty() != true)
	{
		strRtsp = "rtsp://" + rtspUrl.getHost() + ":" + std::to_string(rtspUrl.getPort()) + rtspUrl.getPathAndQuery();
	}

	m_play.Init(TRUE, strRtsp, "admin", "12345", FALSE, VSC_CONNECT_TCP);
#else
	m_play.Init(FALSE, " ", " ", " ", FALSE);
#endif

	m_play.AttachWidget((HWND)winId(), width(), height());
#ifdef DEMO_PLAYBACK
	start();
#endif


}

velibdemo::~velibdemo()
{

}

void velibdemo::run()
{
	astring strBlockPath = "61e5eeb9-c752-45b6-b155-47347baa75a4.vm";

	PlaybackSession sess("1", strBlockPath);
	VideoFrame pFrame;
	pFrame.dataBuf = (u8 *)malloc(1024 * 1024);
	s32 waiting = 1;



	while(1)
	{
		sess.GetAFrame(pFrame, waiting);
		//printf("%s waiting:%d (%d, %d)  stream %d frame %d len %d\n", __FUNCTION__, waiting, pFrame.secs, pFrame.msecs,
		//	    pFrame.streamType, pFrame.frameType, pFrame.dataLen);
		printf("%s (%d, %d)  stream %d frame %d len %d\n", __FUNCTION__, pFrame.secs, pFrame.msecs,
			    pFrame.streamType, pFrame.frameType, pFrame.dataLen);
		m_play.PutRawData(pFrame);

		ve_sleep(waiting *2);
	}
	
}
