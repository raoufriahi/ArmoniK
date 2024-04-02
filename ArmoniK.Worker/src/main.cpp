/*
 * Copyright (c) 2017-2023 UbVideo
 *
 * The computer program contained herein contains proprietary
 * information which is the property of UbVideo.
 * The program may be used and/or copied only with the written
 * permission of UbVideo or in accordance with the
 * terms and conditions stipulated in the agreement/contract under
 * which the programs have been supplied.
 */

#include "vevent.hpp"
#include "webserver.hpp"
#include "vrtspserver.hpp"
#include "mediaserver.hpp"

Factory *gFactory = NULL;
string gAppdir;


/**
 * \brief Signal handler function for handling various signals.
 *
 * This function prints information about the received signal and performs
 * specific actions for certain signals, such as SIGINT and SIGTERM.
 *
 * \param signum The signal number received.
 *
 * \details
 * The function prints the received signal number and handles specific signals:
 * - SIGINT (Ctrl+C): Exits the program with a status of 0.
 * - SIGTERM: Exits the program with a status of 0.
 *
 * \note
 * Ensure that this signal handler is registered appropriately using the
 * signal() or sigaction() functions to handle the desired signals.
 *
 * Example of registering the handler for SIGINT:
 * \code
 * signal(SIGINT, signalHandler);
 * \endcode
 *
 * \param signum The signal number received.
 */
void signalHandler(int signum) {

	VDC_DEBUG("Signal received: %d.\n",signum );
    // Handle specific signals here
    switch(signum) {
        case SIGINT:
            VDC_DEBUG("Handling SIGINT (Ctrl+C) signal.\n");
            exit(0);
            break;
        case SIGTERM:
            VDC_DEBUG("Handling SIGTERM signal.\n");
            exit(0);
            break;
    }
}


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
static BOOL WebServerUserChangeNotify(void* pParam, string strUser, string strPasswd)
{
	/*string strPasswdPath = gAppdir + ".htpasswd";
	SimpleCrypt crypt;
	QString strDePasswd = strPasswd.c_str();

    mg_modify_passwords_file(strPasswdPath.c_str(), "heimdall.com", strUser.c_str(),
		crypt.decryptToString(strDePasswd).toStdString().c_str());*/
	return TRUE;
	
}

int main(int argc, char *argv[])
{

	// Initialize Factory and set program name
    Factory* pFactory = nullptr;
    std::string programName = "UbCore";

     // Register signal handlers
    signal(SIGINT, signalHandler);  // Interrupt signal (Ctrl+C)
    signal(SIGTERM, signalHandler); // Termination signal

    // Set up arguments for VidEnv initialization
    char* vidEnvArgv[] = {(char*)programName.c_str(), nullptr};
    int vidEnvArgc = sizeof(vidEnvArgv) / sizeof(char*) - 1;

    // Initialize VidEnv
    VidEnv env;
    env.init(vidEnvArgc, vidEnvArgv);
    env.run();

    // Set up logger path and create directories
    std::string loggerPath = env.GetAppConfPath("logs");
    Poco::File loggerDir(loggerPath);
    loggerDir.createDirectories();

    // Initialize Debug with logger port and file path
    std::string loggerFile = loggerPath + "vidstorlog";
    Debug::init(9100, loggerFile);

	Debug::logger().info("Running the main program...");	
    pFactory = new Factory(env);
	gFactory = pFactory;
	pFactory->Init();
	//QApplication a(argc1, argv1);

#if 0
	astring strIdTest = "c9d03a04-6ff0-4733-a5bc-6be41876f080";
	VidCamera camTest;
	pFactory->GetConfDB().GetCameraConf(strIdTest, camTest);
#endif

	// Obtain the application directory
	string appDir = env.GetAppDir();
	// Set up paths
	string docRoot = appDir + "www";
	string strPasswdPath = appDir + ".htpasswd";
	// Set the default password for admin
	string strPasswd = "admin";
	pFactory->GetAdminPasswd(strPasswd);
	WebServerUserChangeNotify(nullptr, "admin", strPasswd);
	// Set up web server options

	const char *options[] = {
        "document_root", docRoot.c_str(),
		"listening_ports", PORT, 
		"global_auth_file", strPasswdPath.c_str(),
        "authentication_domain", "heimdall.com",
		0};
	// Convert options to a vector of strings
	std::vector<std::string> cpp_options(options, options + sizeof(options) / sizeof(options[0]) - 1);

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

	pFactory->join();

	/* Init media server */
	VEMediaServer::InitMediaServer();
	/* Start Media server */
	VEMediaServer * pMediaServer = new VEMediaServer(*pFactory);
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
	// Infinite loop to keep the program running
    while (true) {
       sleep(1);  // Introduce a delay to simulate work
    }
	// Rest of the program cleanup or finalization
    VDC_DEBUG("Exiting the main program...  \n");
	return 0;
}
