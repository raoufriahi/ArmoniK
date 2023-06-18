/*
 * Copyright (c) 2017-2023 Heimdall
 *
 * The computer program contained herein contains proprietary
 * information which is the property of Heimdall.
 * The program may be used and/or copied only with the written
 * permission of Heimdall or in accordance with the
 * terms and conditions stipulated in the agreement/contract under
 * which the programs have been supplied.
 */
#include <QtWidgets/QApplication>
#include <QStyleFactory>
#include "server/factory.hpp"
#include <QPixmap>
#include <QSplashScreen>
#include <QtWidgets/QMainWindow>
#include <QTranslator>
#include <QTextCodec>
#include "vevent.hpp"
#include "webserver.hpp"
#include "vrtspserver.hpp"
#include "server/eventserver.hpp"
#include "vonvifdismgr.hpp"

Factory *gFactory = NULL;
astring gAppdir;

#include <Poco/AutoPtr.h>
#include <Poco/Util/SystemConfiguration.h>
#include <Poco/Format.h>
#include <Poco/Util/Application.h>

/* The media server only support linux & macOS */
#ifndef _WIN32
#include "mediaserver.hpp"
#endif

/**
 * \brief Notifies the web server about a user's password change.
 *
 * This function is responsible for notifying the web server when a user's password is changed.
 * The web server can take appropriate actions based on this notification.
 *
 * \param pParam    A pointer to additional parameters or context data (if any).
 * \param strUser   The username of the user whose password was changed.
 * \param strPasswd The new password for the user.
 * \return          Returns a BOOL value indicating the success or failure of the notification.
 *
 * \note            This function should be called whenever a user's password is changed
 *                  to ensure the web server is aware of the change.
 */
static BOOL WebServerUserChangeNotify(void* pParam, astring strUser, astring strPasswd)
{
	astring strPasswdPath = gAppdir + ".htpasswd";
	SimpleCrypt crypt;
	QString strDePasswd = strPasswd.c_str();

    mg_modify_passwords_file(strPasswdPath.c_str(), "heimdall.com", strUser.c_str(),
		crypt.decryptToString(strDePasswd).toStdString().c_str());
	return TRUE;
	
}

int main(int argc, char *argv[])
{
	Factory *pFactory = NULL;
	char *argv1[] = 
        {"heimdall",
		"-platform", 
		"offscreen", 
		NULL};

    int argc1 = sizeof(argv1) / sizeof(char*) - 1;
    char *argv2[] =
    {"heimdall",
     NULL
    };

    int argc2 = sizeof(argv2) / sizeof(char*) - 1;
    VidEnv env;
	env.init(argc2, argv2);
	env.run();

	astring strLoggerPath = env.GetAppConfPath("logs");

	Poco::File file1(strLoggerPath);
	file1.createDirectories();
	astring strLoggerFile = strLoggerPath + "vidstorlog";
	Debug::init(9100, strLoggerFile);

	Debug::logger().info("vidstor started");	
    pFactory = new Factory(env);
	gFactory = pFactory;
	pFactory->Init();
	QApplication a(argc1, argv1);

#if 0
	astring strIdTest = "c9d03a04-6ff0-4733-a5bc-6be41876f080";
	VidCamera camTest;
	pFactory->GetConfDB().GetCameraConf(strIdTest, camTest);
#endif
	
	gAppdir = env.GetAppDir();
    astring docRoot = env.GetAppDir()+ "www";
	astring strPasswdPath = env.GetAppDir() + ".htpasswd";
	/* set htpasswd, when start a  */
	astring strPasswd = "admin";
	pFactory->GetAdminPasswd(strPasswd);
	WebServerUserChangeNotify(NULL, "admin", strPasswd);
	
	const char *options[] = {
        "document_root", docRoot.c_str(),
		"listening_ports", PORT, 
		"global_auth_file", strPasswdPath.c_str(),
        "authentication_domain", "heimdall.com",
		0};
    
	std::vector<std::string> cpp_options;
	for (int i=0; i<(sizeof(options)/sizeof(options[0])-1); i++) {
	    cpp_options.push_back(options[i]);
	}
	
	/* Start RTSP server */
	VRTSPServer *pRTSPServer = new VRTSPServer(*pFactory);

	/* Init Event Server */
	VEventServer *pEventServer = new VEventServer(*pFactory);
	pEventServer->Init();

	/* Init Event framework */
	VEventMgr *pEventMgr = new VEventMgr(*pFactory, *pEventServer);
	pEventMgr->Init();

	VEWebServer *  pWebServer = new VEWebServer(cpp_options, *pFactory, *pEventServer);
	
	pFactory->RegUserChangeNotify(pWebServer, WebServerUserChangeNotify);

	pFactory->start();
	
#ifndef _WIN32
	/* Init media server */
	VEMediaServer::InitMediaServer();
	/* Start Media server */
	VEMediaServer * pMediaServer = new VEMediaServer(*pFactory);
#endif
#if 0
	VONVIFDisMgr *pDisMgr = new VONVIFDisMgr();
	
	QList<QHostAddress> hostAddr = DeviceSearcher::getHostAddress();

	QList<QHostAddress>::iterator i;
	for(i=hostAddr.begin();i!=hostAddr.end();i++)
	{
		pDisMgr->AddHost(((*i).toString()).toStdString(), "9081", "Network_Video_Storage");
	}
#endif
	VDC_DEBUG("Start successfully !\n");
	
	return a.exec();
}
