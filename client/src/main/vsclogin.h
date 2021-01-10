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
#ifndef __VSC_LOGIN_H__
#define __VSC_LOGIN_H__

#include <QDialog>
#include "ui_vsclogin.h"
#include "utility.hpp"
#include <QTimer>
#include <QPoint>
#include <QMouseEvent>

class VSCLogin : public QDialog
{
    Q_OBJECT

public:
    VSCLogin(QWidget *parent = 0);
    ~VSCLogin(){}
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
public:
	BOOL GetIsLogin()
	{
		return m_isLogin;
	}
	BOOL SetDefault()
	{
		m_isLogin = FALSE;
		return TRUE;
	}
	BOOL GetUserPasswd(astring &strUser, astring &strPasswd);

public slots: 
	void LoginClicked();
	void ExitClicked();

public:
	Ui::VSCLogin ui;
private:
	QPoint m_DragPosition;
	bool m_Drag;

private:
	BOOL m_isLogin;
	
};

#endif // __VSC_LOGIN_H__
