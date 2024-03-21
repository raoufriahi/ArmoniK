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
#ifndef __V_VID_ONVIF_CLIENT__
#define __V_VID_ONVIF_CLIENT__

#include "utility.hpp"
#include "debug.hpp"

typedef enum
{
	VVID_PTZ_UP = 1,
	VVID_PTZ_DOWN,
	VVID_PTZ_LEFT,
	VVID_PTZ_RIGHT,
	VVID_PTZ_ZOOM_IN,
	VVID_PTZ_ZOOM_OUT,
	VVID_PTZ_STOP,
	VVID_PTZ_LAST
} VVidOnvifCPtzAct;

class VE_LIBRARY_API VVidOnvifProfile
{
public:
	VVidOnvifProfile()
	:nWidth(0), nHeight(0), nFps(0), nBandwidth(0), bGotUrl(false)
	{
		
	}
public:
	astring strProfileToken;
	astring strCodecName;
	astring strNameDisplay;
	astring strRtspUrl;
	s32 nWidth;
	s32 nHeight;
	s32 nFps;
	s32 nBandwidth;
	bool bGotUrl;
};

typedef std::map<astring, VVidOnvifProfile> VVidOnvifProfileMap;

class VE_LIBRARY_API VVidOnvifC
{
public:
	VVidOnvifC(){}
	~VVidOnvifC(){}
public:
	static bool CheckOnline(astring strUser, astring strPasswd, astring strUrl);
	static bool GetProfiles(astring strUser, astring strPasswd, astring strUrl, 
			VVidOnvifProfileMap &pMap);
};

class VVidOnvifCPtzPri;

class VE_LIBRARY_API VVidOnvifCPtz
{
public:
	VVidOnvifCPtz();
	~VVidOnvifCPtz();
public:
	bool Init(astring strUser, astring strPasswd, astring strUrl);
	bool PtzAction(VVidOnvifCPtzAct act, float speed);
private:
	bool m_bInit;
	astring m_strUser;
	astring m_strPass;
	astring m_strPtzUrl;
	VVidOnvifCPtzPri *m_pPri;
};


#endif /* __V_VID_ONVIF_CLIENT__ */
