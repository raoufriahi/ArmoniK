/*
 * Copyright (c) 2016-2017 Heimdall
 *
 * The computer program contained herein contains proprietary
 * information which is the property of Heimdall.
 * The program may be used and/or copied only with the written
 * permission of Heimdall or in accordance with the
 * terms and conditions stipulated in the agreement/contract under
 * which the programs have been supplied.
 */
#ifndef __V_MOT_TABLE__
#define __V_MOT_TABLE__

#include <QWidget>
#define NOMINMAX
#define NOMINMAX 
#include "tinythread.h"
#include "fast_mutex.h"
#include "utility.hpp"
#include "debug.hpp"
#include <QTimer>
#include <QMenu>
#include <QMimeData>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "recordwrapper.hpp"
#include "vsmotalgo.hpp"
using  namespace tthread;

class VMotTablePri;

class VMotTable : public QWidget
{
	Q_OBJECT

public:
	VMotTable(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~VMotTable();
	void paintEvent(QPaintEvent *e);
	void mousePressEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);

public:
	bool GetCurrentMot(VVidMotReg &pMot);
	bool SetCurrentMot(VVidMotReg &pMot);
	/* Set current  */
	bool UpdateMotType(bool type);
	bool SetMotAll();
	bool SetMotClear();
	bool UpdateImage(astring strUrl);


public slots:
	void SlotMotOff();
	void SlotMotOn();
	void SlotNetowkManagerFinished(QNetworkReply *reply);
	
private:
	VMotTablePri * m_pPri;
	bool m_bPressed;
    
};


#endif // __V_MOT_TABLE__
