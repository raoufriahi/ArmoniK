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
#include "rapidmedia/rapiddec.hpp"

const int AUDIO_DECODE_ATTEMPTS = 1;
static const size_t DEFAULT_PADDING = 16;

class RapidAudioDec :public RapidDec
{
public:
	RapidAudioDec(CodecType codec,  int bit_rate, int sample_rate, 
		int channels, const struct codec_options& options, int decodeAttempts = AUDIO_DECODE_ATTEMPTS );

	virtual ~RapidAudioDec() ;

	BOOL Decode( uint8_t* pBuf, int nSize, RawFrame & pFrame);

private:
	RapidAudioDec( const RapidAudioDec& obj );
	RapidAudioDec& operator = ( const RapidAudioDec& );

	CodecType m_codecType;
	enum AVCodecID m_id;
	AVCodec* m_codec;
	AVCodecContext* m_context;
	struct codec_options m_options;
	AVFrame* m_frame;
	int m_decodeAttempts;
};

