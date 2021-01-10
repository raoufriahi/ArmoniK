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
#include "config/vidclientsetting.h"
#include <QResizeEvent>
#include <QMimeData>
#include <QDrag>
#include <QAction>
#include <QDesktopWidget>
#include <QApplication>
#include <QDateTime>
#include <QTime>
#include <time.h>
#include <QStyleOption>
#include <QPainter>
#include <QScrollArea>
#include "Poco/UUIDGenerator.h"
#include "config/clientsetting/vscversion.h"
#include "config/clientsetting/vscuser.h"
#include "config/clientsetting/vsclicense.h"
#include "config/clientsetting/vsclanguage.h"

using namespace Poco;

VidClientSetting::VidClientSetting(ClientFactory &pFactory, QWidget *parent, Qt::WindowFlags flags)
	: m_pFactory(pFactory), QWidget(parent, flags)
{
	ui.setupUi(this);
	setAcceptDrops(true);
	setMouseTracking(true);
	
	ui.tabWidget->addTab(new VSCVersion(pFactory, this), tr("Version"));
	ui.tabWidget->addTab(new VSCUser(pFactory, this), tr("User"));
	ui.tabWidget->addTab(new VSCLicense(pFactory, this), tr("License"));
	ui.tabWidget->addTab(new VSCLanguage(pFactory, this), tr("Language"));


	//connect(this, SIGNAL(SignalSectionClicked(int, int)), ui.tableWidget, SIGNAL(cellClicked(int, int)));
}

VidClientSetting::~VidClientSetting()
{

}



