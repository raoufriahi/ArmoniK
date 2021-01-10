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
#ifndef VSC_TOOL_BAR_H
#define VSC_TOOL_BAR_H

#include <QWidget>
#include "ui_vsctoolbar.h"
#include "utility.hpp"
#include <QTimer>
#include <QMovie>
#include "vsccalculate.hpp"

class VSCToolBar : public QWidget
{
    Q_OBJECT

public:
    VSCToolBar(QWidget *parent = 0);
    ~VSCToolBar(){}
	
public slots:
	void showcurrentTime();
	void showAlarm();
	void NewAlarm();
	void showProcessingTimer();
	void showProcessing(bool bEnable);
public:
    Ui::VSCToolBar ui;
	
private:
	QTimer *m_Timer;
	QTimer *m_TimerAlarm;
	QTimer *m_TimerPrcessing;
	bool m_alarm;
	int m_alarmCnt;
	VSCCalculate * m_cpu;
	QMovie * m_movie;
    
};

#endif // VSC_TOOL_BAR_H
