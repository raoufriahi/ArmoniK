/*
 * Copyright (c) 2017-2024 UbVideo
 *
 * The computer program contained herein contains proprietary
 * information which is the property of UbVideo.
 * The program may be used and/or copied only with the written
 * permission of UbVideo or in accordance with the
 * terms and conditions stipulated in the agreement/contract under
 * which the programs have been supplied.
 */
#pragma once

#include "Poco/Buffer.h"
typedef Poco::Buffer<u8> VEBuffer;

using namespace std;

typedef struct __VideoSeqFrame
{
	VideoSeqType seqType;
 	unsigned int start;/*Start time of the video*/
	unsigned int end;/*End time of the video*/
	VEBuffer  * dataBuf;
}VideoSeqFrame;

typedef BOOL (*HDFSDataHandler)(void* pData, VideoSeqFrame& frame);

class VHdfsDB;
class HdfsRecSessionData;
class VE_LIBRARY_API HdfsRecSession
{
public:
	HdfsRecSession(int deviceId, string strName, VHdfsDB &pDB);
	~HdfsRecSession();
public:
	MFStatus PushAFrame(VideoFrame *pFrame);

	BOOL RegSeqCallback(HDFSDataHandler pCallback, void * pParam);
	BOOL UnRegSeqCallback(void * pParam);
private:
    	HdfsRecSessionData * m_data;
};


