/*
 * Copyright (c) 2016-2017 Heimdall
 *
 * The computer program contained herein contains proprietary
 * information which is the property of Heimdall.
 * The program may be used and/or copied only with the written
 * permission of Heimdall or in accordance with the
 * terms and conditions stipulated in the agreement/contract under
 * which the programs have been supplied.
 */
#ifndef __V_SCHED_TABLE__
#define __V_SCHED_TABLE__

#include <QWidget>
#define NOMINMAX
#define NOMINMAX 
#include "tinythread.h"
#include "fast_mutex.h"
#include "utility.hpp"
#include "debug.hpp"
#include <QTimer>
#include <QMenu>
#include <QMimeData>
#include "recordwrapper.hpp"
using  namespace tthread;

class VSchedTablePri;

class VSchedTable : public QWidget
{
	Q_OBJECT

public:
	VSchedTable(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~VSchedTable();
	void paintEvent(QPaintEvent *e);
	void mousePressEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);

public:
	bool GetCurrentSched(RecordSchedWeek &pSched);
	bool SetCurrentSched(RecordSchedWeek &pSched);
	/* Set current  */
	bool UpdateSchedType(RecordingType type);
	bool SetSchedAllDay();
	bool SetSchedWorkDay();
	bool SetSchedClear();


public slots:
	void SlotRecordOff();
	void SlotRecordAlarm();
	void SlotRecordSchedule();
	
private:
	VSchedTablePri * m_pPri;
	bool m_bPressed;
    
};


#endif // __V_SCHED_TABLE__
