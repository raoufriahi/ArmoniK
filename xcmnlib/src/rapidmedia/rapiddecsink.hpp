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
#ifndef __RAPID_DEC_SINK_HPP__
#define __RAPID_DEC_SINK_HPP__

#include "rapidmedia/rapidmedia.hpp"
#include "rapidmedia/rapidmediadata.hpp"
#include "cppkit/ck_memory.h"
#include "rapidrtsp.h"
#include "XSDK/TimeUtils.h"
#include "XSDK/XBlockingQueue.h"
#include "rapidmedia/rapidaudiodec.hpp"
#include "rapidmedia/rapidffmpegdec.hpp"

#include "ffkit/fflocky.h"

using namespace ffkit;

using namespace cppkit;
using namespace XSDK;


class RapidDecSink
{
public:
	RapidDecSink(RMRawVideoHandler rawVideoHandler, 
		void *pRawVideoContext, BOOL HWAccel);
	~RapidDecSink() ;

public:
	BOOL DecodeFrame(VideoFrame & pFrame);
	BOOL DecodeAFrame(VideoFrame & pFrame);
	BOOL DecodeVFrame(VideoFrame & pFrame);
private:
	RMRawVideoHandler m_rawVideoHandler;
	void *m_pRawVideoContext;

	RapidDec *m_pVideoDec;
	RapidAudioDec *m_pAudioDec;
	CodecType m_AudioCodec;
	CodecType m_VideoCodec;
	BOOL m_HWAccel;

	InfoFrameI m_FrameI;
	BOOL m_bGotFrameI;
};

#endif /* __RAPID_DEC_SINK_HPP__ */
