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

#include "vonvifdismgr.hpp"
#include "onvifserver.hpp"
#include "onvifserverdiscover.hpp"

class VONVIFDisMgrImpl : public QThread
{
public:
	VONVIFDisMgrImpl();
	~VONVIFDisMgrImpl();
public:
	void run();
	bool AddHost(astring host, astring port, astring type)
	{
		return m_pTask->AddHost(host, port,  type);
	}
	bool DelHost(astring host, astring port)
	{
		return m_pTask->DelHost(host, port);
	}
	
private:
	struct soap * m_pSoap;
	OnvifServerDiscoverTask * m_pTask;
	bool m_bQuit;

};

VONVIFDisMgrImpl::VONVIFDisMgrImpl()
:m_pSoap(soap_new()), m_bQuit(false)
{
	m_pTask = new OnvifServerDiscoverTask(m_pSoap);
}

VONVIFDisMgrImpl::~VONVIFDisMgrImpl()
{

}

void VONVIFDisMgrImpl::run()
{
	m_pTask->Run(m_bQuit);
}


VONVIFDisMgr::VONVIFDisMgr()
:m_pImpl(NULL)
{
	m_pImpl = new VONVIFDisMgrImpl();
	m_pImpl->start();
}

bool VONVIFDisMgr::AddHost(astring host, astring port, astring type)
{
    return m_pImpl->AddHost(host, port, type);
}
bool VONVIFDisMgr::DelHost(astring host, astring port)
{
    return m_pImpl->DelHost(host, port);
}

VONVIFDisMgr::~VONVIFDisMgr()
{

}

