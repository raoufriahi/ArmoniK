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
#ifndef __VSC_DATE_SET_CONF_H__
#define __VSC_DATE_SET_CONF_H__

#include <QDialog>
#include "ui_vscdateset.h"
#include "utility.hpp"
#include <QTimer>
#include <QLabel>
#include <QDate>
#include <QDialog>
#include <QPoint>
#include <QMouseEvent>

#include "XSDK/XMutex.h"
#include "XSDK/XGuard.h"
#include "utility.hpp"
#include "utility/videotype.hpp"
#include "XSDK/TimeUtils.h"
#include "XSDK/XBlockingQueue.h"

using namespace UtilityLib;
using namespace XSDK;

class VVidPBDelegate;

class VSCDateSet : public QDialog
{
    Q_OBJECT

public:
	VSCDateSet(QWidget *parent = 0);
	~VSCDateSet(){}
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
public:
	bool SetDelegate(VVidPBDelegate *pDelegate);
	bool ClearPlay();
	
public slots: 
	void SlotDataClicked(const QDate & date);
	void SlotPageChanged(int year, int month);

signals:
    	void SignalSeekTime(unsigned int);

public:
	Ui::VSCDateSet ui;

private:
	QPoint m_DragPosition;
	bool m_Drag;
	VVidPBDelegate *m_pDelegate;
	XMutex m_cMutex;
};

#endif // __VSC_DATE_SET_CONF_H__
