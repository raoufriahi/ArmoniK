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

#include "vdbfile.hpp"
#include "utility/videotype.hpp"

typedef enum
{
    MEDIA_SESSION_RECORD = 1,
    MEDIA_SESSION_PLAYBACK,
    MEDIA_SESSION_LAST
} MediaSessionType;



class MediaFileData;
class VE_LIBRARY_API MediaFile
{
public:
	MediaFile(string & strPath, unsigned int startTime, MediaSessionType type, 
					unsigned int maxLength, BOOL bSeekIFrame = FALSE);
	~MediaFile();
	
public:
	MFStatus PushAFrame(VideoFrame *pFrame);
	MFStatus GetAFrame(VideoFrame &pFrame, int &waiting);
	BOOL PauseForRead();
	BOOL ReadSeek(unsigned int startTime);
	BOOL ReadSeekByType(unsigned int startTime, int type, BOOL playMode);

	BOOL ReadFrame(void * pkt, unsigned int len, unsigned int time);
	BOOL ReadFrameNext(void * pkt, unsigned int len, unsigned int time, int type);
	BOOL ReadFramePrev(void * pkt, unsigned int len, unsigned int time, int type);
	BOOL SeekToTime(unsigned int seekTime);
	BOOL SeekToNextIFrame();
	BOOL SeekToPreIFrame();
	BOOL GetStartAndEndTime(unsigned int  &startTime, unsigned int &endTime);
	unsigned int GetEndTime();
private:
	BOOL Init();
	BOOL InitWrite();
	BOOL InitRead();
	
private:
	MediaFileData * m_data;
};

