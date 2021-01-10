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
#ifndef __RAPID_DEC_HPP__
#define __RAPID_DEC_HPP__
#include "utility/type.hpp"
#include "utility/videotype.hpp"
#include "stddef.h"
#include "ffkit/ffoptions.h"
#include "ffkit/av_demuxer.h"
#include "ffkit/av_packet.h"
#include "ffkit/av_packet_factory.h"
#include "ffkit/fflocky.h"

using namespace cppkit;
using namespace std;
using namespace ffkit;

class RapidDec
{
public:
	RapidDec(RMRawVideoHandler pHandler = NULL,  void * pVideoContext = NULL)
	:m_Init(FALSE), m_rawVideoHandler(pHandler), m_pRawVideoContext(pVideoContext)
	{
	}
	virtual ~RapidDec() {}
public:
	virtual BOOL Init(){return FALSE;}
	virtual BOOL Decode( uint8_t* pBuf, int nSize, RawFrame & pFrame){return FALSE;}

protected:

	BOOL m_Init;
	RMRawVideoHandler m_rawVideoHandler;
	void *m_pRawVideoContext;
};

#endif /* __RAPID_DEC_HPP__ */
