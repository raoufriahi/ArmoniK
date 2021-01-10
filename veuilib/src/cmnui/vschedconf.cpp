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
#include "vschedconf.hpp"
#include "ui_vschedconf.h"
#include "recordwrapper.hpp"
#include <QRubberBand>
#include <QMouseEvent>
#include <QLabel>
#include <QPainter>

class VSchedConfPri
{
public:
	VSchedConfPri();
	~VSchedConfPri();
public:
};

VSchedConfPri::VSchedConfPri()
{
}
VSchedConfPri::~VSchedConfPri()
{
}

VSchedConf::VSchedConf(QWidget *parent, Qt::WindowFlags flags)
	: QWidget(parent, flags), p_ui(new Ui::VSchedConf), ui(*p_ui), 
	m_pPri(NULL)
{
	/* Width min 24 hour * 10,  heigh min 7 day * 30 */
	m_pPri = new VSchedConfPri();
	ui.setupUi(this);


	connect(ui.pbOff, SIGNAL(clicked()), this, SLOT(SlotRecordOff()));
	connect(ui.pbSched, SIGNAL(clicked()), this, SLOT(SlotRecordSchedule()));
	connect(ui.pbEvent, SIGNAL(clicked()), this, SLOT(SlotRecordAlarm()));
	connect(ui.pbWorkDay, SIGNAL(clicked()), this, SLOT(SlotRecordWorkDay()));
	connect(ui.pbAllDay, SIGNAL(clicked()), this, SLOT(SlotRecordAllDay()));
	connect(ui.pbClear, SIGNAL(clicked()), this, SLOT(SlotRecordClear()));
}

VSchedConf::~VSchedConf()
{
	if (m_pPri)
	{
		delete m_pPri;
		m_pPri = NULL;
	}
	
}

bool VSchedConf::GetCurrentSched(RecordSchedWeek &pSched)
{	
	ui.widget->GetCurrentSched(pSched);
	return true;
}

bool VSchedConf::SetCurrentSched(RecordSchedWeek &pSched)
{
	ui.widget->SetCurrentSched(pSched);
	return true;
}

void VSchedConf::SlotRecordOff()
{
	ui.widget->UpdateSchedType(R_OFF);
}
void VSchedConf::SlotRecordAlarm()
{
	ui.widget->UpdateSchedType(R_ALARM);
}
void VSchedConf::SlotRecordSchedule()
{
	ui.widget->UpdateSchedType(R_SCHED);
}

void VSchedConf::SlotRecordAllDay()
{
	ui.widget->SetSchedAllDay();
}
void VSchedConf::SlotRecordWorkDay()
{
	ui.widget->SetSchedWorkDay();
}

void VSchedConf::SlotRecordClear()
{
	ui.widget->SetSchedClear();
}





