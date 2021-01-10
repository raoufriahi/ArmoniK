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
#ifndef VSCDEVICE_IPC_H
#define VSCDEVICE_IPC_H

#include <QWidget>
#include <QTimer>
#include <QTreeWidgetItem>
#include "server/factory.hpp"

class VSCDeviceIPC : public QTreeWidgetItem
{
public:
	void mousePressEvent(QMouseEvent *event);

public:
	VSCDeviceIPC(QTreeWidgetItem *parent);
    ~VSCDeviceIPC();
	
public:
	u32 GetDeviceId()
	{
	    return 1;
	}
	
public:
    void UpdateOnline(BOOL bonline);
    void UpdateRecord(BOOL bRecording);	


private:
	//DeviceParam m_Param;

};

class VSCDeviceIPCOAPI : public QTreeWidgetItem
{
public:
	void mousePressEvent(QMouseEvent *event);

public:
	VSCDeviceIPCOAPI(QTreeWidgetItem *parent);
    ~VSCDeviceIPCOAPI();
	
public:
	u32 GetDeviceId()
	{
		return 1;
	}
	u32 GetVMSId()
	{
		return 1;
	}
	
	
public:
    void UpdateOnline(BOOL bonline);
    void UpdateRecord(BOOL bRecording);	



};


class VSCDeviceIPCGroup : public QTreeWidgetItem
{
public:
	void mousePressEvent(QMouseEvent *event);

public:
	VSCDeviceIPCGroup(QTreeWidgetItem *parent);
    ~VSCDeviceIPCGroup();
	
public:
	u32 GetDeviceId()
	{
	    return 1;
	}
	
public:
    void UpdateOnline(BOOL bonline);
    void UpdateRecord(BOOL bRecording);	

};

#endif // VSCDEVICE_IPC_H
