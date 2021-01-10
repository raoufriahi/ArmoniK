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
#ifndef __VIDEO_EMAP_CAM_H__
#define __VIDEO_EMAP_CAM_H__

#include <QWidget>
#include <QMenu>
#include <QTimer>
#include "utility.hpp"
#include "client/clientfactory.hpp"

#define NOMINMAX
#define NOMINMAX 

#include "tinythread.h"
#include "fast_mutex.h"
#include "utility.hpp"
#include "ui_vscemapcam.h"

using  namespace tthread;


class VSCEmapCam : public QWidget
{
	Q_OBJECT

public:
	VSCEmapCam(VidEmapCamera &cCam, ClientFactory &pFactory, QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~VSCEmapCam();
	void contextMenuEvent(QContextMenuEvent* e);
	void mousePressEvent(QMouseEvent *event);
	void DragCommon();
	void OnDeviceEvent(int deviceId, int type);
	
public slots:
	void PlayVideo();
	void PlaybackVideo();
	void EmapDelete();
	void DragEmap();
	void showAlarm();
	
signals:
	void signalFocus();
	void EmapCamDel(std::string strCam);

public:
	Ui::VSCEmapCam ui;
private:
	bool m_isRunning;
	ClientFactory &m_pFactory;
	VidEmapCamera m_cCam;
	QAction *m_pDelete;
	QAction *m_pPlay;
	QAction *m_pPlayback;
	QTimer *m_TimerAlarm;
	bool m_alarm;
	int m_alarmCnt;
};

#endif // __VIDEO_EMAP_CAM_H__
