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
#ifndef __VSC_VID_TREE_VIEW_H__
#define __VSC_VID_TREE_VIEW_H__

#include <QWidget>
#include "utility.hpp"
#include <QTimer>
#include <QMovie>
#include <QtWidgets/QMainWindow>
#include "client/clientfactory.hpp"
#include <QTreeWidgetItem>
#include <QTreeWidget>
#include "common/vscvidtreeinf.h"
#include "vscvwidget.h"

class VSCVidTreeView : public VSCVidTreeInf
{
    Q_OBJECT
public:
    VSCVidTreeView(ClientFactory &pFactory, QWidget *parent = 0);
    ~VSCVidTreeView();

signals:
	void ViewSelected(std::string strView);
public:
	void mousePressEvent(QMouseEvent *event);	
	void mouseDoubleClickEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);

	
public:
	virtual void VidFilter(astring strFilter);
	virtual void Init();
public:
	/* Take care all the vid Stor state change */
	bool CallChange(ClientFactoryChangeData data);
public slots:
	void SlotCallChange(int type, std::string strId);
public:
	void TreeUpdate();
private:
	QTreeWidgetItem * m_pRoot;
	bool m_bInit;
};

#endif
