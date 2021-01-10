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
#ifndef __VSC_CAM_REC_H__
#define __VSC_CAM_REC_H__

#include <QWidget>
#include "ui_vsccamrec.h"
#include "utility.hpp"
#include <QTimer>
#include "client/clientfactory.hpp"


class VSCCamRec : public QWidget
{
	Q_OBJECT

public:
	VSCCamRec(ClientFactory &pFactory, VidStor &stor, astring strCam, QWidget *parent = 0);
	~VSCCamRec(){}

public slots:   
	void applyConfig();
	
public:
	Ui::VSCCamRec ui;
	
private:
	ClientFactory &m_pFactory;
	VidStor m_pStor;
	astring m_strCam;

    
};

#endif // __VSC_CAM_REC_H__
