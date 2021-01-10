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
#ifndef __VSC_VID_LIST_FRAME_H__
#define __VSC_VID_LIST_FRAME_H__

#include <QFrame>
#include <QMouseEvent>
#include "common/vscvidinf.h"

class VSCVidListFrame : public QFrame
{
    Q_OBJECT
public:
    VSCVidListFrame(QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~VSCVidListFrame();
signals:
	void SignalClicked();	
public:
	void mousePressEvent(QMouseEvent *event);
private:
};

#endif
	
