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
#ifndef __VIDEO_VIDCLIENT_SETTING_H__
#define __VIDEO_VIDCLIENT_SETTING_H__

#include <QWidget>
#include <QGridLayout>
#include <QScrollArea>
#include <QMimeData>
#include <QDrag>

#define NOMINMAX
#define NOMINMAX 

#include <QWidget>
#include "utility.hpp"
#include <QTimer>
#include <QMovie>
#include <QtWidgets/QMainWindow>
#include "client/clientfactory.hpp"
#include "ui_vidclientsetting.h"
#include <QTreeWidgetItem>
#include <QTreeWidget>
#include <QApplication>

class VidClientSetting : public QWidget
{
    Q_OBJECT

public:
	VidClientSetting(ClientFactory &pFactory, QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~VidClientSetting();

signals:
	//void SignalSectionClicked(int row, int column);

public slots:
	//void SlotSectionClicked(int row, int column);

public:
    Ui::VidClientSetting ui;
private:
	ClientFactory &m_pFactory;

};


#endif // __VIDEO_VIDCLIENT_SETTING_H__
