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
#ifndef __VSC_VID_TREE_INF_H__
#define __VSC_VID_TREE_INF_H__

#include <QWidget>
#include "utility.hpp"
#include <QTimer>
#include <QMovie>
#include <QtWidgets/QMainWindow>
#include "client/clientfactory.hpp"
#include <QTreeWidgetItem>
#include <QTreeWidget>
#include <QApplication>
#include "common/vscviditeminf.h"



/*  QTreeWidget::itemClicked for get the item is checked or not */

class VSCVidTreeInf : public QTreeWidget
{
    Q_OBJECT
public:
    VSCVidTreeInf(ClientFactory &pFactory, QWidget *parent = 0);
    ~VSCVidTreeInf();
	
public:
	virtual void VidFilter(astring strFilter){}
	virtual void Init(){}
	virtual void VidSetCheckedChild(QTreeWidgetItem * item, bool bChecked){}
	virtual void VidGetSelectedItems(VidCameraIdMap &pMap){}
public slots:
	void SlotItemClicked(QTreeWidgetItem * item, int column);
	
protected:
	ClientFactory &m_pFactory;
	/* if the check has been called 
	    use for live and searchpb, search pb will call bsetchecked
	*/
	bool m_bSetChecked;
};

#endif
