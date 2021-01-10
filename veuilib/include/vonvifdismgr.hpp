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
#ifndef __V_ONVIF_DIS_MGR_HPP__
#define __V_ONVIF_DIS_MGR_HPP__
#include "utility.hpp"
#include "debug.hpp"
#include "videotype.hpp"
#include <QWidget>
#include <QThread>

using namespace UtilityLib;


class VONVIFDisMgrImpl;
class VE_LIBRARY_API VONVIFDisMgr
{
public:
	VONVIFDisMgr();
	~VONVIFDisMgr();
public:
	/* Type is Network_Video_Storage or NetworkVideoTransmitter */
	bool AddHost(astring host, astring port, astring type);
	bool DelHost(astring host, astring port);
	
private:
	VONVIFDisMgrImpl *m_pImpl;
};

#endif /* __V_ONVIF_DIS_MGR_HPP__ */
