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
#ifndef __VIDEO_INFO_H
#define __VIDEO_INFO_H

#include <QWidget>

#define NOMINMAX
#define NOMINMAX 
//#include <inttypes.h>


#include "tinythread.h"
#include "fast_mutex.h"
#include "utility.hpp"
#include "ui_vscvideoinfo.h"
//#include "mpipeline.hpp"
using  namespace tthread;


class VSCVideoInfo : public QWidget
{
    Q_OBJECT

public:
    VSCVideoInfo(QWidget *parent = 0, Qt::WindowFlags flags = 0);
    ~VSCVideoInfo();
	bool event(QEvent *e);
signals:
	void signalFocus();

public:
    Ui::VSCVideoInfo ui;
private:
	bool m_isRunning;
};

#endif // __VIDEO_INFO_H
