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
#ifndef VSC_STOR_LICENSE_H
#define VSC_STOR_LICENSE_H

#include <QWidget>
#include "ui_vscstorlicense.h"
#include "utility.hpp"
#include <QTimer>
#include "client/clientfactory.hpp"


class VSCStorLicense : public QWidget
{
    Q_OBJECT

public:
    VSCStorLicense(ClientFactory &pFactory, VidStor &stor, QWidget *parent = 0);
    ~VSCStorLicense(){}
public:
	void UpdateUI();

public slots:   
  void SlotImportLic();
  void SlotDefaultLic();
	
public:
    Ui::VSCStorLicense ui;
	
private:
	ClientFactory &m_pFactory;
	VidStor m_pStor;

    
};

#endif // VSC_LICENSE_H
