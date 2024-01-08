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

class RapidMediaData;
class VE_LIBRARY_API RapidMedia
{
public:
	RapidMedia();
	~RapidMedia();

	/* The Decode one i frame  */
	static bool DecodeiFrame(VideoFrame& packet, RawFrame& pRaw, void* &pDecoder);
	static bool DecodeiFrameFreeDec(void* pDec);
    static std::string GetCodecName(CodecType codec);

	BOOL Init(BOOL bRealStream, string strUrl, string strUser, 
				string strPass, BOOL bHWAccel = FALSE, VSCConnectType connectType = VSC_CONNECT_TCP);
	BOOL PutData(VideoFrame& packet);

	void	SetDataHandler(RMDataHandler handle, void *pContext);
	void	SetRawDataHandler(RMRawVideoHandler handle, void *pContext);

	BOOL Start();
	BOOL Stop();
	BOOL StartRaw();
	BOOL StopRaw();
private:
	RapidMediaData * m_data;
};
