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
#include "config/conf.hpp"
#include "debug.hpp"

#include "XSDK/XMutex.h"
#include "XSDK/XGuard.h"
#include <Poco/AutoPtr.h>
#include <Poco/Util/SystemConfiguration.h>
#include <Poco/Format.h>
#include <Poco/Util/Application.h>
#include <Poco/Environment.h>

using namespace UtilityLib;
using namespace XSDK;
using namespace Poco::Util;

#define VID_PATH_SPLIT "/"


//XGuard guard(m_cMutex);
class VE_LIBRARY_API VidEnv : public Application
{
public:
	VidEnv()
	:m_sys(new Poco::Util::SystemConfiguration)
	{
		m_syshomedir = m_sys->getString("system.homeDir");
		printf("Home dir --> %s\n", m_syshomedir.c_str());
	}
	~VidEnv(){}
protected:
	void initialize(Application& self)
	{
		//Application::initialize(self);
		// add your own initialization code here
	}
	
	void uninitialize()
	{
		// add your own uninitialization code here
		//Application::uninitialize();
	}
	
	void reinitialize(Application& self)
	{
		//Application::reinitialize(self);
		// add your own reinitialization code here
	}

	void defineOptions(OptionSet& options)
	{

		//Application::defineOptions(options);
#if 0
		options.addOption(
			Option("help", "h", "display help information on command line arguments")
				.required(false)
				.repeatable(false)
				.callback(OptionCallback<SampleApp>(this, &SampleApp::handleHelp)));

		options.addOption(
			Option("define", "D", "define a configuration property")
				.required(false)
				.repeatable(true)
				.argument("name=value")
				.callback(OptionCallback<SampleApp>(this, &SampleApp::handleDefine)));
				
		options.addOption(
			Option("config-file", "f", "load configuration data from a file")
				.required(false)
				.repeatable(true)
				.argument("file")
				.callback(OptionCallback<SampleApp>(this, &SampleApp::handleConfig)));

		options.addOption(
			Option("bind", "b", "bind option value to test.property")
				.required(false)
				.repeatable(false)
				.argument("value")
				.binding("test.property"));
#endif
	}
	int main(const ArgVec& args)
	{
		
		m_appdir = this->config().getString("application.dir");
		printf("Application dir --> %s\n", m_appdir.c_str());
		Init();
		return Application::EXIT_OK;	
	}
	void handleOption(const std::string& name, const std::string& value)
	{
	}
public:
	bool Init()
	{
		Poco::Environment::set("QT_QPA_FONTDIR", GetAppDir() + "/lib/fonts/");
		Poco::Environment::set("QT_QPA_PLATFORM_PLUGIN_PATH", GetAppDir() + "/plugins/platforms/");
		return true;
	}
	
	
	astring GetAppDir()
	{
		return m_appdir + VID_PATH_SPLIT;
	}
	astring GetConfDir()
	{
		return m_syshomedir + VID_PATH_SPLIT;
	}

	string GetAppConfPath(astring strApp)
	{
		return GetConfDir() + "vidstor/" + strApp + "/";
	}
	
	bool PrintEnv()
	{
		{
			/* application */
			const char* const properties[] = { 
				  "application.path"
				, "application.name"
				, "application.baseName"
				, "application.dir"
				, "application.configDir"
			};

			std::size_t maxLength = 0;
			for (size_t i = 0; i<sizeof(properties) / sizeof(properties[0]); ++i) {
				std::size_t len = std::string(properties[i]).length();
				maxLength = (maxLength < len) ? len : maxLength;
			}

			for (size_t i = 0; i<sizeof(properties) / sizeof(properties[0]); ++i)
			{
				std::string property(properties[i]);

				std::string padding(" ");
				std::size_t paddingCount = maxLength - property.length();
				while (0 != paddingCount)
				{
					padding += " ";
					--paddingCount;
				}
				printf("properties[i] %s--> %s\n", properties[i], this->config().getString(property).c_str());
			}
		}

		{
			/* system */
			const char* const properties[] = { "system.osName"
				, "system.osVersion"
				, "system.osArchitecture"
				, "system.nodeName"
				, "system.currentDir"
				, "system.homeDir"
				, "system.tempDir"
				, "system.dateTime"
				, "system.pid"
				, "system.env.HOME"
			};

			std::size_t maxLength = 0;
			for (size_t i = 0; i<sizeof(properties) / sizeof(properties[0]); ++i) {
				std::size_t len = std::string(properties[i]).length();
				maxLength = (maxLength < len) ? len : maxLength;
			}

			for (size_t i = 0; i<sizeof(properties) / sizeof(properties[0]); ++i)
			{
				std::string property(properties[i]);

				std::string padding(" ");
				std::size_t paddingCount = maxLength - property.length();
				while (0 != paddingCount)
				{
					padding += " ";
					--paddingCount;
				}
				printf("properties[i] %s--> %s\n", properties[i], m_sys->getString(property).c_str());
			}
		}
		return true;
	}

private:
	//Poco::Util::Application *m_app;
	Poco::Util::SystemConfiguration *m_sys;
	string m_appdir;
	string m_syshomedir;
};

typedef VidEnv* LPVidEnv;

