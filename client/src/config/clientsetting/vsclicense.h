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
#ifndef VSC_LICENSE_H
#define VSC_LICENSE_H

#include <QWidget>
#include "ui_vsclicense.h"
#include "utility.hpp"
#include <QTimer>
#include "client/clientfactory.hpp"


class VSCLicense : public QWidget
{
    Q_OBJECT

public:
    VSCLicense(ClientFactory &pFactory, QWidget *parent = 0);
    ~VSCLicense(){}
public:
	void UpdateUI();

public slots:   
  void SlotImportLic();
  void SlotDefaultLic();
	
public:
    Ui::VSCLicense ui;
	
private:
	ClientFactory &m_pFactory;

    
};

#endif // VSC_LICENSE_H
