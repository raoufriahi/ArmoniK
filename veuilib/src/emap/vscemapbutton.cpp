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
#include "vscemapbutton.h"
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

#include "vscvwidget.h"

//background-color:rgb(42, 43, 44)

VSCEmapButton::VSCEmapButton(QWidget *parent)
:QPushButton(parent)
{

	this->setAcceptDrops(true);
}

VSCEmapButton::~VSCEmapButton()
{

}

void VSCEmapButton::DragCommon()
{
	u32 nId = m_nId;
	VDC_DEBUG( "%s id %d\n",__FUNCTION__, nId);
	QMimeData *mimeData = new QMimeData();
	mimeData->setText(QString::number(nId));

	// Create drag
	QPixmap pixmap(":/action/resources/libuidome.png");
	QPainter painter(&pixmap);

	QDrag *drag = new QDrag(this);
	drag->setMimeData(mimeData);
	drag->setPixmap(pixmap);
	      drag->setHotSpot(QPoint(drag->pixmap().width()/2,
	             drag->pixmap().height()/2));
	drag->exec();
}

void VSCEmapButton::mousePressEvent(QMouseEvent *event)
{
	QWidget::mousePressEvent(event);
	Qt::MouseButtons mouseButtons = event->buttons();
	if( mouseButtons != Qt::LeftButton )
	{
		return;
	}

	return DragCommon();
}


