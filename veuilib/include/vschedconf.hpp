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
#ifndef __V_SCHED_CONF__
#define __V_SCHED_CONF__

#include <QWidget>
#define NOMINMAX
#define NOMINMAX 
#include "tinythread.h"
#include "fast_mutex.h"
#include "utility.hpp"
#include "debug.hpp"
#include "server/factory.hpp"
#include "client/clientfactory.hpp"
#include "client/storstream.hpp"
#include <QTimer>
#include <QMenu>
#include <QMimeData>
using  namespace tthread;

namespace Ui {
class VSchedConf;
}

class VSchedConfPri;

class VE_LIBRARY_API VSchedConf : public QWidget
{
	Q_OBJECT

public:
	VSchedConf(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~VSchedConf();
public:
	bool GetCurrentSched(RecordSchedWeek &pSched);
	bool SetCurrentSched(RecordSchedWeek &pSched);

public slots:
	void SlotRecordOff();
	void SlotRecordAlarm();
	void SlotRecordSchedule();
	void SlotRecordAllDay();
	void SlotRecordWorkDay();
	void SlotRecordClear();

public:
	Ui::VSchedConf *p_ui;
	Ui::VSchedConf &ui;
	
private:
	VSchedConfPri * m_pPri;
    
};


#endif // __V_SCHED_CONF__
