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
#include "vscmainwindows.h"
#include <QtWidgets/QApplication>
#include <QStyleFactory>
#include "server/factory.hpp"
#include "client/clientfactory.hpp"
#include <QPixmap>
#include <QSplashScreen>
#include <QtWidgets/QMainWindow>
#include <QTranslator>
#include <QTextCodec>
#include "vstyle.hpp"
#include "vsclogin.h"
#include "vtaskmgr.hpp"
#include <QFileDialog>

Q_DECLARE_METATYPE (std::string) 

void LoadLangZH(QApplication &a)
{
	QTranslator *translator = new QTranslator(&a);
	bool ok = translator->load("opencvr_zh.qm",
		QCoreApplication::applicationDirPath() + "/translations");

	if (ok)
	{
		qDebug("Translation Files loaded.");
		a.installTranslator(translator);
	}
	QTranslator *translatorVe = new QTranslator(&a);
	ok = translatorVe->load("veuilib_zh.qm",
		QCoreApplication::applicationDirPath() + "/translations");

	if (ok)
	{
		qDebug("Translation Files loaded.");
		a.installTranslator(translatorVe);
	}
	return ;
}

int main(int argc, char *argv[])
{
	int dummy = errno;
	ClientFactory *pFactory = NULL;

	char *argv1[] = 
        {"Heimdall",
		NULL};
	int argc1 = sizeof(argv1) / sizeof(char*) - 1;

        char *argv2[] =
                {"Heimdall",
                NULL};
    int argc2 = sizeof(argv2) / sizeof(char*) - 1;
	VidEnv env;
	env.init(argc2, argv2);
	env.run();
	
	QApplication a(argc1, argv1);
#if 0
	QFileDialog *fileDialog = new QFileDialog();

	if (fileDialog->exec() == QDialog::Accepted) {
	}
#endif
	a.setStyle(new VStyle);
	

	astring strLoggerPath = env.GetAppConfPath("logs");
	Poco::File file1(strLoggerPath);
	file1.createDirectories();
	astring strLoggerFile = strLoggerPath + "rapidclient";
	Debug::init(9200, strLoggerFile);

    Debug::logger().info("Heimdall started");
	//Debug::logger().info("opencvrclient started {} {}", __LINE__, __FUNCTION__);
	//Debug::logger().info("opencvrclient started {} {}", __LINE__, __FUNCTION__);

    QPixmap pixmap(":/logo/resources/log.png");
	QSplashScreen *splash = new QSplashScreen(pixmap);
	qRegisterMetaType<QVector<int> >("QVector<int>");
	qRegisterMetaType<std::string>("std::string");

	splash->setStyleSheet(QStringLiteral("color : white;"));    
	splash->show();
	
    pFactory = new ClientFactory(env);

	pFactory->Init();
	

#ifdef WIN32
	QFont font;
	font.setPointSize(10);
	font.setFamily(("Î¢ÈíÑÅºÚ"));
	font.setBold(false);

	a.setFont(font);
#else
	QFont font;
	font.setPointSize(10);
	font.setFamily(("WenQuanYi Zen Hei"));
	font.setBold(false);

	a.setFont(font);
#endif

	VidLanguage m_lang;
	pFactory->GetLang(m_lang);
	if (m_lang == VID_LANG_AUTO)
	{
		if (QLocale::system().name() == "zh_CN")
		{
			LoadLangZH(a);
		}
	}
	else if (m_lang == VID_ZH_CN)
	{
		LoadLangZH(a);
	}//else if add more language to here

	
	splash->showMessage(QObject::tr("Starting ..."));
	ve_sleep(3000);
	VTaskMgr *pVTaskMgr = new VTaskMgr();

	VSCMainWindows * w = new VSCMainWindows(*pFactory);

	//w.showMaximized();
	w->hide();
	//w.showFullScreen();
	splash->finish(w);
	/* Auto  */
	if (pFactory->GetAutoLogin() == false)
	{
		w->ShowLogin();
	}else
	{
		if (pFactory->GetAutoFullScreen() == false)
		{
			w->showMaximized();
		}else
		{
			w->showMinimized();
		}
	}

	VDC_DEBUG("Start successfully !\n");
	delete splash;

	/* Start Stor client to connect VidStor */
	pFactory->GetStorFactory().Start();

	return a.exec();
}
