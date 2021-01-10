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
#ifndef VSC_VERSION_H
#define VSC_VERSION_H

#include <QWidget>
#include "ui_vscstorversion.h"
#include "utility.hpp"
#include "client/clientfactory.hpp"
#include <QTimer>


class VSCStorVersion : public QWidget
{
    Q_OBJECT

public:
    VSCStorVersion(ClientFactory &pFactory, VidStor &stor, QWidget *parent = 0);
    ~VSCStorVersion(){}

public slots:   
  void applyConfig();
	
public:
    Ui::VSCStorVersion ui;
	
private:
	ClientFactory &m_pFactory;
	VidStor m_pStor;

    
};

#endif // VSC_LICENSE_H
