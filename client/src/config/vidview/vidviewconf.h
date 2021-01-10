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
#ifndef __VIDEO_VIDVIEWCONF_H__
#define __VIDEO_VIDVIEWCONF_H__

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
#include "ui_vidviewconf.h"
#include <QTreeWidgetItem>
#include <QTreeWidget>
#include <QApplication>

class VidViewConfTableItem : public QTableWidgetItem
{
public:
	VidViewConfTableItem(VidView &pView, bool bNew)
		: m_sView(pView), m_bNew(bNew)
	{
	}
	~VidViewConfTableItem(){}
public:
	bool IsNew(){return m_bNew;}
	VidView GetView(){return m_sView;}
private:
	VidView m_sView;
	bool m_bNew;
};

class VidViewConf : public QWidget
{
    Q_OBJECT

public:
	VidViewConf(ClientFactory &pFactory, QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~VidViewConf();

public:
	void TreeWidgetUpdate();

public slots:
	void SlotDeleteView();

public:
    	Ui::VidViewConf ui;
private:
	ClientFactory &m_pFactory;

};


#endif // __VIDEO_VIDVIEWCONF_H__
