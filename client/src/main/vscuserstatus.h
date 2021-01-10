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
#ifndef __VSC_USER_STATUS_H__
#define __VSC_USER_STATUS_H__

#include <QDialog>
#include "ui_vscuserstatus.h"
#include "utility.hpp"
#include <QTimer>
#include <QPoint>
#include <QMouseEvent>

class VSCUserStatus : public QDialog
{
    Q_OBJECT

public:
    VSCUserStatus(QWidget *parent = 0);
    ~VSCUserStatus(){}
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
public:
	BOOL IsQuit()
	{
		return m_quit;
	}

public slots: 
	void QuitClicked();
	void CancelClicked();

public:
	Ui::VSCUserStatus ui;
private:
	QPoint m_DragPosition;
	bool m_Drag;

private:
	BOOL m_quit;
	
};

#endif // __VSC_USER_STATUS_H__
