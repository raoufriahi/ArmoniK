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
#ifndef VSCDEVICE_TREE_H
#define VSCDEVICE_TREE_H

#include <QWidget>
#include <QTreeWidget>
#include <QAction>
#include <QMenu>
#include <QContextMenuEvent>

#include "server/factory.hpp"
#include "vscviewtree.h"


class VSCDeviceTree : public QTreeWidget
{
    Q_OBJECT
public:
    VSCDeviceTree(QWidget *parent = 0);
public:
	void mousePressEvent(QMouseEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);
	void contextMenuEvent(QContextMenuEvent * event);
	void createActions();
	void SetupConnections();

public slots:
	void CameraAddClick();
	void CameraEditClick();
	void CameraDeleteClick();
	void CameraRecordClick();
	void CameraStopRecordClick();
	void CameraHdfsRecordClick();
	void CameraStopHdfsRecordClick();

	void VIPCAddClick();
	void VIPCEditClick();
	void VIPCDeleteClick();
	void ViewDeleteClick();

	void VGroupAddClick();
	void VGroupEditClick();
	void VGroupDeleteClick();
	void VGroupMapClick();
	void StartRecordAllClick();
	void StopRecordAllClick();
	void StartHdfsRecordAllClick();
	void StopHdfsRecordAllClick();

	/* VMS */
	void VMSDeleteClick();
	void VMSRefreshClick();

signals:
	void CameraAddClicked();
	void CameraEditClicked(int nId);
	void CameraDeleteClicked(int nId);
	void CameraDoubleClicked(int nId);


	void VIPCAddClicked();
	void VIPCEditClicked(int nId);
	void VIPCDeleteClicked(int nId);

	void DiskEditClicked();
	
	void ViewDeleteClicked(int nId);

	void VGroupAddClicked();
	void VGroupEditClicked(int nId);
	void VGroupDeleteClicked(int nId);
	void VGroupMapClicked();

	void StartRecordAllClicked();
	void StopRecordAllClicked();
	void StartHdfsRecordAllClicked();
	void StopHdfsRecordAllClicked();

	/* VMS */
	void VMSDeleteClicked(int nId);

private:
	QAction *pActDeleteCamera;
	QAction *pActAddCamera;
	QAction *pActEditCamera;
	QAction *pActRecord;
	QAction *pActStopRecord;
	QAction *pActHdfsRecord;
	QAction *pActStopHdfsRecord;

	QAction *pActRecordAll;
	QAction *pActStopRecordAll;
	QAction *pActHdfsRecordAll;
	QAction *pActStopHdfsRecordAll;

	/* Group */
	QAction *pActDeleteCamGroup;
	QAction *pActAddCamGroup;
	QAction *pActEditCamGroup;

	QAction *pActCamGroupMap;

	QAction *pActDiskEdit;
	
	QAction *pActEditVIPC;
	QAction *pActAddVIPC;
	QAction *pActDelVIPC;

	/* View */
	QAction *pActDelView;

	/* VMS */
	QAction *pActDelVMS;
	QAction *pActRefreshVMS;

	QMenu *pPopMenu;
	
    //~VSCDeviceTree();

};

#endif // VSCDEVICE_TREE_H
