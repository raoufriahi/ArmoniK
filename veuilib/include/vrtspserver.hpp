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
#ifndef __V_RTSP_SERVER_HPP__
#define __V_RTSP_SERVER_HPP__
#include "utility.hpp"
#include "debug.hpp"
#include "videotype.hpp"
#include <QWidget>
#include "server/factory.hpp"
#include <QThread>

using namespace UtilityLib;


class VRTSPServerImpl;
class VE_LIBRARY_API VRTSPServer
{
public:
	VRTSPServer(Factory & pFactory);
	~VRTSPServer();
public:
private:
    Factory & m_pFactory;
    VRTSPServerImpl *m_pImpl;
};

#endif /* __V_RTSP_SERVER_HPP__ */
