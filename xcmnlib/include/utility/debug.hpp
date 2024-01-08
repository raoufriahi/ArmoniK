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

#include "utility.hpp"
#include "spdlog/spdlog.h"
#define NOMINMAX
#include <string>

#define MODE_CONFIG_INT             10

using namespace UtilityLib;
using namespace tthread;

namespace spd = spdlog;

enum RapidLogLevel
{
	logRERROR, 
	logRWARNING, 
	logRINFO, 
	logRDEBUG
};

struct cli_def;
class VE_LIBRARY_API Debug 
{
public:
	Debug(s32 nPort, std::string strLoggerPath)
	:m_nPort(nPort), m_strLoggerPath(strLoggerPath)
	{
		spd::set_level(spd::level::debug);
		m_pLogger = spd::rotating_logger_mt("heimdall", m_strLoggerPath, 1024 * 1024 * 5, 3, true);
	}

	static void run(void * pParam);
	void run1();
	spd::logger  & getLogger();


	static void init(s32 nPort, std::string strLoggerPath);
	static void DebugPrint( const char* format, ... );
	static void RapidLog(RapidLogLevel level, const char* format, ... );
	static void RapidNetLog(RapidLogLevel level, const char* format, ... );
	static spd::logger & logger();

	void setup();
	
	static int check_auth(const char *username, const char *password);
    static int check_enable(const char *password);
	
	static int test(struct cli_def *cli, const char *command, 
		char *argv[], int argc);

private:
	static Debug * gDebug;
	static tthread::thread *gThread;
public:
    struct cli_def *cli;
	s32 m_nPort;
	std::shared_ptr<spd::logger> m_pLogger;
	std::string m_strLoggerPath;

};

#define  VDC_DEBUG Debug::DebugPrint
#define  R_LOG Debug::RapidLog
#define  R_NET_LOG Debug::RapidNetLog

