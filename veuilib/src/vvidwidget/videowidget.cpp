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
#include "videowidget.h"
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
#include <QEvent>

//background-color:rgb(42, 43, 44)


VideoWidget::VideoWidget(QWidget *parent, Qt::WindowFlags flags)
    : QWidget(parent, flags)
{
    setAcceptDrops(true);
    setMouseTracking(true);
	setStyleSheet(QStringLiteral("background-color:rgb(0, 0, 0)"));
	setAttribute(Qt::WA_Hover);
    //QPalette pal = palette();
    //pal.setColor(QPalette::Background, QColor(0x00,0x00,0x00,0xff));
    //setPalette(pal);
	//setAttribute(Qt::WA_TranslucentBackground);
	m_isRunning = false;
}

VideoWidget::~VideoWidget()
{

}

bool VideoWidget::event(QEvent * e)
{

    switch(e->type())
    {
    case QEvent::HoverEnter:		
		//VDC_DEBUG( "%s HoverEnter %p\n",__FUNCTION__, this); 
		emit videoHoverEnter(static_cast<QHoverEvent*>(e));
		return true;
        break;
    case QEvent::HoverLeave:
		//VDC_DEBUG( "%s HoverLeave %p\n",__FUNCTION__, this);  
		emit videoHoverLeave(static_cast<QHoverEvent*>(e));
		return true;
		break;
    case QEvent::HoverMove:
		//VDC_DEBUG( "%s HoverMove %p\n",__FUNCTION__, this);
		emit videoHoverMove(static_cast<QHoverEvent*>(e));
        	return true;
        	break;
    default:
        break;
    }
    return QWidget::event(e);

}

void VideoWidget::mouseMoveEvent(QMouseEvent *e)
{
	//VDC_DEBUG( "%s mouseMoveEvent %p\n",__FUNCTION__, this);

	QWidget::mouseMoveEvent(e);
	emit videoMouseMove(e);
}
#if 1
void VideoWidget::paintEvent(QPaintEvent *)
{
	// if (m_isRunning == false)
	 {
		 QStyleOption opt;
		 opt.init(this);
		 QPainter p(this);
		 style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
	 }
}
#endif

void VideoWidget::resizeEvent( QResizeEvent * event )
{    
	//VDC_DEBUG( "%s Resize\n",__FUNCTION__);   
	QWidget::resizeEvent(event);    
	emit videoResize();
}

