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
#ifndef __RAPID_FFMPEG_DEC_HPP__
#define __RAPID_FFMPEG_DEC_HPP__

#include "ffkit/ffoptions.h"
#include "ffkit/av_demuxer.h"
#include "ffkit/av_packet.h"
#include "ffkit/av_packet_factory.h"
#include "cppkit/ck_types.h"
#include "cppkit/ck_memory.h"
#include "rapidmedia/rapiddec.hpp"
#include "utility/videotype.hpp"

extern "C"
{
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
}

using namespace ffkit;
using namespace cppkit;
using namespace std;

const int FFMPEG_DECODE_ATTEMPTS = 1;

class RapidFFMPEGDec :public RapidDec
{
public:
    RapidFFMPEGDec( const struct codec_options& options, enum AVCodecID id, int decodeAttempts = FFMPEG_DECODE_ATTEMPTS );

    virtual ~RapidFFMPEGDec() ;

    BOOL Decode( uint8_t* pBuf, int nSize, RawFrame & pFrame);

private:
    RapidFFMPEGDec( const RapidFFMPEGDec& obj );
    RapidFFMPEGDec& operator = ( const RapidFFMPEGDec& );
	enum AVCodecID m_id;
	AVCodec* m_codec;
	AVCodecContext* m_context;
	struct codec_options m_options;
	AVFrame* m_frame;
	int m_decodeAttempts;
};

#endif /* __RAPID_FFMPEG_DEC_HPP__ */
