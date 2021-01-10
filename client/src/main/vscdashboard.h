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
#ifndef __VSC_DASH_BOARD_H__
#define __VSC_DASH_BOARD_H__

#include <QWidget>
#include <QGridLayout>
#include <QScrollArea>
#include <QMimeData>
#include <QDrag>

#define NOMINMAX
#define NOMINMAX 

#include "tinythread.h"
#include "fast_mutex.h"
#include "utility.hpp"
#include "ui_vscdashboard.h"
#include "vtaskmgr.hpp"

using  namespace tthread;

typedef std::map<std::string, QWidget *> VTaskItemWidgetList;

class VSCDashBoard : public QWidget
{
    Q_OBJECT

public:
	VSCDashBoard(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~VSCDashBoard();

public slots:
	void UpdateTaskList();
public:
    	Ui::VSCDashBoard ui;
private:
	VTaskItemList m_pTaskList;
	VTaskItemWidgetList m_pTaskWidget;

};


#endif // __VIDEO_PANEL_H__
