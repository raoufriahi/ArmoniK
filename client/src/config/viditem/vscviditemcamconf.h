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
#ifndef __VSC_VID_ITEM_CAM_CONF_H__
#define __VSC_VID_ITEM_CAM_CONF_H__

#include "common/vscviditeminf.h"

class VSCVidItemCamConf : public QObject,public VSCVidItemInf
{
Q_OBJECT
public:
    VSCVidItemCamConf(VidStor cStor, VidCamera cCam, ClientFactory &pFactory, QTreeWidgetItem *parent);
    ~VSCVidItemCamConf();
public:
	virtual astring GetId(){return m_cCam.strid();}
	virtual void VidFilter(astring strFilter);
	astring GetStorId(){return m_cStor.strid();}
	
private:
	VidCamera m_cCam;
	VidStor m_cStor;

};

#endif /* __VSC_VID_ITEM_CAM_CONF_H__ */
