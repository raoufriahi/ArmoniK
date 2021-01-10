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
#ifndef __MEDIA_FILE_HPP__
#define __MEDIA_FILE_HPP__
#include "utility.hpp"
#include "vdbfile.hpp"
#include "utility/videotype.hpp"

typedef enum
{
    MEDIA_SESSION_RECORD = 1,
    MEDIA_SESSION_PLAYBACK,
    MEDIA_SESSION_LAST
} MediaSessionType;

using namespace UtilityLib;

class MediaFileData;
class VE_LIBRARY_API MediaFile
{
public:
	MediaFile(astring & strPath, u32 startTime, MediaSessionType type, 
					u32 maxLength, BOOL bSeekIFrame = FALSE);
	~MediaFile();
	
public:
	MFStatus PushAFrame(VideoFrame *pFrame);
	MFStatus GetAFrame(VideoFrame &pFrame, s32 &waiting);
	BOOL PauseForRead();
	BOOL ReadSeek(u32 startTime);
	BOOL ReadSeekByType(u32 startTime, s32 type, BOOL playMode);

	BOOL ReadFrame(void * pkt, u32 len, u32 time);
	BOOL ReadFrameNext(void * pkt, u32 len, u32 time, s32 type);
	BOOL ReadFramePrev(void * pkt, u32 len, u32 time, s32 type);
	BOOL SeekToTime(u32 seekTime);
	BOOL SeekToNextIFrame();
	BOOL SeekToPreIFrame();
	BOOL GetStartAndEndTime(u32  &startTime, u32 &endTime);
	u32 GetEndTime();
private:
	BOOL Init();
	BOOL InitWrite();
	BOOL InitRead();
	
private:
	MediaFileData * m_data;
};

#endif /* __MEDIA_FILE_HPP__ */
