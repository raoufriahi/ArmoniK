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
#include "vvidpb1.h"
#include "server/factory.hpp"
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
#include <QDialog>
#include "ui_vvidpb1.h"

//background-color:rgb(42, 43, 44)


VVidPB1::VVidPB1(ClientFactory &pFactory, QWidget *parent, Qt::WindowFlags flags)
    : QWidget(parent, flags), m_pFactory(pFactory), p_ui(new Ui::VVidPB1), ui(*p_ui)
{
	ui.setupUi(this);
	setWindowTitle(tr("Heimdall Playback"));
#ifdef WIN32
	setStyleSheet(QString::fromUtf8("font: 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
#endif

	QVBoxLayout* layout = new QVBoxLayout();
	m_pVideo = new VVidPBWidget(*(ui.pbControl), m_pFactory, 0, this->ui.video, true);
	connect(m_pVideo, SIGNAL(ShowFocusClicked(int)), this,
	                                    SLOT(ShowFocusClicked(int)));	
	m_pVideo->hide();

	layout->addWidget(m_pVideo);
	layout->setMargin(0);

	this->ui.video->setLayout(layout);
	m_pVideo->ShowVideoInfo(false);
	m_pVideo->show();
	setAttribute(Qt::WA_DeleteOnClose);

}

void VVidPB1::ShowFocusClicked(int nId)
{
	//m_pVideo->SetVideoFocus(TRUE);
}

bool VVidPB1::StartPlay(astring strStorId, astring strCamId, astring strCamName, 
	 u32 nPlaytime)
{
	m_strStor = strStorId;
	m_strCam = strCamId;
	m_strCamName = strCamName;
	setWindowTitle(tr(m_strCamName.c_str()));
	return m_pVideo->StartPlay(strStorId, strCamId, strCamName, nPlaytime);
}

bool VVidPB1::StopPlay()
{	
	m_pVideo->StopPlay();
	return true;
}

VVidPB1::~VVidPB1()
{
	if (m_pVideo)
	{
		StopPlay();
		delete m_pVideo;
		m_pVideo = NULL;
	}
}
