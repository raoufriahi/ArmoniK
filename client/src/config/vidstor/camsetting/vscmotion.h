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
#ifndef __VSC_MOTION_H__
#define __VSC_MOTION_H__

#include <QWidget>
#include "ui_vscmotion.h"
#include "utility.hpp"
#include <QTimer>
#include "client/clientfactory.hpp"


class VSCMotion : public QWidget
{
	Q_OBJECT

public:
	VSCMotion(ClientFactory &pFactory, VidStor &stor, astring strCam, QWidget *parent = 0);
	~VSCMotion(){}

public slots:   
	void applyConfig();
	void MotConf();
public:
	Ui::VSCMotion ui;
	
private:
	ClientFactory &m_pFactory;
	VidStor m_pStor;
	astring m_strCam;

    
};

#endif // __VSC_MOTION_H__
