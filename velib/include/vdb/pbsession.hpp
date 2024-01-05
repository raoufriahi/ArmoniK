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

class VE_LIBRARY_API PlaybackSession
{
public:
    /* bSeekIFrame is for play a new block from seek method  */
    PlaybackSession(string deviceId, string &strBlockPath, BOOL bSeekIFrame = FALSE);
    ~PlaybackSession();
public:
	MFStatus GetAFrame(VideoFrame &pFrame, int &waiting);
	BOOL SeekToTime(unsigned int seekTime);
	BOOL Pause();
	string GetBlockPath() {
		return m_StrBlockPath;
	}
	
private:
	string m_DeviceId;
    string m_StrBlockPath;
    unsigned int m_startTime;
    MediaFile m_MediaFile;
};

