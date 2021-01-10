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
#ifndef __PTZ_CONTROL_H
#define __PTZ_CONTROL_H

#include <QWidget>

#define NOMINMAX
#define NOMINMAX 
//#include <inttypes.h>

#include <QDialog>
#include "tinythread.h"
#include "fast_mutex.h"
#include "utility.hpp"
#include "ui_vscptzcontrol.h"
#include "client/clientfactory.hpp"
using  namespace tthread;


class VSCPTZControl : public QDialog
{
    Q_OBJECT

public:
	VSCPTZControl(ClientFactory &pFactory, QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~VSCPTZControl();
	bool event(QEvent *e);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);

	void SetControlParam(astring strStorId, astring strCamId);

private slots:
	void PTZAction(FPtzAction action);
	void PTZStop();
	void PTZZoomin();
	void PTZZoomout();
	void PTZLeft();
	void PTZUp();
	void PTZDown();
	void PTZRight();

public:
	Ui::VSCPTZControl ui;
private:
	QPoint m_DragPosition;
	bool m_Drag;

	ClientFactory &m_pFactory;
	astring m_strStorId;
	astring m_strCamId;
};

#endif // __VIDEO_CONTROL_H
