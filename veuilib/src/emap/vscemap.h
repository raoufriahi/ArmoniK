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
#ifndef VSC_EMAP_H
#define VSC_EMAP_H

#include <QWidget>
#include <QTabWidget>
#include "utility.hpp"
#include "client/clientfactory.hpp"
#include "QEvent"
#include "ui_vscemap.h"
#include <QThread>
#include <QImage>
#include <QMenu>
#include <QTimer>
#include "vemap.hpp"
#include "vscemapcam.h"

using namespace std;


typedef std::map<astring, VSCEmapCam *> VSCCamMap;

class VSCEMap : public VEMap
{
	Q_OBJECT
public:
	VSCEMap(std::string strId, ClientFactory &pFactory, QTabWidget &pTabbed, QWidget *parent = 0);
	~VSCEMap();

public:
	void SetupConnections();
	
public:
	virtual void paintEvent(QPaintEvent* event);
	virtual void wheelEvent ( QWheelEvent * event );
	virtual void contextMenuEvent(QContextMenuEvent* e);
	void dragEnterEvent(QDragEnterEvent *event);
	void dragMoveEvent(QDragMoveEvent *event);
	void dropEvent(QDropEvent *event);
	virtual void resizeEvent(QResizeEvent* event);
	virtual void OnDeviceEvent(int deviceId, int type);

public:
	void addCamera(VidCameraId cId , s32 x, s32 y);
	void updateCameraPos(VidEmapCamera &cCam);
	void updateCameras();
	void Lock(){m_Lock.lock();}
	bool TryLock(){return m_Lock.try_lock();}
	void UnLock(){m_Lock.unlock();}

public slots:
	void LoadEmapAction();
	void Help();
	void ResizeEventTimer();
	void delCamera(std::string);
	void FloatingClicked();
	void TabbedClicked();

	void showDisplay1();
	void showDisplay2();
	void showDisplay3();
	void showDisplay4();
	void ShowDisplayClicked(int nId);

public:
       Ui::VSCEMap ui;    
private:
	QImage m_image;
	QRect m_drawingRect;
private:
	QAction *m_pLoadFile;
	QAction *m_pHelp;
	 QMenu * popupMenu;

	 VSCCamMap m_CamMap;
	 QTimer *m_TimerResize;
	 fast_mutex m_Lock;

	QTabWidget &m_pTabWidget;
	BOOL m_bFloated;
	QWidget *m_pParent;

	QAction *m_pFloating;
	QAction *m_pTabbed;

	QAction *m_pDisplay1;
	QAction *m_pDisplay2;
	QAction *m_pDisplay3;
	QAction *m_pDisplay4;
private:
	ClientFactory &m_pFactory;
	std::string m_strId;
};

#endif // VSC_EMAP_H
