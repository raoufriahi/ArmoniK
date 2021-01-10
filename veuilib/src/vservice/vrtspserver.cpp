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
#include "vrtspserver.hpp"
#include "server/factory.hpp"
#ifndef ARM_LINUX
//#include "rtspsapi.hpp"
#endif

class VRTSPServerImpl : public QThread
{
public:
	VRTSPServerImpl(Factory & pFactory);
	~VRTSPServerImpl();
public:
	void run();
	
private:
	bool m_bQuit;
	Factory & m_pFactory;
	BOOL m_pAuth;
	astring m_pMultiAddr;
	u16 m_pPort;
  
};

VRTSPServerImpl::VRTSPServerImpl(Factory & pFactory)
:m_bQuit(false), m_pFactory(pFactory), m_pPort(9554)
{
	//m_pFactory.GetRTSPServer(m_pAuth, m_pMultiAddr, m_pPort);
}

VRTSPServerImpl::~VRTSPServerImpl()
{

}

void VRTSPServerImpl::run()
{
#ifndef ARM_LINUX
#ifndef __APPLE__
    //VSCRTSPRun(m_pPort);
#endif
#endif
    return;
}



VRTSPServer::VRTSPServer(Factory & pFactory)
: m_pFactory(pFactory)
{
#ifndef ARM_LINUX
#ifndef __APPLE__
    //VSCRTSPSetFactory(pFactory);
#endif
#endif
    m_pImpl = new VRTSPServerImpl(pFactory);
    m_pImpl->start();
}

VRTSPServer::~VRTSPServer()
{

}


