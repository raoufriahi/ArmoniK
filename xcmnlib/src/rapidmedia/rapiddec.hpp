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
#include "utility/videotype.hpp"
#include "ffkit/ffoptions.h"
#include "ffkit/fflocky.h"
using namespace ffkit;

class RapidDec
{
public:
	RapidDec(RMRawVideoHandler pHandler = NULL,  void * pVideoContext = NULL)
	:m_Init(FALSE), m_rawVideoHandler(pHandler), m_pRawVideoContext(pVideoContext)
	{
	}
	virtual ~RapidDec() {}

	virtual BOOL Init(){return FALSE;}
	virtual BOOL Decode( uint8_t* pBuf, int nSize, RawFrame & pFrame){return FALSE;}

protected:

	BOOL m_Init;
	RMRawVideoHandler m_rawVideoHandler;
	void *m_pRawVideoContext;
};

