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
#include "pbsession.hpp"
#include "videotype.hpp"

PlaybackSession::PlaybackSession(string deviceId, string &strBlockPath,  BOOL bSeekIFrame)
: m_DeviceId(deviceId),m_StrBlockPath(strBlockPath), 
m_MediaFile(strBlockPath, 0, MEDIA_SESSION_PLAYBACK, MF_FILE_MAX_LENGTH, bSeekIFrame)
{

}

PlaybackSession::~PlaybackSession()
{

}

/* waiting is in ms */
MFStatus PlaybackSession::GetAFrame(VideoFrame &pFrame, int &waiting)
{
	return m_MediaFile.GetAFrame(pFrame, waiting);
}

BOOL PlaybackSession::SeekToTime(unsigned int seekTime)
{
	return m_MediaFile.SeekToTime(seekTime);
}

BOOL PlaybackSession::Pause()
{
	return m_MediaFile.PauseForRead();
}
