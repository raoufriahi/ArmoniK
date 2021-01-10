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
#ifndef __VSC_ABOUT_H__
#define __VSC_ABOUT_H__

#include <QDialog>
#include "ui_vscabout.h"
#include "utility.hpp"
#include <QTimer>
#include <QPoint>
#include <QMouseEvent>

class VSCAbout : public QDialog
{
    Q_OBJECT

public:
    VSCAbout(QWidget *parent = 0);
    ~VSCAbout(){}
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);

public slots: 
	void CancelClicked();

public:
	Ui::VSCAbout ui;
private:
	QPoint m_DragPosition;
	bool m_Drag;
	
};

#endif // __VSC_USER_STATUS_H__
