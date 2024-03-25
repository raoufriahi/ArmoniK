/*
 * Copyright (c) 2017-2023 Heimdall
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
/**
 * \brief Dumps hexadecimal data to the console.
 *
 * This function takes a pointer to a buffer containing binary data and its length,
 * and outputs the hexadecimal representation of the data to the console.
 *
 * \param pNuf Pointer to the data buffer.
 * \param nLen Length of the data buffer.
 *
 * \note This function assumes that the buffer is allocated and valid.
 *       It does not perform any boundary checks.
 */
static void DumpHexData(unsigned char *pNuf, int nLen)
{
    int i = 0;
    for (i = 0; i < nLen; i ++)
    {
		printf(" 0x%x", pNuf[i]);
    }
	printf("\n");
}

/**
 * \brief Constructs a new `velibdemo` object.
 *
 * This constructor initializes a `velibdemo` object, which is a subclass of `QMainWindow`.
 *
 * \param parent The parent widget of the `velibdemo` object.
 */
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

/**
 * \brief Destructor for the velibdemo class.
 * 
 * This destructor is responsible for cleaning up any resources allocated
 * by the velibdemo class. It is automatically called when an object of
 * the velibdemo class is destroyed.
 *
 * \note The destructor does not take any parameters and has no return value.
 */
velibdemo::~velibdemo()
{
    // Implementation details of the destructor...
    // (Add relevant information if needed)
}


/**
 * \brief Executes the main functionality of the velibdemo.
 *
 * This function runs the main logic of the velibdemo application. It performs the necessary setup,
 * initializes variables, and starts the execution loop. It continuously checks for incoming data
 * and processes it accordingly until the application is stopped.
 *
 * \note This function should be called after initializing the velibdemo object.
 *
 * \see velibdemo
 */
void velibdemo::run()
{
	astring strBlockPath = "61e5eeb9-c752-45b6-b155-47347baa75a4.vm";

	PlaybackSession sess("1", strBlockPath);
	VideoFrame pFrame;
	pFrame.dataBuf = (u8 *)malloc(1024 * 1024);
	s32 waiting = 1;

	for(;;){
		sess.GetAFrame(pFrame, waiting);
		//printf("%s waiting:%d (%d, %d)  stream %d frame %d len %d\n", __FUNCTION__, waiting, pFrame.secs, pFrame.msecs,
		//	    pFrame.streamType, pFrame.frameType, pFrame.dataLen);
		printf("%s (%d, %d)  stream %d frame %d len %d\n", __FUNCTION__, pFrame.secs, pFrame.msecs,
			    pFrame.streamType, pFrame.frameType, pFrame.dataLen);
		m_play.PutRawData(pFrame);

		ve_sleep(waiting *2);
	}
	
}
