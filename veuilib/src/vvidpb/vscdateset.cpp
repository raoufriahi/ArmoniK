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
#include "vscdateset.h"
#include "debug.hpp"
#include <QLineEdit>
#include <QScrollBar>
#include <QPainter>
#include "server/factory.hpp"
#include <QMouseEvent>
#include "vvidpbcontrol.h"
#include "vscloading.hpp"
#include "client/storsyncinf.hpp"

#include <QDesktopWidget>
#include <QApplication>
#include <QMessageBox>
#include <QTableWidgetItem>


//XGuard guard(m_cMutex);

VSCDateSet::VSCDateSet(QWidget *parent)
	: QDialog(parent), m_pDelegate(NULL)
{
	ui.setupUi(this);
#ifdef WIN32
	setWindowFlags(Qt::FramelessWindowHint|Qt::NoDropShadowWindowHint| Qt::Tool);
#else
	setWindowFlags(Qt::FramelessWindowHint|Qt::NoDropShadowWindowHint| Qt::ToolTip);
#endif
	connect(ui.calendar, SIGNAL(clicked(const QDate &)), this, SLOT(SlotDataClicked(const QDate &)));
	connect(ui.calendar, SIGNAL(currentPageChanged(int, int)), this, SLOT(SlotPageChanged(int, int)));

	QDate currentDate = QDate::currentDate();
	ui.calendar->setSelectedDate(currentDate);
	//ui.calendar->setSelectionMode(QCalendarWidget::NoSelection);

	SlotPageChanged(ui.calendar->yearShown(), ui.calendar->monthShown());
	setStyleSheet("QTableView{selection-background-color: blue}");
}

void VSCDateSet::SlotDataClicked(const QDate & date)
{
	/* From 8 am to search */
	QTime dayStart(8, 0, 0);
	QDateTime timeStart(date, dayStart);

	u32 nTimeStart = timeStart.toTime_t();


	emit SignalSeekTime(nTimeStart);
}

void VSCDateSet::SlotPageChanged(int year, int month)
{
	QDate firstDay(year, month, 1);
	QTime dayStart(0, 0, 0);
	QTime dayEnd(23, 59, 59);
	HasRecordItemMap recMap;
	for (int i = 1; i <= firstDay.daysInMonth(); i ++)
	{
		VdbHasRecordItem item;
		QDate itemDay(year, month, i);
		QDateTime timeStart(itemDay, dayStart);
		QDateTime timeEnd(itemDay, dayEnd);
		item.id = i;
		item.start = timeStart.toTime_t();
		item.end = timeEnd.toTime_t();
		item.has = false;
		recMap[i] = item;
		QTextCharFormat format = ui.calendar->dateTextFormat(itemDay);
		QBrush greyBrush(QColor(230, 230, 230));
		format.setBackground(greyBrush);
		ui.calendar->setDateTextFormat(itemDay, format);
	}

	/* Search from server */
	XGuard guard(m_cMutex);
	if (m_pDelegate)
	{
		m_pDelegate->SearchHasRec(recMap);
	}
	for (int i = 1; i <= firstDay.daysInMonth(); i ++)
	{
		VdbHasRecordItem item;
		QDate itemDay(year, month, i);
		if (recMap[i].has == true)
		{
			QTextCharFormat format = ui.calendar->dateTextFormat(itemDay);
			format.setBackground(Qt::green);
			ui.calendar->setDateTextFormat(itemDay, format);
		}
		
	}

}

bool VSCDateSet::SetDelegate(VVidPBDelegate *pDelegate)
{
	XGuard guard(m_cMutex);
	m_pDelegate = pDelegate;
	SlotPageChanged(ui.calendar->yearShown(), ui.calendar->monthShown());
	return true;
}

bool VSCDateSet::ClearPlay()
{
	XGuard guard(m_cMutex);
	return true;
}

void VSCDateSet::mousePressEvent(QMouseEvent *event)  
{  
    if (event->button() == Qt::LeftButton) {  
        m_Drag = true;  
        m_DragPosition = event->globalPos() - this->pos();  
        event->accept();  
    }  
}  
  
void VSCDateSet::mouseMoveEvent(QMouseEvent *event)  
{  
    if (m_Drag && (event->buttons() && Qt::LeftButton)) {  
        move(event->globalPos() - m_DragPosition);  
        event->accept();  
    }  
}  
  
void VSCDateSet::mouseReleaseEvent(QMouseEvent *)  
{  
    m_Drag = false;  
} 



