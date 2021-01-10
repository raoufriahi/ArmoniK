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
#ifndef __VSC_VID_TREE_CAM_H__
#define __VSC_VID_TREE_CAM_H__

#include "common/vscvidtreeinf.h"
#include "vscvwidget.h"

class VSCVidTreeCam : public VSCVidTreeInf
{
    Q_OBJECT
public:
    VSCVidTreeCam(ClientFactory &pFactory, QWidget *parent = 0);
    ~VSCVidTreeCam();

signals:
	void CameraSelected(std::string strStor, std::string strCam, std::string strCamName);
public:
	void mousePressEvent(QMouseEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	
public:
	virtual void VidFilter(astring strFilter);
	virtual void Init();
	virtual void VidSetCheckedChild(QTreeWidgetItem * item, bool bChecked);
	virtual void VidGetSelectedItems(VidCameraIdMap &pMap);
public:
	/* Take care all the child state change */
	//static bool CallChange(void* pParam, StorFactoryChangeData data);
	bool CallChange(StorFactoryChangeData data);
public slots:
	void SlotCallChange(int type, std::string strId, std::string strCam);
	
public:
	void TreeUpdate();
	void StorAdd(VidCameraId cId);
	void StorDel(VidCameraId cId);
	void StorOnline(VidCameraId cId, bool bOnline);
private:
	QTreeWidgetItem * m_pRoot;
	bool m_bInit;
};

#endif
