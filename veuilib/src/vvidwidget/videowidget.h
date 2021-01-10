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
#ifndef __VIDEO_WIDGET_H
#define __VIDEO_WIDGET_H

#include <QWidget>

#define NOMINMAX
#define NOMINMAX 
//#include <inttypes.h>


#include "tinythread.h"
#include "fast_mutex.h"
#include "utility.hpp"
//#include "mpipeline.hpp"
using  namespace tthread;


class VideoWidget : public QWidget
{
    Q_OBJECT

public:
    VideoWidget(QWidget *parent = 0, Qt::WindowFlags flags = 0);
    ~VideoWidget();


public:
	void mouseMoveEvent(QMouseEvent *e); 
	bool event(QEvent * e);

	void resizeEvent( QResizeEvent * event );
	void paintEvent(QPaintEvent *);
	void setRunning(bool isRunning)
	{
		m_isRunning = isRunning;
		return;
	}
	
	
signals:
	void videoMouseMove(QMouseEvent *e);
	void videoHoverMove(QHoverEvent *e);
	void videoHoverEnter(QHoverEvent *e);
	void videoHoverLeave(QHoverEvent *e);
    void videoResize();

private:
	bool m_isRunning;
};

#endif // __VIDEO_WIDGET_H
