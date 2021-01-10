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
#include "config/vidstor/vidstorsetting.h"
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
#include "config/vidstor/storsetting/vscstorversion.h"
#include "config/vidstor/storsetting/vscstoruser.h"
#include "config/vidstor/storsetting/vscstorlicense.h"

using namespace Poco;

VidStorSetting::VidStorSetting(VidStor pStor, ClientFactory &pFactory, QWidget *parent, Qt::WindowFlags flags)
	: m_pStor(pStor), m_pFactory(pFactory), QWidget(parent, flags)
{
	ui.setupUi(this);
	setAcceptDrops(true);
	setMouseTracking(true);
	
	ui.tabWidget->addTab(new VSCStorVersion(pFactory, m_pStor, this), tr("Version"));
	ui.tabWidget->addTab(new VSCStorUser(pFactory, m_pStor, this), tr("User"));
	ui.tabWidget->addTab(new VSCStorLicense(pFactory, m_pStor, this), tr("License"));

	//connect(this, SIGNAL(SignalSectionClicked(int, int)), ui.tableWidget, SIGNAL(cellClicked(int, int)));
	ui.storName->setText(m_pStor.strname().c_str());
}

VidStorSetting::~VidStorSetting()
{

}



