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

#include "vservicemgr.hpp"

VServiceMgr * g_pVServiceMgr =NULL;
Factory *gUiFactory;

VServiceMgr::VServiceMgr(Factory & pFactory)
:m_pONVIFDisMgr(NULL), 
m_pVRTSPServer(NULL), m_pVTaskMgr(NULL), 
#if defined (WIN32) && !defined (WIN64)
m_pVVIPCMgr(NULL), m_pVONVIFPGMgr(NULL),
#endif
m_pFactory(pFactory)
{
    gUiFactory = &pFactory;
    m_pONVIFDisMgr  = new VONVIFDisMgr();
    m_pVVIPCMgr = new VVIPCMgr(pFactory, *m_pONVIFDisMgr);
    m_pVRTSPServer = new VRTSPServer(pFactory);
#if defined (WIN32__REMOVE) && !defined (WIN64)
    m_pVHTTPServer = new VHTTPServer(pFactory);
    m_pVHLSServer = new VHLSServer(pFactory);
#endif
    m_pVTaskMgr = new VTaskMgr();
    
}

VServiceMgr::~VServiceMgr()
{

}

VServiceMgr * VServiceMgr::CreateObject(Factory & pFactory)
{
	if (g_pVServiceMgr == NULL)
	{
		g_pVServiceMgr = new VServiceMgr(pFactory);
	}

	return g_pVServiceMgr;
}

