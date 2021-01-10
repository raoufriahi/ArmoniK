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
#ifndef VSCMAINWINDOWS_H
#define VSCMAINWINDOWS_H

#include <QtWidgets/QMainWindow>
#include "ui_vscmainwindows.h"
#include "client/clientfactory.hpp"
#include "live/vscvidlive.h"
#include "config/vscvidconfig.h"
#include "main/vscvidlist.h"
#include "searchpb/vscvidsearchpb.h"
#include "main/vscdashboard.h"
#include "main/vsceventconsole.h"

typedef enum
{
    VSC_VID_IDX_LIVE = 1,
    VSC_VID_IDX_CONF,
    VSC_VID_IDX_PB,
    VSC_VID_IDX_LAST
} VSCVidIdx;

class VSCToolBar;
class VSCMainWindows : public QMainWindow
{
	Q_OBJECT
public:
	VSCMainWindows(ClientFactory &pFactory, QWidget *parent = 0);
	~VSCMainWindows();
	
public:
	void SetupToolBar();
	void ViewHideFocus();
	void closeEvent(QCloseEvent *event); 
#ifdef __APPLE__
	void changeEvent(QEvent *event) 
	{ 
		if(event->type() == QEvent::WindowStateChange) 
		{ 
			//if(windowState() & Qt::WindowMaximized) 

			if(windowState() & Qt::WindowFullScreen)  
			{ 
				hide();
				this->setWindowState(Qt::WindowMaximized);
				show();
			}
		} 
		QWidget::changeEvent(event);  	
	} 
#endif

public slots:
	void MainCloseTab(int index);
	
public slots:
	void ShowVidLive();
	void ShowVidConf();
	void ShowVidPb();
	void ShowDashBoard();
	void ShowEventConsole();
	void about();
	void UserStatus();
	void SetFullScreen();
	void ShowAbout();

	/* Show Login */
	void ShowLogin();
	void ExitOpenCVR();
private:
	QAction *aboutAct;
private:
	VSCToolBar * m_pToolBar;
	VSCVidList * m_pVidList;
	QDockWidget *m_pDockDevicelist;
	VSCVidInf *m_pVidLive;
	VSCVidInf *m_pVidConf;
	VSCVidInf *m_pVidPb;
	VSCVidIdx m_VidIdx;

	QTabWidget * m_pMainArea;
	QWidget *m_pMainView;
	VSCDashBoard *m_pDashBoard;
	VSCEventConsole *m_pEventConsole;
	QToolBar *m_pQToolBar;
	Qt::WindowFlags m_flag;

private:
	Ui::VSCMainWindowsClass ui;
	
private:
	ClientFactory &m_pFactory;
};

#endif // VSCMAINWINDOWS_H
