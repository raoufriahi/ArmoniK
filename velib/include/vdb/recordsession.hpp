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

#include "mediafile.hpp"

class VE_LIBRARY_API RecordSession
{
public:
	RecordSession(string deviceId, string &strBlockPath, unsigned int startTime, long recordId);
    ~RecordSession();
public:
	MFStatus PushAFrame(VideoFrame *pFrame);
	unsigned int GetEndTime();
	long GetRecordId();
	string & GetBlockPath();

private:
	string m_DeviceId;
    string m_StrBlockPath;
    unsigned int m_startTime;
    MediaFile m_MediaFile;
    long m_RecordId;
};

