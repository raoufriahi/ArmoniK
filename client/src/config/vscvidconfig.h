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
#ifndef __VSC_VID_CONF_H__
#define __VSC_VID_CONF_H__

#include "common/vscvidinf.h"
#include "config/vscvidtreeconf.h"

class VSCVidConf : public VSCVidInf
{
    Q_OBJECT
public:
    VSCVidConf(ClientFactory &pFactory, QTabWidget &pTab, QMainWindow *parent);
    ~VSCVidConf();
	
public:
	virtual void VidShow();
	virtual void VidHide();
public:
	virtual VSCVidTreeInf *GetConfTree();

public slots:
	void SlotVidStorAdd();
	void SlotVidCamAdd(std::string strStorId);
	void SlotVidClientConf();
	void SlotVidStorConf(std::string strStorId);
	void SlotVidDiskConf(std::string strStorId);
	void SlotVidCamConf(std::string strStor, std::string strCam);
	void SlotVidViewConf();
	void SlotVidEmapConf();

private:
	VSCVidTreeInf *m_pConfTree;

};

#endif
	
