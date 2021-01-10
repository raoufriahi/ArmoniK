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
#ifndef VELIBDEMO_H
#define VELIBDEMO_H

#include <QtWidgets/QMainWindow>
#include <QThread>
#include "ui_velibdemo.h"
#include "vplay/vplay.hpp"

class velibdemo : public QMainWindow, public QThread
{
	Q_OBJECT

public:
	velibdemo(QWidget *parent = 0);
	~velibdemo();
public:
	void run();

private:
	Ui::velibdemoClass ui;

	VPlay m_play;
};

#endif // VELIBDEMO_H
