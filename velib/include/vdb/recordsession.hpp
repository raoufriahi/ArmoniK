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
#ifndef __RECORD_SESSION_HPP__
#define __RECORD_SESSION_HPP__
#include "utility.hpp"
#include "mediafile.hpp"

class VE_LIBRARY_API RecordSession
{
public:
	RecordSession(astring deviceId, astring &strBlockPath, u32 startTime, s64 recordId);
    ~RecordSession();
public:
	MFStatus PushAFrame(VideoFrame *pFrame);
	u32 GetEndTime();
	s64 GetRecordId();
	astring & GetBlockPath();

private:
	astring m_DeviceId;
    astring m_StrBlockPath;
    u32 m_startTime;
    MediaFile m_MediaFile;
    s64 m_RecordId;
};

#endif /* __RECORD_SESSION_HPP__ */
