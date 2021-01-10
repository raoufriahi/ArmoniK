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
#ifndef VSC_CAM_INFO_H
#define VSC_CAM_INFO_H

#include <QWidget>
#include "ui_vsccaminfo.h"
#include "utility.hpp"
#include "client/clientfactory.hpp"
#include <QTimer>


class VSCCamInfo : public QWidget
{
    Q_OBJECT

public:
    VSCCamInfo(ClientFactory &pFactory, VidStor &stor, astring strCam, QWidget *parent = 0);
    ~VSCCamInfo(){}
public:
	astring GetCameraName()
	{
		return m_strCamName;
	}

public slots:   
  void applyConfig();
	
public:
    Ui::VSCCamInfo ui;
	
private:
	ClientFactory &m_pFactory;
	VidStor m_pStor;
	astring m_strCam;
	astring m_strCamName;

    
};

#endif // VSC_LICENSE_H
