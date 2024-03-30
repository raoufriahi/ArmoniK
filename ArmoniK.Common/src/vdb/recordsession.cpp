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
#include "recordsession.hpp"

RecordSession::RecordSession(string deviceId, string &strBlockPath, unsigned int startTime, long recordId)
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
unsigned int RecordSession::GetEndTime()
{
	return m_MediaFile.GetEndTime();
}
long RecordSession::GetRecordId()
{
	return m_RecordId;
}
string & RecordSession::GetBlockPath()
{
	return m_StrBlockPath;
}
