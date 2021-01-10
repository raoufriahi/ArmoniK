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
#include "vscvideoinfo.h"
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

//background-color:rgb(42, 43, 44)


VSCVideoInfo::VSCVideoInfo(QWidget *parent, Qt::WindowFlags flags)
    : QWidget(parent, flags)
{
	 ui.setupUi(this);
#if 0
	setAttribute(Qt::WA_TranslucentBackground);
	setStyleSheet("background:transparent;");
#ifdef WIN32
	setWindowFlags(Qt::FramelessWindowHint|Qt::NoDropShadowWindowHint| Qt::Tool);
#else
	setWindowFlags(Qt::FramelessWindowHint|Qt::NoDropShadowWindowHint| Qt::ToolTip);
	setFocusPolicy(Qt::NoFocus);
#endif	
#endif
}

VSCVideoInfo::~VSCVideoInfo()
{

}
bool VSCVideoInfo::event(QEvent *e)
{
    if(e->type() == QEvent::HoverEnter)
    {
	   VDC_DEBUG( "%s HoverEnter\n", __FUNCTION__);
#ifdef WIN32
        //emit signalFocus();
#endif
        //e->accept();
    }
    else if(e->type() == QEvent::HoverLeave)
    {
    	VDC_DEBUG( "%s HoverLeave\n", __FUNCTION__);
        //hide();
    }

    return QWidget::event(e);
}

