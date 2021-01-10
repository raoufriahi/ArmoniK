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
#include "ui_vscversion.h"
#include "utility.hpp"
#include "client/clientfactory.hpp"
#include <QTimer>


class VSCVersion : public QWidget
{
    Q_OBJECT

public:
    VSCVersion(ClientFactory &pFactory, QWidget *parent = 0);
    ~VSCVersion(){}

public slots:   
  void applyConfig();
	
public:
    Ui::VSCVersion ui;
	
private:
	ClientFactory &m_pFactory;

    
};

#endif // VSC_LICENSE_H
