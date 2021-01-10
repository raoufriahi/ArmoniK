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
#ifndef __VSC_LANGUAGE_H__
#define __VSC_LANGUAGE_H__

#include <QWidget>
#include "ui_vsclanguage.h"
#include "utility.hpp"
#include <QTimer>
#include "client/clientfactory.hpp"


class VSCLanguage : public QWidget
{
	Q_OBJECT

public:
	VSCLanguage(ClientFactory &pFactory, QWidget *parent = 0);
	~VSCLanguage(){}
public:
	void SetupValue();

public slots:   
	void applyConfig();
	
public:
	Ui::VSCLanguage ui;
	
private:
	ClientFactory &m_pFactory;
	VidLanguage m_nLang;
    
};

#endif // __VSC_USER_H__
