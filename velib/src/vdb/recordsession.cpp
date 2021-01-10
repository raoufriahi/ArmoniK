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
#include "recordsession.hpp"

RecordSession::RecordSession(astring deviceId, astring &strBlockPath, u32 startTime, s64 recordId)
: m_DeviceId(deviceId),m_StrBlockPath(strBlockPath), 
m_MediaFile(strBlockPath, startTime, MEDIA_SESSION_RECORD, MF_FILE_MAX_LENGTH),
m_RecordId(recordId)
{

}
RecordSession::~RecordSession()
{

}

MFStatus RecordSession::PushAFrame(VideoFrame *pFrame)
{
	return m_MediaFile.PushAFrame(pFrame);
}
u32 RecordSession::GetEndTime()
{
	return m_MediaFile.GetEndTime();
}
s64 RecordSession::GetRecordId()
{
	return m_RecordId;
}
astring & RecordSession::GetBlockPath()
{
	return m_StrBlockPath;
}
