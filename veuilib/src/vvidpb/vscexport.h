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
#ifndef __VSC_USER_EXPORT_H__
#define __VSC_USER_EXPORT_H__

#include <QDialog>
#include "ui_vscexport.h"
#include "utility.hpp"
#include <QTimer>
#include <QPoint>
#include <QMouseEvent>
#include "vtaskmgr.hpp"

class VVidPBDelegate;

class VSCExport : public QDialog
{
	Q_OBJECT

public:
	VSCExport(astring strStor, astring strId, astring strCamName, u32 startTime, QWidget *parent = 0);
	~VSCExport(){}
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);

public:	
	bool SetStartTime(u32 startTime);
	bool SetDelegate(VVidPBDelegate *pDelegate);


public slots: 
	void StartClicked();
	void CancelClicked();
	void StartDateTimeChanged(const QDateTime &dateTime);

public:
	Ui::VSCExport ui;
	
private:
	QPoint m_DragPosition;
	bool m_Drag;
	u32 m_startTime;

	QDateTime m_endMax;
	QDateTime m_endMin;
	astring m_strStor;
	astring m_strId;
	astring m_strCamName;
	VVidPBDelegate *m_pDelegate;
	XMutex m_cMutex;

};

#endif // __VSC_USER_STATUS_H__
