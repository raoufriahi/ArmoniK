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
#ifndef __PB_SESSION_HPP__
#define __PB_SESSION_HPP__
#include "utility.hpp"
#include "mediafile.hpp"

class VE_LIBRARY_API PlaybackSession
{
public:
    /* bSeekIFrame is for play a new block from seek method  */
    PlaybackSession(astring deviceId, astring &strBlockPath, BOOL bSeekIFrame = FALSE);
    ~PlaybackSession();
public:
	MFStatus GetAFrame(VideoFrame &pFrame, s32 &waiting);
	BOOL SeekToTime(u32 seekTime);
	BOOL Pause();
	astring GetBlockPath()
	{
		return m_StrBlockPath;
	}
	
private:
	astring m_DeviceId;
    astring m_StrBlockPath;
    u32 m_startTime;
    MediaFile m_MediaFile;
};

#endif /* __RECORD_SESSION_HPP__ */
